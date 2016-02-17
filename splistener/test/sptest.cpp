/*
 * This program is for testing the splistener library.
 */

#include <iostream>
#include <string>
#include "splistener.h"

int main(int argc, char *argv[]) {
	const char *model_path = "pocketsphinx/model/en-us/";
	const char *kws_path = NULL;
	std::string words, arg;
	
	for (int i = 1; i < argc - 1; ++i) {
		arg = std::string(argv[i]);
		if (arg == "-m") {
			model_path = argv[i + 1];
			++i;
		}
		if (arg == "-k") {
			kws_path = argv[i + 1];
			++i;
		}
	}

	std::cout << "Using model/dict in  " << model_path << std::endl;
	if (kws_path)
		std::cout << "Using keyword search with " << kws_path << std::endl;

	if (!spInitListener(model_path, kws_path)) {
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
