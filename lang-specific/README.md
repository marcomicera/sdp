### Solo movibili
- `unique_ptr<T>`
- `future<T>`
- `thread`

# 6. Composizione di oggetti

### Confronto signatures
- Costruttore di copia
    ```cpp
    CBuffer(const CBuffer& source)
    ```
- Operatore di assegnazione
    ```cpp
    CBuffer& operator=(const CBuffer& source)
    ```
- Costruttore di movimento
    ```cpp
    CBuffer(CBuffer&& source)
    ```
- Assegnazione per movimento
    ```cpp
    CBuffer& operator=(CBuffer&& source)
    ```

# 7. Ereditarietà e polimorfismo

### Type casting
- `static_cast<T>`
- `dynamic_cast<T>`
- `reinterpret_cast<T>`
- `const_cast<T>`

# 9. Programmazione generica

#### C++ Smart Pointers
- `std::shared_ptr<BaseType>`
- `std::weak_ptr<BaseType>`
- `std::unique_ptr<BaseType>`

# 11. Librerie

### Tassonomia delle librerie
- Librerie statiche (linker)
- Librerie dinamiche
    - Collegate dinamicamente (loader)
    - Caricate dinamicamente (dal programma stesso)

# 13. Programmazione concorrente in C++

### Esecuzione asincrona
- Funzione `std::async()`
- Restituisce `std::future<T>`
    - `get()`
    - `wait()`
- `std::launch::async`
- `std::launch::deferred`
- `std::shared_future<T>` = `shared()` su un `std::future<T>`
    - Stessi metodi `get()`, etc.

### `std::mutex`
- `lock()` e `unlock()`
    - `std::recursive_mutex`
        - `std::timed_mutex`
        - `std::recursive_timed_mutex`
    - `std::lock_guard<Lockable>`
        - `std::unique_lock<Lockable>`
            `unlock()` e `lock()`
            - Il costruttore ha piu' politiche di gestione
                - `adopt_lock`
                - `defer_lock`

### `std::atomic<T>`
- `load()`, `store()`, `operator++()` per numeri
- `fetch_add(val)` e `fetch_sub(val)` per `+=` e `-=` rispettivamente
- `exchange(val)` per lo scambio di valori

# 14. Threads

#### Retrieve results while secondary thread is still running
-  `std::promise<T>`
    - `get_future().get()`

# 15. Condition variables

### The Singleton pattern in a multi-threaded environment
- `std::once_flag` e `std::call_once()`

# 16. Interprocess communication on Windows
- `Event`
    - `CreateEvent(...)`, `OpenEvent(...)`
    - `SetEvent(...)`, `ResetEvent(...)`
    - `PulseEvent(...)`
- `Semaphore`
    - `Up()`, `Down()`
    - `WaitForSingleObject()`
    - `CreateSemaphore(...)`, `OpenSemaphore(...)`
    - `WaitForSingleObject()`, `WaitForMultipleObjects()`
    - `ReleaseSemaphore(...)`
- Mutex
- Mailslot
    - `CreateMailslot(...)`
    - `GetMailslotInfo(...)`
    - `ReadFile(...)`, `ReadFileEx(...)`
- Pipe
    - Anonymous
    - Named
        - `CreateNamedPipe(...)`, `OpenFile(...)`, `CallNamedPipe(...)`
        - `ReadFile()`, `WriteFile()`
- File mapping
    - `CreateFileMapping(...)`
    - `MapViewOfFile(...)`, `UnmapViewOfFile(...)`
- Socket
- RPC

#### Calls
- `WaitForSingleObject()`
- `WaitForMultipleObjects()`

# 17. Interprocess communication on Linux
- Message Queues
    - `msgget()`, `msgsnd()`, `msgrcv()`, `msgctl()`
- Pipe
    - `pipe()`
- FIFO
- Shared Memory
    - `shmget()`

# IPC: Windows vs. Linux

| Windows        | Linux         | Description                                             |
|----------------|---------------|---------------------------------------------------------|
| Event          | /             | CV                                                      |
| Semaphore      |               |                                                         |
| Mutex          |               | Semaphore = 1                                           |
| Mailslot       | Message queue | Event queue                                             |
| Anonymous pipe | Pipe          | Monodirezionali, byte-oriented, bloccanti (`|` in Unix) |
| Named pipe     | FIFO          | Bidirezionali, also message-oriented, also non-blocking |
| File mapping   | Shared memory |                                                         |
| Socket         |               |                                                         |
| RPC            |               |                                                         |
