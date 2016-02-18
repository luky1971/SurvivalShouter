/*
 * Copyright (c) 2016 Ahnaf Siddiqui.
 *
 * splistener utilizes PocketSphinx to continuously
 * recognize speech from a microphone and return spoken text.
 *
 * PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.
 */

#include "splistener.h"

#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>
#include <mutex>
#include "pocketsphinx.h"
#include "sphinxbase/ad.h"

static ps_decoder_t *ps = NULL;
static cmd_ln_t *config = NULL;

static ad_rec_t *mic = NULL;
static int16_t buf[SPLBUFSIZE];

static std::mutex ps_mtx;

static char *words = NULL;
static uint16_t words_buf_size;
static std::mutex words_mtx;
static std::thread listen_thread;

static std::ofstream sp_log;
static std::string sp_error;

static void spError(std::string err_msg) {
    sp_error = "ERROR: " + err_msg;
    sp_log << sp_error << std::endl;
}

static void spFatal(std::string err_msg) {
    sp_error = "FATAL ERROR: " + err_msg;
    sp_log << sp_error << std::endl;
    spCleanUp();
}

/**
 * You must sucessfully call spInitListener
 * once before using this function.
 *
 * Reads the next block of audio from the microphone
 * up to SPLBUFSIZE number of samples
 * (defined in splistener.h).
 * If an utterance was completed in that block,
 * the transcription is stored in the string words.
 *
 * When calling this function in a realtime loop, delay
 * by some amount of time between calls keeping in mind
 * your recording's sample rate and maximum samples read
 * per call (ex. sleep the thread for 100 milliseconds)
 * so that some audio can be recorded for the next call.
 *
 * @return true if a speech session was completed and
 *         transcribed this block, otherwise false.
 */
static bool spDecode() {
    static bool uttered = false;

    // lock pocketsphinx resources to make sure they 
    // don't get freed by main thread while in use
    std::lock_guard<std::mutex> ps_lock(ps_mtx);
    if(!mic || !ps)
        return false;

    int samples_read = ad_read(mic, buf, SPLBUFSIZE);
    if (samples_read <= 0) {
        spError("failed to read audio :(");
        return false;
    }

    ps_process_raw(ps, buf, samples_read, FALSE, FALSE);
    bool talking = ps_get_in_speech(ps);

    // Just started talking
    if (talking && !uttered) {
        uttered = true;
        return false;
    }

    // Stopped talking, so transcribe what was said
    // and begin the next utterance
    if (!talking && uttered) {
        ps_end_utt(ps);
        const char *trans = ps_get_hyp(ps, NULL);

        if (ps_start_utt(ps) < 0) {
            spError("failed to start utterance :(");
        }
        uttered = false;

        int l = strlen(trans);
        if (trans && l > 0) {
            std::lock_guard<std::mutex> lock(words_mtx);
            if (words && l + 1 > words_buf_size) {
                delete words;
                words = NULL;
            }
            if (!words) {
                words = new char[l + 1];
                words_buf_size = l + 1;
            }

            std::copy(trans, trans + l, words);
            words[l] = '\0';
            
            return true;
        }
    }

    return false;
}

/**
 * Transcribes speech from microphone audio
 * at intervals of specified length in milliseconds.
 *
 * @param delay The delay in milliseconds
 *              between decodes.
 */
static void spListen(int32_t sample_rate, int delay) {
    // Start recording
    sp_log << "Opening microphone with sample rate "
        << sample_rate << std::endl;
    if ((mic = ad_open_dev(NULL, sample_rate)) == NULL) {
        spFatal("failed to open microphone :(");
        return;
    }

    if (ad_start_rec(mic) < 0) {
        spFatal("failed to start recording :(");
        return;
    }
    if (ps_start_utt(ps) < 0) {
        spFatal("failed to start utterance :(");
        return;
    }

    std::chrono::milliseconds delay_dur(delay);
    // Record and decode loop
    while (ps && mic) {
        spDecode();
        std::this_thread::sleep_for(delay_dur);
    }
}

SPLEXPORT bool spInitListener(  const char *hmm_path,
                                const char *kws_path,
                                const char *lm_path,
                                const char *dict_path,
                                int32_t sample_rate,
                                int delay) {
    sp_log.open("splog.txt", std::ios_base::app);
    sp_error = "";

    if (kws_path) {
        config = cmd_ln_init(NULL, ps_args(), TRUE,
            "-hmm", hmm_path,
            "-kws", kws_path,
            "-dict", dict_path,
            NULL);
    }
    else {
        config = cmd_ln_init(NULL, ps_args(), TRUE,
            "-hmm", hmm_path,
            "-lm", lm_path,
            "-dict", dict_path,
            NULL);
    }

    if (config == NULL) {
        spFatal("pocketsphinx command line initialization failed :(");
        return false;
    }
    
    if ((ps = ps_init(config)) == NULL) {
        spFatal("pocketsphinx decoder initialization failed :(");
        return false;
    }

    // Record on new thread
    listen_thread = std::thread(spListen, sample_rate, delay);
    return true;
}

SPLEXPORT char *spGetWords() {
    std::lock_guard<std::mutex> lock(words_mtx);
    if (words) {
        char *s = words;
        words = NULL;
        return s;
    }
    else {
        return new char('\0');
    }
}

SPLEXPORT void spCleanUp() {
    std::lock_guard<std::mutex> ps_lock(ps_mtx);
    if (mic) {
        ad_close(mic);
        mic = NULL;
    }
    if (ps) {
        ps_free(ps);
        ps = NULL;
    }
    if (config) {
        cmd_ln_free_r(config);
        config = NULL;
    }
    if (words) {
        delete words;
        words = NULL;
    }
    if (sp_log.is_open()) {
        sp_log.close();
    }
}

SPLEXPORT char *spGetError() {
    char *s = new char[sp_error.size() + 1];
    std::copy(sp_error.begin(), sp_error.end(), s);
    s[sp_error.size()] = '\0';
    return s;
}
