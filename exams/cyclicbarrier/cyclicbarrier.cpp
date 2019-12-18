/*
 * Una Barriera Ciclica è un oggetto di sincronizzazione che permette a N thread
 * (con N specificato all'atto della costruzione dell'oggetto) di attendersi.
 * Tale oggetto dispone di un solo metodo "void attendi()" la cui invocazione
 * blocca il thread chiamante fino a che altri N-1 thread non risultano bloccati insieme ad esso.
 * Quando il numero di thread bloccati raggiunge N, tutti thread si sbloccano e il metodo attendi() ritorna.
 * Ulteriori chiamate al metodo attendi() si comportano analogamente: le prime N-1 invocazioni si bloccano,
 * all'arrivo della successiva invocazione tutti i thread si sbloccano e i metodi ritornano.
 * Si implementi una classe C++ che implementi tale comportamento, usando la libreria standard C++.
 */

#import <mutex>
#import <condition_variable>

using namespace std;

class CyclicBarrier {

    unsigned int size;
    volatile unsigned int waiting_threads;
    mutex m;
    condition_variable full_barrier;

public:

    explicit CyclicBarrier(unsigned int size): size(size), waiting_threads(0) {}

    void wait() {
        unique_lock<mutex> l(m);
        ++waiting_threads;
        if (waiting_threads < size) {
            full_barrier.wait(l, [this]() { return !(waiting_threads < size); });
        } else {
            full_barrier.notify_all();
            waiting_threads = 0;
        }
    }
};
