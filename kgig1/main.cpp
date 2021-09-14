#include <iostream>
#include <string>
typedef unsigned char uchar;
void inversion_color (FILE* fout, int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d %d\n%d\n", type, width, height, bpp);
    for (int i = 0; i < height * width * cpp; i ++)
    {
        uchar b;
        b = 255 - *(p + i);
        fwrite(&b, sizeof(uchar), 1, fout);
    }
}

void flip_horizontal (FILE* fout, int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d %d\n%d\n", type, width, height, bpp);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < cpp; k++) {
                uchar b = *(p + i * width * cpp + (width - j)  * cpp - cpp + k);
                fwrite(&b, sizeof(uchar), 1, fout);
            }
        }
}

void flip_vertical (FILE* fout, int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d %d\n%d\n", type, width, height, bpp);
    width = width * cpp;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            uchar b = *(p + (height - i - 1) * width + j);
            fwrite(&b, sizeof(uchar), 1, fout);
        }
}

void flip_left (FILE* fout, int type, int width, int height, int bpp, int cpp, uchar* p)
{

    fprintf(fout, "P%d\n%d %d\n%d\n", type, height, width, bpp);
    for (int i = 0; i < width; i ++)
        for (int j = height - 1; j >= 0; j --)
        {
            for (int k = 0; k < cpp; k ++) {
                uchar b = *(p + j * width * cpp + i * cpp + k);
                fwrite(&b, sizeof(uchar), 1, fout);
            }
        }
}

void flip_right (FILE* fout, int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d %d\n%d\n", type, height, width, bpp);
    for (int i = width - 1; i >= 0; i --)
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < cpp; k ++) {
                uchar b = *(p + j * width * cpp + i * cpp + k);
                fwrite(&b, sizeof(uchar), 1, fout);
            }
        }
}

void P5_P6 (FILE* fin, FILE* fout, int &command, int width, int height, int bpp, int type)
{
    uchar* p;
    int cpp = 1;
    if (type == 6) {
        cpp = 3;
    }
    p = (uchar*)malloc(cpp * width * height * sizeof(uchar));
    int k = fread(p, sizeof(uchar), width * height * cpp, fin);
    if (k < 1)
    {
        std::cout << "Wrong type of the file";
            fclose(fout);
        exit(1);
    }
    switch (command)
    {
        case 0:
            inversion_color(fout, type, width, height, bpp, cpp, p);
            break;
        case 1:>
            flip_horizontal(fout, type, width, height, bpp, cpp, p);
            break;
        case 2:
            flip_vertical(fout, type, width, height, bpp, cpp, p);
            break;
        case 3:
            flip_left(fout, type, width, height, bpp, cpp, p);
            break;
        case 4:
            flip_right(fout, type, width, height, bpp, cpp, p);
            break;
        default:
            std::cout << "Wrong command";
            break;
    }
    free(p);
}

int main(int argc, char* argv[]) {
    if ((argc < 4) || (argc > 4))
    {
        std::cout << "Wrong arguments";
        return 1;
    }
    FILE* fin = fopen(argv[1], "rb");
    if (fin == NULL) {
        std::cout << "Can't open input file";
        return 1;
    }
    int command = atoi(argv[3]);
    if ((command < 0) || (command > 4))
    {
        std::cout << "Wrong command";
        fclose(fin);
        return 1;
    }
    int width;
    int height;
    int bpp;
    int type;
    int i = fscanf(fin, "P %d", &type);
    if ((i < 1) || ((type != 5) && (type != 6)))
    {
        std::cout << "Wrong type of the file";
        return 1;
    }
    else
    {
        i = fscanf(fin, "\n%d %d\n%d\n", &width, &height, &bpp);
        if (i == 3) {
            if ((width <= 0) || (height <= 0) || (bpp != 255))
            {
                std::cout << "Wrong type of the file";
                fclose(fin);
                return 1;
            }
            FILE *fout = fopen(argv[2], "wb");
            if (fout == NULL) {
                std::cout << "Can't open output file";
                fclose(fin);
                return 1;
            }
            P5_P6(fin, fout, command, width, height, bpp, type);
            fclose(fout);
        }
        else
        {
            std::cout << "Wrong type of the file";
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);
    return 0;
}