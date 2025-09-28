P_IN	= -c "piper1-gpl/libpiper/src/piper.cpp"
P_INC	= -I"piper1-gpl/libpiper/include" -I"onnxruntime/include/" -I"espeak-ng/src/include"

make: ucd espeak piper

ucd:

	cd "ucd-tools"; \
	./autogen.sh; \
	./configure --disable-shared --enable-static; \
	make -B; 
espeak:


	cd "espeak-ng"; \
	./autogen.sh; \
	./configure --disable-shared --enable-static; \
	make -B; 
piper:

	$(CXX) $(P_IN) $(P_INC) -o "piper1-gpl/piper.o"
	ar -rsc "piper1-gpl/libpiper.a" "piper1-gpl/piper.o"

onnx:

	x86_64-w64-mingw32-dlltool -D onnxruntime/lib/onnxruntime.dll -d onnxruntime.def -l onnxruntime/lib/libonnxruntime.a
#cross compilation

cross: onnx ucd_cross espeak_cross piper_cross

ucd_cross:

	cd "ucd-tools"; \
	./autogen.sh; \
	CFLAGS="/usr/x86_64-w64-mingw32/lib/libstdc++.a" ./configure --build=x86_64-pc-linux-gnu --host=x86_64-w64-mingw32 --disable-shared --enable-static; \
	make -B; 
espeak_cross:

	-@cd "espeak-ng"; \
	./autogen.sh; \
	CFLAGS="-I espeak-ng/" CXXFLAGS="-Iespeak-ng/src/windows/ /usr/x86_64-w64-mingw32/lib/libstdc++.a" ./configure --build=x86_64-pc-linux-gnu --host=x86_64-w64-mingw32 --disable-shared --enable-static; \
	make -B; 
piper_cross:

	$(CXX) $(P_IN) $(P_INC) -DORT_DLL_IMPORT -o "piper1-gpl/piper.o" /usr/x86_64-w64-mingw32/lib/libstdc++.a
	ar -rsc "piper1-gpl/libpiper.a" "piper1-gpl/piper.o"

#	$(CXX) -o piper1-gpl/libpiper.a piper1-gpl/piper.o \
#	        ./espeak-ng/src/.libs/libespeak-ng.a ./ucd-tools/src/.libs/libucd.a ./onnxruntime/lib/libonnxruntime.a
clean:

	cd "ucd-tools"; \
	make clean; 

	cd "espeak-ng"; \
	make clean; 

	rm -f "piper1-gpl/piper.o"
	rm -f piper1-gpl/libpiper.*
	rm -f onnxruntime/lib/libonnxruntime.a
