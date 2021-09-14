#pragma warning(disable : 4996)
#include <cstdio>
#include <cmath>
#include <iostream>

int type, h, w, max;
unsigned char *pixels;

//useful functions
template <class T>
void swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

float CharTofloat(char *strnum)
{
	float num = 0;
	int i = 0, j = 0, s = 1;

	if (strnum[0] == '-')
	{
		s = -1;
		i++;
	}
	for (; strnum[i] != '\0' && strnum[i] != '.'; i++)
		num = num * 10 + strnum[i] - '0';
	if (strnum[i] == '\0')
		return num;

	for (; strnum[j + i + 1] != '\0'; j++)
		num = num * 10 + strnum[j + i + 1] - '0';
	for (int k = 0; k < j; k++)
		num /= 10;

	return num;
}

template <class T>
T sign(T num) { return num < 0 ? -1 : num > 0 ? 1 : 0; }

bool OnPic(int coord_x, int coord_y) { return coord_x >= 0 && coord_x < w && coord_y >= 0 && coord_y < h; }

int Rounding(float num) { return (int)(sign(num) * (fabs(num) + 0.5)); }

int round_up(float num) { return int(num) + (int(num) != num); }

//file check
char ReadFile(char* filename)
{
	FILE* f;

	// file checking (closing file)
	if ((f = fopen(filename, "rb")) == NULL)
	{
		fprintf(stderr, "Put another filename!\n");
		return 0;
	}

	if (fscanf(f, "P%i%i%i%i\n", &type, &w, &h, &max) == NULL)
	{
		fprintf(stderr, "Can't read this file\n");
		fclose(f);
		return 0;
	}

	// data checking (closing file)
	if (type != 5)
	{
		fprintf(stderr, "Wrong filetype\n");
		fclose(f);
		return 0;
	}

	if (max != 255)
	{
		fprintf(stderr, "Wrong value of max color\n");
		fclose(f);
		return 0;
	}

	pixels = new unsigned char[w * h];

	//memory checking (closing file and deleting array)
	if (pixels == nullptr)
	{
		fprintf(stderr, "Not enoudh memory!\n");
		fclose(f);
		return 0;
	}

	fread(pixels, 1, w * h, f);
	fclose(f);
	return 1;
}

char WriteFile(char* filename)
{
	FILE* f;

	if ((f = fopen(filename, "wb")) == NULL)
	{
		fprintf(stderr, "Can't open output file\n");
		delete[] pixels;
		return 0;
	}

	fprintf(f, "P%i\n%i %i\n%i\n", type, w, h, max);
	fwrite(pixels, 1, w * h, f);

	fclose(f);
	delete[] pixels;
	return 1;
}

//graphic fucntions
float Correction(float color, float g) { return (g != 0) ? pow(color / max, 1 / g) * max : (color / max <= 0.0031308) ? 12.92 * color : (pow(1.055 * color / max, 1 / 2.4) - 0.055) * max; }

float ToNormal(float color, float g) { return (g != 0) ? pow(color / max, g) * max : (color / max <= 12.92 * 0.0031308) ? color / 12.92 : pow((color / max + 0.055), 2.4) / 1.055 * max; }

float TrueColor(float color) { return color < 0 ? 0 : color > max ? max : color; }

void PutPoint(bool is_swapped, float x, float y, float color, float g, float intensity)
{
	if (is_swapped)
		swap(x, y);
	if (OnPic(x, y))
		pixels[Rounding(y * w + x)] = (int)TrueColor(Correction(ToNormal(pixels[Rounding(y * w + x)], g) * (1 - intensity) + ToNormal(color, g) * intensity, g));
}

//easy line
void EasyLine(float x1, float y1, float x2, float y2, int color, float g, float thickness)
{
	float intensity = 0;
	char is_swapped = 0;
	if (thickness < 1)
	{
		color *= thickness;
		thickness = 1;
	}


	if (x1 == x2)
	{
		swap(x1, y1);
		swap(x2, y2);
		is_swapped = 1;
	}

	if (x1 > x2)
		swap(x1, x2);

	for (int i = x1; i <= round_up(x2); i++)
	{
		float begin = y1 - (thickness - 1) / 2, end = y1 + (thickness - 1) / 2;
		for (int j = int(begin); j <= round_up(end); j++)
		{
			float intensity_y = fmin(1, (thickness + 1) / 2 - fabs(j - y1));
			float intensity_x = (i == int(x1) && int(x1) != x1) ? i + 1 - x1 : (i == round_up(x2) && int(x2) != x2) ? x2 + 1 - i : intensity_y;
			float intensity = (intensity_x + intensity_y) / 2;
			PutPoint(is_swapped, i, j, color, g, intensity);
		}
	}
}

//wu line

float func(float k, float x, float x1, float y1) { return k * (x - x1) + y1; }

float fpartd(float num) { return num - int(num); }

float fpartu(float num) { return int(num) + 1 - num; }

void WuLine(float x1, float y1, float x2, float y2, int color, float g, float thickness)
{
	bool is_swaped = false;

	if (abs(x1 - x2) < abs(y1 - y2))
	{
		is_swaped = true;
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	if (thickness < 1)
	{
		color *= thickness;
		thickness += 0.5;
	}

	float k = (y2 - y1) / (x2 - x1);
	float thick_x = fabs(thickness * cos(atan(-1.0 / k))), thick_y = fabs(thickness * sin(atan(-1.0 / k)));
	
	for (int curr_x = int(x1 - thick_x / 2); curr_x <= round_up(x2 + thick_x / 2) ; curr_x++)
	{
		float y = func(k, curr_x, x1, y1);
		float begin = func((x1 - x2) / (y2 - y1), curr_x, x1, y1), end = func((x1 - x2) / (y2 - y1), curr_x, x2, y2);

		if (begin > end)
			swap(begin, end);
		begin = fmax(y - (thick_y) * fabs(k), begin - 1);
		end = fmin(y + (thick_y) * fabs(k), end) + 1;

		for (int curr_y = int(begin); curr_y <= int(end); curr_y++)
		{
			float intensity = 1;
//			intensity = fmin(1, fabs((end - begin + 1) / 2 - fabs((end + begin) / 2 - curr_y)));
			if (curr_y == int(begin))
				intensity = (fpartu(begin) >= 0.9 || fpartu(begin) <= 0.1) ? 0.5 : fpartu(begin);
			else if (curr_y == int(end))
				intensity = (fpartd(end) >= 0.9 || fpartd(end) <= 0.1) ? 0.5 : fpartd(end);
			PutPoint(is_swaped, curr_x, curr_y, color, g, intensity);
		}
	}
}

//main functions
void MakeLine(float x1, float y1, float x2, float y2, float g, int color, float thickness)
{
	if ((Rounding(x1 - x2) == 0) || (Rounding(y1 - y2) == 0))
		EasyLine(x1, y1, x2, y2, color, g, thickness);
	else
		WuLine(x1, y1, x2, y2, color, g, thickness);
}

int main(int argc, char** argv)
{
	if (argc < 9 || argc > 10)
	{
		fprintf(stderr, "Wrong data!\n");
		return 1;
	}

	if(!ReadFile(argv[1]))
		return 1;

	int color = (int)CharTofloat(argv[3]);
	float thickness = CharTofloat(argv[4]);
	float x1 = CharTofloat(argv[5]), y1 = CharTofloat(argv[6]), x2 = CharTofloat(argv[7]), y2 = CharTofloat(argv[8]);
	float g = argc < 10 ? 0 : CharTofloat(argv[9]);
	if (g < 0)
	{
		fprintf(stderr, "Gamma under zero!\n");
		return 1;
	}

	MakeLine(x1, y1, x2, y2, g, color, thickness);

	if (!WriteFile(argv[2]))
		return 1;

	return 0;
}