//
// Created by abarbs on 11/17/15.
//

#ifndef SBEXAMPLES_STATELESS_DELETER_H
#define SBEXAMPLES_STATELESS_DELETER_H

template<typename T, typename D, D Deleter>
struct stateless_deleter {
    using pointer = T;

    void operator()(T x) {
        Deleter(x);
    }
};

#endif //SBEXAMPLES_STATELESS_DELETER_H
