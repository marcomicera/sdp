/*
 * Settembre 2019. Traccia non disponibile. Descrizione sommaria:
 *
 *
 * Gioco online tra due giocatori, gestione di più thread.
 *
 * Implementare classe Challenge che ha, oltre al costruttore, i seguenti metodi:
 *
 * - void accept(bool response)
 *   Indica il responso dello sfidato alla richiesta di giocare
 *
 * - void result1(int score)
 *   Indica lo score dello sfidante
 *
 * - void result2(int score)
 *   Indica lo score dello sfidato
 *
 * - int winner()
 *   Ritorna:
 *   - 0 se è pari
 *   - 1 se ha vinto lo sfidante
 *   - 2 se ha vinto lo sfidato
 *   - (-1) se la richiesta è stata rifiutata
 *
 * `accept()` deve precedere `result1` e` result2`, che invece non hanno ordine di chiamata tra loro.
 * Tutti e 3 possono essere chiamati una sola volta, la violazione dei vincoli genera eccezione.
 *
 * `winner()` si mette in attesa senza consumare CPU se il gioco è in corso o non è ancora cominciato.
 */

#import <thread>
#import <mutex>
#import <condition_variable>
#import <iostream>

using namespace std;

class ChallengeException: public exception {
    string _cause;
public:
    explicit ChallengeException(const string& cause): _cause(cause) {}
    const char* what() const throw() { return _cause.c_str(); }
};

class Challenge {

    thread challenger, challenged;
    mutex m;

    int score1, score2;
    bool result1_set, result2_set;

    /*
     * 0: challenger challenged the adversary, no response yet
     * 1: challenged refused the challenge
     * 2: challenged accepted the challenge
     * 3: one result has been set
     * 4: both results have been set
     */
    unsigned short status;

    condition_variable accept_called, game_accept, results_set;

public:

    Challenge(): status(0), result1_set(false), result2_set(false) {
        challenger = thread([this]() {
            unique_lock<mutex> l(m);
            game_accept.wait(l, [this]() { return !(status == 0); });
            if (status == 1) {

            } else if (status == 2) {
                results_set.wait(l, [this]() { return !(status < 4); });
            }
        });

        challenged = thread([this]() {
            unique_lock<mutex> l(m);
            accept_called.wait(l, [this]() { return !(status == 0 ); });
            if (status == 1) {

            } else if (status == 2) {
                results_set.wait(l, [this]() { return !(status < 4); });
            }
        });
    }

    void accept(bool response) {
        lock_guard<mutex> l(m);

        if (status > 2) {
            throw ChallengeException("Cannot accept the game twice.");
        }

        if (response) {
            cout << "Challenge accepted." << endl;
            status = 2;
        } else {
            cout << "Challenge refused." << endl;
            status = 1;
        }
        game_accept.notify_one();
        accept_called.notify_one();
    }

    void result1(int score) {
        lock_guard<mutex> l(m);

        if (status != 1) {
            if (!(status == 2 || status == 3)) {
                throw ChallengeException("Setting result in an invalid status.");
            }

            if (result1_set) {
                throw ChallengeException("Cannot set a result twice.");
            }

            score1 = score;
            result1_set = true;

            if (status == 3) {
                status = 4;
                results_set.notify_all();
            } else {
                status = 3;
            }
        }
    }

    void result2(int score) {
        lock_guard<mutex> l(m);

        if (status != 1) {
            if (!(status == 2 || status == 3)) {
                throw ChallengeException("Setting result in an invalid status.");
            }

            if (result2_set) {
                throw ChallengeException("Cannot set a result twice.");
            }

            score2 = score;
            result2_set = true;

            if (status == 3) {
                status = 4;
                results_set.notify_all();
            } else {
                status = 3;
            }
        }
    }

    int winner() {
        lock_guard<mutex> l(m);

        if (status == 1) {
            return -1;
        } else if (status == 4) {
            if (score1 == score2) {
                return 0;
            } else if (score1 > score2) {
                return 1;
            } else if (score1 < score2) {
                return -1;
            }
        } else {
            throw ChallengeException("Retrieving result in an invalid status.");
        }
    }

    ~Challenge() {
        if (challenger.joinable()) {
            challenger.join();
        }
        if (challenged.joinable()) {
            challenged.join();
        }
    }
};
