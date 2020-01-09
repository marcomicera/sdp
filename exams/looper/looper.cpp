#include <iostream>
#include <string>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <atomic>

using namespace std;

/*
 * Sia data una classe Handler che gestisce una coda di tipo Message tramite un metodo handle.
 */

template<class Message>
class Handler {
public:

    virtual void handle(Message msg);
};

/*
 * Sia data anche una classe Looper che gestisce il prelievo di tutti i messaggi presenti in coda
 * tramite un thread privato, mentre il metodo send(...) inserisce i messaggi in coda.
 *
 * Il contenuto private della classe Looper è stato ommesso esplicitamente.
 * Realizzare la classe Looper facendo in modo che quando l’oggetto viene distrutto tutti i
 * messaggi presenti in coda vengano gestiti senza la possibilità di ulteriori inserimenti (4pt)
 */

template<class Message>
class Looper {

private:

    thread looper_thread;
    Handler<Message> *handler;
    queue<Message> message_queue;
    mutex m;
    condition_variable empty_queue;
    atomic<bool> terminated;

public:

    Looper(Handler<Message> *handle) : handler(handle), terminated(false) {
        looper_thread = move(thread([this]() {
            unique_lock<mutex> l(m);
            while (!terminated) {
                empty_queue.wait(l, [this]() { return !(message_queue.size() == 0 && !terminated); });
                handler->handle(message_queue.front());
                message_queue.pop();
            }
            cout << "Looper thread is terminating..." << endl;
        }));
    }

    ~Looper() {
        cout << "Looper destructor has been invoked..." << endl;
        terminated = true;
        empty_queue.notify_one();
        if (looper_thread.joinable()) {
            cout << "Looper destructor is joining the looper thread..." << endl;
            looper_thread.join();
        }
        cout << "...looper destructor terminated." << endl;
    }

    void send(Message msg) {
        if (!terminated) {
            lock_guard<mutex> l(m);
            message_queue.push(msg);
            empty_queue.notify_one();
        }
    }
};
