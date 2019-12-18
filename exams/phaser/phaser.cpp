/*
 * La classe Phaser ha i metodi "void attendi()", "void aggiungi()" e "void rimuovi()".
 * La classe permette a N-1 thread di attendersi tra loro, ma quando si arriva a N-1 i thread si sbloccano.
 * I metodi aggiungi e rimuovi modificano il valore di N (specificato nella costruzione dell'oggetto)
 * e di conseguenza sbloccano i thread se necessario.
 */

#import <mutex>
#import <condition_variable>

using namespace std;

class Phaser {

    volatile int num_threads_threshold;
    volatile int num_waiting_threads;
    mutex m;
    condition_variable full_phaser;

public:

    explicit Phaser(int num_threads_threshold): num_threads_threshold(num_threads_threshold), num_waiting_threads(0) {}

    void wait() {
        unique_lock<mutex> ul(m);
        ++num_waiting_threads;
        if (num_waiting_threads < num_threads_threshold - 1) {
            full_phaser.wait(ul, [this]() { return !(num_waiting_threads < num_threads_threshold - 1); });
            --num_waiting_threads;
        } else {
            full_phaser.notify_all();
        }
    }

    void add() {
        lock_guard<mutex> lg(m);
        if (num_waiting_threads >= num_threads_threshold++) {
            full_phaser.notify_all();
        }
    }

    void remove() {
        lock_guard<mutex> lg(m);
        if (num_waiting_threads >= (--num_threads_threshold) - 1) {
            full_phaser.notify_all();
        }
    }
};
