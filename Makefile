INC	= -I"./thirdparty/" -I"./thirdparty/miniaudio/" -I"./thirdparty/piper1-gpl/libpiper/include/"
CFLAGS	= -fdiagnostics-color=always -Wall -Wextra -std=c99
LFLAGS	= -lpthread -lstdc++ -lm

make:

	-@mkdir build/
	-@mkdir build/data/
	-@mkdir build/data/art/
	-@mkdir build/data/export/

	-@printf "default" > build/data/config

	-@cp -f thirdparty/art/art build/data/art/

	-@cp -rf voice build/data/
	-@cp -rf thirdparty/espeak-ng/espeak-ng-data/ build/data/

	$(CC) -Wl,-rpath,./data $(CFLAGS) $(INC) \
		-c /home/tznl/git/kazetts/thirdparty/miniaudio/miniaudio.c \
		-o /home/tznl/git/kazetts/build/miniaudio.o

	$(CC) -Wl,-rpath,./data $(CFLAGS) $(INC) \
		-c /home/tznl/git/kazetts/src/main.c \
		-o /home/tznl/git/kazetts/build/main.o

	$(CC) -Wl,-rpath,./data $(CFLAGS) \
		-o /home/tznl/git/kazetts/build/run \
		/home/tznl/git/kazetts/build/main.o /home/tznl/git/kazetts/build/miniaudio.o \
		./thirdparty/piper1-gpl/libpiper.a ./thirdparty/espeak-ng/src/.libs/libespeak-ng.a \
		./thirdparty/ucd-tools/src/.libs/libucd.a \
		-L"thirdparty/onnxruntime/lib/" -lonnxruntime \
		$(LFLAGS)
	
	-@cp thirdparty/onnxruntime/lib/libonnxruntime.so build/data/
	-@cp thirdparty/onnxruntime/lib/libonnxruntime.so.1 build/data/

	-@rm build/*.o

cross:

	-@mkdir build/
	-@mkdir build/data/
	-@mkdir build/data/art/
	-@mkdir build/data/export/

	-@printf "default" > build/data/config

	-@cp -f thirdparty/art/art build/data/art/

	-@cp -rf voice build/data/
	-@cp -rf thirdparty/espeak-ng/espeak-ng-data/ build/data/

	$(CC) $(CFLAGS) $(INC) -I"./thirdparty/dirent/include/" \
		-c /home/tznl/git/kazetts/thirdparty/miniaudio/miniaudio.c \
		-o /home/tznl/git/kazetts/build/miniaudio.o

	$(CC) $(CFLAGS) $(INC) -I"./thirdparty/dirent/include/" \
		-c /home/tznl/git/kazetts/src/main.c \
		-o /home/tznl/git/kazetts/build/main.o

	$(CC) $(CFLAGS) --static \
		-o /home/tznl/git/kazetts/build/run /home/tznl/git/kazetts/build/main.o /home/tznl/git/kazetts/build/miniaudio.o \
		-L"/usr/x86_64-w64-mingw32/lib/" \
		./thirdparty/piper1-gpl/libpiper.a ./thirdparty/espeak-ng/src/.libs/libespeak-ng.a \
		./thirdparty/ucd-tools/src/.libs/libucd.a ./thirdparty/onnxruntime/lib/libonnxruntime.a \
		/usr/x86_64-w64-mingw32/lib/libstdc++.dll.a /usr/x86_64-w64-mingw32/lib/libwinpthread.dll.a -lm

	-@cp thirdparty/onnxruntime/lib/onnxruntime.dll build/
	-@cp /usr/x86_64-w64-mingw32/lib/libstdc++-6.dll build/
	-@cp /usr/x86_64-w64-mingw32/bin/libwinpthread-1.dll build/
	-@cp /usr/x86_64-w64-mingw32/lib/libgcc_s_seh-1.dll build/

	-@rm build/*.o
