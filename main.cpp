#include "global.h"
#include "node.hpp"
#include "io.hpp"
#include "token.hpp"
#include "cmd_line.h"
#include "graph.h"
#include "tokenize.hpp"
/*
 * This is a Transcompiler intended for small ~500 max python files.
 * I will probably work on larger projects, but now I'm just writing
 * this to transform my small time python scripts that I hevely use
 * into a faster compiled file.
 */
int main(int argc, char *argv[])
{
        //avoid warning
        argc+=0;argv+=0;
        char filename[] = "test.py";
        turtle::Document Document;
        puts("Ready, Set, Go!");
        //set explicit scope to deallocate file data and save memory
        {
                std::string file;
                readfile(filename, file);

                DEBUG_M(puts("Tokenizing");)
                turtle::tokenize(file, Document.Lexemes);
                file.clear();

                DEBUG_M(puts("Finished tokenizing, now lexing");)
                turtle::lex(Document);
                Document.Lexemes.clear();

                construct_graph(Document);
                //DEBUG_M(printLexTokens(Document));
        }
        /*printf("%d\n",
               (int)turtle::token::flag::Keyword::_size()+
               (int)turtle::token::flag::Data::_size()+
               (int)turtle::token::flag::Arithmetic::_size()+
               turtle::token::flag::Operator::_size()+
               turtle::token::flag::Control::_size()
               );*/
        //Keep python logic and CPP separate in order to modulerize code to differing langs

        //pythonToJson(str){

        //  lines = logicalLines(physicalLines)
        //  lines = foreach(line){ tokenizeLine(line) }
        //  nodes = SyntaxGroups(lines)
        //  python_Patterns = semanticAnalysis(groups)
        //
        //  jsonRepresentation = toJson(python_Patterns)
        //  ~Transform to Json~
        //  saveAndWriteToFile(jsonRepresentation)
        //
        //}

        //jsonToCPP(str){
        //  str = getfile()
        //  group
        //  str = stringifyCPPLogic(map<> patterns)
        //  writeCPPfile(str)
        //}
        return 0;
}
