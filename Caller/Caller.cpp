#include <iostream>
#include <format>

using namespace std;

const std::initializer_list<int> firstTileSizes = { 1, 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> secondTileNumbers = { 1, 2, 5, 10, 20, 50, 100, 200, 500 };
const std::initializer_list<int> matrixSizes = {  100, 500, 1000, 2000 };
const std::initializer_list<int> threadNumbers = { 3, 5, 10, 16, 20, 25 };

int main()
{
	//write any other calls here
	system(format("mpiexec -n {} ./pain.exe {} {} {}", 3, 1, 2, 2).c_str());
	for (int i : firstTileSizes) {
		for (int j : secondTileNumbers) {
			for (int n : matrixSizes) {
				for (int t : threadNumbers) {
					if (i < n && j < n && t <= ceil(n / i) * j) {
						system(format("mpiexec -n {} ./pain.exe {} {} {}", t, i, j, n).c_str());
					}
				}
			}
		}
	}
	return 0;
}
