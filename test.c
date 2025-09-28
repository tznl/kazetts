#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "piper.h"
#include "miniaudio.h"
#ifdef _WIN32
    #include "dirent.h"
#else
    #include <dirent.h>
#endif
#ifdef PIPER_DLL
#define PIPER_API __declspec(dllimport)
#else
#define PIPER_API
#endif



enum command_return {
	C_VOID,
	C_EXIT,
	C_INVALID,
	C_LIST_VOICE,
	C_SET_VOICE
};

typedef struct {
	char     riff[4];        // "RIFF"
	uint32_t chunkSize;
	char     wave[4];        // "WAVE"
	char     fmt[4];         // "fmt "
	uint32_t subchunk1Size;  // 16 for PCM
	uint16_t audioFormat;    // 3 = IEEE float
	uint16_t numChannels;
	uint32_t sampleRate;
	uint32_t byteRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;
	char     data[4];        // "data"
	uint32_t subchunk2Size;
} wav_header;

int pcm_to_wav(char* in, char* out);
char* read_line( FILE * f ); 
int cmd_parse(char* input);
void find_first_files(const char *folder, char *onnx_path, char *json_path, size_t path_size);
void list_dirs();
int folder_exists(const char *folder);
void write_config(const char *folder);

int main()
{
	printf("test\n");
	char* audio_in  = "./data/export/output.raw";
	char* audio_out = "./data/export/output.wav";

	FILE* f_config = fopen("./data/config", "r");
	if (!f_config) {
		perror("config failed to load\n");
		return 1;
	}
	char line[256];
	if (fgets(line, sizeof(line), f_config) != NULL) { 
		// fgets includes the newline if it exists, remove it
		size_t len = 0;
		while (line[len] != '\0') len++;
		if (len > 0 && line[len - 1] == '\n') {
			line[len - 1] = '\0';
		}
	}

	FILE* art = fopen("./data/art/art", "r");
	if (!art) {
		perror("art failed to load\n");
		return 1;
	}
	int c;
	while ((c = fgetc(art)) != EOF) {
		putchar(c);
	} 
	fclose(art);

	printf("\033[0;31m");
	printf("Loading... Please Wait\r");
	printf("\033[0m");
	fflush(stdout);
	
	ma_result result;
	ma_engine engine;
	
		result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return result;
	}

	char folder[256] = "./data/voice/";
	strcat(folder, line);
	char onnx[256] = {0};
	char onnx_json[256] = {0};
	find_first_files(folder, onnx, onnx_json, sizeof(onnx));
	
	piper_audio_chunk chunk;
	piper_synthesizer *synth = piper_create(
		onnx,
		onnx_json,
		"./data/espeak-ng-data");
	
	while(1){
	}
	return 1;
}

void list_dirs() {
	const char *folder = "./data/voice/";  // folder to scan
	DIR *dir = opendir(folder);

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		//skip "." and ".."
		if (entry->d_name[0] == '.') continue;

		//check if entry is a directory
		if (entry->d_type == 4) { //again DT_DIR == 4 and i have no idea why
			printf("\033[0;34m");
			printf("%s\n", entry->d_name);
			printf("\033[0m");
		}
	}

	closedir(dir);
}

void find_first_files(const char *folder, char *onnx_path, char *json_path, size_t path_size) 
{
	DIR *dir = opendir(folder);
	if (!dir) {
		perror("Failed to open folder");
		return;
	}

	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
		//skip directories (4 = DT_DIR but wont work for some reason)
		if (entry->d_type == 4 ) continue;

		//check for .onnx
		if (!onnx_path[0] && strstr(entry->d_name, ".onnx")) {
			snprintf(onnx_path, path_size, "%s/%s", folder, entry->d_name);
		}

		//check for .json
		if (!json_path[0] && strstr(entry->d_name, ".json")) {
			snprintf(json_path, path_size, "%s/%s", folder, entry->d_name);
		}
	
		//stop if both found
		if (onnx_path[0] && json_path[0]) break;
	}

	closedir(dir);
}

int pcm_to_wav(char* in, char* out) 
{
	FILE *fin = fopen(in, "rb");
	if (!fin) {
		perror("fopen input");
		return 1;
	}
	fseek(fin, 0, SEEK_END);
	long dataSize = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	FILE *fout = fopen(out, "wb");
	if (!fout) {
		perror("fopen output");
		fclose(fin);
		return 1;
	}

	wav_header header;
	memcpy(header.riff, "RIFF", 4);
	header.chunkSize = 36 + dataSize;
	memcpy(header.wave, "WAVE", 4);
	memcpy(header.fmt, "fmt ", 4);
	header.subchunk1Size = 16;
	header.audioFormat = 3;  // IEEE float
	header.numChannels = 1;
	header.sampleRate = 22050;
	header.bitsPerSample = 32;
	header.byteRate = header.sampleRate * header.numChannels * (header.bitsPerSample / 8);
	header.blockAlign = header.numChannels * (header.bitsPerSample / 8);
	memcpy(header.data, "data", 4);
	header.subchunk2Size = dataSize;

	fwrite(&header, sizeof(header), 1, fout);

	char *buffer = malloc(dataSize);
	fread(buffer, 1, dataSize, fin);
	fwrite(buffer, 1, dataSize, fout);

	free(buffer);
	fclose(fin);
	fclose(fout);

	return 0;
}

char* read_line( FILE * f ) 
{
	int cap = 32,  next = 0, c;
	char * p = malloc( cap );
	while( 1 ) { 
		if ( next == cap ) {
			p = realloc( p, cap *= 2 );
		}
		c = fgetc( f ); 
		if ( c == EOF || c == '\n' ) {
			p[next++] = 0;
			break;
		}
		p[next++] = c;
	}
	if ( c == EOF && next == 1 ) {
		free( p );
		p = NULL;
	}

	return p;
}

int folder_exists(const char *folder) 
{
	char full_path[256];
	memset(full_path, '\0', sizeof(full_path));
	strcat(full_path, "./data/voice/");
	strcat(full_path, folder);
	DIR *dir = opendir(full_path);
	if (dir) {
		closedir(dir);
		return 1;
	}
	return 0;
}

void write_config(const char *folder) 
{
	FILE *file = fopen("./data/config", "w");
	if (!file) {
		perror("Failed to open config file");
		return;
	}
	fprintf(file, "%s\n", folder);
	fclose(file);
}


int cmd_parse(char* input)
{
	char *space = strchr(input, ' ');
	int cmd_length;

	if (space != NULL) {
		cmd_length = space - input;
	} else {
		cmd_length = strlen(input);
	}

	char command[256];
	strncpy(command, input, cmd_length);
	command[cmd_length] = '\0';

	char *arg = NULL;
	if (space != NULL) {
		arg = space + 1; // pointer to folder name
	}

	if (!strcmp(command, "/void")) {
		printf("Continuing\n");
		return C_VOID;
	} else if (!strcmp(command, "/exit")) {
		return C_EXIT;
	} else if (!strcmp(command, "/list_voice")) {
		list_dirs();
		return C_LIST_VOICE;
	} else if (!strcmp(command, "/set_voice")) {
		if (!arg) {
			printf("Usage: /set_voice folder_name\n");
			return C_INVALID;
		}
		if (!folder_exists(arg)) {
			printf("Voice folder doesn't exist:  \033[0;31m%s\033[0m\n", arg);
			return C_INVALID;
		}
		printf("Voice folder set to: \033[0;34m%s\033[0m\n", arg);
		write_config(arg);
		return C_SET_VOICE; // or a dedicated C_SET_VOICE
	} else {
		printf("Invalid Command: %s\n", command);
		return C_INVALID;
	}
}
