# System calls, programming, embedded systems, etc.
Course held by Prof. Malnati

#### Useful links
- [OneNote notes](https://1drv.ms/u/s!AkzjmTTkND1lhPVBx3kR1-NqPIPxTw)
- [O.S. books](http://www.os-book.com/)

# Table of contents
1. [Piattaforme di esecuzione](#1-piattaforme-di-esecuzione)
    + [Interfacciarsi con il sistema operativo](#interfacciarsi-con-il-sistema-operativo)
    + [Convenzioni](#convenzioni)
    + [Gestione degli errori](#gestione-degli-errori)
      - [Windows](#windows)
      - [Linux](#linux)
    + [Gestione dei caratteri](#gestione-dei-caratteri)
      - [Windows](#windows)
        * [Single character](#single-character)
        * [Strings](#strings)
      - [Linux](#linux-1)
    + [POSIX](#posix)
2. [Il modello di esecuzione](#2-il-modello-di-esecuzione)
      - [Innalzamento di privilegio](#innalzamento-di-privilegio)
      - [Creazione di un processo](#creazione-di-un-processo)
        * [GCC/Linux start function](#gcclinux-start-function)
        * [Windows start function](#windows-start-function)
      - [Compiling with `-nostdlib`](#compiling-with--nostdlib)
3. [Allocazione della memoria](#3-allocazione-della-memoria)
        * [MMU flags](#mmu-flags)
          + [Accesso a locazioni non mappate](#accesso-a-locazioni-non-mappate)
        * [Ciclo di vita delle variabili](#ciclo-di-vita-delle-variabili)
    + [Spazio di indirizzamento](#spazio-di-indirizzamento)
    + [Allocazione statica e dinamica](#allocazione-statica-e-dinamica)
    + [Puntatori e il loro utilizzo](#puntatori-e-il-loro-utilizzo)
    + [Allocazione in Linux](#allocazione-in-linux)
    + [Allocazione in Windows](#allocazione-in-windows)
4. [Introduzione al C++](#4-introduzione-al-c)
5. [Gestione delle eccezioni](#5-gestione-delle-eccezioni)
6. [Composizione di oggetti](#6-composizione-di-oggetti)
    + [Costruttore di copia](#costruttore-di-copia)
    + [Operatore di assegnazione](#operatore-di-assegnazione)
    + [La regola dei tre](#la-regola-dei-tre)
    + [Movimento](#movimento)
        * [Costruttore di movimento](#costruttore-di-movimento)
        * [Assegnazione per movimento](#assegnazione-per-movimento)
    + [Paradigma *Copy&Swap*](#paradigma-copyswap)
      - [Why does it work?](#why-does-it-work)
    + [`std::move()`](#stdmove)
7. [Ereditarietà e polimorfismo](#7-ereditarietà-e-polimorfismo)
    + [Simple example](#simple-example)
      - [Ereditarieta' multipla](#ereditarieta-multipla)
    + [Polimorfismo](#polimorfismo)
    + [Type casting](#type-casting)
        * [Example for this chapter](#example-for-this-chapter)
      - [`static_cast<T>`](#static_castt)
      - [`dynamic_cast<T>`](#dynamic_castt)
      - [`reinterpret_cast<T>`](#reinterpret_castt)
      - [`const_cast<T>`](#const_castt)
8. [Funzioni e operatori](#8-funzioni-e-operatori)
    + [Puntatori a funzione](#puntatori-a-funzione)
    + [Oggetti funzionali o *funtori*](#oggetti-funzionali-o-funtori)
    + [Lambda functions](#lambda-functions)
    + [Operator overloading](#operator-overloading)
9. [Programmazione generica](#9-programmazione-generica)
    + [Generic functions](#generic-functions)
    + [Generic classes (templates)](#generic-classes-templates)
    + [Specializzare un template](#specializzare-un-template)
    + [Drawbacks](#drawbacks)
    + [Smart Pointer template](#smart-pointer-template)
      - [Different strategies](#different-strategies)
      - [C++ Smart Pointers](#c-smart-pointers)
10. [Librerie C++](#10-librerie-c)
      - [I/O](#i-o)
      - [Standard Template Library](#standard-template-library)
    + [Il processo di compilazione](#il-processo-di-compilazione)
11. [Librerie](#11-librerie)
    + [Tassonomia delle librerie](#tassonomia-delle-librerie)
        - [Librerie statiche](#librerie-statiche)
        - [Librerie dinamiche](#librerie-dinamiche)
12. [Programmazione concorrente](#12-programmazione-concorrente)
    + [Thread e memoria](#thread-e-memoria)
    + [Sincronizzazione in Windows](#sincronizzazione-in-windows)
    + [Sincronizzazione in Linux](#sincronizzazione-in-linux)

# 1. Piattaforme di esecuzione

### Interfacciarsi con il sistema operativo
- **API** (system calls)
    - Riguardano il codice sorgente
    - Completamente indipendenti dalla macchina fisica
- **ABI**: Application Binary Interface
    - Definisce che formato l'oggetto compilato deve assumere affinché sia eseguibile sulla macchina fisica
    - Dipendono dalla macchina fisica (x86, arm, etc.)
    - Esempi
        - Convenzioni di chiamata e passaggio dei parametri
        - Uso dei registri della CPU
        - Innalzamento di privilegio e invocazione del O.S.
    - Supportata dalla **toolchain**
        - Compiler (da codice sorgente `.c` a macchina `.o`)
        - Linker ()
        - Debugger
        - Profiler
        - Inspector

### Convenzioni
- Strutture dati che descrivono lo sttao del sistema
- Accessibili tramite le API attraverso riferimenti opachi
    - `HANDLE` in Windows
    - File descriptors in Linux

### Gestione degli errori

#### Windows
- `BOOL`: `typedef` of `int`
- `HANDLE`: `typedef` of `void*`, `< 0` means failure
- `PVOID`: `typedef` of `void*`, `NULL` means failure
    - `malloc()` returns a `PVOID` because it doesn't know the content type
- `LONG`/`DWORD` (unsigned 32 bit): `typedef` of `unsigned long`
    - `GetLastError()`
        - VisualStudio's `ErrorLookup` for its meaning or `FormatMessage()`

#### Linux
- `errno`

### Gestione dei caratteri

#### Windows
##### Single character
`#ifdef UNICODE` switches between:
- `char`: 8 bit ASCII
- `wchar_t`: Unicode `UTF-16`

`TCHAR` is generic.
```c
BOOL CreateDirectoryA(LPCSTR lpPathName, ...);
BOOL CreateDirectoryW(LPCWSTR lpPathName, ...);

#ifdef UNICODE
#define CreateDirectory CreateDirectoryW 
#else
#define CreateDirectory CreateDirectoryA
#endif // !UNICODE
```
##### Strings
- `LPSTR`: `char` sequence with final `\0`
- `LPWSTR`: `wchar_t` sequence
- `LPTSTR`: `TCHAR` sequence (generic)

#### Linux
Non-ASCII as `UTF-8`

### POSIX
*Portable Operating System Interface*, low-level API standard and set of shell commands.
Windows and macOS are fully POSIX compliant, Windows and Android only partially.

# 2. Il modello di esecuzione

#### Innalzamento di privilegio
- Softare interrupt (trap)
- `SYSENTER`/`SYSEXIT`, on modern processors

#### Creazione di un processo
- Creazione dello **spazio di indizzamento**
    - Insieme di locazioni di memoria accessibili tramite indirizzo virtuale
    - La MMU traduce da virtuale a fisico (e lancia page faults)
    - Nuova entry nella `GDT` (Global Descriptor Table)
    - **File eseguibile**: code, data, stack and heap
        - Formato ELF su Linux e PE2 su Windows
        - Caricato dal loader
    - All'inizio è vuoto, e ci saranno page faults
- Caricamento dell'eseguibile in memoria
- Caricamento delle librerie
    - DLLs (la shared già presenti non vengono ricaricate)
- Avvio dell'esecuzione
    - `main(int argc, char** argv)` fino a `exit()`

##### GCC/Linux start function
```c
int __libc_start_main(
    int (*main)(int, char**, char**), // code
    int argc,
    char** ubp_av,
    void (*init)(void), // inizializza variabili globali
    void (*fini)(void), // distrugge variabili globali in ordine inverso
    void (*rtld_fini)(void), // 'run-time loader finish', il loader de-mappa
    void* stack_end // quanto stack ha a disposizione
    // verra' eseguita una exit() alla fine
);
```

##### Windows start function
Funziona allo stesso modo. Ne esistono 4:

|              | Unicode | Mode    |
|--------------|---------|---------|
| `main()`     | no      | console |
| `wmain()`    | yes     | console |
| `WinMain()`  | no      | gui     |
| `wWinMain()` | yes     | gui     |

#### Compiling with `-nostdlib`
Take a look at the [example](ex1-nostdlib) ([guide](https://blogs.oracle.com/linux/hello-from-a-libc-free-world-part-1-v2)).

# 3. Allocazione della memoria

##### MMU flags
La MMU ha, per ogni pagina, un bit di:
- Read
- Write
- Execute (fetchable)
- [COW](https://en.wikipedia.org/wiki/Copy-on-write), copy-on-write, a.k.a. implicit sharing
    - When a resource is shared and the CPU wants to modify it, it has to copy it first
    - The copy operation is hence deferred to the first write. It's done with an interrupt
    - Utilizzato dalla `fork()` quando viene creato lo spazio di indirizzamento del child process

###### Accesso a locazioni non mappate
- Segmentation fault on Linux
- Access violation on Windows

##### Ciclo di vita delle variabili 
- Globali: prima dell'esecuzione del programma 
- Locali: alla chiamata di una funzione 
- Dinamiche (heap): da malloc() a free() 
    - Ciclo di vita indipendente

### Spazio di indirizzamento
- Diverse aree per garantire diversi tipi di accesso (R/W/X)
- Aree
    - Area protetta (ad uso dell'O.S.)
    - Variabili d'ambiente
    - Codice eseguibile
    - Stack
        - Variabili locali
            - Ciclo di vita della funzione/blocco
            - Indirizzo relativo all'inizio dello stack
            - Valore iniziale casuale
        - Valori di ritorno
        - Parametri
    - Variabili globali
        - Indirizzo fisso e assoluto determinato dal compilatore e linker
            - Perche' devono essere sempre accessibili
        - Inizializzate o non
    - Heap (variabili dinamiche)
        - `malloc()` e `free()`, ciclo di vita indipendente
        - Variabili dinamiche accessibili soo tramite puntatori
        - Inizializzate o no
- Lo starting point nello spazio virtuale e' random, cosi' da rendere la vita difficile ai virus
- [Page file](https://www.howtogeek.com/126430/htg-explains-what-is-the-windows-page-file-and-should-you-disable-it/)
    - When your RAM becomes full, Windows moves some of the data from your RAM back to your hard drive, placing it in the page file
    - This file is a form of virtual memory.

### Allocazione statica e dinamica
- Linux
    - `void* malloc(size_t s)`
    - `void* malloc(int n, size_t s)`, per `n` volte
    - `void* realloc(void* p, size_t s)`, estende/riduce `p` con la nuova dimensione `s`
- Windows
    - *Global heap* e *local heap*
- C++
    - Costruttore `new`/`delete`, che inizializza sempre
    - `new[]`/`delete[]` per array
    - Rilasciare con la funzione sbagliata puo' corrompere le strutture dati degli allocatori, con conseguenze imprevedibili

### Puntatori e il loro utilizzo
- Appartenente ad altri oggetti
    ```c
    int a = 10;
    int* pA = &a;
    ```
- Allocato allo scopo
    ```c
    int* pB = new int(24);
    ```
- Valori invalidi
    - `0`
    - Macro `NULL`, ovvero `((void*)0)
    - C++11 keyword `nullptr`
- Aritmetica dei puntatori
    ```c
    char* ptr = "String";
    for (int i = 0; *(ptr + 1) != 0; ++i) { /* ... */ }
    ```
- Rischi
    - *Dangling pointer*: puntatore che fa riferimento ad un area di memoria non piu' valida
    - *Memory leakage*
    - *Wild pointer*: puntatore non inizializzato

### Allocazione in Linux
- `malloc()` effettua una syscall dell'O.S., i cui dettagli dipendono dall'implementazione
    - `malloc()` per dati > 128KB chiama la `mmap()` che crea un blocco separato in un segmento inutilizzato
- Ogni PCB (*Process Control Block*) contiene una `mm_struct`(*memory descriptor*), che contiene
    - `start_brk`/`brk`: inizio/fine heap
        - `int brk(void* end_data_segment);` sposta il puntatore di fine heap
        - `void* sbrk(intptr_t increment);` puo' incrementare lo heap
    - `start_stack`

### Allocazione in Windows
- `malloc()` usa il *global heap*
- *Local heaps*
    - `HANDLE HeapCreate()`
    - `void* HeapAlloc(HANDLE heap, DWORD options, SIZE_T s )`
    - `BOOL HeapFree(Handle heap, DWORD options, void* ptr)`
    - `BOOL HeapDestroy(HANDLE heap)`
        - Le aree di memoria diventano inaccessibili

# 4. Introduzione al C++
- Gli `static` fields sono nella memoria globale
- Come evitare dangling pointers:
    ```cpp
    char* ptr = "String";
    // ...
    delete ptr;
    ptr = NULL; // <---
    ```
    - Array dinamici
        ```cpp
        ptr = new[count] Something();
        delete[] ptr;
        ptr = NULL;
        ```

# 5. Gestione delle eccezioni
- `#include<std::exception>`
    - `what()` contiene il messaggio
    - `#include<stdexcept>`
        - `std::logic_error`
            - `domain_error`, `invalid_argument`, `length_error`, `out_of_range`, etc.
        - `std:runtime_error`
            - `overflow_error`, `range_error`, `underflow_error`, etc.
            - `#include<system_error>`, solo in C++11
                - `system_error` contiene `code()` che restituisce l'equivalente di `errno`
    - `#include<new>` per mancanza di memoria
    - `#include<ios>` I/O errors
    - `#include<future>` esecuzione asincrona, C++11
    - `#include<typeinfo>` cast dinamici o RTTI (*RunTime Type Identification*)
- `catch( ... )` cattura tutte le eccezioni
- `finally` non esiste
    - Lo stack viene contratto al lancio di un'eccezione
    - Tutti i distruttori interessati vengono chiamati
        - Paradigma **RAII**: *Resource Acquisistion Is Initialization*, cioe' chiudere la risorsa nel distruttore
        - I distruttori non dovrebbero lanciare eccezioni

# 6. Composizione di oggetti

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
Per renderlo non copiabile, renderlo `private` e senza body.

### Operatore di assegnazione
Per oggetti gia' esistenti.
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
Per renderlo non assegnabile, renderlo `private` e senza body.

### La regola dei tre
Devono esistere tutte le tre seguenti funzioni, o nessuna:
- Costruttore di copia
- Operatore di assegnazione
- Distruttore

Altrimenti il compilatore fornira' la propria implementazione.

### Movimento
- "Svuotare" un oggetto che sta per essere distrutto del suo contenuto e "travasarlo" in un altro oggetto
- Candidati al movimento:
    - Variabili locali al termine del blocco in cui sono state definite
    - Risultati di espressioni temporanee
    - Oggetti anonimi costruiti a partire dal tipo per passarlo come parametro
        ```cpp
        function(std:string("ciao"));
        ```
    - **RVALUE**s (tutto cio' che non ha un nome e puo' comparire solo alla destra di `=`)
        - `int&&` e' un RVALUE
- **Il compiler sceglie** se usare il costruttore/assegnazione di movimento o di copia
    - Usa quello di movimento **se subito dopo viene chiamato il distruttore**
- I thread e `std::unique_ptr` non sono copiabili, ma solo movibili

##### Costruttore di movimento
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
- Compiler's pseudocode:
    ```cpp
    Obj_MoveConstructor(dst, src);
    Obj_Destructor(src)
    ```
- Esempio:
    ```cpp
    string f() {
        string x("...");
        string a(x); // x e' un LVALUE, percio' viene copiato
        string b(a + x); // (a + x) e' un RVALUE, viene spostato
        string c(funzioneCheRitornaString()); // Risultato mosso in c
        return c; // c viene mosso nel risultato
    }
    ```
- Il costruttore di copia non porta a nessun vantaggio se la classe contiene tutti tipi primitivi
    - Per i tipi primitivi, il costo della copia e' uguale al costo del movimento

##### Assegnazione per movimento
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
#### [Why does it work?](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)
> We first notice an important choice: the parameter argument is taken by-value.\
> While one could just as easily do the following (and indeed, many naive implementations of the idiom do):
```cpp
intArray& operator=(const intArray& that) {
    dumb_array temp(that); // inefficient!
    swap(*this, temp);

    return *this;
}
```
1. Passing by value [is faster](https://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/)
2. > If you're going to make a copy of something in a function, let the compiler do it in the parameter list
> Either way, this method of obtaining our resource is the key to eliminating code duplication: we get to use the code from the copy-constructor to make the copy, and never need to repeat any bit of it.

> Observe that upon entering the function that all the new data is already allocated, copied, and ready to be used. This is what gives us a strong exception guarantee for free: we won't even enter the function if construction of the copy fails, and it's therefore not possible to alter the state of `*this`.

> At this point we are home-free, because `swap` is non-throwing. We swap our current data (`this`) with the copied data (`that`), safely altering our state, and the old data gets put into the temporary. The old data is then released when the function returns. (Where upon the parameter's scope ends and its destructor is called.)

### `std::move()`
- Forza il passaggio per movimento
    - Fa uno static cast ad un RVALUE
- Esempio
    ```cpp
    std::string str("hello");
    std::vector<std::string> v;
    v.push_back(str);   // `str` e' copiato
                        // v = ["hello"], str = "hello"
    v.push_back(std::move(str));    // `str` e' spostato
                                    // v = ["hello", "hello"], str = ""
    ```

# 7. Ereditarietà e polimorfismo

### Simple example
```cpp
class File {
    int fileDescriptor;
public:
    uint_8 read();
    size_t readBlock(uint_8 *ptr, size_t offset, size_t count);
    int close();
}

class TextFile : public File {
    CharCodec codec;
public:
    wchar_t readChar();
    size_t readCharBlock(wchar_t *ptr, size_t offset, size_t count);
};
```
`TextFile` puo' chiamare `File::read()`.

#### Ereditarieta' multipla
- E' possibile
- Per questo non esiste la `super` keyword
    - La sub-class deve utilizzare lo scope-operator
        ```cpp
        class TextFile : public File {
            // ...
        public:
            TextFile(): File() { File::read(); }
        }
        ```

### Polimorfismo
- Example
    ```cpp
    File* f = new TextFile(); // ok
    TextFile* tf = new File(); // error
    ```
- Difference with Java:

    |   | Java | C++ |
    |---|---|---|
    | Polymorfic | - | `virtual` |
    | Non-overridable | `final` | - |
    
    - I metodi poliformici introducono un overhead
- Overridden methods:
    ```cpp
    class File {
    public:
        int m() { return 1; }
        virtual int mv() { return 1; } // solo le `virtual` sono polimorfiche
        virtual int ma() = 0; // classe non implementabile (polimorfico puro)
    }

    class TextFile : public File {
    public:
        int m() { return 2; }
        int mv() { return 2; }
    }

    int main(int argc, char** argv) {
        File* prt = new TextFile(); // se File::ma() non esistesse
        std::cout << ptr->m(); // 1 (2 in Java)
        std::cout << ptr->mv(); // 2
        // ...
    }
    ```
- Distruttori virtuali
    - Se una classe ha una funzione `virtual`, dovrebbe avere anche il distruttore `virtual`, altrimenti potrebbe essere chiamato un distruttore sbagliato
- Per far si' che il compiler distingua quali implementazioni usare quando viene utilizzato un puntatore alla super-classe, le classi con almeno un metodo `virtual` hanno un puntatore aggiuntivo che punta ad una **V-Table** che ha tante entry quanti sono i metodi `virtual` nella classe stessa
    - Le entry nella V-Table contengono gli indirizzi delle implementazioni concrete dei metodi polimorfici
    - La V-Table e' statica, costruita a compile-time, condivisa da tutte le istanze concrete della stessa classe
    - Ereditarieta'
        - Ereditarieta' semplice: una V-Table, prima le entry della super-class, poi quelle della classe stessa
        - Erediterieta' multipla: tante V-Table quante sono le super-classi piu' una per la classe stessa

### Type casting

##### Example for this chapter
```cpp
class Base1;
class Base2;
class Derivata : public Base1, public Base2 {};
//...
Derivata* d = new Derivata();
Base1* b1;
Base2* b2;
```

#### `static_cast<T>`
- Esempio
    ```cpp
    b1 = static_cast<Base1*>(d);
    b2 = static_cast<Base2*>(d);
    ```
- Il compiler deve avere delle regole di conversione tra i tipi.
    - Importatore:
        ```cpp
        /* not explicit */ Base1(Derivata d) { /* ... */ }
        ```
        - `explicit` avverte il compiler di non utilizzare la funzione per effettuare `static_cast<T>`
    - Esportatore:
        ```cpp
        Base1 Derivata::operator_cast() { /* ... */} // FIXME
        ```
- Viene cambiata il puntatore alla V-Table
    - Durante un downcast, la dimensione della V-Table diminuisce (il valore del puntatore aumenta)
- L'upcasting e' sempre garantito, il downcasting no

#### `dynamic_cast<T>`
- RTTI: Run-Time Type Identification
    - Ogni oggetto ha un hash per il compatibility check
- Downcasting sicuro, perche' se non e' possibile ritorna:
    - `0` su puntatore invalido
    - Runtime exception su riferimento incompatibile
- Esempio di downcasting
    ```cpp
    Base1* b1 = new Base1();
    Derivata* d = dynamic_cast<Derivata*>(b1); // d = NULL
    ```
    - `static_cast<Derivata*>(b1)` avrebbe eseguito, creando problemi

#### `reinterpret_cast<T>`
- Il cast del C
- Interpreta la sequenza di bit come un altro tipo
    - Nessun check
- Adatto al low level programming
    - Chiamate dall'O.S.
    - Byte ricevuti da altro hardware

#### `const_cast<T>`
- Rimuove il descrittore `const` da una variabile

# 8. Funzioni e operatori

### Puntatori a funzione
- Syntax
    ```cpp
    <return_value> (*var) (<arguments>)
    ```
- Example
    ```cpp
    int f(int i, double d) { /* ... */ }
    int (*var)(int, double);
    var = f;
    var = &f; // it's the same, more coherent
    var(10, 3.14); // calls f()
    ```
    - Default arguments
        ```cpp
        int g(int, int = 4);
        int (*p)(int) = &g; // ERROR: missing 2nd default argument
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
- Usage
    - Non-deterministic functions (they have access to `this`)
    - Example
        ```cpp
        class Accumulator {
            int tot;
        public:
            Accumulator():tot(0) {}
            int operator()(int v) {
                tot += v;
                return v;
            }
            int tot() { return tot; }
        };

        void main () {
            Accumulator a;
            for (int i = 0; i < 10; i++)
                a(i);
            std::cout << "Total: " << a.tot() << std::endl; // 45
        }
        ```
- Oggetti funzionali e puntatori a funzione
    ```cpp
    template <typename F>
    void some_function(F& f) {
        f(); // puntatore a funzione o oggetto funzionale
    }
    ```

### Lambda functions
- Syntax
    ```cpp
    [<captured_vars>](<params>) -> <return_type> { <function_body> }
    ```
- Usato nella libreria standard: lambda function
    ```cpp
    int main() {
        std::vector<int> v;
        // ...
        std::for_each(v.begin(), v.end(), 
            [](int i) { std::cout << i << " "; }
        );
    }
    ```
    - Il tipo di ritorno non sempre e' omettibile
        ```cpp
        [](int num, int den) -> double {
            if (den == 0)
                return std::NaN;
            else
                return (double)num/den;
        }
        ```
- Captured vars
    - Variabili locali il cui valore o riferimento si vuole rendere disponibili nella funzione
    - Le context variables devono essere dichiarate nell'outer block in cui la lamba function e' definita
    - `[x, y]` per valore
    - `[&x, &y]` per riferimento
    - `[&]` cattura tutto per riferimento
- Usage
    - Algoritmi generici
    - Programmazione funzionale: programmazione in cui funzioni ritornano altre funzioni
    - Programmazione concorrente

### Operator overloading
- Non si possono definire nuovi operatori
- Non si possono cambiare le precedenze

# 9. Programmazione generica
- In Java, a runtime e' tutto `Object`, ma il down-casting tempo durante l'esecuzione
    - A runtime e' possibile non rispettare i vincoli
        ```java
        MyList<Integer> myList = new MyList<>(); // `MyList<Object>`
        myList.append("String"); // OK
        ```
- In C++, i templates sono **compilati** in due fasi:
    1. Coerenza ai vincoli
    2. Istanziazione dei template: generazione del codice a seconda degli utilizzi
- I templates quindi **sono completamente risolti in fase di compilazione**
- Applicabile a funzioni o a classi

### Generic functions
- Example
    ```cpp
    template <class T>
    const T& max(const T& t1, const T& t2) {
        return (t1 < t2 ? t2 : t1);
    }
    ```
    - `T` deve avere `operator+()`
    - `T` deve aver un costruttore di copia, per derivare una variabile temporanea a partire da un dato costante
    - Forzare la scelta
        ```cpp
        max<double>(2, 3.14);
        ```

### Generic classes (templates)
- Example
    ```cpp
    template <class T>
    class Accum {
        T total;
    public:
        Accum(T start): total(start) {}
        T operator+(const T& t) { // T must override operator+()
            return total = total + t;
        }
        T value() {
            return total; // T must have a copy constructor, ritorno per copia
        }
    };

    Accum<std::string> sa("");
    Accum<int> ia(0);
    ```
- Anche con valori fissati
    ```cpp
    template <class T, int size> // valore costante, non una espressione
    ```

### Specializzare un template
- Alcuni tipi potrebbero essere non utilizzabili all'interno di un template
    - Potrebbero non avere l'implementazione di un operatore usato nella definizione del template stesso.\
    E.g., se nella definizione del template, due oggetti generici si sommano, il tipo dovra' implementare `operator+()`.
- Alternative:
    - Cambiare il template: raro, dati dalle librerie
    - Modificare la classe
    - Specializzare il template
        - Example
            ```cpp
            class Person { // incapsula una stringa
                std::string name;
            public:
                Person(std::string n): name(n) {}
                // Does not override operator+().
                // How could we use the `Accum`(ulator) template
                // with `Person`(s)?
            };

            // Template specializzato per la classe `Person`
            template<> class Accum<Person> {
                int total;
            public:
                Accum(int start = 0): total(start) {}
                int operator+(const Person&) { return ++total; }
            };
            ```

### Drawbacks
- Vanno verificate le assunzioni sui tipi effettivamente usati
    - Esempio: templates della standard library assumono che l'oggetto sia:
        1. Copiabile (costruttore di copia)
        1. Assegnabile (operatore di assegnazione)
    - Altrimenti: compile error
        - Error message non preciso, perche' C++ prova prima ad effettuare conversioni

### Smart Pointer template
- Overload degli operatori classici: `*` e `->`
    - `!` per la validita': `while(!ptr)`
    - Esempio stupido
        ```cpp
        class int_ptr {
            int* ptr;
            int_ptr(const int_ptr&); // no copy constructor
            int_ptr& operator=(const int_ptr&); // no assignment
        public:
            explicit int_ptr(int* p): ptr(p) {}
            ~int_ptr() { delete ptr; }  // Assunzione, il `p` sovrastante
                                        // deve essere allocato sullo heap,
                                        // altrimenti questo non ha senso.
            int& operator*() { return *ptr; } // Errore se `p` fosse NULL
        };

        int i = 1;
        int_ptr ip(&i); // il distruttore chiamera' `delete` su un elemento
                        // dello stack => errore
        ```
- Vantaggi sui puntatori classici:
    - Garanzia di inizializzazione (costruttore) e rilascio (distruttore)
    - Conteggio dei riferimenti
    - Accesso controllato
- Generic smart pointer
    ```cpp
    template <class T>
    class smart_ptr {
        T* ptr;
        smart_ptr(const smart_ptr<T>&);
        smart_ptr<t>& operator=(const smart_ptr<T>&);
    public:
        explicit smart_ptr(T* p = 0): ptr(p) {}
        ~smart_ptr() { delete ptr; }
        T& operator*() { return *ptr; }
        T* operator->() { return ptr; }
    };

    void example() {
        smart_ptr<MyClass> p(new MyClass());    // RAII: il ciclo di vita della
                                                // risorsa (il pointer) coincide
                                                // con quello dell'oggetto che
                                                // lo racchiude.
        
        p->execute();   // destructor deletes pointer in case of exception
                        // (stack unwinding)

        // no `delete` on pointer
    }
    ```

#### Different strategies
- Passaggio di proprieta'
- Creazione di una copia
- Condivisione con conteggio dei riferimenti
    - Garbage collector, with all its drawbacks (no cycles, overhead, etc.)
- Condivisione in lettura (`const`) e duplicazione in scrittura

#### C++ Smart Pointers
```cpp
#include <memory>
```
- `std::shared_ptr<BaseType>`
    - Conteggio dei riferimenti
    - Copiabile e assegnabile
    - Inizializzabile 
        - `make_shared<BaseType>(params...)`
        - Copia o assegnazione
    - `NullPointerException` se inizializzato con `NULL`
    - Due campi: 64 bit, accesso due volte
        - Proprieta' condivisa `objectPtr`
        - Blocco di controllo `counterPtr`
            - `counter`, contatore dei riferimenti
            - `weakCnt`
            - `objectPtr`, per distruttore
    - E' possibile specificare un distruttore (altrimenti usa `delete`)
    - `get()` restituisce una copia del puntatore nativo
        - Per backward-compatibility
    - `reset()` decrementa il contatore ed elimina il riferimento
- `std::weak_ptr<BaseType>`
    - Senza conteggio di riferimenti = cicli
    - Si crea a partire da uno `shared_ptr`
    - Si accede al dato promuovendolo temporaneamente ad uno `shared_ptr` tramite `lock()`
        - Controllare la validita' con `expired()`
    - Puntano solo al blocco di controllo
        - Non partecipano al conteggio dei riferimenti
    - Esempio
        ```cpp
        std::weak_ptr<int> gw;
        
        void f() {
            if (auto spt = gw.lock())
                std::cout << "gw: " << *spt << "\n";
            else
                std::cout << "gw e' scaduto\n";
        }
        
        int main() {
            {
                auto sp = std::make_shared<int>(42);
                gw = sp;
                f(); // OK
            
            } // sp viene distrutto, gw scade

            f(); // scaduto
        }
        ```
- `std::unique_ptr<BaseType>`
    - Non copiabile, ne' assegnabile
    - Movibile per
        - Costruzione
        - Operatori di movimento
            - Trasferito ad un altro `unique_ptr`
            - Esplicitamente con `std::move()`
    - Usi
        - Garantire la distruzione di un oggetto
        - Gestione sicura di oggetti polimorfici
        - `std::make_unique<BaseType>()`

# 10. Librerie C++

#### I/O
- `stdio.h` (`printf()`, `scanf()`, etc.) gestiscono solo i tipi elementari
- `ios` e' la classe base (virtuale) di tutti gli stream
    - Stato dello stream
    - Manipolazione del formato
- `std::cout`, `std::cin`, `std::cerr` e `std::clog` sono variabili
    - `if (!cout)` o `if (!cin.good())` per fare il check di errori, perche' non vengono lanciate eccezioni

#### Standard Template Library
- `std::set` richiede l'implementazione di `operator<()`
- Iterators
    - Gli iteratori sono sottoclassi dei containers (`vector`, `list`, etc.)
    - Esempio
        ```cpp
        class C;
        list<C> l(10);

        list<C>::iterator iter = l.begin();
        list<C>::iterator end = l.end();

        for (; iter != end; ++iter) {
            C elem = *iter;
            // ...
        }

        std::for_each(l.being(), l.end(), print);
        ```
- Le `map` usano `pair<t1, t2>`

### Il processo di compilazione
- [The `extern` keyword](https://docs.microsoft.com/en-us/cpp/cpp/extern-cpp?view=vs-2019)
    > The `extern` keyword is applied to a global variable, function or template declaration to specify that the name of that thing has external linkage

    > `extern "C"` specifies that the function is defined elsewhere and uses the C-language calling convention.\
    The `extern "C"` modifier may also be applied to multiple function declarations in a block.

# 11. Librerie
- L'uso di una libreria richiede due fasi
    - Identificazione dei moduli necessasri e loro caricamento in memoria
    - Aggiornamento degli indirizzi per puntare correttamente ai moduli caricati
- Librerie gestite da:
    - Linker (librerie statiche)
    - Loader (librerie collegate dinamicamente)
    - Durante l'esecuzione dal programma stesso (librerie caricate dinamicamente)
- Il loader carica dei `.dll` in modo ricorsivo


### Tassonomia delle librerie
- Librerie statiche (linker)
- Librerie dinamiche
    - Collegate dinamicamente (loader)
    - Caricate dinamicamente (dal programma stesso)

#### Librerie statiche
- Archivio, `lib*.a` in Linux e `.lib` in Windows
    - Concatenazione di file `.o`
    - L'*archiver* `ar` aggiunge file `.o` ad una libreria statica
- Flag `-l` al compiler per specificare librerie
    - In Linux, `-lpthread` diventa `lib_pthread.a`
    - Vengono cercate nella current directory e in `LD_LIBRARY_PATH`
    - `gcc` usa `-lc`, `g++` usa `lcc
- Flag `-L` al compiler per specificare directory contenenti librerie
- Stessi contenuti in processi differenti
    - Pagine fisiche replicate
- Ogni applicazione che fa uso di una libreria statica dev'essere ricompilata ad ogni modifica della libreria

#### Librerie dinamiche
- L'executable contiene info su dove recuperare le librerie
- File `.so` in Linux (ELF) e `.dll` in Windows (PE2)
- Condivisione di pagine fisiche
    - Librerie mappate in spazi di indirizzamento lontani dall'executable
- Linux dynamic linker: `ld.so`
    - E' anch'esso una libreria dinamica
    - Non ha riferimenti a nessun'altra libreria
    - Viene sempre mappato in memoria
    - Mappato nello spazio virtale, ma vengono usate sempre le stesse pagine fisiche
- In Windows, il dynamic linker fa parte del kernel
- Come si caricano in Linux
    - `dlopen()` mappa un `.o` nello spazio di indirizzamento del programma
    - `dlsym()` cerca l'indirizzo di un simbolo (variable o funzione) di un file `.o` aperto
    - `dlerror()` per l'ultimo errore occorso
    - `dlclose()` unmappa una libreria `.so` dallo spazio di indirizzamento
    - Esempio
        ```c
        #include <stdio.h>
        #include <dlfcn.h>

        void invoke(char* lib, char* m, float arg) {
            void* dl_handle = dlopen(lib, RTLD_LAZY); // lazy loading
            if (!dl_handle) return;
            float (*func)(float) = dlsym(dl_handle, m); // cerca `cosf()`
            if (func==NULL) return;
            printf("Result: %f\n", (*func)(arg)); // chiama `cosf()` di 'libm.so'
            dlclose(dl_handle);
        }

        int main(int argc, char *argv[]){
            invoke("libm.so", "cosf", 3.14156f);
        }
        ```
- Come si caricano in Windows 
    - I DLL possono contenere anche risorse (pointers)
    - I DLL possono avere una funzione di ingresso `DllMain`
        - Il loader la invoca prima di ritornare il controllo al programma
        - La DLL puo' fare una inizializzazione globale
        - Parametri: `DllMain(HINSTANCE handle, DWORD r, PVOID unused)`
            - `HANDLE` della DLL
            - Flag
                - `DLL_PROCESS_ATTACH`: DLL mappata in un processo
                - `DLL_PROCESS_DETACH`: DLL about to be un-mapped
                - `DLL_THREAD_ATTACH` e `DLL_THREAD_DETACH` per Visual Basic
    - `LoadLibrary()` mappa un DLL nello spazio di indirizzamento del programma
        - `HANDLE` per l'indirizzo della DLL (`HMODULE`) o errore con `GetLastError()`
- Condivisione variabili
    - Le variabili globali di una stessa DLL non vengono condivise tra processi diversi
        - ELF e PE hanno segmenti di codice e dati, e questi ultimi non sono condivisi
    - E' possibile creare zone di memoria condivise tra tutti i processi che utilizzano una certa DLL
        - Solo per variabili inizializzate
        - Apposito segmento in DLL
    - [Esportazione simboli](https://stackoverflow.com/questions/10222566/what-is-dllspecdllimport-and-dllspecdllexport-means)
        - > `__dllspec(dllexport)` exports a symbol. It makes it available from outside a DLL. 
        - > `__declspec(dllimport)` imports a symbol. It practically says "this symbol is not defined in this application, it needs to be imported from a DLL file".
        - In header files `.h` appositi
        - [Esempio](https://stackoverflow.com/questions/37571359/c-cpp-calling-a-dll-dynamically-with-parameters)
            ```c
            // SampleDLL.h
            #ifdef EXPORTING_DLL    // il pre-compiler vede tutti i simboli di tutti
                                    // i moduli. Questa define deve avere un nome
                                    // particolare per evitare dei clash
            extern __declspec(dllexport) void HelloWorld();
            #else
            extern __declspec(dllimport) void HelloWorld();
            #endif
            ```

            ```c
            // SampleDLL.c
            #define EXPORTING_DLL
            #include "sampleDLL.h"

            BOOL APIENTRY DllMain(…)

            void HelloWorld() { printf("Hello world"); }
            ```

            ```c
            // Static_Dll_usage.c 
            #include "sampleDLL.h"
            void someMethod() {
                HelloWorld();
            }
            ```

            ```c
            // Dynamic_Dll_usage.c
            typedef VOID (*DLLPROC) (LPTSTR);

            // L"string" per widechar (UNICODE)
            HINSTANCE hDLL = LoadLibrary(L"sampleDLL.dll");

            if (hDLL != NULL) {
                DLLPROC Hw = (DLLPROC)GetProcAddress(hDLL, L"HelloWorld");
                if (Hw != NULL)
                    (*Hw)(); // "Hello world'
                FreeLibrary(hDLL);
            }
            ```
    - `extern c` per evitare il name mangling del compiler
        ```cpp
        // SampleDLL.def
        LIBRARY "sampleDLL"

        EXPORTS
            HelloWorld
        ```
    - Vecchia soluzione: file module definition `.def` con la lista di simboli da importare ed esportare
- Compilazione DLL: i file `.lib`
    - Visual Studio genera, oltre al `.dll`, un file `.lib`.
    - Contengono stub methods delle API offerte.
        - "Se la DLL non e' stata caricata, caricala, altrimenti chiama direttamente la funzione"
- `FreeLibrary()` per il rilascio

# 12. Programmazione concorrente

### Thread e memoria
- I thread condividono:
    - Spazio di indirizzamento (variabili globali)
    - Codice
    - Costanti
    - Heap
- Ogni thread ha:
    - Stack delle chiamate (variabili locali)
    - Puntatore all'ultimo contesto per la gestione delle eccezioni
        - Registro `EP`
        - Ad ogni blocco `try` viene salvato un puntatore al suo blocco `catch`
    - Lo stato del proprio processore virtuale
        - Instruction pointer `IP`, etc...
        - Per preemption
        - E' nel kernel

### Sincronizzazione in Windows
- Oggetti user
    - `CriticalSection`, che usa `test-and-set`
    - `ConditionVariable`
- Oggetti kernel
    - `Mutex`, `Event`, `Semaphore`, `Pipe`, `Mailslot`, etc.

### Sincronizzazione in Linux
- Oggetti `PThreads`
    - `phread_mutex`, `pthread_cond`, etc.
- Oggetti kernel
    - Semaphor, pipe, signal, futex

# 13. Programmazione concorrente in C++
- C++ standardizza il concetto di thread in `std::thread`

### Esecuzione asincrona
```cpp
#include <future>
```
- Funzione `std::async()`
- Parametri:
    - Il nome di una funzione/lamba expression/oggetto funzionale (*callable*)
    - Parametri per la funzione
- Funzione eseguita in un altro thread
- Restituisce `std::future<T>`
    - E' solo movibile (non e' copiabile)
    - `get()` per il risultato
        - Rilancia l'eccezione se il thread asincrono ne ha lanciata una
        - Si blocca in attesa se l'operazione non e' finita
        - Se l'esecuzione non e' iniziata, ne forza l'avvio
        - Chiamabile una volta sola, altrimenti eccezione
    - `wait()`
        - Consente di non consumare il risultato subito, garantendo al chiamante della `get()` di ottenere qualcosa di significativo
        - Chiamabile piu' volte
        - Varianti temporali
            ```cpp
            wait_for(std::chrono::duration duration)
            wait_until(std::chrono::time_point time_point)
            ```
            - Restituiscono
                - `std::future_status::deferred`, funzione non partita
                - `std::future_status::ready`, risultato pronto
                - `std::future_status::timeout`, risultato non pronto
            - Non forzano l'avvio in caso di task *deferred*
            - Indicando durata nulla, permettono di scoprire se il task sia gia' terminato o no
    - Il suo distruttore aspetta che il risultato sia scritto
- Esempio
    ```cpp
    #include <future>
    #include <string>
    std::string f1(std::string p1, double p2) { /* .. */ }
    std::string f2(int p)  { /* .. */ }

    int main() {
        std::future<std::string> future1 = std::async(f1, "string", 3.14);
        std::string res2 = f2(18); // thread principale
        std::string res1 = future1.get();
        std::string result = res1 + res2;
    }
    ```
- `std::launch::async` attiva sempre un thread secondario
    - `std::async` puo' non creare un thread se ci sono poche risorse
    - Pericoloso perche' lo stack viene copiato per ogni thread
- `std::launch::deferred`
    - Lazy evaulation upon calling `get()` or `wait()`
    - Usato in caso di tante richieste simultanee
    - Il sistema prova a creare un secondo thread, altrimenti segna l'attivita' come *deferred*
    - Il metodo `shared()` su un `std::future<T>` ritorna un `std::shared_future<T>`
        - Puo' essere interrogato piu' di una volta
            - Usato quando dev'essere utilizzato da piu' thread
        - Costa di piu' in termini di memoria
        - Stessi metodi `get()`, etc.
            - Invalida il `std::future<T>` originale
        - E' anche copiabile

### Forme di sincronizzazione
- `std::mutex`
    - `lock()` e `unlock()`
    - Internamente memorizza una memory barrier, ovvero l'ID dell'owner (scrivibile con `test-and-set`)
    - Un thread che chiama `lock()` su un `std::mutex` gia' preso viene bloccato
    - Altri tipi
        - `std::recursive_mutex` e' ricorsivo (`lock()` piu' volte) ma occorre chiamare `unlock()` tante volte quanto e' stato chiamato il `lock()` (si comporta quindi come un semaforo)
        - `std::timed_mutex` aggiunge i metodi `try_lock_for()` e `try_lock_until()`
            - Esempio
                ```cpp
                #include <mutex>

                std::mutex m;

                void someFunction() {
                    while (m.try_lock() == false) {
                        do_some_work();
                    }

                    // Il lock e' gia' stato acquisito da `try_lock`, quindi
                    // `l` registra `m` al suo interno, senza acquisirlo una
                    // seconda volta
                    std::lock_guard<std::mutex> l(m, std::adopt_lock);
                    
                    // ...
                    
                    // quando l viene distrutto, rilascia il possesso del mutex
                }
                ```
        - `std::recursive_timed_mutex`
      - `std::lock_guard<Lockable>` per il paradigma RAII
        - `lock()` nel suo costruttore e `unlock()` nel suo distruttore
        - Esempio
            ```cpp
            template <class T>
            class shared_list {
                std::list<T> list;
                std::mutex m; // il `lock_guard` lavora su un mutex
            
                T& operator=(const shared_list<T>& that);
                shared_list(const shared_list<T>& that);

            public:
            
                int size() {
                    std::lock_guard<std::mutex> l(m);
                    return list.size();
                    // il distruttore di `lock_guard` rilascia il lock
                }

                T front() {
                    std::lock_guard<std::mutex> l(m);
                    return list.front();    // se lancia un'eccezione,
                                            // stack-unwinding chiama
                                            // il distrutore di `lock_guard`
                }
            
                void push_front(T t) {
                    std::lock_guard<std::mutex> l(m);
                    list.push_front(t);
                }
            };
            ```
    - `std::unique_lock<Lockable>` estende `lock_guard`
        - Consente di rilasciare e riacquisire l'oggetto `Lockable` tramite `unlock()` e `lock()` (in questo ordine)
            - Possono lanciare `std::system_error`
        - Il costruttore ha piu' politiche di gestione
            - `adopt_lock` verifica che il thread possieda gia' il `Lockable` passato come parametro e lo adotta
            - `defer_lock` si limita a registrare il riferimento al `Lockable`, senza cercare di acquisirlo
