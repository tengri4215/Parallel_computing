#include <iostream>
#include <fstream>
#include <format>
#include <chrono>
#include <cassert>

using namespace std;

const std::initializer_list<int> firstTileSizes = { 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> secondTileNumbers = { 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> matrixSizes = {30};
const std::initializer_list<int> threadNumbers = { 3, 5, 7, 9, 25 };

ofstream logs("../Release/logs.txt", ios_base::app);

float** Calc(float** A, float** B, size_t n) {
	float** C = new float*[n];
	for (int i = 0; i < n; i++) {
		C[i] = new float[n];
	}
	auto start = std::chrono::steady_clock::now();
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
	auto end = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	cout << n << ' ' << time << endl;
	logs << n << ' ' << time << endl;
	return C;
}

float** GenMatrix(size_t n) {
	float** M = new float* [n];
	for (int i = 0; i < n; i++) {
		M[i] = new float[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			M[i][j] = 200 * ((float)rand() / RAND_MAX) - 100;
		}
	}
	return M;
}

float** Solve(int threadNumber, int r1, int Q2, size_t n, float** A, float** B) {
	ofstream out("../Release/input.txt");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			out << A[i][j] << ' ';
		}
		out << endl;
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			out << B[i][j] << ' ';
		}
		out << endl;
	}
	system(format("mpiexec -n {} ../Release/pain.exe {} {} {} {}", threadNumber, r1, Q2, n, "r").c_str());
	float** C = new float* [n];
	for (int i = 0; i < n; i++) {
		C[i] = new float[n];
	}
	ifstream result("../Release/output.txt");
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			result >> C[i][j];
		}
	}
	return C;
}

bool Check(int n, float** A, float** B) {
	float eps = 1e-3;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			if (fabs(A[i][j] - B[i][j]) > eps) {
				return false;
			}
		}
	}
	return true;
}


int main()
{
	//system(format("mpiexec -n {} ./pain.exe {} {} {}", 5, 2, 2, 10).c_str());
	//write any other calls here
	// 
	//for (int i : firstTileSizes) {
	//	for (int j : secondTileNumbers) {
	//		if (i < 100 && j < 100) {
	//			cerr << i << ' ' << j << '\n';
	//			system(format("mpiexec -n {} ../Release/pain.exe {} {} {}\n", 9, i, j, 100).c_str());
	//		}
	//		
	//		system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", 9, i, j, 1000).c_str());
	//	}
	//}
	/*for (int i : threadNumbers) {
		system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", i, 10, 10, 1000).c_str());
	}
	for (int i : matrixSizes) {
		system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", 9, 10, 10, i).c_str());
	}*/
	for (int t = 0; t < 1; t++) {
		for (int i : matrixSizes) {
			float** A = GenMatrix(i);
			float** B = GenMatrix(i);
			float** C = Calc(A, B, i);
			float** CC = Solve(8, 10, 10, i, A, B);
			/*for (int ii = 0; ii < i; ++ii) {
				for (int jj = 0; jj < i; ++jj) {
					std::cout << CC[ii][jj] << std::endl;
				}
			}*/

			assert(Check(i, C, CC));
			for (int j = 0; j < i; j++) {
				delete[] A[j];
				delete[] B[j];
				delete[] C[j];
				delete[] CC[j];
			}
			system("pause");

			delete[] A;
			delete[] B;
			delete[] C;
			delete[] CC;
		}
	}
	
	return 0;
}
