/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <string.h>
#include <stdio.h>
#include <csjp_logger.h>
#include <csjp_file.h>

using namespace csjp;

class FieldDesc
{
public:
	FieldDesc(const StringChunk & name, const StringChunk & type,
			const StringChunk & sqlDecl, const StringChunk & comment) :
		name(name),
		type(type),
		enumName(name),
		sqlDecl(sqlDecl),
		comment(comment)
	{
		if('a' <= enumName[0] && enumName[0] <= 'z')
			enumName[0] += 'A'-'a';
	}

	String name;
	String type;
	String enumName;
	String sqlDecl;
	String comment;
};

bool operator<(const FieldDesc & a, const FieldDesc & b)
{
	return a.name < b.name;
}

class Generator
{
	enum State {
		Default,
		Declaration,
		FieldList,
		ConstraintList,
		NumOfStates
	};
public:
	enum State state;
	StringChunk tplDir;

	StringChunk typeName;
	Array<FieldDesc> fields;
	Array<FieldDesc> keyFields;
	Array<FieldDesc> nonKeyFields;
	Array<String> constraints;

	String keyList; // ???

	String code;

Generator(const StringChunk & tplDir) :
	state(State::Default),
	tplDir(tplDir)
{
}

void include_CommonMarkerValues(StringChunk & tpl)
{
	if(tpl.chopFront("@Type@")) {
		tpl.trimFront("\n\r");
		//code.trimBack("\t");
		code << typeName;
	} else {
		code << tpl[0];
		tpl.chopFront(1);
	}
}

void include_foreachField(StringChunk & tpl, const Array<FieldDesc> & fields)
{
	unsigned pos = 0;
	for(unsigned i = 0; i < fields.length; i++){
		if(*tpl.data != '@'){
			code << *tpl.data;
			tpl.chopFront(1);
		}
		if(tpl.chopFront("@Type@")) {
			tpl.trimFront("\n\r");
			//code.trimBack("\t");
			code << typeName;
		} else if(tpl.chopFront("@FieldName@")){
			code << fields[i].name;
		} else if(tpl.chopFront("@SqlDecl@")){
			if(fields[i].name.length)
				code << "`" << fields[i].name << "` ";
			code << fields[i].sqlDecl;
		} else
			include_CommonMarkerValues(tpl);
	}
}

void processTemplateLine(StringChunk & tpl)
{
	unint pos;
        if(tpl.chopFront("@ForEachFieldBegin@")){
		tpl.trimFront("\n\r");
                code.trimBack("\t");
                /*$sectionPos = tpl.find(pos, "@ForEachFieldEnd@");
                $section = rtrim(substr($tpl, $pos, $sectionPos-$pos), "\t");
                $pos = $sectionPos + 17;
                include_skip_newlines($tpl, $pos);
                $sectionLast = strstr($section, "@ForEachFieldLast@");
                if($sectionLast !== false){
                        $sectionLast = ltrim(substr($sectionLast, 18), "\n\r");
                        $section = rtrim(strstr($section, "@ForEachFieldLast@", true), "\t");
                }*/
/*                unsigned last = count(classFields) - 1;
                for(int i = 0; i <= last; i++)
                        if(last == i && sectionLast !== false)
                                include_foreachField(sectionLast, classFields[i]);
                        else
                                include_foreachField(section, classFields[i]);
				*/
	} else
		include_CommonMarkerValues(tpl);
}

void readDeclaration(StringChunk & line)
{
	unint pos;
	switch(state){
		case State::Declaration :
			{
			auto words = line.split(" \t");
			if(words[0] == "Class"){
				typeName = words[1];
				// cleanup field and constraint info
			} else if(words[0] == "Fields" && words[1] == "{"){
				state = State::FieldList;
			} else if(words[0] == "Constraints" && words[1] == "{"){
				state = State::ConstraintList;
			}
			}
			break;
		case State::FieldList :
			{
			StringChunk comment;
			if(line.findFirst(pos, "//")){
				comment.assign(line.data + pos, line.length - pos);
				line.cutAt(pos);
			}
			auto words = line.split(";");
			if(words.length == 1 && words[0] == "}"){
				state = State::Declaration;
				break;
			}
			
			StringChunk type;
			StringChunk name;
			if(0 < words.length){
				words[0].trim(" \t");
				if(words[0].findLastOf(pos, " \t")){
					type.assign(words[0].data, pos);
					words[0].chopFront(pos + 1);
					name.assign(words[0]);
				}
			}

			bool key = false;
			if(1 < words.length){
				auto modifiers = words[1].split(",");
				key = modifiers.has("key");
			}

			StringChunk sql;
			if(2 < words.length)
				sql = words[2];

			fields.add(name, type, sql, comment);
			if(key)
				keyFields.add(name, type, sql, comment);
			else
				nonKeyFields.add(name, type, sql, comment);
			}
			break;
		case State::ConstraintList :
			{
			auto words = line.split(";");
			if(words.length == 1 && words[0] == "}"){
				state = State::Declaration;
				break;
			}
			
			line.trim("\n ");
			constraints.add(line);
			}
			break;
		default:
			break;
	}
}

void processInputLine(StringChunk & line)
{
	unint pos;
	while(line.length){
		switch(state){
			case State::Declaration :
				if(line.findFirst(pos, "@EndDecl@")){
					StringChunk decl(line.data, pos);
					readDeclaration(decl);
					line.chopFront(pos + strlen("@EndDecl@"));
					state = State::Default;
				} else
					readDeclaration(line);
				code << line;
				break;
			default :
				if(line.findFirst(pos, "@")){
					code.append(line.data, pos);
					line.chopFront(pos);
					if(line.chopFront("@BeginDecl@")){
						code.append("@BeginDecl@");
						state = State::Declaration;
					} else if(line.chopFront("@include@")){
						auto files = line.split(" ");
						for(unsigned i = 0; i < files.length; i++){
							String tplFileName(tplDir);
							tplFileName << files[i] << ".cpp";
							File tplFile(tplFileName);
							String tpl = tplFile.readAll();
							tpl.replace("\r", "");
							auto lines = tpl.split("\n");
							unsigned j;
							try{
								for(j = 0; j < lines.length; j++){
									lines[j].trimBack("\t ");
									processTemplateLine(lines[j]);
								}
							} catch (Exception & e) {
								e.note("Excpetion happened while processing file %s line %u.", tplFileName.str, j);
								EXCEPTION(e);
								break;
							}
						}
					}
				} else {
					code << line;
					line.clear();
				}
		}
	}
	code << "\n";
}

};



int main(int argc, char *args[])
{
	StringChunk tplDir;
	StringChunk inputFileName;

#ifdef DEBUG
	csjp::verboseMode = true;
#endif

	int argi = 1;

	if(1 <= argc - argi && (
			!strcmp(args[argi], "--help") ||
			!strcmp(args[argi], "-h"))){
		printf(	"Usage: %s [--logdir|-l dir] [--verbose|-v]\n"
			"	%s [--help|-h]\n"
			"\n", args[0], args[0]);
		return 0;
	}

	while(argi < argc){
		if(2 <= argc - argi && (
				!strcmp(args[argi], "--logdir") ||
				!strcmp(args[argi], "-l"))){
			csjp::setLogDir(args[argi+1]);
			argi += 2;
			printf("argi 1: %d\n", argi);
			continue;
		}
		if(1 <= argc - argi && (
				!strcmp(args[argi], "--verbose") ||
				!strcmp(args[argi], "-v"))){
			csjp::verboseMode = true;
			argi++;
			printf("argi 2: %d\n", argi);
			continue;
		}
		if(1 <= argc - argi && (
				!strcmp(args[argi], "--tpldir") ||
				!strcmp(args[argi], "-t"))){
			tplDir = args[argi+1];
			argi += 2;
			printf("argi 3: %d\n", argi);
			continue;
		}
		if(1 <= argc - argi && (
				!strcmp(args[argi], "--inputfilename") ||
				!strcmp(args[argi], "-i"))){
			inputFileName = args[argi+1];
			argi += 2;
			printf("argi 4: %d\n", argi);
			continue;
		}

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		LOG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

	Generator generator(tplDir);
	File inputFile(inputFileName);
	String input = inputFile.readAll();
	input.replace("\r", "");
	auto lines = input.split("\n");
	unsigned i;
	try{
		for(i = 0; i < lines.length; i++){
			lines[i].trimBack("\t ");
			generator.processInputLine(lines[i]);
		}
	} catch (Exception & e) {
		e.note("Excpetion happened while processing file %s line %u.", inputFileName.data, i);
		EXCEPTION(e);
	}
	puts(generator.code.str);
}
