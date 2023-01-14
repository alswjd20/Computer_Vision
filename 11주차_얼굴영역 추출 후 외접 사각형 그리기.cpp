#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	if (hInfo.biBitCount == 24) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(Output, sizeof(BYTE), W * H * 3, fp);
	}
	else if (hInfo.biBitCount == 8) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), W * H, fp);
	}
	fclose(fp);
}

void FillColor(BYTE* Image, int X, int Y, int W, int H, BYTE R, BYTE G, BYTE B)
{
	Image[Y * W * 3 + X * 3] = B; // Blue 성분
	Image[Y * W * 3 + X * 3 + 1] = G; // Green 성분
	Image[Y * W * 3 + X * 3 + 2] = R; // Red 성분
}

void RGB2YCbCr(BYTE* Image, BYTE* Y, BYTE* Cb, BYTE* Cr, int W, int H)
{
	for (int i = 0; i < H; i++) { // Y좌표
		for (int j = 0; j < W; j++) { // X좌표
			Y[i * W + j] = (BYTE)(0.299 * Image[i * W * 3 + j * 3 + 2] + 0.587 * Image[i * W * 3 + j * 3 + 1] + 0.114 * Image[i * W * 3 + j * 3]);
			Cb[i * W + j] = (BYTE)(-0.16874 * Image[i * W * 3 + j * 3 + 2] - 0.3313 * Image[i * W * 3 + j * 3 + 1] + 0.5 * Image[i * W * 3 + j * 3] + 128.0);
			Cr[i * W + j] = (BYTE)(0.5 * Image[i * W * 3 + j * 3 + 2] - 0.4187 * Image[i * W * 3 + j * 3 + 1] - 0.0813 * Image[i * W * 3 + j * 3] + 128.0);
		}
	}
}

void DrawRectOutline(BYTE* Img, int W, int H, int LU_X, int LU_Y, int RD_X, int RD_Y)
{
	for (int i = LU_X; i < RD_X; i++) 	FillColor(Img, i, LU_Y, W, H, 255, 0, 0);
	for (int i = LU_X; i < RD_X; i++) 	FillColor(Img, i, RD_Y, W, H, 255, 0, 0);
	for (int i = LU_Y; i < RD_Y; i++) 	FillColor(Img, LU_X, i, W, H, 255, 0, 0);
	for (int i = LU_Y; i < RD_Y; i++) 	FillColor(Img, RD_X, i, W, H, 255, 0, 0);
}

void Obtain2DBoundingBox(BYTE* Image, int W, int H, int* LUX, int* LUY, int* RDX, int* RDY)
{
	int flag = 0;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Image[i * W * 3 + j * 3] != 255) { // 배경을 흰색으로 처리했습니다
				*LUY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;
	for (int i = H - 1; i >= 0; i--) {
		for (int j = 0; j < W; j++) {
			if (Image[i * W * 3 + j * 3] != 255) {
				*RDY = i;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;

	for (int j = 0; j < W; j++) {
		for (int i = 0; i < H; i++) {
			if (Image[i * W * 3 + j * 3] != 255) {
				*LUX = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
	flag = 0;
	for (int j = W - 1; j >= 0; j--) {
		for (int i = 0; i < H; i++) {
			if (Image[i * W * 3 + j * 3] != 255) {
				*RDX = j;
				flag = 1;
				break;
			}
		}
		if (flag == 1) break;
	}
}


int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("C:\\Users\\minje\\Desktop\\Project1\\face.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int H = hInfo.biHeight, W = hInfo.biWidth;
	BYTE* Image;
	BYTE* Output;
	if (hInfo.biBitCount == 24) { // 트루컬러
		Image = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else { // 인덱스(그레이)
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	fclose(fp);


	BYTE* Y = (BYTE*)malloc(ImgSize);
	BYTE* Cb = (BYTE*)malloc(ImgSize);
	BYTE* Cr = (BYTE*)malloc(ImgSize);

	RGB2YCbCr(Image, Y, Cb, Cr, W, H);

	// 피부영역만 검출 (Y, Cb, Cr 모델 기준)
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if (Cb[i * W + j] < 127 && Cb[i * W + j] > 85 
				&& Cr[i * W + j] < 173 && Cr[i * W + j] > 140) {
				Output[i * W * 3 + j * 3] = Image[i * W * 3 + j * 3];
				Output[i * W * 3 + j * 3 + 1] = Image[i * W * 3 + j * 3 + 1];
				Output[i * W * 3 + j * 3 + 2] = Image[i * W * 3 + j * 3 + 2];
			}
			else
				Output[i * W * 3 + j * 3] = Output[i * W * 3 + j * 3 + 1] = Output[i * W * 3 + j * 3 + 2] = 255;
			// 배경을 흰 색으로 처리했습니다
		}
	}


	int LUX, LUY, RDX, RDY;
	Obtain2DBoundingBox(Output, W, H, &LUX, &LUY, &RDX, &RDY);
	DrawRectOutline(Output, W, H, LUX, LUY, RDX, RDY);

	free(Y);
	free(Cb);
	free(Cr);

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "output.bmp");

	free(Image);
	free(Output);
	return 0;
}