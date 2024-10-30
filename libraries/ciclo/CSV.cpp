#include "CSV.hpp"

String CSV::to_csv(String nome1, double *dado1, String nome2, int *dado2, int quantidade) {
    String csv = "";
    csv = nome1 + "," + nome2 + "\n";
    for ( int i = 0 ; i < quantidade ; i++ ) {
        csv += String(dado1[i]) + "," + String(dado2[i]) + "\n";
    }
    return csv;
}