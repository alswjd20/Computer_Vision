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
	// �������� ����
	const int S = size;
	for (int i = 0; i < size - 1; i++) // pivot index
	{
		for (int j = i + 1; j < size; j++) // �񱳴�� index
		{
			if (arr[i] > arr[j]) 	swap(&arr[i], &arr[j]);
		}
	}
	return arr[S / 2];
}
int main()
{
	BITMAPFILEHEADER hf; // 14����Ʈ
	BITMAPINFOHEADER hInfo; // 40����Ʈ
	RGBQUAD hRGB[256]; // 1024����Ʈ
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
	BYTE* Temp = (BYTE*)malloc(ImgSize); // �ӽù迭
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	/* Median filtering */
	int Length = 9;  // ����ũ�� �� ���� ����

	int Margin = Length / 2; // ������, ����ũ�� �� ���� ���� ������ 2 �� ������

	int WSize = Length * Length; // WSize�� ����ũ�� ũ��

	BYTE* temp = (BYTE*)malloc(sizeof(BYTE) * WSize); // temp��� �迭�� ����ũ�� ũ���� WSize��ŭ �����Ҵ�

	int W = hInfo.biWidth, H = hInfo.biHeight;  // ������ ���� ���̿� ���� ���̸� ������ ������ ����

	int i, j, m, n;

	for (i = Margin; i < H - Margin; i++) { // ������ Y��ǥ(��)

		for (j = Margin; j < W - Margin; j++) { // ������ X��ǥ(��)

			for (m = -Margin; m <= Margin; m++) { // ����ũ�� ��

				for (n = -Margin; n <= Margin; n++) { // ����ũ�� ��

					temp[(m + Margin) * Length + (n + Margin)] = Image[(i + m) * W + j + n];
					// ����ũ�� ��� ���� ����������, ������ ȭ�ҵ�(2����)�� 1���� �迭�� Temp�� ����

				}

			}

			Output[i * W + j] = Median(temp, WSize); // Median�Լ��� temp�迭�� �� ����� �־� ��ȯ�� �߰����� output�迭�� ����

		}

	} // ������ X��ǥ�� Y��ǥ�� �ٲ㰡�� �ݺ� ���� 

	free(temp);
	/* Median filtering */

	SaveBMPFile(hf, hInfo, hRGB, Output, hInfo.biWidth, hInfo.biHeight, "median_9.bmp");


	free(Image);
	free(Output);
	free(Temp);
	return 0;
}