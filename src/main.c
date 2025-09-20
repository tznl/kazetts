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

	FILE *fout = fopen("output.wav", "wb");
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

	printf("Wrote WAV file %s (%ld bytes audio)\n", out, dataSize);
	return 0;
}

int main(void) 
{
	piper_synthesizer *synth = piper_create("../voice/en_US-amy-medium.onnx",
                                            "../voice/en_US-amy-medium.onnx.json",
                                            "../thirdparty/piper1-gpl/libpiper/install/espeak-ng-data");
	FILE* f = fopen("./output.raw", "wb");
	if (!f) {
		perror("Failed to open output.raw");
		piper_free(synth);
		return 1;
	}

	piper_synthesize_options options = piper_default_synthesize_options(synth);
	// Change options here:
	// options.length_scale = 2;
	// options.speaker_id = 5;

	// Start synthesis
	piper_synthesize_start(
		synth,
		"wazzaaaap",
		&options // pass NULL for defaults 
	);

	piper_audio_chunk chunk;
	while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
		fwrite(chunk.samples, sizeof(float), chunk.num_samples, f);
	}
	
	fclose(f);

	pcm_to_wav();

	piper_free(synth);
	return 0;
}
