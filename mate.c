#define MATE_IMPLEMENTATION // Adds the implementation of functions for mate
#include "./thirdparty/mate.h"

i32 main(void) {
	StartBuild();
	{
		Executable e = CreateExecutable((ExecutableOptions){
			.output	  = "run",
			.warnings = FLAG_WARNINGS,
			.std	  = FLAG_STD_C99,
		});

		AddFile(e, "./src/*.c");
		AddFile(e, "./thirdparty/miniaudio/miniaudio.c");

		AddIncludePaths(e, "./src/include/",
				"./thirdparty/",
				"./thirdparty/miniaudio/",
				"./thirdparty/piper1-gpl/libpiper/install/include/");

		if (isLinux()) {
			AddLibraryPaths(e, "./thirdparty/piper1-gpl/libpiper/install/", 
					"./thirdparty/piper1-gpl/libpiper/install/lib64/");
			LinkSystemLibraries(e, "piper", "onnxruntime", "pthread", "m");

//			FileCopy("./thirdparty/piper1-gpl/libpiper/install/libpiper.a", 
//					"./build/");
//			FileCopy("./thirdparty/piper1-gpl/libpiper/install/lib64/libonnxruntime.so", 
//					"./build/");
		} else if (isWindows()) {
//			AddLibraryPaths(e, "");
//			LinkSystemLibraries(e, "lpiper", "l");
		}

		InstallExecutable(e);

		// wont do this because the program requires dynamics libs for now
//		errno_t err = RunCommand(e.outputPath);
//		Assert(err == SUCCESS, "RunCommand: failed, return from exe should always be SUCCESS"); // Assert that we return SUCCESS == 0
		
	}
	EndBuild();
}
