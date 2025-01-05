////
//// Created by hasni on 01.12.2024.
////
#include "main.h"
namespace MonteKarlo {
    Derevo* Game::read(Derevo* kor,std::ifstream& inFile){
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
    Derevo* Game::readmk(unsigned long long int& epoch_kol) {
        torch::load(net, path_netname.string());
        Derevo* kor= nullptr;
        std::ifstream inFile(path_dataname, std::ios::in);
        inFile >>epoch_kol >> N >> SN >> MOD_N >> LIM_POINT >> LIM_HOD >> C >> VALUE;
        kor=read(kor, inFile);
        inFile.close();
        return kor;
    }
}
