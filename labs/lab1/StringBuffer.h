//
// Created by marcomicera on 10/31/19.
//

#ifndef LAB1_STRINGBUFFER_H
#define LAB1_STRINGBUFFER_H


#include <cstdlib>

class StringBuffer {

protected:

    /**
     * Puntatore ad un buffer di caratteri dinamico, in grado
     * di memorizzare stringhe di caratteri di diversa lunghezza.
     */
    char* buffer;

    /**
     * Buffer default dimension.
     */
    static const u_int DEFAULT_DIMENSION = 60;

    /**
     * How many characters are being used.
     */
    u_int used_characters = 0;

public:

    StringBuffer();
    explicit StringBuffer(const char*);
    StringBuffer(const StringBuffer&);
    virtual ~StringBuffer();
    size_t size();
    size_t bufsize();
    void clear();
    void insert(const char*, size_t);
    void insert(const StringBuffer&, size_t);
    void append(const char*);
    void append(const StringBuffer&);
    const char* c_str();
    void set(const char* str);
    void set(const StringBuffer&);
};


#endif //LAB1_STRINGBUFFER_H
