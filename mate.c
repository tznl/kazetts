#define MATE_IMPLEMENTATION // Adds the implementation of functions for mate
#include "./thirdparty/mate.h/mate.h"

i32 main(void) {
	StartBuild();
	{
		Executable e = CreateExecutable((ExecutableOptions){
			.output	  = "run",
			.warnings = FLAG_WARNINGS,
			.std	  = FLAG_STD_C99,
			.flags	  = "-Wl,-rpath,./data"
		});

		AddFile(e, "./src/*.c");
		AddFile(e, "./thirdparty/miniaudio/miniaudio.c");

		AddIncludePaths(e, "./src/include/",
				"./thirdparty/",
				"./thirdparty/miniaudio/",
				"./thirdparty/piper1-gpl/libpiper/install/include/");

		Mkdir(s("./build/data/"));
		Mkdir(s("./build/data/export/"));
		Mkdir(s("./build/data/voice/"));

		Mkdir(s("./build/data/art/"));
		if (FileAdd(
			s("./build/data/config"), 
			s("default"))) {
			printf("File write failed\n");
		} else if (!FileCopy(
			s("./thirdparty/art/konata1"), 
			s("./build/data/art/konata1"))) {
			printf("konata art copied\n");
		}

		if (isLinux()) {
			#ifdef __linux__
			AddLibraryPaths(e, "./thirdparty/piper1-gpl/libpiper/install/", 
					"./thirdparty/piper1-gpl/libpiper/install/lib64/");
			LinkSystemLibraries(e, "piper", "onnxruntime", "pthread", "m");

			if	(FileDelete(s("./run.sh")) == FILE_DELETE_ACCESS_DENIED) {
				printf("File delete failed\n");
			} else if (FileAdd(
				s("./run.sh"), 
				s("#!/bin/sh\ncd ./build\n./run\n"))) {
				printf("File write failed\n");
			} else if (FileCopy(
				s("./thirdparty/piper1-gpl/libpiper/install/libpiper.so"), 
				s("./build/data/libpiper.so"))) {
				printf("File copy failed: libpiper.so\n");
			} else if (FileCopy(
				s("./thirdparty/piper1-gpl/libpiper/install/lib64/libonnxruntime.so"), 
				s("./build/data/libonnxruntime.so"))) {
				printf("File copy failed: libonnxruntime.so\n");
			} else if (FileCopy(
				s("./thirdparty/piper1-gpl/libpiper/install/lib64/libonnxruntime.so.1"), 
				s("./build/data/libonnxruntime.so.1"))) {
				printf("File copy failed: libonnxruntime.so.1\n");
			} else {
				printf("Don't forget this:\n\ncp -r \"thirdparty/piper1-gpl/libpiper/install/espeak-ng-data/\" \"build/data/\"\n");
				printf("cp -r \"voice/*\" \"build/data/voice/\"\n\n");
			}

			chmod("./run.sh", S_IRUSR | S_IWUSR | S_IXUSR);
			#endif
		} else if (isWindows()) {
			AddIncludePaths(e, "./thirdparty/dirent/include/dirent.h");
			//AddLibraryPaths(e, "");
			//LinkSystemLibraries(e, "lpiper", "l");
		}

		InstallExecutable(e);

		// wont do this because the program requires dynamics libs for now
//		errno_t err = RunCommand(e.outputPath);
//		Assert(err == SUCCESS, "RunCommand: failed, return from exe should always be SUCCESS"); // Assert that we return SUCCESS == 0
	}
	EndBuild();
}
