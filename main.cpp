#include "main.h"
namespace MonteKarlo {
    class Game {
    private:
        std::shared_ptr<Net> net = std::make_shared<Net>();
        torch::Device device = torch::kCUDA;
        std::shared_ptr<torch::optim::Adam> optimizer;
        Derevo* dval=new Derevo(false, nullptr);
    public:
        double fun_zero(double T) {
            return C * sqrt(log(T));
        }

        std::vector<vmtype> product(unsigned long long int n) {
            std::vector<vmtype> m(SN, 0);
            unsigned long long int k;
            if (n != 0) {
                k = SN - 1;
                while (n) {
                    m[k] = n % 3;
                    --m[k];
                    n /= 3;
                    --k;
                }
            }
            return m;
        }

        std::pair<bool, Derevo *> nw(Derevo *derevo, unsigned long long int dop) {
            bool flag = true;
            if (derevo->get_depth() == LIM_HOD - 1) {
                flag = false;
            }
            unsigned long long int l3 = dop % MOD_N;
            unsigned long long int l2 = (dop / MOD_N) % MOD_N;
            unsigned long long int l1 = (dop / MOD_N) / MOD_N;
            std::vector<vmtype> u = product(l1);
            std::vector<vmtype> v = product(l2);
            std::vector<vmtype> q = product(l3);
            Derevo *dopd = new Derevo(u, v, q, flag, derevo, dop);
            derevo->set_sled(dopd);
            return std::pair<bool, Derevo *>(flag, dopd);
        }

        char if_game(Derevo *derevo) {
            if (derevo->get_depth() < LIM_HOD) {
                if (derevo->get_pole() == ZERO) {
                    derevo->set_flag(false);
                    return 1;
                } else {
                    return 0;
                }
            }
            return 2;
        }

        int64_t win(Derevo *derevo) {
            torch::Tensor data = torch::from_blob(derevo->get_pole().get_data().data(), {static_cast<long>(SN3)},
                                                  torch::kFloat32).clone();
            data = data.to(device);
            auto pred = net->forward(data);
            torch::Tensor max_index = torch::argmax(pred);
            return max_index.item<int64_t>();
        }

        std::pair<Derevo *, char> mod(Derevo *derevo) {
            char p = if_game(derevo);
            bool flag = true;
            while (p == 0 && flag) {
                unsigned long long int dop = win(derevo);
                dval->set_value(dop);
                if (!derevo->find_sled(dval)) {
                    std::pair<bool, Derevo *> pr = nw(derevo, dop);
                    derevo = pr.second;
                    flag = pr.first;
                }
                p = if_game(derevo);
            }
            return std::pair<Derevo *, char>(derevo, p);
        }

        void train(torch::Tensor &data, torch::Tensor &target) {
            // Reset gradients.
            optimizer->zero_grad();
            // Execute the model on the input data.
            torch::Tensor prediction = net->forward(data);
            // Compute a loss value to judge the prediction of our model.
            auto lossfn = torch::nn::MSELoss();
            auto loss = lossfn(prediction, target);
            // Compute gradients of the loss w.r.t. the parameters of our model.
            loss.backward();
            // Update the parameters based on the calculated gradients.
            optimizer->step();
//        std::cout<<loss.item()<<std::endl;
            // Output the loss and checkpoint every 100 batches.
            torch::save(net, "D:\\Project\\C++\\CLion\\Alpha_tensor2\\model\\net.pt");
        }

        void back(Derevo *derevo, char hod) {
            double kef = 0;
            if (hod == 1) {
                kef = 1.0 / (double) derevo->get_depth();
            }
            while (derevo->get_pred() != nullptr) {
                derevo->set_w(derevo->get_w() + kef);
                derevo->set_n(derevo->get_n() + 1.0);
                if (derevo->get_sled_size() > 0) {
                    torch::Tensor data = torch::from_blob(derevo->get_pole().get_data().data(),
                                                          {static_cast<long>(SN3)}, torch::kFloat32).clone();
                    torch::Tensor target = torch::from_blob(derevo->out().data(), {static_cast<long>(LIM_POINT)},
                                                            torch::kFloat32).clone();//.view({1, LIM_POINT})
                    target = torch::softmax(target, 0, torch::kFloat32);
                    target = target.to(device);
                    data = data.to(device);
                    train(data, target);
                }

                derevo = derevo->get_pred();
            }
        }

        void algorithm(Derevo *kor) {
            Derevo *derevo = kor;
            while (derevo->get_sled_size() > 0) {
                unsigned long long int dop = rand() % LIM_POINT;
                if (derevo->get_sled_size() >= dop) {
                    derevo = derevo->sled_back();
                } else if (derevo->get_flag()) {
                    unsigned long long int dop1 = rand() % LIM_POINT;
                    dval->set_value(dop1);
                    while (derevo->find_sled(dval)) {
                        dop1 = rand() % LIM_POINT;
                    }
                    derevo = nw(derevo, dop1).second;
                } else {
                    break;
                }
            }
            std::pair<Derevo *, char> pr = mod(derevo);
            derevo = pr.first;
//        ++VALUE;
            back(derevo, pr.second);
        }

        unsigned long long int save(Derevo *kor) {
            const std::string &filename = "data1.1.mk";
            std::ofstream outFile(filename, std::ios::out);
            if (!outFile) {
                std::cerr << "Ошибка открытия файла для записи!" << std::endl;
                return -1;
            }
            outFile << N << " " << SN << ' ' << MOD_N << ' ' << LIM_POINT << ' ' << LIM_HOD << ' ' << C << ' ' << VALUE
                    << ' ';
            std::stack<Derevo *> st;
            st.push(kor);
            unsigned long long int kol = 0;
            for (; !st.empty(); ++kol) {
                Derevo *derevo = st.top();
                st.pop();
                outFile << derevo->get_n() << " " << derevo->get_w() << ' ' << derevo->get_flag() << ' '
                        << derevo->get_value() << ' '
                        << derevo->get_depth() << ' ';
                for (unsigned long long int i = 0; i < SN; ++i) {
                    for (unsigned long long int j = 0;j < SN; ++j) {
                        for (unsigned long long int k = 0; k < SN; ++k) {
                            outFile << derevo->get_pole().get_value()[i][j][k] << ' ';
                        }
                    }
                }
                outFile << derevo->get_sled_size() << ' ';
                for (auto i: derevo->get_sled()) {
                    st.push(i);
                }
            }
            outFile.close();
            return kol;
        }

        Game() {
            optimizer = std::make_shared<torch::optim::Adam>(net->parameters(), torch::optim::AdamOptions(0.001));
            net->to(device);
            if (torch::cuda::is_available()) {
                std::cout << "CUDA is available! Training on GPU.\n";
                device = torch::kCUDA;
            } else {
                std::cout << "Training on CPU.\n";
            }

            Derevo *kor = new Derevo(true, nullptr);
            unsigned long long int epoch_kol = 3;
            for (unsigned long long int epoch = 0; epoch < epoch_kol; ++epoch) {
                std::cout << epoch << std::endl;
                algorithm(kor);
            }
            std::cout << save(kor) << std::endl;
        }
    };
}
int main() {
    std::srand(std::time(nullptr));
    double tim= clock();
    MonteKarlo::Game();
    std::cout<<std::endl<<(clock()-tim)/1000<<std::endl;
    return 0;
}
