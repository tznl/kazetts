#define MATE_IMPLEMENTATION // Adds the implementation of functions for mate
#include "./thirdparty/mate.h/mate.h"

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
			#ifdef __linux__
			const char *linux_path = "./run.sh";
			const char *linux_data = "#!/bin/sh\ncd ./build\nLD_LIBRARY_PATH=$PWD/../thirdparty/piper1-gpl/libpiper/install/:$PWD/../thirdparty/piper1-gpl/libpiper/install/lib64/ ./run\n";
			remove(linux_path);
			FILE *linux_file = fopen(linux_path, "wb");
			fwrite(linux_data, 1, strlen(linux_data), linux_file);
			chmod(linux_path, S_IRUSR | S_IWUSR | S_IXUSR);
			fclose(linux_file);

			AddLibraryPaths(e, "./thirdparty/piper1-gpl/libpiper/install/", 
					"./thirdparty/piper1-gpl/libpiper/install/lib64/");
			LinkSystemLibraries(e, "piper", "onnxruntime", "pthread", "m");
			#endif
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
