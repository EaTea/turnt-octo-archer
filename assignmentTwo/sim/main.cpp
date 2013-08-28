#include "sim.h"

int main()
{
    std::ifstream       file("plop.csv");

    for(CSVIterator loop(file);loop != CSVIterator();++loop)
    {
        std::cout << "4th Element(" << (*loop)[3] << ")\n";
    }
}
