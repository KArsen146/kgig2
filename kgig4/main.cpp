#include <iostream>
#include "transfer.h"
#include <cmath>
#include <string.h>
int count_i, count_o, num, numo;
double* first, * second, * third;
typedef unsigned char uchar;
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

char* create_name(char* name,int num)
{
    char* new_name;
    int j = strlen(name);
    while ((name[j] != '.') && (j > -1))
        j--;
    new_name = new char[100];
    for (int i = 0;i < 100; i++)
        new_name[i] = '\0';
    strncat(new_name, name, j);
    switch (num)
    {
    case 1: {
        strcat(new_name, "_1.pgm");
        break;
    }
    case 2:
    {
        strcat(new_name, "_2.pgm");
        break;
    }
    default:
    {
        strcat(new_name, "_3.pgm");
        break;
    }
    }
    strcat(new_name, "\0");
    return new_name;
}

int input_files(char* in, int &width, int &height, int &bpp)
{
    if (count_i==1) {
        FILE* fin = fopen(in, "rb");
        if (fin==NULL)
            return 1;
        int type;
        int i = fscanf(fin, "P%d", &type);
        if ((type!=6) || (i<1)) {
            fclose(fin);
            return 2;
        }
        i = fscanf(fin, "\n%d %d\n%d\n", &width, &height, &bpp);
        if ((i<3) || (bpp<0) || (bpp>255)) {
            fclose(fin);
            return 2;
        }
        uchar* p = (uchar*) malloc(3*width*height*sizeof(uchar));
        int k = fread(p, 1, 3*width*height, fin);
        if (k < 1) {
            fclose(fin);
            return 2;
        }
         first = new double[width*height];
        second = new double[width*height];
        third= new double[width*height];
        for (i = 0; i<width*height; i++) {
            first[i] = (double) (*(p + i * 3));
            second[i] = (double) (*(p + i * 3 + 1));
            third[i] = (double) (*(p + i * 3 + 2));
        }
        free(p);
        fclose(fin);
        return 0;
    }
    int width1, height1;
    for (int i = 1; i < 4; i++)
    {
        char* new_name;
        new_name =create_name(in, i);
        FILE* fin = fopen(new_name, "rb");
        if (fin == NULL)
            return 1;
        int type;
        int it = fscanf(fin, "P%d", &type);
        if ((type != 5) || (it < 1)) {
            fclose(fin);
            return 2;
        }
        it = fscanf(fin, "\n%d %d\n%d\n", &width, &height, &bpp);
        if (i == 1) {
            width1 = width;
            height1 = height;
        }

        if ((it < 3) || (bpp < 0) || (bpp > 255) || (width1 != width) || (height1 != height)) {
            fclose(fin);
            return 2;
        }
        switch (i)
        {
        case 1:
        {
            first = new double[width * height];
            int k = 0;
            uchar* p = (uchar*) malloc(3*width*height*sizeof(uchar));
            k = fread(p, 1, width * height, fin);
            for (int i = 0; i < width * height; i++)
                first[i] = (double)(*(p + i));
            free(p);
            break;
        }
        case 2:
        {
            second = new double[width * height];
            uchar* p = (uchar*) malloc(3*width*height*sizeof(uchar));
            int k = fread(p, 1, width * height, fin);
            for (int i = 0; i < width * height; i++)
                second[i] = (double)(*(p + i));
            free(p);
            break;
        }
        default:
        {
            third = new double[width * height];
            uchar* p = (uchar*) malloc(3*width*height*sizeof(uchar));
            int k = fread(p, 1, width * height, fin);
            for (int i = 0; i < width * height; i++)
                third[i] = (double)(*(p + i));
            free(p);
            break;
        }
        }
        fclose(fin);
        width1 = width;
        height1 = height;
    }
    return 0;
}
int output_files(char* out, int width, int height, int bpp)
{
    if (count_o == 1)
    {
        FILE* fout = fopen(out, "wb");
        if (fout == NULL)
            return 1;
        uchar u;
        fprintf(fout, "P6\n%d %d\n%d\n", width, height, bpp);
        for (int i = 0; i<width*height; i++) {
            u = (uchar) first[i];
            fwrite(&u, 1, 1, fout);
            u = (uchar) second[i];
            fwrite(&u, 1, 1, fout);
            u = (uchar) third[i];
            fwrite(&u, 1, 1, fout);
        }
        delete(first);
        delete(second);
        delete(third);
        fclose(fout);
        return 0;
    }
    for (int i = 1; i < 4; i++)
    {
        char * new_name = create_name(out, i);
        FILE* fout = fopen(new_name, "wb");
        if (fout == NULL)
            return 1;
        uchar u;
        fprintf(fout, "P5\n%d %d\n%d\n", width, height, bpp);
        switch (i)
        {
        case 1:
        {
            for (int j = 0; j<width*height; j++)
            {
                u = (uchar) first[j];
                fwrite(&u, 1, 1, fout);
            }
            delete(first);
            break;
        }
        case 2:
        {
            for (int j = 0; j<width*height; j++)
            {
                u = (uchar) second[j];
                fwrite(&u, 1, 1, fout);
            }
            delete(second);
            break;
        }
        default:
        {
            for (int j = 0; j<width*height; j++)
            {
                u = (uchar) third[j];
                fwrite(&u, 1, 1, fout);
            }
            delete(third);
            break;
        }
        }
        fclose(fout);
    }
    return 0;
}
int main(int argc, char* argv[])
{
    try {
        if (argc != 11)
            throw Exception("Incorrect nubmer of arguments");
        std::vector<bool> used(4, false);
        std::vector<std::string> commands(4);
        commands[0] = "-f";
        commands[1] = "-t";
        commands[2] = "-i";
        commands[3] = "-o";
        std::string from, to;
        int i = 1;
        while (i < 10)
        {
            int j = 0;
            while ((j < 4) && (commands[j] != argv[i]))
                j++;
            if ((used[j]) || (j == 4))
                throw Exception("Incorrect command");
            i++;
           switch (j)
            {
            case 0:
            {
                from = argv[i];
                break;
            }
            case 1:
            {
                to = argv[i];
                break;
            }
            case 2:
            {
                count_i = atoi(argv[i]);
                i++;
                num = i;
                break;
            }
            default:
            {
                count_o = atoi(argv[i]);
                i++;
                numo = i;
                break;
            }
            }
            i++;
        }
        int width, height, bpp;
        if (input_files(argv[num],width, height, bpp) != 0)
            throw Exception("Problems with input file");
        uchar u;
        transfer clas(width, height, first, second, third);
        if (clas.convert(from, to) != 0)
            throw Exception("Incorrect colorspace");
        if (output_files(argv[numo], width, height, bpp) != 0)
            throw Exception("Problems with output file");
    }
    catch (Exception &ex)
    {
        std::cerr<<ex.out_error();
        delete(first);
        delete(second);
        delete(third);
        return 1;
    }
    return 0;
}