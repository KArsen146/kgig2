#include <iostream>
#include <algorithm>
#include <cmath>
typedef unsigned char uchar;
double correct(double value, double gamma, int bpp)
{
    value = double(value) / bpp;
    if (value > 1)
        value = 1;
    if (gamma == 0)
    {
        if (value < 0.04045)
            return  (bpp * value) / 12.92;
        else
            return bpp * (pow((200.0 * value + 11.0) / 211.0, 2.4));
    }
    else {
        return bpp * pow(value, gamma);
    }
}
double to_real(double value, double gamma, int bpp)
{
    value = double(value)/ bpp;
    if (gamma == 0)
    {
        if (value < 0.0031308)
            return  value * 12.92 * bpp;
        else
            return bpp * ((211.0 * pow(value, 0.4166) - 11.0) / 200.0);
    }
    else {
        return bpp * pow(value, 1 / gamma);
    }
}
void print_point(bool tmp, int bpp, double bright, uchar* p2, int width, int height, double x, double y, double &intens, double gamma, double fat1)
{
    if (!tmp)
    {
        double it = y;
        y = x;
        x = it;
    }
    if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
        if (fat1 != 0)
            intens *= fat1;
        int index = abs(y * width + x) + 0.5;
        *(p2 + index) = correct(*(p2 + index), gamma, bpp);
        *(p2 + index) = to_real((*(p2 + index) * (1 - intens) + bright * intens), gamma, bpp);
        if (*(p2 + index) < 0)
            *(p2 + index) = 0;
        if (*(p2 + index) > bpp)
            *(p2 + index) = bpp;
    }
}
void line(uchar* p2, int &width, int &height, double& x0, double& x1, double & y0, double& y1, int bpp, double &bright, double &fatness, double gamma)
{
    bool tmp = true;
    double dx = abs(x1-x0);
    double dy = abs(y1-y0);
    if (dx < dy) {
        std::swap(dx, dy);
        std::swap(x0, y0);
        std::swap(y1, x1);
        tmp = false;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    dy = y1 - y0;
    bright = correct(bright, gamma, bpp);
    double fat1 = 0;
    if (fatness < 1)
    {
        fat1 = fatness;
        fatness += 0.5;
    }
    double y_0, y_1, k, k_x, k_y;
    if (dy == 0) {
        for (int x = x0; x <= ceil(x1); x++)
        {
            y_0 = y1 - (fatness - 1) / 2;
            y_1 = y1 + (fatness - 1) / 2;
            for (int y = int(y_0); y <= ceil(y_1); y++)
            {
                k_y = std::min(1.0, (fatness + 1) / 2 - abs(y - y1));
                k_x = k_y;
                if (int(x1) != x1)
                {
                    if (x == int(x1))
                        k_x = x + 1 - x1;
                    if (x == ceil(x1))
                        k_x = x1 + 1 - x;
                }
                double intens = (k_x + k_y) / 2;
                print_point(tmp, bpp, bright, p2, width, height, x, y, intens, gamma, fat1);
            }
        }
    }
    else {
        double max_y = y0;
        double max_x = x0;
        double buf = y0;
        k = (x1-x0)/(y1-y0);
        k_x = abs(fatness*cos(atan(k)));
        k_y = abs(fatness*sin(atan(k)));
        for (int x = int(x0-k_x/2); x<=ceil(x1+k_x/2); x++) {
            double _y = (x-x0)/k+y0;

            y_0 = -k*(x-x0)+y0;
            y_1 = -k*(x-x1)+y1;
            if (y_0>y_1)
                std::swap(y_0, y_1);
            y_0 = std::max(_y-(k_y)*abs(1/k), y_0-1);
            y_1 = std::min(_y+(k_y)*abs(1/k), y_1)+1;
            if (max_y<y_1) {
                max_y = y_1;
                max_x = x;
            }
            for (int y = int(y_0); y<=y_1; y++) {
                double intens = 1;
                if (y==int(y_0)) {
                    intens = 1-y_0+int(y_0);
                    if ((intens>=0.85))
                        intens = 0.85;
                    if (intens<=0.15)
                        intens = 0;
                    if ((max_y==y_1) && (max_x==x))
                        buf = intens;
                }
                else if (y==int(y_1)) {
                    intens = y_1-int(y_1);
                    if ((intens>=0.85))
                        intens = 0.85;
                    if (intens<=0.15)
                        intens = 0;
                }
                print_point(tmp, bpp, bright, p2, width, height, x, y, intens, gamma, fat1);
            }
        }
        print_point(tmp, bpp, bright, p2, width, height, max_x, max_y, buf, gamma, fat1);
    }
}
void P5_line (FILE* fout, uchar* p, int width, int height, int bpp, double x0, double x1, double y0, double y1, double &bright, double &fatness, double &gamma)
{
    fprintf(fout, "P5\n%d %d\n%d\n", width, height, bpp);
    line(p, width, height, x0, x1, y0, y1, bpp, bright, fatness, gamma);
    fwrite(p, sizeof(uchar), width*height, fout);
}
int main(int argc, char* argv[]) {
    if ((argc != 10) && (argc != 9))
    {
        std::cerr << "Wrong arguments";
        return 1;
    }
    FILE* fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        std::cerr << "Can't open input file";
        return 1;
    }
    int width, height, bpp;
    double bright = atoi(argv[3]);
    if ((bright < 0) || (bright > 255))
    {
        std::cerr << "Incorrect brightness";
        return 1;
    }
    double fatness = atof(argv[4]);
    if (fatness <= 0)
    {
        std::cerr << "Incorrect fatness";
    }
    double x0 = atof(argv[5]);
    double y0 = atof(argv[6]);
    double x1 = atof(argv[7]);
    double y1 = atof(argv[8]);
    double gamma = 0;
    if (argc == 10)
        gamma = atof(argv[9]);
    if (gamma < 0)
    {
        std::cerr << "Incorrect gamma";
        return 1;
    }
    int type;
    int i = fscanf(fin, "P %d", &type);
    if ((i < 1) || (type != 5))
    {
        std::cerr << "Wrong type of the fin";
        fclose(fin);
        return 1;
    }
    i = fscanf(fin, "\n%d %d\n%d\n", &width, &height, &bpp);
    if (i != 3)
    {
        std::cerr << "Wrong type of the fin";
        fclose(fin);
        return 1;
    }
    if (width <= 0)
    {
        std::cerr << "Wrong width";
    }
    if (height <= 0)
    {
        std::cerr << "Wrong height";
    }
    if ((x0 < 0) || (x0 > width))
    {
        std::cerr << "Incorrect x0 coordinate";
        return 1;
    }
    if ((y0 < 0) || (y0 > height))
    {
        std::cerr << "Incorrect y0 coordinate";
        return 1;
    }
    if ((x1 < 0) || (x1 > width))
    {
        std::cerr << "Incorrect x1 coordinate";
        return 1;
    }
    if ((y1 < 0) || (y1 > height))
    {
        std::cerr << "Incorrect y1 coordinate";
        return 1;
    }
    uchar* p;
    p = (uchar*)malloc( width * height * sizeof(uchar));
    int k = fread(p, sizeof(uchar), width * height, fin);
    if (k < 1)
    {
        std::cerr << "Wrong type of the fin";
        fclose(fin);
        return 1;
    }
    FILE* fout = fopen(argv[2], "wb");
    if (fout == NULL) {
        std::cerr << "Can't open output file";
        fclose(fin);
        return 1;
    }
    P5_line(fout, p, width, height, bpp, x0, x1, y0, y1, bright, fatness, gamma);
    free(p);
    return 0;
}