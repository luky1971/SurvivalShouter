/*
 * This program is for testing the splistener library.
 */

#include <iostream>
#include <string>
#include "splistener.h"

int main(int argc, char *argv[]) {
	const char *hmm = "pocketsphinx/model/en-us/en-us";
	const char *lm = "pocketsphinx/model/en-us/en-us.lm.bin";
	const char *dict = "pocketsphinx/model/en-us/cmudict-en-us.dict";
	const char *kws = NULL;
	std::string words, arg;
	
	for (int i = 1; i < argc - 1; ++i) {
		arg = std::string(argv[i]);
		if (arg == "-hmmm") {
			hmm = argv[i + 1];
			++i;
		}
		else if (arg == "-lm") {
			lm = argv[i + 1];
			++i;
		}
		else if (arg == "-dict") {
			dict = argv[i + 1];
			++i;
		}
		else if (arg == "-kws") {
			kws = argv[i + 1];
			++i;
		}
	}

	if (!spInitListener(hmm, kws, lm, dict)) {
		std::cout << spGetError() << std::endl;
		return 1;
	}

	while (words != "exit") {
		words = std::string(spGetWords());
		if (words.length() > 0) {
			std::cout << words << std::endl;
		}
	}

	spCleanUp();
	return 0;
}
