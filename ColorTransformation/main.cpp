#define _CRT_SECURE_NO_WARNINGS

#include <iostream>

#define HEIGHT 512
#define WIDTH 512

using namespace std;

typedef struct RGB {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

typedef struct Point {
	int x;
	int y;
};

template<typename T> T** MemAlloc2D(int nHeight, int nWidth, RGB initRGB)
{
	T** rtn = new T*[nHeight];
	for (int h = 0; h < nHeight; h++)
	{
		rtn[h] = new T[nWidth];
		for (int w = 0; w < nWidth; w++)
		{
			rtn[h][w] = initRGB;
		}
	}
	return rtn;
}

template<typename T> void MemFree2D(T** arr2D, int nHeight)
{
	for (int h = 0; h < nHeight; h++)
	{
		delete[] arr2D[h];
	}
	delete[] arr2D;
}

void FileRead(const char* strFilename, RGB** arr2D, int nHeight, int nWidth)
{
	FILE* fp_in = fopen(strFilename, "rb");
	for (int h = 0; h < nHeight; h++)
	{
		fread(arr2D[h], sizeof(RGB), nWidth, fp_in);
	}

	fclose(fp_in);
}

void FileWrite(const char* strFilename, RGB** arr2D, int nHeight, int nWidth)
{
	FILE* fp_out = fopen(strFilename, "wb");
	for (int h = 0; h < nHeight; h++)
	{
		fwrite(arr2D[h], sizeof(RGB), nWidth, fp_out);
	}

	fclose(fp_out);
}
template<typename T> void ConverterRgb(T** in)
{	// update order BGR file to RGB file.
	unsigned char temp;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			temp = in[y][x].r;
			in[y][x].r = in[y][x].b;
			in[y][x].b = temp;
		}
	}
}

template<typename T> void WhiteBalancing(T** in, T** out, Point pos)
{	// update order BGR file to RGB file.
	unsigned char temp;
	double rate[3] = { 0,0,0 };
	unsigned char* position = (unsigned char*)addressof(in[pos.y][pos.x]);
	for (int i = 0; i < 3; i++)
	{
		rate[i] = 255.0 / *(position + i);
	}


	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			for (int i = 0; i < 3; i++)
			{
				unsigned char* inPixel = (unsigned char*)addressof(in[y][x]) + i;
				unsigned char* outPixel = (unsigned char*)addressof(out[y][x]) + i;

				*outPixel = ( (*inPixel) * rate[i] > 255.0) ? 255.0 : (*inPixel) * rate[i];
			}
		}
	}
}

void main()
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// File Open & Memory Allocation

	RGB** orangeArr2D = MemAlloc2D<RGB>(HEIGHT, WIDTH, { 0,0,0 });
	RGB** whiteArr2D = MemAlloc2D<RGB>(HEIGHT, WIDTH, { 0, 0, 0 });

	FileRead("orange.raw", orangeArr2D, HEIGHT, WIDTH);
	WhiteBalancing(orangeArr2D, whiteArr2D, { 300,200 });

	FileWrite("white1.raw", whiteArr2D, HEIGHT, WIDTH);

	return;
}