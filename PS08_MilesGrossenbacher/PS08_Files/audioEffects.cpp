#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     /* sleep() */
#include <stdbool.h>	/* true, false */
#include <string.h>		/* memset() */
#include <ctype.h>		/* lolower() */
#include <sndfile.h>	/* libsndfile */
#include <portaudio.h>	/* portaudio */
#include <ncurses.h> 	/* This library is for getting input without hitting return */
#include "baseEffect.h"
#include "passThrough.h"
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
// #include "sigFilter.h"
// #include "sigDRC.h"
//Begin
#include "sigFilter.h"
#include "sigDRC.h"
//End

#define SAMPLE_RATE         48000
#define NUM_CHN	            2
#define FRAMES_PER_BUFFER   1024
#define HPF                 0
#define LPF                 1

/* PortAudio callback function protoype */
static int paCallback( const void *inputBuffer, void *outputBuffer,
    unsigned long framesPerBuffer,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData );

/* define IOData structure */
typedef struct tagIOData {
    /* effects object pointer */
    baseEffect *pe;
    /* SndFile data structures */
    SNDFILE *sndfile[2];
    SF_INFO sfinfo[2];
} IOData;

int	main(int argc, const char *argv[])
{
    /* callback data structure */
    IOData io_data, *p = &io_data;
	/* PortAudio data structures */
    PaStream *stream;
    PaError err;
    PaStreamParameters outputParams;
    PaStreamParameters inputParams;

    /* effects objects */
    baseEffect *pass_through;
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
    // baseEffect *sig_filter;
    // baseEffect *sig_drc;
//Begin
    baseEffect *sig_filter;
    baseEffect *sig_drc;
//End
    pass_through = new passThrough(); /* create object and run constructor */
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
    // sig_filter = new sigFilter();
    // sig_drc = new sigDRC();
//Begin
    sig_filter = new sigFilter();
    sig_drc = new sigDRC();
//End

    /* set callback object pointer to NULL */
    p->pe = NULL;

    /* NULL signals default of no file I/O */
    p->sndfile[0] = NULL;
    p->sndfile[1] = NULL;

    /* zero libsndfile structures */
    memset(&p->sfinfo[0], 0, sizeof(p->sfinfo[0])); /// &p-> (why)
    memset(&p->sfinfo[1], 0, sizeof(p->sfinfo[1]));

    /* process command line */
    if ( argc > 1 && (strcmp(argv[1], "-h")) == 0) {  /// -h (why)
        /* usage */
        fprintf(stderr, "Usage: %s [ifile] [ofile]\n", argv[0]);
        return -1;
    }
    /* open WAV files if there are associated command line args */
    /* input */
    if (argc > 1 && argv[1] != NULL) {
        /* open WAV file */                     // is &io_data.sfinfo[0] the same?
        if ( (p->sndfile[0] = sf_open (argv[1], SFM_READ, &p->sfinfo[0])) == NULL ) {
            fprintf (stderr, "Error: could not open wav file: %s\n", argv[1]);
            return -1;
        }
        /* Print file information */
        printf("Opening %s as input\n", argv[1]);
        printf("Frames: %8d, Channels: %d, Samplerate: %d, %s\n",
            (int)p->sfinfo[0].frames, p->sfinfo[0].channels, p->sfinfo[0].samplerate, argv[1]);

    }
    if (argc > 2 && argv[2] != NULL) {
        p->sfinfo[1].channels = p->sfinfo[0].channels;
        p->sfinfo[1].samplerate = p->sfinfo[0].samplerate;
        p->sfinfo[1].format = p->sfinfo[0].format;
        if ( (p->sndfile[1] = sf_open (argv[2], SFM_WRITE, &p->sfinfo[1])) == NULL ) {
            fprintf (stderr, "Error: could not open wav file: %s\n", argv[2]);
            return -1;
        }
        /* Print file information */
        printf("Opening %s as output\n", argv[2]);
    }
    /* pause to read screen */
    sleep(1);

    /* Initializing PortAudio */
    err = Pa_Initialize();
    if (err != paNoError) {
        printf("PortAudio error: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        return -1;
    }

    /* Input stream parameters */
    inputParams.device = Pa_GetDefaultInputDevice();
    inputParams.channelCount = NUM_CHN;
    inputParams.sampleFormat = paFloat32;
    inputParams.suggestedLatency =
        Pa_GetDeviceInfo(inputParams.device)->defaultLowInputLatency;
    inputParams.hostApiSpecificStreamInfo = NULL;

    /* Ouput stream parameters */
    outputParams.device = Pa_GetDefaultOutputDevice();
    outputParams.channelCount = NUM_CHN;
    outputParams.sampleFormat = paFloat32;
    outputParams.suggestedLatency =
        Pa_GetDeviceInfo(outputParams.device)->defaultLowOutputLatency;
    outputParams.hostApiSpecificStreamInfo = NULL;

    /* Open audio stream */
    err = Pa_OpenStream(&stream,
        &inputParams,
        &outputParams,
        SAMPLE_RATE, FRAMES_PER_BUFFER,
        paNoFlag, /* flags */
        paCallback,
        &io_data);

    if (err != paNoError) {
        printf("PortAudio error: open stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        return -1;
    }

     /* Start audio stream */
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: start stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        return -1;
    }

    /* Initialize ncurses
     * to permit interactive character input
     */
	initscr(); /* Start curses mode */
	cbreak();  /* Line buffering disabled */
	noecho(); /* Uncomment this if you don't want to echo characters when typing */

    printw("Select effect:\n");
    printw("P   pass input to output\n");
    printw("L   lowpass filter input\n");
    printw("H   highpass filter input\n");
    printw("D   run Dynamic Range Control on input\n");
    printw("Q to quit\n");
    mvprintw(6, 0, "               Selection: ");
	refresh();

	char ch = '\0'; /* init ch to null character */
    char *str;
    int done = 0;
	while (!done) {
		ch = toupper(getch()); /* ncurses will hang in this call */
		switch (ch) {
            case 'P':
                str = (char *)"Pass Through";
                p->pe = pass_through;
                break;
            case 'L':
                str = (char *)"Low Pass Filter";
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
                //sig_filter->init(LPF); //Have we said what LPF is?
                //p->pe = sig_filter;
//Begin
                sig_filter->init(LPF);
                p->pe = sig_filter;
//End
                break;
            case 'H':
                str = (char *)"High Pass Filter";
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
                // sig_filter->init(HPF); //Have we said what HPF is?
                // p->pe = sig_filter;
//Begin
                sig_filter->init(HPF);
                p->pe = sig_filter;
//End
                break;
            case 'D':
                str = (char *)"DRC";
//YOUR TODO: uncomment the following lines when you add the Filter or DRC features
                // sig_drc->init(0); //dummy mode value
                // p->pe = sig_drc;
//Begin
                sig_drc->init(0); //dummy mode value
                p->pe = sig_drc;
//End
                break;
            case 'Q':
                /* quit */
                str = (char *)"";
                done = 1;
                break;
        }
        mvprintw(6, 0, "Effect %c %16s, New selection: ", ch, str);
        refresh();
	}

	/* End curses mode  */
	endwin();

    /* Stop stream */
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: stop stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* Close stream */
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: close stream: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* Terminate PortAudio */
    err = Pa_Terminate();
    if (err != paNoError) {
        printf("PortAudio error: terminate: %s\n", Pa_GetErrorText(err));
        printf("\nExiting.\n");
        exit(1);
    }

    /* close optional output wav file */
    if (p->sndfile[1] != NULL) {
        sf_close (p->sndfile[1]);
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
    IOData *p = (IOData *)userData; /* Cast data passed through stream to our structure. */
    float *input = (float *)inputBuffer;
    float *output = (float *)outputBuffer;
    //printf("%lu\n", sizeof(output)/sizeof(output[0]));
    unsigned int num_frames = framesPerBuffer;
    baseEffect *pe = p->pe;

    /* optional file input, with wrap-around */
    if (p->sndfile[0] != NULL) {
        int count = sf_readf_float (p->sndfile[0], input, num_frames);
        if (count != num_frames) {
            /* rewind to beginning */
            sf_seek(p->sndfile[0], 0, SEEK_SET) ;
            /* read remainder of frame */
            sf_readf_float (p->sndfile[0], &input[count], num_frames-count);
        }
    }

    if (pe == NULL) {
        /* if no object selected, then zero output */
        for (int i=0; i<num_frames; i++)
            for (int j=0; j<NUM_CHN; j++)
                output[i*NUM_CHN+j] = 0.0;
    }
    else {
        /* process according to selected effect object */
        pe->process(input, output, num_frames, NUM_CHN);
    }

    /* optional file output */
    if (p->sndfile[1] != NULL) {
        sf_writef_float (p->sndfile[1], output, num_frames);
    }

    return 0;
}
