#include <mpi.h>
#include <stdio.h>

#define N 1000000   // Reduce for testing (original = 2,000,000,000)

long long grinder(long long seed, long long m, long long a, long long mod) {
    long long r = seed;
    for (long long i = 0; i < N; i++) {
        r = (r * m + a) % mod;
    }
    return r;
}

int main(int argc, char** argv) {
    int rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start = MPI_Wtime();

    // Example IDs (replace with yours)
    long long A = 1087;
    long long B = 2023;

    long long alpha, beta;

    if (rank == 0) {
        // Student 1
        long long a1 = grinder(A, 31, 17, 9973);
        long long a2 = grinder(a1, 37, 11, 9973);
        alpha = (a1 + a2) % 9973;

        MPI_Send(&alpha, 1, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD);

        MPI_Recv(&beta, 1, MPI_LONG_LONG, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        long long verify = grinder(alpha + beta, 7, 3, 101);

        long long R = grinder(alpha * beta + A + B, 13, 7, 9973);
        long long password = R % 10000;

        printf("Student 1:\n");
        printf("Alpha = %lld\n", alpha);
        printf("Beta = %lld\n", beta);
        printf("Verify = %lld\n", verify);
        printf("Password = %lld\n", password);
    }
    else if (rank == 1) {
        // Student 2
        MPI_Recv(&alpha, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        long long b1 = grinder(alpha, B, 13, 9973);
        long long b2 = grinder(b1, 41, 19, 9973);
        beta = (b1 + b2) % 9973;

        MPI_Send(&beta, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);

        long long verify = grinder(alpha + beta, 7, 3, 101);

        printf("Student 2:\n");
        printf("Alpha = %lld\n", alpha);
        printf("Beta = %lld\n", beta);
        printf("Verify = %lld\n", verify);
    }

    double end = MPI_Wtime();

    if (rank == 0) {
        printf("Execution Time: %f seconds\n", end - start);
    }

    MPI_Finalize();
    return 0;
}
