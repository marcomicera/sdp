/*
 * La classe generica Exchanger<T> permette a due thread di scambiarsi un valore di tipo T.
 * Essa offre il metodo T exchange( T t) che blocca il thread corrente fino a che un altro thread non
 * invoca lo stesso metodo, sulla stessa istanza. Quando questo avviene, il metodo restituisce l'oggetto
 * passato come parametro dal thread opposto.
 * Si implementi la classe C++, usando la libreria standard C++11.
 */

#import <mutex>
#include <condition_variable>

using namespace std;

template<class T>
class Exchanger {

    // ID
    unsigned short id;

    mutex m;
    T item1, item2;
    bool item1_set, item2_set;
    condition_variable item2_set_cv;

public:

    explicit Exchanger(unsigned short exchange_id) : id(exchange_id), item1_set(false), item2_set(false) {}

    T exchange(T t) {
        unique_lock<mutex> l(m);
        if (!item1_set) {
            item1 = t;
            item1_set = true;
            item2_set_cv.wait(l, [this]() { return !(item2_set == false); });
            item2_set - false;
            return item2;
        } else {
            item2 = t;
            item2_set = true;
            item2_set_cv.notify_one();
            item1_set = false;
            return item1;
        }
    }

    unsigned short getId() {
        return id;
    }
};
