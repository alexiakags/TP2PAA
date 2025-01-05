#include "../include/caverna.h"

int main() {
    Caverna *caverna = carregarArquivo("input/caverna1.txt");
    imprimirCaverna(caverna);
    
    resolverCaverna(caverna, "./output/naoPeide.txt");
    liberarCaverna(caverna);
    return 0;
}