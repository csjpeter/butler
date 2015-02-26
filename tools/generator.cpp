/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <string.h>
#include <csjp_logger.h>
#include <csjp_file.h>

using namespace csjp;

class Generator
{
public:
	String sqlTableName;
	String sqlTableAutoIncField;
	String localDbClass;
	String remoteName;
	String keyList;
        String customizations;
        String code;

void include_CommonMarkerValues(StringChunk & tpl, unsigned & pos)
{
        if(tpl.startsWith("@TableName@")) {
                tpl.chopFront(11);
		tpl.trimFront("\n\r");
		//code.trimBack("\t");
                code << $sqlTableName;
        }/* else if(substr($tpl, $pos, 19) == "@TableAutoIncField@") {
                $pos += 19;
                include_skip_newlines($tpl, $pos);
                #$code = rtrim($code, "\t");
                $code .= $sqlTableAutoIncField;
        } else if(substr($tpl, $pos, 14) == "@LocalDbClass@") {
                $pos += 14;
                include_skip_newlines($tpl, $pos);
                #$code = rtrim($code, "\t");
                $code .= $localDbClass;
        } else if(substr($tpl, $pos, 12) == "@RemoteName@") {
                $pos += 12;
                include_skip_newlines($tpl, $pos);
                #$code = rtrim($code, "\t");
                $code .= $remoteName;
        } else if(substr($tpl, $pos, 9) == "@KeyList@") {
                $pos += 9;
                include_skip_newlines($tpl, $pos);
                #$code = rtrim($code, "\t");
                $code .= strtolower($keyList);
        } else if(substr($tpl, $pos, 17) == "@RemoteNameLower@") {
                $pos += 17;
                include_skip_newlines($tpl, $pos);
                #$code = rtrim($code, "\t");
                $code .= strtolower($remoteName);
        } else if(substr($tpl, $pos, 19) == "@CustomizationInit@") {
                $pos += 19;
                if(in_array("init", $customizations))
                        $code .= "\$o->init();";
                else {
                        include_skip_newlines($tpl, $pos);
                        $code = rtrim($code, "\t");
                }
        } else if(substr($tpl, $pos, 20) == "@CustomizationApply@") {
                $pos += 20;
                if(in_array("apply", $customizations))
                        $code .= "\$this->apply();";
                else {
                        include_skip_newlines($tpl, $pos);
                        $code = rtrim($code, "\t");
                }
        }*/ else {
                code << tpl[0];
                tpl.chopFront(1);
        }
}

};












int main(int argc, char *args[])
{
	String tplDir;

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
		if(1 <= argc - argi && (
				  !strcmp(args[argi], "--tpldir") ||
				  !strcmp(args[argi], "-t"))){
			tplDir = args[argi+1];
			argi += 2;
			continue;
		}

		if(!strcmp(args[argi], "")) /* The rests are Qt arguments. */
			break;

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		LOG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

	Generator generator;
}
