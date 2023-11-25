#include<stdio.h> 
#include<mpi.h> 
#include<stdlib.h>
#include <math.h>
#include <utility>
#include <iostream>

using namespace std;

float** A;
float** B;
float** C;
int r1, Q2, n, Q1, r2;
int numprocs;
int tilesForProcess = 0;


void Tile(int igl, int jgl) {
	for (int i = igl * r1; i < std::min((igl + 1) * r1, n); ++i) {
		for (int j = jgl * r2; j < std::min((jgl + 1) * r2, n); ++j) {
			for (int k = 0; k < n; ++k) {
				C[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void designate() {
	int base = Q1 * Q2 / (numprocs - 1);
	int rem = (Q1 * Q2) % (numprocs - 1);
	for (int i = 1; i < numprocs; ++i) {
		int t = base;
		if (rem > 0) { ++t; --rem; }
		//std::cerr << i << ' ' << t << '\n';
		MPI_Send(&t, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
	}
	
}

int main(int argc, char* argv[])
{
	int myid, namelen;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);        // starts MPI
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);  // get current process id
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);      // get number of processeser
	MPI_Get_processor_name(processor_name, &namelen);
	

	MPI_Barrier(MPI_COMM_WORLD);
	r1 = *argv[0];
	Q2 = *argv[1];
	n = *argv[2];
	A = new float* [n];
	B = new float* [n];
	C = new float* [n];
	for (int i = 0; i < n; i++) {
		A[i] = new float[n];
		B[i] = new float[n];
		C[i] = new float[n];
	}
	if (myid == 0) {
		//std::cerr << numprocs << '\n';
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				A[i][j] = 200 * ((float)rand() / RAND_MAX) - 100;
				B[i][j] = 200 * ((float)rand() / RAND_MAX) - 100;
			}
		}
		Q1 = ceil(float(n) / r1);
		r2 = ceil(float(n) / Q2);
		designate();
		//std::cerr << Q1 << ' ' << r2 << '\n';
		//for (int i = 0; i < n; i++) {
		//	for (int j = 0; j < n; j++) {
		//		std::cerr << A[i][j] << ' ';
		//	}
		//	std::cerr << '\n';
		//}
		int target_process = 0;
		for (int igl = 0; igl < Q1; ++igl) {
			for (int jgl = 0; jgl < Q2; ++jgl) {
				target_process++;
				if (target_process >= numprocs) target_process = 1;
				//std::cerr << target_process << ' ';
				MPI_Send(&igl, 1, MPI_INT, target_process, 1, MPI_COMM_WORLD);
				MPI_Send(&jgl, 1, MPI_INT, target_process, 1, MPI_COMM_WORLD);
				for (int i = igl * r1; i < min((igl + 1) * r1, n); ++i) {
					MPI_Send(A[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, target_process, 1, MPI_COMM_WORLD);
					MPI_Send(B[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, target_process, 1, MPI_COMM_WORLD);
				}

			}
		}
		target_process = 0;
		for (int igl = 0; igl < Q1; ++igl) {
			for (int jgl = 0; jgl < Q2; ++jgl) {
				for (int i = igl * r1; i < min((igl + 1) * r1, n); ++i) {
					target_process++;
					if (target_process >= numprocs) target_process = 1;
					MPI_Recv(C[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, target_process, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cerr << A[i][j] << ' ';
			}
			std::cerr << '\n';
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cerr << B[i][j] << ' ';
			}
			std::cerr << '\n';
		}
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				std::cerr << C[i][j] << ' ';
			}
			std::cerr << '\n';
		}
	}
	else {
		int tilenumber;
		MPI_Recv(&tilenumber, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		//if (myid == 4) std::cerr << myid << ' ' << tilenumber << '\n';
		for (int t = 0; t < tilenumber; t++) {
			//if (myid == 4) std::cerr << t << '\n';
			int igl, jgl;
			MPI_Recv(&igl, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&jgl, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//if (myid == 4) std::cerr << igl << ' ' << jgl << '\n';
			Q1 = ceil(float(n) / r1);
			r2 = ceil(float(n) / Q2);
			for (int i = igl * r1; i < min((igl + 1) * r1, n); ++i) {
				//if (myid == 4) std::cerr << i << ' ' << jgl * r2 << ' ' << min(r2, n - jgl * r2) << '\n';
				MPI_Recv(A[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				
				MPI_Recv(B[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			//if (myid == 4) std::cerr << igl << ' ' << jgl << '\n';
			Tile(igl, jgl);
			for (int i = igl * r1; i < min((igl + 1) * r1, n); ++i) {
				MPI_Send(C[i] + jgl * r2, min(r2, n - jgl * r2), MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	for (int i = 0; i < n; i++) {
		delete[] A[i];
		delete[] B[i];
		delete[] C[i];
	}
	delete[] A;
	delete[] B;
	delete[] C;
	MPI_Finalize();

	return 0;
}