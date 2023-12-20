#pragma once

#include <immintrin.h>

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