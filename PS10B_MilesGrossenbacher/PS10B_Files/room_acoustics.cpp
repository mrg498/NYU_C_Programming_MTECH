/* Program to model room acoustics using acoustic image method */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>     /* memset */
#include <ctype.h>      /* loupper() */
#include <math.h>       /* round */
#include <sndfile.h>	/* libsndfile */
#include <portaudio.h>  /* portaudio */
#include <unistd.h> /*sleep*/
#include "room_acoustics.h"

/* data structure to pass to callback */
typedef struct {
    /* pointer to room data structure */
    Room *room;
    /* input signal */
    float *signal;
    int frames;
    int channels;
    int buf_frames;
    int next_frame;
} callBackData;

/* room data structure
 * room must be a global to be accessible by display functions
 */
Room room_struct, *room = &room_struct;

/* globals for debug output signal */
SNDFILE *osfile = 0;
float *osignal = 0;
int ocount = 0;

/* PortAudio callback function protoype */
int paCallback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );

/* function prototypes for external functions */
float parametric_reverb(float x, float reflect);
void clear_reverb(void);
void display_ncurses( void );
void display_openGL( void );

//Usage:
int usage(char* argv0){
  printf("%s%s%s\n", "Usage: " ,argv0, " ifile.wav");
  return -1;
}

int main(int argc, char *argv[])
{
    char *ifile;
    float *signal;
    /* for debug output wav file */
    char *ofile = 0;
    SF_INFO osfinfo;

    /* libsndfile data structures */
    SNDFILE *sfile;
    SF_INFO sfinfo;
    /* Zero libsndfile structures */
    memset(&sfinfo, 0, sizeof(sfinfo));
    memset(&osfinfo, 0, sizeof(osfinfo));

    /* My callback data structure */
    callBackData my_data;
    /* PortAudio data structures */
    PaStream *stream;
    PaError err;
    PaStreamParameters outputParams;
    PaStreamParameters inputParams;



    /* parse command line
     * print usage
     * open input WAV file and check that it is mono
     */
    //YOUR CODE HERE

    //if argc is not 2, print Usage
    if(argc < 2){
      return usage(argv[0]);
    }

    //parse input wav file into ifile
    ifile = argv[1];

    // check if there is an ofile
    if(argc == 3){
      ofile = argv[2];
    }


    //open the wav file
    sfinfo.format = 0;
  	if ( (sfile = sf_open (ifile, SFM_READ, &sfinfo)) == NULL ) {
  			fprintf (stderr, "Error: could not open wav file: %s\n", ifile);
  			return -1;
  	}

    //check if the sound file is mono
    if(sfinfo.channels != 1){
      printf("%s\n", "Error: ifile.wav must be mono");
      return -1;
    }


    /* allocate buffer to hold input signal, using calloc()
     * buffer must hold the entire signal plus reverb "ring-out" interval
     * that is, (sfinfo.frames + sfinfo.samplerate*MAX_REVERB) * sfinfo.channels)
     */
    //YOUR CODE HERE

    //initiate buf_samples
    float buf_samples;
    buf_samples = (sfinfo.frames + sfinfo.samplerate*MAX_REVERB) * sfinfo.channels;



    signal = (float *)calloc(buf_samples, sizeof(float));
    int count = 0;
    if ((count = sf_readf_float (sfile, signal, sfinfo.frames)) != sfinfo.frames) {
			fprintf(stderr, "Error: on sample frame %d\n",count);
			return -1;
		}

    sf_close(sfile);


    /* read input signal
     * remainder of buffer will be zeros
     */
    /* close wav file */

    /* set up callback data structure */
    my_data.room = room;
    my_data.signal = signal;
    my_data.frames = sfinfo.frames;
    my_data.channels = sfinfo.channels;
    my_data.buf_frames = buf_samples; // works since input is mono
    my_data.next_frame = 0;

    /* open optional debug output wav file */
    if (ofile) {
        printf("Opening %s\n", ofile);
        osfinfo.samplerate = sfinfo.samplerate;
        osfinfo.channels = sfinfo.channels;
        osfinfo.format = sfinfo.format;
        if ( (osfile = sf_open (ofile, SFM_WRITE, &osfinfo)) == NULL ) {
            fprintf (stderr, "ERROR: could not open output wav file: %s\n", ofile);
            return -1;
        }
        if ( (osignal = (float*)calloc(buf_samples, sizeof(osignal))) == NULL) {
            fprintf(stderr, "ERROR: cannot calloc buffer\n");
            return -1;
        }
        ocount = 0;
    }
    else {
        osfile = 0;
        osignal = 0;
        ocount = 0;
    }

    /* room is centered at (0, 0, 0)
     * with initial dimensions XDIM, YDIM, ZDIM
     */
    room->dx = XDIM/2.0;
    room->dy = YDIM/2.0;
    room->dz = ZDIM/2.0;
    /* source and listener initial positions */
    room->source.x = -10;
    room->source.y = -5;
    room->source.z = -2;
    room->listener.x = 5;
    room->listener.y = 0;
    room->listener.z = 0;
    /* reflections */
    room->wall_reflection = W_REF;
    room->num_images = 6;   // room has 6 sides, one image per side
    /* switches */
    room->delay_info = true;
    room->images = true;
    room->parametric = PARA_STATE;
    /* sampling rate */
    room->samplerate = sfinfo.samplerate;

    /* Copy the following code blocks from the tone_generator.c code in
     * problem set PS07:*/

     /* Initializing PortAudio*/

     err = Pa_Initialize();
     if (err != paNoError) {
         printf("PortAudio error: %s\n", Pa_GetErrorText(err));
         printf("\nExiting.\n");
         exit(1);
     }

     /* Input stream parameters*/
     inputParams.device = Pa_GetDefaultInputDevice();
     inputParams.channelCount = 1;
     inputParams.sampleFormat = paFloat32;
     inputParams.suggestedLatency =
         Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
     inputParams.hostApiSpecificStreamInfo = NULL;


     /* Ouput stream parameters*/
     outputParams.device = Pa_GetDefaultOutputDevice();
     outputParams.channelCount = 2;
     outputParams.sampleFormat = paFloat32;
     outputParams.suggestedLatency =
         Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
     outputParams.hostApiSpecificStreamInfo = NULL;


     /* Open audio stream*/
     err = Pa_OpenStream(&stream,
         &inputParams, /* no input */
         &outputParams,
         sfinfo.samplerate, FRAMES_PER_BUFFER, // need to switch these
         paNoFlag, /* flags */
         paCallback,
         &my_data);

     if (err != paNoError) {
         printf("PortAudio error: open stream: %s\n", Pa_GetErrorText(err));
         printf("\nExiting.\n");
         exit(1);
     }


     /* Check for errors*/
     /* Start audio stream*/
     err = Pa_StartStream(stream);
     if (err != paNoError) {
         printf(  "PortAudio error: start stream: %s\n", Pa_GetErrorText(err));
         printf("\nExiting.\n");
         exit(1);
     }
     printf("Starting playout\n");

     /* [Put the sleep(5) statement]*/
     //calc_images(room);
     DISPLAY();
     printf("Finished playout\n");

     /* Stop stream*/
     err = Pa_StopStream(stream);
     if (err != paNoError) {
         printf(  "PortAudio error: stop stream: %s\n", Pa_GetErrorText(err));
         printf("\nExiting.\n");
         exit(1);
     }


    /* Close stream*/
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: close stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }


     /* Terminate PortAudio*/
     err = Pa_Terminate();
     if (err != paNoError) {
         printf("PortAudio error: terminate: %s\n", Pa_GetErrorText(err));
         printf("\nExiting.\n");
         exit(1);
     }

     /*if (osfile) {
       sf_writef_float(osfile, osignal, ocount);
       printf("Closing output WAV file\n");
       sf_close(osfile);
     }*/


    return 0;
}

/* callback */
int paCallback(
    const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
{
    /* Cast data passed to callback to our structure type */
    callBackData *pd = (callBackData *)userData;
    float *output = (float *)outputBuffer;
    //float *input = (float *)inputBuffer; //not used in this program

    /* only consider direct sound if enable_images is false */
    int num_images = pd->room->images ? pd->room->num_images : 0;
    bool enable_parametric = pd->room->parametric;

    //YOUR CODE HERE
    /* input is mono, output is stereo
     * for each output sample, sum direct and image valuess into output signal
     * considering delay and attenuation
     */


     float v = 0.0;
     int k = 0;

     for(int i = 0; i < framesPerBuffer; i++){
       float v = 0.0;
       for(int j = 0; j <= num_images; j++){
         k = pd->next_frame + i - room->image[j].samp_delay;
          if(k >= 0 && k < pd->buf_frames){
           // if we want the signals from the images


             v += room->image[j].atten * pd->signal[k];

             if(enable_parametric && j == num_images){
               k = pd->next_frame + i - room->max_delay;
               v += parametric_reverb(pd->signal[k], room->wall_reflection);
             }

         }
       }


       /*if (osignal && ocount < pd->buf_frames) {
         osignal[pd->next_frame+i] = 0;
         ocount++;
       }*/
       output[i*2 + 0] = v;
       output[i*2 + 1] = v;
     }



    /* increment pointer to next sample */
    pd->next_frame += framesPerBuffer;
    /* if next output buf processing puts this past end of signal buffer
     * reset to start of signal buffer
     */
    if (pd->next_frame > pd->buf_frames - framesPerBuffer) {
        pd->next_frame = 0;
#ifdef PARA_REVERB
    /* If parametric reverb enabled, then clear parametric reverb delay lines */
        clear_reverb();
#endif
        ocount = 0;
    }

    return 0;
}
