#include <torch/torch.h>
#include "vmul3.h"

    struct Net : torch::nn::Module {
        torch::nn::Linear fcc{nullptr};
        Net() {
            assert(MonteKarlo::SN3 > 0 && MonteKarlo::LIM_POINT > 0);
            fcc = register_module("fcc", torch::nn::Linear(MonteKarlo::SN3, MonteKarlo::LIM_POINT));
        }
        torch::Tensor forward(torch::Tensor x) {
            x = fcc->forward(x);
            x = torch::relu(x);
            x = torch::softmax(x, 0, torch::kFloat32);
            return x;
        }
    };
//
// Created by hasni on 12.10.2024.
//
