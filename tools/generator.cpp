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
	FieldDesc(StringChunk name, StringChunk type, StringChunk sqlDecl, StringChunk comment) :
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

	String typeName;
	Array<FieldDesc> fields;
	Array<FieldDesc> keyFields;
	Array<FieldDesc> nonKeyFields;
	Array<String> constraints;

	String keyList; // ???

	String code;

Generator(const String & tplDir) :
	state(State:Default),
	tplDir(tplDir)
{
}

void include_CommonMarkerValues(StringChunk & tpl)
{
	if(tpl.chopFront("@Type@")) {
		tpl.trimFront("\n\r");
		//code.trimBack("\t");
		code << typeName;
	} else if(tpl.chopFront("@TableAutoIncField@")) {
		tpl.trimFront("\n\r");
		code << sqlTableAutoIncField;
	} else if(tpl.chopFront("@KeyList@")) {
		tpl.trimFront("\n\r");
		code << strtolower(keyList);
	} else {
		code << tpl[0];
		tpl.chopFront(1);
	}
}

void include_foreachField(StringChunk & tpl, const Json & field)
{
	unsigned pos = 0;
	while(tpl.length){
		if(*tpl.data != '@'){
			code << *tpl.data;
			tpl.chopFront(1);
	if(tpl.chopFront("@Type@")) {
		tpl.trimFront("\n\r");
		//code.trimBack("\t");
		code << typeName;
	} else if(tpl.chopFront("@FieldName@")){
		code << field["name"];
	} else if(tpl.chopFront("@SqlDecl@")){
		if(field["name"].length)
			code << "`" << field["name"] << "` ";
		code << field["sqlDecl"];
	} else if(tpl.chopFront("@FieldSqlDecl@")){
		code << field["default"];
	} else
		include_CommonMarkerValues(tpl);
	}
}

void processTemplateLine(StringChunk & tpl)
{
	unsigned pos;
        if(tpl.chopFront("@ForEachFieldBegin@")){
		tpl.trimFront("\n\r");
                code.trimBack("\t");
                $sectionPos = tpl.find(pos, "@ForEachFieldEnd@");
                $section = rtrim(substr($tpl, $pos, $sectionPos-$pos), "\t");
                $pos = $sectionPos + 17;
                include_skip_newlines($tpl, $pos);
                $sectionLast = strstr($section, "@ForEachFieldLast@");
                if($sectionLast !== false){
                        $sectionLast = ltrim(substr($sectionLast, 18), "\n\r");
                        $section = rtrim(strstr($section, "@ForEachFieldLast@", true), "\t");
                }
                $last = count($classFields) - 1;
                for($i=0; $i<=$last; $i++)
                        if($last == $i && $sectionLast !== false)
                                include_foreachField($sectionLast, $classFields[$i]);
                        else
                                include_foreachField($section, $classFields[$i]);
	} else
		include_CommonMarkerValues($tpl, $pos);
}

void readDeclaration(StringChunk & line)
{
	unsigned pos;
	switch(state){
		case State::Declaration :
			auto words = line.split(" \t");
			if(words[0] == "Class"){
				typeName = words[1];
				// cleanup field and constraint info
			} else if(words[0] == "Fields" && words[1] == "{"){
				state = State::FieldList;
			} else if(words[0] == "Constraints" && words[1] == "{"){
				state = State::ConstraintList;
			}
			break;
		case State::FieldList :
			StringChunk comment;
			if(line.find(pos, "//")){
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
				word[0].trim(" \t");
				if(word[0].findLastOf(pos, " \t")){
					type.assign(word[0].data, pos);
					word[0].chopFront(pos + 1);
					name.assign(word[0]);
				}
			}

			bool key = false;
			if(1 < words.length){
				auto modifiers = word[1].split(",");
				key = modifiers.has("key");
			}

			StringChunk sql((2 < words.length) ? word[2] : "");

			fields.add(name, type, sql, comment);
			if(key)
				keyFields.add(name, type, sql, comment);
			else
				nonKeyFields.add(name, type, sql, comment);
			break;
		case State::ConstraintList :
			auto words = line.split(";");
			if(words.length == 1 && words[0] == "}"){
				state = State::Declaration;
				break;
			}
			
			line.trim();
			constraints.add(line);
			break;
		default:
	}
}

void processInputLine(const StringChunk & line)
{
	unsigned pos;
	while(line.length){
		switch(state){
			case State::Declaration :
				if(line.find(pos, "@EndDecl@")){
					StringChunk decl(line.data, pos);
					readDeclaration(decl);
					line.chopFront(pos + strlen("@EndDecl@"));
					state = State::Default;
				} else
					readDeclaration(line);
				code << line;
				break;
			default :
				if(line.find(pos, "@")){
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
							tpl = tplFile.readAll();
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
		if(1 <= argc - argi && (
				!strcmp(args[argi], "--inputfilename") ||
				!strcmp(args[argi], "-i"))){
			inputFileName = args[argi+1];
			argi += 2;
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
		e.note("Excpetion happened while processing file %s line %u.", inputFileName.str, i);
		EXCEPTION(e);
		break;
	}
	puts(generator.code.str);
}
