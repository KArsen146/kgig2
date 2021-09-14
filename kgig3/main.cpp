#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <string>
typedef unsigned char uchar;
void gradient(uchar* p, int width, int height, int bpp, double gamma)
{
    double k = double(255)/ width;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            *(p + i * width + j) = round(bpp * pow(double(j * k)/ bpp, double(1 / gamma)));
        }
}

class Exception
{
public:
    Exception(std::string error)
    {
        error_ = error;
    }
    std::string out_error()
    {
        return error_;
    }
private:
    std::string error_;
};

class dithering
{
private:
    static std::vector<std::vector<double>> matrix()
    {
        std::vector<std::vector<double>> v(8, std:: vector<double> (8));
        v[0] =  {1, 49,  13,  61,   4,  52,  16,  64};
        v[1] = {33,  17,  45,  29,  36,  20,  48,  32};
        v[2] = {9,  57,   5 , 53,  12,  60 ,  8,  56};
        v[3] = {41,  25,  37,  21,  44,  28,  40 , 24};
        v[4] = {3,  51,  15,  63,   2,  50 , 14 , 62};
        v[5] = {35,  19,  47,  31,  34,  18,  46 , 30};
        v[6] = {11,  59,   7,  55,  10,  58,   6 , 54};
        v[7] = {43,  27,  39,  23,  42,  26,  38 , 22};
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                v[i][j] = double(v[i][j]) / 65 - 0.5;
        return v;
    }
    static std::vector<std::vector<double>> matrix2()
    {
        std::vector<std::vector<double>> v(4, std:: vector<double> (4));
        v[0] =  {7, 13, 11, 4};
        v[1] = {12, 16, 14, 8};
        v[2] = {10, 15, 6, 2};
        v[3] = {5, 9, 3, 1};
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                v[i][j] = double(v[i][j]) / 16 - 0.5;
        return v;
    }
    static std::vector<std::vector<double>> err1(int type, int width)
    {
        std::vector<std::vector<double>> err(type, std::vector<double> (width));
        for (int i = 0; i < type; i++)
            for (int j = 0; j < width; j++)
                err[i][j] = 0;
        return err;
    }
    static int closest_color(double k, int bit)
    {
        int k1 = round(k);
        if (k1 >= 255)
            return 255;
        if (k1 < 0)
            return 0;
        int k2 = k1 >> (8 - bit);
        k1 = 0;
        for (int i = 0; i < 7/ bit  + 1; i++)
            k1 = (k1 << bit) + k2;
        k1 = k1 >> ((7 / bit + 1) * bit - 8);
        return k1;
    }
    static void _err_floyd(std::vector<std::vector<double>> &er, int i, int j, double err, int width)
    {
        if (j < width - 1)
        {
            er[i % 2][j + 1] += double(7 * err) / 16 ;
            er[(i + 1) % 2][j + 1] += double(err) / 16;
        }
        er[(i + 1) % 2][j] += double(5 * err) / 16;
        if (j > 0)
            er[(i + 1) % 2][j - 1] += double(3 * err) / 16;
    }
    static void _err_jjn(std::vector<std::vector<double>> &er, int i, int j, double err, int width)
    {
        if (j < width - 1)
        {
            er[i % 3][j + 1] += double(7) / 48 * err;
            er[(i + 1) % 3][j + 1] += double(5) / 48 * err;
            er[(i + 2) % 3][j + 1] += double(3) / 48 * err;
        }
        if (j < width - 2)
        {
            er[i % 3][j + 2] += double(5) / 48 * err;
            er[(i + 1) % 3][j + 2] += double(3) / 48 * err;
            er[(i + 2) % 3][j + 2] += double(1) / 48 * err;
        }
        er[(i + 1) % 3][j] += double(7) / 48 * err;
        er[(i + 2) % 3][j] += double(5) / 48 * err;
        if (j > 0)
        {
            er[(i + 1) % 3][j - 1] += double(5) / 48 * err;
            er[(i + 2) % 3][j - 1] += double(3) / 48 * err;
        }
        if (j > 1)
        {
            er[(i + 1) % 3][j - 2] += double(3) / 48 * err;
            er[(i + 2) % 3][j - 2] += double(1) / 48 * err;
        }
    }
    static void _err_sierra(std::vector<std::vector<double>> &er, int i, int j, double err, int width)
    {
        if (j < width - 1)
        {
            er[i % 3][j + 1] += double(5) / 32 * err;
            er[(i + 1) % 3][j + 1] += double(4) / 32 * err;
            er[(i + 2) % 3][j + 1] += double(2) / 32 * err;
        }
        if (j < width - 2)
        {
            er[i % 3][j + 2] += double(3) / 32 * err;
            er[(i + 1) % 3][j + 2] += double(2) / 32 * err;
        }
        er[(i + 1) % 3][j] += double(5) / 32 * err;
        er[(i + 2) % 3][j] += double(3) / 32 * err;
        if (j > 0)
        {
            er[(i + 1) % 3][j - 1] += double(4) / 32 * err;
            er[(i + 2) % 3][j - 1] += double(2) / 32 * err;
        }
        if (j > 1)
        {
            er[(i + 1) % 3][j - 2] += double(2) / 32 * err;
        }
    }
    static void _err_atk(std::vector<std::vector<double>> &er, int i, int j, double err, int width)
    {
        if (j < width - 1)
        {
            er[i % 3][j + 1] += double(1) / 8 * err;
            er[(i + 1) % 3][j + 1] += double(1) / 8 * err;
        }
        if (j < width - 2)
            er[i % 3][j + 2] += double(1) / 8 * err;
        er[(i + 1) % 3][j] += double(1) / 8 * err;
        er[(i + 2) % 3][j] += double(1) / 8 * err;
        if (j > 0)
            er[(i + 1) % 3][j - 1] += double(1) / 8 * err;
    }
    static int closest_color(double k, std::vector<std::vector<double>> &er, int bit, int i, int j, int width, int type)
    {
        k += er[i % (std::min(3, type))][j];
        er[i % (std::min(3, type))][j] = 0;
        int k1 = round(k);
        if (k1 > 255)
            k1 =  255;
        else
        if (k1 < 0)
            k1 = 0;
        else
        {
            int k2 = k1 >> (8 - bit);
            k1 = 0;
            for (int i = 0; i < 7 / bit + 1; i++)
                k1 = (k1 << bit) + k2;
            k1 = k1 >> ((7 / bit + 1) * bit - 8);
        }
        double err = k - k1;
        if (type == 2)
        {
            _err_floyd(er, i, j, err, width);
        }
        if (type == 3)
        {
            _err_jjn(er, i, j, err, width);
        }
        if (type == 4)
        {
            _err_sierra(er, i, j, err, width);
        }
        if (type == 5)
        {
            _err_atk(er, i, j, err, width);
        }
        return k1;
    }
    std::vector<std::vector<double>> v;
public:
    dithering(int k, uchar* p, int width, int height, int bpp, double gamma, int bit)
    {
        switch (k)
        {
            case 0:
                _nxn(p, width, height, bpp, gamma, bit, 0);
                break;
            case 1:
                v = matrix();
                dithering::_nxn(p, width, height, bpp, gamma, bit, 8);
                break;
            case 2:
                dithering::rnd(p, width, height, bpp, gamma, bit);
                break;
            case 3:
                error(p, width, height, bpp, gamma, bit, 2);
                break;
            case 4:
                error(p, width, height, bpp, gamma, bit, 3);
                break;
            case 5:
                error(p, width, height, bpp, gamma, bit, 4);
                break;
            case 6:
                error(p, width, height, bpp, gamma, bit, 5);
                break;
            case 7:
                v = matrix2();
                dithering::_nxn(p, width, height, bpp, gamma, bit, 4);
                break;
            default:
                break;
        }
    }
    void _nxn(uchar* p, int width, int height, int bpp, double gamma, int bit, int n)
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                *(p + i * width + j) = bpp * pow(double(*(p + i * width + j)) / bpp, gamma);
                if (n == 0)
                    *(p + i * width + j) = closest_color(*(p + i * width + j), bit);
                else
                    *(p + i * width + j) = closest_color(*(p + i * width + j) +  bpp * (dithering::v[i % n][j % n]), bit);
                *(p + i * width + j) = pow((double(*(p + i * width + j)) / bpp), double(1) / gamma) * bpp;
            }
    }
    static void rnd(uchar* p, int width, int height, int bpp, double gamma, int bit)
    {
        random();
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                *(p + i * width + j) = bpp * pow(double(*(p + i * width + j)) / bpp, gamma);
                *(p + i * width + j) = closest_color(*(p + i * width + j) +pow(-1, j) * bpp * double(random() % 50) / 100, bit);
                *(p + i * width + j) = pow((double(*(p + i * width + j)) / bpp), double(1) / gamma) * bpp;
            }
    }
    static void error(uchar* p, int width, int height, int bpp, double gamma, int bit, int type)
    {
        std::vector<std::vector<double>> er = err1(std::min(type, 3) ,width);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                *(p + i * width + j) = bpp * pow(double(*(p + i * width + j)) / bpp, gamma);
                *(p + i * width + j) = closest_color(*(p + i * width + j), er, bit, i, j, width, type);
                *(p + i * width + j) = pow((double(*(p + i * width + j)) / bpp), double(1) / gamma) * bpp;
            }
        }
    }
};
int main(int argc, char* argv[])
{
    try{
        if ((argc > 7) || (argc < 7))
            throw Exception("Wrong arguments");
        FILE* fin = fopen(argv[1], "rb");
        if (fin == nullptr)
            throw Exception("Can't open inputfile");
        int width = 256;
        int height = 256;
        int bpp = 255;
        int type = 0;
        int i = fscanf(fin, "P %d", &type);
        if ((i != 1) || (type != 5))
        {
            fclose(fin);
            throw Exception("Incorrect file type");
        }
        i = fscanf(fin, "\n%d %d\n%d\n", &width, &height, &bpp);
        if (i != 3)
        {
            fclose(fin);
            throw Exception("Incorrect file type");
        }
        if ((width <=0) || (height <= 0) || (bpp != 255))
        {
            fclose(fin);
            throw Exception("Incorrect file type");
        }
        double gamma;
        int bit;
        uchar *p = (uchar*)malloc(width*height*sizeof(uchar));
        i = fread(p, sizeof(uchar), width * height, fin);
        if (i != width*height)
        {
            fclose(fin);
            throw Exception("Incorrect file type");
        }
        FILE* fout = fopen(argv[2], "wb");
        if (fout == nullptr)
        {
            fclose(fin);
            throw Exception("Can't open outputfile");
        }
        int dith = std::stoi(argv[4]);
        if ((dith < 0) || (dith > 7))
        {
            fclose(fin);
            fclose(fout);
            throw Exception("No such dithering");
        }
        bit = std::stoi(argv[5]);
        gamma = std::stod(argv[6]);
        if (gamma == 0)
            gamma = 2.2;
        if(std::stoi(argv[3]) == 1)
            gradient(p, width, height, bpp, gamma);
        dithering A(dith, p, width, height, bpp, gamma, bit);
        fprintf(fout, "P5\n%d %d\n%d\n", width, height, bpp);
        fwrite(p, sizeof(uchar), width*height, fout);
        fclose(fin);
        fclose(fout);
    }
    catch (Exception &ex)
    {
        std::cerr << ex.out_error() << std::endl;
        return 1;
    }
    return 0;
}