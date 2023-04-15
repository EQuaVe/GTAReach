/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

%{
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <boost/algorithm/string/replace.hpp> //ani:-100


#include "tchecker/parsing/declaration.hh"
#include "tchecker/parsing/parsing.hh"
#include "tchecker/utils/log.hh"
#include "tchecker/dbm/db.hh" //ani:-100 

#include "system.tab.hh"


// Tell Flex the lexer's prototype ...
#define YY_DECL \
tchecker::parsing::system::parser_t::symbol_type spyylex \
(std::string const & filename, \
tchecker::parsing::system_declaration_t * & system_declaration)

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
#undef yywrap
#define yywrap() 1


// The location of the current token.
static tchecker::parsing::system::location loc;
	
void sp_reset_locations() {
	loc.initialize(nullptr, 1, 1);
}
	
using namespace tchecker::parsing;
%}

%option noyywrap nounput batch noinput
/* %option debug */

id        [[:alpha:]_][[:alnum:]_.]*
integer   [-+]?[0-9]+

%{
  // Code run each time a pattern is matched.
	// move token's length wide
  #define YY_USER_ACTION  loc.columns(static_cast<int>(spyyleng));
%}

%x COMMENT
%x ATTR_KEY
%x ATTR_VALUE
%x ECA_PROGRAM
%x ECA_PROG_GUARD
%x ECA_PROG_DO

%%

%{
  // Code run each time spyylex is called.
  loc.step();
%}


"#"            { BEGIN COMMENT; }
":"            { return system::parser_t::make_TOK_COLON(loc); }
"@"            { return system::parser_t::make_TOK_AT(loc); }
"?"            { return system::parser_t::make_TOK_QMARK(loc); }
"{"            { BEGIN ATTR_KEY;
	                return system::parser_t::make_TOK_LBRACE(loc); }
"}"            { return system::parser_t::make_TOK_RBRACE(loc); }
"{{"           { BEGIN ECA_PROGRAM;
                  return system::parser_t::make_TOK_PROGRAM_START(loc);}
"clock"        { return system::parser_t::make_TOK_CLOCK(loc); }
"edge"         { return system::parser_t::make_TOK_EDGE(loc); }
"event"        { return system::parser_t::make_TOK_EVENT(loc); }
"int"          { return system::parser_t::make_TOK_INT(loc); }
"location"     { return system::parser_t::make_TOK_LOCATION(loc); }
"process"      { return system::parser_t::make_TOK_PROCESS(loc); }
"sync"         { return system::parser_t::make_TOK_SYNC(loc); }
"system"       { return system::parser_t::make_TOK_SYSTEM(loc); }
"prophecy"     { return system::parser_t::make_TOK_DEC_PROPHECY_CLOCK(loc); /*prophecy clock declaration*/}
"history"      { return system::parser_t::make_TOK_DEC_HISTORY_CLOCK(loc); /*history clock declaration*/}
"timer"        { return system::parser_t::make_TOK_DEC_TIMER(loc); /*timer clock declaration*/}
"normal"       { return system::parser_t::make_TOK_DEC_NORMAL_CLOCK(loc); /*normal clock declaration*/}
{id}           { return system::parser_t::make_TOK_ID(spyytext, loc); }
{integer}      { return system::parser_t::make_TOK_INTEGER(spyytext, loc); }
[ \t]+         { loc.step(); }
[\n]+          { loc.lines(static_cast<int>(spyyleng)); loc.step();
	               return system::parser_t::make_TOK_EOL(loc); }
<<EOF>>        { return system::parser_t::make_TOK_EOF(loc); }



<COMMENT>{
[\n]+          { BEGIN INITIAL;
                 loc.lines(static_cast<int>(spyyleng)); loc.step();
	               return system::parser_t::make_TOK_EOL(loc); }
.*             ;
}



<ATTR_KEY>{
"}"            { BEGIN INITIAL;
                 return system::parser_t::make_TOK_RBRACE(loc); }
":"            { BEGIN ATTR_VALUE;
                 return system::parser_t::make_TOK_COLON(loc); }
{id}           { return system::parser_t::make_TOK_ID(spyytext, loc); }
[ \t]+         { loc.step(); }
[\n]+          { loc.lines(static_cast<int>(spyyleng)); loc.step(); }
}



<ATTR_VALUE>{
":"            { BEGIN ATTR_KEY;
                 return system::parser_t::make_TOK_COLON(loc); }
[ \t]*"}"      { BEGIN INITIAL;
                 return system::parser_t::make_TOK_RBRACE(loc); }
[\n]+           { loc.lines(static_cast<int>(spyyleng)); loc.step(); }
[^:{}#]*       {                
                 std::string s(spyytext);
                 
                 //ani:-100
                 boost::replace_all(s,"-INF",std::to_string(tchecker::dbm::MINUS_INF_VALUE));
                 boost::replace_all(s,"INF",std::to_string(tchecker::dbm::INF_VALUE));

                 size_t nb_nl = std::count(s.begin(), s.end(), '\n');
                 auto res = system::parser_t::make_TOK_TEXT(s, loc); //ani:-100
                 loc.lines(static_cast<int>(nb_nl));
                 return res;
                }
}




<ECA_PROGRAM>{
"do:"       {BEGIN ECA_PROG_DO;
              return system::parser_t::make_TOK_PROG_DO(loc); /*start do operation*/}
"provided:" {BEGIN ECA_PROG_GUARD;
              return system::parser_t::make_TOK_PROG_GUARD(loc); /*start guard operation*/}
"}}"        {BEGIN INITIAL; return system::parser_t::make_TOK_PROGRAM_STOP(loc); /*end program*/}
[[:space:]]|[[:space:]]+ { /*skip space operation(s)*/}
}

<ECA_PROG_DO>{
{id}        {return system::parser_t::make_TOK_ID(spyytext, loc);}
"["         {return system::parser_t::make_TOK_SQ_BRACK_OPEN(loc);}
"]"         {return system::parser_t::make_TOK_SQ_BRACK_CLOSE(loc);}
","         {return system::parser_t::make_TOK_COMMA(loc);}
"="         {return system::parser_t::make_TOK_EQUAL(loc); /*assign timer*/}
";"         {BEGIN ECA_PROGRAM; return system::parser_t::make_TOK_SEMICOLON(loc); /*end do program*/}
(-?[0-9][0-9]*)|(\+?-?"INF") {         
                std::string s(spyytext);
                /* std::cout << "ANI:167:" << s << std::endl; */
                //ani:-100
                boost::replace_all(s,"-INF",std::to_string(tchecker::dbm::MINUS_INF_VALUE)); //replace -INF with MINUS_INF_VALUE
                boost::replace_all(s,"INF",std::to_string(tchecker::dbm::INF_VALUE)); //replace INF with INF_VALUE

                size_t nb_nl = std::count(s.begin(), s.end(), '\n');
                auto res = system::parser_t::make_TOK_TEXT(s, loc); //ani:-100
                loc.lines(static_cast<int>(nb_nl));
                return res;
              }
"/"             {return system::parser_t::make_TOK_DIV(loc);}
"+"             {return system::parser_t::make_TOK_PLUS(loc);}
"*"             {return system::parser_t::make_TOK_MUL(loc);}
"%"             {return system::parser_t::make_TOK_MOD(loc);}
"-"             {return system::parser_t::make_TOK_MINUS(loc);}

[[:space:]]|[[:space:]]+ {}
}

<ECA_PROG_GUARD>{
"+"             {return system::parser_t::make_TOK_PLUS(loc);}
(\+?-?[0-9][0-9]*\+?-?\*?\%?\/?-?([0-9][0-9]*)?)|(\+?-?"INF") {         
                std::string s(spyytext);
                
                //ani:-100
                boost::replace_all(s,"-INF",std::to_string(tchecker::dbm::MINUS_INF_VALUE)); //replace -INF with MINUS_INF_VALUE
                boost::replace_all(s,"INF",std::to_string(tchecker::dbm::INF_VALUE)); //replace INF with INF_VALUE

                size_t nb_nl = std::count(s.begin(), s.end(), '\n');
                auto res = system::parser_t::make_TOK_TEXT(s, loc); //ani:-100
                loc.lines(static_cast<int>(nb_nl));
                return res;
              }
{id}          {return system::parser_t::make_TOK_ID(spyytext, loc);}
"<"             {return system::parser_t::make_TOK_LT(loc);}
"<="            {return system::parser_t::make_TOK_LE(loc);}
">"             {return system::parser_t::make_TOK_GT(loc);}
">="            {return system::parser_t::make_TOK_GE(loc);}
"=="            {return system::parser_t::make_TOK_EQ(loc);}
"&&"            {return system::parser_t::make_TOK_AND(loc);}
"-"             {return system::parser_t::make_TOK_MINUS(loc);}

"("             {return system::parser_t::make_TOK_PAR_OPEN(loc);}
")"             {return system::parser_t::make_TOK_PAR_CLOSE(loc);}

"!="            {return system::parser_t::make_TOK_NEQ(loc);}
"/"             {return system::parser_t::make_TOK_DIV(loc);}

"\*"             {std::cout << " ANI:STAR\n"; return system::parser_t::make_TOK_MUL(loc);}
"%"             {return system::parser_t::make_TOK_MOD(loc);}

[[:space:]]|[[:space:]]+ {}

";"         { BEGIN ECA_PROGRAM;
              return system::parser_t::make_TOK_SEMICOLON(loc);}
}



<*>.|\n        { std::stringstream msg;
                 msg << loc << " Invalid character: " << spyytext;
                 throw std::runtime_error(msg.str()); }




%%

namespace tchecker {

  namespace parsing {

    tchecker::parsing::system_declaration_t * parse_system_declaration(std::string const & filename)
    {
      if (filename.empty() || (filename == "-"))
        return tchecker::parsing::parse_system_declaration(stdin, "");
      
      std::FILE * f = std::fopen(filename.c_str(), "r");
      if (f == nullptr)
        throw std::runtime_error("cannot open " + filename + ": " + strerror(errno));
        
      tchecker::parsing::system_declaration_t * sysdecl = nullptr;
      try {
        sysdecl = tchecker::parsing::parse_system_declaration(f, filename);
        std::fclose(f);
      }
      catch (...) {
        std::fclose(f);
        throw;
      }
      return sysdecl;
    }
    
    
    tchecker::parsing::system_declaration_t *
    parse_system_declaration(std::FILE * f, std::string const & filename)
		{
      std::size_t old_error_count = tchecker::log_error_count();
      
      spyyrestart(f);

      // Initialise
      sp_reset_locations();
      BEGIN INITIAL;
      
      // Parse
      tchecker::parsing::system_declaration_t * sysdecl = nullptr;
      
      try {
        tchecker::parsing::system::parser_t parser(filename, sysdecl);
				parser.parse();
        spyy_flush_buffer(YY_CURRENT_BUFFER);
      }
      catch (...) {
        delete sysdecl;
        spyy_flush_buffer(YY_CURRENT_BUFFER);
        throw;
      }
      
      if (tchecker::log_error_count() > old_error_count) {
        delete sysdecl;
        sysdecl = nullptr;
      }
      
      return sysdecl;
    }
    
  }
  
}