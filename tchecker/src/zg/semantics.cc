/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include "tchecker/zg/semantics.hh"
#include "tchecker/dbm/dbm.hh"
#include <unordered_set>

namespace tchecker {

namespace zg {

/* standard_semantics_t */

tchecker::state_status_t standard_semantics_t::initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool delay_allowed,
                                                       tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_ids,
                                                      const std::unordered_set<int> & prophecy_clock_ids,
                                                      const std::unordered_set<int> & normal_clock_ids)
{
  tchecker::dbm::zero(dbm, dim);

  if (tchecker::dbm::constrain(dbm, dim, invariant) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;

  return tchecker::STATE_OK;
}

tchecker::state_status_t standard_semantics_t::next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                                    tchecker::clock_constraint_container_t const & src_invariant,
                                                    tchecker::clock_constraint_container_t const & guard,
                                                    tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                                    tchecker::clock_constraint_container_t const & tgt_invariant,
                                                    const std::unordered_set<int> & history_clock_ids,
                                                    const std::unordered_set<int> & prophecy_clock_ids,
                                                    const std::unordered_set<int> & normal_clock_ids) //ani:-100
{
  if (src_delay_allowed) {
    tchecker::dbm::open_up(dbm, dim);

    if (tchecker::dbm::constrain(dbm, dim, src_invariant) == tchecker::dbm::EMPTY)
      return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED; // should never occur
  }

  if (tchecker::dbm::constrain(dbm, dim, guard) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_GUARD_VIOLATED;

  tchecker::dbm::reset(dbm, dim, clkreset);

  if (tchecker::dbm::constrain(dbm, dim, tgt_invariant) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED;

  return tchecker::STATE_OK;
}


//ani:-100
bool standard_semantics_t::is_final_dbm(tchecker::dbm::db_t const* dbm, tchecker::clock_id_t dim,
                                    const std::unordered_set<int> & history_clock_ids,
                                    const std::unordered_set<int> & prophecy_clock_ids,
                                    const std::unordered_set<int> & normal_clock_ids)//ani:-100
                                    {
                                      return true;
                                    }

/* elapsed_semantics_t */

tchecker::state_status_t elapsed_semantics_t::initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool delay_allowed,
                                                      tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_ids,
                                                      const std::unordered_set<int> & prophecy_clock_ids,
                                                      const std::unordered_set<int> & normal_clock_ids)
{
  tchecker::dbm::zero(dbm, dim);

  if (tchecker::dbm::constrain(dbm, dim, invariant) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;

  if (delay_allowed) {
    tchecker::dbm::open_up(dbm, dim);

    if (tchecker::dbm::constrain(dbm, dim, invariant) == tchecker::dbm::EMPTY)
      return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;
  }

  return tchecker::STATE_OK;
}

tchecker::state_status_t elapsed_semantics_t::next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                                   tchecker::clock_constraint_container_t const & src_invariant,
                                                   tchecker::clock_constraint_container_t const & guard,
                                                   tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                                   tchecker::clock_constraint_container_t const & tgt_invariant,
                                                   const std::unordered_set<int> & history_clock_ids,
                                                   const std::unordered_set<int> & prophecy_clock_ids,
                                                   const std::unordered_set<int> & normal_clock_ids)//ani:-100
{
  // std::cout << "ani:-918 outputting current zone\n";
  if (tchecker::dbm::constrain(dbm, dim, src_invariant) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;

  if (tchecker::dbm::constrain(dbm, dim, guard) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_GUARD_VIOLATED;

  tchecker::dbm::reset(dbm, dim, clkreset);

  if (tchecker::dbm::constrain(dbm, dim, tgt_invariant) == tchecker::dbm::EMPTY)
    return tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED;

  if (tgt_delay_allowed) {
    tchecker::dbm::open_up(dbm, dim);

    if (tchecker::dbm::constrain(dbm, dim, tgt_invariant) == tchecker::dbm::EMPTY)
      return tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED;
  }

  return tchecker::STATE_OK;
}

//ani:-100
bool elapsed_semantics_t::is_final_dbm(tchecker::dbm::db_t const* dbm, tchecker::clock_id_t dim,
                                    const std::unordered_set<int> & history_clock_ids,
                                    const std::unordered_set<int> & prophecy_clock_ids,
                                    const std::unordered_set<int> & normal_clock_ids)//ani:-100
                                    {
                                      return true;
                                    }


/* eca_gen2_semantics_t */
//ani:-100

tchecker::state_status_t eca_gen2_semantics_t::initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                                      bool delay_allowed,
                                                      tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_id_map,
                                                      const std::unordered_set<int> & prophecy_clock_id_map,
                                                      const std::unordered_set<int> & normal_clock_id_map)
{

  
  // std::cout<< "ani:-321 intitial_zone\n";
  //ani:-100
  tchecker::dbm::eca_zero(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map);
  
  // std::cout << "ani:-149 outputting intiial zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-151 outputted after eca_0\n";

  if (delay_allowed) {
    tchecker::dbm::eca_open_up(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map);
  }
  for(auto invar:invariant)
    if (tchecker::dbm::eca_constrain_single(dbm, dim, invar, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map) == tchecker::dbm::EMPTY)
      return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;
  // std::cout << "ani:-159 outputting before leaving zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-161 outputted zone\n";
  return tchecker::STATE_OK;
}

tchecker::state_status_t eca_gen2_semantics_t::next(tchecker::dbm::db_t * dbm, 
                                                   tchecker::clock_id_t dim, 
                                                   bool src_delay_allowed,
                                                   tchecker::clock_constraint_container_t const & src_invariant,
                                                   tchecker::clock_constraint_container_t const & guard,
                                                   tchecker::clock_reset_container_t const & clkreset, 
                                                   bool tgt_delay_allowed,
                                                   tchecker::clock_constraint_container_t const & tgt_invariant,
                                                   const std::unordered_set<int> & history_clock_id_map,
                                                   const std::unordered_set<int> & prophecy_clock_id_map,
                                                   const std::unordered_set<int> & normal_clock_id_map)
{

  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map));


  // std::cout << std::endl;
  // for(auto i:guard){
  //   std::cout << "ani:guards:" << i <<std::endl;
  // }
  // std::cout << "ani:-1118 outputting current zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-1119 outputted current zone\n";

  for(auto src_invar:src_invariant)
    if (tchecker::dbm::eca_constrain_single(dbm, dim, src_invar, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map) == tchecker::dbm::EMPTY){
      // std::cout << "ani:-218 STATE_CLOCKS_SRC_INVARIANT_VIOLATED\n";
      return tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED;
    }
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map));

  int ptr_guard = 0;
  int ptr_do = 0;



    while(ptr_guard<guard.size() || ptr_do<clkreset.size()){
      while(ptr_guard<guard.size() && guard[ptr_guard].id1()!=0){ //guard[ptr_guard].id1()==0 when id1() is the "tmp" clock
      //   //conjoin guard at ptr_guard
        // std::cout << "ani:-221 conjoining guard " << guard[ptr_guard] << std::endl;
        if(tchecker::dbm::eca_constrain_single(dbm, dim, guard[ptr_guard], history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map) == tchecker::dbm::EMPTY){
          // std::cout << "ani:-232 STATE_CLOCKS_GUARD_VIOLATED\n";
          return tchecker::STATE_CLOCKS_GUARD_VIOLATED;
        }
        // std::cout << "ani:-224 after conjoining guard:\n";
        // tchecker::dbm::output_matrix(std::cout, dbm, dim);
        
        ptr_guard++;
      }
      if(ptr_guard<guard.size())
        ptr_guard++;

      while(ptr_do<clkreset.size() && clkreset.at(ptr_do).left_id()!=0){ //clkreset.at(ptr_do).left_id()==0 when left_id() is the "tmp" clock
        // do the do operation
        // std::cout << "ani:-214 reset release " << clkreset.at(ptr_do).left_id() << std::endl;
        if(prophecy_clock_id_map.find(clkreset.at(ptr_do).left_id()+1)!=prophecy_clock_id_map.end())
          tchecker::dbm::eca_release_caller(dbm, dim, clkreset.at(ptr_do).left_id(),history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map);
        else
          tchecker::dbm::eca_reset_caller(dbm, dim, clkreset.at(ptr_do).left_id(), history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map);
        ptr_do++;
      }
      if(ptr_do<clkreset.size())
        ptr_do++;
      // std::cout << "ani:-223 after one operation "  << std::endl;
      // tchecker::dbm::output_matrix(std::cout, dbm, dim);
      // std::cout << std::endl << std::endl;;
    }

  // std::cout << "ani:-1128 outputting current zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-1129 outputted current zone\n";


  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map));

  // std::cout << "ani:-2118 outputting after executing program zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-2119 outputted after executing program zone\n";


  //time elapse
  if (tgt_delay_allowed) {
    tchecker::dbm::eca_open_up(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map);
  }
  // std::cout << "problem after_eca_open_up?\n";
  
  for(auto tgt_inv:tgt_invariant)  
    if (tchecker::dbm::eca_constrain_single(dbm, dim, tgt_inv, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map) == tchecker::dbm::EMPTY){
      // std::cout << "ani:268 STATE_CLOCKS_TGT_INVARIANT_VIOLATED\n"; 
      return tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED;
    }

  // std::cout << "ani:-2118 outputting next zone\n";
  // tchecker::dbm::output_matrix(std::cout, dbm, dim);
  // std::cout << "ani:-2119 outputted next zone\n\n";
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_id_map, prophecy_clock_id_map, normal_clock_id_map));
  // std::cout << "ani:-3119 outputted next zone\n\n";
  return tchecker::STATE_OK;
}

//ani:-100
bool eca_gen2_semantics_t::is_final_dbm(tchecker::dbm::db_t const* dbm, tchecker::clock_id_t dim,
                                    const std::unordered_set<int> & history_clock_id_map,
                                    const std::unordered_set<int> & prophecy_clock_id_map,
                                    const std::unordered_set<int> & normal_clock_id_map)//ani:-100
                                    {
  
  


  return tchecker::dbm::eca_is_final_dbm(dbm,dim,history_clock_id_map,prophecy_clock_id_map,normal_clock_id_map);
}







/* factory */

tchecker::zg::semantics_t * semantics_factory(enum semantics_type_t semantics)
{
  switch (semantics) {
  case tchecker::zg::STANDARD_SEMANTICS:
    return new tchecker::zg::standard_semantics_t{};
  case tchecker::zg::ELAPSED_SEMANTICS:
    return new tchecker::zg::elapsed_semantics_t{};
  case tchecker::zg::eca_gen2_SEMANTICS:  //ani:-100
    return new tchecker::zg::eca_gen2_semantics_t{};
  default:
    throw std::invalid_argument("Unknown zone semantics");
  }
}
} // end of namespace zg

} // end of namespace tchecker