#include <cstdlib>
#include <string>
#include <ctime>
#include <fstream>
#include <stack>
#include "main.h"
#include "derevo.h"
#include <torch/torch.h>
#include "model.cpp"
namespace MonteKarlo {


    double fun_zero(double T){
        return C * sqrt(log(T));
    }
    std::vector<vmtype> product(unsigned long long int n){
        std::vector<vmtype> m(SN,0);
        unsigned long long int k;
        if (n!=0) {
            k = SN-1;
            while (n) {
                m[k] = n % 3;
                --m[k];
                n /= 3;
                --k;
            }
        }
        return m;
    }
    std::pair<bool,Derevo*> nw(Derevo* derevo,unsigned long long int dop){
        bool flag= true;
        if (derevo->get_depth()==LIM_HOD-1){
            flag = false;
        }
        unsigned long long int l3=dop%MOD_N;
        unsigned long long int l2=(dop/MOD_N)%MOD_N;
        unsigned long long int l1=(dop/MOD_N)/MOD_N;
        std::vector<vmtype> u=product(l1);
        std::vector<vmtype> v=product(l2);
        std::vector<vmtype> q=product(l3);
        derevo->set_setrd(dop);
        Derevo* dopd=new Derevo(u,v,q,flag,derevo, dop);
        derevo->set_sled(dopd);
        return std::pair<bool,Derevo*>(flag,dopd);
    }
    char if_game(Derevo* derevo){
        if (derevo->get_depth()<LIM_HOD){
            if (derevo->get_pole()==ZERO){
                derevo->set_flag(false);
                return 1;
            }else{
                return 0;
            }
        }
        return 2;
    }
    unsigned long long int win(){
        return rand()%LIM_POINT;
    }
    std::pair<Derevo*,char> mod(Derevo* derevo){
        char p=if_game(derevo);
        bool flag=true;
        while (p==0&&flag){
            unsigned long long int dop=win();
            if (!derevo->find_setrd(dop)){
                std::pair<bool,Derevo*> pr=nw(derevo,dop);
                derevo=pr.second;
                flag=pr.first;
            }
            p= if_game(derevo);
        }
        return std::pair<Derevo*,char>(derevo,p);
    }
    void train(std::shared_ptr<Net> net, torch::optim::Adam& optimizer, torch::Tensor& data, torch::Tensor& target){
        // Reset gradients.
        optimizer.zero_grad();
        // Execute the model on the input data.
        torch::Tensor prediction = net->forward(data);
        // Compute a loss value to judge the prediction of our model.
        auto lossfn = torch::nn::MSELoss();
        auto loss=lossfn(prediction,target);
        // Compute gradients of the loss w.r.t. the parameters of our model.
        loss.backward();
        // Update the parameters based on the calculated gradients.
        optimizer.step();
//        std::cout<<loss.item()<<std::endl;
        // Output the loss and checkpoint every 100 batches.
        torch::save(net, "D:\\Project\\C++\\CLion\\Alpha_tensor2\\model\\net.pt");
    }
    void back(Derevo* derevo,char hod, std::shared_ptr<Net> net, torch::optim::Adam& optimizer, torch::Device& device){
        double kef=0;
        if (hod==1){
            kef=1.0/(double)derevo->get_depth();
        }
        while (derevo->get_pred()!= nullptr){
            derevo->set_w(derevo->get_w() + kef);
            derevo->set_n(derevo->get_n()+1.0);
            if (derevo->get_sled_size()>0) {
                torch::Tensor data = torch::from_blob(derevo->get_pole().get_data().data(), {static_cast<long>(SN3)}, torch::kFloat32).clone();
                torch::Tensor target =  torch::from_blob(derevo->out().data(), {static_cast<long>(LIM_POINT)}, torch::kFloat32).clone();//.view({1, LIM_POINT})
                target=torch::softmax(target,0, torch::kFloat32);
                target=target.to(device);
                data=data.to(device);
                train(net, optimizer, data, target);
            }

            derevo=derevo->get_pred();
        }
    }
    void algorithm(Derevo* kor,std::shared_ptr<Net> net, torch::optim::Adam& optimizer, torch::Device& device){
        Derevo* derevo=kor;
        while (derevo->get_sled_size()>0){
            unsigned long long int dop=rand()%LIM_POINT;
            if (derevo->get_sled_size()>=dop){
                derevo=derevo->sled_back();
            }else if (derevo->get_flag()){
                unsigned long long int dop1=rand()%LIM_POINT;
                while (derevo->find_setrd(dop1)){
                    dop1=rand()%LIM_POINT;
                }
                derevo = nw(derevo,dop1).second;
            }else{
                break;
            }
        }
        std::pair<Derevo*,char>pr=mod(derevo);
        derevo=pr.first;
//        ++VALUE;
        back(derevo,pr.second, net, optimizer, device);
    }
    unsigned long long int save(Derevo* kor) {
        const std::string& filename="data1.mk";
        std::ofstream outFile(filename, std::ios::out);
        if (!outFile) {
            std::cerr << "Ошибка открытия файла для записи!" << std::endl;
            return -1;
        }
        outFile<<N<<" "<<SN<<' '<<MOD_N<<' '<<LIM_POINT<<' '<<LIM_HOD<<' '<<C<<' '<<VALUE<<' ';
        std::stack<Derevo*> st;
        st.push(kor);
        unsigned long long int kol=0;
        for (;!st.empty();++kol){
            Derevo* derevo=st.top();
            st.pop();
            outFile<<derevo->get_n()<<" "<<derevo->get_w()<<' '<<derevo->get_flag()<<' '<<derevo->get_value()<<' '
                   <<derevo->get_depth()<<' ';
            for (unsigned long long int i=0;i<SN;++i){
                outFile<<derevo->get_u()[i]<<' ';
            }
            for (unsigned long long int i=0;i<SN;++i){
                outFile<<derevo->get_v()[i]<<' ';
            }
            for (unsigned long long int i=0;i<SN;++i){
                outFile<<derevo->get_q()[i]<<' ';
            }
            outFile<<derevo->get_sled_size()<<' ';
            for (auto i:derevo->get_setrd()){
                outFile<<i<<' ';
            }
            for (auto i:derevo->get_sled()){
                st.push(i);
            }
        }
        outFile.close();
        return kol;
    }
    void game(){
        std::shared_ptr<Net> net = std::make_shared<Net>();
//        Net net=new Net();
        torch::Device device = torch::kCUDA;
        //torch::Device device = torch::cuda::is_available() ? torch::Device(torch::kCUDA) : torch::Device(torch::kCPU);
        net->to(device);
        if (torch::cuda::is_available()) {
            std::cout << "CUDA is available! Training on GPU.\n";
            device = torch::kCUDA;
        } else {
            std::cout << "Training on CPU.\n";
        }
        torch::optim::Adam optimizer(net->parameters());

        Derevo* kor=new Derevo(true, nullptr);
        unsigned long long int epoch_kol = 1e1;
        for (unsigned long long int epoch=0;epoch<epoch_kol;++epoch){
            std::cout<<epoch<<std::endl;
            algorithm(kor, net, optimizer, device);
        }
        std::cout<<save(kor)<<std::endl;
    }
}
int main() {
    std::srand(std::time(nullptr));
    double tim= clock();
    MonteKarlo::game();
    std::cout<<std::endl<<(clock()-tim)/1000<<std::endl;
    return 0;
}
