#include "vmul3.h"
namespace MonteKarlo {
    double const N=2;
    const unsigned long long int SN=N*N;
    long long int SN3=SN*SN*SN;
    unsigned long long int MOD_N=pow(3.0,SN);
    long long int LIM_POINT=pow(3,SN*3);
    unsigned long long int LIM_HOD=N*N*3;
    double C=sqrt(2);
    unsigned long long int VALUE=0;
    VMul3::VMul3() {
        v = std::vector<std::vector<std::vector<vmtype>>>(SN, std::vector<std::vector<vmtype>>(SN,
                                                                                           std::vector<vmtype>(SN, 0)));
    }

    VMul3::VMul3(std::string type) {
        v = std::vector<std::vector<std::vector<vmtype>>>(SN, std::vector<std::vector<vmtype>>(SN,
                                                                                           std::vector<vmtype>(SN, 0)));
        if (type == "init") {
            pole_init();
        }
    }

    void VMul3::pole_init() {
        for (unsigned long long int i = 0; i < N; ++i) {
            for (unsigned long long int j = 0; j < N; ++j) {
                for (unsigned long long int k = 0; k < N; ++k) {
                    v[i * N + k][k * N + j][i * N + j] = 1;
                }
            }

        }
    }

    bool VMul3::operator==(const VMul3 &b) {
        VMul3 ans = *this;
        for (unsigned long long int i = 0; i < v.size(); ++i) {
            for (unsigned long long int j = 0; j < v.size(); ++j) {
                for (unsigned long long int k = 0; k < v.size(); ++k) {
                    if (ans.v[i][j][k] != b.v[i][j][k]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    VMul3 VMul3::operator-(const VMul3 &b) {
        VMul3 ans = *this;
        for (unsigned long long int i = 0; i < v.size(); ++i) {
            for (unsigned long long int j = 0; j < v.size(); ++j) {
                for (unsigned long long int k = 0; k < v.size(); ++k) {
                    ans.v[i][j][k] -= b.v[i][j][k];
                }
            }
        }
        return ans;
    }

    VMul3::VMul3(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq) {
        v = std::vector<std::vector<std::vector<vmtype>>>(SN, std::vector<std::vector<vmtype>>(SN,
                                                                                           std::vector<vmtype>(SN, 0)));
        for (unsigned long long int i = 0; i < uu.size(); ++i) {
            for (unsigned long long int j = 0; j < vv.size(); ++j) {
                for (unsigned long long int k = 0; k < qq.size(); ++k) {
                    v[i][j][k] = uu[i] * vv[j] * qq[k];
                }
            }
        }
    }
    std::vector<std::vector<std::vector<vmtype>>> VMul3::get_value(){
        return v;
    }
    std::vector<vmtype> VMul3::get_data(){
        std::vector<vmtype> ans;
        for (unsigned long long int i=0;i<v.size();++i){
            for (unsigned long long int j=0;j<v[0].size();++j){
                ans.insert(ans.end(),v[i][j].begin(),v[i][j].end());
            }
        }
        return ans;
    }
    void VMul3::print() {
        for (unsigned long long int i=0;i<v.size();++i){
            for (unsigned long long int j=0;j<v[0].size();++j){
                for (unsigned long long int k=0;k<v[0][0].size();++k){
                    std::cout<<v[i][j][k]<<" ";
                }
                std::cout<<std::endl;
            }
            std::cout<<"-------------------"<<std::endl;
        }
        std::cout<<"////////////////////////"<<std::endl;
    }
}
//
// Created by hasni on 07.10.2024.
//
