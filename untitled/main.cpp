#include <iostream>
typedef unsigned char uchar;
FILE* fin = fopen("Untitled_2.pnm", "rb");
FILE* fout = fopen("kgig-out.pnm", "wb");

void inversion (int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d\n%d\n%d\n", type, width, height, bpp);
    for (int i = width * height * cpp - 1; i >= 0; i--)
    {
        uchar b;
        b = *(p + i);
        fwrite(&b, sizeof(uchar), 1, fout);
    }
}

void flip_horizontal (int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d\n%d\n%d\n", type, width, height, bpp);
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            for (int k = 0; k < cpp; k++) {
                uchar b = *(p + i * width * cpp + (width - j) * cpp - cpp + k);
                fwrite(&b, sizeof(uchar), 1, fout);
            }
        }
}

void flip_vertical (int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d\n%d\n%d\n", type, width, height, bpp);
    width = width * cpp;
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            uchar b = *(p + (height - i - 1) * width + j);
            fwrite(&b, sizeof(uchar), 1, fout);
        }
}

void flip_left (int type, int width, int height, int bpp, int cpp, uchar* p)
{
    fprintf(fout, "P%d\n%d\n%d\n%d\n", type, height, width, bpp);
    for (int i = 0; i < height; i++)
        for (int j = width - 1; j >= 0; j--)
        {
            for (int k = 0; k < cpp; k++) {
                uchar b = *(p + j * height + i * cpp + k);
                fwrite(&b, sizeof(uchar), 1, fout);
            }
        }
}

void flip_right (int type, int width, int height, int bpp, int cpp, uchar* p)
{

}
void P5 (int &command, int width, int height, int bpp, int type)
{
    uchar* p;
    int cpp = 1;
    if (type == 6) {
        cpp = 3;
    }
    p = (uchar*)malloc(cpp * width * height * sizeof(uchar));
    fread(p, sizeof(uchar), width * height * cpp, fin);
    switch (command)
    {
        case 0:
            inversion(type, width, height, bpp, cpp, p);
            break;
        case 1:
            flip_horizontal(type, width, height, bpp, cpp, p);
            break;
        case 2:
            flip_vertical(type, width, height, bpp, cpp, p);
            break;
        case 3:
            flip_left(type, width, height, bpp, cpp, p);
            break;
        case 4:
            break;
    }
    free(p);
    fclose(fin);
    fclose(fout);
}
using namespace std;
int main() {
    int command;
    int width;
    int height;
    int bpp;
    cin >> command;
    if (fin == NULL) {
        cout << "File is empty";
        return 1;
    }
    int type;
    int i = fscanf(fin, "P %d", &type);
    if ((i < 1) || ((type != 5) && (type != 6)))
    {
        cout << "Wrong type of the file";
        return 1;
    }
    else
    {
        fscanf(fin, " %d %d %d\n", &width, &height, &bpp);
        P5(command, width, height, bpp, type);
    }
    fclose(fin);
    return 0;
}