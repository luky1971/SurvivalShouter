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
#include "pocketsphinx.h"
#include "sphinxbase/ad.h"

static ps_decoder_t *ps = NULL;
static cmd_ln_t *config = NULL;

static ad_rec_t *ad;
static int16_t buf[SPLBUFSIZE];
static bool uttered;

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

SPLEXPORT bool spInitListener(	const char *model_path, 
								const char *mic, 
								int32_t sample_rate) {
	sp_log.open("splog.txt", std::ios_base::app);
	sp_error = "";

	std::string path(model_path);
	if (path.back() == '/')
		path.pop_back();

	std::string lang(path.substr(path.find_last_of("/\\") + 1));
	
	sp_log << std::endl 
		<< "Using language " << lang << " at " << path << std::endl;

	// Initialize pocketsphinx
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", (path + "/" + lang).c_str(),
		"-lm", (path + "/" + lang + ".lm.bin").c_str(),
		"-dict", (path + "/cmudict-" + lang + ".dict").c_str(),
		NULL);

	if (config == NULL) {
		spFatal("pocketsphinx config initialization failed :(");
		return false;
	}
	
	if ((ps = ps_init(config)) == NULL) {
		spFatal("pocketsphinx initialization failed :(");
		return false;
	}


	// Start recording
	sp_log << "Opening microphone with sample rate " 
		<< sample_rate << std::endl;
	if ((ad = ad_open_dev(mic, sample_rate)) == NULL) {
		spFatal("failed to open microphone :(");
		return false;
	}
	if (ad_start_rec(ad) < 0) {
		spFatal("failed to start recording :(");
		return false;
	}
	if (ps_start_utt(ps) < 0) {
		spFatal("failed to start utterance :(");
		return false;
	}
	uttered = false;

	return true;
}

SPLEXPORT bool spListen(char *words, int len) {
	int samples_read = ad_read(ad, buf, SPLBUFSIZE);
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
			l = l <= len - 1 ? l : len - 1;
			std::copy(trans, trans + l, words);
			words[l] = '\0';
			return true;
		}
	}

	return false;
}

SPLEXPORT void spCleanUp() {
	if (ad)
		ad_close(ad);
	if (ps)
		ps_free(ps);
	if (config)
		cmd_ln_free_r(config);
	// Note: log file is closed automatically
}

SPLEXPORT const char *spGetError() {
	return sp_error.c_str();
}
