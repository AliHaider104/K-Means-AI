#include<iostream>
#include<fstream>
#include<conio.h>
#include<time.h>
#include<math.h>
#include"Header.h"
using namespace std;

int ROW=50;
int COL=66;

struct RGB
{
	int R;
	int G;
	int B;
};

RGB** GetImage()
{
	// HEAP MEMORY
	RGB** img = new RGB*[ROW];

	for (int i = 0; i < ROW; i++)
	{
		img[i] = new RGB[COL];
	}

	ifstream fin;
	fin.open("YourImage.txt");

	int g1;
	char g2;

	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			// READING ACCORIDNG TO .TXT FORMAT
			fin >> g1;
			fin >> g2;
			fin >> g1;
			fin >> g2;
			fin >> g2;
			fin >> img[i][j].R;
			fin >> g2;
			fin >> img[i][j].G;
			fin >> g2;
			fin >> img[i][j].B;
			fin >> g2;
		}
	}

	return img;
}

void MakeAnImageG(RGB** TargetImg)
{
	int x1 = 400, x2 = 402, y1 = 40, y2 = 42;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			myRect(x1, y1, x2, y2, TargetImg[i][j].R, TargetImg[i][j].G, TargetImg[i][j].B, TargetImg[i][j].R, TargetImg[i][j].G, TargetImg[i][j].B);
			x1 += 2;
			x2 += 2;
		}

		y1 += 2;
		y2 += 2;
		x1 = 400;
		x2 = 402;
	}
}

void Random(RGB* Means, int MeanCount)
{
	srand(time(0));
	for (int i = 0; i < MeanCount; i++)
	{
		Means[i].R=rand()%255;
		Means[i].G=rand()%255;
		Means[i].B=rand()%255;
	}
}

double Distance(int R, int G, int B, int mR, int mG, int mB)
{
	return(sqrt((R-mR)*(R - mR) + (B-mB)*(B-mB) + (G-mG)*(G-mG)));
}

int SmallestDistance(double* Distance,int size)
{
	double min = 999;
	int index = -1;
	for (int i = 0; i < size; i++)
	{
		if (Distance[i] < min)
		{
			min = Distance[i];
			index = i;
		}
	}
	return index;
}

void K_Means(RGB** img)
{
	cout << "Enter Number Of Means : ";
	int MeanCount = 0;
	cin >> MeanCount;

	RGB* Means = new RGB[MeanCount];

	Random(Means, MeanCount);

	//--> B array
	int SAMPLE = ROW * COL;
	int** b = new int*[SAMPLE];
	for (int i = 0; i < SAMPLE; i++)
	{
		b[i] = new int[MeanCount];
	}


	//--->
	int itCount = 0;
	while (itCount < 1000) {

		for (int i = 0; i < SAMPLE; i++)
		{
			for (int j = 0; j < MeanCount; j++)
			{
				b[i][j] = 0;
			}
		}

		double* DISTANCES = new double[MeanCount];
		int pixelNumber = 0;

		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				for (int k = 0; k < MeanCount; k++)
				{
					DISTANCES[k] = Distance(img[i][j].R, img[i][j].G, img[i][j].B, Means[k].R, Means[k].G, Means[k].B);
				}

				int index = SmallestDistance(DISTANCES, MeanCount);
				b[pixelNumber][index] = 1;
				pixelNumber++;
			}
		}

		RGB** dot = new RGB*[SAMPLE];
		for (int i = 0; i < SAMPLE; i++)
		{
			dot[i] = new RGB[MeanCount];
		}


		//----> DOT PRODUCT

		int SampleNumber = 0;
		for (int i = 0; i < ROW; i++)
		{
			for (int j = 0; j < COL; j++)
			{
				for (int k = 0; k < MeanCount; k++)
				{
					dot[SampleNumber][k].R = b[SampleNumber][k] * img[i][j].R;
					dot[SampleNumber][k].G = b[SampleNumber][k] * img[i][j].G;
					dot[SampleNumber][k].B = b[SampleNumber][k] * img[i][j].B;
				}

				SampleNumber++;
			}
		}

		RGB* newMeans = new RGB[MeanCount];
		int counter = 0;

		for (int i = 0; i < MeanCount; i++)
		{
			newMeans[i].R = 0;
			newMeans[i].G = 0;
			newMeans[i].B = 0;
			counter = 0;

			for (int k = 0; k < SAMPLE; k++)
			{
				newMeans[i].R = newMeans[i].R + dot[k][i].R;
				newMeans[i].G = newMeans[i].G + dot[k][i].G;
				newMeans[i].B = newMeans[i].B + dot[k][i].B;

				if (b[k][i] == 1)
					counter++;
			}

			if (counter != 0) {
				newMeans[i].R = newMeans[i].R / counter;
				newMeans[i].G = newMeans[i].G / counter;
				newMeans[i].B = newMeans[i].B / counter;
			}
		}

		delete[] Means;
		Means = newMeans;
		newMeans = NULL;

		itCount++;
	}

	int sampleCount = 0;
	for (int i = 0; i < ROW; i++)
	{
		for (int j = 0; j < COL; j++)
		{
			for (int k = 0; k < MeanCount; k++)
			{
				if (b[sampleCount][k] == 1)
				{
					img[i][j].R = Means[k].R;
					img[i][j].G = Means[k].G;
					img[i][j].B = Means[k].B;
				}
			}
			sampleCount++;
		}
	}

}

int main()
{
	RGB** Img=GetImage();
	cout << "ORIGINAL IMAGE : ";
	MakeAnImageG(Img);
	_getch();
	system("cls");

	K_Means(Img);
	MakeAnImageG(Img);
	_getch();
}