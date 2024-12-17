#include "vmul3.h"

namespace MonteKarlo {
    double N=2;
    unsigned long long int SN=N*N;
    unsigned long long int SN3=SN*SN*SN;
    unsigned long long int SN2=SN*SN;
    unsigned long long int MOD_N=pow(3.0,SN);
    long long int LIM_POINT=pow(3,SN*3);
    unsigned long long int LIM_HOD=N*N*N*2;
    double C=sqrt(2);
    unsigned long long int VALUE=0;
    unsigned long long int T=0;
    VMul3::VMul3() {
        v =std::vector<vmtype>(SN3, 0);
    }
    VMul3::~VMul3() {
        v.clear();
    }
    VMul3::VMul3(std::string type) {
        v =std::vector<vmtype>(SN3, 0);
        if (type == "init") {
            pole_init();
        }
    }

    void VMul3::pole_init() {
        if (floor(N)==N) {
            for (unsigned long long int i = 0; i < N; ++i) {
                for (unsigned long long int j = 0; j < N; ++j) {
                    for (unsigned long long int k = 0; k < N; ++k) {
                        v[(i *N + k)*SN2+(k * N + j)*SN+i * N + j] = 1;
                    }
                }
            }
        }else {
            for (unsigned long long int i = 0; i < SN; ++i) {
                v[i*SN2+i*SN+i] =1;
            }
        }
    }

    bool VMul3::operator==(const VMul3 &b) {
        std::optional<size_t> result = tbb::parallel_reduce(
                tbb::blocked_range<size_t>(0, SN3),
                std::optional<size_t>{},
                [&](const tbb::blocked_range<size_t>& range, std::optional<size_t> local_result) {
                    for (size_t i = range.begin(); i < range.end(); ++i) {
                        if (v[i] == b.v[i]) {
                            return std::make_optional(i); // Найдено
                        }
                    }
                    return local_result; // Не найдено в этом диапазоне
                },
                [](std::optional<size_t> a, std::optional<size_t> b) -> std::optional<size_t> {
                    // Объединяем результаты
                    return a ? a : b;
                }
        );
        if (result==SN3){
            return true;
        }
        return false;
    }

    VMul3 VMul3::operator-(const VMul3 &b) {
        VMul3 ans = *this;
        tbb::parallel_for(tbb::blocked_range<size_t>(1, SN3), [&](const tbb::blocked_range<size_t>& range) {
            for (size_t i = range.begin(); i < range.end(); ++i) {
                ans.v[i] = v[i] - b.v[i - 1];
            }
        });
        return ans;
    }

    VMul3::VMul3(std::vector<vmtype> &uu, std::vector<vmtype> &vv, std::vector<vmtype> &qq) {
        v =std::vector<vmtype>(SN3, 0);
//        tbb::parallel_for(tbb::blocked_range<unsigned long long>(0, uu.size() * vv.size() * qq.size()),
//                          [&](const tbb::blocked_range<unsigned long long>& range) {
//                              for (unsigned long long idx = range.begin(); idx < range.end(); ++idx) {
//                                  unsigned long long i = idx / (vv.size() * qq.size());
//                                  unsigned long long j = (idx / qq.size()) % vv.size();
//                                  unsigned long long k = idx % qq.size();
//
//                                  v[i * SN2 + j * SN + k] = uu[i] * vv[j] * qq[k];
//                              }
//                          });
//        tbb::parallel_for(
//                tbb::blocked_range<unsigned long long>(0, uu.size()),  // Внешний цикл по i
//                [&](const tbb::blocked_range<unsigned long long>& range_i) {
//                    for (unsigned long long i = range_i.begin(); i < range_i.end(); ++i) {
//                        for (unsigned long long j = 0; j < vv.size(); ++j) {
//                            for (unsigned long long k = 0; k < qq.size(); ++k) {
//                                v[i * SN2 + j * SN + k] = uu[i] * vv[j] * qq[k];
//                            }
//                        }
//                    }
//                }
//        );
        tbb::parallel_for(
                tbb::blocked_range2d<unsigned long long>(0, uu.size(), 0, vv.size()),
                [&](const tbb::blocked_range2d<unsigned long long>& range) {
                    for (unsigned long long i = range.rows().begin(); i < range.rows().end(); ++i) {
                        for (unsigned long long j = range.cols().begin(); j < range.cols().end(); ++j) {
                            for (unsigned long long k = 0; k < qq.size(); ++k) {
                                v[i * SN2 + j * SN + k] = uu[i] * vv[j] * qq[k];
                            }
                        }
                    }
                });
    }
    std::vector<vmtype> VMul3::get_value(){
        return v;
    }
    std::vector<vmtype> VMul3::get_data(){
        return v;
    }
    void VMul3::print() {
        for (unsigned long long int i=0;i<v.size();++i){
            for (unsigned long long int j=0;j<SN;++j){
                for (unsigned long long int k=0;k<SN;++k){
                    std::cout<<v[i*SN2+j*SN+k]<<" ";
                }
                std::cout<<std::endl;
            }
            std::cout<<"-------------------"<<std::endl;
        }
        std::cout<<"////////////////////////"<<std::endl;
    }

    void VMul3::set_v(unsigned long long int i, unsigned long long int j, unsigned long long int k, vmtype val) {
        v[i*SN2+j*SN+k]=val;
    }
}
//
// Created by hasni on 07.10.2024.
//
