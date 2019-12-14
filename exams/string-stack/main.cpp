#include "stack.cpp"
#include <vector>
#include <thread>
#include <iostream>

#define N 20

using namespace std;

int main() {
    Stack myStack(10);

    vector<thread> poolThread;

    for (int i = 0; i < N; ++i)
        poolThread.push_back(
                thread([i, &myStack]() {
                           myStack.push(to_wstring(i));
                           wcout << myStack.pop() << endl;
                       }
                )
        );

    for (int i = 0; i < N; i++)
        poolThread[i].join();

    return 0;
}
