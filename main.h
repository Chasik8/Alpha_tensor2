//
// Created by hasni on 04.10.2024.
//
#include <cstdlib>
#include <string>
#include <ctime>
#include <stack>
#include "derevo.h"
#include <torch/torch.h>
#include <fstream>
#include <filesystem>
#include "model.h"
#ifndef ALPA_TENSOR_MAIN_H
#define ALPA_TENSOR_MAIN_H
namespace MonteKarlo {
    class Game {
    private:
        std::shared_ptr<Net> net = std::make_shared<Net>();
        torch::Device device = torch::kCUDA;
        std::shared_ptr<torch::optim::Adam> optimizer;
        Derevo* dval=new Derevo(false, nullptr);

        const std::string global_dir="solution";
        const std::filesystem::path path_global_dir=std::filesystem::path(global_dir);
        unsigned long long int conf_iter=-1;
        std::string netname;
        std::string dataname;
        std::string netname_new;
        std::string dataname_new;
        std::filesystem::path path_netname;
        std::filesystem::path path_dataname;
        std::filesystem::path path_netname_new;
        std::filesystem::path path_dataname_new;
        std::filesystem::path path_conf_iter=std::filesystem::path("conf.txt");
    public:
        void run();

        Derevo* read(Derevo* kor,std::ifstream& inFile);

        Derevo* readmk(unsigned long long int& epoch_kol);

        double fun_zero(double T);

        std::vector<vmtype> product(unsigned long long int n);

        std::pair<bool, Derevo *> nw(Derevo *derevo, unsigned long long int dop);

        char if_game(Derevo *derevo);

        int64_t win(Derevo *derevo);

        std::pair<Derevo *, char> mod(Derevo *derevo);

        void train(torch::Tensor &data, torch::Tensor &target);

        void back(Derevo *derevo, char hod);

        void algorithm(Derevo *kor);

        unsigned long long int save(Derevo *derevo, unsigned long long int epoch_kol);

        void sv(Derevo *derevo, std::ofstream& outFile);

        void clear(Derevo *kor);
        Game();
    };
};
#endif //ALPA_TENSOR_MAIN_H
