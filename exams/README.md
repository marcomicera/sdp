# Tips and tricks

Here are some tips and tricks useful for the written exam.
Please note that the following tips keep the overall syntaxt as short
as possible, hence these are not recommended for the every-day life.

There are a lot of smarter ways to do these things (e.g., the `std::rand()` is discouraged online). 

## Locking
- `unique_lock<mutex>` allows `lock()` and `unlock()`
    - Suitable for `condition_variable`
    - Waiting
        ```cpp
      #include <condition_variable>
      using namespace std;
      
      // ...

      condition_variable cv;
      
      // ...
      
      unique_lock<mutex> ul(m);
      cv.wait(ul, [this]() { return !(while_condition); });
        ```
        - A simple `cv.wait(ul)` **does not perform the `unlock()` operation**
- `lock_guard<mutex>` does not

## Data structures

### `std::map`
- Declaration
    ```cpp
    map<string, int> table;
    ```
    - No need to use a constructor: this is not Java!
- Insertion
    ```cpp
    table.insert(pair<string, int>(user, score));
    ```
- _Exists_
    ```cpp
    table.find(user) == table.end()
    ```
- Iteration
    ```cpp
    for (auto const &it : highscore_table) {
        cout << "User:\t" << it.first << "\tScore:\t" << it.second << endl;
    }
    ```

### `std::vector`
- Declaration
    ```cpp
    vector<string> random_usernames = {"jack", "wes", "alex"};
    ```
- Iteration
    ```cpp
    for (auto& t: thread_pool) {
        t.join();
    }
    ```

## Threading
Useful when creating dummy main programs for testing purposes.

1. Thread pool
    ```cpp
    vector<thread> thread_pool;
    ```
1. Creating threads
    ```cpp
    #define THREAD_POOL_SIZE 20
    
    // ...
    
    for (unsigned short thread_id = 0; thread_id < THREAD_POOL_SIZE; ++thread_id) {
        thread_pool.push_back(thread([&data_structure, thread_id]() { /* `thread_id` by value! */
            unsigned short min = 0, max = 10;
            unsigned short random_number = min + rand() % (max - min);
            // thread body
        }
    ```
1. Wait for their completion
    ```cpp
    for (auto& t: thread_pool) {
        t.join();
    }
    ```