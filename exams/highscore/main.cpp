#include <cstdlib>
#include <thread>
#include <iostream>
#include "highscore.cpp"
#include <vector>

#define USERS 20

using namespace std;

int main() {

    Highscore highscore;
    vector<string> random_usernames = {"jack", "wes", "alex"};
    mutex cout_mutex;
    vector<thread> thread_pool;

    for (unsigned short thread_id = 0; thread_id < USERS; ++thread_id) {
        thread_pool.push_back(thread([&highscore, &cout_mutex, &random_usernames, thread_id]() {
            unsigned short operation = 0 + rand() % (3 - 0);
            switch (operation) {
                case 0: { // get table
                    map<string, int> highscore_table = highscore.get_table();
                    cout_mutex.lock();
                    cout << "Highscore table by thread " << thread_id << endl;
                    for (auto const &it : highscore_table) {
                        cout << "User:\t" << it.first << "\tScore:\t" << it.second << endl;
                    }
                    cout_mutex.unlock();
                    break;
                }
                case 1: { // score insertion
                    unsigned short random_username_index = 0 + rand() % (3 - 0);
                    unsigned short random_score = 0 + rand() % (10 - 0);
                    highscore.insert_score(random_usernames[random_username_index], random_score);
                    cout_mutex.lock();
                    cout << "Thread " << thread_id << " inserted " << random_usernames[random_username_index] << ", "
                         << random_score << endl;
                    cout_mutex.unlock();
                    break;
                }
                case 2: { // wait for changes
                    cout_mutex.lock();
                    cout << "Thread " << thread_id << " is waiting for changes" << endl;
                    cout_mutex.unlock();
                    highscore.wait_for_changes();
                    cout_mutex.lock();
                    cout << "Thread " << thread_id << " is no longer waiting for changes" << endl;
                    cout_mutex.unlock();
                    break;
                }
                default: {
                    break;
                }
            }
        }));
    }

    for (auto& t: thread_pool) {
        t.join();
    }

    return 0;
}
