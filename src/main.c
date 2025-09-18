#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "piper.h"  // make sure compiler can find this (use -I/path/to/piper/include)

int main(void) {
    // Create synthesizer
    piper_synthesizer *synth = piper_create("../voice/en_US-amy-medium.onnx",
                                            "../voice/en_US-amy-medium.onnx.json",
                                            "../thirdparty/piper1-gpl/libpiper/install/espeak-ng-data");

    // Open output file in binary mode
    FILE *audio_stream = fopen("output.raw", "wb");
    if (!audio_stream) {
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
        "You better lose yourself in mom's spaghetti, it's ready. You better never let it go. You only get one spaghetti, do not miss your chance to blow.",
        &options /* pass NULL for defaults */
    );

    // Write audio chunks to file
    piper_audio_chunk chunk;
    while (piper_synthesize_next(synth, &chunk) != PIPER_DONE) {
        fwrite(chunk.samples, sizeof(float), chunk.num_samples, audio_stream);
    }

    // Clean up
    fclose(audio_stream);
    piper_free(synth);

    return 0;
}
