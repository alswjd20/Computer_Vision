#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void main()
{
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hinfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	fp = fopen("C:\\Users\\minje\\Desktop\\Project1\\LENNA.bmp", "rb");
	if(fp == NULL) return;
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hinfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hinfo.biWidth * hinfo.biHeight;
	BYTE * Image = (BYTE *)malloc(ImgSize);

	BYTE * output1 = (BYTE *)malloc(ImgSize);
	BYTE * output2 = (BYTE *)malloc(ImgSize);
	BYTE * output3 = (BYTE *)malloc(ImgSize);

	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	for (int i = 0; i < ImgSize; i++)
		output1[i] = Image[i];

	fp = fopen("output1.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output1, sizeof(BYTE), ImgSize, fp);
	free(output1);
	fclose(fp);

	for (int i = 0; i < ImgSize; i++)
		output2[i] = Image[i] + 50;
	// 별도 overflow로 인한 클리핑처리는 필요없다 하셔서 하지 않았습니다.

	fp = fopen("output2.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output2, sizeof(BYTE), ImgSize, fp);
	free(output2);
	fclose(fp);

	for (int i = 0; i < ImgSize; i++)
		output3[i] = 255 - Image[i];

	fp = fopen("output3.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hinfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(output3, sizeof(BYTE), ImgSize, fp);
	free(output3);
	fclose(fp);

	free(Image);

}
