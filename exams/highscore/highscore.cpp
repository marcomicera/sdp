/*
 * Una classe concorrente gestisce la classifica di un gioco online:
 * ogni concorrente è identificato da un nickname e ha un punteggio associato.
 * Un metodo ritorna una copia della classifica, un altro inserisce un nuovo punteggio
 * relativo a un concorrente (inserendo un nuovo concorrente in classifica
 * se non esistente o aggiornando il punteggio se maggiore di quello vecchio),
 * un metodo si mette in attesa di variazioni sulla classifica.
 */

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>

using namespace std;

class Highscore {

    map<string, int> table;
    mutex m;
    condition_variable change_occurred;
    bool changed;

public:

    map<string, int> get_table() {
        lock_guard<mutex> lg(m);
        return table;
    }

    void insert_score(const string& user, int score) {
        if (score > 0) {
            lock_guard<mutex> lg(m);
            if (table.find(user) == table.end()) {
                table.insert(pair<string, int>(user, score));
                changed = true;
                change_occurred.notify_all();
            } else if (score > table[user]) {
                table[user] = score;
                changed = true;
                change_occurred.notify_all();
            }
        }
    }

    void wait_for_changes() {
        unique_lock<mutex> ul(m);
        change_occurred.wait(ul, [this]() { return changed; });
    }
};
