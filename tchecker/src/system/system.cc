/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <limits>
#include <vector>

#include "tchecker/system/system.hh"

namespace tchecker {

namespace system {

/*!
 \class system_builder_t
 \brief builds a system from a system declaration
 */
class system_builder_t : public tchecker::parsing::declaration_visitor_t {
public:
  /*!
   \brief Constructor
   */
  system_builder_t(tchecker::system::system_t & system) : _system(system) {}

  /*!
   \brief Destructor
   */
  virtual ~system_builder_t() = default;

  /*!
   \brief Build system from inner declarations
   */
  virtual void visit(tchecker::parsing::system_declaration_t const & d)
  {
    for (tchecker::parsing::declaration_t const * decl : d.declarations())
      decl->visit(*this);
  }

  /*!
   \brief Add clock variable
   */
  virtual void visit(tchecker::parsing::clock_declaration_t const & d)
  {
    if (std::numeric_limits<tchecker::clock_id_t>::max() < d.size())
      throw std::out_of_range("clock size cannot be represented");
    tchecker::clock_id_t size = d.size();
    _system.add_clock(d.name(), size, d.attributes());
    
    //ani:-100
    //ani:-102 add try error block here!
    // std::cout << "Added clock " << d.name() << std::endl;;
    
    if(d.clock_type()==1){ //clock type =1 denotes history clock
      // std::cout << "ani:-2234 " << "history clock " << d.name() << std::endl;
      _system.history_clock_ids.push_back(_system.clock_id(d.name()));
      _system.history_clock_events.push_back(d.event_name());
    }
    else if(d.clock_type()==2){ //clock type =2 denotes prophecy clock
      // std::cout << "ani:-2235 " << "prophecy clock " << d.name() << std::endl;
      _system.prophecy_clock_ids.push_back(_system.clock_id(d.name()));
      _system.prophecy_clock_events.push_back(d.event_name());
    }
    else{
      //normal clocks are a subset of history clocks with the difference in initial zone
      // std::cout << "ani:-2236 " << "normal clock " << d.name() << std::endl;
      _system.history_clock_ids.push_back(_system.clock_id(d.name()));
      _system.history_clock_events.push_back(d.event_name());
      _system.normal_clock_ids.push_back(_system.clock_id(d.name()));
    }

  }

  /*!
   \brief Add bounded integer variable
   */
  virtual void visit(tchecker::parsing::int_declaration_t const & d)
  {
    if (std::numeric_limits<tchecker::intvar_id_t>::max() < d.size())
      throw std::out_of_range("integer variable size cannot be represented");
    tchecker::intvar_id_t size = d.size();
    _system.add_intvar(d.name(), size, d.min(), d.max(), d.init(), d.attributes());
  }

  /*!
   \brief Add a process
   */
  virtual void visit(tchecker::parsing::process_declaration_t const & d) { /*std::cout << "Adding process decl " << d.name() << std::endl;;*/ _system.add_process(d.name(), d.attributes()); /*std::cout << "Added process decl " << d.name() << std::endl;*/}

  /*!
   \brief Add an event
   */
  virtual void visit(tchecker::parsing::event_declaration_t const & d) {  /*std::cout << "Adding event decl " << d.name() << std::endl;*/_system.add_event(d.name(), d.attributes()); /*std::cout << "Added event decl " << d.name() << std::endl;*/}

  /*!
   \brief Add a location
   */
  virtual void visit(tchecker::parsing::location_declaration_t const & d)
  {
    // std::cout << "Adding location decl " << d.name() << std::endl;    
    tchecker::process_id_t pid = _system.process_id(d.process().name());
    _system.add_location(pid, d.name(), d.attributes());
    // std::cout << "Added location decl " << d.name() << std::endl;
  }

  /*!
   \brief Add an edge
   */
  virtual void visit(tchecker::parsing::edge_declaration_t const & d)
  {

    tchecker::process_id_t pid = _system.process_id(d.process().name());
    tchecker::loc_id_t src = _system.location(pid, d.src().name())->id();
    tchecker::loc_id_t tgt = _system.location(pid, d.tgt().name())->id();
    tchecker::event_id_t event = _system.event_id(d.event().name());
    // std::cout << "Adding edge " << d.process().name() << " " << d.src().name() << " -> " << d.tgt().name() << " " << d.event().name() << std::endl;
    _system.add_edge(pid, src, tgt, event, d.attributes());
    // std::cout << "Added edge " << d.process().name() << " " << d.src().name() << " -> " << d.tgt().name() << " " << d.event().name() << std::endl;
    
  }

  /*!
   \brief Add a synchronization
   */
  virtual void visit(tchecker::parsing::sync_declaration_t const & d)
  {
    std::vector<tchecker::system::sync_constraint_t> v;
    for (tchecker::parsing::sync_constraint_t const * c : d.sync_constraints()) {
      tchecker::process_id_t pid = _system.process_id(c->process().name());
      tchecker::event_id_t event = _system.event_id(c->event().name());
      v.emplace_back(pid, event, c->strength());
    }

    _system.add_synchronization(v, d.attributes());
  }

private:
  tchecker::system::system_t & _system; /*!< System to build */
};

/* system_t */

system_t::system_t(std::string const & name, tchecker::system::attributes_t const & attr) : _name(name), _attr(attr) {}

system_t::system_t(tchecker::parsing::system_declaration_t const & sysdecl) : _name(sysdecl.name()), _attr(sysdecl.attributes())
{
  tchecker::system::system_builder_t builder(*this);
  sysdecl.visit(builder);
  // std::cout << "visited everything sysdecl.visit(builder) finished!" << std::endl;
}

void system_t::add_clock(std::string const & name, tchecker::clock_id_t size, tchecker::system::attributes_t const & attr)
{
  if (has_variable(name))
    throw std::invalid_argument("Variable is already declared");
  tchecker::system::clocks_t::add_clock(name, size, attr);
}

void system_t::add_edge(tchecker::process_id_t pid, tchecker::loc_id_t src, tchecker::loc_id_t tgt,
                        tchecker::event_id_t event_id, tchecker::system::attributes_t const & attr)
{
  if (!tchecker::system::processes_t::is_process(pid))
    throw std::invalid_argument("Process is not declared");
  if (!tchecker::system::locs_t::is_location(src))
    throw std::invalid_argument("Source location is not declared");
  if (!tchecker::system::locs_t::is_location(tgt))
    throw std::invalid_argument("Traget location is not declared");
  if (!tchecker::system::events_t::is_event(event_id))
    throw std::invalid_argument("Event is not declared");
  tchecker::system::edges_t::add_edge(pid, src, tgt, event_id, attr);
}

void system_t::add_intvar(std::string const & name, tchecker::intvar_id_t size, tchecker::integer_t min,
                          tchecker::integer_t max, tchecker::integer_t initial, tchecker::system::attributes_t const & attr)
{
  if (has_variable(name))
    throw std::invalid_argument("Variable is already declared");
  tchecker::system::intvars_t::add_intvar(name, size, min, max, initial, attr);
}

void system_t::add_location(tchecker::process_id_t pid, std::string const & name, tchecker::system::attributes_t const & attr)
{
  if (!tchecker::system::processes_t::is_process(pid))
    throw std::invalid_argument("Process is not declared");
  tchecker::system::locs_t::add_location(pid, name, attr);
}

void system_t::add_synchronization(std::vector<tchecker::system::sync_constraint_t> const & v,
                                   tchecker::system::attributes_t const & attr)
{
  for (tchecker::system::sync_constraint_t const & c : v) {
    if (!tchecker::system::processes_t::is_process(c.pid()))
      throw std::invalid_argument("Process id not declared");
    if (!tchecker::system::events_t::is_event(c.event_id()))
      throw std::invalid_argument("Event is not declared");
  }
  tchecker::system::synchronizations_t::add_synchronization(v, attr);
}

bool system_t::has_variable(std::string const & name)
{
  return tchecker::system::clocks_t::is_clock(name) || tchecker::system::intvars_t::is_intvar(name);
}

} // end of namespace system

} // end of namespace tchecker
