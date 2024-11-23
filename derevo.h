//
// Created by hasni on 04.10.2024.
//
#include <set>
#include "vmul3.h"
#ifndef ALPA_TENSOR_DEREVO_H
#define ALPA_TENSOR_DEREVO_H
namespace MonteKarlo {
    extern VMul3 ZERO;
    extern VMul3 POLE;
    class Derevo {
    private:
        double n = 1;
        double w = 0;
        std::vector<vmtype> u;
        std::vector<vmtype> v;
        std::vector<vmtype> q;
        Derevo *pred;
        std::set<Derevo *> sled;
        std::set<unsigned long long int> setrd;
        bool flag;
        unsigned long long int value;
        unsigned long long int depth;
        unsigned long long int generation_key;
    public:
        Derevo(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq, bool fflag, Derevo *ppred, unsigned long long int gen_key);

        Derevo(bool fflag, Derevo *ppred);

        Derevo *sled_back();

        bool find_setrd(unsigned long long int a);

        void set_setrd(unsigned long long int a);

        bool set_w(double a);

        bool set_n(double a);

        bool set_flag(bool a);

        void set_sled(Derevo *a);

        bool get_flag();

        unsigned long long int get_depth();

        unsigned long long int get_sled_size();

        Derevo *get_pred();

        double get_w();

        double get_n() const;

        double get_wn() const;

        unsigned long long int get_value() const;

        std::set<Derevo *> get_sled() const;

        std::set<unsigned long long int> get_setrd() const;

        std::vector<vmtype> get_u();

        std::vector<vmtype> get_v();

        std::vector<vmtype> get_q();

        VMul3 get_pole();

        bool operator<(const Derevo* other) const;

        double fun(double T) const;

        std::vector<vmtype> out();
    };
}
#endif //ALPA_TENSOR_DEREVO_H
