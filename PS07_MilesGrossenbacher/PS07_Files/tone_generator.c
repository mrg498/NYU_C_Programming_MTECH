/*****************************************************************************
 * tone_generator.c
 *
 * Plays a tone to speaker using PortAudio
 * Wav file access uses Sndfile
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "portaudio.h"

#define SAMPLE_RATE 48000
#define FRAMES_PER_BUFFER 512
#define PI		3.14159265358979323846

/* Define a struct and typdef to soundData */
typedef struct
{
    float *tone;            //pointer to tone buffer
    unsigned int num_samp;  //number of samples in tone buffer
    unsigned int next_samp; //next sample to play in tone buffer
    unsigned long count;    //count of samples played
} soundData;

/* Callback function protoype */
static int paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );

void usage(char *argv0)
{
    //YOUR CODE HERE
    printf("%s%s%s\n", "Usage: " ,argv0, " [–f freq_in_Hz] [–a level_in_dBFS] [-s sampling_freq_in_Hz]");
    exit(-1);
}

int main(int argc, char *argv[])
{
    int i, level_dB;
    double f0, fs, level;
    soundData data;

    PaStream *stream;
    PaError err;
    PaStreamParameters outputParams;
    PaStreamParameters inputParams;

    /* Set default values */

    f0 = 440; //HZ
    fs = SAMPLE_RATE; //sampling rate
    level_dB = -24; //dbfs

    /*
     * Parse command line
     */
    /*if(argc > 7 || (argv[1][0] != '-')){
      usage(argv[0]);
    }*/


    for(int i = 1; i < argc; i++){
      if(argv[i][0] == '-'){
        switch (argv[i][1]) {
          case 'f':
            f0 = atoi(argv[i+1]);
            break;
          case 'a':
            level_dB = atoi(argv[i+1]);
            break;
          case 's':
            fs = atoi(argv[i+1]);
            break;
        }
      }
    }


    /* print parameter values */
    printf("Generating tone: f0 %f, fs %f, dB %d\n", f0, fs, level_dB);

    /* Set up tone parameters */
    /* integer freq f0 guarantees that there is an integer num_samp per second */

    data.num_samp = fs;
    data.next_samp = 0;
    data.count = 0;
    level = pow(10, level_dB/20.0);
    printf("Tone level %f\n", level);
    printf("Tone buffer is %d samples\n", data.num_samp);

    /* malloc storage for tone */

    data.tone = (float*)malloc(data.num_samp * sizeof(float));

    /* initialize tone */

    i = 0;
    while(i < fs){
      data.tone[i] = level * sin((2*PI*i*f0)/fs);
      i++;
    }

    printf("Tone initialized\n");

    /* initialize PortAudio */
    err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* Input stream parameters */
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = 1;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency =
        Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    /* Ouput stream parameters */
    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = 2;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency =
        Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = NULL;

    /* Open audio stream */
    err = Pa_OpenStream(&stream,
        &inputParams, /* no input */
        &outputParams,
        SAMPLE_RATE, FRAMES_PER_BUFFER,
        paNoFlag, /* flags */
        paCallback,
        &data);

    if (err != paNoError) {
        printf("PortAudio error: open stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* start audio stream */
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: start stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    printf("Starting playout\n");
    /* play out for 5 seconds */
    while (data.count < 5 * fs) {
        printf("%ld %d\n", data.count, data.next_samp);
        sleep(1);
    }
    printf("Finished playout\n");

    /* stop stream */
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: stop stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* close stream */
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: close stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* terminate PortAudio */
    err = Pa_Terminate();
    if (err != paNoError) {
        printf("PortAudio error: terminate: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    return 0;
}

/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * in the routine that requires significant resources.
 */
static int paCallback(const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData)
{
    /* Cast data passed through stream to our structure. */
    soundData *data = (soundData *)userData;
    float *out = (float *)outputBuffer;
    //float *in = (float *)inputBuffer; //don't need input
    unsigned int i, j;

    /* fill audio buffer on each callback */
    j = data->next_samp;
    for (i = 0; i < framesPerBuffer; i++) {

        if(j >= data->num_samp){
          j = 0;
        }
        out[2*i] = data->tone[j]; // left
        out[2*i + 1] = data->tone[j]; // right
        j++;
        data->count++;

    }
    data->next_samp = j; // save next_samp

    return 0;
}
