# System calls, programming, embedded systems, etc.
Course held by Prof. Malnati

#### Useful links
- [OneNote notes](https://1drv.ms/u/s!AkzjmTTkND1lhPVBx3kR1-NqPIPxTw)
- [O.S. books](http://www.os-book.com/)

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

# 7. Ereditarietà e polimorfismo

# 8. Funzioni e operatori

# 9. Programmazione generica
