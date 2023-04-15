/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <boost/dynamic_bitset.hpp>

#include "tchecker/algorithms/search_order.hh"
#include "tchecker/ta/state.hh"
#include "zg-eca_gsim_gen.hh"

namespace tchecker {

namespace tck_reach {

namespace zg_eca_gsim_gen {

/* node_t */

node_t::node_t(tchecker::zg::state_sptr_t const & s) : _state(s) {}

node_t::node_t(tchecker::zg::const_state_sptr_t const & s) : _state(s) {}

/* node_hash_t */

std::size_t node_hash_t::operator()(tchecker::tck_reach::zg_eca_gsim_gen::node_t const & n) const
{
  // NB: we hash on the discrete part of the state in n to check all nodes
  // with same discrete part for covering
  return tchecker::ta::hash_value(n.state());
}

/* node_le_t */

node_le_t::node_le_t(tchecker::eca_amap_gen2::eca_a_map_t const & amap) : _amap(amap),
                                                            _G(new std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *>()),
                                                            _Gdf(new std::vector<tchecker::typed_simple_clkconstr_expression_t const *>()),
                                                            _history_clock_ids(*(new std::unordered_set<int> ())),
                                                            _prophecy_clock_ids(*(new std::unordered_set<int> ())),
                                                            _normal_clock_ids(*(new std::unordered_set<int> ()))
{}//ani:-100


node_le_t::node_le_t(tchecker::ta::system_t const & system) : _amap(*tchecker::eca_amap_gen2::compute_eca_amap(system)),
                                                              _G(new std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *>()),
                                                              _Gdf(new std::vector<tchecker::typed_simple_clkconstr_expression_t const *>()),
                                                              _history_clock_ids(system.history_clock_id_map),
                                                              _prophecy_clock_ids(system.prophecy_clock_id_map),
                                                              _normal_clock_ids(system.normal_clock_id_map)
                                                              
                                                              
{}//ani:-100

node_le_t::node_le_t(tchecker::tck_reach::zg_eca_gsim_gen::node_le_t const & node_le) : _amap(node_le._amap),
                                                                                    _G(node_le._G),
                                                                                    _Gdf(node_le._Gdf),
                                                                                    _history_clock_ids(node_le._history_clock_ids),
                                                                                    _prophecy_clock_ids(node_le._prophecy_clock_ids),
                                                                                    _normal_clock_ids(node_le._normal_clock_ids)
                                                                                    

{}//ani:-100

node_le_t::node_le_t(tchecker::tck_reach::zg_eca_gsim_gen::node_le_t && node_le) : _amap(std::move(node_le._amap)), 
                                                                              _G(std::move(node_le._G)),
                                                                              _Gdf(std::move(node_le._Gdf)),
                                                                              _history_clock_ids(std::move(node_le._history_clock_ids)),
                                                                              _prophecy_clock_ids(std::move(node_le._prophecy_clock_ids)),
                                                                              _normal_clock_ids(std::move(node_le._normal_clock_ids))
                                                                              
{
  node_le._G = nullptr;
  node_le._Gdf = nullptr;
  //ani:-100 what to do with constructor of move type?
}

node_le_t::~node_le_t()
{
  _G = nullptr;
  _Gdf = nullptr;
  
  //ani:-100 hope this is correct way to destroy a vector object!
  //ani:-101
  //not sure move destroys old object?
  // node_le._history_clock_ids.clear();
  // node_le._prophecy_clock_ids.clear();
  // node_le._normal_clock_ids.clear();
}


bool node_le_t::operator()(tchecker::tck_reach::zg_eca_gsim_gen::node_t const & n1,
                           tchecker::tck_reach::zg_eca_gsim_gen::node_t const & n2) const
{
  _amap.bounds(n2.state().vloc(), *_G, *_Gdf);
  return tchecker::zg::eca_g_le(n1.state(), n2.state(), *_G, *_Gdf, _history_clock_ids, _prophecy_clock_ids, _normal_clock_ids);
}

/* edge_t */

edge_t::edge_t(tchecker::zg::transition_t const & t) : _vedge(t.vedge_ptr()) {}

/* graph_t */
graph_t::graph_t(std::shared_ptr<tchecker::zg::zg_t> const & zg, std::size_t block_size, std::size_t table_size)
    : tchecker::graph::subsumption::graph_t<tchecker::tck_reach::zg_eca_gsim_gen::node_t, tchecker::tck_reach::zg_eca_gsim_gen::edge_t,
                                            tchecker::tck_reach::zg_eca_gsim_gen::node_hash_t,
                                            tchecker::tck_reach::zg_eca_gsim_gen::node_le_t>(
                                                block_size, table_size, tchecker::tck_reach::zg_eca_gsim_gen::node_hash_t(),
                                                tchecker::tck_reach::zg_eca_gsim_gen::node_le_t(zg->system())),
                                            _zg(zg)
{}

graph_t::~graph_t()
{
  tchecker::graph::subsumption::graph_t<tchecker::tck_reach::zg_eca_gsim_gen::node_t, tchecker::tck_reach::zg_eca_gsim_gen::edge_t,
                                        tchecker::tck_reach::zg_eca_gsim_gen::node_hash_t,
                                        tchecker::tck_reach::zg_eca_gsim_gen::node_le_t>::clear();
}

void graph_t::attributes(tchecker::tck_reach::zg_eca_gsim_gen::node_t const & n, std::map<std::string, std::string> & m) const
{
  _zg->attributes(n.state_ptr(), m);
}

void graph_t::attributes(tchecker::tck_reach::zg_eca_gsim_gen::edge_t const & e, std::map<std::string, std::string> & m) const
{
  m["vedge"] = tchecker::to_string(e.vedge(), _zg->system().as_system_system());
}


/* dot_output */

/*!
 \class node_lexical_less_t
 \brief Less-than order on nodes based on lexical ordering
*/
class node_lexical_less_t {
public:
  /*!
   \brief Less-than order on nodes based on lexical ordering
   \param n1 : a node
   \param n2 : a node
   \return true if n1 is less-than n2 w.r.t. lexical ordering over the states in
   the nodes
  */
  bool operator()(tchecker::tck_reach::zg_eca_gsim_gen::graph_t::node_sptr_t const & n1,
                  tchecker::tck_reach::zg_eca_gsim_gen::graph_t::node_sptr_t const & n2) const
  {
    return tchecker::zg::lexical_cmp(n1->state(), n2->state()) < 0;
  }
};

/*!
 \class edge_lexical_less_t
 \brief Less-than ordering on edges based on lexical ordering
 */
class edge_lexical_less_t {
public:
  /*!
   \brief Less-than ordering on edges based on lexical ordering
   \param e1 : an edge
   \param e2 : an edge
   \return true if e1 is less-than  e2 w.r.t. the tuple of edges in e1 and e2
  */
  bool operator()(tchecker::tck_reach::zg_eca_gsim_gen::graph_t::edge_sptr_t const & e1,
                  tchecker::tck_reach::zg_eca_gsim_gen::graph_t::edge_sptr_t const & e2) const
  {
    return tchecker::lexical_cmp(e1->vedge(), e2->vedge()) < 0;
  }
};

std::ostream & dot_output(std::ostream & os, tchecker::tck_reach::zg_eca_gsim_gen::graph_t const & g, std::string const & name)
{
  return tchecker::graph::subsumption::dot_output<tchecker::tck_reach::zg_eca_gsim_gen::graph_t,
                                                  tchecker::tck_reach::zg_eca_gsim_gen::node_lexical_less_t,
                                                  tchecker::tck_reach::zg_eca_gsim_gen::edge_lexical_less_t>(os, g, name);
}

/* run */
//ani:-100
std::tuple<tchecker::algorithms::covreach::stats_t, std::shared_ptr<tchecker::tck_reach::zg_eca_gsim_gen::graph_t>>
run(std::shared_ptr<tchecker::parsing::system_declaration_t> const & sysdecl, std::string const & labels,
    std::string const & search_order, std::size_t block_size, std::size_t table_size)
{
  // std::cout << "ani:---10007 constructing system\n"; 
  std::shared_ptr<tchecker::ta::system_t const> system{new tchecker::ta::system_t{*sysdecl}};
  
  // exit(0);
  // std::cout << "ani:---10008 constructing zone-graph\n";
  std::shared_ptr<tchecker::zg::zg_t> zg{
      tchecker::zg::factory(system, tchecker::zg::eca_gen2_SEMANTICS, tchecker::zg::NO_EXTRAPOLATION, block_size)};
  
  // std::cout << "ani:---10009 constructing zg_eca_g_sim\n";
  //ani:4 this is the point where lu-bounds G-SIM are computed!
  std::shared_ptr<tchecker::tck_reach::zg_eca_gsim_gen::graph_t> graph{
      new tchecker::tck_reach::zg_eca_gsim_gen::graph_t{zg, block_size, table_size}};
  
  // std::cout << "ani:-44444 ending amap computation\n";
  // for(auto tmp:system->used_history_clocks_ids){
  //   std::cout << "ani:570 " << tmp << std::endl;
  // }
  // exit(0);
  boost::dynamic_bitset<> accepting_labels = system->as_syncprod_system().labels(labels);

  tchecker::tck_reach::zg_eca_gsim_gen::algorithm_t algorithm;

  enum tchecker::waiting::policy_t policy = tchecker::algorithms::fast_remove_waiting_policy(search_order);

  tchecker::algorithms::covreach::stats_t stats = algorithm.run(*zg, *graph, accepting_labels, policy);

  return std::make_tuple(stats, graph);
}


} // end of namespace zg_eca_gsim_gen

} // end of namespace tck_reach

} // end of namespace tchecker