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

    Derevo *Derevo::sled_back() {
        return *(--sled.end());
    }

    bool Derevo::find_setrd(unsigned long long int a) {
        return setrd.find(a) != setrd.end();
    }

    void Derevo::set_setrd(unsigned long long int a) {
        setrd.insert(a);
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
    std::set<Derevo *> Derevo::get_sled() const {
        return sled;
    }
    std::set<unsigned long long int> Derevo::get_setrd() const {
        return setrd;
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
        sled.insert(a);
    }


    bool Derevo::get_flag() {
        return flag;
    }

    double Derevo::get_wn() const{
        return w / n;
    }

    double Derevo::fun(double T) const{
        return get_wn()+C*sqrt(log(T)/get_n());
    }
    bool Derevo::operator<(const Derevo* other) const{
        if (fun((double)VALUE)>other->fun((double)VALUE)){
            return true;
        }
        return false;
    }
    std::vector<vmtype> Derevo::out(){
//        if (sled.empty()){
//            return std::vector<char>(0,0);
//        }
        std::vector<vmtype> m(LIM_POINT,0);
        for (auto i: sled){
            m[i->generation_key]=i->get_wn();
        }
        return m;
    }
}
//
// Created by hasni on 07.10.2024.
//
