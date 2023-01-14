#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo,
	RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);
	fclose(fp);
}
void swap(BYTE* a, BYTE* b)
{
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}
BYTE Median(BYTE* arr, int size)
{
	// 오름차순 정렬
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // 비교대상 index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S / 2];
}
int main()
{
	BITMAPFILEHEADER hf; // 14바이트
	BITMAPINFOHEADER hInfo; // 40바이트
	RGBQUAD hRGB[256]; // 1024바이트
	FILE* fp;
	fp = fopen("C:\\Users\\minje\\Desktop\\Project1\\lenna_impulse.bmp", "rb");
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Temp = (BYTE*)malloc(ImgSize); // 임시배열
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	/* Median filtering */
	int Length = 9;  // 마스크의 한 변의 길이

	int Margin = Length / 2; // 마진은, 마스크의 한 변의 길이 나누기 2 한 정수값

	int WSize = Length * Length; // WSize는 마스크의 크기

	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); // temp라는 배열에 마스크의 크기인 WSize만큼 동적할당

	int W = hInfo.biWidth, H = hInfo.biHeight;  // 영상의 가로 길이와 세로 길이를 각각의 변수에 저장

	int i, j, m, n;

	for (i = Margin; i < H - Margin; i++) { // 영상의 Y좌표(행)

		for (j = Margin; j < W - Margin; j++) { // 영상의 X좌표(열)

			for (m = -Margin; m <= Margin; m++) { // 마스크의 행

				for (n = -Margin; n <= Margin; n++) { // 마스크의 열

					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n];
					// 마스크의 행과 열을 움직여가며, 영상의 화소들(2차원)을 1차원 배열인 Temp에 저장

				}

			}

			Output[i * W + j] = Median(temp, WSize); // Median함수에 temp배열과 그 사이즈를 넣어 반환된 중간값을 output배열에 저장

		}

	} // 영상의 X좌표와 Y좌표를 바꿔가며 반복 실행 

	free(temp);
	/* Median filtering */

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_9.bmp");


	free(Image);
	free(Output);
	free(Temp);
	return 0;
}