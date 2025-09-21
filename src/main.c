#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "piper.h"
#include "miniaudio.h"


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

int pcm_to_wav() 
{
	char* in = "output.raw";
	char* out = "output.wav";

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

//	printf("Wrote WAV file %s (%ld bytes audio)\n", out, dataSize);
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

int main(void) 
{
	char* input;
	
	ma_result result;
	ma_engine engine;

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return result;
	}

	piper_audio_chunk chunk;
	piper_synthesizer *synth = piper_create(
		"../voice/en_US-amy-medium.onnx",
		"../voice/en_US-amy-medium.onnx.json",
		"../thirdparty/piper1-gpl/libpiper/install/espeak-ng-data");
	piper_synthesize_options options = piper_default_synthesize_options(synth);

	printf("loop start\n\n");
	while (1) {
		printf("> ");
		input = read_line(stdin);
		FILE* f = fopen("./output.raw", "wb");
		if (!f) {
			perror("Failed to open output.raw");
			return 1;
		}
		piper_synthesize_start(
			synth,
			input,
			&options);

		while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
			fwrite(chunk.samples, sizeof(float), chunk.num_samples, f);
		}

		pcm_to_wav();
		ma_engine_play_sound(&engine, "output.wav", NULL);
		free(input);
//		fclose(f);
	}

	piper_free(synth); 
	free(input);
	return 0;
}
