//
// Created by hasni on 10.12.2024.
//

#ifndef ALPHA_TENSOR2_READ_H
#define ALPHA_TENSOR2_READ_H
#include "main.h"
namespace MonteKarlo {
    Derevo* read(Derevo* kor,std::ifstream& inFile);
    Derevo* readmk(unsigned long long int& epoch_kol);
}
#endif //ALPHA_TENSOR2_READ_H
