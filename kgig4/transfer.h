//
// Created by Арсений Карпов on 30.05.2020.
//

#ifndef KGIG4_TRANSFER_H
#define KGIG4_TRANSFER_H
#include <string>
#include <vector>
#include <algorithm>
class transfer {
private:
    double* _first{};
    double* _second{};
    double* _third{};
    int _width;
    int _height;
    std::vector<std::string> base;
    double for_HSL_RGB(double T, double q, double p);
    void HSL_RGB();
    void RGB_HSL();
    void HSV_RGB();
    void RGB_HSV();
    void YCbCr_RGB(double Kb, double Kr);
    void RGB_YCbCr(double Kb, double Kr);
    void YCoCg_RGB();
    void RGB_YCoCg();
    void CMY_RGB();
public:
    transfer(int width, int height, double* first, double* second, double* third);
    int convert(std::string from, std::string to);
};

#endif //KGIG4_TRANSFER_H
