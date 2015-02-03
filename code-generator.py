#!/usr/bin/python

import sys
from optparse import OptionParser

"""C++ code generator

This program can generate C++ code portions by preprocessing specially commented C++ code.
"""

__author__ = "Peter Csaszar (csjpeter@gmail.com)"
__version__ = "$Revision: 1.0 $"
__date__ = "$Date: 2015/02/02 20:00:00 $"
__copyright__ = "Copyright (c) 2015 Peter Csaszar"
__license__ = "BSD"


_Type = ""
_KeyName = ""
_KeyType = ""
_Fields = []


def parseClassName(line) :
    global _Type
    global _KeyName
    global _TypeName
    global _Fields
    end = line.find("//")
    decl = line[0:end].strip()
    end = decl.find(" ")
    _Type = decl[end+1:]
    _Fields = []
    _KeyName = ""
    _KeyType = ""
    return

def parseField(line) :
    global _Fields
    end = line.find(";")
    decl = line[0:end].strip()
    end = decl.rfind(" ")
    k = decl[end+1:]
    v = decl[0:end]
    _Fields.append({'name': k, 'type': v})
    return k 

def readIncludeFile(line) :
    includeFileName = options.template_directory + command[8:] + ".cpp"
    try:
        f = open(includeFileName, 'r')
        template = f.read()
    except Exception :
        f.close()
        raise
    else:
        f.close()
    return template


def generateForField(tpl, fieldName, fieldType) :
    global _Type
    global _KeyType
    global _KeyName
    tplLen = len(tpl)
    pos = 0;
    while pos < tplLen :
        c = tpl[pos]
        if c != '@' :
            sys.stdout.write(c)
        elif tpl.startswith("@Type@", pos) :
            sys.stdout.write(_Type)
            pos += 5
        elif tpl.startswith("@KeyType@", pos) :
            sys.stdout.write(_KeyType)
            pos += 8
        elif tpl.startswith("@KeyName@", pos) :
            sys.stdout.write(_KeyName)
            pos += 8
        elif tpl.startswith("@FieldName@", pos) :
            sys.stdout.write(fieldName)
            pos += 10
        elif tpl.startswith("@FieldType@", pos) :
            sys.stdout.write(fieldType)
            pos += 10
        elif tpl.startswith("@FieldEnumName@", pos) :
            sys.stdout.write(fieldName[:1].upper())
            sys.stdout.write(fieldName[1:])
            pos += 14
        else :
            sys.stdout.write(c)
        pos += 1

usage = "usage: %prog [-t template-directory] | [-h]"
op = OptionParser(usage=usage)
op.add_option("-t", "--template-directory", type="string", default=None,
                        help="path to the directory, where template c++ codes can be found")
options, args = op.parse_args()

for line in sys.stdin :
    start = line.find("//#")
    sys.stdout.write(line)
    if start == -1 :
        continue
    start += 3
    end = line.find("//", start)
    command = line[start:end].strip()
    if command == "ClassName" :
        parseClassName(line)
    elif command == "Field" :
        parseField(line)
    elif command == "KeyField" :
        _KeyName = parseField(line)
        _KeyType = _Fields[len(_Fields)-1]['type']
    elif command.startswith('include ') :
        tpl = readIncludeFile(line)
        tplLen = len(tpl)
        pos = 0;
        while pos < tplLen :
            c = tpl[pos]
            if c != '@' :
                sys.stdout.write(c)
            elif tpl.startswith("@Type@", pos) :
                sys.stdout.write(_Type)
                pos += 5
            elif tpl.startswith("@KeyType@", pos) :
                sys.stdout.write(_KeyType)
                pos += 8
            elif tpl.startswith("@KeyName@", pos) :
                sys.stdout.write(_KeyName)
                pos += 8
            elif tpl.startswith("@KeyEnumName@", pos) :
                sys.stdout.write(_KeyName[:1].upper())
                sys.stdout.write(_KeyName[1:])
                pos += 12
            elif tpl.startswith("@ForEachFieldBegin@", pos) :
                endPos = tpl.find("@ForEachFieldEnd@", pos)
                pos += 20
                sectionLast = ""
                section = tpl[pos:endPos]
                midPos = section.find("@ForEachFieldLast@")
                if -1 < midPos :
                    sectionLast = section[midPos+19:]
                    section = section[:midPos]
                last = len(_Fields) - 1
                for i in range(0, len(_Fields)) :
                    n = _Fields[i]['name']
                    t = _Fields[i]['type']
                    if last == i and sectionLast != "":
                        generateForField(sectionLast, n, t)
                    else :
                        generateForField(section, n, t)
                pos = endPos + 17
            else :
                sys.stdout.write(c)
            pos += 1
    else :
        raise Exception("Unknow preprocessor command: "+command)

