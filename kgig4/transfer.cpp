//
// Created by Арсений Карпов on 30.05.2020.
//

#include "transfer.h"
transfer:: transfer(int width, int height, double* first, double* second, double* third)
:_width(width), _height(height), _first(first), _second(second), _third(third) {
    base.resize(7);
    base[0] = "RGB";
    base[1] = "HSL";
    base[2] = "HSV";
    base[3] = "YCbCr.601";
    base[4] = "YCbCr.709";
    base[5] = "YCoCg";
    base[6] = "CMY";
}

double transfer::for_HSL_RGB(double T, double q, double p)
{
    T = (T < 0) ? T + 1 : (T > 1) ? T - 1 : T;
    if (T < double(1) / 6)
        return p + (q - p) * 6 * T;
    if (T <  double(1) / 2)
        return q;
    if (T < double(2) / 3)
        return p + ((q - p) * (double(2) / 3 - T) * 6);
    return p;
}

void transfer::HSL_RGB()
{
    double Q, P, L, S, H;
    for (int i = 0 ; i < _width * _height; i++)
    {
        H = _first[i] / 255.0;
        S = _second[i] / 255.0;
        L = _third[i] / 255.0;
        Q = (L < 0.5) ? L * (1 + S) : (L + S - L * S);
        P = 2 * L - Q;
        _first[i] = for_HSL_RGB(H + 1.0 / 3.0, Q, P) * 255;
        _second[i] = for_HSL_RGB(H, Q, P) * 255;
        _third[i] = for_HSL_RGB(H - 1.0 / 3.0, Q, P) * 255;
    }
}

void transfer::RGB_HSL()
{
    double _max, _min, H, S, L, r, g, b;
    for (int i = 0; i < _width * _height; i++)
    {
        r = _first[i] / 255.0;
        g = _second[i] / 255.0;
        b = _third[i] / 255.0;
        _max = std::max(r, std::max(g, b));
        _min = std::min(r, std::min(g, b));
        L = (_max + _min) / 2.0;
        if ((_max == _min) || (L == 0))
            S = 0;
        else
        {
            if (L <= 0.5)
                S = (_max - _min) / (2.0 * L);
            else
                S = (_max - _min) / (2.0 - 2.0 * L);
        }
        if (_max == _min)
            H = 0;
        else
        {
            if (_max == r)
            {
                H = 60.0 * (g - b) / (_max - _min);
                if (g < b)
                    H += 360;
            }
            if (_max == g)
                H = 60.0 * (b - r) / (_max - _min) + 120;
            if (_max == b)
                H = 60.0 * (r - g) / (_max - _min) + 240;
        }
        _first[i] = H * 255 / 360.0;
        _second[i] = S * 255;
        _third[i] = L * 255;
    }
}

void transfer::HSV_RGB()
{
    double H, S, V, Hi,Vmin, Vinc, Vdec, a;
    int H1;
    for (int i = 0; i < _width * _height; i++)
    {
        H = _first[i] / 255.0 * 360;
        S = _second[i] / 255.0;
        V = _third[i] / 255.0;
        Vmin = (1 - S) * V;
        Hi = (H / 60.0);
        H1 = int(Hi);
        a = (V - Vmin) * (Hi - H1);
        H1 = H1 % 6;
        Vinc = Vmin + a;
        Vdec = V - a;
        switch (H1)
        {
        case 0:
        {
            _first[i] = V * 255;
            _second[i] = Vinc * 255;
            _third[i] = Vmin * 255;
            break;
        }
        case 1:
        {
            _first[i] = Vdec * 255;
            _second[i] = V * 255;
            _third[i] = Vmin * 255;
            break;
        }
        case 2:
        {
            _first[i] = Vmin * 255;
            _second[i] = V * 255;
            _third[i] = Vinc * 255;
            break;
        }
        case 3:
        {
            _first[i] = Vmin * 255;
            _second[i] = Vdec * 255;
            _third[i] = V * 255;
            break;
        }
        case 4:
        {
            _first[i] = Vinc * 255;
            _second[i] = Vmin * 255;
            _third[i] = V * 255;
            break;
        }
        default:
        {
            _first[i] = V * 255;
            _second[i] = Vmin * 255;
            _third[i] = Vdec * 255;
            break;
        }
        }
    }
}

void transfer::RGB_HSV()
{
    double S, V, r, g, b, H, _max, _min;
    for (int i = 0; i <_width*_height; i++)
    {
        r = _first[i] / 255.0;
        g = _second[i] / 255.0;
        b = _third[i] / 255.0;
        _max = std::max(r, std::max(g, b));
        _min = std::min(r, std::min(g, b));
        V = _max;
        if (_max == 0)
            S = 0;
        else
            S = 1 - double(_min) / _max;
        if (_max == _min)
            H = 0;
        else
        {
            if (_max == r)
            {
                H = 60.0 * (g - b) / (_max - _min);
                if (g < b)
                    H += 360;
            }
            if (_max == g)
                H = 60.0 * (b - r) / (_max - _min) + 120;
            if (_max == b)
                H = 60.0 * (r - g) / (_max - _min) + 240;
        }
        _first[i] = H * 255 / 360.0;
        _second[i] = S * 255;
        _third[i] = V * 255;
    }
}

void transfer::YCbCr_RGB(double Kb, double Kr)
{
    double Y, Cb, Cr;
    for (int i = 0; i < _width * _height; i++)
    {
        Y = _first[i] / 255.0;
        Cb = _second[i] / 255.0 * 2 - 1;
        Cr = _third[i] / 255.0 * 2 - 1;
        _first[i] = std::max(std::min((Y + Cr * (1 - Kr)) * 255, 255.0), 0.0);
        _second[i] = std::max(std::min((Y - double((Kb * (1 - Kb) * Cb + Cr * (1 - Kr) * Kr)) / (1 - Kr - Kb))* 255, 255.0), 0.0);
        _third[i] = std::max(std::min((Y + Cb * (1 - Kb)) * 255, 255.0), 0.0);
    }
}

void transfer::RGB_YCbCr(double Kb, double Kr)
{
    double r, g, b;
    for (int i = 0; i < _width*_height; i++)
    {
        r = _first[i] / 255.0;
        g = _second[i] / 255.0;
        b = _third[i] / 255.0;
        _first[i] = Kr * r + (1 - Kr - Kb) * g + Kb * b;
        _second[i] = double(b - _first[i])/ (1 - Kb) + 1;
        _third[i] = double (r - _first[i]) / (1 - Kr) + 1;
        _first[i] = std::max(std::min(_first[i] * 255, 255.0), 0.0);
        _second[i] = std::max(std::min(_second[i] * 255 / 2, 255.0), 0.0);
        _third[i] = std::max(std::min(_third[i] * 255 / 2, 255.0), 0.0);
    }
}

void transfer::YCoCg_RGB()
{
    double Y, Co, Cg;
    for (int i = 0; i < _width * _height; i++)
    {
        Y = _first[i] / 255.0;
        Co = _second[i] / 255.0 - 0.5;
        Cg = _third[i] / 255.0 - 0.5;
        _first[i] = std::max(std::min((Y + Co - Cg) * 255, 255.0), 0.0);
        _second[i] = std::max(std::min((Y + Cg) * 255, 255.0), 0.0);
        _third[i] = std::max(std::min((Y - Co - Cg) * 255, 255.0), 0.0);
    }
}

void transfer::RGB_YCoCg()
{
    double r, g, b;
    for (int i = 0; i < _width * _height; i++)
    {
        r = _first[i] / 255.0;
        g = _second[i] / 255.0;
        b = _third[i] / 255.0;
        _first[i] = std::max(std::min((r / 4.0 + g / 2.0 + b / 4.0) * 255, 255.0), 0.0);
        _second[i] = std::max(std::min((0.5 * r - 0.5 * b + 0.5) * 255, 255.0), 0.0);
        _third[i] = std::max(std::min((- r / 4.0 + g / 2.0 - b / 4.0 + 0.5) * 255, 255.0), 0.0);
    }
}

void transfer::CMY_RGB()
{
    for (int i = 0; i < _width * _height; i++)
    {
        _first[i] = 255 - _first[i];
        _second[i] = 255 - _second[i];
        _third[i] = 255 - _third[i];
    }
}

int transfer:: convert(std::string from, std::string to)
{
    int i = 0;
    while ((from != base[i])  && (i < 7))
        i++;
    switch (i) {
    case 0:
        break;
    case 1:
    {
        HSL_RGB();
        break;
    }
    case 2:
    {
        HSV_RGB();
        break;
    }
    case 3:
    {
        YCbCr_RGB(0.114, 0.299);
        break;
    }
    case 4:
    {
        YCbCr_RGB(0.0722, 0.2126);
        break;
    }
    case 5:
    {
        YCoCg_RGB();
        break;
    }
    case 6:
    {
        CMY_RGB();
        break;
    }
    default:
    {
        return 1;
    }
    }
    i = 0;
    while ((to != base[i]) && (i < 7))
        i++;
    switch (i) {
    case 0:
        break;
    case 1:
    {
        RGB_HSL();
        break;
    }
    case 2:
    {
        RGB_HSV();
        break;
    }
    case 3:
    {
        RGB_YCbCr(0.114, 0.299);
        break;
    }
    case 4:
    {
        RGB_YCbCr(0.0722, 0.2126);
        break;
    }
    case 5:
    {
        RGB_YCoCg();
        break;
    }
    case 6:
    {
        CMY_RGB();
        break;
    }
    default:
    {
        return 1;
    }
    }
    return 0;
}