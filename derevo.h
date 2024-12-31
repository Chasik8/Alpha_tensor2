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
        VMul3 pl;
        Derevo *pred;
        std::vector<Derevo*> sled;
        bool flag;
        unsigned long long int value;
        unsigned long long int depth;
        unsigned long long int generation_key;
    public:
        Derevo(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq, bool fflag, Derevo *ppred, unsigned long long int gen_key);

        Derevo(bool fflag, Derevo *ppred);

        ~Derevo();

        void set_pl(unsigned long long int i,unsigned long long int j,unsigned long long int k, vmtype val);

        Derevo *sled_back();

        bool set_w(double a);

        bool set_n(double a);

        bool set_flag(bool a);

        bool set_depth(unsigned long long int a);

        void set_sled(Derevo *a);

        Derevo * get_sled_elem(unsigned long long int index);

        bool get_flag();

        unsigned long long int get_depth();

        unsigned long long int get_sled_size();

        Derevo *get_pred();

        double get_w();

        double get_n() const;

        double get_wn() const;

        unsigned long long int get_value() const;

        std::vector<Derevo *> get_sled() const;

        VMul3 get_pole();

        bool operator<(const Derevo* other) const;

        double fun() const;

        std::vector<vmtype> out();

        bool set_value(unsigned long long int a);

        std::pair<double, unsigned long long int> Derevo::find_max_index();

        std::pair<bool, unsigned long long int> Derevo::find_new(unsigned long long int target);
    };
}
#endif //ALPA_TENSOR_DEREVO_H
