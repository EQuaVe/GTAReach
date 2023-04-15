/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_DBM_DBM_HH
#define TCHECKER_DBM_DBM_HH

#include <functional>
#include <iostream>
#include <unordered_set>

#include "tchecker/basictypes.hh"
#include "tchecker/dbm/db.hh"
#include "tchecker/variables/clocks.hh"
#include "tchecker/expression/typed_expression.hh"

/*!
 \file dbm.hh
 \brief DBMs and operations on DBMs
 \note A DBM is a symbolic representation for conjunctions of difference constraints like x-y#c with
 # either < or <=. This implementation uses DBMs to represent sets of clock valuations. We assume a
 special clock 0 that represents the value 0. A DBM of dimension dim hence has clock 0 and dim-1 actual
 clocks with indices 1 to dim-1.
 A DBM of dimension dim is represented as a dim*dim array of difference bounds tchecker::dbm::db_t.
 The difference bound #c in [i,j] encodes the constraint x_i - x_j # c. The difference bound in [i,j] for
 dbm of dimension dim is accessed with dbm[i*dim+j].
 A DBM can be seen as weighted graph where nodes are clocks (including 0) and for any two clocks x and y,
 the weigth of the edge x -> y is the difference bound #c of the constraint y - x #c in the DBM.
 A DBM is consistent if for every clock x, the bound for x - x is <=0, and if the bound for 0 - x is less-than
 or equal-to <=0 (i.e. clocks are positive or zero). Notice that consistent zones are not empty.
 A DBM is tight if for every constraint x - y # c, the bound #c is the shortest-path from y to x in the
 corresponding graph. Every non-empty DBM has a unique corresponding tight DBM. Empty DBMs have a negative
 cycle on some clock x. Hence they cannot be tightened: there is no shortest path from x to x.
 */

namespace tchecker {

namespace dbm {

/*!
 \brief Status of a DBM
 */
enum status_t {
  EMPTY = 0,
  NON_EMPTY,
  MAY_BE_EMPTY,
};

/*!
 \brief Universal zone
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the true/universal zone (all valuations, positive and negative)
 dbm is tight.
 */
void universal(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

/*!
 \brief Universal zone
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the true/universal zone (all valuations, positive and negative)
 dbm is tight.
 */
void eca_universal(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                          const std::unordered_set<int> &history_clock_ids,
                                          const std::unordered_set<int> &prophecy_clock_ids, 
                                          const std::unordered_set<int> &normal_clock_ids);



/*!
 \brief Universal positive zone
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the positive true/universal zone (i.e. conjunction x >= 0 for all clocks x)
 dbm is tight.
 */
void universal_positive(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

/*!
 \brief Empty zone
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the empty zone (is_empty_0() returns true on dbm)
 dbm IS NOT TIGHT (empty DBMs cannot be tight)
 */
void empty(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Initial zone in ECA. All history clocks are set to +inf and all
 prophecy clocks x_p are non deterministically set to any value between [-inf, 0]
 with the following two constraints: 0-x_p<=inf and x_p-0<=0. All normal clocks
 are set to 0
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the zone that contains the initial zone of ECA of dimension dim.
 dbm is tight.
 */
void eca_zero(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, const std::unordered_set<int> &history_clock_ids, const std::unordered_set<int> &prophecy_clock_ids, const std::unordered_set<int> &normal_clock_ids);



/*!
 \brief Initialize to zero
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \post dbm represents the zone that contains the valuation 0 in dimension dim.
 dbm is tight.
 */
void zero(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

/*!
 \brief Consistency checking
 \param dbm : a dbm
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of tchecker::dbm::db_t.
 dim >= 1 (checked by assertion)
 \return true if the diagonal in dbm is <=0, and false otherwise
 */
bool is_consistent(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Consistency checking in ECA
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of tchecker::dbm::db_t.
 dim >= 1 (checked by assertion)
 \return true if the diagonal in dbm is <=0 and all history clocks
 (that is the history clocks) are atleast 0 (i.e. 0-i#c implies (#,c)<=(<=,0))
 and all the prophecy clocks are atmost 0, (i.e i-0#c implies (#,c)<=(<=,0)
 , and false otherwise
 */
bool eca_is_consistent(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                        const std::unordered_set<int> &history_clock_ids,
                        const std::unordered_set<int> &prophecy_clock_ids, 
                        const std::unordered_set<int> &normal_clock_ids);

/*!
 \brief Fast emptiness predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm has been tightened
 dim >= 1 (checked by assertion).
 \return true if dbm has a negative difference bound on (0,0), false otherwise
 \note this function only checks the (0,0) entry of dbm. Hence, dbm may be empty but not be detected by this function. However,
 all other functions set (0,0) to a value less-than <=0 when they generate an empty zone. So this function can be used as a
 safe and efficient emptiness check if dbm has been generated by calls to functions in this file.
 */
bool is_empty_0(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Fast ECA final zone predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm has been tightened
 dim >= 1 (checked by assertion).
 \return true if dbm can be a final zone (i.e for all prophecy clocks a_p, a_p-0#c implies (#,c)=(<=,-INF)), false otherwise.
 */
bool eca_is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                        const std::unordered_set<int> &history_clock_ids,
                        const std::unordered_set<int> &prophecy_clock_ids, 
                        const std::unordered_set<int> &normal_clock_ids);

/*!
 \brief Universality predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion).
 \return true if dbm represents the true/universal zone, false otherwise (see tchecker::dbm::universal)
 */
bool is_universal(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

/*!
 \brief Positivity predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion).
 \return true if dbm is positive (i.e. for every valuation in dbm, and every clock x, x>=0), false otherwise
 */
bool is_positive(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

/*!
 \brief Positivity & universality predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion).
 \return true if dbm is the set of all positive valuations, false otherwise
 \note efficient algorithm for tchecker::dbm::is_universal(dbm, dim) && tchecker::dbm::is_positive(dbm dim)
 */
bool is_universal_positive(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

/*!
 \brief Tightness predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \return true if dbm is tight, false otherwise
 */
bool is_tight(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Tightness predicate
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \return true if dbm is tight, false otherwise
 */
bool eca_is_tight(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Tighten an ECA DBM
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 dbm is consistent (checked by assertion)
 \post dbm is not nullptr (checked by assertion)
 dbm is tight if it is not empty.
 if dbm is empty, then the difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, NON_EMPTY otherwise
 \note Applies Floyd-Warshall algorithm on dbm seen as a weighted graph.
 */
enum tchecker::dbm::status_t eca_tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
              const std::unordered_set<int> &history_clock_ids,
              const std::unordered_set<int> &prophecy_clock_ids, 
              const std::unordered_set<int> &normal_clock_ids);

/*!
 \brief Tighten a DBM
 \param dbm : a DBM
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 dbm is consistent (checked by assertion)
 \post dbm is not nullptr (checked by assertion)
 dbm is tight if it is not empty.
 if dbm is empty, then the difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, NON_EMPTY otherwise
 \note Applies Floyd-Warshall algorithm on dbm seen as a weighted graph.
 */
enum tchecker::dbm::status_t tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief Tighten an ECA DBM w.r.t. a constraint
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param x : first clock
 \param y : second clock
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 x != y (checked by assertion)
 \post for all clocks u and v, the edge u->v in the graph is tight w.r.t. the
 edge y->x. That is, the length of the path u->v is the minimum between its
 length before the call and the length of the path u->y->x->v.
 if dbm is empty, then the difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, MAY_BE_EMPTY otherwise
 \note if every edge in dbm is tight w.r.t. all other edges except i->j, then after the call, dbm is either empty, or it is
 not empty and tight.
 */
enum tchecker::dbm::status_t eca_tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                     tchecker::clock_id_t y);

/*!
 \brief Tighten a DBM w.r.t. a constraint
 \param dbm : a DBM
 \param dim : dimension of dbm
 \param x : first clock
 \param y : second clock
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 x != y (checked by assertion)
 \post for all clocks u and v, the edge u->v in the graph is tight w.r.t. the
 edge y->x. That is, the length of the path u->v is the minimum between its
 length before the call and the length of the path u->y->x->v.
 if dbm is empty, then the difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, MAY_BE_EMPTY otherwise
 \note if every edge in dbm is tight w.r.t. all other edges except i->j, then after the call, dbm is either empty, or it is
 not empty and tight.
 */
enum tchecker::dbm::status_t tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                     tchecker::clock_id_t y);

/*!
 \brief Constrain a DBM
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : first clock
 \param y : second clock
 \param cmp : constraint comparator
 \param value : constraint value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 value can be represented in a tchecker::dbm::db_t
 \post dbm has been intersected with constraint `x - y # value` where # is < if cmp is LT, and # is <= if cmp is LE
 dbm is tight if it is not empty.
 if dbm is empty, then its difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, NON_EMPTY otherwise
 \throw std::invalid_argument : if `cmp value` cannot be represented as a tchecker::dbm::db_t (only if compilation flag
 DBM_UNSAFE is not set)
 */
enum tchecker::dbm::status_t constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                       tchecker::clock_id_t y, tchecker::dbm::comparator_t cmp, tchecker::integer_t value);


//ani:-100
/*!
 \brief Constrain an ECA DBM
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : first clock
 \param y : second clock
 \param cmp : constraint comparator
 \param value : constraint value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is eca consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 value can be represented in a tchecker::dbm::db_t
 \post dbm has been intersected with constraint `x - y # value` where # is < if cmp is LT, and # is <= if cmp is LE
 dbm is tight if it is not empty.
 if dbm is empty, then its difference bound in (0,0) is less-than <=0 (tchecker::dbm::is_empty_0() returns true)
 \return EMPTY if dbm is empty, NON_EMPTY otherwise
 \throw std::invalid_argument : if `cmp value` cannot be represented as a tchecker::dbm::db_t (only if compilation flag
 DBM_UNSAFE is not set)
 */
enum tchecker::dbm::status_t eca_constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                        tchecker::clock_id_t y, tchecker::dbm::comparator_t cmp, tchecker::integer_t value,
                                        const std::unordered_set<int> &history_clock_ids,
                                        const std::unordered_set<int> &prophecy_clock_ids, 
                                        const std::unordered_set<int> &normal_clock_ids);





/*!
 \brief Constrain a DBM w.r.t. clock constraints container
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param constraints : clock constraints
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 all clock constraints in constraints are expressed over system clocks
 \post dbm has been intersected with constraints
 if dbm is empty, then its difference bound in (0,0) is less-than <=0
 (tchecker::dbm::is_empty_0() returns true)
 the resulting DBM is tight and consistent if not empty
 \return EMPTY is the resulting DBM is empty, NON_EMPTY otherwise
*/
enum tchecker::dbm::status_t constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                       tchecker::clock_constraint_container_t const & constraints);



//ani:-100
/*!
 \brief Constrain an ECA DBM w.r.t. clock constraint
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param constraints : clock constraints
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is eca consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 all clock constraints in constraints are expressed over system clocks
 \post dbm has been intersected with constraints
 if dbm is empty, then its difference bound in (0,0) is less-than <=0
 (tchecker::dbm::is_empty_0() returns true)
 the resulting DBM is tight and consistent if not empty
 \return EMPTY is the resulting DBM is empty, NON_EMPTY otherwise
*/
enum tchecker::dbm::status_t eca_constrain_single(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                       tchecker::clock_constraint_t const & constraint,
                                        const std::unordered_set<int> &history_clock_ids,
                                        const std::unordered_set<int> &prophecy_clock_ids, 
                                        const std::unordered_set<int> &normal_clock_ids);

/*!
 \brief Equality predicate
 \param dbm1 : a first dbm
 \param dmb2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \return true if dbm1 and dbm2 are equal, false otherwise
 */
bool is_equal(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim);

/*!
 \brief Inclusion predicate
 \param dbm1 : a first dbm
 \param dmb2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \return true if dbm1 is included into dbm2, false otherwise
 */
bool is_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim);

/*!
 \brief Reset a clock
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : left-value clock
 \param y : right-value clock
 \param value : a value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 0 <= value (checked by assertion)
 `<= value` can be represented by tchecker::dbm::db_t (checked by assertion)
 \post dbm has been updated according to reset `x := y + value`
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 \note wrapper for dedicated functions depending on y and value. Set y to 0 in order to reset x to constant value. Set value
 to 0 in order to reset x to y. Set both y and value non-zero to reset x to the sum y+value. Increment x by value if y==x.
 \throw std::invalid_argument : if `<= value` cannot be represented as a tchecker::dbm::db_t (only if compilation flag
 DBM_UNSAFE is not set)
 */
void reset(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
           tchecker::integer_t value);

/*!
 \brief Reset from a clock reset container
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param resets : a clock reset container
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 all resets are expressed over system clocks
 \post dbm has been updated according to the sequence of resets
 dbm is tight and consistent
 */
void reset(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_reset_container_t const & resets);

/*!
 \brief Reset a clock to a constant
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : left-value clock
 \param value : a value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= value *checked by assertion)
 `<= value` can be represented by tchecker::dbm::db_t
 \post dbm has been updated according to reset x := value.
 dbm is consistent.
 dbm is tight.
 \throw std::invalid_argument : if `<= value` cannot be represented as a tchecker::dbm::db_t (only if compilation flag
 DBM_UNSAFE is not set)
 */
void reset_to_value(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::integer_t value);

/*!
 \brief Reset a clock to another clock
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : left-value clock
 \param y : right-value clock
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <  y < dim (checked by assertion)
 \post dbm has been updated according to reset x := y.
 dbm is consistent.
 dbm is tight.
 */
void reset_to_clock(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y);

/*!
 \brief Reset a clock to the sum of a clock and a non-negative integer
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : left-value clock
 \param y : right-value clock
 \param value : a value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 0 <= value (checked by assertion)
 `<= value` can be represented by tchecker::dbm::db_t (checked by assertion)
 \post dbm has been updated according to reset `x := y + value`
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 */
void reset_to_sum(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
                  tchecker::integer_t value);

/*!
 \brief Reset a clock to the sum of a clock and a negative integer
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param x : left-value clock
 \param y : right-value clock
 \param value : a value
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 0 <= x < dim (checked by assertion)
 0 <= y < dim (checked by assertion)
 value < 0 (checked by assertion)
 `<= value` can be represented by tchecker::dbm::db_t (checked by assertion)
 \post dbm has been updated according to reset `x := y + value`
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 */
void reset_to_subtraction(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
                  tchecker::integer_t value);

//ani:-100
/*!
 \brief Reset from a history clock in ECA according to clock reset
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param reset : a clock reset type
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 all resets are expressed over system clocks
 \post dbm has been updated according to the history clock reset
 dbm is tight and consistent
 */
void eca_reset_caller(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t reset_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids);



//ani:-100
/*!
 \brief Reset from a history clock in ECA according to clock reset
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param reset : the clock id of clock to reset
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 all resets are expressed over system clocks
 \post dbm has been updated according to the history clock reset
 dbm is tight and consistent
 */
void eca_reset(tchecker::dbm::db_t * dbm,
                tchecker::clock_id_t dim, 
                tchecker::clock_id_t reset_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids);

/*!
 \brief Open up (delay)
 \param dbm : a dbm
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \post all upper bounds on clocks in dbm are <infinity.
 dbm is tight.
 */
void open_up(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim);

//ani:-100
/*!
 \brief ECA open up (delay)
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \post all upper bounds on clocks in dbm are <=infinity.
 dbm is tight.
 */
void eca_open_up(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
                  const std::unordered_set<int> &history_clock_ids, 
                  const std::unordered_set<int> &prophecy_clock_ids, 
                  const std::unordered_set<int> &normal_clock_ids);

//ani:-100
/*!
 \brief Release a prophecy clock with index
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 
 \post dbm has been updated according to the release of a prophecy clock
 dbm is tight and consistent
 */
void eca_release_caller(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                tchecker::clock_id_t prophecy_clock_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids);

//ani:-100
/*!
 \brief Release a prophecy clock with index
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion).
 
 \post dbm has been updated according to the release of a prophecy clock
 dbm is tight and consistent
 */
void eca_release(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,tchecker::clock_id_t prophecy_clock_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids);
                

/*!
 \brief Intersection
 \param dbm : a dbm
 \param dbm1 : a dbm
 \param dbm2 : a dbm
 \param dim : dimension of dbm1, dbm1 and dbm2
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm, dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm, dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \post dbm is the intersection of dbm1 and dbm2
 dbm is consistent
 dbm is tight
 \return EMPTY if the intersection of dbm1 and dbm2 is empty, NON_EMPTY otherwise
 \note dbm can be one of dbm1 or dbm2
 */
enum tchecker::dbm::status_t eca_intersection(tchecker::dbm::db_t * dbm, tchecker::dbm::db_t const * dbm1,
                                          tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
                                          const std::unordered_set<int> &history_clock_ids,
                                          const std::unordered_set<int> &prophecy_clock_ids, 
                                          const std::unordered_set<int> &normal_clock_ids);


/*!
 \brief Intersection
 \param dbm : a dbm
 \param dbm1 : a dbm
 \param dbm2 : a dbm
 \param dim : dimension of dbm1, dbm1 and dbm2
 \pre dbm, dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm, dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion).
 \post dbm is the intersection of dbm1 and dbm2
 dbm is consistent
 dbm is tight
 \return EMPTY if the intersection of dbm1 and dbm2 is empty, NON_EMPTY otherwise
 \note dbm can be one of dbm1 or dbm2
 */
enum tchecker::dbm::status_t intersection(tchecker::dbm::db_t * dbm, tchecker::dbm::db_t const * dbm1,
                                          tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim);

/*!
 \brief ExtraM extrapolation
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param m : clock bounds for clocks 1 to dim-1 (m[0] is the bound for clock 1 and so on)
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is positive (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 m is an array of size dim-1
 m[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \post extrapolation ExtraM has been applied to dbm w.r.t. clock
 bounds m (see "Lower and upper bounds in zone-based abstractions of timed automata", Behrmann, Bouyer, Larsen and Pelanek.
 Int. J. STTT, 2006)
 \note set m[i] to -tchecker::dbm::INF_VALUE if clock i has no bound
 */
void extra_m(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * m);

/*!
 \brief ExtraM+ extrapolation
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param m : clock bounds for clocks 1 to dim-1 (m[0] is the bound for clock 1 and so on)
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is positive (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 m is an array of size dim-1
 m[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \post extrapolation ExtraM+ has been applied to dbm w.r.t. clock
 bounds m (see "Lower and upper bounds in zone-based abstractions of timed automata", Behrmann, Bouyer, Larsen and Pelanek.
 Int. J. STTT, 2006)
 \note set m[i] to -tchecker::dbm::INF_VALUE if clock i has no bound
 */
void extra_m_plus(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * m);

/*!
 \brief ExtraLU extrapolation
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param l : clock lower bounds for clocks 1 to dim-1 (l[0] is the bound for clock 1 and so on)
 \param u : clock upper bounds for clocks 1 to dim-1 (u[0] is the bound for clock 1 and so on)
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is positive (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 l and u are arrays of size dim-1
 l[i], u[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \post extrapolation ExtraLU has been applied to dbm w.r.t. clock
 bounds l and u (see "Lower and upper bounds in zone-based abstractions of timed automata", Behrmann, Bouyer, Larsen and
 Pelanek. Int. J. STTT, 2006)
 \note set l[i]/u[i] to -tchecker::dbm::INF_VALUE if clock i has no lower/upper bound
 */
void extra_lu(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * l,
              tchecker::integer_t const * u);

/*!
 \brief ExtraLU+ extrapolation
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param l : clock lower bounds for clocks 1 to dim-1 (l[0] is the bound for clock 1 and so on)
 \param u : clock upper bounds for clocks 1 to dim-1 (u[0] is the bound for clock 1 and so on)
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dbm is consistent (checked by assertion)
 dbm is positive (checked by assertion)
 dbm is tight (checked by assertion)
 dim >= 1 (checked by assertion)
 l and u are arrays of size dim-1
 l[i], u[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \post extrapolation ExtraLU+ has been applied to dbm w.r.t. clock
 bounds l and u (see "Lower and upper bounds in zone-based abstractions of timed automata", Behrmann, Bouyer, Larsen and
 Pelanek. Int. J. STTT, 2006)
 \note set l[i]/u[i] to -tchecker::dbm::INF_VALUE if clock i has no lower/upper bound
 */
void extra_lu_plus(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * l,
                   tchecker::integer_t const * u);

/*!
 \brief Checks inclusion w.r.t. abstraction aLU
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param l : clock lower bounds for clocks 1 to dim-1 (l[0] is the bound for clock 1 and so on)
 \param u : clock upper bounds for clocks 1 to dim-1 (u[0] is the bound for clock 1 and so on)
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 l and u are arrays of size dim-1
 l[i], u[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \return true if dbm1 <= aLU(dbm2), false otherwise (see "Better abstractions for timed automata", Herbreteau, Srivathsan
 and Walukiewicz. Inf. Comput., 2016)
 \note set l[i]/u[i] to -tchecker::dbm::INF_VALUE if clock i has no lower/upper bound
 */
bool is_alu_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
               tchecker::integer_t const * l, tchecker::integer_t const * u);

/*!
 \brief Checks inclusion w.r.t. abstraction aM
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param m : clock bounds for clocks 1 to dim-1 (m[0] is the bound for clock 1 and so on)
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 m is an array of size dim-1
 m[i] < tchecker::dbm::INF_VALUE for all i>=0 (checked by assertion)
 \return true if dbm1 <= aM(dbm2), false otherwise (see "Better abstractions for timed automata", Herbreteau, Srivathsan
 and Walukiewicz. Inf. Comput., 2016)
 \note set m[i] to -tchecker::dbm::INF_VALUE if clock i has no lower/upper bound
 */
bool is_am_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
              tchecker::integer_t const * m);

/*!
 \brief Checks simulation w.r.t. G-simulation (diagonal-free)
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param Gdf : a set of non-diagonal constraints
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 \return true if dbm1 <=_Gdf dbm2, false otherwise
 */
bool is_g_le_nd(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
            std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf);

/*!
 \brief Checks simulation w.r.t. G-simulation
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param G   : a set of diagonal constraints
 \param Gdf : a set of non-diagonal constraints
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 \return true if dbm1 <=_(G U Gdf) dbm2, false otherwise (see "Fast Algorithms for Handling Diagonal Constraints in Timed Automata", Paul Gastin, Sayan Mukherjee, B Srivathsan, in proceedings of CAV 2019)
 */
bool is_g_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
            std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
            std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf);


/*!
 \brief Checks simulation w.r.t. G-simulation (diagonal-free) for ECA
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param Gdf : a set of non-diagonal constraints
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 \return true if dbm1 <=_Gdf dbm2, false otherwise
 */
//ani:-100
bool is_eca_g_le_nd(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
                std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                const std::unordered_set<tchecker::integer_t> & normal_clock_ids);


//ani:-100 i don't know what to write here as there is no such function declaration in dbm.hh!
bool is_eca_g_le_star(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim, 
                      std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
                      std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                      const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                      const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                      const std::unordered_set<tchecker::integer_t> & normal_clock_ids);

/*!
 \brief Checks simulation w.r.t. G-simulation for ECA
 \param dbm1 : a first dbm
 \param dbm2 : a second dbm
 \param dim : dimension of dbm1 and dbm2
 \param G   : a set of diagonal constraints
 \param Gdf : a set of non-diagonal constraints
 \param history_clock_ids : a vector of integer values of id of history clocks
 \param prophecy_clock_ids : a vector of integer values of id of prophecy clocks
 \param normal_clock_ids : a vector of integer values of id of normal clocks
 \pre dbm1 and dbm2 are not nullptr (checked by assertion)
 dbm1 and dbm2 are dim*dim arrays of difference bounds
 dbm1 and dbm2 are consistent (checked by assertion)
 dbm1 and dbm2 are positive (checked by assertion)
 dbm1 and dbm2 are tight (checked by assertion)
 dim >= 1 (checked by assertion)
 \return true if dbm1 <=_(G U Gdf) dbm2, false otherwise (see "Fast Algorithms for Handling Diagonal Constraints in Timed Automata", Paul Gastin, Sayan Mukherjee, B Srivathsan, in proceedings of CAV 2019)
 */
//ani:-100
bool is_eca_g_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
            std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
            std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                const std::unordered_set<tchecker::integer_t> & normal_clock_ids);

/*!
 \brief Hash function
 \param dbm : a dbm
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion)
 \return hash code of dbm
 */
std::size_t hash(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

/*!
 \brief Output a DBM as a matrix
 \param os : output stream
 \param dbm : a dbm
 \param dim : dimension of dbm
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion).
 \post dbm has been output to os
 \return os after output
 */
std::ostream & output_matrix(std::ostream & os, tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim);

/*!
 \brief Output a DBM as a conjunction of constraints
 \param os : output stream
 \param dbm : a dbm
 \param dim : dimension of dbm
 \param clock_name : map from clock IDs to strings
 \pre dbm is not nullptr (checked by assertion)
 dbm is a dim*dim array of difference bounds
 dim >= 1 (checked by assertion).
 clock_name maps any clock ID in [0,dim) to a name
 \post the relevant constraints in dbm has been output to os. Relevant constraints are those that differ from the
 universal DBM.
 \return os after output
 */
std::ostream & output(std::ostream & os, tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                      std::function<std::string(tchecker::clock_id_t)> clock_name);

/*!
 \brief Lexical ordering
 \param dbm1 : first dbm
 \param dim1 : dimension of dbm1
 \param dbm2 : second dbm
 \param dim2 : dimension of dbm2
 \pre dbm1 and dbm2 are ot nullptr (checked by assertion)
 dbm1 is a dim1*dim1 array of difference bounds
 dbm2 is a dim2*dim2 array of difference bounds
 dim1 >= 1 and dim2 >= 1 (checked by assertion)
 \return 0 if dbm1 and dbm2 are equal, a negative value if dbm1 is smaller than dbm2 w.r.t. lexical ordering, and a positive
 value otherwise
 */
int lexical_cmp(tchecker::dbm::db_t const * dbm1, tchecker::clock_id_t dim1, tchecker::dbm::db_t const * dbm2,
                tchecker::clock_id_t dim2);

} // end of namespace dbm

} // end of namespace tchecker

#endif // TCHECKER_DBM_DBM_HH
