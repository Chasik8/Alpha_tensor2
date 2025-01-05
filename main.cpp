#include "main.h"
namespace MonteKarlo {

    double Game::fun_zero(double T) {
        return C * sqrt(log(T));
    }

    Game::Game() {
        optimizer = std::make_shared<torch::optim::Adam>(net->parameters(), torch::optim::AdamOptions(0.001));
        net->to(device);
        if (torch::cuda::is_available()) {
            std::cout << "CUDA is available! Training on GPU.\n";
            device = torch::kCUDA;
        } else {
            std::cout << "Training on CPU.\n";
        }
        if (!std::filesystem::exists(path_global_dir)) {
            // Если папка не существует, создаём её
            if (std::filesystem::create_directory(path_global_dir)) {
                std::cout << "Папка успешно создана. " + path_global_dir.string() << std::endl;
            }
        }
        const std::string dirname=std::to_string(N)+"_"+std::to_string(SN)+"_"+std::to_string(C)+"_"+std::to_string(LIM_HOD);
        const std::filesystem::path path_dirname=path_global_dir/std::filesystem::path(dirname);
        if (!std::filesystem::exists(path_dirname)) {
            // Если папка не существует, создаём её
            if (std::filesystem::create_directory(path_dirname)) {
                std::cout << "Папка успешно создана. " + path_dirname.string() << std::endl;
            }
        }
        if (std::filesystem::exists(path_dirname/path_conf_iter)) {
            std::ifstream inp(path_dirname / path_conf_iter, std::ios::in);
            std::string dops;
            inp >> dops;
            conf_iter = std::atoi(dops.c_str());
            inp.close();
        }
        std::ofstream out(path_dirname / path_conf_iter, std::ios::out);
        out << conf_iter+1;
        out.close();

        netname = "net"+std::to_string(conf_iter)+".pt";
        dataname = "data"+std::to_string(conf_iter)+".mk";
        netname_new = "net"+std::to_string(conf_iter+1)+".pt";
        dataname_new = "data"+std::to_string(conf_iter+1)+".mk";
        path_netname=path_dirname/std::filesystem::path(netname);
        path_dataname=path_dirname/std::filesystem::path(dataname);
        path_netname_new=path_dirname/std::filesystem::path(netname_new);
        path_dataname_new=path_dirname/std::filesystem::path(dataname_new);
    }

    void Game::back(Derevo *derevo, char hod) {
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

    void Game::clear(Derevo *kor) {
        for (auto i: kor->get_sled()){
            clear(i);
            delete i;
        }
    }
    void Game::algorithm(Derevo *kor) {
        Derevo *derevo = kor;
        while (derevo->get_sled_size() > 0) {
            std::pair<double, unsigned long long int> dop_pr=derevo->find_max_index();
            if (dop_pr.first>dval->fun()) {
                derevo = derevo->get_sled_elem(dop_pr.second);
            }else{
                break;
            }
        }
        std::pair<Derevo *, char> pr = mod(derevo);
        derevo = pr.first;
        back(derevo, pr.second);
    }


    void Game::train(torch::Tensor &data, torch::Tensor &target) {
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
    }

    std::pair<Derevo *, char> Game::mod(Derevo *derevo) {
        char p = if_game(derevo);
        bool flag = true;
        while (p == 0 && flag) {
            unsigned long long int dop = win(derevo);
            auto pr_dop= derevo->find_new(dop);
            std::pair<bool, Derevo *> pr;
            if (!pr_dop.first) {
                pr = nw(derevo, dop);
            }else{
                pr = nw(derevo, pr_dop.second);
                std::cout<<"нейронка совпала"<<dop<<" "<<pr_dop.second<<" \n";
            }
            derevo = pr.second;
            flag = pr.first;
            p = if_game(derevo);
        }
        return std::pair<Derevo *, char>(derevo, p);
    }

    int64_t Game::win(Derevo *derevo) {
        torch::Tensor data = torch::from_blob(derevo->get_pole().get_data().data(), {static_cast<long>(SN3)},
                                              torch::kFloat32).clone();
        data = data.to(device);
        auto pred = net->forward(data);
        torch::Tensor max_index = torch::argmax(pred);
        return max_index.item<int64_t>();
    }

    char Game::if_game(Derevo *derevo) {
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

    std::pair<bool, Derevo *> Game::nw(Derevo *derevo, unsigned long long int dop) {
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

    std::vector<vmtype> Game::product(unsigned long long int n) {
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

    void Game::run() {

        unsigned long long int epoch_kol = 3;
        std::cout<<"Введите количество эпох: ";
        std::cin>>epoch_kol;
        unsigned long long int epoch_pred =0;
        Derevo *kor;
        if (conf_iter==-1) {
            kor = new Derevo(true, nullptr);
        }else {
            kor=readmk(epoch_pred);
        }
        bool size_kor=true;
        for (unsigned long long int epoch = epoch_pred;epoch<epoch_kol+epoch_pred;++epoch,++T) {
            if (epoch%100==0) {
                std::cout << epoch << std::endl;
            }
            if (size_kor&&kor->get_sled_size()==LIM_POINT){
                std::cout<<"size_kor\n";
                size_kor=false;
            }
            algorithm(kor);
            if (epoch%10000==0) {
                save(kor,epoch+epoch_pred);
            }
        }
        std::cout << save(kor,epoch_kol+epoch_pred) << std::endl;
        std::cout << VALUE << std::endl;
    }
}
int main() {
    std::srand(std::time(nullptr));
    MonteKarlo::Game game;
    double tim = clock();
    game.run();
    std::cout<<std::endl<<(clock()-tim)/1000<<std::endl;
    return 0;
}
