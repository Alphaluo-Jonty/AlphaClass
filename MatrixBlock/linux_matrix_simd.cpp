﻿#include <sys/time.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <immintrin.h>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;


static int matrixN[4] = { 512, 1024, 2048, 4096 };
static int blockM[5] = { 8, 16, 32, 64, 128 };
static vector<double> vecBaseTime;
static vector<double> vecSIMDTime;
static vector<double> vecEigenTime;

static struct timeval startTime;
static struct timeval endTime;
static unsigned long diff;

#define N 1024

#define FLOAT_SIZE 8 // 一个AVX向量中包含8个单精度浮点数
#define BYTE_SIZE 16 // 一个AVX向量中包含32个字节



float rand_float(float s) {
    return 4 * s * (1 - s);
}

void matrix_gen(float* a, float* b, int n, float seed) {
    float s = seed;
    for (int i = 0; i < n * n; ++i) {
        s = rand_float(s);
        a[i] = s;
        s = rand_float(s);
        b[i] = s;
    }
}

// 一维数组的矩阵乘法--串行
void matrix_multiply(float* A, float* B, int n, float* C) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            for (int k = 0; k < n; ++k) {
                C[n * i + j] += A[n * i + k] * B[n * k + j];
            }
        }
    }
}

// 二维数组矩阵乘法--串行
void matrix(float** a, float** b, float** c, int M) {
	int i, j, k;
	for (i = 0; i < M; i++) {
		for (k = 0; k < M; k++) {
			for (j = 0; j < M; j = j + 1) {
				c[i][j] += a[i][k] * b[k][j];
			}
		}
	}
}

//两层循环展开
void matrix_loop_two(float** a, float** b, float** c, int M) {
	int i, j, k;
	for (i = 0; i < M; i++) {
		for (k = 0; k < M; k++) {
			for (j = 0; j < M; j = j + 2) {
				c[i][j] += a[i][k] * b[k][j];
				c[i][j + 1] += a[i][k] * b[k][j + 1];
			}
		}
	}
}

// 矩阵相乘（分块）
void matrix_multiply_block(float* A, float* B, int m, float* C) {
    // 分块矩阵乘法, m是小矩阵的阶数
    int bm = m * (N / m);
    int r, c, k, b, d;
    int sum;
    for (k = 0; k < bm; k += m) {
        for (d = 0; d < bm; d += m) {
            for (r = 0; r < N; ++r) {
                for (c = d; c < d + m; ++c) {
                    sum = C[r * N + c];
                    for (b = k; b < k + m; ++b) {
                        sum += A[r * N + k] * B[k * N + c];
                    }
                    C[r * N + c] = sum;
                }
            }
        }
    }

}

// 分块矩阵相乘--串行
inline void base_block_multiply(int n, int block_size, float* A, float* B, float* C) {
	int i, j, k;
	for (i = 0; i < block_size; ++i) {
		for (j = 0; j < block_size; ++j) {
			for (k = 0; k < block_size; ++k) {
				C[i * n + j] += A[i * n + k] * B[k * n + j];
			}
		}
	}
}


// 矩阵相乘（分块）--串行
inline void matrix_mul_bk(int n, int block_size, float* A, float* B, float* C) {
	for (int sj = 0; sj < n; sj += block_size) {
		for (int si = 0; si < n; si += block_size) {
			for (int sk = 0; sk < n; sk += block_size) {
				// base_block_multiply(n, block_size, A + si * n + sk, \
					B + sk * n + sj, C + si * n + sj);
				int i, j, k;
				float* a1 = A + si * n + sk;
				float* b1 = B + sk * n + sj;
				float* c1 = C + si * n + sj;
				for (i = 0; i < block_size; ++i) {
					for (j = 0; j < block_size; ++j) {
						for (k = 0; k < block_size; ++k) {
							c1[i * n + j] += a1[i * n + k] * b1[k * n + j];
						}
					}
				}
			}	
		}			
	}	
}

// 矩阵相乘（分块）--AVX
void matrix_mul_bk_avx(int n, int block_size, float* A, float* B, float* C) {
	for (int sj = 0; sj < n; sj += block_size) {
		for (int si = 0; si < n; si += block_size) {
			for (int sk = 0; sk < n; sk += block_size) {
				int i, j;
				float* a1 = A + si * n + sk;  // 当前的分块矩阵a1
				float* b1 = B + sk * n + sj;
				float* c1 = C + si * n + sj;
				// 为使用SIMD指令，对b1进行转置
				float* b1_t = (float*)_mm_malloc(sizeof(float) * block_size * block_size, 64);
				for (int ti = 0; ti < block_size; ++ti) {
					for (int tj = 0; tj < block_size; ++tj) {
						b1_t[tj * block_size + ti] = b1[ti * block_size + tj];
					}
				}
				for (i = 0; i < block_size; ++i) {
					for (j = 0; j < block_size; ++j) {
						//加载数组当前元素的寄存器，每次读取8个单精度浮点数
						__m256 va1 = _mm256_setzero_ps();
						__m256 vb1 = _mm256_setzero_ps();
						__m256 vc1 = _mm256_setzero_ps();
						__m256 sum_vc = _mm256_setzero_ps();
						for (int m = 0; m < block_size; m += FLOAT_SIZE) {
							va1 = _mm256_load_ps(&a1[i * block_size + m]);
							vb1 = _mm256_load_ps(&b1_t[j * block_size + m]);
							sum_vc = _mm256_fmadd_ps(va1, vb1, sum_vc);
						}			
						// 累加求和
						sum_vc = _mm256_add_ps(sum_vc, _mm256_permute2f128_ps(sum_vc, sum_vc, 1));
						sum_vc = _mm256_hadd_ps(sum_vc, sum_vc);
						c1[i * n + j] = _mm256_cvtss_f32(_mm256_hadd_ps(sum_vc, sum_vc));
					}
				}
				// 释放_mm_malloc分配的内存
				_mm_free(b1_t);
			}
		}
	}
}



//数组转置存储
void rotate(float** b, float** copy_b, int M) {
	int i, j;
	for (i = 0; i < M; i++) {
		for (j = 0; j < M; j++) {
			copy_b[j][i] = b[i][j];
		}
	}
}

void run_solution_simd(int n, int m)
{
	// 在堆中存放矩阵
	float* A = (float*)_mm_malloc(sizeof(float) * n * n, 64); // 按64字节对齐
	float* B = (float*)_mm_malloc(sizeof(float) * n * n, 64);
	float* C = (float*)_mm_malloc(sizeof(float) * n * n, 64);

	// 随机生成矩阵A和B
	matrix_gen(A, B, n, 0.8);

	gettimeofday(&startTime, NULL);

	// 矩阵相乘
	matrix_mul_bk_avx(n, m, A, B, C);

	gettimeofday(&endTime, NULL);
	diff = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
	// 输出计时结果，以微秒为单位
	cout << "N = " << n << ", M = " << m << ", time cost: " << diff << " us" << endl;
	vecSIMDTime.push_back(diff);
	// 释放
	_mm_free(A);
	_mm_free(B);
	_mm_free(C);
}

void run_solution_base(int n, int m)
{
	// 在堆中存放矩阵
	float* A = new float[ n * n ];
	float* B = new float[ n * n ];
	float* C = new float[ n * n ];

	// 随机生成矩阵A和B
	matrix_gen(A, B, n, 0.8);

	gettimeofday(&startTime, NULL);
	// 矩阵相乘
	matrix_mul_bk(n, m, A, B, C);

	gettimeofday(&endTime, NULL);
	diff = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
	// 输出计时结果，以微秒为单位
	cout << "N = " << n << ", M = " << m << ", time cost: " << diff << " us" << endl;

	vecBaseTime.push_back(diff);
	// 释放内存
	delete[] A;
	delete[] B;
	delete[] C;
}

void run_solution_eigen(int n)
{
	// 在堆中存放矩阵,用MatrixXf类型
	MatrixXf A(n, n);
	MatrixXf B(n, n);
	MatrixXf C(n, n);

	// 随机生成矩阵A和B
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			A(i, j) = rand_float(0.8);
			B(i, j) = rand_float(0.8);
			C(i, j) = 0;
		}
	}

	gettimeofday(&startTime, NULL);
	// 矩阵相乘
	C = A * B;

	gettimeofday(&endTime, NULL);
	diff = 1000000 * (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_usec - startTime.tv_usec);
	// 输出计时结果，以微秒为单位
	cout << "N = " << n << ", time cost: " << diff << " us" << endl;

	vecEigenTime.push_back(diff);
}

int main()
{
	run_solution_simd(2048, 64);
	cout << setprecision(5) << vecSIMDTime[0] / 1000 << endl;
	// int index = 0;
	// for (int i = 0; i < 4; ++i) {
	// 	for (int j = 0; j < 5; ++j) {
	// 		run_solution_base(matrixN[i], blockM[j]);
	// 		run_solution_simd(matrixN[i], blockM[j]);
	// 		cout << "SMID 加速比：" << setprecision(4) << vecBaseTime[index] / vecSIMDTime[index] << endl;
	// 		run_solution_eigen(matrixN[i]);
	// 		cout << "Eigen 加速比："<<  setprecision (4) << vecBaseTime[index] / vecEigenTime[index] << endl;
	// 		cout << " " << endl;
	// 		index++;
	// 	}
	// }

    return 0;
}
