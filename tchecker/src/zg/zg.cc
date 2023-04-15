/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include "tchecker/zg/zg.hh"
#include "tchecker/dbm/db.hh"

namespace tchecker {

namespace zg {

/* Semantics functions */

tchecker::state_status_t initial(tchecker::ta::system_t const & system,
                                 tchecker::intrusive_shared_ptr_t<tchecker::shared_vloc_t> const & vloc,
                                 tchecker::intrusive_shared_ptr_t<tchecker::shared_intval_t> const & intval,
                                 tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_zone_t> const & zone,
                                 tchecker::intrusive_shared_ptr_t<tchecker::shared_vedge_t> const & vedge,
                                 tchecker::clock_constraint_container_t & invariant, 
                                 tchecker::zg::semantics_t & semantics,
                                 tchecker::zg::extrapolation_t & extrapolation,
                                 tchecker::zg::initial_value_t const & initial_range)
{
  tchecker::state_status_t status = tchecker::ta::initial(system, vloc, intval, vedge, invariant, initial_range);
  if (status != tchecker::STATE_OK)
    return status;

  tchecker::dbm::db_t * dbm = zone->dbm();
  tchecker::clock_id_t dim = zone->dim();
  bool delay_allowed = tchecker::ta::delay_allowed(system, *vloc);

  //ani:-100
  status = semantics.initial(dbm, dim, delay_allowed, invariant, system.history_clock_id_map, system.prophecy_clock_id_map, system.normal_clock_id_map);

  if (status != tchecker::STATE_OK)
    return status;

  extrapolation.extrapolate(dbm, dim, *vloc);

  return tchecker::STATE_OK;
}

tchecker::state_status_t next(tchecker::ta::system_t const & system,
                              tchecker::intrusive_shared_ptr_t<tchecker::shared_vloc_t> const & vloc,
                              tchecker::intrusive_shared_ptr_t<tchecker::shared_intval_t> const & intval,
                              tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_zone_t> const & zone,
                              tchecker::intrusive_shared_ptr_t<tchecker::shared_vedge_t> const & vedge,
                              tchecker::clock_constraint_container_t & src_invariant,
                              tchecker::clock_constraint_container_t & guard, 
                              tchecker::clock_reset_container_t & reset,
                              tchecker::clock_constraint_container_t & tgt_invariant, 
                              tchecker::zg::semantics_t & semantics,
                              tchecker::zg::extrapolation_t & extrapolation, 
                              tchecker::zg::outgoing_edges_value_t const & edges)
{
  bool src_delay_allowed = tchecker::ta::delay_allowed(system, *vloc);
  // std::cout << "ani:701 " << std::endl;
  // for(auto tmp_g:guard){
  //   std::cout << "ani:51 " << tmp_g.value() << std::endl;
  // }
  // //ani:-103 this next function populates the allocator with appropriate constraints!  
  tchecker::state_status_t status =
      tchecker::ta::next(system, vloc, intval, vedge, src_invariant, guard, reset, tgt_invariant, edges);
  if (status != tchecker::STATE_OK)
    return status;

  // std::cout << "ani:801 " << std::endl;

  // std::cout << "ani:555 " << std::endl;
  // for(auto tmp_g:guard){
  //   std::cout << "ani:556 " << tmp_g.value() << std::endl;
  // }
  tchecker::dbm::db_t * dbm = zone->dbm();
  tchecker::clock_id_t dim = zone->dim();
  bool tgt_delay_allowed = tchecker::ta::delay_allowed(system, *vloc);

  //ani:-100
  tchecker::system::edge_const_shared_ptr_t tmp_edge; 
  for (tchecker::system::edge_const_shared_ptr_t const & edge : edges){
    // std::cout << "all outgoing edges:" << edge->src() << " " << edge->tgt() << " " << edge->event_id() << " " << system.event_name(edge->event_id()) << std::endl;
    // std::cout << "one outgoing edge:"  << *edges.begin() << std::endl;
    tmp_edge = edge;
  }
  // std::cout << "ani:802\n";
  //ani:-100
  //status = semantics.next(dbm, dim, src_delay_allowed, src_invariant, guard, reset, tgt_delay_allowed, tgt_invariant,system.used_history_clocks_ids,system.used_history_clock_events,system.used_prophecy_clocks_ids,system.used_prophecy_clock_events,system.normal_clock_ids,system.event_name(tmp_edge->event_id()));
  status = semantics.next(dbm, dim, src_delay_allowed, src_invariant, guard, reset, tgt_delay_allowed, tgt_invariant,system.history_clock_id_map,system.prophecy_clock_id_map,system.normal_clock_id_map);
  // status = semantics.next(dbm, dim, src_delay_allowed, src_invariant, guard, reset, tgt_delay_allowed, tgt_invariant,system.history_clock_ids,system.history_clock_events,system.prophecy_clock_ids,system.prophecy_clock_events,system.normal_clock_ids);
  
  // std::cout << "ani:901 " << std::endl;
  if (status != tchecker::STATE_OK)
    return status;

  // std::cout << "ani:902 " << std::endl;
  
  extrapolation.extrapolate(dbm, dim, *vloc);
  // std::cout << "ani:903 " << std::endl;
  
  return tchecker::STATE_OK;
}

/* labels */

bool satisfies(tchecker::ta::system_t const & system, tchecker::zg::state_t const & s, boost::dynamic_bitset<> const & labels)
{
  return !s.zone().is_empty() && tchecker::ta::satisfies(system, s, labels);
}

//is this final eca
//ani:-100
/*bool eca_final(tchecker::zg::state_t const & s,std::vector<int> history_clock_ids, std::vector<int> prophecy_clock_ids, std::vector<int> normal_clock_ids){
  return s.zone().eca_is_final(history_clock_ids,prophecy_clock_ids,normal_clock_ids);
}//may need to change in the caller?*/


/* attributes */

void attributes(tchecker::ta::system_t const & system, tchecker::zg::state_t const & s, std::map<std::string, std::string> & m)
{
  tchecker::ta::attributes(system, s, m);
  m["zone"] = tchecker::to_string(s.zone(), system.clock_variables().flattened().index());
}

void attributes(tchecker::ta::system_t const & system, tchecker::zg::transition_t const & t,
                std::map<std::string, std::string> & m)
{
  tchecker::ta::attributes(system, t, m);
}

/* zg_t */

zg_t::zg_t(std::shared_ptr<tchecker::ta::system_t const> const & system,
           std::unique_ptr<tchecker::zg::semantics_t> && semantics,
           std::unique_ptr<tchecker::zg::extrapolation_t> && extrapolation, std::size_t block_size)
    : _system(system), _semantics(std::move(semantics)), _extrapolation(std::move(extrapolation)),
      _state_allocator(block_size, block_size, _system->processes_count(), block_size,
                       _system->intvars_count(tchecker::VK_FLATTENED), block_size,
                       _system->clocks_count(tchecker::VK_FLATTENED) + 1),
      _transition_allocator(block_size, block_size, _system->processes_count())
{
}

//ani:-100
/*zg_t::zg_t(std::shared_ptr<tchecker::ta::system_t const> const & system,
           std::unique_ptr<tchecker::zg::semantics_t> && semantics,
           std::unique_ptr<tchecker::zg::extrapolation_t> && extrapolation, std::size_t block_size)
    : _system(system), _semantics(std::move(semantics)), _extrapolation(std::move(extrapolation)),
      _state_allocator(block_size, block_size, _system->processes_count(), block_size,
                       _system->intvars_count(tchecker::VK_FLATTENED), block_size,
                       (system->normal_clock_ids.size()+system->used_history_clocks_ids.size()+system->used_prophecy_clocks_ids.size()) + 1),
      _transition_allocator(block_size, block_size, _system->processes_count())
{
}*/

/*zg_t::zg_t(std::shared_ptr<tchecker::ta::system_t const> const & system,
           std::unique_ptr<tchecker::zg::semantics_t> && semantics,
           std::unique_ptr<tchecker::zg::extrapolation_t> && extrapolation, std::size_t block_size)
    : _system(system), _semantics(std::move(semantics)), _extrapolation(std::move(extrapolation)),
      _state_allocator(block_size, block_size, _system->processes_count(), block_size,
                       (system->normal_clock_ids.size()+system->used_history_clocks_ids.size()+system->used_prophecy_clocks_ids.size()), block_size,
                       (system->normal_clock_ids.size()+system->used_history_clocks_ids.size()+system->used_prophecy_clocks_ids.size()) + 1),
      _transition_allocator(block_size, block_size, _system->processes_count())
{
}*/

tchecker::zg::initial_range_t zg_t::initial_edges() { return tchecker::zg::initial_edges(*_system); }

void zg_t::initial(tchecker::zg::initial_value_t const & init_edge, std::vector<sst_t> & v)
{
  tchecker::zg::state_sptr_t s = _state_allocator.construct();
  tchecker::zg::transition_sptr_t t = _transition_allocator.construct();
  
  tchecker::state_status_t status = tchecker::zg::initial(*_system, *s, *t, *_semantics, *_extrapolation, init_edge);
  v.push_back(std::make_tuple(status, s, t));
}

tchecker::zg::outgoing_edges_range_t zg_t::outgoing_edges(tchecker::zg::const_state_sptr_t const & s)
{
  return tchecker::zg::outgoing_edges(*_system, s->vloc_ptr());
}

void zg_t::next(tchecker::zg::const_state_sptr_t const & s, tchecker::zg::outgoing_edges_value_t const & out_edge,
                std::vector<sst_t> & v)
{
  tchecker::zg::state_sptr_t nexts = _state_allocator.clone(*s);
  tchecker::zg::transition_sptr_t t = _transition_allocator.construct();
  // std::cout << "ani:49 " << std::endl;
  // for(auto tmp_guard2:t->guard_container()){
  //   std::cout << "ani:48 " << tmp_guard2.value();
  // }
  tchecker::state_status_t status = tchecker::zg::next(*_system, *nexts, *t, *_semantics, *_extrapolation, out_edge);
  v.push_back(std::make_tuple(status, nexts, t));
}

//ani:-100
bool zg_t::satisfies(tchecker::zg::const_state_sptr_t const & s, boost::dynamic_bitset<> const & labels)
{
  //return tchecker::zg::satisfies(*_system, *s, labels) && tchecker::zg::eca_final(*s,_system->history_clock_ids,_system->prophecy_clock_ids,_system->normal_clock_ids);;
  // std::cout << "ani:-343 " << (s->zone().dbm_ptr()==s->zone().dbm()) << std::endl;
  return tchecker::zg::satisfies(*_system, *s, labels) && _semantics->is_final_dbm(s->zone().dbm(),s->zone().dim(),_system->history_clock_id_map,_system->prophecy_clock_id_map,_system->normal_clock_id_map);
}

void zg_t::attributes(tchecker::zg::const_state_sptr_t const & s, std::map<std::string, std::string> & m)
{
  tchecker::zg::attributes(*_system, *s, m);
}

void zg_t::attributes(tchecker::zg::const_transition_sptr_t const & t, std::map<std::string, std::string> & m)
{
  tchecker::zg::attributes(*_system, *t, m);
}

tchecker::ta::system_t const & zg_t::system() const { return *_system; }

/* factory */

tchecker::zg::zg_t * factory(std::shared_ptr<tchecker::ta::system_t const> const & system,
                             enum tchecker::zg::semantics_type_t semantics_type,
                             enum tchecker::zg::extrapolation_type_t extrapolation_type, std::size_t block_size)
{
  std::unique_ptr<tchecker::zg::extrapolation_t> extrapolation{
      tchecker::zg::extrapolation_factory(extrapolation_type, *system)};
  if (extrapolation.get() == nullptr)
    return nullptr;
  std::unique_ptr<tchecker::zg::semantics_t> semantics{tchecker::zg::semantics_factory(semantics_type)};
  return new tchecker::zg::zg_t(system, std::move(semantics), std::move(extrapolation), block_size);
}

tchecker::zg::zg_t * factory(std::shared_ptr<tchecker::ta::system_t const> const & system,
                             enum tchecker::zg::semantics_type_t semantics_type,
                             enum tchecker::zg::extrapolation_type_t extrapolation_type,
                             tchecker::clockbounds::clockbounds_t const & clock_bounds, std::size_t block_size)
{
  std::unique_ptr<tchecker::zg::extrapolation_t> extrapolation{
      tchecker::zg::extrapolation_factory(extrapolation_type, clock_bounds)};
  if (extrapolation.get() == nullptr)
    return nullptr;
  std::unique_ptr<tchecker::zg::semantics_t> semantics{tchecker::zg::semantics_factory(semantics_type)};
  return new tchecker::zg::zg_t(system, std::move(semantics), std::move(extrapolation), block_size);
}

} // end of namespace zg

} // end of namespace tchecker
