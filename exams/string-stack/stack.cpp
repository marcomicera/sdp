/*
 * Un'applicazione Win32 presenta più thread che devono operare in modo concorrente su uno stack di stringhe unicode.
 * Si implementi una classe C++ che incapsuli il comportamento di tale stack in modalità thread-safe.
 * Deve essere permesso l'inserimento di nuove stringhe nello stack e l'estrazione dell'ultima stringa inserita
 * (notificando il caso di stack vuoto). Porre particolare attenzione alla gestione della memoria e della concorrenza.
 */

#pragma once

#include <string>
#include <mutex>
#include <condition_variable>
#include <utility>

using namespace std;

class Stack {
private:
    wstring *stack;
    int size;
    int nitems;
    mutex m;
    condition_variable nonEmptyStack;
public:
    Stack(int N) : size(N), nitems(0) {
        stack = new wstring[N];
    }

    ~Stack() {
        delete[] stack;
    }

    void push(wstring item) {
        unique_lock<mutex> ul(m);

        nonEmptyStack.wait(ul, [this]() { return nitems != size; });

        stack[nitems] = std::move(item);

        ++nitems;
    }

    wstring pop() {
        lock_guard<mutex> ul(m);

        if (nitems == 0)
            return L"Empty stack!\n";
        else {
            --nitems;

            wstring popped_item = stack[nitems];

            nonEmptyStack.notify_all();

            return popped_item;
        }
    }
};
