/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <cxxabi.h>

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
		constraint(constraint),
		key(false),
		set(false),
		link(false)
	{
	}

	FieldDesc(const StringChunk & name, const StringChunk & type,
			const StringChunk & sqlDecl, const StringChunk & comment,
			bool key, bool set, bool link) :
		name(name),
		type(type),
		enumName(name),
		sqlDecl(sqlDecl),
		comment(comment),
		key(key),
		set(set),
		link(link)
	{
		if('a' <= enumName[0] && enumName[0] <= 'z')
			enumName[0] += 'A'-'a';
		if(set){
			setSubType.assign(type);
			setSubType.chopBack(3);
		}
	}

	String name;
	String type;
	String setSubType;
	String enumName;
	String sqlDecl;
	String comment;
	String constraint;
	bool key;
	bool set;
	bool link;
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

	Declaration() { clear(); }

	void clear()
	{
		state = &Declaration::parseDeclaration;
		typeName.clear();
		fields.clear();
	}

	void parseFieldList(const StringChunk & line)
	{
		unint pos;
		StringChunk comment;
		StringChunk decl(line);
		// cut of the comment
		if(line.findFirst(pos, "//")){
			if(0 < pos && (line[pos-1] == '\t' || line[pos-1] == ' '))
				pos -= 1;
			comment.assign(line.str + pos, line.length - pos);
			decl.assign(line.str, pos);
		}

		auto words = decl.split(";");
		for(auto& word : words) word.trim(" \t");

		if(!words.length) // skip empty lines
			return;
		if(words[0] == "") // skip empty lines
			return;

		if(words.length == 1 && words[0] == "}"){
			state = &Declaration::parseDeclaration;
			return;
		}

		StringChunk type;
		StringChunk name;
		if(0 < words.length){
			if(words[0].findLastOf(pos, " \t")){
				type.assign(words[0].str, pos);
				words[0].chopFront(pos + 1);
				name.assign(words[0]);
			}
		}

		bool key = false;
		bool set = false;
		bool link = false;
		if(1 < words.length){
			auto modifiers = words[1].split(",");
			for(auto& mod : modifiers) mod.trim(" \t");
			key = modifiers.has("key");
			set = modifiers.has("set");
			link = modifiers.has("link");
		}

		StringChunk sql;
		if(2 < words.length)
			sql = words[2];

		fields.setCapacity(fields.capacity+1);
		fields.add(name, type, sql, comment, key, set, link);
	}

	void parseConstraintList(const StringChunk & line)
	{
		auto words = line.split(";");
		if(words.length == 1){
			words[0].trim("\n\r \t");
			if(words[0] == "}"){
				state = &Declaration::parseDeclaration;
				return;
			}
		}

		StringChunk l(line);
		l.trim("\n\r \t");

		fields.setCapacity(fields.capacity+1);
		fields.add(l);
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
		unsigned i = 0;
		unint pos;
		bool declPhase = false;
		try{
			Object<Declaration> decl;
			for(auto & line : lines){
				i++;
				line.trimBack("\t ");
				if(line.findFirst(pos, "@BeginDecl@")) {
					decl.ptr = new Declaration();
					declPhase = true;
				} else if(line.findFirst(pos, "@EndDecl@")){
					declarations.add(decl);
					declPhase = false;
				} else if(declPhase)
					decl->parse(line);
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
		} else if(tpl.chopFront("@TableFieldList@")) {
			tpl.trimFront("\n\r");
			String fieldList;
			for(auto& field : declaration.fields){
				if(field.set || !field.name.length)
					continue;
				if(fieldList.length)
					fieldList << ", ";
				fieldList << field.name;
			}
			code << fieldList;
		} else if(tpl.chopFront("@KeyFieldList@")) {
			tpl.trimFront("\n\r");
			String fieldList;
			for(auto& field : declaration.fields){
				if(!field.key || !field.name.length)
					continue;
				if(fieldList.length)
					fieldList << ", ";
				fieldList << field.name;
			}
			code << fieldList;
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

		unsigned lastIdx = 0;
		unsigned endIdx = 0;
		unsigned i = 0;
		unsigned numOfFields = 0;
		while(i < fields.length){
			const auto& field = fields[i];
			if((what == "Field" && !field.name.length)
					|| (what == "KeyField" && !field.key)
					|| (what == "LinkField" && !field.link)
					|| (what == "NonKeyField" && field.key)
					|| (what == "TableField" && !field.name.length)
					|| (what == "TableField" && field.set)
					|| (what == "SetField" && !field.set)
					|| (what == "Constraint" && field.name.length)){
				i++;
				continue;
			}
			lastIdx = endIdx;
			endIdx = i;
			i++;
			numOfFields++;
			//LOG("Field name: %.*s", (int)field.name.length, field.name.str);
		}
		//LOG("what: %s, lastIdx: %u, endIdx: %u", what, lastIdx, endIdx);

		if(!numOfFields){
			unint pos;
			if(tpl.findFirst(pos, endTag.str, endTag.length))
				tpl.chopFront(pos + endTag.length);
			return;
		}

		i = 0;
		unsigned numOfTableFields = 0;
		while(i < fields.length){
			const auto& field = fields[i];
			i++;
			if(!field.name.length || field.set)
				continue;
			numOfTableFields++;
		}

		unint pos = 0;
		if(endIdx == 0){
			unint pos_e = 0;
			if(!tpl.findFirst(pos_e, endTag.str))
				throw ParseError("Missing ForEach End tag.");
			if(tpl.findFirst(pos, lastTag.str))
				if(pos < pos_e)
					tpl.chopFront(pos + lastTag.length);
		}

		tpl.trimFront("\n\r");
		code.trimBack("\t");

		StringChunk block(tpl.str, tpl.length);
		unint tplLineNoBegin = tplLineNo;
		unint tplLastLineStartPosBegin = tplLastLineStartPos;

		i = 0;
		unsigned idx = 0;
		while(i < fields.length && block.length){
			const auto& field = fields[i];

			if((what == "Field" && !field.name.length)
					|| (what == "KeyField" && !field.key)
					|| (what == "LinkField" && !field.link)
					|| (what == "NonKeyField" && field.key)
					|| (what == "TableField" && !field.name.length)
					|| (what == "TableField" && field.set)
					|| (what == "SetField" && !field.set)
					|| (what == "Constraint" && field.name.length)){
				i++;
				continue;
			}

			//LOG("-Field name: %.*s", (int)field.name.length, field.name.str);
			//LOG("-i: %d, lastIdx: %u, endIdx: %u", i, lastIdx, endIdx);

			// until a '@' character just append anything to the code
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
				if(i == lastIdx){
					tpl.chopFront(block.str - tpl.str);
					tpl.trimFront("\n\r");
					block = tpl;
					i++;
					idx++;
					//LOG("lastIdx");
				} else {
					block = tpl;
					tplLineNo = tplLineNoBegin;
					tplLastLineStartPos = tplLastLineStartPosBegin;
					i++;
					idx++;
					//LOG("last next");
				}
			} else if(block.chopFront(endTag.str)){
				if(i == endIdx){
					tpl.chopFront(block.str - tpl.str);
					tpl.trimFront("\n\r");
					block.clear();
					//LOG("endIdx");
				} else {
					block = tpl;
					tplLineNo = tplLineNoBegin;
					tplLastLineStartPos = tplLastLineStartPosBegin;
					i++;
					idx++;
					//LOG("end next");
				}
			} else if(block.chopFront("@NumOfFields@")){
				code << numOfFields;
			} else if(block.chopFront("@NumOfTableFields@")){
				code << numOfTableFields;
			} else if(block.chopFront("@FieldType@")) {
				code << field.type;
			} else if(block.chopFront("@FieldSetSubType@")) {
				code << field.setSubType;
			} else if(block.chopFront("@FieldIdx@")){
				code << idx;
			} else if(block.chopFront("@FieldName@")){
				code << field.name;
			} else if(block.chopFront("@FieldEnumName@")){
				code << field.enumName;
			} else if(block.chopFront("@FieldSqlDecl@")){
				code << "`" << field.name << "` " << field.sqlDecl;
			} else if(block.chopFront("@Constraint@")){
				code << field.constraint;
			} else if(block.chopFront("@FieldComment@")){
				code << field.comment;
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
			else if(tpl.chopFront("@ForEachTableFieldBegin@"))
				parseForEach("TableField", declaration.fields);
			else if(tpl.chopFront("@ForEachLinkFieldBegin@"))
				parseForEach("LinkField", declaration.fields);
			else if(tpl.chopFront("@ForEachSetFieldBegin@"))
				parseForEach("SetField", declaration.fields);
			else if(tpl.chopFront("@ForEachKeyFieldBegin@"))
				parseForEach("KeyField", declaration.fields);
			else if(tpl.chopFront("@ForEachNonKeyFieldBegin@"))
				parseForEach("NonKeyField", declaration.fields);
			else if(tpl.chopFront("@ForEachConstraintBegin@"))
				parseForEach("Constraint", declaration.fields);
			else if(tpl.chopFront("@IfSingleKeyBegin@")){
				unint pos;
				unsigned c = 0;
				for(auto& field : declaration.fields)
					if(field.key)
						c++;
				if(c != 1)
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
	InputCode(const StringChunk & tplDir, const OwnerContainer<Declaration> & declarations) :
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
		for(auto& file : files){
			String tplFileName(tplDir);
			tplFileName << file;
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

	InputCode inputCode(tplDir, declarations);
	File declFile(declFileName);
	String declBuf = declFile.readAll();
	declBuf.replace("\r", "");
	Declaration::parseAllDeclaration(declarations, declBuf);

	unsigned l;
	SorterOwnerContainer<String> lines;
	try{
		char * line = 0;
		ssize_t read = 0;
		size_t len = 0;
		while ((read = getline(&line, &len, stdin)) != -1) {
			l++;
			Object<String> lineStr(new String());
			lineStr->adopt(line, len);
			lineStr->trim("\n\r");
			lineStr->trimBack("\t ");
			lines.add(lineStr);
			StringChunk lineChunk(lines.last().str, lines.last().length);
			inputCode.parse(lineChunk);
		}
	} catch (Exception & e) {
		e.note("Excpetion happened while processing stdin line %u.", 3);
		EXCEPTION(e);
		return -1;
	}
	puts(inputCode.code.str);
}
