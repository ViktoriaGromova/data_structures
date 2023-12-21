#pragma once



float relu(float value)
{
    return value > 0 ? value : 0;
}

void convolution(const float *src, int batch, int srcC, int srcH, int srcW,
                 int kernelY, int kernelX, int dilationY, int dilationX,
                 int strideY, int strideX, int padY, int padX, int padH, int padW, int group,
                 const float *weight, const float *bias, float *dst, int dstC, int dstH, int dstW)
{
    dstC = dstC / group;
    srcC = srcC / group;
    for (int b = 0; b < batch; ++b)
    {
        for (int g = 0; g < group; ++g)
        {
            for (int dc = 0; dc < dstC; ++dc)
            {
                for (int dy = 0; dy < dstH; ++dy)
                {
                    for (int dx = 0; dx < dstW; ++dx)
                    {
                        float sum = 0;
                        for (int sc = 0; sc < srcC; ++sc)
                        {
                            for (int ky = 0; ky < kernelY; ky++)
                            {
                                for (int kx = 0; kx < kernelX; kx++)
                                {
                                    int sy = dy * strideY + ky * dilationY - padY;
                                    int sx = dx * strideX + kx * dilationX - padX;
                                    if (sy >= 0 && sy < srcH && sx >= 0 && sx < srcW)
                                        sum +=
                                            src[(((b * group + g) * srcC + sc) * srcH + sy) * srcW + sx] *
                                            weight[(((g * dstC + dc) * srcC + sc) * kernelY + ky) * kernelX + kx];
                                }
                            }
                        }
                        dst[(((b * group + g) * dstC + dc) * dstH + dy) * dstW + dx] = sum + bias[g * dstC + dc];
                    }
                }
            }
        }
    }
}