#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <elpa/elpa.h>

int main(int argc, char** argv) {

    const int zero = 0;
    int myid;
    int n_proc;
    int n_prow;
    int n_pcol;
    int my_prow;
    int my_pcol;
    int blacs_ctxt;
    int blk;
    int l_row;
    int l_col;
    int ierr;
    int dim;
    int tmp;
    int i;

    double *mat;
    double *eval;
    double *evec;

    elpa_t eh;

    // Set up MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);

    tmp = (int) round(sqrt((double) n_proc));

    for (n_pcol = tmp; n_pcol > 1; n_pcol--) {
        if (n_proc%n_pcol == 0) {
            break;
        }
    }

    n_prow = n_proc/n_pcol;

    if (myid == 0) {
        printf("MPI done\n");
    }

    // Set up BLACS
    blacs_ctxt = Csys2blacs_handle(MPI_COMM_WORLD);
    Cblacs_gridinit(&blacs_ctxt, "R", n_prow, n_pcol);
    Cblacs_gridinfo(blacs_ctxt, &n_prow, &n_pcol, &my_prow, &my_pcol);

    dim = atoi(argv[1]);
    blk = atoi(argv[2]);

    l_row = numroc_(&dim, &blk, &my_prow, &zero, &n_prow);
    l_col = numroc_(&dim, &blk, &my_pcol, &zero, &n_pcol);

    if (myid == 0) {
        printf("BLACS done\n");
        printf("Dimension: %d\n", dim);
        printf("Block size: %d\n", blk);
    }

    mat = malloc(l_row * l_col * sizeof(double));
    evec = malloc(l_row * l_col * sizeof(double));
    eval = malloc(dim * sizeof(double));

    for (i = 0; i < l_row * l_col; i++) {
        mat[i] = 123.0;
    }

    if (elpa_init(20190501) != ELPA_OK) {
        printf("ELPA version error?");
        return 1;
    }

    eh = elpa_allocate(&ierr);

    elpa_set(eh, "na", dim, &ierr);
    elpa_set(eh, "nev", dim, &ierr);
    elpa_set(eh, "local_nrows", l_row, &ierr);
    elpa_set(eh, "local_ncols", l_col, &ierr);
    elpa_set(eh, "nblk", blk, &ierr);
    elpa_set(eh, "mpi_comm_parent", MPI_Comm_c2f(MPI_COMM_WORLD), &ierr);
    elpa_set(eh, "process_row", my_prow, &ierr);
    elpa_set(eh, "process_col", my_pcol, &ierr);

    elpa_setup(eh);

    elpa_eigenvectors(eh, mat, eval, evec, &ierr);

    elpa_deallocate(eh, &ierr);
    elpa_uninit(&ierr);

    if (myid == 0) {
        printf("ELPA done\n");
        printf("Min eigenvalue: %f\n", eval[0]);
        printf("Max eigenvalue: %f\n", eval[dim-1]);
    }

    free(mat);
    free(evec);
    free(eval);

    Cblacs_gridexit(blacs_ctxt);

    MPI_Finalize();

    return 0;
}
