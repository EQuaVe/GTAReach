/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#if BOOST_VERSION <= 106600
#include <boost/functional/hash.hpp>
#else
#include <boost/container_hash/hash.hpp>
#endif

#include "tchecker/zg/state.hh"

namespace tchecker {

namespace zg {

state_t::state_t(tchecker::intrusive_shared_ptr_t<tchecker::shared_vloc_t> const & vloc,
                 tchecker::intrusive_shared_ptr_t<tchecker::shared_intval_t> const & intval,
                 tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_zone_t> const & zone)
    : tchecker::ta::state_t(vloc, intval), _zone(zone)
{
  assert(_zone.ptr() != nullptr);
}

state_t::state_t(tchecker::ta::state_t const & s, tchecker::intrusive_shared_ptr_t<tchecker::shared_vloc_t> const & vloc,
                 tchecker::intrusive_shared_ptr_t<tchecker::shared_intval_t> const & intval,
                 tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_zone_t> const & zone)
    : tchecker::ta::state_t(s, vloc, intval), _zone(zone)
{
  assert(_zone.ptr() != nullptr);
}

bool operator==(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2)
{
  return tchecker::ta::operator==(s1, s2) && (s1.zone() == s2.zone());
}

bool operator!=(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2) { return !(s1 == s2); }

bool operator<=(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2)
{
  return tchecker::ta::operator==(s1, s2) && (s1.zone() <= s2.zone());
}

std::size_t hash_value(tchecker::zg::state_t const & s)
{
  std::size_t h = tchecker::ta::hash_value(s);
  boost::hash_combine(h, s.zone());
  return h;
}

bool alu_le(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2,
                 tchecker::clockbounds::map_t const & l, tchecker::clockbounds::map_t const & u)
{
  // std::cout << "ani:53 inside alu_le \n";
  return tchecker::ta::operator==(s1, s2) && s1.zone().alu_le(s2.zone(), l, u);
}

bool g_le(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2,
          std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *>  & G, 
          std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf)
{
  // std::cout << "ani:52 inside g_le \n";
  return tchecker::ta::operator==(s1, s2) && s1.zone().g_le(s2.zone(), G, Gdf);
}

//ani:-100
bool eca_g_le(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2,
          std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *>  & G, 
          std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf,
          std::unordered_set<tchecker::integer_t> history_clock_ids,
          std::unordered_set<tchecker::integer_t> prophecy_clock_ids,
          std::unordered_set<tchecker::integer_t> normal_clock_ids)
{
  // std::cout << "  ani:50 inside eca_g_le " << tchecker::ta::operator==(s1, s2) << std::endl;
  return tchecker::ta::operator==(s1, s2) && s1.zone().eca_g_le(s2.zone(), G, Gdf, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
}


int lexical_cmp(tchecker::zg::state_t const & s1, tchecker::zg::state_t const & s2)
{
  int ta_cmp = tchecker::ta::lexical_cmp(s1, s2);
  if (ta_cmp != 0)
    return ta_cmp;
  return s1.zone().lexical_cmp(s2.zone());
}

} // end of namespace zg

} // end of namespace tchecker
