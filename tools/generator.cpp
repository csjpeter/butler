/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <string.h>
#include <stdio.h>
#include <csjp_logger.h>
#include <csjp_file.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

using namespace csjp;

class FieldDesc
{
public:
	FieldDesc(const StringChunk & constraint) :
		constraint(constraint)
	{
	}

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
	String constraint;
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
	Array<FieldDesc> constraints;

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
			if(0 < pos && (line[pos-1] == '\t' || line[pos-1] == ' '))
				pos -= 1;
			comment.assign(line.str + pos, line.length - pos);
			decl.assign(line.str, pos);
		}
		auto words = decl.split(";");
		for(unint i = 0; i < words.length; i++)
			words[i].trimFront(" \t");
		if(words.length == 1 && words[0] == "}"){
			state = &Declaration::parseDeclaration;
			return;
		}

		StringChunk type;
		StringChunk name;
		if(0 < words.length){
			words[0].trimBack(" \t");
			if(words[0].findLastOf(pos, " \t")){
				type.assign(words[0].str, pos);
				words[0].chopFront(pos + 1);
				name.assign(words[0]);
			}
		}

		bool key = false;
		if(1 < words.length){
			words[1].trim(" \t");
			auto modifiers = words[1].split(",");
			key = modifiers.has("key");
		}

		StringChunk sql;
		if(2 < words.length)
			sql = words[2];

		fields.setCapacity(fields.capacity+1);
		fields.add(name, type, sql, comment);
		if(key){
			keyFields.setCapacity(keyFields.capacity+1);
			keyFields.add(name, type, sql, comment);
		} else {
			nonKeyFields.setCapacity(nonKeyFields.capacity+1);
			nonKeyFields.add(name, type, sql, comment);
		}
	}

	void parseConstraintList(const StringChunk & line)
	{
		auto words = line.split(";");
		if(words.length == 1 && words[0] == "}"){
			state = &Declaration::parseDeclaration;
			return;
		}

		constraints.setCapacity(constraints.capacity+1);
		constraints.add(line);
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

	static void parseAllDeclaration(OwnerContainer<Declaration> & declarations,
			const String & data)
	{
		auto lines = data.split("\n", false);
		unsigned i;
		unint pos;
		bool declPhase = false;
		try{
			Object<Declaration> decl;
			for(i = 0; i < lines.length; i++){
				lines[i].trimBack("\t ");
				if(lines[i].findFirst(pos, "@BeginDecl@")) {
					decl.ptr = new Declaration();
					declPhase = true;
				} else if(lines[i].findFirst(pos, "@EndDecl@")){
					declarations.add(decl);
					declPhase = false;
				} else if(declPhase)
					decl->parse(lines[i]);
			}
		} catch (Exception & e) {
			e.note("Excpetion happened while processing declaration file line %u.", i);
			EXCEPTION(e);
		}
	}
};

inline bool operator<(const Declaration & a, const Declaration & b)
{
	return a.typeName < b.typeName;
}

inline bool operator<(const char * a, const Declaration & b)
{
	return a < b.typeName;
}

inline bool operator<(const Declaration & a, const char * b)
{
	return a.typeName < b;
}

class TemplateParser
{
	String & code;
	const Declaration & declaration;
	StringChunk tpl;
	unint tplLineNo;
	unint tplLastLineStartPos;
	const char * const until;
public:
	TemplateParser(String & code, const String & tpl, const Declaration & declaration) :
		code(code), declaration(declaration), tpl(tpl.str, tpl.length),
		tplLineNo(1), tplLastLineStartPos(0), until(tpl.str + tpl.length)
	{
	}

	void parseCommonMarker(StringChunk & tpl)
	{
		if(tpl.chopFront("@Type@")) {
			tpl.trimFront("\n\r");
			//code.trimBack("\t");
			code << declaration.typeName;
		} else if(tpl.length) {
			code << tpl[0];
			tpl.chopFront(1);
		}
	}

	void parseForEach(const char * what, const Array<FieldDesc> & fields)
	{
		String lastTag, endTag;
		lastTag << "@ForEach" << what << "Last@";
		endTag << "@ForEach" << what << "End@";

		unint pos;
		if(fields.length == 1 && tpl.findFirst(pos, lastTag.str))
			tpl.chopFront(pos + lastTag.length);

		tpl.trimFront("\n\r");
		code.trimBack("\t");

		StringChunk block(tpl.str, tpl.length);
		unint tplLineNoBegin = tplLineNo;
		unint tplLastLineStartPosBegin = tplLastLineStartPos;

		for(unint i = 0; i < fields.length && block.length;){
			const char * iter = block.str;
			while(iter < until && *iter != '@'){
				if(iter[0] == '\n'){
					tplLineNo++;
					tplLastLineStartPos = iter - block.str;
				}
				iter++;
			}
			code.append(block.str, iter - block.str);
			block.chopFront(iter - block.str);

			if(block.chopFront(lastTag.str)){
				if(i == fields.length - 2){
					tpl.chopFront(block.str - tpl.str);
					tpl.trimFront("\n\r");
					block = tpl;
					i++;
				} else {
					block = tpl;
					tplLineNo = tplLineNoBegin;
					tplLastLineStartPos = tplLastLineStartPosBegin;
					i++;
				}
			} else if(block.chopFront(endTag.str)){
				if(i == fields.length - 1){
					tpl.chopFront(block.str - tpl.str);
					tpl.trimFront("\n\r");
					block.clear();
				} else {
					block = tpl;
					tplLineNo = tplLineNoBegin;
					tplLastLineStartPos = tplLastLineStartPosBegin;
					i++;
				}
			} else if(block.chopFront("@FieldType@")) {
				code << fields[i].type;
			} else if(block.chopFront("@FieldName@")){
				code << fields[i].name;
			} else if(block.chopFront("@FieldEnumName@")){
				code << fields[i].enumName;
			} else if(block.chopFront("@FieldSqlDecl@")){
				code << "`" << fields[i].name << "` " << fields[i].sqlDecl;
			} else if(block.chopFront("@FieldComment@")){
				code << fields[i].comment;
			} else {
				parseCommonMarker(block);
			}
		}
	}

	void parse()
	{
		while(tpl.length){
			const char * iter = tpl.str;
			while(iter < until && *iter != '@'){
				if(iter[0] == '\n'){
					tplLineNo++;
					tplLastLineStartPos = iter - tpl.str;
				}
				iter++;
			}
			code.append(tpl.str, iter - tpl.str);
			tpl.chopFront(iter - tpl.str);

			if(tpl.chopFront("@ForEachFieldBegin@"))
				parseForEach("Field", declaration.fields);
			else if(tpl.chopFront("@ForEachKeyFieldBegin@"))
				parseForEach("KeyField", declaration.keyFields);
			else if(tpl.chopFront("@ForEachNonKeyFieldBegin@"))
				parseForEach("NonKeyField", declaration.nonKeyFields);
			else if(tpl.chopFront("@ForEachConstraintBegin@"))
				parseForEach("Constraint", declaration.constraints);
			else if(tpl.chopFront("@IfSingleKeyBegin@")){
				unint pos;
				if(declaration.keyFields.length != 1)
					if(tpl.findFirst(pos, "@IfSingleKeyEnd@"))
						tpl.chopFront(pos);
			} else if(tpl.chopFront("@IfSingleKeyEnd@"))
				;
			else
				parseCommonMarker(tpl);
		}
	}
};

class InputCode
{
	void (InputCode::*state)(const StringChunk &);

	const StringChunk & tplDir;
	const OwnerContainer<Declaration> & declarations;
	unint declIdx;
public:
	String code;

public:
	InputCode(const StringChunk & tplDiri, const OwnerContainer<Declaration> & declarations) :
		state(&InputCode::parseCode),
		tplDir(tplDir),
		declarations(declarations),
		declIdx(0)
	{
	}

	void parseCode(const StringChunk & line)
	{
		unint pos;
		if(!line.findFirst(pos, "@")){
			code << line << "\n";
			return;
		}
		if(line.findFirst(pos, "@declare@"))
			parseDeclaration(line, pos + 10);
		else if(line.findFirst(pos, "@include@"))
			parseInclude(line, pos + 10);
		else
			code << line << "\n";
	}

	void parseDeclaration(const StringChunk & line, unint pos)
	{
		StringChunk declaredClass(line.str + pos, line.length - pos);
		declaredClass.trim(" \t\n\r");
		declIdx = declarations.index(declaredClass.str);
	}

	void parseInclude(const StringChunk & line, unint pos)
	{
		StringChunk includeList(line.str + pos, line.length - pos);
		auto files = includeList.split(" ");
		for(unsigned i = 0; i < files.length; i++){
			String tplFileName(tplDir);
			tplFileName << files[i] << ".cpp";
			File tplFile(tplFileName);
			String tpl = tplFile.readAll();
			TemplateParser tplParser(code, tpl, declarations[declIdx]);
			tplParser.parse();
		}
	}

	void parse(const StringChunk & line)
	{
		(this->*state)(line);
	}
};



int main(int argc, char *args[])
{
	set_segmentation_fault_signal_handler();
	StringChunk tplDir;
	StringChunk declFileName;

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
				!strcmp(args[argi], "--declarations") ||
				!strcmp(args[argi], "-d"))){
			declFileName = args[argi+1];
			argi += 2;
			continue;
		}

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		LOG("Bad argument given: '%s'", args[argi]);
		return 1;
	}

	OwnerContainer<Declaration> declarations;
	Declaration declaration;

	InputCode generator(tplDir, declarations);
	File declFile(declFileName);
	String declBuf = declFile.readAll();
	declBuf.replace("\r", "");
	Declaration::parseAllDeclaration(declarations, declBuf);

	unsigned i;
	SorterOwnerContainer<String> lines;
	try{
		char *l = 0;
		ssize_t read = 0;
		while ((read = getline(&l, 0, stdin)) != -1) {
			Object<String> line(new String());
			line->adopt(l);
			line->trim("\n\r");
			line->trimBack("\t ");
			lines.add(line);
			StringChunk lineChunk(lines[i].str, lines[i].length);
			generator.parse(lineChunk);
		}
	} catch (Exception & e) {
		e.note("Excpetion happened while processing stdin line %u.", i);
		EXCEPTION(e);
	}
	puts(generator.code.str);
}
