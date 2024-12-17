#include "derevo.h"
namespace MonteKarlo {
    VMul3 ZERO=VMul3();
    VMul3 POLE=VMul3("init");
    Derevo::Derevo(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq, bool fflag, Derevo *ppred, unsigned long long int gen_key) {
        pl = ppred->get_pole() - VMul3(uu, vv, qq);
        flag = fflag;
        pred = ppred;
        if (ppred != nullptr) {
            depth = ppred->depth + 1;
        } else {
            depth = 0;
        }
        value = VALUE;
        ++VALUE;
        generation_key=gen_key;
    }
    Derevo::~Derevo() {
        // Очистка множества sled, если оно содержит указатели
        for (auto elem : sled) {
            delete elem; // Рекурсивное удаление дочерних элементов
        }
        sled.clear();
        // Очистка памяти для pred (если это необходимо)
        pred = nullptr; // Указатель pred не владеет объектом, предполагаем nullptr
    }
    Derevo::Derevo(bool fflag, Derevo *ppred) {
        pl=VMul3("init");
        flag = fflag;
        pred = ppred;
        if (ppred != nullptr) {
            depth = ppred->depth + 1;
        } else {
            depth = 0;
        }
        value = VALUE;
        ++VALUE;
    }
//    bool Derevo::CompareById::operator()(const Derevo* lhs, const Derevo* rhs) const {
//        return lhs->value < rhs->value;
//    }
    Derevo *Derevo::sled_back() {
        return *(--sled.end());
    }

    bool Derevo::set_w(double a) {
        return w = a;
    }

    double Derevo::get_w() {
        return w;
    }
    unsigned long long int Derevo::get_value() const {
        return value;
    }
    std::vector<Derevo *> Derevo::get_sled() const {
        return sled;
    }

    VMul3 Derevo::get_pole() {
        return pl;
    }

    bool Derevo::set_flag(bool a) {
        return flag = a;
    }

    Derevo *Derevo::get_pred() {
        return pred;
    }

    unsigned long long int Derevo::get_depth() {
        return depth;
    }

    unsigned long long int Derevo::get_sled_size() {
        return sled.size();
    }

    double Derevo::get_n() const{
        return n;
    }

    bool Derevo::set_n(double a) {
        return n = a;
    }

    void Derevo::set_sled(Derevo *a) {
        sled.push_back(a);
    }


    bool Derevo::get_flag() {
        return flag;
    }

    double Derevo::get_wn() const{
        return w / n;
    }

    double Derevo::fun() const{
        return get_wn()+C*sqrt(log(T)/get_n());
    }
    bool Derevo::operator<(const Derevo* other) const{
        if (fun()>other->fun()){
            return true;
        }
        return false;
    }
    std::vector<vmtype> Derevo::out(){
        std::vector<vmtype> m(LIM_POINT,0);
        for (auto i: sled){
            m[i->generation_key]=i->get_wn();
        }
        return m;
    }
    bool Derevo::set_value(unsigned long long int a){
        value=a;
        return true;
    }
    unsigned long long int Derevo::find_max_index(){

        return tbb::parallel_reduce(
             tbb::blocked_range<size_t>(0, sled.size()),
             std::make_pair(sled[0]->fun(), 0),
             [&](const tbb::blocked_range<size_t>& range, std::pair<double, unsigned long long int> local_max) {
                 for (size_t i = range.begin(); i < range.end(); ++i) {
                     double ans=sled[i]->fun();
                     if (local_max.first<ans){
                         local_max.first=ans;
                         local_max.second=i;
                     }
                 }
                 return local_max;
             },
             [](std::pair<double, unsigned long long int> x, std::pair<double, unsigned long long int> y) {
                 if (x.first<y.first){
                     return y;
                 }
                 return x;
             }
         ).second;
    }

    void Derevo::set_pl(unsigned long long int i, unsigned long long int j, unsigned long long int k, vmtype val) {
        pl.set_v(i,j,k,val);
    }

    bool Derevo::set_depth(unsigned long long int a) {
        depth=a;
        return true;
    }

    Derevo *Derevo::get_sled_elem(unsigned long long int index) {
        return sled[index];
    }
}
//
// Created by hasni on 07.10.2024.
//
