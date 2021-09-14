#include <iostream>
#include <algorithm>
#include <math.h>
typedef unsigned char uchar;
void print_point(bool tmp, int bpp, int bright, uchar* p2, int width, int x, int y, double intens, double gamma)
{
    if (!tmp) {
        int it = y;
        y = x;
        x = it;
    }
    *(p2 + y * width + x) =  bpp * pow(intens * bright / bpp, 1 / 2.0);
}
void line(uchar* p2, int &width, double& x0, double& x1, double & y0, double& y1, int bpp, int bright, double fatness, double gamma)
{
    bool tmp = true;
    double dx = abs(x1 - x0);
    double dy = abs(y1 - y0);
    if (dx < dy)
    {
        std::swap(dx,dy);
        std::swap(x0, y0);
        std:: swap(y1,x1);
        tmp = false;
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    print_point(tmp, bpp, bright, p2, width, x0, y0, 1, gamma);
    print_point(tmp, bpp, bright, p2, width, x1, y1, 1, gamma);
    dx = x1 - x0;
    dy = y1 - y0;
    if (dy == 0)
    {
        for (int x = x0 + 1; x < x1; x++)
        {
            print_point(tmp, bpp, bright, p2, width, x, y0, bright / bpp, gamma);
        }
    }
    double derr = double(dy)/dx;
    double y = y0 + derr;
    for (int x = x0 + 1; x < x1; x++)
    {
        print_point(tmp, bpp, bright, p2, width, x, y, 1 - y + int(y), gamma);
        print_point(tmp, bpp, bright, p2, width, x, y + 1, y - int(y), gamma);
        y += derr;
    }
}

void P5_line (FILE* fout, uchar* p, int width, int height, int bpp, double x0, double x1, double y0, double y1, int &bright, double &fatness, double &gamma)
{
    fprintf(fout, "P5\n%d %d\n%d\n", width, height, bpp);
    uchar * p2 = (uchar*)malloc(width*height*sizeof(uchar));
    for (int i = 0; i < width * height; i++)
        *(p2 + i) = 0;
    line(p2, width, x0, x1, y0, y1, bpp, bright, fatness, gamma);
    fwrite(p2, sizeof(uchar), width*height, fout);
    free(p2);
}
int main(int argc, char* argv[]) {
    if (argc != 10)
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
    int bright = atoi(argv[3]);
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
    double gamma = atof(argv[9]);
    if (gamma <= 0)
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
    bright = pow(double(bright)/ bpp, 2.0) * bpp;
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