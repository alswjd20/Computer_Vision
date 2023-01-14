#pragma warning(disable:4996)

#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>


void Binarization(BYTE* Img, BYTE* Out, int W, int H, BYTE Threshold)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] < Threshold) Out[i] = 0;
		else Out[i] = 255;
	}
}


int GozalezBinThresh(BYTE* Img, int W, int H)
{
	int ImgSize = W * H;
	int diff = 0;
	int Thres = 125;
	int new_Thres = 0;

	do {
		int count_small = 0, count_big = 0, sum_small = 0, sum_big = 0;

		for (int i = 0; i < ImgSize; i++) {
			if (Img[i] < Thres) {
				count_small += 1;
				sum_small += Img[i];
			}
			else {
				count_big += 1;
				sum_big += Img[i];
			}
		}
		double average_small = (double)(sum_small / count_small);
		double average_big = (double)(sum_big / count_big);

		int new_Thres = (int)((average_small + average_big)/2);
		int diff = abs(new_Thres - Thres);
		Thres = new_Thres;

	} while(diff > 3);

	return Thres;
}


void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB,
	BYTE* Output, int W, int H, const char* FileName) {

	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W*H, fp);
	fclose(fp);

}

int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트

	FILE* fp = fopen("C:\\Users\\minje\\Desktop\\Project2\\coin.bmp", "rb");
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Thres = GozalezBinThresh(Image, hInfo.biWidth, hInfo.biHeight);
	Binarization(Image, Output, hInfo.biWidth, hInfo.biHeight, Thres);
	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output.bmp");

	BITMAPFILEHEADER hf2; // 14바이트
	BITMAPINFOHEADER hInfo2; // 40바이트
	RGBQUAD hRGB2[256]; // 1024바이트

	FILE* fp2 = fopen("C:\\Users\\minje\\Desktop\\Project2\\LENNA.bmp", "rb");
	fread(&hf2, sizeof(BITMAPFILEHEADER), 1, fp2);
	fread(&hInfo2, sizeof(BITMAPINFOHEADER), 1, fp2);
	fread(hRGB2, sizeof(RGBQUAD), 256, fp2);
	int ImgSize2 = hInfo2.biWidth * hInfo2.biHeight;
	BYTE* Image2 = (BYTE*)malloc(ImgSize2);
	BYTE* Output2 = (BYTE*)malloc(ImgSize2);
	fread(Image2, sizeof(BYTE), ImgSize2, fp2);
	fclose(fp2);

	int Thres2 = GozalezBinThresh(Image2, hInfo2.biWidth, hInfo2.biHeight);
	Binarization(Image2, Output2, hInfo2.biWidth, hInfo2.biHeight, Thres2);
	SaveBMPFile(hf2, hInfo2, hRGB, Output2, hInfo2.biWidth, hInfo2.biHeight, "output2.bmp");

	
	free(Image);
	free(Output);
	free(Image2);
	free(Output2);

	return 0;
}