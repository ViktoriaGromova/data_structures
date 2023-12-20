#pragma once 

#include "base_conv.h"

void im2col(const float * src, int srcC, int srcH, int srcW,
    int kernelY, int kernelX, int dilationY, int dilationX, 
    int strideY, int strideX, int padY, int padX, int padH, int padW, float * buf, int dstH, int dstW)
{
  for (int sc = 0; sc < srcC; ++sc)
  {
    for (int ky = 0; ky < kernelY; ky++)
    {
      for (int kx = 0; kx < kernelX; kx++)
      {
        for (int dy = 0; dy < dstH; ++dy)
        {
          for (int dx = 0; dx < dstW; ++dx)
          {
            int sy = dy * strideY + ky * dilationY - padY;
            int sx = dx * strideX + kx * dilationX - padX;
            if (sy >= 0 && sy < srcH && sx >= 0 && sx < srcW)
              *buf++ = src[(sc*srcH + sy)*srcW + sx];
            else
              *buf++ = 0;
          }
        }
      }
    }
  }
}

void gemm_nn(int M, int N, int K, float alpha, const float * A, int lda, 
    float beta, const float * B, int ldb, float * C, int ldc)
{
  for (int i = 0; i < M; ++i)
  {
    for (int j = 0; j < N; ++j)
    {
      C[i*ldc + j] = beta;
      for (int k = 0; k < K; ++k)
        C[i*ldc + j] += alpha * A[i*lda + k] * B[k*ldb + j];
    }
  }
}

void convolution_opt1(const float * src, int batch, int srcC, int srcH, int srcW,
    int kernelY, int kernelX, int dilationY, int dilationX,
    int strideY, int strideX, int padY, int padX, int padH, int padW, int group,
    const float * weight, const float * bias, float * dst, int dstC, float * buf, int dstH, int dstW)
{
  int M = dstC / group;
  int N = dstH * dstW;
  int K = srcC * kernelY * kernelX / group;
  for (int b = 0; b < batch; ++b)
  {
    im2col(src, srcC, srcH, srcW, kernelY, kernelX, dilationY, dilationX,
      strideY, strideX, padY, padX, padH, padW, buf, dstH, dstW);
    for (int g = 0; g < group; ++g)
      gemm_nn(M, N, K, 1, weight + M * K * g, K, 0, buf + N * K * g, N, dst + M * N * g, N);
    for (int i = 0; i < dstC; ++i)
      for (int j = 0; j < N; ++j)
        dst[i*N+ j] = relu(dst[i*N + j] + bias[i]);
    src += srcC*srcH*srcW;
    dst += dstC*dstH*dstW;
  }
}