/*
 * This program is for testing the splistener library.
 */

#include <iostream>
#include <string>
#include "splistener.h"

int main(int argc, char *argv[]) {
	const char *model_path = "pocketsphinx/model/en-us/";
	std::string words;
	
	if (argc > 1)
		model_path = argv[1];

	if (!spInitListener(model_path, NULL)) {
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
