#include <stdlib.h>
#include <string.h>
#include "options.h"

int recupererOption(char option[]) {
	if (!strcmp(option,"-h")) return OPT_H;
	else if (!strcmp(option,"-e")) return OPT_E;
	else if (!strcmp(option,"-s")) return OPT_S;
	else if (!strcmp(option,"-sh")) return OPT_SH;
	else if (!strcmp(option,"-ts")) return OPT_TS;
	else return NO_OPT;
}
