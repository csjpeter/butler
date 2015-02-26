/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <string.h>
#include <csjp_logger.h>
#include <csjp_file.h>


int main(int argc, char *args[])
{
#ifdef DEBUG
	csjp::verboseMode = true;
#endif

	int argi = 1;

	if(1 <= argc - argi && (
			  !strcmp(args[argi], "--help") ||
			  !strcmp(args[argi], "-h"))){
		printf(	"Usage: %s [--logdir|-l dir] [--verbose|-v]\n"
			"       %s [--help|-h]\n"
			"\n", args[0], args[0]);
		return 0;
	}

	while(argi < argc){
		if(2 <= argc - argi && (
				  !strcmp(args[argi], "--logdir") ||
				  !strcmp(args[argi], "-l"))){
			csjp::setLogDir(args[argi+1]);
			argi += 2;
			continue;
		}
		if(1 <= argc - argi && (
				  !strcmp(args[argi], "--verbose") ||
				  !strcmp(args[argi], "-v"))){
			csjp::verboseMode = true;
			argi++;
			continue;
		}

		if(!strcmp(args[argi], "")) /* The rests are Qt arguments. */
			break;

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		LOG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

}
