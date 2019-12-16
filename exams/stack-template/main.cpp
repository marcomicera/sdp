#include "stack.cpp"
#include <vector>
#include <thread>
#include <iostream>
#include <random>

#define N 100

int main() {

    Stack<int> my_stack(50);
    vector<thread> thread_pool;
    mutex cout_mutex;

    for (int i = 0; i < N; i++)
        thread_pool.push_back(
                thread(
                        [i, &my_stack, &cout_mutex]() {
                            random_device rd;
                            mt19937 rng(rd());
                            uniform_int_distribution<int> uni(1, 3);
                            auto val = uni(rng);
                            if (val == 1) {
                                my_stack.push(i);
                                lock_guard<mutex> l(cout_mutex);
                                cout << "Pushed item: " << i << endl;
                            }
                            if (val == 2) {
                                lock_guard<mutex> l(cout_mutex);
                                cout << "Stack length: " << my_stack.grandezza() << endl;
                            }
                            if (val == 3) {
                                lock_guard<mutex> l(cout_mutex);
                                cout << "Popped item: " << my_stack.pop() << endl;
                            }
                        }
                )
        );

    for (int i = 0; i < N; i++)
        thread_pool[i].join();

    return 0;
}
