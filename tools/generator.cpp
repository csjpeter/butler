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

class Declaration
{
	void (Declaration::*state)(const StringChunk &);
public:
	StringChunk typeName;
	Array<FieldDesc> fields;
	Array<FieldDesc> keyFields;
	Array<FieldDesc> nonKeyFields;
	Array<StringChunk> constraints;

	Declaration() { clear(); }

	void clear()
	{
		state = &Declaration::parseDeclaration;
		typeName.clear();
		fields.clear();
		keyFields.clear();
		nonKeyFields.clear();
		constraints.clear();
	}

	void parseFieldList(const StringChunk & line)
	{
		unint pos;
		StringChunk comment;
		StringChunk decl(line);
		if(line.findFirst(pos, "//")){
			comment.assign(line.data + pos, line.length - pos);
			decl.assign(line.data, pos);
		}
		auto words = decl.split(";");
		if(words.length == 1 && words[0] == "}"){
			state = &Declaration::parseDeclaration;
			return;
		}

		StringChunk type;
		StringChunk name;
		if(0 < words.length){
			words[0].trimBack(" \t");
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

		FieldDesc field(name, type, sql, comment);
		fields.add(field);
		if(key)
			keyFields.add(field);
		else
			nonKeyFields.add(field);
	}

	void parseConstraintList(const StringChunk & line)
	{
		auto words = line.split(";");
		if(words.length == 1 && words[0] == "}"){
			state = &Declaration::parseDeclaration;
			return;
		}

		constraints.add(StringChunk(line));
	}

	void parseDeclaration(const StringChunk & line)
	{
		auto words = line.split(" \t");
		if(words[0] == "Class"){
			typeName = words[1];
			// cleanup field and constraint info
		} else if(words[0] == "Fields" && words[1] == "{"){
			state = &Declaration::parseFieldList;
		} else if(words[0] == "Constraints" && words[1] == "{"){
			state = &Declaration::parseConstraintList;
		}
	}

	void parse(const StringChunk & line)
	{
		(this->*state)(line);
	}
};

class TemplateParser
{
	String & code;
	const Declaration & declaration;
public:
	TemplateParser(String & code, const Declaration & declaration) :
		code(code), declaration(declaration)
	{
	}

	void include_CommonMarkerValues(StringChunk & tpl)
	{
		if(tpl.chopFront("@Type@")) {
			tpl.trimFront("\n\r");
			//code.trimBack("\t");
			code << declaration.typeName;
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
				code << declaration.typeName;
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

	void parse(const StringChunk & tpl)
	{
	}
};

class InputCode
{
	void (InputCode::*state)(const StringChunk &);

	const StringChunk & tplDir;
	Declaration declaration;
public:
	String code;

public:
	InputCode(const StringChunk & tplDir) :
		state(&InputCode::parseCode),
		tplDir(tplDir)
	{
	}

	void parseCode(const StringChunk & line)
	{
		unint pos;
		if(!line.findFirst(pos, "@")){
			code << line << "\n";
			return;
		}
		if(line.findFirst(pos, "@BeginDecl@")){
			declaration.clear();
			state = &InputCode::parseDeclaration;
		} else if(line.findFirst(pos, "@include@")){
			pos += 10;
			StringChunk includeList(line.data + pos, line.length - pos);
			parseInclude(includeList);
		} else
			code << line << "\n";
	}

	void parseDeclaration(const StringChunk & line)
	{
		unint pos;
		if(line.findFirst(pos, "@EndDecl@")){
			state = &InputCode::parseCode;
		} else
			declaration.parse(line);
	}

	void parseInclude(const StringChunk & includeList)
	{
		auto files = includeList.split(" ");
		for(unsigned i = 0; i < files.length; i++){
			String tplFileName(tplDir);
			tplFileName << files[i] << ".cpp";
			File tplFile(tplFileName);
			String tpl = tplFile.readAll();
			StringChunk chunk(tpl.str, tpl.length);
			TemplateParser tplParser(code, declaration);
			tplParser.parse(chunk);
		}
	}

	void parse(const StringChunk & line)
	{
		(this->*state)(line);
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

	InputCode generator(tplDir);
	File inputFile(inputFileName);
	String input = inputFile.readAll();
	input.replace("\r", "");
	auto lines = input.split("\n");
	unsigned i;
	try{
		for(i = 0; i < lines.length; i++){
			lines[i].trimBack("\t ");
			generator.parse(lines[i]);
		}
	} catch (Exception & e) {
		e.note("Excpetion happened while processing file %s line %u.", inputFileName.data, i);
		EXCEPTION(e);
	}
	puts(generator.code.str);
}
