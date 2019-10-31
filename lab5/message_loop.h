//
// Created by marcomicera on 10/31/19.
//

#ifndef LAB1_MESSAGE_LOOP_H
#define LAB1_MESSAGE_LOOP_H


#include <future>

class message_loop {

public:

    /**
     * Tale metodo ha effetto solo la prima volta che viene invocato e determina l’avvio del
     * thread responsabile del ciclo di elaborazione dei messaggi.
     */
    void startup();

    /**
     * Tale metodo comporta lo svuotamento della coda e la distruzione del thread, se è mai
     * stato avviato tramite il metodo precedente. Il metodo ha effetto una volta sola e viene
     * invocato anche dal distruttore della classe. Dopo la sua invocazione, eventuali tentativi di
     * inserire nuovi messaggi nella coda con i metodi successivi, lanciano un’eccezione.
     */
    void shutdown();

    /**
     * Inserisce nella coda dei messaggi un task da svolgere. Quando il thread incapsulato lo
     * estrarrà, provvederà ad invocarlo. Se questo metodo viene invocato dopo shutdown(),
     * lancia un’eccezione.
     */
    void post_task(std::shared_ptr<std::packaged_task<void()>>);

    /**
     * Come nel caso precedente inserisce il task nella coda dei messaggi. In questo caso, però, il
     * messaggio non potrà essere estratto prima che scadano delay millisecondi. Eventuali altri
     * messaggi inseriti successivamente nella coda tramite postTask(...) o postDelayed(...)
     * potranno essere elaborati prima del messaggio corrente se la loro scadenza risulterà più
     * ravvicinata. Se questo metodo viene invocato dopo shutdown(), lancia un’eccezione.
     */
    void post_delayed(std::shared_ptr<std::packaged_task<void()>>, std::chrono::milliseconds);
};


#endif //LAB1_MESSAGE_LOOP_H
