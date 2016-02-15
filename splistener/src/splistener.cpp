/*
 * Copyright (c) 2016 Ahnaf Siddiqui.
 *
 * splistener utilizes PocketSphinx to continuously
 * recognize speech from a microphone and return spoken text.
 *
 * PocketSphinx is Copyright (c) 1999-2008 Carnegie Mellon University.
 */

#include "splistener.h"

#include <string>
#include "pocketsphinx.h"

static ps_decoder_t *ps = NULL;
static cmd_ln_t *config = NULL;

SPLEXPORT int spInitListener(const char *model_path) {
	std::string path(model_path);

	if (path.back() != '/')
		path += '/';

	/*
	std::string hmm(path + "en-us");
	std::string lm(path + "en-us.lm.bin");
	std::string dict(path + "cmudict-en-us.dict");
	*/
	
	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", (path + "en-us").c_str(),
		"-lm", (path + "en-us.lm.bin").c_str(),
		"-dict", (path + "cmudict-en-us.dict").c_str(),
		NULL);

	if (config == NULL
		|| (ps = ps_init(config)) == NULL) {
		cmd_ln_free_r(config);
		return 0;
	}

	return 1;
}

SPLEXPORT void spCleanUp() {
	if(ps)
		ps_free(ps);
	if(config)
		cmd_ln_free_r(config);
}
