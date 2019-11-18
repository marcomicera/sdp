//
// Created by marcomicera on 10/31/19.
//

#ifndef LAB1_MESSAGE_LOOP_H
#define LAB1_MESSAGE_LOOP_H


#include <future>
#include <queue>

/**
 * Un ciclo dei messaggi, o message-loop, è un tipico costrutto di programmazione adottato in
 * presenza di interfacce grafiche o di sistemi reattivi che intendono prediligere l’esecuzione sincrona
 * di call-back nell’ambito di un singolo thread pur offrendo la possibilità di reagire allo scorrere del tempo.
 * Il costrutto può essere implementato sotto forma di classe che incapsula un thread, una coda di
 * messaggi basati su priorità e le necessarie primitive di sincronizzazione volte a garantire la
 * correttezza dell’esecuzione.
 *
 * La classe offre i seguenti metodi thread-safe:
 */
class message_loop {

protected:

    std::priority_queue<std::shared_ptr<std::packaged_task<void()>>> queue;

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

/*
 * Competenze da acquisire
 * - Tecniche di sincronizzazione
 * - Uso di packaged_task, condition_variable, once_flag
 * - Uso delle funzioni relative allo scorrere del tempo
 */

#endif //LAB1_MESSAGE_LOOP_H
