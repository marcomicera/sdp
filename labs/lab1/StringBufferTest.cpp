//
// Created by marcomicera on 10/31/19.
//

#include <cstdio>
#include "StringBuffer.h"

int main() {
    StringBuffer s1("Hello");
    StringBuffer s2("world!");
    s1.append(" ");
    s1.append(s2);
    printf(s1.c_str()); //Hello world!
    s1.set("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed doeiusmod tempor incididunt ut labore et dolore magna aliqua.");
    s1.append("\n");
    printf("%zu",s1.size()); //124
    s2.clear();
    for (int i=0; i<10; i++)
        s2.insert(s1,0);
    printf(s2.c_str()); //Lorem ipsum ... 10 volte
    printf("%zu",s2.size()); //1240
    return 0;
}
