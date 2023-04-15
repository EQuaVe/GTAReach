/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <cassert>
#include <iostream>
#include <memory>
#include <string>

#include "tchecker/clockbounds/solver.hh"
#include "tchecker/expression/expression.hh"
#include "tchecker/expression/type_inference.hh"
#include "tchecker/expression/typechecking.hh"
#include "tchecker/expression/static_analysis.hh"//ani:-100

#include "tchecker/parsing/parsing.hh"
#include "tchecker/statement/statement.hh"
#include "tchecker/statement/typechecking.hh"
#include "tchecker/ta/static_analysis.hh"
#include "tchecker/ta/system.hh"
#include "tchecker/utils/log.hh"
#include "tchecker/vm/compilers.hh"

namespace tchecker {

namespace ta {


//ani:-100
// class guard_extractor_t : public tchecker::typed_expression_visitor_t, 
//                                public tchecker::typed_statement_visitor_t
//   {
//     public:
//       /*!
//       \brief Constructor
//       tchecker::integer_t * _max
//       */
//       guard_extractor_t() : _used_clocks()
//       {
//       }

//       /*!
//       \brief Copy constructor
//       \param updater : an updater
//       \post this is a copy of updater
//       */
//       guard_extractor_t(tchecker::ta::guard_extractor_t const & optimizer): _used_clocks(optimizer._used_clocks)
//       {
//       }

//       /*!
//       \brief Move constructor
//       \param updater : an updater
//       \post updater has been moved to this
//       */
//       guard_extractor_t(tchecker::ta::guard_extractor_t && optimizer): _used_clocks(std::move(optimizer._used_clocks))
//       {
//       }

//       /*!
//       \brief Destructor
//       */
//       ~guard_extractor_t() = default;

//       /*!
//       \brief Visitor
//       \post sub-expression has been visited
//       */
//       virtual void visit(tchecker::typed_par_expression_t const & expr) { 
//         // std::cout << "ani:6604 \n";
//         expr.expr().visit(*this); 
//       }

//       /*!
//       \brief Visitor
//       \post expr is added to Gdf
//       */
//       virtual void visit(tchecker::typed_simple_clkconstr_expression_t const & expr)
//       {
//         // std::cout << "ani:6603 \n";
//         _used_clocks.emplace(tchecker::extract_lvalue_variable_ids(expr.clock()).begin());
//         // std::cout << "ani:6607 " << _used_clocks << std::endl;

//       }

//       /*!
//       \brief Visitor
//       */
//       virtual void visit(tchecker::typed_diagonal_clkconstr_expression_t const & expr)
//       {
//         // std::cout << "ani:6602 \n";
//       }

//       /*!
//       \brief Visitor
//       */
//       virtual void visit(tchecker::typed_binary_expression_t const & expr) 
//       { 
//         // std::cout << "ani:6601 \n";// << stmt.first() << " " << stmt.second() << std::endl;
//         tchecker::ta::guard_extractor_t extractor1;
//         tchecker::ta::guard_extractor_t extractor2;

//         expr.left_operand().visit(extractor1);
//         for(auto tmp:extractor1._used_clocks)
//           _used_clocks.emplace(tmp);
//         // std::cout << "ani:66001 visited left, now visiting right!\n";
//         expr.right_operand().visit(extractor2);
//         for(auto tmp:extractor2._used_clocks)
//           _used_clocks.emplace(tmp);
//       }

//       // Other visitors on expressions
//       virtual void visit(tchecker::typed_int_expression_t const &) { }
//       virtual void visit(tchecker::typed_var_expression_t const &) { }
//       virtual void visit(tchecker::typed_bounded_var_expression_t const &) { }
//       virtual void visit(tchecker::typed_array_expression_t const &) { }
//       virtual void visit(tchecker::typed_unary_expression_t const &) { }
//       virtual void visit(tchecker::typed_ite_expression_t const &) { }

//       /*!
//       \brief Visitor
//       \post first and second statements have been visited
//       */
//       virtual void visit(tchecker::typed_sequence_statement_t const & stmt)
//       {}

//       /*!
//       \brief Visitor
//       \post For assignment x:=y+c, a constraint for every clock x (x could be an array) and every clock y (y could be
//       an array) using value c has been added to _solver using method add_assignment
//       */
//       virtual void visit(tchecker::typed_sum_to_clock_assign_statement_t const & stmt)
//       {
//       }

//       virtual void visit(tchecker::typed_int_to_clock_assign_statement_t const & stmt) 
//       {}

//       // Other visitors on statements
//       virtual void visit(tchecker::typed_if_statement_t const & stmt) {}
//       virtual void visit(tchecker::typed_while_statement_t const & stmt) {}
//       virtual void visit(tchecker::typed_clock_to_clock_assign_statement_t const &) {}
//       virtual void visit(tchecker::typed_nop_statement_t const &) {}
//       virtual void visit(tchecker::typed_assign_statement_t const &) {}
//       virtual void visit(tchecker::typed_local_var_statement_t const &) {}
//       virtual void visit(tchecker::typed_local_array_statement_t const &) {}
    
//     public:
//       std::set<int> _used_clocks;
//   };

system_t::system_t(tchecker::parsing::system_declaration_t const & sysdecl) : tchecker::syncprod::system_t(sysdecl)
{
  compute_from_syncprod_system();
  
  for(auto i:this->history_clock_ids){
    this->history_clock_id_map.insert(i+1);
  }
  for(auto i:this->prophecy_clock_ids){
    if (i!=0)
      this->prophecy_clock_id_map.insert(i+1);
  }
  for(auto i:this->normal_clock_ids){
    this->normal_clock_id_map.insert(i+1);
    this->history_clock_id_map.erase(i+1); //make normal clocks and history clocks disjoint!!
  }
  
  // std::cout << "Normal_clock_ids: ";
  // for(auto i:this->normal_clock_id_map){
  //   std::cout << i << ", ";
  // }
  // std::cout << std::endl;
  
  // std::cout << "Prophecy_clock_ids: ";
  // for(auto i:this->prophecy_clock_ids){
  //   std::cout << i << ", ";
  // }
  // std::cout << std::endl;
  
  // std::cout << "History_clock_ids: ";
  // for(auto i:this->history_clock_ids){
  //   std::cout << i << ", ";
  // }
  // std::cout << std::endl;



  // std::cout << " ani:00000000: system_t" << std::endl;
  
  /*
  //ani:-100
  for(auto tmp:this->edges()){
    // std::cout << "ani:580 edge:" << tmp->id() << " " << this->guard(tmp->id()) << std::endl;
    
    tchecker::ta::guard_extractor_t guard_extractor;
    
    // std::cout << "ani:6590 \n";
    this->guard(tmp->id()).visit(guard_extractor);
    
    for(auto tmp:guard_extractor._used_clocks){
      // used_history_clocks_ids

      auto history_clock_iter = std::find(this->history_clock_ids.begin(),this->history_clock_ids.end(),tmp);
      
      if(history_clock_iter!=this->history_clock_ids.end()){
        auto history_clock_iter_tmp = std::find(this->used_history_clocks_ids.begin(),this->used_history_clocks_ids.end(),*history_clock_iter);
        
        if(history_clock_iter_tmp==this->used_history_clocks_ids.end()){
          this->used_history_clocks_ids.push_back(*history_clock_iter);
          int event_position = history_clock_iter-history_clock_ids.begin();
          this->used_history_clock_events.push_back(this->history_clock_events[event_position]);
        }
      
      }
      

      auto prophecy_clock_iter = std::find(this->prophecy_clock_ids.begin(),this->prophecy_clock_ids.end(),tmp);
      if(prophecy_clock_iter!=this->prophecy_clock_ids.end()){
        auto prophecy_clock_iter_tmp = std::find(this->used_prophecy_clocks_ids.begin(),this->used_prophecy_clocks_ids.end(),*prophecy_clock_iter);
        if(prophecy_clock_iter_tmp==this->used_prophecy_clocks_ids.end()){
          this->used_prophecy_clocks_ids.push_back(*prophecy_clock_iter);
          int event_position = prophecy_clock_iter-prophecy_clock_ids.begin();
          this->used_prophecy_clock_events.push_back(this->prophecy_clock_events[event_position]);        
        }
      }
      // std::cout << "ani:6608 " << tmp << std::endl;

      

    }
      

  }*/

  /*
  events_prop_ids.clear();
  events_hist_ids.clear();
  std::cout << "11111111\n";
  for(int i=0;i<this->prophecy_clock_ids.size();i++){
    std::cout << "propclk started string" << this->prophecy_clock_events[i]  << "string ended"<< std::endl;
    int eve_id = this->event_id(this->prophecy_clock_events[i]); //there is no empty event!!!
    int pro_id = this->prophecy_clock_ids[i];

    this->events_prop_ids.push_back(std::make_tuple(eve_id, pro_id));
  }
  std::cout << "11111111 end\n";

  for(int i=0;i<this->history_clock_ids.size();i++){
    int eve_id = this->event_id(this->history_clock_events[i]);
    int his_id = this->history_clock_ids[i];

    this->events_hist_ids.push_back(std::make_tuple(eve_id, his_id));
  }

  for(auto i:events_hist_ids){
    std::cout << "ani:---1 " << std::get<0>(i) << " " << std::get<1>(i) << std::endl;
  }

  for(auto tmp1:this->history_clock_ids){
    auto history_clock_iter_tmp = std::find(this->used_history_clocks_ids.begin(),this->used_history_clocks_ids.end(),tmp1);
    if(history_clock_iter_tmp==used_history_clocks_ids.end())
      un_used_history_clocks_ids.push_back(tmp1);
  }

  for(auto tmp1:this->prophecy_clock_ids){
    auto prophecy_clock_iter_tmp = std::find(this->used_prophecy_clocks_ids.begin(),this->used_prophecy_clocks_ids.end(),tmp1);
    if(prophecy_clock_iter_tmp==used_prophecy_clocks_ids.end())
      un_used_prophecy_clocks_ids.push_back(tmp1);
  }
  
  */
  /*
  std::cout << "used history clock ids\n";
  for(auto tmp:this->used_history_clocks_ids){
    std::cout << "ani:582 " << tmp << ", ";
  }
  std::cout << std::endl;
  std::cout << "used history clock events\n";
  for(auto tmp:this->used_history_clock_events){
    std::cout << "ani:583 " << tmp << ", ";
  }
  std::cout << std::endl;
  std::cout << std::endl;  

  std::cout << "unused history clock ids\n";
  for(auto tmp:this->un_used_history_clocks_ids){
    std::cout << "ani:584 " << tmp << ", ";
  }
  std::cout << std::endl;
  std::cout << std::endl;


  std::cout << "used prophecy clock ids\n";
  for(auto tmp:this->used_prophecy_clocks_ids){
    std::cout << "ani:585 " << tmp << ", ";
  }
  std::cout << std::endl;
  for(auto tmp:this->used_prophecy_clock_events){
    std::cout << "ani:586 " << tmp << ", ";
  }
  std::cout << std::endl;
  std::cout << std::endl;

  std::cout << "unused prophecy clock ids\n";
  for(auto tmp:this->un_used_prophecy_clocks_ids){
    std::cout << "ani:587 " << tmp << ", ";
  }
  std::cout << std::endl;
  */

  
}

system_t::system_t(tchecker::system::system_t const & system) : tchecker::syncprod::system_t(system)
{
  compute_from_syncprod_system();
}

system_t::system_t(tchecker::syncprod::system_t const & system) : tchecker::syncprod::system_t(system)
{
  compute_from_syncprod_system();
}

system_t::system_t(tchecker::ta::system_t const & system)
    : tchecker::syncprod::system_t(system.as_syncprod_system()), _vm(system._vm)
{
  compute_from_syncprod_system();
}

tchecker::ta::system_t & system_t::operator=(tchecker::ta::system_t const & system)
{
  if (this != &system) {
    tchecker::syncprod::system_t::operator=(system);
    _vm = system._vm;
    compute_from_syncprod_system();
  }
  return *this;
}

tchecker::typed_expression_t const & system_t::guard(tchecker::edge_id_t id) const
{
  assert(is_edge(id));

  // std::cout << "ani:6 " << *(_guards[id]._typed_expr) << " " << _guards[id]._typed_expr->type() <<" " << *_guards[id]._compiled_expr << std::endl;
  return *_guards[id]._typed_expr;
}

tchecker::bytecode_t const * system_t::guard_bytecode(tchecker::edge_id_t id) const
{
  assert(is_edge(id));
  return _guards[id]._compiled_expr.get();
}

tchecker::typed_statement_t const & system_t::statement(tchecker::edge_id_t id) const
{
  assert(is_edge(id));
  return *_statements[id]._typed_stmt;
}

tchecker::bytecode_t const * system_t::statement_bytecode(tchecker::edge_id_t id) const
{
  assert(is_edge(id));
  return _statements[id]._compiled_stmt.get();
}

bool system_t::is_urgent(tchecker::loc_id_t id) const
{
  assert(is_location(id));
  return _urgent[id] == 1;
}

tchecker::typed_expression_t const & system_t::invariant(tchecker::loc_id_t id) const
{
  assert(is_location(id));
  return *_invariants[id]._typed_expr;
}

tchecker::bytecode_t const * system_t::invariant_bytecode(tchecker::loc_id_t id) const
{
  assert(is_location(id));
  return _invariants[id]._compiled_expr.get();
}

void system_t::compute_from_syncprod_system()
{
  _invariants.clear();
  _guards.clear();
  _statements.clear();
  _urgent.reset();

  tchecker::loc_id_t const locations_count = this->locations_count();
  tchecker::edge_id_t const edges_count = this->edges_count();

  _invariants.resize(locations_count);
  _guards.resize(edges_count);
  _statements.resize(edges_count);
  _urgent.resize(locations_count);

  for (tchecker::loc_id_t id = 0; id < locations_count; ++id) {
    auto const & attr = tchecker::syncprod::system_t::location(id)->attributes();
    set_invariant(id, attr.values("invariant"));
    set_urgent(id, attr.values("urgent"));
  }

  for (tchecker::edge_id_t id = 0; id < edges_count; ++id) {
    auto const & attr = tchecker::syncprod::system_t::edge(id)->attributes();
    // std::cout << "ani13 " << attr.values("provided") << std::endl;
    for(auto tmp:attr.values("provided")){
      // std::cout << "ani:14 " << tmp.first << " " << tmp.second << std::endl;

      std::string context = "In process " + tchecker::syncprod::system_t::process_name(edge(id)->pid()) + ", edge from " +
                        tchecker::syncprod::system_t::location(edge(id)->src())->name() + " to " +
                        tchecker::syncprod::system_t::location(edge(id)->tgt())->name() + " with event " +
                        tchecker::syncprod::system_t::event_name(edge(id)->event_id()) + ", provided";
      
      // auto *tmp_attr = new tchecker::parsing::attr_t(tmp.first, tmp.second, std::string(""), std::string(""));
      
      // tchecker::parsing::attributes_t tmp_attrs;
      // tmp_attrs.insert(tmp_attr);

      // tchecker:system::attributes_t tmp_attrs_system(tmp_attrs);
      tchecker::expression_t * value_expr = tchecker::parsing::parse_expression(context, tmp.second);
      // std::cout << "ani:15 " << value_expr->to_string() << std::endl;
      // for(auto tmp:value_expr){
      //   std::cout << "ani:16 " << tmp << std::endl;
      // }
    }
    set_guards(id, attr.values("provided"));
    set_statements(id, attr.values("do"));
  }

  if (tchecker::ta::has_guarded_weakly_synchronized_event(*this))
    throw std::invalid_argument("Transitions over weakly synchronized events should not have guards");
}

static tchecker::expression_t *
conjunction_from_attrs(tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & attrs,
                       std::string const & context)
{
  tchecker::expression_t * expr = nullptr;

  for (auto && [key, value] : attrs) {
    tchecker::expression_t * value_expr = tchecker::parsing::parse_expression(context, value);
    // std::cout << "ani:12 " << value_expr->to_string() << std::endl;
    if (value_expr == nullptr) {
      delete expr;
      return nullptr;
    }

    if (expr != nullptr)
      expr = new tchecker::binary_expression_t(tchecker::EXPR_OP_LAND, expr, value_expr);
    else
      expr = value_expr;
  }

  // empty list of attributes
  if (expr == nullptr)
    expr = new tchecker::int_expression_t(1);

  return expr;
}

void system_t::set_invariant(tchecker::loc_id_t id,
                             tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & invariants)
{
  std::string context = "In process " + tchecker::syncprod::system_t::process_name(location(id)->pid()) + ", location " +
                        tchecker::syncprod::system_t::location(id)->name() + ", invariant";

  tchecker::integer_variables_t localvars;

  std::shared_ptr<tchecker::expression_t> invariant_expr{conjunction_from_attrs(invariants, context)};
  if (invariant_expr.get() == nullptr)
    throw std::invalid_argument("Syntax error");

  std::shared_ptr<tchecker::typed_expression_t> invariant_typed_expr{
      tchecker::typecheck(*invariant_expr, localvars, integer_variables(), clock_variables())};

  if (!tchecker::bool_valued(invariant_typed_expr->type())) {
      std::stringstream oss;
      oss << context << " expression is not bool valued";
      throw std::invalid_argument (oss.str ());
  }

  try {
    std::shared_ptr<tchecker::bytecode_t> invariant_bytecode{tchecker::compile(*invariant_typed_expr),
                                                             std::default_delete<tchecker::bytecode_t[]>()};
    _invariants[id] = {invariant_typed_expr, invariant_bytecode};
  }
  catch (std::exception const & e) {
    std::stringstream oss;
    oss << context << " " << e.what();
    throw std::invalid_argument (oss.str ());
  }
}

void system_t::set_urgent(tchecker::loc_id_t id,
                          tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & flags)
{
  if (flags.begin() != flags.end())
    _urgent[id] = 1;
}

void system_t::set_guards(tchecker::edge_id_t id,
                          tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & guards)
{
  std::string context = "In process " + tchecker::syncprod::system_t::process_name(edge(id)->pid()) + ", edge from " +
                        tchecker::syncprod::system_t::location(edge(id)->src())->name() + " to " +
                        tchecker::syncprod::system_t::location(edge(id)->tgt())->name() + " with event " +
                        tchecker::syncprod::system_t::event_name(edge(id)->event_id()) + ", provided";

  tchecker::integer_variables_t localvars;

  std::shared_ptr<tchecker::expression_t> guard_expr{conjunction_from_attrs(guards, context)};
  // std::cout << "ani:11 " << guard_expr->to_string() << std::endl;
  // std::cout << "ani:12 " << guard_expr-> << std::endl;
  
  if (guard_expr.get() == nullptr)
    throw std::invalid_argument("Syntax error");

  std::shared_ptr<tchecker::typed_expression_t> guard_typed_expr{
      tchecker::typecheck(*guard_expr, localvars, integer_variables(), clock_variables())};

  if (!tchecker::bool_valued (guard_typed_expr->type ())) {
    std::stringstream oss;
    oss << context << " expression is not bool valued";
    throw std::invalid_argument (oss.str ());
  }

  try {
    std::shared_ptr<tchecker::bytecode_t> guard_bytecode{tchecker::compile(*guard_typed_expr),
                                                         std::default_delete<tchecker::bytecode_t[]>()};
    _guards[id] = {guard_typed_expr, guard_bytecode};
  }
  catch (std::exception const & e) {
    std::stringstream oss;
    oss << context << " " << e.what();
    throw std::invalid_argument (oss.str ());
  }
}

static tchecker::statement_t *
sequence_from_attrs(tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & attrs,
                    std::string const & context)
{
  tchecker::statement_t * stmt = nullptr;

  for (auto && [key, value] : attrs) {
    tchecker::statement_t * value_stmt = tchecker::parsing::parse_statement(context, value);

    if (value_stmt == nullptr) {
      delete stmt;
      return nullptr;
    }

    if (stmt != nullptr)
      stmt = new tchecker::sequence_statement_t(stmt, value_stmt);
    else
      stmt = value_stmt;
  }

  // empty list of statements
  if (stmt == nullptr)
    stmt = new tchecker::nop_statement_t;

  return stmt;
}

void system_t::set_statements(tchecker::edge_id_t id,
                              tchecker::range_t<tchecker::system::attributes_t::const_iterator_t> const & statements)
{
  std::string context = "In process " + tchecker::syncprod::system_t::process_name(edge(id)->pid()) + ", edge from " +
                        tchecker::syncprod::system_t::location(edge(id)->src())->name() + " to " +
                        tchecker::syncprod::system_t::location(edge(id)->tgt())->name() + " with event " +
                        tchecker::syncprod::system_t::event_name(edge(id)->event_id()) + ", do";

  tchecker::integer_variables_t localvars;

  std::shared_ptr<tchecker::statement_t> stmt{sequence_from_attrs(statements, context)};
  if (stmt.get() == nullptr)
    throw std::invalid_argument("Syntax error");


  std::shared_ptr<tchecker::typed_statement_t> typed_stmt{
      tchecker::typecheck(*stmt, localvars, integer_variables(), clock_variables(),
                          [](std::string const &e) {
                            std::cerr << tchecker::log_error << e << std::endl;
                          })};

  try {
    std::shared_ptr<tchecker::bytecode_t> bytecode{tchecker::compile(*typed_stmt),
                                                   std::default_delete<tchecker::bytecode_t[]>()};
    _statements[id] = {typed_stmt, bytecode};
  }
  catch (std::exception const & e) {
    std::stringstream oss;
    oss << context << " " << e.what();
    throw std::invalid_argument(oss.str());
  }
}

} // end of namespace ta

} // end of namespace tchecker
