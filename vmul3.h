//
// Created by hasni on 07.10.2024.
//
#include <iostream>
#include <vector>
#include <cmath>
#include <tbb/tbb.h>
//#include <tbb/parallel_for.h>
//#include <tbb/parallel_reduce.h>
//#include <tbb/blocked_range.h>
//#include <tbb/blocked_range2d.h>
//#include <tbb/global_control.h>
#include <optional>
#ifndef ALPA_TENSOR_VMUL3_H
#define ALPA_TENSOR_VMUL3_H
namespace MonteKarlo{
    extern double N;
    extern unsigned long long int SN;
    extern unsigned long long int SN3;
    extern unsigned long long int SN2;
    extern unsigned long long int MOD_N;
    extern long long int LIM_POINT;
    extern unsigned long long int LIM_HOD;
    extern double C;
    extern unsigned long long int VALUE;
    extern unsigned long long int T;
    typedef float vmtype;
    class VMul3 {
    private:
        std::vector<vmtype> v;
        void pole_init();
    public:
        VMul3();

        ~VMul3();

        explicit VMul3(std::string type);

        std::vector<vmtype> get_value();

        VMul3(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq);

        VMul3 operator-(const VMul3 &b);

        bool operator==(const VMul3 &b);

        void print();

        std::vector<vmtype> VMul3::get_data();

        void set_v(unsigned long long int i,unsigned long long int j,unsigned long long int k, vmtype val);
    };

};
#endif //ALPA_TENSOR_VMUL3_H
