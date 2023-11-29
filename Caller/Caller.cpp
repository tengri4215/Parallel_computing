#include <iostream>
#include <fstream>
#include <format>

using namespace std;

const std::initializer_list<int> firstTileSizes = { 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> secondTileNumbers = { 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> matrixSizes = {  100, 500, 1000, 2000 };
const std::initializer_list<int> threadNumbers = { 3, 5, 7, 9, 25 };


float** Calc(float** A, float** B, size_t n) {
	float** C = new float*[n];
	for (int i = 0; i < n; i++) {
		C[i] = new float[n];
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			C[i][j] = 0;
			for (int k = 0; k < n; k++) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
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

void Solve(int threadNumber, int r1, int Q2, size_t n, float** A, float** B) {
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
}



int main()
{
	cout << format("mpiexec -n {} ./pain.exe {} {} {}", 5, 2, 2, 10).c_str() << endl;
	system(format("mpiexec -n {} ./pain.exe {} {} {}", 5, 2, 2, 10).c_str());
	//write any other calls here
	// 
	//for (int i : firstTileSizes) {
	//	for (int j : secondTileNumbers) {
	//		if (i < 100 && j < 100) {
	//			cerr << i << ' ' << j << '\n';
	//			system(format("mpiexec -n {} ../Release/pain.exe {} {} {}\n", 9, i, i, 100).c_str());
	//		}
	//		
	//		system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", 9, i, i, 1000).c_str());
	//	}
	//}
	//for (int i : threadNumbers) {
	//	system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", threadNumbers, 10, 10, 1000).c_str());
	//}
	//for (int i : matrixSizes) {
	//	system(format("mpiexec -n {} ../Release/pain.exe {} {} {}", 9, 10, 10, i).c_str());
	//}
	return 0;
}
