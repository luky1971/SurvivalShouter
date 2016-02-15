/*
 * Copyright (c) 2016 Ahnaf Siddiqui.
 *
 * splistener utilizes PocketSphinx to continuously 
 * recognize speech from a microphone and return spoken text.
 *
 * PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.
 */

// You may modify these according to your needs
#define SPLEXPORT __declspec(dllexport)
#define SPLCDECL

#ifdef SPLCDECL
extern "C" {
#endif

/**
 * Initializes the PocketSphinx decoder and 
 * begins recording from the default microphone.
 *
 * Call before using other functions.
 *
 * @param model_path Path to the directory with the 
 *                   language model and dictionary files 
 *                   that should be used (ex. 
 *                   pocketsphinx/model/en-us/)
 * @return 1 if initialization was successful, otherwise 0.
 */
SPLEXPORT int spInitListener(const char *model_path);

/**
 * Call at the end of a speech recognition session
 * to free splistener's resources.
 */
SPLEXPORT void spCleanUp();

#ifdef SPLCDECL
}
#endif
