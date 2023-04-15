/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_ZG_TRANSITION_HH
#define TCHECKER_ZG_TRANSITION_HH

#include "tchecker/ta/transition.hh"
#include "tchecker/utils/allocation_size.hh"

/*!
 \file transition.hh
 \brief Transitions for zone graphs
 */

namespace tchecker {

namespace zg {

/*!
 \class transition_t
 \brief Transition for zone graphs
 */
class transition_t : public tchecker::ta::transition_t {
public:
  using tchecker::ta::transition_t::transition_t;
};

/*!
 \brief Equality check
 \param t1 : transition
 \param t2 : transition
 \return see tchecker::ta::operator== on transitions
 */
bool operator==(tchecker::zg::transition_t const & t1, tchecker::zg::transition_t const & t2);

/*!
 \brief Disequality check
 \param t1 : transition
 \param t2 : transition
 \return see tchecker::ta::operator!= on transitions
 */
inline bool operator!=(tchecker::zg::transition_t const & t1, tchecker::zg::transition_t const & t2) { return !(t1 == t2); }

/*!
 \brief Hash
 \param t : transition
 \return Hash value for transition t
 */
std::size_t hash_value(tchecker::zg::transition_t const & t);

/*!
 \brief Lexical ordering on transitions of timed automata
 \param t1 : first transition
 \param t2 : second transition
 \return 0 if t1 and t2 are equal, a negative value if t1 is smaller than t2 w.r.t. lexical ordering on tuple of edges, source
 invariant, guard, reset, target invariant, a positive value otherwise
 */
int lexical_cmp(tchecker::zg::transition_t const & t1, tchecker::zg::transition_t const & t2);

/*!
 \brief Type of shared transition
 */
using shared_transition_t = tchecker::make_shared_t<tchecker::zg::transition_t>;

/*!
\brief Type of pointer to shared transition
*/
using transition_sptr_t = tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_transition_t>;

/*!
\brief Type of pointer to shared const transition
*/
using const_transition_sptr_t = tchecker::intrusive_shared_ptr_t<tchecker::zg::shared_transition_t const>;

} // end of namespace zg

/*!
 \class allocation_size_t
 \brief Specialization of tchecker::allocation_size_t for class tchecker::zg::transition_t
 */
template <> class allocation_size_t<tchecker::zg::transition_t> {
public:
  /*!
   \brief Allocation size
   \param args : arguments for a constructor of class tchecker::zg::transition_t
   \return allocation size for objects of class tchecker::zg::transition_t
   */
  template <class... ARGS> static constexpr std::size_t alloc_size(ARGS &&... args)
  {
    return sizeof(tchecker::zg::transition_t);
  }
};

} // end of namespace tchecker

#endif // TCHECKER_ZG_TRANSITION_HH
