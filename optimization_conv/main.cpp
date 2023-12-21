#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>

#include "base_conv.h"
#include "opt1_conv.h"

using Timer = std::chrono::time_point<std::chrono::high_resolution_clock>;

Timer now()
{
    return std::chrono::high_resolution_clock::now();
}

double getDuration(Timer start, Timer end)
{
    return static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) * 1e-6;
}

int main()
{
    setlocale(LC_ALL, "");
    const int batch = 4;  // Количество изображений в пакете
    const int srcC = 3;   // Количество входных каналов
    const int srcH = 100; // Высота входа
    const int srcW = 100; // Ширина входа

    const int kernelY = 7; // Высота фильтра
    const int kernelX = 7; // Ширина фильтра

    const int dilationY = 1; // Недострение по вертикали
    const int dilationX = 1; // Недострение по горизонтали

    const int strideY = 1; // Шаг по вертикали
    const int strideX = 1; // Шаг по горизонтали

    const int padY = 1; // Паддинг по вертикали
    const int padX = 1; // Паддинг по горизонтали
    const int padH = 0; // Паддинг до
    const int padW = 0; // Паддинг после

    const int group = 1;  // Количество групп свертки
    const int dstC = 100; // Количество выходных каналов

    const int dstH = (srcH + padY + padH - (dilationY * (kernelY - 1) + 1)) / strideY + 1;
    const int dstW = (srcW + padX + padW - (dilationX * (kernelX - 1) + 1)) / strideX + 1;

    const int srcSize = batch * srcC * srcH * srcW;
    const int weightSize = group * dstC * srcC * kernelY * kernelX;
    const int biasSize = group * dstC;
    const int dstSize = batch * group * dstC * dstH * dstW;

    auto *src = new float[srcSize];
    auto *weight = new float[weightSize];
    auto *bias = new float[biasSize];
    auto *dst = new float[dstSize];

    std::fill(src, src + srcSize, 1.0f);
    std::fill(weight, weight + weightSize, 1.0f);
    std::fill(bias, bias + biasSize, 1.0f);

    double conv[100];
    std::cout << "Ref :" << std::endl;
    float *buf = (float *)malloc(dstH * dstW * srcC * kernelY * kernelX * sizeof(float));
    for (int i = 0; i < 100; ++i)
    {
        Timer start = now();
        convolution(src, batch, srcC, srcH, srcW, kernelY, kernelX, dilationY, dilationX,
                    strideY, strideX, padY, padX, padH, padW, group, weight, bias, dst, dstC, dstH, dstW);
        Timer end = now();
        *(conv + i) = getDuration(start, end);
    }
    std::sort(conv, conv + 100);
    std::cout << (conv[50] + conv[49]) / 2 << " ms" << std::endl;

    double conv_opt1[100];
    std::cout << "Opt :" << std::endl;
    for (int i = 0; i < 100; ++i)
    {
        Timer start = now();
        float *buf = (float *)malloc(dstH * dstW * srcC * kernelY * kernelX * sizeof(float));
        convolution_opt1(src, batch, srcC, srcH, srcW, kernelY, kernelX, dilationY, dilationX,
                         strideY, strideX, padY, padX, padH, padW, group, weight, bias, dst, dstC, buf, dstH, dstW);
        Timer end = now();
        *(conv_opt1 + i) = getDuration(start, end);
    }
    std::sort(conv_opt1, conv_opt1 + 100);
    std::cout << (conv_opt1[50] + conv_opt1[49]) / 2 << " ms" << std::endl;
    return 0;
}