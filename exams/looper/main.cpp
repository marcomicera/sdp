#include <iostream>
#include <utility>
#include "looper.cpp"

using namespace std;

struct Message {
    string text;

public:
    explicit Message(string text): text(std::move(text)) {}
};

class TestHandler: public Handler<Message> {
    void handle(Message msg) override {
        cout << msg.text << endl;
    }
};

int main() {

    TestHandler test_handler;
    Looper<Message> looper(&test_handler);

    looper.send(Message("1st message"));

    return 0;
}
