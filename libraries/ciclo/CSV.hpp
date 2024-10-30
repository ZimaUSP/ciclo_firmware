#ifndef __CSV_HPP__
#define __CSV_HPP__

#include "Arduino.h"

class CSV {
    public:
        String to_csv(String nome1, double dado1[], String nome2, int dado2[], int quantidade);
};

#endif  // __CSV_HPP__