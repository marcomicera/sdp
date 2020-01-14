#import "exchanger.cpp"
#import <vector>
#import <thread>
#import <iostream>

#define EXCHANGES 10

using namespace std;

int main() {

    mutex cout_lock;
    vector<thread> thread_pool;

    for (unsigned short exchange_id = 0; exchange_id < EXCHANGES; ++exchange_id) {

        Exchanger<int> e(exchange_id);

        for (unsigned short thread_id = 0; thread_id < 2; ++thread_id) {

            thread_pool.push_back(thread([&e, &cout_lock, thread_id]() {

                int exchange_value;

                if (thread_id == 0) {
                    exchange_value = 3;
                } else {
                    exchange_value = 5;
                }

                int result = e.exchange(exchange_value);

                cout_lock.lock();
                cout << "Exchange " << e.getId() << ": thread " << thread_id << " got value " << result << '.' << endl;
                cout_lock.unlock();
            }));
        }
    }

    for (auto &thread: thread_pool) {
        thread.join();
    }

    return 0;
}
