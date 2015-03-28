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

String & lower(String & str)
{
	unsigned s = str.length;
	for(unsigned i = 0; i < s; i++){
		char & c = str[i];
		if('A' <= c && c <= 'Z')
			c += 'a'-'A';
	}
	return str;
}

class FieldDesc
{
public:
	FieldDesc(const StringChunk & constraint) :
		constraint(constraint.str, constraint.length),
		key(false),
		set(false),
		link(false),
		derived(false),
		spec(false)
	{
	}

	FieldDesc(const StringChunk & name, const StringChunk & type,
			const StringChunk & sqlDecl, const StringChunk & comment,
			bool key, bool set, bool link, bool derived, bool spec) :
		name(name.str, name.length),
		type(type.str, type.length),
		enumName(name.str, name.length),
		sqlDecl(sqlDecl.str, sqlDecl.length),
		comment(comment.str, comment.length),
		key(key),
		set(set),
		link(link),
		derived(derived),
		spec(spec)
	{
		if('a' <= enumName[0] && enumName[0] <= 'z')
			enumName[0] += 'A'-'a';
		nameLower <<= name;
		lower(nameLower);
		if(set){
			setSubType <<= type;
			setSubType.chopBack(3);
		}
	}

	String name;
	String nameLower;
	String type;
	String setSubType;
	String enumName;
	String sqlDecl;
	String comment;
	String constraint;
	bool key;
	bool set;
	bool link;
	bool derived;
	bool spec;
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
	String typeNameLower;
	Array<FieldDesc> fields;

	Declaration() { clear(); }

	void clear()
	{
		state = &Declaration::parseDeclaration;
		typeName.clear();
		typeNameLower.clear();
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
		bool derived = false;
		bool spec = false; // in gui represent it as string
		if(1 < words.length){
			auto modifiers = words[1].split(",");
			for(auto& mod : modifiers) mod.trim(" \t");
			key = modifiers.has("key");
			set = modifiers.has("set");
			link = modifiers.has("link");
			derived = modifiers.has("derived");
			spec = modifiers.has("spec");
		}

		StringChunk sql;
		if(2 < words.length)
			sql = words[2];

		fields.setCapacity(fields.capacity+1);
		fields.add(name, type, sql, comment, key, set, link, derived, spec);
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
			typeNameLower <<= words[1];
			lower(typeNameLower);
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
		auto lines = split(data, "\n", false);
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
			e.note("Excpetion happened while processing declaration file line %.", i);
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

inline bool operator<(const StringChunk & a, const Declaration & b)
{
	return a < b.typeName;
}

inline bool operator<(const Declaration & a, const StringChunk & b)
{
	return a.typeName < b;
}

class TemplateParser
{
	String & code;
	StringChunk tpl;
	const OwnerContainer<Declaration> & declarations;
	const StringChunk & tplDir;
	unint declIdx;
	unint tplLineNo;
	unint tplLastLineStartPos;
	const char * const until;
public:
	TemplateParser(String & code, const StringChunk & tpl,
			const OwnerContainer<Declaration> & declarations,
			const StringChunk & tplDir, unint declIdx = 0) :
		code(code), tpl(tpl.str, tpl.length), declarations(declarations), tplDir(tplDir),
		declIdx(declIdx), tplLineNo(1), tplLastLineStartPos(0), until(tpl.str + tpl.length)
	{
	}

	void parseCommonMarker(StringChunk & tpl)
	{
		if(tpl.chopFront("@Type@")) {
			tpl.trimFront("\n\r");
			//code.trimBack("\t");
			code << declarations[declIdx].typeName;
		} else if(tpl.chopFront("@type@")) {
			tpl.trimFront("\n\r");
			//code.trimBack("\t");
			code << declarations[declIdx].typeNameLower;
		} else if(tpl.chopFront("@TableFieldList@")) {
			tpl.trimFront("\n\r");
			String fieldList;
			for(auto& field : declarations[declIdx].fields){
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
			for(auto& field : declarations[declIdx].fields){
				if(!field.key || !field.name.length)
					continue;
				if(fieldList.length)
					fieldList << ", ";
				fieldList << field.name;
			}
			code << fieldList;
		} else if(tpl.length) {
			if(tpl[0] == '@')
				LOG("Fail");
			code << tpl[0];
			tpl.chopFront(1);
		}
	}

	bool parseForEachField(StringChunk & tpl, bool skipMode = false)
	{
		const Array<FieldDesc> & fields = declarations[declIdx].fields;
		const char * what = 0;

		if(tpl.chopFront("@For{Field@"))
			what = "Field";
		else if(tpl.chopFront("@For{DerivedField@"))
			what = "DerivedField";
		else if(tpl.chopFront("@For{TableField@"))
			what = "TableField";
		else if(tpl.chopFront("@For{LinkField@"))
			what = "LinkField";
		else if(tpl.chopFront("@For{SetField@"))
			what = "SetField";
		else if(tpl.chopFront("@For{KeyField@"))
			what = "KeyField";
		else if(tpl.chopFront("@For{NonKeyField@"))
			what = "NonKeyField";
		else if(tpl.chopFront("@For{Constraint@"))
			what = "Constraint";
		else
			return false;
		tpl.trimFront("\n\r");
		//LOG("what: %", what);

		StringChunk lastTag("@-@");
		StringChunk endTag("@}@");

		unsigned lastIdx = 0;
		unsigned endIdx = 0;
		unsigned i = 0;
		unsigned numOfFields = 0;
		while(i < fields.length){
			const auto& field = fields[i];
			if((what == "Field" && !field.name.length)
					|| (what == "Field" && field.derived)
					|| (what == "DerivedField" && !field.derived)
					|| (what == "KeyField" && !field.key)
					|| (what == "LinkField" && !field.link)
					|| (what == "NonKeyField" && field.key)
					|| (what == "TableField" && !field.name.length)
					|| (what == "TableField" && field.set)
					|| (what == "TableField" && field.derived)
					|| (what == "SetField" && !field.set)
					|| (what == "Constraint" && field.name.length)){
				i++;
				continue;
			}
			lastIdx = endIdx;
			endIdx = i;
			i++;
			numOfFields++;
			//LOG("Field name: %", field.name);
		}
		//LOG("what: s, lastIdx: %, endIdx: %", what, lastIdx, endIdx);

		// no fields to iterate on or in skip mode
		if(!numOfFields || skipMode){
			unint posBegin;
			unint posEnd;
			while(true){
				posBegin = tpl.length + 2;
				posEnd = tpl.length + 1;
				tpl.findFirst(posBegin, "@For{");
				tpl.findFirst(posEnd, endTag);
				if(posBegin < posEnd){
					tpl.chopFront(posBegin);
					//LOG("into it 0");
					parseForEachField(tpl, true);
					//LOG("out from it 0");
					continue;
				}
				if(posEnd != tpl.length + 1)
					tpl.chopFront(posEnd + endTag.length);
				break;
			}
			return true;
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

		// only one field to iterate on
		if(endIdx == 0){
			unint posBegin;
			unint posLast;
			unint posEnd;
			while(true) {
				posBegin = tpl.length + 2;
				posLast = tpl.length + 1;
				posEnd = tpl.length;
				tpl.findFirst(posBegin, "@For{");
				tpl.findFirst(posLast, lastTag);
				tpl.findFirst(posEnd, endTag);
				if(posBegin < posLast && posBegin < posEnd){
					tpl.chopFront(posBegin);
					//LOG("into it 1");
					parseForEachField(tpl);
					//LOG("out from it 1");
					continue;
				}
				if(posLast < posEnd)
					tpl.chopFront(posLast + lastTag.length);
				break;
			}
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
					|| (what == "Field" && field.derived)
					|| (what == "DerivedField" && !field.derived)
					|| (what == "KeyField" && !field.key)
					|| (what == "LinkField" && !field.link)
					|| (what == "NonKeyField" && field.key)
					|| (what == "TableField" && !field.name.length)
					|| (what == "TableField" && field.set)
					|| (what == "TableField" && field.derived)
					|| (what == "SetField" && !field.set)
					|| (what == "Constraint" && field.name.length)){
				i++;
				continue;
			}

			//LOG("-Field name: %", field.name);
			//LOG("-i: %, lastIdx: %, endIdx: %", i, lastIdx, endIdx);

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

			if(block.startsWith("@For{")){
				//LOG("into it ...");
				if(parseForEachField(block)){
					//LOG("out from it ...");
					continue;
				}
			} else if(block.chopFront(lastTag)){
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
			} else if(block.chopFront(endTag)){
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
			} else if(block.chopFront("@IfIsSpec{@")){
				unint pos;
				if(!field.spec)
					if(block.findFirst(pos, "@IfIsSpec}@"))
						block.chopFront(pos);
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfIsSpec}@")){
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfNotSpec{@")){
				unint pos;
				if(field.spec)
					if(block.findFirst(pos, "@IfNotSpec}@"))
						block.chopFront(pos);
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfNotSpec}@")){
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfIsSet{@")){
				unint pos;
				if(!field.set)
					if(block.findFirst(pos, "@IfIsSet}@"))
						block.chopFront(pos);
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfIsSet}@")){
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfNotSet{@")){
				unint pos;
				if(field.set)
					if(block.findFirst(pos, "@IfNotSet}@"))
						block.chopFront(pos);
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@IfNotSet}@")){
				block.trimFront("\n\r");
				block.trimFront(" \t");
			} else if(block.chopFront("@NumOfFields@")){
				code << numOfFields;
			} else if(block.chopFront("@NumOfTableFields@")){
				code << numOfTableFields;
			} else if(block.chopFront("@.Type@")) {
				code << field.type;
			} else if(block.chopFront("@.SetSubType@")) {
				code << field.setSubType;
			} else if(block.chopFront("@.Idx@")){
				code << idx;
			} else if(block.chopFront("@.Name@")){
				code << field.name;
			} else if(block.chopFront("@.name@")){
				code << field.nameLower;
			} else if(block.chopFront("@.EnumName@")){
				code << field.enumName;
			} else if(block.chopFront("@.SqlDecl@")){
				code << field.sqlDecl;
			} else if(block.chopFront("@Constraint@")){
				code << field.constraint;
			} else if(block.chopFront("@.Comment@")){
				code << field.comment;
			} else {
				parseCommonMarker(block);
			}
		}
		return true;
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

			if(parseForEachField(tpl)) {
				;
			} else if(tpl.chopFront("@ForTypes{")){
				unint pos;
				if(!tpl.findFirst(pos, "@"))
					pos = tpl.length;
				StringChunk typeList(tpl.str, pos);
				tpl.chopFront(pos+1);
				auto types = typeList.split(" \t,\n\r");
				unint origDeclIdx = declIdx;
				StringChunk origTpl(tpl);
				if(!tpl.findFirst(pos, "@}ForTypes@"))
					throw ParseError("Missing @}ForTypes@");
				tpl.cutAt(pos); 
				for(auto & type : types){
					type.trim(" \t\r\n");
					if(!declarations.has(type))
						throw ParseError("Unknown class %", type);
					declIdx = declarations.index(type);
					TemplateParser parser(code, tpl,
							declarations, tplDir, declIdx);
					parser.parse();
				}
				declIdx = origDeclIdx;
				tpl = origTpl;
				tpl.findFirst(pos, "@}ForTypes@");
				tpl.chopFront(pos);
				tpl.chopFront("@}ForTypes@");
			} else if(tpl.chopFront("@IfSingleKey{@")){
				unint pos;
				unsigned c = 0;
				for(auto& field : declarations[declIdx].fields)
					if(field.key)
						c++;
				if(c != 1)
					if(tpl.findFirst(pos, "@IfSingleKey}@"))
						tpl.chopFront(pos);
				tpl.trimFront("\n\r");
			} else if(tpl.chopFront("@IfSingleKey}@")){
				tpl.trimFront("\n\r");
			} else if(tpl.chopFront("@IfHasLinkField{@")){
				unint pos;
				unsigned c = 0;
				for(auto& field : declarations[declIdx].fields)
					if(field.link)
						c++;
				if(c == 0)
					if(tpl.findFirst(pos, "@IfHasLinkField}@"))
						tpl.chopFront(pos);
				tpl.trimFront("\n\r");
			} else if(tpl.chopFront("@IfHasLinkField}@")){
				tpl.trimFront("\n\r");
			} else if(tpl.chopFront("@declare@")){
				unint pos;
				if(!tpl.findFirst(pos, "\n"))
					pos = tpl.length;
				StringChunk declaredClass(tpl.str, pos);
				tpl.chopFront(pos);
				declaredClass.trim(" \t\n\r");
				if(!declarations.has(declaredClass))
					throw ParseError("Unknown class %", declaredClass);
				declIdx = declarations.index(declaredClass);
			} else if(tpl.chopFront("@include@")){
				unint pos;
				if(!tpl.findFirst(pos, "\n"))
					pos = tpl.length;
				StringChunk includeList(tpl.str, pos);
				tpl.chopFront(pos);
				auto files = includeList.split(" ");
				for(auto& file : files){
					String tplFileName(tplDir.str, tplDir.length);
					tplFileName << file;
					File tplFile(tplFileName);
					String newTpl = tplFile.readAll();
					StringChunk newTplChunk(newTpl.str, newTpl.length);
					TemplateParser parser(code, newTplChunk,
							declarations, tplDir, declIdx);
					parser.parse();
				}
			} else
				parseCommonMarker(tpl);
		}
	}
};



int main(int argc, char *args[])
{
	set_segmentation_fault_signal_handler();
	StringChunk tplDir;
	StringChunk declFileName;
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
				!strcmp(args[argi], "--declarations") ||
				!strcmp(args[argi], "-d"))){
			declFileName = args[argi+1];
			argi += 2;
			continue;
		}
		if(1 <= argc - argi && (
				!strcmp(args[argi], "--input") ||
				!strcmp(args[argi], "-i"))){
			inputFileName = args[argi+1];
			argi += 2;
			continue;
		}

		fprintf(stderr, "Bad argument given: '%s'\n", args[argi]);
		LOG("Bad argument given: '%'", args[argi]);
		return 1;
	}

	OwnerContainer<Declaration> declarations;
	String code;

	File declFile(declFileName);
	String declBuf = declFile.readAll();
	declBuf.replace("\r", "");
	Declaration::parseAllDeclaration(declarations, declBuf);

	LOG("Declared classes:");
	for(auto & i : declarations){
		LOG(" - % : %", i.typeName, declarations.has(i.typeName) ? "true" : "false");
	}
	
	File inputFile(inputFileName);
	String inputBuf = inputFile.readAll();
	inputBuf.replace("\r", "");
	StringChunk inputChunk(inputBuf.str, inputBuf.length);

	TemplateParser parser(code, inputChunk, declarations, tplDir);
	try {
		parser.parse();
	} catch (Exception & e) {
		EXCEPTION(e);
		return -1;
	}
	puts(code.str);
}
