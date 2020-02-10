#include <iostream>
#include <algorithm>
#include <regex>
#include <bitset>
#include "turtle.h"
//#define NDEBUG
#include "json.hpp"

//using pointers like a jumptable
//
//typedef std::vector<std::pair<std::tuple<int, int, std::any>, std::vector<std::shared_ptr<void>>>> node;
//
//┌─  typedef std::pair< std::tuple<int, int, std::any> , NodeRoot> DocNode; <──┐
//│   typedef std::vector<DocNode> DocRoot;                                     │
//└─>  typedef std::vector<std::shared_ptr<void>> NodeRoot;                   ──┘
//
//std::vector<
//             std::pair<
//                  .first
//                      std::tuple<
//                         [0] int NodeId
//                         [1] int NodeType
//                         [2] std::any NodeData
//                      >
//                  .second
//                      std::vector<
//                         std::shared_ptr<void> of Parent
//                         std::shared_ptr<void> of children
//              >
//>
//      |Parent | Child |
//Parent|   0   |   1   |
//      | ──────────────|
//Child |   1   |   0   |

/*struct DocNode : public std::vector<int>
{
   //NodeTypes
   //0) UNKOWN
   //1) Document Root
   //2) Scope group
   //3) Parent
   //4) Child
   unsigned int NodeType = 0;
   std::any tokenData;
   unsigned int TokenID = 0;
   unsigned int scope = 0;
   //this->

   //Initalise root of document
   //void make_children bow chica wow wow
   DocNode(const std::string &tmpData, const unsigned int &tmpType, const unsigned int &tmpIndex, const unsigned int &tmpScope)
   {
      tokenData = std::move(tmpData);
      NodeType = std::move(tmpType);
      TokenID = std::move(tmpIndex);
      scope = std::move(tmpScope);
   }
   void make_child(unsigned int nodeID)
   {
      push_back(nodeID);
   }
};*/
bool is_Newline(const std::string &tmp) { return tmp[0] == '\n'; }
/*struct DocRoot : std::vector<DocNode>
{
   unsigned int numberOfTokens = 0;
   unsigned int scopeIndex = 0;
   unsigned int lineIndex = 0;
   void create_child(const std::string &tokenData, const unsigned int NodeType)
   {
      if (is_Newline(tokenData))
      {
         ++lineIndex;
         scopeIndex += (tokenData.length() - 1) - scopeIndex;
         //scopeIndex;
      }
      push_back(std::move(DocNode(tokenData, NodeType, numberOfTokens, scopeIndex)));
      ++numberOfTokens;
   }
};*/

typedef std::pair<std::string, std::bitset<32>> tokenData;

int main(int argc, char *argv[])
{
   //only accept one argument
   switch (argc)
   {
   case 1:
   {
      help();
      return EXIT_SUCCESS;
      break;
   }
   case 2:
   {
      if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
      {
         help();
         return EXIT_SUCCESS;
      }
      break;
   }
   default:
   {
      std::cerr << "To many args\n";
      return EXIT_FAILURE;
      break;
   }
   }

   const char *filename = argv[1];

   //incase I ever want to do more than one file at once in the future
   std::vector<const char *> files;
   files.push_back(filename);
   //std::for each with the return as a break statment
   nlohmann::json DocumentJSON;
   DocumentJSON["DocumentData"] = R"({"lang": "C++"})"_json;
   DocumentJSON["tokens"] = nlohmann::json::array();
   (void)std::any_of(files.begin(), files.end(), [&](auto &file) {
      {
         std::string filedata = "";

         if (getFileDataReadOnly(file, filedata))
         {
            std::cerr << "\033[1;31mError:\033[0m Could not open file '" << file << "'\n";
            return EXIT_FAILURE;
         }

         if (filedata.empty())
         {
            std::cerr << "\033[1;31mError:\033[0m file " << file << "is empty\n";
            return EXIT_FAILURE;
         }
         std::regex TokenRegex(R"(("""(?:[^\\"]|\\.)*"""|('(?:[^\\']|\\.)*'|"(?:[^\\"]|\\.)*")|(#[^\n]*)|(\n[ \t]*)|^|(([<>*\/]{2}=?)|([=<>!+\-*\\&|]=))|([!"\#%&'()*+,\-.\/:;<=>?@\[\\\]^‘{|}~]|[^\t\f\v\s!"\#%&'()*+,\-.\/:;<=>?@\[\\\]^‘{|}~]+)))");
         std::regex_iterator<std::string::iterator> rend, rit(filedata.begin(), filedata.end(), TokenRegex);
         ++rit; // The first token is always "" skip over it
         // .reserve(std::distance(rit, rend));
         for (unsigned int lineIndex = 0, tokenIndex = 0, scope = 0, oldScope = 0, scopeIndex = 0;
              rit != rend;
              ++rit)
         {
            const std::string tok = rit->str();
            if (tok[0] == '\n' || tok[0] == '\r')
            {
               ++lineIndex;
               oldScope = std::move(scope);
               scope = tok.size() - 1; // the number of spaces minis the '\n' character
               if (scope >= oldScope)
               {
                  ++scopeIndex;
               }
               else
               {
                  scopeIndex = 0;
               }
            }
            else
            {
               ++tokenIndex;
            }
            DocumentJSON["tokens"].push_back(std::move(nlohmann::json::array({lineIndex, scope, scopeIndex, tokenIndex, std::move(tok)})));
         }
      }
      std::cout << DocumentJSON["tokens"].dump();
      return EXIT_SUCCESS;
   });
   puts("\nDone");
   return EXIT_SUCCESS;
}
