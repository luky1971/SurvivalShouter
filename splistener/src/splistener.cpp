
#include "splistener.h"

#include "pocketsphinx.h"

__declspec(dllexport) float initListener() {
	ps_decoder_t *ps = NULL;
	cmd_ln_t *config = NULL;

	// config = cmd_ln_init(NULL, ps_args(), TRUE, "-hmm", )

	return 5.0;
}
