/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ZG_SEMANTICS_HH
#define TCHECKER_ZG_SEMANTICS_HH

#include "tchecker/basictypes.hh"
#include "tchecker/dbm/db.hh"
#include "tchecker/variables/clocks.hh"
#include <unordered_set>

/*!
 \file semantics.hh
 \brief Operational semantics on DBMs
 */

namespace tchecker {

namespace zg {

/*!
 \class semantics_t
 \brief Semantics for zone graphs implemented with DBMs
 */
class semantics_t {
public:
  /*!
  \brief Destructor
   */
  virtual ~semantics_t() = default;

  /*!
  \brief Compute initial zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param delay_allowed : true if delay is allowed in initial state
  \param invariant : invariant
  \post dbm is the initial zone w.r.t. delay_allowed and invariant
  \return STATE_OK if the resulting dbm is not empty, other values if the
  resulting dbm is empty (see details in implementations)
   */
  virtual tchecker::state_status_t initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool delay_allowed,
                                           tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_ids,
                                                      const std::unordered_set<int> & prophecy_clock_ids,
                                                      const std::unordered_set<int> & normal_clock_ids) = 0;

  
  /*!
  \brief Compute next zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param src_delay_allowed : true if delay allowed in source state
  \param src_invariant : invariant in source state
  \param guard : transition guard
  \param clkreset : transition reset
  \param tgt_delay_allowed : true if delay allowed in target state
  \param tgt_invariant : invariant in target state
  \post dbm has been updated to its strongest postcondition w.r.t. src_delay_allowed,
  src_invariant, guard, clkreset, tgt_delay_allowed and tgt_invariant
  \return STATE_OK if the resulting dbm is not empty, other values if the resulting
  dbm is empty (see details in implementations)
   */
  virtual tchecker::state_status_t next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                        tchecker::clock_constraint_container_t const & src_invariant,
                                        tchecker::clock_constraint_container_t const & guard,
                                        tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                        tchecker::clock_constraint_container_t const & tgt_invariant,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids) = 0;
  
  /*!
  \brief Determine if current zone can be a final zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \return TRUE if and only if zone given by dbm is a final zone
   */
  virtual bool is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids)
                                        {
                                          return true;
                                        }

};

/*!
\class standard_semantics_t
\brief Standard semantics: each transition in the zone graph consists of a delay
(if allowed) followed by a transition from the timed automaton
*/
class standard_semantics_t final : public tchecker::zg::semantics_t {
public:
  /*!
  \brief Destructor
  */
  virtual ~standard_semantics_t() = default;

  /*!
  \brief Compute initial zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param delay_allowed : true if delay is allowed in initial state
  \param invariant : invariant
  \post dbm is the zone that only containts the zero valuation
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if the zero valuation does not
  satisfy invariant.
   */
  virtual tchecker::state_status_t initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool delay_allowed,
                                           tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_ids,
                                                      const std::unordered_set<int> & prophecy_clock_ids,
                                                      const std::unordered_set<int> & normal_clock_ids);


  /*!
  \brief Compute next zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param src_delay_allowed : true if delay allowed in source state
  \param src_invariant : invariant in source state
  \param guard : transition guard
  \param clkreset : transition reset
  \param tgt_delay_allowed : true if delay allowed in target state
  \param tgt_invariant : invariant in target state
  \post dbm has been delayed (if allowed), then intersected with src_invariant,
  then intersected with guard, then reset w.r.t clkreset, then intersected with
  tgt_invariant
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if intersection with src_invariant
  result in an empty zone, tchecker::STATE_CLOCKS_GUARD_VIOLATED if intersection
  with guard result in an empty zone,
  tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED if intersection with
  tgt_invariant result in an empty zone
  */
  virtual tchecker::state_status_t next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                        tchecker::clock_constraint_container_t const & src_invariant,
                                        tchecker::clock_constraint_container_t const & guard,
                                        tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                        tchecker::clock_constraint_container_t const & tgt_invariant,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids);

  /*!
  \brief Determine if current zone can be a final zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \return TRUE if and only if zone given by dbm is a final zone
   */
  virtual bool is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids);
};

/*!
\class elapsed_semantics_t
\brief elapsed semantics: each transition in the zone graph consists of a
transition from the automaton, followed by a delay (if allowed)
*/
class elapsed_semantics_t final : public tchecker::zg::semantics_t {
public:
  /*!
  \brief Destructor
  */
  virtual ~elapsed_semantics_t() = default;

  /*!
  \brief Compute initial zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param delay_allowed : true if delay is allowed in initial state
  \param invariant : invariant
  \post dbm is the zone that contains all the time successors of the zero
  valuation that satisfy invariant if delay_allowed, otherwise the zero
  valuation if it satisfies invariant
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if the (time successors of the)
  zero valuation does not satisfy invariant.
   */
  virtual tchecker::state_status_t initial(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool delay_allowed,
                                           tchecker::clock_constraint_container_t const & invariant,
                                                      const std::unordered_set<int> & history_clock_ids,
                                                      const std::unordered_set<int> & prophecy_clock_ids,
                                                      const std::unordered_set<int> & normal_clock_ids);

  /*!
  \brief Compute next zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param src_delay_allowed : true if delay allowed in source state
  \param src_invariant : invariant in source state
  \param guard : transition guard
  \param clkreset : transition reset
  \param tgt_delay_allowed : true if delay allowed in target state
  \param tgt_invariant : invariant in target state
  \post dbm has been intersected with src_invariant, then intersected with
  guard, then reset w.r.t clkreset, then intersected with tgt_invariant, then
  delayed (if allowed) and intersected wth tgt_invariant again (if delayed)
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if intersection with src_invariant
  result in an empty zone, tchecker::STATE_CLOCKS_GUARD_VIOLATED if intersection
  with guard result in an empty zone,
  tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED if intersection with
  tgt_invariant result in an empty zone
  */
  virtual tchecker::state_status_t next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                        tchecker::clock_constraint_container_t const & src_invariant,
                                        tchecker::clock_constraint_container_t const & guard,
                                        tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                        tchecker::clock_constraint_container_t const & tgt_invariant,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids);
  
  /*!
  \brief Determine if current zone can be a final zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \return TRUE if and only if zone given by dbm is a final zone
   */
  virtual bool is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids);
};



/*!
\class eca_gen2_semantics_t
\brief eca_gen2_semantics_t: each transition in the zone graph consists of a
transition from the automaton, followed by a delay (if allowed)
*/
class eca_gen2_semantics_t final : public tchecker::zg::semantics_t {
public:
  /*!
  \brief Destructor
  */
  virtual ~eca_gen2_semantics_t() = default;

  /*!
  \brief Compute initial zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param delay_allowed : true if delay is allowed in initial state
  \param invariant : invariant
  \post dbm is the zone that contains all the time successors of the zero
  valuation that satisfy invariant if delay_allowed, otherwise the zero
  valuation if it satisfies invariant
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if the (time successors of the)
  zero valuation does not satisfy invariant.
   */
  virtual tchecker::state_status_t initial(tchecker::dbm::db_t * dbm, 
                                            tchecker::clock_id_t dim,
                                            bool delay_allowed,
                                            tchecker::clock_constraint_container_t const & invariant,
                                            const std::unordered_set<int> & history_clock_ids,
                                            const std::unordered_set<int> & prophecy_clock_ids,
                                            const std::unordered_set<int> & normal_clock_ids);

  /*!
  \brief Compute next zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \param src_delay_allowed : true if delay allowed in source state
  \param src_invariant : invariant in source state
  \param guard : transition guard
  \param clkreset : transition reset
  \param tgt_delay_allowed : true if delay allowed in target state
  \param tgt_invariant : invariant in target state
  \post dbm has been intersected with src_invariant, then intersected with
  guard, then reset w.r.t clkreset, then intersected with tgt_invariant, then
  delayed (if allowed) and intersected wth tgt_invariant again (if delayed)
  \return tchecker::STATE_OK if the resulting DBM is not empty. Otherwise,
  tchecker::STATE_CLOCKS_SRC_INVARIANT_VIOLATED if intersection with src_invariant
  result in an empty zone, tchecker::STATE_CLOCKS_GUARD_VIOLATED if intersection
  with guard result in an empty zone,
  tchecker::STATE_CLOCKS_TGT_INVARIANT_VIOLATED if intersection with
  tgt_invariant result in an empty zone
  */
  
  virtual tchecker::state_status_t next(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, bool src_delay_allowed,
                                        tchecker::clock_constraint_container_t const & src_invariant,
                                        tchecker::clock_constraint_container_t const & guard,
                                        tchecker::clock_reset_container_t const & clkreset, bool tgt_delay_allowed,
                                        tchecker::clock_constraint_container_t const & tgt_invariant,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids
                                        );

/*!
  \brief Determine if current zone can be a final zone
  \param dbm : a DBM
  \param dim : dimension of dbm
  \return TRUE if and only if zone given by dbm is a final zone
   */
  virtual bool is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                                        const std::unordered_set<int> & history_clock_ids,
                                        const std::unordered_set<int> & prophecy_clock_ids,
                                        const std::unordered_set<int> & normal_clock_ids);

};


/*!
 \brief type of semantics
*/
enum semantics_type_t {
  STANDARD_SEMANTICS, /*!< see tchecker::zg::standard_semantics_t */
  ELAPSED_SEMANTICS,  /*!< see tchecker::zg::elapsed_semantics_t */
  eca_gen2_SEMANTICS, /*!< see tchecker::zg::eca_gen_semantics_t */ 
  
};

/*!
 \brief Zone graph semantics factory
 \param semantics_type : type zone graph semantics
 \return A zone graph semantics of type semantics
 \note the returned semantics must be deallocated by the caller
 \throw std::invalid_argument : if semantics is unknown
*/
tchecker::zg::semantics_t * semantics_factory(enum semantics_type_t semantics_type);

} // end of namespace zg

} // end of namespace tchecker

#endif // TCHECKER_ZG_SEMANTICS_HH
