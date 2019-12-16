/*
 * Scrivere un'applicazione Stack<T> che gestisca l'inserimento/prelievo di elementi <T> con
 * un metodo "push(T elemento)" e "T pop()". Deve essere presente un metodo "int grandezza()" che restituisca
 * il numero di elementi inseriti. Nel caso lo stack sia pieno, il thread di inserimento si deve bloccare,
 * così come si deve bloccare il thread di prelievo se lo stack è vuoto.
 */

#include <mutex>
#include <condition_variable>

using namespace std;

template<class T>
class Stack {
private:
    T *stack;
    unsigned int dim;
    mutex m;
    condition_variable nonEmptyStack;
    condition_variable emptyStack;
    int nitems;
public:
    Stack(unsigned int dim) : nitems(0), dim(dim) {
        stack = new T[dim];
    }

    ~Stack() {
        delete[] stack;
    }

    void push(T item) {
        unique_lock<mutex> ul(m);
        nonEmptyStack.wait(ul, [this]() { return nitems != dim; });
        stack[nitems] = std::move(item);
        ++nitems;
        emptyStack.notify_all();
    }

    T pop() {
        unique_lock<mutex> ul(m);
        emptyStack.wait(ul, [this]() { return nitems != 0; });
        --nitems;
        T item = stack[nitems];
        nonEmptyStack.notify_all();
        return item;
    }

    int grandezza() {
        lock_guard<mutex> lg(m);
        return nitems;
    }
};
