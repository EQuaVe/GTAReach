/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

%skeleton "lalr1.cc" // c++
%require "3.0.4"

%defines
%define parser_class_name {parser_t}
%define api.namespace {tchecker::parsing::system}
%define api.prefix {spyy}
%define api.token.constructor
%define api.value.type variant
%define parse.assert true
%define parse.trace true
%define parse.error verbose


%code requires {
  #include <cstdlib>
  #include <iostream>
  #include <sstream>
  #include <limits>
  #include <string>
  #include <vector>
  
  #include <boost/algorithm/string.hpp>
  
  #include "tchecker/basictypes.hh"
  #include "tchecker/parsing/declaration.hh"
  #include "tchecker/utils/log.hh"
  #include "tchecker/dbm/db.hh" //ani:-100 

}



%param { std::string const & filename }
%param { tchecker::parsing::system_declaration_t * & system_declaration }


%locations


%code {
  // Declare the lexer for the parser's sake.
  tchecker::parsing::system::parser_t::symbol_type spyylex
  (std::string const & filename,
  tchecker::parsing::system_declaration_t * & system_declaration);
  
  // Error detection
  static unsigned int old_error_count;

  //ani:-100
  std::vector<std::string> prophecy_clocks_parser(0); //maintain the set of all prophecy clocks defined (superset of timers and event prophecy clocks)
  std::vector<std::string> history_clocks_parser(0); //maintain the set of all history clocks defined (superset of normal and event history clocks)
  std::vector<std::string> timer_clocks_parser(0); //maintain the set of all timers defined (subset of prophecy clocks)
  std::vector<std::string> event_history_clocks(0); //maintain the set of all event history clocks (subset of history clocks)
  std::vector<std::string> event_prophecy_clocks(0); //maintain the set of all prophecy clocks (subset of prophecy clocks)
  
  
  std::vector<std::string> good_prophecy_clocks(0); //maintain a set of all prophecy clocks which are always checked for prophecy <=-inf or ==-inf or >=0 or ==0 before release operation
  
  std::vector<std::string> prophecy_clocks_verify(0); //maintain the set of prophecy clocks which appear in a prophecy-prophecy guard

  bool continue_successor_comp = false; //variable stores whether finiteness is not guaranteed




  std::vector<std::string> tmp_single_prop_guards(0); //this is a temproary variable
  //this will only store for single guards, prophecy clocks which are checked for <=-inf or >=0;
  //for a guard program appearing in an edge it is like a filter which only has prophecy clocks which are checked for <=-inf or ==-inf or >=0 or ==0

  std::vector<std::string> tmp_prop_guards(0); //this is a temporary variable 
  //this will contain clks tmp, clks, tmp, clks
  //where clks are checked either for <=-inf or for >=0 in their corresponding guard numbers
  //both tmp_single_prop_guards and tmp_prop_guards are equal

  std::vector<std::string> parser_integer_vars(0); //this list will maintain the integer variables defined in input file
}

%initial-action {
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &const_cast<std::string &>(filename);
  
  old_error_count = tchecker::log_error_count();
  
  

};



%token                TOK_CLOCK          "clock"
%token                TOK_COLON          ":"
%token                TOK_AT             "@"
%token                TOK_QMARK          "?"
%token                TOK_LBRACE         "{"
%token                TOK_RBRACE         "}"
%token                TOK_EDGE           "edge"
%token                TOK_EOF          0 "end of file" // 0 means EOF
%token                TOK_EOL            "\n"
%token                TOK_EVENT          "event"
%token <std::string>  TOK_ID             "identifier"
%token                TOK_INT            "int"
%token <std::string>  TOK_INTEGER        "integer value"
%token                TOK_LOCATION       "location"
%token                TOK_PROCESS        "process"
%token                TOK_SYNC           "sync"
%token                TOK_SYSTEM         "system"
%token <std::string>  TOK_TEXT           "text value"
%token <std::string>  TOK_HIST_CLK       "history clock"
%token <std::string>  TOK_PROP_CLK       "prophecy clock"
%token                TOK_DEC_HISTORY_CLOCK   "history"
%token                TOK_DEC_PROPHECY_CLOCK   "prophecy"
%token                TOK_DEC_NORMAL_CLOCK   "normal"
%token                TOK_DEC_TIMER       "timer"
%token                TOK_LE             "<="
%token                TOK_LT             "<"
%token                TOK_GE             ">="
%token                TOK_GT             ">"
%token                TOK_AND            "&&"
%token                TOK_MINUS           "-"
%token                TOK_COMMA           ","
%token                TOK_PROGRAM_START   "{{"
%token                TOK_PROGRAM_STOP    "}}"
%token                TOK_PROG_DO         "do"
%token                TOK_PROG_GUARD      "provided:"
%token                TOK_SEMICOLON       ";"
%token                TOK_EQUAL           "="
%token                TOK_EQ              "=="
%token                TOK_SQ_BRACK_CLOSE      "]"
%token                TOK_SQ_BRACK_OPEN       "["

%token                TOK_PAR_OPEN        "("
%token                TOK_PAR_CLOSE       ")"
%token                TOK_NEQ             "!="
%token                TOK_DIV             "/"
%token                TOK_PLUS            "+"
%token                TOK_MUL             "*"
%token                TOK_MOD             "%"



%type <unsigned int>                                                uinteger
%type <tchecker::integer_t>										                      integer
%type <std::vector<tchecker::parsing::sync_constraint_t const *>>   sync_constraint_list
%type <tchecker::parsing::sync_constraint_t *>                      sync_constraint
%type <tchecker::parsing::attr_t *>                                 attr
%type <tchecker::parsing::attributes_t>                             attr_list
                                                                    non_empty_attr_list
%type <std::string>                                                 text_or_empty
%type <enum tchecker::sync_strength_t>                              sync_strength
%type <std::vector<std::string>>                                    eca_guards2 //is a list of all guards converted to OR operation
%type <std::vector<std::string>>                                    non_empty_eca_guards2
%type <std::vector<std::string>>                                    eca_release_reset
%type <std::vector<std::string>>                                    eca_single_guard2
%type <std::vector<std::string>>                                    non_empty_reset_release
%type <std::vector<std::vector<std::string>>>                       program_guard2 //can be though of as a tuple
                                                                                    //where program_guards2[0] is a vector and stores all possible <= guards in OR format
                                                                                    //program_guards2[1] stores the do operations
%type <std::vector<std::vector<std::string>>>                       program_do2   //can be though of as a tuple
                                                                                    //where program_do2[0] is a vector and stores all possible <= guards in OR format
                                                                                    //program_do2[1] stores the do operations
%type <std::string>                                                 predicate_expr; //placeholder for constraints concerning int variables
%type <std::string>                                                 int_term;

%printer { yyoutput << $$; }                                        <*>;
%printer { yyoutput << * $$; }                                      attr
                                                                    sync_constraint;
%printer {
  for (auto it = $$.begin(); it != $$.end(); ++it) {
    if (it != $$.begin())
      yyoutput << ",";
    yyoutput << **it;
  }
}                                                                  sync_constraint_list;

%printer {
  for (auto it = $$.begin(); it != $$.end(); ++it) {
    for (auto it2 = (*it).begin(); it2!=(*it).end();++it2)
    { 
      if (it2 != (*it).begin())
        yyoutput << " V ";
      else
        yyoutput << "(";
      yyoutput << *it2;
    }
    yyoutput << ") N ";
  }
}
                                                                  eca_guards2
                                                                  eca_release_reset
                                                                  non_empty_eca_guards2
                                                                  non_empty_reset_release
                                                                  ;
                                                                  

%printer {
  for (auto it = $$.begin(); it != $$.end(); ++it) {
    if (it != $$.begin())
      yyoutput << " V ";
    else
      yyoutput << "(";
    yyoutput << *it;
  }
    yyoutput << ")";
}                                                               
                                                                eca_single_guard2;

%printer {
  for (auto it = $$.begin(); it != $$.end(); ++it) {
    for(auto it2:*it){
      yyoutput << it2 << " ";
    }
    yyoutput << std::endl;
  }
    yyoutput << "\n";
}                                                             program_guard2
                                                              program_do2;

%start system

%%

system:
eol_sequence TOK_SYSTEM ":" TOK_ID attr_list {
  system_declaration = new system_declaration_t($4, std::move($5));
}
"\n" eol_sequence declaration_list
{
  if (tchecker::log_error_count() > old_error_count) {
    delete system_declaration;
    system_declaration = nullptr;
  }
}
| error TOK_EOF
{
  delete system_declaration;
  system_declaration = nullptr;
}
;


declaration_list:
non_empty_declaration_list eol_sequence
{}
|
{}
;


non_empty_declaration_list:
declaration
{}
| non_empty_declaration_list eol_sequence declaration
{}
;


declaration:
TOK_CLOCK ":" uinteger ":" TOK_ID attr_list "\n"
{ 
  auto const * d = system_declaration->get_clock_declaration($5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of clock " << $5 << std::endl;
  else {
    auto const * intd = system_declaration->get_int_declaration($5);
    if (intd != nullptr)
      std::cerr << tchecker::log_error << @5 << " variable " << $5 << " already declared as an int" << std::endl;
    else {
      try {
        //ani:-100
        std::string event_name = "";
        d = new tchecker::parsing::clock_declaration_t($5, $3, std::move($6),0,event_name);
        if ( ! system_declaration->insert_clock_declaration(d) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of clock declaration failed" << std::endl;
          delete d;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
  }
}

| TOK_CLOCK ":" TOK_DEC_PROPHECY_CLOCK ":" TOK_ID attr_list"\n"
{ /* A "tmp" clock as prophecy is used as a flag.
     A program is of the form provided: g1; do: r1; provided: g2 ..., where g1, g2, ... are guards
     and r1, r2, ... are resets/releases.
     It is stored as guards: g1 && tmp<=0 && g2 && tmp<=0 ...
     and do operations r1, tmp, r2, tmp,...
     The "tmp" clock acts as a flag while executing the program and allows 
     to switch between guards and release/reset operations alternately.
  */
  //is the flag clock defined? if not define it!
  bool is_tmp_clk_defined = ((std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), "tmp")!=prophecy_clocks_parser.end()) && (prophecy_clocks_parser.size()!=0));
  if(!is_tmp_clk_defined){
    //declare tmp clock
    auto const * dtmp = system_declaration->get_clock_declaration("tmp");
    if (dtmp != nullptr)
      std::cerr << tchecker::log_error << @5 << " multiple declarations of clock tmp"  << std::endl;
    else {
      try {
        //ani:-100
        tchecker::parsing::attributes_t* attr_list_tmp = new tchecker::parsing::attributes_t();
        attr_list_tmp->clear();
        
        std::string event_name = "";
        const unsigned int _clock_size = 1;
        dtmp = new tchecker::parsing::clock_declaration_t(std::string("tmp"), _clock_size, std::move(*attr_list_tmp), 2, event_name);
        if ( ! system_declaration->insert_clock_declaration(dtmp) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
          delete dtmp;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }

    prophecy_clocks_parser.push_back("tmp");
    good_prophecy_clocks.push_back("tmp");
  }

  //now continue with the clock definition defined in the input
  auto const * d = system_declaration->get_clock_declaration($5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of clock " << $5 << std::endl;
  else {
    try {
      //ani:-100
      std::string event_name = "";
      const unsigned int _clock_size = 1;
      d = new tchecker::parsing::clock_declaration_t($5, _clock_size, std::move($6),2,event_name); //in the clock definition clock type 2 to denotes prophecy clock 
      if ( ! system_declaration->insert_clock_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  
  prophecy_clocks_parser.push_back($5); //adding clock name in prophecy clocks set
  good_prophecy_clocks.push_back($5); //since there is no edge declared until now this is gets added in good_prophecy_clocks set
}

| TOK_CLOCK ":" TOK_DEC_HISTORY_CLOCK ":" TOK_ID attr_list"\n"
{ 

  bool is_tmp_clk_defined = ((std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), "tmp")!=prophecy_clocks_parser.end()) && (prophecy_clocks_parser.size()!=0));
  if(!is_tmp_clk_defined){
    //declare tmp clock
    auto const * dtmp = system_declaration->get_clock_declaration("tmp");
    if (dtmp != nullptr)
      std::cerr << tchecker::log_error << @5 << " multiple declarations of clock tmp"  << std::endl;
    else {
      try {
        //ani:-100
        tchecker::parsing::attributes_t* attr_list_tmp = new tchecker::parsing::attributes_t();
        attr_list_tmp->clear();
        
        std::string event_name = "";
        const unsigned int _clock_size = 1;
        dtmp = new tchecker::parsing::clock_declaration_t(std::string("tmp"), _clock_size, std::move(*attr_list_tmp), 2, event_name);
        if ( ! system_declaration->insert_clock_declaration(dtmp) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
          delete dtmp;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
    
    prophecy_clocks_parser.push_back("tmp");
    good_prophecy_clocks.push_back("tmp");
  }

  auto const * d = system_declaration->get_clock_declaration($5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of clock " << $5 << std::endl;
  else {
    try {
      //ani:-100
      std::string event_name = "";
      const unsigned int _clock_size = 1;
      d = new tchecker::parsing::clock_declaration_t($5, _clock_size, std::move($6),1,event_name);
      if ( ! system_declaration->insert_clock_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of history clock declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  
  history_clocks_parser.push_back($5);
}

| TOK_CLOCK ":" TOK_DEC_TIMER ":" TOK_ID attr_list"\n"
{ 

  bool is_tmp_clk_defined = ((std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), "tmp")!=prophecy_clocks_parser.end()) && (prophecy_clocks_parser.size()!=0));
  if(!is_tmp_clk_defined){
    //declare tmp clock
    auto const * dtmp = system_declaration->get_clock_declaration("tmp");
    if (dtmp != nullptr)
      std::cerr << tchecker::log_error << @5 << " multiple declarations of clock tmp"  << std::endl;
    else {
      try {
        //ani:-100
        tchecker::parsing::attributes_t* attr_list_tmp = new tchecker::parsing::attributes_t();
        attr_list_tmp->clear();
        
        std::string event_name = "";
        const unsigned int _clock_size = 1;
        dtmp = new tchecker::parsing::clock_declaration_t(std::string("tmp"), _clock_size, std::move(*attr_list_tmp), 2, event_name);
        if ( ! system_declaration->insert_clock_declaration(dtmp) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
          delete dtmp;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
    
    prophecy_clocks_parser.push_back("tmp");
    good_prophecy_clocks.push_back("tmp");
  }


  auto const * d = system_declaration->get_clock_declaration($5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of clock " << $5 << std::endl;
  else {
    try {
      //ani:-100
      std::string event_name = "";
      const unsigned int _clock_size = 1;
      d = new tchecker::parsing::clock_declaration_t($5, _clock_size, std::move($6),2,event_name);
      if ( ! system_declaration->insert_clock_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  
  prophecy_clocks_parser.push_back($5);
  timer_clocks_parser.push_back($5);
  good_prophecy_clocks.push_back($5);

}

| TOK_CLOCK ":" TOK_DEC_NORMAL_CLOCK ":" TOK_ID attr_list"\n"
{

  bool is_tmp_clk_defined = ((std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), "tmp")!=prophecy_clocks_parser.end()) && (prophecy_clocks_parser.size()!=0));
  if(!is_tmp_clk_defined){
    //declare tmp clock
    auto const * dtmp = system_declaration->get_clock_declaration("tmp");
    if (dtmp != nullptr)
      std::cerr << tchecker::log_error << @5 << " multiple declarations of clock tmp"  << std::endl;
    else {
      try {
        //ani:-100
        tchecker::parsing::attributes_t* attr_list_tmp = new tchecker::parsing::attributes_t();
        attr_list_tmp->clear();
        
        std::string event_name = "";
        const unsigned int _clock_size = 1;
        dtmp = new tchecker::parsing::clock_declaration_t(std::string("tmp"), _clock_size, std::move(*attr_list_tmp), 2, event_name);
        if ( ! system_declaration->insert_clock_declaration(dtmp) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
          delete dtmp;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
    
    prophecy_clocks_parser.push_back("tmp");
    good_prophecy_clocks.push_back("tmp");
  }

  auto const * d = system_declaration->get_clock_declaration($5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of clock " << $5 << std::endl;
  else {
    try {
      //ani:-100
      std::string event_name = "";
      const unsigned int _clock_size = 1;
      d = new tchecker::parsing::clock_declaration_t($5, _clock_size, std::move($6),3,event_name);
      if ( ! system_declaration->insert_clock_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of history clock declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  
  history_clocks_parser.push_back($5);
}


| TOK_EDGE ":" TOK_ID ":" TOK_ID ":" TOK_ID ":" TOK_ID attr_list "\n"
{
  auto const * proc = system_declaration->get_process_declaration($3);
  if (proc == nullptr)
    std::cerr << tchecker::log_error << @3 << " process " << $3 << " is not declared" << std::endl;
  else {
    auto const * src = system_declaration->get_location_declaration($3, $5);
    if (src == nullptr)
      std::cerr << tchecker::log_error << @5 << " location " << $5 << " is not declared in process " << $3 << std::endl;
    else {
      auto const * tgt = system_declaration->get_location_declaration($3, $7);
      if (tgt == nullptr)
        std::cerr << tchecker::log_error << @7 << " location " << $7 << " is not declared in process " << $3 << std::endl;
      else {
        auto const * event = system_declaration->get_event_declaration($9);
        if (event == nullptr)
          std::cerr << tchecker::log_error << @9 << " event " << $9 << " is not declared" << std::endl;
        else {
          try {
            //ani:-100
            //this will get called when attr_list is empty
            //if event clocks are defined for the event then add a guard of event prophecy clock=0 and do for prophecy clocks before and add a do for history clock after
            bool is_prop_def = (std::find(event_prophecy_clocks.begin(), event_prophecy_clocks.end(), std::string($9)+"_p")!=event_prophecy_clocks.end());
            bool is_hist_def = (std::find(event_history_clocks.begin(), event_history_clocks.end(), std::string($9)+"_h")!=event_history_clocks.end());

            if(is_prop_def){
              //prophecy_clock is defined
              tchecker::parsing::attr_t* tmp_attr = new tchecker::parsing::attr_t(std::string("provided"), std::string($9)+std::string("_p==0")+" && tmp<=0", std::string(""), std::string(""));              
              $10.insert(tmp_attr);

              tmp_attr = new tchecker::parsing::attr_t(std::string("do"), std::string($9)+std::string("_p=0"), std::string(""), std::string(""));
              $10.insert(tmp_attr);
            }
            if(is_hist_def){
              //history_clock is defined
              tchecker::parsing::attr_t* tmp_attr = new tchecker::parsing::attr_t(std::string("do"), std::string($9)+std::string("_h=0"), std::string(""), std::string(""));
              $10.insert(tmp_attr);
            }
            if(is_prop_def || is_hist_def){
              tchecker::parsing::attr_t* tmp_attr = new tchecker::parsing::attr_t(std::string("do"), "tmp=0", std::string(""), std::string(""));
              $10.insert(tmp_attr);
            }
            auto * d = new tchecker::parsing::edge_declaration_t(*proc, *src, *tgt, *event, std::move($10));
            if ( ! system_declaration->insert_edge_declaration(d) ) {
              std::cerr << tchecker::log_error << @$ << " insertion of edge declaration failed" << std::endl;
              delete d;
            }
          }
          catch (std::exception const & e) {
            std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
          }
        }
      }
    }
  }

}


| TOK_EDGE ":" TOK_ID ":" TOK_ID ":" TOK_ID ":" TOK_ID TOK_PROGRAM_START program_guard2 TOK_PROGRAM_STOP "\n"
{   


  if(!continue_successor_comp){ //if till now finiteness is guaranteed
    tmp_prop_guards = tmp_single_prop_guards;
    
    //checking finiteness (X_D safety: if prop1-prop2 </<= c)
    //if there is a prophecy clock that appears in do operation before appearing in tmp_prop_guards, then remove it from good_prophecy_clocks!!!
    //and if there is a clock in prophecy_clocks_verify and not in good_prophecy_clocks then finiteness is not guranteed!!!!


    unsigned int tmp_do_ptr = 0, tmp_prop_check_ptr = 0;
    //tmp_do_ptr iterates over the do operations of this edge
    //tmp_prop_check_ptr iterates over the guards with prophecy clocks(<= or ==) -inf or (>= or ==)0 over this edge

    std::vector<std::string> all_prop_checked;
    //all_prop_checked contains a list of all prophecy clocks which are checked either for <=-inf or for ==-inf or for >=0 or for ==0

    while(tmp_do_ptr<$11[1].size() || tmp_prop_check_ptr<tmp_prop_guards.size()){
      
      //iterating through guards
      while(tmp_prop_guards[tmp_prop_check_ptr]!="tmp" && tmp_prop_check_ptr<tmp_prop_guards.size()){
        
        bool is_already = std::find(all_prop_checked.begin(), all_prop_checked.end(), tmp_prop_guards[tmp_prop_check_ptr])!=all_prop_checked.end();
        if(is_already==0){
          all_prop_checked.push_back(tmp_prop_guards[tmp_prop_check_ptr]);
        }
        tmp_prop_check_ptr++;
      }
      if(tmp_prop_guards[tmp_prop_check_ptr]=="tmp")
        tmp_prop_check_ptr++;
      

      //iterating through do operations
      while($11[1][tmp_do_ptr]!="tmp" && tmp_do_ptr<$11[1].size()){

        bool is_prop = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $11[1][tmp_do_ptr])!=prophecy_clocks_parser.end();
        bool is_finite = std::find(all_prop_checked.begin(), all_prop_checked.end(), $11[1][tmp_do_ptr])!=all_prop_checked.end();

        if(is_prop==1 && is_finite==0){ //if the prophecy clock is released without checking for <=-inf or ==-inf or >=0 or ==0
                                        //then it is not a good prophecy clock
          auto is_good_iter = std::find(good_prophecy_clocks.begin(), good_prophecy_clocks.end(), $11[1][tmp_do_ptr]);
          if(is_good_iter!=good_prophecy_clocks.end()){
            good_prophecy_clocks.erase(is_good_iter);
          }
        }
        tmp_do_ptr++;
      }
      if($11[1][tmp_do_ptr]=="tmp")
        tmp_do_ptr++;
    }


    tmp_prop_guards.clear();
    tmp_single_prop_guards.clear();

  
    //if prophecy_clocks_verify is not a subseteq of good prophecy clocks then finiteness not guranteed!!!!
    for(auto i:prophecy_clocks_verify){
      
      bool is_finite = (std::find(good_prophecy_clocks.begin(), good_prophecy_clocks.end(), i)!=good_prophecy_clocks.end());
      
      if(is_finite==0){   //there is a clock i which is in prophecy_clocks_verify but not in good_prophecy_clocks
                          //that is there is a diagonal constraint corresponding to it in one of the edges but it is
                          //not checked for <=-inf or ==-inf or ==0 or >=0 explicitly before releasing
        std::cout << "Successor computation is not guaranteed to be finite! Want to continue? (Respond Y for Yes, N for No)\n";
        std::cout << "Line number " << @$ << std::endl;
        char want_to_continue;
        std::cin >> want_to_continue;
        if(want_to_continue!='y' && want_to_continue!='Y')
          exit(-1);
        else{
          continue_successor_comp = true;
          break;
        }
      }
    }
  }
  //finiteness check over!! (X_D safety check over)


  //for event clocks
  bool is_prop_def = (std::find(event_prophecy_clocks.begin(), event_prophecy_clocks.end(), std::string($9)+"_p")!=event_prophecy_clocks.end());
  bool is_hist_def = (std::find(event_history_clocks.begin(), event_history_clocks.end(), std::string($9)+"_h")!=event_history_clocks.end());
  //if the prophecy clock is defined for this event then add a statement prophecy clock ==0 && tmp<=0 in the front of
  //all elements of the vector $11[0]
  //and the statement prophecy clock, tmp=0 in front of the do operation $11[1]

  if(is_prop_def==1){

    //adding guard operation
    if($11[0].size()==0){ //this means there is no guard yet
      $11[0].push_back(std::string($9)+"_p==0 && tmp<=0");
    }
    else{
      for(unsigned int iter_g=0; iter_g<$11[0].size();iter_g++){
        $11[0][iter_g] = std::string($9)+"_p==0 && tmp<=0 && " + $11[0][iter_g];
      }
    }

    //adding do operation
    if($11[1].size()==0){
      $11[1].push_back(std::string($9)+"_p");
      $11[1].push_back("tmp");
    }
    else{
      std::vector<std::string> v = $11[1]; //ani:-101 optimization possible
      $11[1].clear();
      $11[1].push_back(std::string($9)+"_p");
      $11[1].push_back("tmp");
      for(auto i:v){
        $11[1].push_back(i);
      }
    }
  }


  //if the history clock is defined for this event then add a statement tmp<=0 in the back for
  //all elements of the guard part of the vector $11[0] 
  //and the statement history clock=0, tmp=0 in back of the do part $11[1]
  if(is_hist_def==1){
    if($11[0].size()==0){
      $11[0].push_back("tmp<=0");
    }
    else{
      for(unsigned int iter_g=0; iter_g<$11[0].size();iter_g++){
        $11[0][iter_g] = $11[0][iter_g] + " && tmp<=0";
      }
    }

    $11[1].push_back(std::string($9)+"_h");
    $11[1].push_back("tmp");

  }
  

  auto const * proc = system_declaration->get_process_declaration($3);
  if (proc == nullptr)
    std::cerr << tchecker::log_error << @3 << " process " << $3 << " is not declared" << std::endl;
  else {
    auto const * src = system_declaration->get_location_declaration($3, $5);
    if (src == nullptr)
      std::cerr << tchecker::log_error << @5 << " location " << $5 << " is not declared in process " << $3 << std::endl;
    else {
      auto const * tgt = system_declaration->get_location_declaration($3, $7);
      if (tgt == nullptr)
        std::cerr << tchecker::log_error << @7 << " location " << $7 << " is not declared in process " << $3 << std::endl;
      else {
        auto const * event = system_declaration->get_event_declaration($9);
        if (event == nullptr)
          std::cerr << tchecker::log_error << @9 << " event " << $9 << " is not declared" << std::endl;
        else {
          try {
            //ani:-100

            tchecker::parsing::attributes_t* attr_list_full = new tchecker::parsing::attributes_t();
            attr_list_full->clear();

            //if there is a guard then there is a do operation always
            if($11[0].size()!=0){

              for(auto i=$11[0].begin(); i!=$11[0].end();i++){
                
                attr_list_full->clear();
                tchecker::parsing::attr_t* tmp_attr_do = nullptr;
                std::string all_reset_release_clocks = "";
                for(auto j=0;j<$11[1].size();j++){

                  
                  std::stringstream int_var_string($11[1][j]);
                  std::string int_var_id="";
                  std::getline(int_var_string, int_var_id, '=');
                  // std::cout << "ANI: " << $11[1][j] << " " << int_var_id << std::endl;
                  if($11[1][j].find('=')!=std::string::npos && 
                  std::find(parser_integer_vars.begin(),parser_integer_vars.end(),int_var_id)!=parser_integer_vars.end()){
                    all_reset_release_clocks = all_reset_release_clocks + $11[1][j] + ";";  
                  }
                  
                  else{
                    all_reset_release_clocks = all_reset_release_clocks + $11[1][j]+"=0;";
                  }
                }

                tmp_attr_do = new tchecker::parsing::attr_t(std::string("do"), all_reset_release_clocks, std::string(""), std::string(""));
                attr_list_full->insert(tmp_attr_do);

                
                tchecker::parsing::attr_t* tmp_attr_guard = nullptr;

                tmp_attr_guard = new tchecker::parsing::attr_t(std::string("provided"), *i, std::string(""), std::string(""));
                attr_list_full->insert(tmp_attr_guard);
                
                auto * d = new tchecker::parsing::edge_declaration_t(*proc, *src, *tgt, *event, std::move(*attr_list_full));
                
                if (!system_declaration->insert_edge_declaration(d) ) {
                  std::cerr << tchecker::log_error << @$ << " insertion of edge declaration failed" << std::endl;
                  delete d;
                }

              }
            }
            else{//if there is no guard then there is no do operation

              tchecker::parsing::attributes_t attr_list;
              attr_list.clear();

              auto * d = new tchecker::parsing::edge_declaration_t(*proc, *src, *tgt, *event, std::move(attr_list));
              if ( ! system_declaration->insert_edge_declaration(d) ) {
                std::cerr << tchecker::log_error << @$ << " insertion of edge declaration failed" << std::endl;
                delete d;
              }
            }
          }
          catch (std::exception const & e) {
            std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
          }
        }
      }
    }
  }

}

| TOK_EVENT ":" TOK_ID attr_list "\n"
{

  auto const * d = system_declaration->get_event_declaration($3);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @3 << " multiple declarations of event " << $3 << std::endl;
  else {
    try {
      d = new tchecker::parsing::event_declaration_t($3, std::move($4));
      if ( ! system_declaration->insert_event_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of event declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << @$ << " " << e.what() << std::endl;
    }
  }
}
//bool:bool
| TOK_EVENT ":" TOK_ID ":" uinteger ":" uinteger attr_list "\n"
{
  bool is_tmp_clk_defined = ((std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), "tmp")!=prophecy_clocks_parser.end()) && (prophecy_clocks_parser.size()!=0));
  if(!is_tmp_clk_defined){
    //declare tmp clock
    auto const * dtmp = system_declaration->get_clock_declaration("tmp");
    if (dtmp != nullptr)
      std::cerr << tchecker::log_error << @5 << " multiple declarations of clock tmp"  << std::endl;
    else {
      try {
        //ani:-100
        tchecker::parsing::attributes_t* attr_list_tmp = new tchecker::parsing::attributes_t();
        attr_list_tmp->clear();
        
        std::string event_name = "";
        const unsigned int _clock_size = 1;
        dtmp = new tchecker::parsing::clock_declaration_t(std::string("tmp"), _clock_size, std::move(*attr_list_tmp), 2, event_name);
        if ( ! system_declaration->insert_clock_declaration(dtmp) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of prophecy clock declaration failed" << std::endl;
          delete dtmp;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
    
    prophecy_clocks_parser.push_back("tmp");
    good_prophecy_clocks.push_back("tmp");
  }
  
  //ani:-100


  auto const * d = system_declaration->get_event_declaration($3);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @3 << " multiple declarations of event " << $3 << std::endl;
  else {
    try {
      d = new tchecker::parsing::event_declaration_t($3, std::move($8));
      if ( ! system_declaration->insert_event_declaration(d) ) {
        std::cerr << tchecker::log_error << @$ << " insertion of event declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << @$ << " " << e.what() << std::endl;
    }
    
    //inserting history and prophecy clocks
    const unsigned int _clock_size = 1;
    std::string _clock_empty_attributes = "";
    
    //if the history clock bit is 1, then the history clock is defined
    if($5==1){
      std::string _hist_suffix = "_h";
      std::string _clock_history = $3 + _hist_suffix;
      
      event_history_clocks.push_back(_clock_history);
      history_clocks_parser.push_back(_clock_history);
      auto const * clockd = system_declaration->get_clock_declaration(_clock_history);
      if (clockd != nullptr)
        std::cerr << tchecker::log_error << @$ << " variable " << _clock_history << " already declared as a clock" << std::endl;
    
      try {
        //ani:-100
        //std::move($8) needs to be changed
        clockd = new tchecker::parsing::clock_declaration_t(_clock_history, _clock_size, std::move($8), 1, $3);
        if (!system_declaration->insert_clock_declaration(clockd)) {
          std::cerr << tchecker::log_error << @$ << " insertion of clock declaration failed" << std::endl;
          delete clockd;
        }        
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }

    //if the prophecy clock bit is 1, then the prophecy clock is defined!
    if($7==1){
      std::string _prop_suffix = "_p";
      std::string _clock_prophecy = $3 + _prop_suffix;
      event_prophecy_clocks.push_back(_clock_prophecy);
      
      prophecy_clocks_parser.push_back(_clock_prophecy);
      good_prophecy_clocks.push_back(_clock_prophecy);
      
      auto const * clockd = system_declaration->get_clock_declaration(_clock_prophecy);
      if (clockd != nullptr)
        std::cerr << tchecker::log_error << @$ << " variable " << _clock_prophecy << " already declared as a clock" << std::endl;
    
      try {
        //ani:-100
        //std::move($8) needs to be changed
        clockd = new tchecker::parsing::clock_declaration_t(_clock_prophecy, _clock_size, std::move($8), 2, $3);
        if (!system_declaration->insert_clock_declaration(clockd)) {
          std::cerr << tchecker::log_error << @$ << " insertion of clock declaration failed" << std::endl;
          delete clockd;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
  }

}

| TOK_INT ":" uinteger ":" integer ":" integer ":" integer ":" TOK_ID attr_list "\n"
{
  auto const * d = system_declaration->get_int_declaration($11);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @11 << " multiple declarations of int variable " << $11 << std::endl;
  else {
    auto const * clockd = system_declaration->get_clock_declaration($11);
    if (clockd != nullptr)
      std::cerr << tchecker::log_error << @11 << " variable " << $11 << " already declared as a clock" << std::endl;
    else {
      try {
        d = new tchecker::parsing::int_declaration_t($11, $3, $5, $7, $9, std::move($12));
        if ( ! system_declaration->insert_int_declaration(d) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of int declaration failed" << std::endl;
          delete d;
        }
        parser_integer_vars.push_back($11);
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
  }
}

| TOK_LOCATION ":" TOK_ID ":" TOK_ID attr_list "\n"
{
  auto const * d = system_declaration->get_location_declaration($3, $5);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @5 << " multiple declarations of location " << $5 << " in process " << $3 << std::endl;
  else {
    auto const * proc = system_declaration->get_process_declaration($3);
    if (proc == nullptr)
      std::cerr << tchecker::log_error << @3 << " process " << $3 << " is not declared" << std::endl;
    else {
      try {
        d = new location_declaration_t($5, *proc, std::move($6));
        if ( ! system_declaration->insert_location_declaration(d) ) {
          std::cerr << tchecker::log_error << @$ << " insertion of location declaration failed" << std::endl;
          delete d;
        }
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
  }
}

| TOK_PROCESS ":" TOK_ID attr_list "\n"
{
  auto const * d = system_declaration->get_process_declaration($3);
  if (d != nullptr)
    std::cerr << tchecker::log_error << @3 << " multiple declarations of process " << $3 << std::endl;
  else {
    try {
      d = new tchecker::parsing::process_declaration_t($3, std::move($4));
      if ( ! system_declaration->insert_process_declaration(d) ) {
        std::cerr << tchecker::log_error << @5 << " insertion of process declaration failed" << std::endl;
        delete d;
      }
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @5 << " " << e.what() << std::endl;
    }
  }
}

| TOK_SYNC ":" sync_constraint_list attr_list "\n"
{
  try {
    auto const * d = new tchecker::parsing::sync_declaration_t(std::move($3), std::move($4));
    if ( ! system_declaration->insert_sync_declaration(d) ) {
      std::cerr << tchecker::log_error << @$ << " insertion of sync declaration failed" << std::endl;
      delete d;
    }
  }
  catch (std::exception const & e) {
    std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
  }
}

| error "\n"
;

program_guard2: TOK_PROG_GUARD eca_guards2 TOK_SEMICOLON program_do2
{

  $$ = std::vector<std::vector<std::string>>(2);
  //$$[0] is a list of all or guards
  //$$[1] is a list of all do do things

  //in a program_do2 call always whether it is empty or non empty there is atleast one object in the release
  if($2.size()!=0 && $4[0].size()!=0){ //if there is atleast one guard defined and at least one guard in program_do2

    for(auto guard:$2){
      for(auto next_guards:$4[0])
          $$[0].push_back(guard+" && tmp<=0 && "+next_guards);
      }
  }
  else if($2.size()!=0 && $4[0].size()==0){ //if there is atleast one guard defined and no guard in program_do2
    for(auto guard:$2)
      $$[0].push_back(guard+"&& tmp<=0");
  } 
  else if($2.size()==0 && $4[0].size()==0){ //if there is no guard defined and no guard in program_do2
    $$[0].push_back("tmp<=0");
  }
  else if($2.size()==0 && $4[0].size()!=0){ //if there is no guard defined and some guard(s) in program_do2
    for(auto next_guards:$4[0]){
      $$[0].push_back("tmp<=0 && "+next_guards);
    }
  }
  
  $$[1] = $4[1];

  //invariant: $$[0].size() is never 0 in this case

  // tmp_prop_guards = tmp_single_prop_guards; //for finiteness check

}
|{ //this happens iff do is getting called in the end or the program is empty
  $$ = std::vector<std::vector<std::string>>(2);
};

program_do2: TOK_PROG_DO eca_release_reset TOK_SEMICOLON program_guard2
{
  
  //if program_guard2 is empty ie $4[0].size()==0 and $4[1].size()==0 then this is the last release reset
  $$ = std::vector<std::vector<std::string>>(2);
  
  for(auto guard:$4[0])
    $$[0].push_back(guard);

  
  
  bool add_final_tmp_do = 0; //useful for timers/ if there is a timer assignment in last do operation, then need to add a release operation in this do and another guard and its corresponding empty do operation
  
  for(unsigned int iter_rel_clks=0;iter_rel_clks<$2.size();iter_rel_clks++){
    bool is_clk_timer = (std::find(timer_clocks_parser.begin(), timer_clocks_parser.end(), $2[iter_rel_clks])!=timer_clocks_parser.end());
    if(is_clk_timer){
      //this is a timer
 
      if(iter_rel_clks<$2.size()-1 && $2[iter_rel_clks+1][0]=='='){ //if this is a timer assignment operation

        //assignment of the timer
        $$[1].push_back($2[iter_rel_clks]);
        if($$[0].size()!=0){
          for(unsigned int iter_guard=0;iter_guard<$$[0].size();iter_guard++)
            $$[0][iter_guard] = $2[iter_rel_clks]+$2[iter_rel_clks+1] + " && " + $$[0][iter_guard];
        }
        else{ //if there is no guard then add a guard after the do operation!
          $$[0].push_back($2[iter_rel_clks]+$2[iter_rel_clks+1] + " && tmp<=0");
          add_final_tmp_do = 1;
        
          // tmp_prop_guards.push_back("tmp");
          tmp_single_prop_guards.push_back("tmp");
          
        }
        iter_rel_clks++;
      }
      else{
        //releasing a timer!!!
        $$[1].push_back($2[iter_rel_clks]);
        if($$[0].size()!=0){
          for(unsigned int iter_guard=0;iter_guard<$$[0].size();iter_guard++)
            $$[0][iter_guard] = $2[iter_rel_clks]+ "==" + std::to_string(tchecker::dbm::MINUS_INF_VALUE) + " && " + $$[0][iter_guard];
        }
        else{ //if there is no guard then add a guard after the do operation!
          $$[0].push_back($2[iter_rel_clks]+ "==" + std::to_string(tchecker::dbm::MINUS_INF_VALUE) + " && tmp<=0");
          add_final_tmp_do = 1;
        
          // tmp_prop_guards.push_back("tmp");
          tmp_single_prop_guards.push_back("tmp");
          
        }
      }
    }
    else{
      $$[1].push_back($2[iter_rel_clks]);
    }
  }
  
  $$[1].push_back("tmp");

  if(add_final_tmp_do==1)
    $$[1].push_back("tmp");
  
  // assert(add_final_tmp_do==0 || $4[1].size()==0); //if add_final_tmp_do==1 then $4[1].size()==0

  for(auto do_ops:$4[1])
    $$[1].push_back(do_ops);

  // for(auto m:$$[1]){
  //   std::cout << "ANI:1126:" << m << std::endl;
  // }
}
| { //only get called from program_guard when guard ends with a do, and there is no do at the end
    //so return a tmp=0 do here because our assumption is that guard, do, guard, do,....,guard, do
  $$ = std::vector<std::vector<std::string>>(2);
  $$[1].push_back("tmp");
};



eca_release_reset: non_empty_reset_release
{
  $$ = $1;
}
| {
  $$.clear();
};

non_empty_reset_release: TOK_ID TOK_COMMA non_empty_reset_release
{

  /*
  bool is_event_clock = (std::find(event_history_clocks.begin(), event_history_clock.end(), $1)!=event_history_clock.end());
  is_event_clock = is_event_clock || (std::find(event_prophecy_clock.begin(), event_prophecy_clock.end(), $1)!=event_prophecy_clock.end());
  if(is_event_clock){
    try{
      std::cerr << tchecker::log_error << @$ << " cannot release or reset an event clock " << $1 << " manually " << std::endl;
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  */

  std::vector<std::string> v;
  v.push_back($1);
  for(auto res_rel:$3)
    v.push_back(res_rel);
  
  $$ = v;
}

| TOK_ID TOK_EQUAL int_term TOK_COMMA non_empty_reset_release {
  // std::cout << "ANI:MATCHED HERE1 " << $1 << " = " << $3 << std::endl;; 
  auto iter_int = std::find(parser_integer_vars.begin(),parser_integer_vars.end(),$1);
  
  if(iter_int==parser_integer_vars.end()){
    try{
      std::cerr << tchecker::log_error << @$ << " do assignment failed " << $1 << " not an integer variable" << std::endl;
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  else{
    // std::cout << "ANI: " << $1 << " " << $3 << std::endl; 
    std::vector<std::string> v;
    if (iter_int!=parser_integer_vars.end()){
      v.push_back($1+"="+$3);
    }
    for(auto res_rel:$5)
      v.push_back(res_rel);
    $$ = v;
  }
}

| TOK_ID TOK_EQUAL int_term {
  // std::cout << "ANI:MATCHED HERE1 " << $1 << " = " << $3 << std::endl;; 
  auto iter_int = std::find(parser_integer_vars.begin(),parser_integer_vars.end(),$1);
  
  if(iter_int==parser_integer_vars.end()){
    try{
      std::cerr << tchecker::log_error << @$ << " do assignment failed " << $1 << " not an integer variable" << std::endl;
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  else{
    // std::cout << "ANI: " << $1 << " " << $3 << std::endl; 
    std::vector<std::string> v;
    if (iter_int!=parser_integer_vars.end()){
      v.push_back($1+"="+$3);
    }
    $$ = v;
  }
}

| TOK_ID TOK_EQUAL TOK_TEXT TOK_COMMA non_empty_reset_release
{ 
  auto iter = std::find(timer_clocks_parser.begin(),timer_clocks_parser.end(),$1);
  auto iter_int = std::find(parser_integer_vars.begin(),parser_integer_vars.end(),$1);
  // std::cout << "ani: -211 " << (iter==timer_clocks_parser.end()) << " " << $1 << " " << $3 << std::endl;
  if(iter==timer_clocks_parser.end() && iter_int==parser_integer_vars.end()){
    try{
      std::cerr << tchecker::log_error << @$ << " do assignment failed, clock " << $1 << " not a timer nor an integer variable" << std::endl;
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  else{
    std::vector<std::string> v;
    
    int value_assign = std::stoi($3);
    // value_assign = -1*value_assign; assuming timers are assigned negative values only
    // std::cout << "ani: -090 " << value_assign << std::endl;
    if (iter_int!=parser_integer_vars.end()){
      v.push_back($1+"="+std::to_string(value_assign));
    }
    else{
      v.push_back($1);
      v.push_back("=="+std::to_string(value_assign));
    }

    $$ = v;

    for(auto rel_res:$5){
      $$.push_back(rel_res);
    }
  }

}

| TOK_ID TOK_EQUAL TOK_TEXT
{ 
  auto iter = std::find(timer_clocks_parser.begin(),timer_clocks_parser.end(),$1);
  auto iter_int = std::find(parser_integer_vars.begin(),parser_integer_vars.end(),$1);
  if(iter==timer_clocks_parser.end() && iter_int==parser_integer_vars.end()){
    try{
      std::cerr << tchecker::log_error << @$ << " do assignment failed, clock " << $1 << " not a timer nor an integer variable" << std::endl;
    }
    catch (std::exception const & e) {
      std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
    }
  }
  else{
    std::vector<std::string> v;
    
    int value_assign = std::stoi($3);
    // value_assign = -1*value_assign;
    
    if(iter==timer_clocks_parser.end()){
      v.push_back($1+"="+std::to_string(value_assign));
    }
    else{
      v.push_back($1);
      v.push_back("=="+std::to_string(value_assign));
    }
    $$ = v;
  }

}


|  TOK_ID
{
  $$.clear();
  $$.push_back($1);
};

eca_guards2: non_empty_eca_guards2
{
  $$ = $1;
  tmp_single_prop_guards.push_back("tmp");
}
| {
  std::vector<std::string>v;
  v.clear();
  $$ = v;

  tmp_single_prop_guards.push_back("tmp");
};

non_empty_eca_guards2: eca_single_guard2 TOK_AND non_empty_eca_guards2{
  //taking cross product of all guards i.e. AxB
  std::vector<std::string> all_guards;
  for(auto i=$3.begin();i!=$3.end();i++){
    for(auto j=$1.begin();j!=$1.end();j++){
      all_guards.push_back(*i+" && "+*j);
    }
  }
  $$ = all_guards;
}
| eca_single_guard2{
  $$=$1;
};

eca_single_guard2: TOK_ID TOK_LE TOK_TEXT {
  std::vector<std::string> guard;
  
  std::string s = $1+"<="+$3;
  guard.push_back(s);

  $$ = guard;

  //finiteness check if prophecy clock is checked <=-inf
  bool is_prop = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_minus_inf = (std::stoi($3)==tchecker::dbm::MINUS_INF_VALUE);
  if(is_prop==1 && is_minus_inf==1){
    bool is_already = (std::find(tmp_single_prop_guards.begin(), tmp_single_prop_guards.end(), $1)!=tmp_single_prop_guards.end());
    if(is_already==0)
      tmp_single_prop_guards.push_back($1);
  }


}
| TOK_ID TOK_LT TOK_TEXT {
  std::vector<std::string> guard;
  
  std::string s = $1+"<"+$3;
  guard.push_back(s);
  
  $$ = guard;

}
| TOK_ID TOK_GE TOK_TEXT {
  std::vector<std::string> guard;
  
  std::string s = $1+">="+$3;
  guard.push_back(s);
  
  $$ = guard;

  //finiteness check if the prophecy clock is checked for >=0
  bool is_prop = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_zero = (std::stoi($3)==0);
  if(is_prop==1 && is_zero==1){
    bool is_already = (std::find(tmp_single_prop_guards.begin(), tmp_single_prop_guards.end(), $1)!=tmp_single_prop_guards.end());
    if(is_already==0)
      tmp_single_prop_guards.push_back($1);
  }

}
| TOK_ID TOK_GT TOK_TEXT {
  std::vector<std::string> guard;
  
  std::string s = $1+">"+$3;
  guard.push_back(s);
  
  $$ = guard;
}
| TOK_ID TOK_EQ TOK_TEXT {
  std::vector<std::string> guard;
  
  std::string s = $1+"<="+$3 +" && "+$1+">="+$3;
  guard.push_back(s);
  
  $$ = guard;

  //for finiteness check if prophecy clock is checked for ==-inf or ==0
  bool is_prop = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_minus_inf = (std::stoi($3)==tchecker::dbm::MINUS_INF_VALUE);
  bool is_zero = (std::stoi($3)==0);
  
  if(is_prop==1 && (is_minus_inf==1 || is_zero==1)){
    bool is_already = (std::find(tmp_single_prop_guards.begin(), tmp_single_prop_guards.end(), $1)!=tmp_single_prop_guards.end());
    if(is_already==0)
      tmp_single_prop_guards.push_back($1);
  }

}
| TOK_ID TOK_MINUS TOK_ID TOK_LE TOK_TEXT {
  std::vector<std::string> guard;

  std::string s = $1+"-"+$3+"<="+$5;
  guard.push_back(s);


  $$ = guard;

  //required for finiteness check
  bool is_prop1 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_prop2 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3)!=prophecy_clocks_parser.end());
  if(is_prop1==1 && is_prop2==1){
    bool is_prop3 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $1)!=prophecy_clocks_verify.end());
    if(is_prop3==0)
      prophecy_clocks_verify.push_back($1);
    bool is_prop4 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $3)!=prophecy_clocks_verify.end());
    if(is_prop4==0)
      prophecy_clocks_verify.push_back($3);
  }

}
| TOK_ID TOK_MINUS TOK_ID TOK_LT TOK_TEXT {
  std::vector<std::string> guard;

  std::string s = $1+"-"+$3+"<"+$5;
  guard.push_back(s);
  
  $$ = guard;

  //required for finiteness check
  bool is_prop1 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_prop2 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3)!=prophecy_clocks_parser.end());
  if(is_prop1==1 && is_prop2==1){
    bool is_prop3 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $1)!=prophecy_clocks_verify.end());
    if(is_prop3==0)
      prophecy_clocks_verify.push_back($1);
    bool is_prop4 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $3)!=prophecy_clocks_verify.end());
    if(is_prop4==0)
      prophecy_clocks_verify.push_back($3);
  }
}
| TOK_ID TOK_MINUS TOK_ID TOK_GE TOK_TEXT {
  auto is_prop1 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1);
  auto is_hist1 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $1);
  
  auto is_prop2 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3);
  auto is_hist2 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $3);
  
  std::vector<std::string> guards;

  if(is_prop1!=prophecy_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //prophecy1-prophecy2 >= number  iff prophecy2-prophecy1 <= -number or prophecy2==prophecy1==-inf
    
    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    std::string guard1 =  "";
    guard1 = $3 + "-" + $1 + "<=" + $5;

    std::string guard2 =  "";
    guard2 = $1 + "==" + "-INF && " + $3 + "==" + "-INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
    
  }
  if(is_hist1!=history_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //history-prophecy >= number iff prophecy-history <= -number

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = $3 + "-" + $1 + "<=" + $5;    
    
    guards.push_back(guard);
  
  }
  if(is_prop1!=prophecy_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //prophecy - history >= number iff history - prophecy <= -number

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = $3 + "-" + $1 + "<=" + $5;
    
    guards.push_back(guard);

  }
  if(is_hist1!=history_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //histroy1 - history2 >= number iff history2-history1<=-number or history1==history2==INF
    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    std::string guard1 =  "";
    guard1 = $3 + "-" + $1 + "<=" + $5;

    std::string guard2 =  "";
    guard2 = $1 + "==" + "INF && " + $3 + "==" + "INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
  
  }
  $$=guards;

  //required for finiteness check
  bool is_prop5 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_prop6 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3)!=prophecy_clocks_parser.end());
  if(is_prop5==1 && is_prop6==1){
    bool is_prop7 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $1)!=prophecy_clocks_verify.end());
    if(is_prop7==0)
      prophecy_clocks_verify.push_back($1);
    bool is_prop8 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $3)!=prophecy_clocks_verify.end());
    if(is_prop8==0)
      prophecy_clocks_verify.push_back($3);
  }
}
| TOK_ID TOK_MINUS TOK_ID TOK_GT TOK_TEXT {
  auto is_prop1 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1);
  auto is_hist1 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $1);
  
  auto is_prop2 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3);
  auto is_hist2 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $3);
  
  std::vector<std::string> guards;

  if(is_prop1!=prophecy_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //prophecy1-prophecy2 >= number  iff prophecy2-prophecy1 <= -number or prophecy2==prophecy1==-inf
    
    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    std::string guard1 =  "";
    guard1 = $3 + "-" + $1 + "<" + $5;

    std::string guard2 =  "";
    guard2 = $1 + "==" + "-INF && " + $3 + "==" + "-INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
    
  }
  if(is_hist1!=history_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //history-prophecy >= number iff prophecy-history <= -number

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = $3 + "-" + $1 + "<" + $5;    
    
    guards.push_back(guard);
  
  }
  if(is_prop1!=prophecy_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //prophecy - history >= number iff history - prophecy <= -number

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = $3 + "-" + $1 + "<" + $5;
    
    guards.push_back(guard);

  }
  if(is_hist1!=history_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //histroy1 - history2 >= number iff history2-history1<=-number or history1==history2==INF
    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    std::string guard1 =  "";
    guard1 = $3 + "-" + $1 + "<" + $5;

    std::string guard2 =  "";
    guard2 = $1 + "==" + "INF && " + $3 + "==" + "INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
  
  }
  $$=guards;


    //required for finiteness check
  bool is_prop5 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_prop6 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3)!=prophecy_clocks_parser.end());
  if(is_prop5==1 && is_prop6==1){
    bool is_prop7 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $1)!=prophecy_clocks_verify.end());
    if(is_prop7==0)
      prophecy_clocks_verify.push_back($1);
    bool is_prop8 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $3)!=prophecy_clocks_verify.end());
    if(is_prop8==0)
      prophecy_clocks_verify.push_back($3);
  }
}
| TOK_ID TOK_MINUS TOK_ID TOK_EQ TOK_TEXT {
  auto is_prop1 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1);
  auto is_hist1 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $1);
  
  auto is_prop2 = std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3);
  auto is_hist2 = std::find(history_clocks_parser.begin(), history_clocks_parser.end(), $3);
  
  std::vector<std::string> guards;

  if(is_prop1!=prophecy_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //prophecy1-prophecy2 == number  iff prophecy1-prophecy2 <= number and prophecy1-prophecy2>=number
    
    //iff prophecy1-prophecy2 <= number and (prophecy2-prophecy1<=-number or prophecy2==prophecy1==-inf)
    
    //iff (prophecy1-prophecy2 <= number and prophecy2-prophecy1<=-number) or 
    //    (prophecy1-prophecy2 <= number and prophecy2==prophecy1==-inf)
    
    std::string guard_main = $1 + "-" + $3 + "<=" + $5;

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    

    std::string guard1 =  "";
    guard1 = guard_main + " && " + $3 + "-" + $1 + "<=" + $5;

    std::string guard2 =  "";
    guard2 = guard_main + " && " + $1 + "==" + "-INF && " + $3 + "==" + "-INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
    
  }
  if(is_hist1!=history_clocks_parser.end() && is_prop2!=prophecy_clocks_parser.end()){
    //history-prophecy == number iff history-prophecy >= number && history-prophecy <= number
    //iff prophecy-history <= -number && history-prophecy <= number
    
    std::string guard_le = $1 + "-" + $3 + "<=" + $5;

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = guard_le + " && " +$3 + "-" + $1 + "<=" + $5;    
    guards.push_back(guard);
  
  }
  if(is_prop1!=prophecy_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //prophecy - history == number iff prophecy - history >= number && prophecy - history <= number
    // iff history - prophecy <= number && prophecy - history <= number
    
    std::string guard_le = $1 + "-" + $3 + "<=" + $5;

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }

    std::string guard =  "";
    guard = guard_le + " && " + $3 + "-" + $1 + "<=" + $5;
    
    guards.push_back(guard);

  }
  if(is_hist1!=history_clocks_parser.end() && is_hist2!=history_clocks_parser.end()){
    //histroy1 - history2 == number iff history1-history2<=number && history1-history2>=number
    // iff history1-history2<=number && (history2-history1<=-number or history2 == history1 == inf)
    
    std::string guard_main = $1 + "-" + $3 + "<=" + $5;

    if($5[0]=='-'){
      $5[0] = ' ';
    }
    else{
      if($5[0]=='+')
        $5[0]='-';
      else
        $5 = '-' + $5;
    }
    std::string guard1 =  "";
    guard1 = guard_main + " && " + $3 + "-" + $1 + "<=" + $5;

    std::string guard2 =  "";
    guard2 = guard_main + " && " + $1 + "==" + "INF && " + $3 + "==" + "INF";

    boost::replace_all(guard2, "INF", std::to_string(tchecker::dbm::INF_VALUE));

    guards.push_back(guard1);
    guards.push_back(guard2);
  
  }
  $$=guards;


  //required for finiteness check
  bool is_prop5 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $1)!=prophecy_clocks_parser.end());
  bool is_prop6 = (std::find(prophecy_clocks_parser.begin(), prophecy_clocks_parser.end(), $3)!=prophecy_clocks_parser.end());
  if(is_prop5==1 && is_prop6==1){
    bool is_prop7 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $1)!=prophecy_clocks_verify.end());
    if(is_prop7==0)
      prophecy_clocks_verify.push_back($1);
    bool is_prop8 = (std::find(prophecy_clocks_verify.begin(), prophecy_clocks_verify.end(), $3)!=prophecy_clocks_verify.end());
    if(is_prop8==0)
      prophecy_clocks_verify.push_back($3);
  }
}
| predicate_expr {
  std::vector<std::string> guard;
  
  std::string s = $1;
  guard.push_back(s);
  
  $$ = guard;
}
;

predicate_expr: "(" predicate_expr ")" 
{
  $$ = $2;
}
| int_term TOK_EQ int_term 
{
  $$ = $1 + "==" + $3;
} 
| int_term "!=" int_term {
  $$ = $1 + "!=" + $3;
}
| int_term "<" int_term {
  $$ = $1 + "<" + $3;
}
| int_term ">=" int_term {
  $$ = $1 + ">=" + $3;
}
| int_term ">" int_term {
  $$ = $1 + ">" + $3;
}
| int_term "<=" int_term {
  $$ = $1 + "<=" + $3;
}
;

int_term: int_term TOK_PLUS int_term{
  $$ = $1 + "+" + $3;
  std::cout << "ANI:1812:" << $$ << std::endl;
}
| int_term TOK_MINUS int_term{
  $$ = $1 + "-" + $3;
}
| int_term TOK_MUL int_term{
  $$ = $1 + "*" + $3;
  std::cout << "ANI:1819:" << $$ << std::endl;
}
| int_term TOK_DIV int_term{
  $$ = $1 + "/" + $3;
}
| int_term TOK_MOD int_term{
  $$ = $1 + "%" + $3;
}
| TOK_MINUS int_term {
  $$ = "-" + $2;
}
| TOK_ID {
  if (std::find(parser_integer_vars.begin(), parser_integer_vars.end(), $1)!=parser_integer_vars.end())
    $$ = $1;
  else{
    std::cerr << tchecker::log_error << @1 << " " << $1 << " is not an integer variable" << std::endl;
  }
  // std::cout << "ANI:1811:" << $$ << std::endl;
}
| TOK_TEXT {
    $$ = $1;
    // std::cout << "ANI:1815:" << $$ << std::endl;
}
;

attr_list:
"{" non_empty_attr_list "}"
{ $$ = std::move($2); }
| "{" "}"
{ $$.clear(); }
| { $$.clear(); }
;


non_empty_attr_list:
attr
{
  if ($1 != nullptr)
    $$.insert($1);
}
| non_empty_attr_list ":" attr
{
  if ($3 != nullptr)
    $1.insert($3);
  $$ = std::move($1);
}
;


attr:
TOK_ID ":" text_or_empty
{
  $$ = nullptr;
  if ($1 == "")
    std::cerr << tchecker::log_error << @1 << " empty attribute key" << std::endl;
  else {
    std::stringstream key_loc, value_loc;
    key_loc << @1;
    value_loc << @3;
    boost::trim($3);
    $$ = new tchecker::parsing::attr_t($1, $3, key_loc.str(), value_loc.str());
  }
}
;


text_or_empty:
TOK_TEXT
{ $$ = std::move($1); }
|
{ $$.clear(); }
;


sync_constraint_list:
sync_constraint
{
  if ($1 != nullptr)
  $$.push_back($1);
}
| sync_constraint_list ":" sync_constraint
{
  if ($3 != nullptr)
  $1.push_back($3);
  $$ = std::move($1);
}
;


sync_constraint:
TOK_ID "@" TOK_ID sync_strength
{
  $$ = nullptr;
  auto const * proc = system_declaration->get_process_declaration($1);
  if (proc == nullptr)
    std::cerr << tchecker::log_error << @1 << " process " << $1 << " is not declared" << std::endl;
  else {
    auto const * event = system_declaration->get_event_declaration($3);
    if (event == nullptr)
      std::cerr << tchecker::log_error << @3 << " event " << $3 << " is not declared" << std::endl;
    else {
      try {
        $$ = new tchecker::parsing::sync_constraint_t(*proc, *event, $4);
      }
      catch (std::exception const & e) {
        std::cerr << tchecker::log_error << @$ << " " << e.what() << std::endl;
      }
    }
  }
}
;


sync_strength:
"?"
{ $$ = tchecker::SYNC_WEAK; }
|
{ $$ = tchecker::SYNC_STRONG; }
;


eol_sequence:
eol_sequence TOK_EOL
|
;


integer:
TOK_INTEGER
{
  try {
    long long l = std::stoll($1, nullptr, 10);
    if ( (l < std::numeric_limits<tchecker::integer_t>::min()) || (l > std::numeric_limits<tchecker::integer_t>::max()) )
      throw std::out_of_range("integer constant our of range");
    $$ = static_cast<tchecker::integer_t>(l);
  }
  catch (std::exception const & e) {
    std::cerr << tchecker::log_error << @1 << " value " << $1 << " out of range ";
    std::cerr << std::numeric_limits<tchecker::integer_t>::min() << "," << std::numeric_limits<tchecker::integer_t>::max();
    std::cerr << std::endl;
    $$ = 0;
  }
}
;


uinteger:
TOK_INTEGER
{
  try {
    unsigned long l = std::stoul($1, nullptr, 10);
    if (l > std::numeric_limits<unsigned int>::max())
      throw std::out_of_range("unsigned integer out of range");
    $$ = static_cast<unsigned int>(l);
  }
  catch (std::exception const & e) {
    std::cerr << tchecker::log_error << @1 << " value " << $1 << " out of range ";
    std::cerr << std::numeric_limits<unsigned int>::min() << "," << std::numeric_limits<unsigned int>::max();
    std::cerr << std::endl;
    $$ = 0;
  }
}
;


%%

void tchecker::parsing::system::parser_t::error(location_type const & l, std::string const & msg)
{
  std::cerr << tchecker::log_error << l << " " << msg << std::endl;
}
