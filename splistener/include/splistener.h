/*
 * Copyright (c) 2016 Ahnaf Siddiqui.
 *
 * splistener utilizes PocketSphinx to continuously 
 * recognize speech from a microphone and return spoken text.
 * The current version of splistener is NOT re-entrant;
 * only use this in one thread per application!
 *
 * PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.
 */

#include <stdint.h>

// You may modify these according to your needs
#define SPLBUFSIZE 2048
#define SPLEXPORT __declspec(dllexport)
#define SPLCDECL

#ifdef SPLCDECL
extern "C" {
#endif

/**
 * Initializes the PocketSphinx decoder and 
 * begins recording from a microphone.
 *
 * Call before using other functions.
 *
 * @param model_path Path to the directory with the 
 *                   language model and dictionary files 
 *                   that should be used (ex. 
 *                   pocketsphinx/model/en-us/).
 * @param mic Name of the microphone to use,
 *            or NULL to use the default microphone.
 * @param sample_rate The sample rate of the recording 
 *        (ex. try 16000).
 * @return true if initialization was successful, 
 *         otherwise false.
 */
SPLEXPORT bool spInitListener(	const char *model_path, 
								const char *mic, 
								int32_t sample_rate);

/**
 * You must sucessfully call spInitListener 
 * once before using this function.
 *
 * Reads the next block of audio from the microphone
 * up to SPLBUFSIZE number of samples (defined above).
 * If a speaking session was completed in that block,
 * the transcription is stored in the string words.
 *
 * When calling this function in a realtime loop, delay
 * by some amount of time between calls keeping in mind
 * your recording's sample rate and maximum samples read
 * per call (ex. sleep the thread for 100 milliseconds)
 * so that some audio can be recorded for the next call.
 *
 * If you want to be informed of errors in this 
 * function, you will have to check manually
 * (perhaps periodically) by calling spGetError.
 *
 * @param words String where speech transcription is 
 *              stored if the function returns true.
 * @param len Maximum number of characters to store in 
 *            string words (including null character).
 * @return true if a speech session was completed and
 *         transcribed this block, otherwise false.
 */
SPLEXPORT bool spListen(char *words, int len);

/**
 * Call at the end of a speech recognition application
 * to free splistener's resources.
 */
SPLEXPORT void spCleanUp();

/**
 * Gets the last error if there was one.
 * Returns a writeable copy of the string for 
 * compatibility with managed .NET environments.
 *
 * @return The last error message if there was an error,
 *         otherwise an empty string.
 */
SPLEXPORT char *spGetError();

#ifdef SPLCDECL
}
#endif
