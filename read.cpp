////
//// Created by hasni on 01.12.2024.
////
#include "read.h"
namespace MonteKarlo {
    Derevo* read(Derevo* kor,std::ifstream& inFile){
        double n,w;
        bool flag;
        unsigned long long int value;
        unsigned long long int depth;
        inFile >> n >> w >> flag
               >> value
               >> depth;
        Derevo* derevo=new Derevo(false,kor);
        if (kor!= nullptr) {
            kor->set_sled(derevo);
        }
        derevo->set_n(n);
        derevo->set_w(w);
        derevo->set_flag(flag);
        derevo->set_value(value);
        derevo->set_depth(depth);
        for (unsigned long long int i = 0; i < SN; ++i) {
            for (unsigned long long int j = 0;j < SN; ++j) {
                for (unsigned long long int k = 0; k < SN; ++k) {
                    vmtype val;
                    inFile >> val;
                    derevo->set_pl(i,j,k,val);
                }
            }
        }
        unsigned long long int kol;
        inFile>>kol;
        for (unsigned long long int i=0;i<kol;++i) {
            read(derevo, inFile);
        }
        return derevo ;
    }
    Derevo* readmk(unsigned long long int& epoch_kol) {
        Derevo* kor= nullptr;
        const std::string &filename = "data2.mk";
        std::ifstream inFile(filename, std::ios::in);
        inFile >>epoch_kol >> N >> SN >> MOD_N >> LIM_POINT >> LIM_HOD >> C >> VALUE;
        kor=read(kor, inFile);
        inFile.close();
        return kor;
    }
}
