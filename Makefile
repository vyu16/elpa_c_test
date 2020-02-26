ELPA_VER = 2019.05.002

# Intel @Timewarp
# CC = mpiicc
# CFLAGS = -O2 -I/home/wy29/opt/elpa/build/include/elpa-2019.11.001
# LOADER = mpiicc
# LDFLAGS = -L/home/wy29/opt/elpa/build/lib -lelpa -L/opt/intel/mkl/lib/intel64 -lmkl_scalapack_lp64 -lmkl_blacs_intelmpi_lp64 -lmkl_sequential -lmkl_core -lmkl_intel_lp64 -lifcore

# GCC @Summit
# CC = mpicc
# CFLAGS = -O2 -mcpu=power9 -I/ccs/home/vwzyu/opt/elpa/gcc/include/elpa-$(ELPA_VER)
# LOADER = mpicc
# LDFLAGS = -L/ccs/home/vwzyu/opt/elpa/gcc/lib -lelpa -L${OLCF_ESSL_ROOT}/lib64 -lessl -L${OLCF_NETLIB_SCALAPACK_ROOT}/lib -lscalapack -L${OLCF_NETLIB_LAPACK_ROOT}/lib64 -llapack -L${OLCF_CUDA_ROOT}/lib64 -lcublas -lcudart -lm -lmpi_ibm_mpifh -lgfortran

SRC = test.c
EXE = $(SRC:.c=.x)

all: $(EXE)

%.x: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f *.[ox]
