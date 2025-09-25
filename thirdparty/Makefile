UCD_DIR	= "ucd-tools/src/"
UCD_IN	= $(UCD_DIR)case.c  $(UCD_DIR)categories.c  $(UCD_DIR)ctype.c  $(UCD_DIR)proplist.c  $(UCD_DIR)scripts.c  $(UCD_DIR)tostring.c
UCD_OUT	= case.o categories.o ctype.o proplist.o scripts.o tostring.o
UCD_INC	= -I$(UCD_DIR)include/

E_ROOT	= espeak-ng/build/
E_INS	= $(E_ROOT)espeak_ng-install/
E_BUILD	= $(E_ROOT)espeak_ng/
E_SRC	= $(E_ROOT)espeak_ng_src/

P_IN	= -c "piper1-gpl/libpiper/src/piper.cpp"
P_INC	= -I"piper1-gpl/libpiper/include" -I"onnxruntime/include/" -I"$(E_INS)include"
P_LDIR	= -L"espeak-ng/build/espeak_ng-install/lib/" -L"ucd-tools/" -L"onnxruntime/lib/"
P_LFLAG	= -lonnxruntime -lucd -lespeak-ng

CFLAGS	= -std=c99
make: ucd espeak piper

clean:

	rm "ucd-tools/libucd.a"
	rm "piper1-gpl/libpiper.a"
	rm -rf "espeak-ng/build/"

ucd:

	$(CC) -c $(UCD_IN) $(CFLAGS) $(UCD_INC)
	ar -rsc libucd.a $(UCD_OUT)
	rm *.o
	mv libucd.a "ucd-tools/"
espeak:


	mkdir $(E_ROOT)

	cmake -B "$(E_BUILD)" -S "espeak-ng" \
	  -DCMAKE_INSTALL_PREFIX="$(E_INS)" \
	  -DBUILD_SHARED_LIBS=OFF \
	  -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
	  -DUSE_ASYNC=OFF \
	  -DUSE_MBROLA=OFF \
	  -DUSE_LIBSONIC=OFF \
	  -DUSE_LIBPCAUDIO=OFF \
	  -DUSE_KLATT=OFF \
	  -DUSE_SPEECHPLAYER=OFF \
	  -DEXTRA_cmn=ON \
	  -DEXTRA_ru=ON \
	  -DCMAKE_C_FLAGS="-D_FILE_OFFSET_BITS=64 -I$(E_BUILD)/src/espeak_ng_external/src/ucd-tools/src/include" \
	  -DCMAKE_CXX_FLAGS="-D_FILE_OFFSET_BITS=64 -I$(E_BUILD)/src/espeak_ng_external/src/ucd-tools/src/include"

	cmake --build "$(E_BUILD)" --target install -- -j$$(nproc)

piper:

	$(CXX) $(P_IN) $(P_INC) $(P_LDIR)
	ar -rsc libpiper.a piper.o
	rm piper.o
	mv libpiper.a "piper1-gpl/"
