#import "challenge.cpp"
#import <iostream>

using namespace std;

int main() {

    Challenge c;
    c.accept(true);
    c.result1(5);
    c.result2(3);
    cout << "Winner: " << c.winner() << endl;

    return 0;
}
