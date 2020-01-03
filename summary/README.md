# Course summary
Brief summary of the entire course program.

# Table of contents
1. [Piattaforme di esecuzione](#1-piattaforme-di-esecuzione)
2. [Il modello di esecuzione](#2-il-modello-di-esecuzione)
3. [Allocazione della memoria](#3-allocazione-della-memoria)
4. Introduzione al C++
5. Gestione delle eccezioni
6. [Composizione di oggetti](#6-composizione-di-oggetti)
7. [Ereditarietà e polimorfismo](#7-ereditarietà-e-polimorfismo)
8. [Funzioni e operatori](#8-funzioni-e-operatori)
9. [Programmazione generica](#9-programmazione-generica)
10. [Librerie C++](#10-librerie-c)
11. [Librerie](#11-librerie)
12. [Programmazione concorrente](#12-programmazione-concorrente)
13. [Programmazione concorrente in C++](#13-programmazione-concorrente-in-c)
14. [Threads](#14-threads)
15. [Condition variables](#15-condition-variables)
16. [Interprocess communication on Windows](#16-interprocess-communication-on-windows)
17. [Interprocess communication on Linux](#17-interprocess-communication-on-linux)
18. [C# introduction](#18-c-introduction)
19. [WPF](#19-wpf)
20. [Multithreading in .NET e C#](#20-multithreading-in-net-e-c)

# 1. Piattaforme di esecuzione

### Interfacciarsi con il sistema operativo
- **API** (system calls)
    - Riguardano il codice sorgente
    - Completamente indipendenti dalla macchina fisica
- **ABI**: Application Binary Interface
    - Definisce che formato l'oggetto compilato deve assumere affinché sia eseguibile sulla macchina fisica

### Gestione degli errori
- Linux: `errno`
- Windows: `GetLastError()`

# 2. Il modello di esecuzione

#### Creazione di un processo
1. Creazione dello [**spazio di indizzamento**](#spazio-di-indirizzamento)
    - **File eseguibile** (ELF/PE2): code, data, stack and heap
1. Caricamento dell'eseguibile in memoria
1. Caricamento delle librerie (DLLs)
1. Avvio dell'esecuzione
    1. Inizializzazione:
        - Stack
        - Registri
        - Exception data structures
    1. Costruttori degli oggetti globali
    1. `main(int argc, char** argv)`
    1. `exit()`, che rilascia:
        - Spazio di indirizzamento
        - Tutte le risorse allocate

# 3. Allocazione della memoria

##### MMU page flags
R, W, X, [COW](https://en.wikipedia.org/wiki/Copy-on-write)

##### Tipi di variabili 
- Globali
    - Lifecycle: prima dell'esecuzione del programma
    - **Indirizzo fisso e assoluto**
- Locali
    - Lifecycle: blocco
    - **Indirizzo relativo** all'inizio dello stack
- Dinamiche (heap)
    - Lifecycle: da `malloc()` a `free()`

### Spazio di indirizzamento
- OS kernel space
- Variabili d'ambiente
- Codice eseguibile (R/X)
- Stack (R/W)
    - Variabili locali
    - Valori di ritorno
    - Parametri
- Costanti (R)
- Variabili globali (R/W)
- Heap (variabili dinamiche)

# 6. Composizione di oggetti

### La regola dei tre
Devono esistere tutte le tre seguenti funzioni, o nessuna:
- Costruttore di copia
- Operatore di assegnazione
- Distruttore

Altrimenti il compilatore fornira' la propria implementazione.

### Costruttore di copia
```cpp
class CBuffer {
    int size;
    char* ptr;
public:
    CBuffer(const CBuffer& source) {
        this->size = source.size;
        this->ptr = new char[size];
        memcpy(this->ptr, source.ptr, size); 
    }
};
```

### Operatore di assegnazione
```cpp
CBuffer& operator=(const CBuffer& source) {
    
    // Altrimenti verrebbe eliminato l'oggetto corrente, e ci sarebbero errori durante la copia.
    if (this != &source) {
        
        // Distruzione dell'oggetto corrente per evitare un memory leakage
        delete[] this->ptr;
        this->ptr = NULL; // evita dangling pointer se `new char[size]` fallisce successivamente.
                          // `delete NULL;` e' lecito.
                          // In caso in cui `new char[size]` fallisca, il compiler rilascerebbe
                          // un'altra volta la memoria se `this->prt != NULL`
                          // (perche' fa roll-back, non si e' in un blocco try...).

        // Copia di tutti i fields, come nel costruttore di copia
        this->size = source.size;
        this->ptr = new char[size]; // puo' lanciare un eccezione se non c'e' memoria
        memcpy(this->ptr, source.ptr, size); 
    }
    
    return *this; // allows '= chains'
}
```

### Costruttore di movimento
```cpp
class CBuffer {
    int size;
    char* ptr;
public:
    // Il compilatore decide quando usarlo, preferendolo a quello di copia
    CBuffer(CBuffer&& source) { // '&&' = RValue reference
        this->size = source.size;
        this->ptr = source.ptr; // vantaggio
        source.ptr = NULL; // l'originale viene modificato!
    }
};
```

### Assegnazione per movimento
```cpp
class CBuffer {
    int size;
    char* ptr;
public:
    CBuffer& operator=(CBuffer&& source) {
        if (this != &source) {
            delete[] this->ptr; // svuotamento dell'oggetto destinazione
            this->size = source.size;
            this->ptr = source.ptr;
            // source.size = 0; // inutile, `source` verra' distrutto 
            source.ptr = NULL; // la sua successiva distruzione non fara' danno
        }
        return *this;
    }
};
```

### Paradigma *Copy&Swap*
Per evitare errori dovuti alla dimenticanza di `this->prt = NULL;`
```cpp
class intArray {

    std::size_t mSize;
    int* mArray;

public:

    // Costruttore
    intArray(std::size_t size = 0): mSize(size), mArray(mSize ? new int[mSize] : NULL) {}

    // Costruttore di copia
    intArray(const intArray& that): mSize(that.mSize), mArray(mSize ? new int[mSize] : NULL) {
        std::copy(that.mArray, that.mArray + mSize, mArray);
    }

    // Distruttore
    ~intArray() { delete[] mArray; }

    // Funzione esterna ad una classe, ma che puo' accedere ai suoi `private` fields 
    friend void swap(intArray& a, intArray& b) {
        std::swap(a.mSize, b.mSize);
        std::swap(a.mArray, b.mArray);
    }

    // Operatore di assegnazione secondo il paradigma Copy&Swap
    intArray& operator=(intArray that) { // per valore, copiato o mosso a seconda
                                         // del contesto
        swap(*this, that); // usa la versione custom di questa classe
        return *this;
    }

    // Costruttore di movimento secondo il paradigma Copy&Swap
    intArray(intArray&& that): mSize(0), mArray(NULL) {
        swap(*this, that);
    }
};
```

# 7. Ereditarietà e polimorfismo

### Metodi virtuali e virtuali astratti
```cpp
    class File {
    public:
        virtual int v() { return 1; }
        virtual int va() = 0; // virtual astratto
    }
```

#### V-Table
Le classi con almeno un metodo `virtual` hanno un puntatore aggiuntivo che punta ad una **V-Table** che ha tante entry quanti sono i metodi `virtual` nella classe stessa.

- Le sue entries contengono gli indirizzi delle implementazioni concrete dei metodi polimorfici
- Costruita a compile-time
    - Statica
    - Condivisa da tutte le istanze concrete della stessa classe
- Ereditarieta'
    - Ereditarieta' semplice: una V-Table, prima le entry della super-class, poi quelle della classe stessa
    - Erediterieta' multipla: tante V-Table quante sono le super-classi piu' una per la classe stessa

### Type casting

#### `static_cast<T>`
- Done at compile time
    - Efficient
- Viene cambiata il puntatore alla V-Table
- Usato per conversioni lungo l'asse ereditario
    - Upcasting sempre garantito, il downcasting no
- Il compiler deve avere delle regole di conversione tra i tipi
    - Importatore:
    - Esportatore:

#### `dynamic_cast<T>`
- RTTI: Run-Time Type Information
    - Ogni oggetto ha un hash per il compatibility check
- Downcasting sicuro, perche' se non e' possibile ritorna:
    - `0` su puntatore invalido
    - Runtime exception su riferimento incompatibile

#### `reinterpret_cast<T>`
- Il cast del C
- Interpreta la sequenza di bit come un altro tipo
    - Nessun check
- Adatto al low level programming

#### `const_cast<T>`
- Rimuove il descrittore `const` da una variabile

# 8. Funzioni e operatori

### Puntatori a funzione
- Example
    ```cpp
    int (*var)(int, double);
    var = f;
    var = &f; // it's the same, more coherent
    ```

### Oggetti funzionali o *funtori*
- Example
    ```cpp
    class FC {
    public:
        int operator()(int v) {
            return v*2;
        }
    };

    FC fc;
    int i = fc(5);  // object behaving like a function
                    // i = 10
    ```

### Lambda functions
- Example
    ```cpp
    int main() {
        std::vector<int> v;
        // ...
        std::for_each(v.begin(), v.end(), 
            [](int i) -> void { std::cout << i << " "; }
        );
    }
    ```
- Usage
    - Algoritmi generici
    - Programmazione funzionale: programmazione in cui funzioni ritornano altre funzioni
    - Programmazione concorrente

# 9. Programmazione generica

# 10. Librerie C++

# 11. Librerie

# 12. Programmazione concorrente

# 13. Programmazione concorrente in C++

# 14. Threads

# 15. Condition variables

# 16. Interprocess communication on Windows

# 17. Interprocess communication on Linux

# 18. C# introduction

# 19. WPF

# 20. Multithreading in .NET e C#

