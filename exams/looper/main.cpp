#include <iostream>
#include <utility>
#include <thread>
#include "looper.cpp"

#define THREAD_POOL_SIZE 20

using namespace std;

struct Message {
    string text;

public:
    explicit Message(string text): text(std::move(text)) {}
};

class TestHandler: public Handler<Message> {
    void handle(Message msg) override {
        cout << "Handler is handling this message: " << msg.text << endl;
    }
};

int main() {

    vector<thread> thread_pool;
    TestHandler test_handler;
    Looper<Message> looper(&test_handler);

    for (unsigned short thread_id = 0; thread_id < THREAD_POOL_SIZE; ++thread_id) {
        thread_pool.push_back(thread([&looper, thread_id]() {
            looper.send(Message("thread " + to_string(thread_id) + " is in the house."));
        }));
    }

    return 0;
}
