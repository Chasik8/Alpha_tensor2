#include <torch/torch.h>
struct Net : torch::nn::Module {
    torch::nn::Linear fcc{nullptr};
    Net() {
//        std::cout<<MonteKarlo::SN3<<" "<<MonteKarlo::LIM_POINT<<" "<<std::endl;
//        assert(MonteKarlo::SN3 > 0 && MonteKarlo::LIM_POINT > 0);
//            fcc = register_module("fcc", torch::nn::Linear(MonteKarlo::SN3, MonteKarlo::LIM_POINT));
        fcc = register_module("fcc", torch::nn::Linear(8, 15));
    }
    torch::Tensor forward(torch::Tensor x) {
        //std::cout<<x.sizes()<<std::endl;
        x = fcc->forward(x);
        std::cout<<x.sizes()<<std::endl;
        x = torch::relu(x);
        x = torch::softmax(x, 0, torch::kFloat32);
        return x;
    }
};
//
// Created by hasni on 12.10.2024.
//
