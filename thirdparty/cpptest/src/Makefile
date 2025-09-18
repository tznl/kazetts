CC = g++

SRC	= main.cpp

CFLAGS	= -std=c++11 -lpiper -lonnxruntime 
INCDIRS	=  -I../../piper1-gpl/libpiper/install/include/
LIBDIRS	= -L../../piper1-gpl/libpiper/install/ -L../../piper1-gpl/libpiper/install/lib64/
make:

	$(CC) $(SRC) -o ../run $(CFLAGS) $(INCDIRS) $(LIBDIRS)
