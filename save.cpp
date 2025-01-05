//
// Created by hasni on 05.01.2025.
//
#include "main.h"
namespace MonteKarlo {
    unsigned long long int Game::save(Derevo *derevo, unsigned long long int epoch_kol) {
        torch::save(net, path_netname_new.string());
        std::ofstream outFile(path_dataname_new, std::ios::out);
        if (!outFile) {
            std::cerr << "Ошибка открытия файла для записи!" << std::endl;
            return -1;
        }
        outFile << epoch_kol << " " << N << " " << SN << ' ' << MOD_N << ' ' << LIM_POINT << ' ' << LIM_HOD << ' ' << C
                << ' ' << VALUE
                << ' ';
        sv(derevo, outFile);
        outFile.close();
        return 0;
    }

    void Game::sv(Derevo *derevo, std::ofstream &outFile) {
        outFile << derevo->get_n() << " " << derevo->get_w() << ' ' << derevo->get_flag() << ' '
                << derevo->get_value() << ' '
                << derevo->get_depth() << ' ';
        for (unsigned long long int i = 0; i < SN; ++i) {
            for (unsigned long long int j = 0;j < SN; ++j) {
                for (unsigned long long int k = 0; k < SN; ++k) {
                    outFile << derevo->get_pole().get_value()[i*SN2+j*SN+k] << ' ';
                }
            }
        }
        outFile << derevo->get_sled_size() << ' ';
        for (auto i: derevo->get_sled()) {
            sv(i,outFile);
        }
    }
}
