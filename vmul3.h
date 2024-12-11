//
// Created by hasni on 07.10.2024.
//
#include <iostream>
#include <vector>
#include <cmath>
#ifndef ALPA_TENSOR_VMUL3_H
#define ALPA_TENSOR_VMUL3_H
namespace MonteKarlo{
    extern double N;
    extern unsigned long long int SN;
    extern long long int SN3;
    extern unsigned long long int MOD_N;
    extern long long int LIM_POINT;
    extern unsigned long long int LIM_HOD;
    extern double C;
    extern unsigned long long int VALUE;
    typedef float vmtype;
    class VMul3 {
    private:
        std::vector<std::vector<std::vector<vmtype>>> v;
        void pole_init();
    public:
        VMul3();

        explicit VMul3(std::string type);

        std::vector<std::vector<std::vector<vmtype>>> get_value();

        VMul3(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq);

        VMul3 operator-(const VMul3 &b);

        bool operator==(const VMul3 &b);

        void print();

        std::vector<vmtype> VMul3::get_data();

        void set_v(unsigned long long int i,unsigned long long int j,unsigned long long int k, vmtype val);
    };

};
#endif //ALPA_TENSOR_VMUL3_H
