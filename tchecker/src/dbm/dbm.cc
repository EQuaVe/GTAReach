/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <cassert>

#if BOOST_VERSION <= 106600
#include <boost/functional/hash.hpp>
#else
#include <boost/container_hash/hash.hpp>
#endif

#include "tchecker/dbm/dbm.hh"
#include "tchecker/utils/ordering.hh"
#include "tchecker/expression/static_analysis.hh"

namespace tchecker {

namespace dbm {

#define DBM(i, j)  dbm[(i)*dim + (j)]
#define DBM1(i, j) dbm1[(i)*dim + (j)]
#define DBM2(i, j) dbm2[(i)*dim + (j)]
#define M(i)       (i == 0 ? 0 : m[i - 1])
#define L(i)       (i == 0 ? 0 : l[i - 1])
#define U(i)       (i == 0 ? 0 : u[i - 1])

void universal(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i) {
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::LT_INFINITY;
    DBM(i, i) = tchecker::dbm::LE_ZERO;
  }

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void eca_universal(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                          const std::unordered_set<int> &history_clock_ids,
                                          const std::unordered_set<int> &prophecy_clock_ids, 
                                          const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);


  DBM(0, 0) = tchecker::dbm::LE_ZERO;
  
  for (unsigned int i : normal_clock_ids) {

    for (unsigned int j : history_clock_ids)
        DBM(i, j) = tchecker::dbm::LT_INFINITY;

    for (unsigned int j : prophecy_clock_ids)
      DBM(i, j) = tchecker::dbm::LE_INFINITY;
    
    for (unsigned int j : normal_clock_ids)
      DBM(i,j) = tchecker::dbm::LT_INFINITY;

    DBM(i, 0) = tchecker::dbm::LT_INFINITY;
    DBM(0, i) = tchecker::dbm::LE_ZERO;
    DBM(i, i) = tchecker::dbm::LE_ZERO;  
  }

  for (unsigned int i : history_clock_ids) {

    for (unsigned int j : history_clock_ids)
        DBM(i, j) = tchecker::dbm::LE_INFINITY;

    for (unsigned int j : prophecy_clock_ids)
      DBM(i, j) = tchecker::dbm::LE_INFINITY;
    
    for (unsigned int j : normal_clock_ids)
      DBM(i,j) = tchecker::dbm::LE_INFINITY;

    DBM(i, 0) = tchecker::dbm::LE_INFINITY;
    DBM(0, i) = tchecker::dbm::LE_ZERO;
    DBM(i, i) = tchecker::dbm::LE_INFINITY;  
  }

  for (unsigned int i : prophecy_clock_ids) {
 
    for (unsigned int j : history_clock_ids)
        DBM(i, j) = tchecker::dbm::LE_ZERO;

    for (unsigned int j : prophecy_clock_ids)
      DBM(i, j) = tchecker::dbm::LE_INFINITY;
    
    for (unsigned int j : normal_clock_ids)
      DBM(i, j) = tchecker::dbm::LT_INFINITY;


    DBM(i, 0) = tchecker::dbm::LE_ZERO;
    DBM(0, i) = tchecker::dbm::LE_INFINITY;
    DBM(i, i) = tchecker::dbm::LE_INFINITY;
  }
  

  
  
  

  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids,
                                          prophecy_clock_ids, 
                                          normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
}

void universal_positive(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      DBM(i, j) = ((i == j) || (i == 0) ? tchecker::dbm::LE_ZERO : tchecker::dbm::LT_INFINITY);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void empty(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  tchecker::dbm::universal(dbm, dim);
  DBM(0, 0) = tchecker::dbm::LT_ZERO;
}

void zero(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::LE_ZERO;
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}


void eca_zero(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
              const std::unordered_set<int> &history_clock_ids,
              const std::unordered_set<int> &prophecy_clock_ids, 
              const std::unordered_set<int> &normal_clock_ids)
{ //initial zone of eca
  assert(dbm != nullptr);
  assert(dim >= 1);
  //DBM at index 1 is tmp clock, so not relevant for us
  DBM(0,0) = tchecker::dbm::LE_ZERO;
  //{0} index contains the zero clock

  

  //history-prophecy <= inf
  //prophecy-history <= -inf
  for(auto i:history_clock_ids){
    for(auto j:prophecy_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_INFINITY; //i-j<=inf
      DBM(j,i) = tchecker::dbm::LE_MINUS_INFINITY; //j-i<=-inf
    }
  }
  
  //prophecy-normal <= -inf
  //normal-prophecy <= inf
  for(auto i:prophecy_clock_ids){
    for(auto j:normal_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_ZERO;
      DBM(j,i) = tchecker::dbm::LE_INFINITY;
    }
  }

  //normal-history <= -inf
  //history-normal <= inf
  for(auto i:normal_clock_ids){
    for(auto j:history_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_MINUS_INFINITY;
      DBM(j,i) = tchecker::dbm::LE_INFINITY;
    }
  }

  //history - 0 <= inf
  //0 - history <= -inf
  //history - history <= inf
  for(auto i:history_clock_ids){
    DBM(i,0) = tchecker::dbm::LE_INFINITY; //for history clocks i-0<=INF
    DBM(0,i) = tchecker::dbm::LE_MINUS_INFINITY; //for history clocks 0-i<=-INF
    for(auto j:history_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_INFINITY; //i-j<=inf
    }
  }

  //prophecy - 0 <= 0
  //0- prophecy <= inf
  //prophecy - prophecy <= inf
  for(auto i:prophecy_clock_ids){
    DBM(i,0) = tchecker::dbm::LE_ZERO; //for prophecy clocks i-0<=0
    DBM(0,i) = tchecker::dbm::LE_INFINITY; //for prophecy clocks 0-i<=INF
    DBM(i,i) = tchecker::dbm::LE_INFINITY; //i-i<=0
    for(auto j:prophecy_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_INFINITY; //i-j<=inf
    }
  }


  //normal - 0 <= 0
  //0 - normal <= 0
  //normal - normal <= 0
  for(auto i:normal_clock_ids){
    DBM(i,0) = tchecker::dbm::LE_ZERO; //for normal clocks i-0<=0
    DBM(0,i) = tchecker::dbm::LE_ZERO; //for normal clocks 0-i<=0
    // DBM(i,i) = tchecker::dbm::LE_ZERO; //for normal clocks i-i<=0
    for(auto j:normal_clock_ids){
      DBM(i,j) = tchecker::dbm::LE_ZERO;
    } 
  }
  
  // tchecker::dbm::eca_tighten(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids); 
  //no need to tighten as already tight by construction

  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
}


bool is_consistent(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    if (DBM(i, i) != tchecker::dbm::LE_ZERO)
      return false;
  return true;
}

bool eca_is_consistent(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                        const std::unordered_set<int> &history_clock_ids,
                        const std::unordered_set<int> &prophecy_clock_ids, 
                        const std::unordered_set<int> &normal_clock_ids)
{
  //checks and not asserts
  //history clocks always non-negative
  //prop clocks aleays non-+ve
  assert(dbm != nullptr);
  assert(dim >= 1);
  
  // history clock constraints
  for(auto i:history_clock_ids)
    if (DBM(0,i)>tchecker::dbm::LE_ZERO || DBM(i,0)<tchecker::dbm::LE_ZERO)
      return false;

  for(auto i:normal_clock_ids)
    if (DBM(0,i)>tchecker::dbm::LE_ZERO || DBM(i,0)<tchecker::dbm::LE_ZERO)
      return false;


  // prophecy clock constraints
  for(int i:prophecy_clock_ids)
    if (DBM(0,i)<tchecker::dbm::LE_ZERO || DBM(i,0)>tchecker::dbm::LE_ZERO)
      return false;

  if (DBM(0,0)!= tchecker::dbm::LE_ZERO)
    return false;

  for(auto i:normal_clock_ids)
    if (DBM(i,i)!= tchecker::dbm::LE_ZERO)
      return false;

  for (tchecker::clock_id_t i = 2; i < dim; ++i)
    if (DBM(i, i)!=tchecker::dbm::LE_ZERO || DBM(i, i)!=tchecker::dbm::LE_INFINITY){
      return false;
  }
  return true;
}


bool is_empty_0(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  return (DBM(0, 0) < tchecker::dbm::LE_ZERO);
}

bool eca_is_final_dbm(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                        const std::unordered_set<int> &history_clock_ids,
                        const std::unordered_set<int> &prophecy_clock_ids, 
                        const std::unordered_set<int> &normal_clock_ids){
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(eca_is_tight(dbm,dim));

  for(int i:prophecy_clock_ids)
    if(DBM(0,i) != tchecker::dbm::LE_INFINITY)
      return false;
  return true;
}

bool is_universal(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // <inf everywhere except <=0 on diagonal
  for (tchecker::clock_id_t i = 0; i < dim; ++i) {
    if (DBM(i, i) != tchecker::dbm::LE_ZERO)
      return false;

    for (tchecker::clock_id_t j = 0; j < dim; ++j) {
      if ((i != j) && (DBM(i, j) != tchecker::dbm::LT_INFINITY))
        return false;
    }
  }
  return true;
}

bool is_positive(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t x = 0; x < dim; ++x)
    if (DBM(0, x) > tchecker::dbm::LE_ZERO)
      return false;
  return true;
}

bool is_universal_positive(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t y = 0; y < dim; ++y)
    if (DBM(0, y) != tchecker::dbm::LE_ZERO)
      return false;
  for (tchecker::clock_id_t x = 1; x < dim; ++x) {
    if ((DBM(x, 0) != tchecker::dbm::LT_INFINITY) || (DBM(x, x) != tchecker::dbm::LE_ZERO))
      return false;
    for (tchecker::clock_id_t y = x + 1; y < dim; ++y)
      if ((DBM(x, y) != tchecker::dbm::LT_INFINITY) || (DBM(y, x) != tchecker::dbm::LT_INFINITY))
        return false;
  }
  return true;
}

bool is_tight(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      for (tchecker::clock_id_t k = 0; k < dim; ++k)
        if (tchecker::dbm::sum(DBM(i, k), DBM(k, j)) < DBM(i, j))
          return false;
  return true;
}


bool eca_is_tight(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      for (tchecker::clock_id_t k = 0; k < dim; ++k){
        if (i==1 || j==1 || k==1) continue;
        if (tchecker::dbm::eca_sum(DBM(i, k), DBM(k, j)) < DBM(i, j))
          return false;
      }
  return true;
}



enum tchecker::dbm::status_t tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);

  for (tchecker::clock_id_t k = 0; k < dim; ++k) {
    for (tchecker::clock_id_t i = 0; i < dim; ++i) {
      if ((i == k) || (DBM(i, k) == tchecker::dbm::LT_INFINITY)) // optimization
        continue;
      for (tchecker::clock_id_t j = 0; j < dim; ++j)
        DBM(i, j) = tchecker::dbm::min(tchecker::dbm::sum(DBM(i, k), DBM(k, j)), DBM(i, j));
      if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
        DBM(0, 0) = tchecker::dbm::LT_ZERO;
        return tchecker::dbm::EMPTY;
      }
    }
  }
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  return tchecker::dbm::NON_EMPTY;
}

enum tchecker::dbm::status_t eca_tighten2(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
              const std::unordered_set<int> &history_clock_ids,
              const std::unordered_set<int> &prophecy_clock_ids, 
              const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  //since clock with index 1 is the tmp clock


  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    DBM(1,i) = tchecker::dbm::LE_INFINITY;
    DBM(i,1) = tchecker::dbm::LE_INFINITY;
  }
  DBM(1,0) = tchecker::dbm::LE_ZERO;
  DBM(0,1) = tchecker::dbm::LE_INFINITY;
  

  for (tchecker::clock_id_t i = 0; i < dim; ++i) {
    for (tchecker::clock_id_t k = 0; k < dim; ++k) {
      if ((i == k) || (DBM(i, k) == tchecker::dbm::LE_INFINITY)) // optimization
        continue;
      for (tchecker::clock_id_t j = 0; j < dim; ++j){
        DBM(i, j) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(i, k), DBM(k, j)), DBM(i, j));
      }
      
      if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
        DBM(0, 0) = tchecker::dbm::LT_ZERO;
        return tchecker::dbm::EMPTY;
      }
    }
  }
  
  
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids,
                                        prophecy_clock_ids, 
                                        normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));

  return tchecker::dbm::NON_EMPTY;
}

enum tchecker::dbm::status_t eca_tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
              const std::unordered_set<int> &history_clock_ids,
              const std::unordered_set<int> &prophecy_clock_ids, 
              const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  //since clock with index 1 is the tmp clock

  for(tchecker::clock_id_t k = 2;k<dim;++k){

    if(DBM(0,k) == tchecker::dbm::LE_INFINITY) continue;

    for (tchecker::clock_id_t j = 2; j < dim; ++j){
      DBM(0, j) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(0, k), DBM(k, j)), DBM(0, j));
    }

    DBM(0, 0) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(0, k), DBM(k, 0)), DBM(0, 0));

    if (DBM(0, 0) < tchecker::dbm::LE_ZERO) {
      DBM(0, 0) = tchecker::dbm::LT_ZERO;
      return tchecker::dbm::EMPTY;
    }
  }


  for(tchecker::clock_id_t i = 2;i<dim;++i){
    
    if(DBM(i,0) == tchecker::dbm::LE_INFINITY) continue;
    
    for (tchecker::clock_id_t j = 2; j < dim; ++j){
      DBM(i, j) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(i, 0), DBM(0, j)), DBM(i, j));
    }
    
    DBM(i, 0) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(i, 0), DBM(0, 0)), DBM(i, 0));//not required i think ani:-101 optimization possible
    
    if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
        DBM(0, 0) = tchecker::dbm::LT_ZERO;
        return tchecker::dbm::EMPTY;
    }
  }

  for(tchecker::clock_id_t i = 2;i<dim;++i){
    for (tchecker::clock_id_t k = 2; k < dim; ++k){
      if(i==k || DBM(i,k) == tchecker::dbm::LE_INFINITY) continue;
      DBM(i, 0) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(i, k), DBM(k, 0)), DBM(i, 0));
    }
    if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
      DBM(0, 0) = tchecker::dbm::LT_ZERO;
      return tchecker::dbm::EMPTY;
    }
  }


  for (tchecker::clock_id_t i = 2; i < dim; ++i) {
    for (tchecker::clock_id_t k = 2; k < dim; ++k) {
      if ((i == k) || (DBM(i, k) == tchecker::dbm::LE_INFINITY)) // optimization
        continue;
      for (tchecker::clock_id_t j = 2; j < dim; ++j){
        DBM(i, j) = tchecker::dbm::min(tchecker::dbm::eca_sum(DBM(i, k), DBM(k, j)), DBM(i, j));
      }
      
      if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
        DBM(0, 0) = tchecker::dbm::LT_ZERO;
        return tchecker::dbm::EMPTY;
      }
    }
  }
  
  
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids,
                                        prophecy_clock_ids, 
                                        normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));

  return tchecker::dbm::NON_EMPTY;
}


enum tchecker::dbm::status_t tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                     tchecker::clock_id_t y)
{
  assert(dbm != nullptr);
  assert(dim >= 1);

  if (DBM(x, y) == tchecker::dbm::LT_INFINITY)
    return tchecker::dbm::MAY_BE_EMPTY;

  for (tchecker::clock_id_t i = 0; i < dim; ++i) {
    // tighten i->y w.r.t. i->x->y
    if (i != x) {
      tchecker::dbm::db_t db_ixy = tchecker::dbm::sum(DBM(i, x), DBM(x, y));
      if (db_ixy < DBM(i, y))
        DBM(i, y) = db_ixy;
    }

    // tighten i->j w.r.t. i->y->j
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::min(DBM(i, j), tchecker::dbm::sum(DBM(i, y), DBM(y, j)));

    if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
      DBM(0, 0) = tchecker::dbm::LT_ZERO;
      return tchecker::dbm::EMPTY;
    }
  }

  return tchecker::dbm::MAY_BE_EMPTY;
}

enum tchecker::dbm::status_t eca_tighten(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
                                     tchecker::clock_id_t x,
                                     tchecker::clock_id_t y)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert(dim%2 == 1);
  
  // std::cout << "ani:541 before tightning \n";
  // output_matrix(std::cout, dbm, dim);
  // std::cout << std::endl;

  //changed this from LT_INFINITIY TO LE_INFINITY!
  if (DBM(x, y) == tchecker::dbm::LE_INFINITY)
    return tchecker::dbm::MAY_BE_EMPTY;

  //tighten 0->y w.r.t 0->x->y
  tchecker::dbm::db_t db_0xy = tchecker::dbm::eca_sum(DBM(0, x), DBM(x, y));
  if (db_0xy < DBM(0, y))
    DBM(0, y) = db_0xy;
  
  // tighten 0->0 w.r.t. 0->y->0
  DBM(0, 0) = tchecker::dbm::min(DBM(0, 0), tchecker::dbm::eca_sum(DBM(0, y), DBM(y, 0)));

  // tighten 0->j w.r.t. 0->y->j
  for (tchecker::clock_id_t j = 2; j < dim; ++j)
    DBM(0, j) = tchecker::dbm::min(DBM(0, j), tchecker::dbm::eca_sum(DBM(0, y), DBM(y, j)));

  if (DBM(0, 0) < tchecker::dbm::LE_ZERO) {
    DBM(0, 0) = tchecker::dbm::LT_ZERO;
    return tchecker::dbm::EMPTY;
  }

  for (tchecker::clock_id_t i = 2; i < dim; ++i) {
    // tighten i->y w.r.t. i->x->y
    if (i != x) {
      tchecker::dbm::db_t db_ixy = tchecker::dbm::eca_sum(DBM(i, x), DBM(x, y));
      if (db_ixy < DBM(i, y))
        DBM(i, y) = db_ixy;
    }

    // tighten i->0 w.r.t. i->y->0
    DBM(i, 0) = tchecker::dbm::min(DBM(i, 0), tchecker::dbm::eca_sum(DBM(i, y), DBM(y, 0)));

    // tighten i->j w.r.t. i->y->j
    for (tchecker::clock_id_t j = 2; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::min(DBM(i, j), tchecker::dbm::eca_sum(DBM(i, y), DBM(y, j)));

    if (DBM(i, i) < tchecker::dbm::LE_ZERO) {
      DBM(0, 0) = tchecker::dbm::LT_ZERO;
      return tchecker::dbm::EMPTY;
    }
  }

  return tchecker::dbm::MAY_BE_EMPTY;
}



enum tchecker::dbm::status_t constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x,
                                       tchecker::clock_id_t y, tchecker::dbm::comparator_t cmp, tchecker::integer_t value)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  assert(x < dim);
  assert(y < dim);

  tchecker::dbm::db_t db = tchecker::dbm::db(cmp, value);
  if (db >= DBM(x, y))
    return tchecker::dbm::NON_EMPTY;

  DBM(x, y) = db;

  auto res = tchecker::dbm::tighten(dbm, dim, x, y);

  if (res == tchecker::dbm::MAY_BE_EMPTY)
    res = tchecker::dbm::NON_EMPTY; // since dbm was tight before

  assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::is_consistent(dbm, dim));
  assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::is_tight(dbm, dim));

  return res;
}



enum tchecker::dbm::status_t eca_constrain_old(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
                                            tchecker::clock_id_t x, tchecker::clock_id_t y, 
                                            tchecker::dbm::comparator_t cmp, tchecker::integer_t value)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert(tchecker::dbm::eca_is_consistent(dbm, dim));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
  assert(x < dim);
  assert(y < dim);

  tchecker::dbm::db_t db = tchecker::dbm::db(cmp, value);
  if (db >= DBM(x, y))
    return tchecker::dbm::NON_EMPTY;

  DBM(x, y) = db;



  auto res = tchecker::dbm::eca_tighten(dbm, dim, x, y);

  if (res == tchecker::dbm::MAY_BE_EMPTY)
    res = tchecker::dbm::NON_EMPTY; // since dbm was tight before

  // assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::eca_is_consistent(dbm, dim));
  assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::eca_is_tight(dbm, dim));

  return res;
}



enum tchecker::dbm::status_t eca_constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
                                            tchecker::clock_id_t x, tchecker::clock_id_t y, 
                                            tchecker::dbm::comparator_t cmp, tchecker::integer_t value,
                                            const std::unordered_set<int> &history_clock_ids,
                                            const std::unordered_set<int> &prophecy_clock_ids, 
                                            const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert(tchecker::dbm::eca_is_consistent(dbm, dim));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
  assert(x < dim);
  assert(y < dim);

  tchecker::dbm::db_t db = tchecker::dbm::db(cmp, value);
  if (db >= DBM(x, y))
    return tchecker::dbm::NON_EMPTY;

  DBM(x, y) = db;

  if (x!=0 && y!=0){
    //standardize
    if(prophecy_clock_ids.find(x)!=prophecy_clock_ids.end() && DBM(0,x)==tchecker::dbm::LE_INFINITY){
      for(int j=2;j<dim;j++)
        if(x!=j && DBM(j,x)!=tchecker::dbm::LE_INFINITY){
          DBM(0,x)= tchecker::dbm::LT_INFINITY;
          break;
        }
    }

    if(prophecy_clock_ids.find(y)!=prophecy_clock_ids.end() && DBM(0,y)==tchecker::dbm::LE_INFINITY){
      for(int j=2;j<dim;j++)
        if(y!=j && DBM(j,y)!=tchecker::dbm::LE_INFINITY){
          DBM(0,y)= tchecker::dbm::LT_INFINITY;
          break;
        }
    }

    if((history_clock_ids.find(y)!=history_clock_ids.end() || normal_clock_ids.find(y)!=normal_clock_ids.end())
         && DBM(y,0)==tchecker::dbm::LE_INFINITY){
      for(int j=2;j<dim;j++)
        if(y!=j && DBM(y,j)!=tchecker::dbm::LE_INFINITY){
          DBM(y,0)= tchecker::dbm::LT_INFINITY;
          break;
        }
    }
    
    if((history_clock_ids.find(x)!=history_clock_ids.end() || normal_clock_ids.find(x)!=normal_clock_ids.end())
     && DBM(x,0)==tchecker::dbm::LE_INFINITY){
      for(int j=2;j<dim;j++)
        if(x!=j && DBM(x,j)!=tchecker::dbm::LE_INFINITY){
          DBM(x,0)= tchecker::dbm::LT_INFINITY;
          break;
        }
    }
    
  }

  auto res = tchecker::dbm::eca_tighten(dbm, dim, x, y);

  if (res == tchecker::dbm::MAY_BE_EMPTY)
    res = tchecker::dbm::NON_EMPTY; // since dbm was tight before

  // assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::eca_is_consistent(dbm, dim));
  assert((res == tchecker::dbm::EMPTY) || tchecker::dbm::eca_is_tight(dbm, dim));

  return res;
}

enum tchecker::dbm::status_t constrain(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                       tchecker::clock_constraint_container_t const & constraints)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  for (tchecker::clock_constraint_t const & c : constraints) {
    tchecker::clock_id_t id1 = (c.id1() == tchecker::REFCLOCK_ID ? 0 : c.id1() + 1);
    tchecker::clock_id_t id2 = (c.id2() == tchecker::REFCLOCK_ID ? 0 : c.id2() + 1);
    auto cmp = (c.comparator() == tchecker::clock_constraint_t::LT ? tchecker::dbm::LT : tchecker::dbm::LE);
    if (tchecker::dbm::constrain(dbm, dim, id1, id2, cmp, c.value()) == tchecker::dbm::EMPTY)
      return tchecker::dbm::EMPTY;
  }
  return tchecker::dbm::NON_EMPTY;
}

enum tchecker::dbm::status_t eca_constrain_single(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                                        tchecker::clock_constraint_t const & constraint,
                                        const std::unordered_set<int> &history_clock_ids,
                                        const std::unordered_set<int> &prophecy_clock_ids, 
                                        const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert(tchecker::dbm::eca_is_consistent(dbm, dim));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));

  tchecker::clock_id_t id1 = (constraint.id1() == tchecker::REFCLOCK_ID ? 0 : constraint.id1() + 1);
  tchecker::clock_id_t id2 = (constraint.id2() == tchecker::REFCLOCK_ID ? 0 : constraint.id2() + 1);
  auto cmp = (constraint.comparator() == tchecker::clock_constraint_t::LT ? tchecker::dbm::LT : tchecker::dbm::LE);
  if (tchecker::dbm::eca_constrain(dbm, dim, id1, id2, cmp, constraint.value(),history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::EMPTY)
    return tchecker::dbm::EMPTY;
    
  return tchecker::dbm::NON_EMPTY;
}





bool is_equal(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      if (DBM1(i, j) != DBM2(i, j))
        return false;
  return true;
}

bool is_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      if (DBM1(i, j) > DBM2(i, j))
        return false;
  return true;
}

void reset(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
           tchecker::integer_t value)
{
  if (y == 0)
    reset_to_value(dbm, dim, x, value);
  else if (value == 0)
    reset_to_clock(dbm, dim, x, y);
  else if (value > 0)
    reset_to_sum(dbm, dim, x, y, value);
  else
    reset_to_subtraction(dbm, dim, x, y, value);
}

void reset(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_reset_container_t const & resets)
{
  for (tchecker::clock_reset_t const & r : resets) {
    tchecker::clock_id_t lid = (r.left_id() == tchecker::REFCLOCK_ID ? 0 : r.left_id() + 1);
    tchecker::clock_id_t rid = (r.right_id() == tchecker::REFCLOCK_ID ? 0 : r.right_id() + 1);
    tchecker::dbm::reset(dbm, dim, lid, rid, r.value());
  }
}

void eca_reset_caller(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t reset_id,
                      const std::unordered_set<int> &history_clock_ids,
                      const std::unordered_set<int> &prophecy_clock_ids, 
                      const std::unordered_set<int> &normal_clock_ids){
  // assert(dim%2==1);
  assert(reset_id!=tchecker::REFCLOCK_ID);
  // assert(reset_id%2==0);
  
  eca_reset(dbm, dim, reset_id+1, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
}


void eca_reset(tchecker::dbm::db_t * dbm, 
                tchecker::clock_id_t dim, 
                tchecker::clock_id_t reset_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids){

  DBM(0,reset_id) = tchecker::dbm::LE_ZERO;
  DBM(reset_id,0) = tchecker::dbm::LE_ZERO;

  // for (tchecker::clock_id_t i = 1; i < dim; ++i) {
  //   DBM(reset_id, i) = tchecker::dbm::LE_INFINITY;
  //   DBM(i, reset_id) = tchecker::dbm::LE_INFINITY;
  // }

  for (tchecker::clock_id_t i = 2; i < dim; ++i) {
    DBM(reset_id, i) = DBM(0, i);
    DBM(i, reset_id) = DBM(i, 0);
  }


  // tchecker::dbm::eca_tighten(dbm,dim,
  //             history_clock_ids,
  //             prophecy_clock_ids, 
  //             normal_clock_ids);

  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
}

void reset_to_value(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::integer_t value)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  assert(x < dim);
  assert(0 <= value);

  // set x == value
  DBM(x, 0) = tchecker::dbm::db(tchecker::dbm::LE, value);
  DBM(0, x) = tchecker::dbm::db(tchecker::dbm::LE, -value);

  // tighten: x->y is set to x->0->y and y->x to y->0->x for all y!=0
  for (tchecker::clock_id_t y = 1; y < dim; ++y) {
    DBM(x, y) = tchecker::dbm::sum(DBM(x, 0), DBM(0, y));
    DBM(y, x) = tchecker::dbm::sum(DBM(y, 0), DBM(0, x));
  }

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void reset_to_clock(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  assert(x < dim);
  assert(0 < y);
  assert(y < dim);

  // x is identified to y w.r.t. all clocks z
  for (tchecker::clock_id_t z = 0; z < dim; ++z) {
    DBM(x, z) = DBM(y, z);
    DBM(z, x) = DBM(z, y);
  }
  DBM(x, x) = tchecker::dbm::LE_ZERO; // cheaper than testing in loop

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void reset_to_sum(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
                  tchecker::integer_t value)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  assert(x < dim);
  assert(y < dim);
  assert(0 <= value);

  // NB: this is a reset not a constraint. Hence, resetting x:=y+value
  // only impacts difference bounds involving x (i.e. x-z and z-x
  // for all z including y and 0). Other difference bounds u-v for
  // any u,v distinct from x are not touched since the values of u
  // and v are not modified when x is reset to y+value

  // If x == y:
  // x' - x <= v & x - z <= d & z = z' --> x' - z' <= d+v
  // x - x' <= -v & z - x <= d & z = z' --> z' - x' <= d-v

  // If x != y:
  // x - y <= v & y - z <= d --> x - z <= d+v
  // y - x <= -v & z - y <= d --> z - x <= d-v
  for (tchecker::clock_id_t z = 0; z < dim; ++z) {
    DBM(x, z) = tchecker::dbm::add(DBM(y, z), value);
    DBM(z, x) = tchecker::dbm::add(DBM(z, y), -value);
  }
  DBM(x, x) = tchecker::dbm::LE_ZERO; // cheaper than testing in loop

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void reset_to_subtraction(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::clock_id_t x, tchecker::clock_id_t y,
                  tchecker::integer_t value)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
  assert(x < dim);
  assert(y < dim);
  assert(value < 0);

  // NB: this is a reset not a constraint. Hence, resetting x:=y+value
  // only impacts difference bounds involving x (i.e. x-z and z-x
  // for all z including y and 0). Other difference bounds u-v for
  // any u,v distinct from x are not touched since the values of u
  // and v are not modified when x is reset to y+value

  // If x == y:
  // x' - x <= v & x - z <= d & z = z' --> x' - z' <= d+v
  // x - x' <= -v & z - x <= d & z = z' --> z' - x' <= d-v

  // If x != y:
  // x - y <= v & y - z <= d --> x - z <= d+v
  // y - x <= -v & z - y <= d --> z - x <= d-v
  for (tchecker::clock_id_t z = 0; z < dim; ++z) {
    DBM(x, z) = tchecker::dbm::add(DBM(y, z), value);
    DBM(z, x) = tchecker::dbm::add(DBM(z, y), -value);
  }
  DBM(x, x) = tchecker::dbm::LE_ZERO; // cheaper than testing in loop

  tchecker::dbm::db_t dbm_univ_pos[dim*dim];
  tchecker::dbm::universal_positive(dbm_univ_pos, dim);
  tchecker::dbm::intersection(dbm, dbm, dbm_univ_pos, dim);

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void open_up(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  for (tchecker::clock_id_t i = 1; i < dim; ++i)
    DBM(i, 0) = tchecker::dbm::LT_INFINITY;

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void eca_open_up(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, 
                  const std::unordered_set<int> &history_clock_ids,
                  const std::unordered_set<int> &prophecy_clock_ids,
                  const std::unordered_set<int> &normal_clock_ids){
  assert(dbm != nullptr);
  assert(dim >= 1);
  
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));

  for(auto i:history_clock_ids){
    if(DBM(i,0)<tchecker::dbm::LT_INFINITY)
      DBM(i,0) = tchecker::dbm::LT_INFINITY;
  }
  for(auto i:normal_clock_ids){
    DBM(i,0) = tchecker::dbm::LT_INFINITY;
  }
  for(auto i:prophecy_clock_ids){
    if(DBM(i,0)!=tchecker::dbm::LE_MINUS_INFINITY)
      DBM(i,0) = tchecker::dbm::LE_ZERO;
  }

  
  if(history_clock_ids.size()>0 || prophecy_clock_ids.size()>0){
    tchecker::dbm::eca_tighten(dbm,dim,
                  history_clock_ids,
                  prophecy_clock_ids, 
                  normal_clock_ids);
  }

  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
}


void eca_release_caller(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                tchecker::clock_id_t prophecy_clock_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert((dim%2) == 1);
  assert(prophecy_clock_id!=tchecker::REFCLOCK_ID);
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
  eca_release(dbm, dim, prophecy_clock_id+1,history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
}


void eca_release(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim,
                tchecker::clock_id_t prophecy_clock_id,
                const std::unordered_set<int> &history_clock_ids,
                const std::unordered_set<int> &prophecy_clock_ids, 
                const std::unordered_set<int> &normal_clock_ids)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  // assert((dim%2) == 1);
  
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
  
  DBM(0,prophecy_clock_id) = tchecker::dbm::LE_INFINITY;
  DBM(prophecy_clock_id,0) = tchecker::dbm::LE_ZERO;
  
  for(tchecker::clock_id_t i=2;i<dim;i++){
    DBM(i,prophecy_clock_id) = tchecker::dbm::LE_INFINITY;
    DBM(prophecy_clock_id,i) = DBM(0,i);
  }

  DBM(prophecy_clock_id,prophecy_clock_id) = tchecker::dbm::LE_INFINITY;
  
  // tchecker::dbm::eca_tighten(dbm,dim,
  //               history_clock_ids,
  //               prophecy_clock_ids, 
  //               normal_clock_ids);
  
  assert(tchecker::dbm::eca_is_consistent(dbm, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm, dim));
}











enum tchecker::dbm::status_t eca_intersection(tchecker::dbm::db_t * dbm, tchecker::dbm::db_t const * dbm1,
                                          tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
                                          const std::unordered_set<int> &history_clock_ids,
                                          const std::unordered_set<int> &prophecy_clock_ids, 
                                          const std::unordered_set<int> &normal_clock_ids)
{
  assert(dim >= 1);
  assert(dbm != nullptr);
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(tchecker::dbm::eca_is_consistent(dbm1, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_consistent(dbm2, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm1, dim));
  assert(tchecker::dbm::eca_is_tight(dbm2, dim));

  DBM(0, 0) = tchecker::dbm::min(DBM1(0, 0), DBM2(0, 0));

  for (tchecker::clock_id_t i = 2; i < dim; ++i){
    DBM(i, 0) = tchecker::dbm::min(DBM1(i, 0), DBM2(i, 0));
    DBM(0, i) = tchecker::dbm::min(DBM1(0, i), DBM2(0, i));
  }
  
  for (tchecker::clock_id_t i = 2; i < dim; ++i)
    for (tchecker::clock_id_t j = 2; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::min(DBM1(i, j), DBM2(i, j));

  return tchecker::dbm::eca_tighten(dbm, dim,
                history_clock_ids,
                prophecy_clock_ids, 
                normal_clock_ids);
}

enum tchecker::dbm::status_t intersection(tchecker::dbm::db_t * dbm, tchecker::dbm::db_t const * dbm1,
                                          tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim)
{
  assert(dim >= 1);
  assert(dbm != nullptr);
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(tchecker::dbm::is_consistent(dbm1, dim));
  assert(tchecker::dbm::is_consistent(dbm2, dim));
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  for (tchecker::clock_id_t i = 0; i < dim; ++i)
    for (tchecker::clock_id_t j = 0; j < dim; ++j)
      DBM(i, j) = tchecker::dbm::min(DBM1(i, j), DBM2(i, j));

  return tchecker::dbm::tighten(dbm, dim);
}

void extra_m(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * m)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  bool modified = false;

  // let DBM(i,j) be (#,cij)
  // DBM(i,j) becomes  <inf    if  cij > M(i)
  //                   <-M(j)  if -cij > M(j)
  //          unchanged otherwise

  // i=0 (first row), only the second case applies
  for (tchecker::clock_id_t j = 1; j < dim; ++j) {
    tchecker::integer_t Mj = M(j);
    assert(Mj < tchecker::dbm::INF_VALUE);
    if (DBM(0, j) == tchecker::dbm::LE_ZERO)
      continue;
    tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
    if (-c0j > Mj) {
      DBM(0, j) = (Mj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LE_ZERO : tchecker::dbm::db(tchecker::dbm::LT, -Mj));
      modified = true;
    }
  }

  // i>0, both cases apply
  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    tchecker::integer_t Mi = M(i);
    assert(Mi < tchecker::dbm::INF_VALUE);

    for (tchecker::clock_id_t j = 0; j < dim; ++j) {
      tchecker::integer_t Mj = M(j);
      assert(Mj < tchecker::dbm::INF_VALUE);

      if (i == j)
        continue;
      if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
        continue;

      tchecker::integer_t cij = tchecker::dbm::value(DBM(i, j));
      if (cij > Mi) {
        DBM(i, j) = tchecker::dbm::LT_INFINITY;
        modified = true;
      }
      else if (-cij > Mj) {
        DBM(i, j) = (Mj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LT_INFINITY : tchecker::dbm::db(tchecker::dbm::LT, -Mj));
        modified = true;
      }
    }
  }

  if (modified)
    tchecker::dbm::tighten(dbm, dim);

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void extra_m_plus(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * m)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  bool modified = false;

  // let DBM(i,j) be (#,cij)
  // DBM(i,j) becomes  <inf    if  cij > M(i)
  //                   <inf    if -c0i > M(i)
  //                   <inf    if -c0j > M(j), i != 0
  //                   <-M(j)  if -cij > M(j), i  = 0
  //          unchanged otherwise

  // NB: the first line (i.e. 0-line) must be modified last to keep
  // c0i and c0j intact for 2nd and 3rd cases

  // i > 0, all cases except the 4th apply
  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    tchecker::integer_t Mi = M(i);
    assert(Mi < tchecker::dbm::INF_VALUE);

    tchecker::integer_t c0i = tchecker::dbm::value(DBM(0, i));

    if (-c0i > Mi) { // 2nd case
      for (tchecker::clock_id_t j = 0; j < dim; ++j) {
        if (i == j)
          continue;
        if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
          continue;

        DBM(i, j) = tchecker::dbm::LT_INFINITY;
        modified = true;
      }
    }
    else { // 1st and 3rd cases apply
      for (tchecker::clock_id_t j = 0; j < dim; ++j) {
        tchecker::integer_t Mj = M(j);
        assert(Mj < tchecker::dbm::INF_VALUE);

        if (i == j)
          continue;
        if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
          continue;

        tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
        tchecker::integer_t cij = tchecker::dbm::value(DBM(i, j));

        if (cij > Mi || -c0j > Mj) {
          DBM(i, j) = tchecker::dbm::LT_INFINITY;
          modified = true;
        }
      }
    }
  }

  // i = 0, only the 4th case apply
  assert(M(0) < tchecker::dbm::INF_VALUE);
  for (tchecker::clock_id_t j = 1; j < dim; ++j) {
    tchecker::integer_t Mj = M(j);
    assert(Mj < tchecker::dbm::INF_VALUE);

    tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
    if (-c0j > Mj) {
      DBM(0, j) = (Mj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LE_ZERO : tchecker::dbm::db(tchecker::dbm::LT, -Mj));
      modified = true;
    }
  }

  if (modified)
    tchecker::dbm::tighten(dbm, dim);

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void extra_lu(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * l, tchecker::integer_t const * u)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  bool modified = false;

  // let DBM(i,j) be (#,cij)
  // DBM(i,j) becomes  <inf    if  cij > L(i)
  //                   <-U(j)  if -cij > U(j)
  //          unchanged otherwise

  // i=0 (first row), only second case applies
  for (tchecker::clock_id_t j = 1; j < dim; ++j) {
    tchecker::integer_t Uj = U(j);
    assert(Uj < tchecker::dbm::INF_VALUE);
    if (DBM(0, j) == tchecker::dbm::LE_ZERO)
      continue;
    tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
    if (-c0j > Uj) {
      DBM(0, j) = (Uj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LE_ZERO : tchecker::dbm::db(tchecker::dbm::LT, -Uj));
      modified = true;
    }
  }

  // i>0, both cases apply
  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    tchecker::integer_t Li = L(i);
    assert(Li < tchecker::dbm::INF_VALUE);
    assert(U(i) < tchecker::dbm::INF_VALUE);

    for (tchecker::clock_id_t j = 0; j < dim; ++j) {
      assert(L(j) < tchecker::dbm::INF_VALUE);
      tchecker::integer_t Uj = U(j);
      assert(Uj < tchecker::dbm::INF_VALUE);

      if (i == j)
        continue;
      if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
        continue;

      tchecker::integer_t cij = tchecker::dbm::value(DBM(i, j));
      if (cij > Li) {
        DBM(i, j) = tchecker::dbm::LT_INFINITY;
        modified = true;
      }
      else if (-cij > Uj) {
        DBM(i, j) = (Uj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LT_INFINITY : tchecker::dbm::db(tchecker::dbm::LT, -Uj));
        modified = true;
      }
    }
  }

  if (modified)
    tchecker::dbm::tighten(dbm, dim);

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

void extra_lu_plus(tchecker::dbm::db_t * dbm, tchecker::clock_id_t dim, tchecker::integer_t const * l,
                   tchecker::integer_t const * u)
{
  assert(dbm != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));

  bool modified = false;

  // let DBM(i,j) be (#,cij)
  // DBM(i,j) becomes  <inf    if  cij > L(i)
  //                   <inf    if -c0i > L(i)
  //                   <inf    if -c0j > U(j), i != 0
  //                   <-U(j)  if -c0j > U(j), i  = 0
  //          unchanged otherwise

  // NB: the first line (i.e. 0-line) must be modified last to keep
  // c0i and c0j intact for 2nd, 3rd and 4th cases

  // i > 0, all cases except the 4th apply
  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    tchecker::integer_t Li = L(i);
    assert(Li < tchecker::dbm::INF_VALUE);
    assert(U(i) < tchecker::dbm::INF_VALUE);

    tchecker::integer_t c0i = tchecker::dbm::value(DBM(0, i));

    if (-c0i > Li) { // 2nd case
      for (tchecker::clock_id_t j = 0; j < dim; ++j) {
        if (i == j)
          continue;
        if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
          continue;

        DBM(i, j) = tchecker::dbm::LT_INFINITY;
        modified = true;
      }
    }
    else { // 1st and 3rd cases apply
      for (tchecker::clock_id_t j = 0; j < dim; ++j) {
        assert(L(j) < tchecker::dbm::INF_VALUE);
        tchecker::integer_t Uj = U(j);
        assert(Uj < tchecker::dbm::INF_VALUE);

        if (i == j)
          continue;
        if (DBM(i, j) == tchecker::dbm::LT_INFINITY)
          continue;

        tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
        tchecker::integer_t cij = tchecker::dbm::value(DBM(i, j));

        if (cij > Li || -c0j > Uj) {
          DBM(i, j) = tchecker::dbm::LT_INFINITY;
          modified = true;
        }
      }
    }
  }

  // i = 0, only the 4th case apply
  assert(L(0) < tchecker::dbm::INF_VALUE);
  assert(U(0) < tchecker::dbm::INF_VALUE);

  for (tchecker::clock_id_t j = 1; j < dim; ++j) {
    assert(L(j) < tchecker::dbm::INF_VALUE);
    tchecker::integer_t Uj = U(j);
    assert(Uj < tchecker::dbm::INF_VALUE);

    tchecker::integer_t c0j = tchecker::dbm::value(DBM(0, j));
    if (-c0j > Uj) {
      DBM(0, j) = (Uj == -tchecker::dbm::INF_VALUE ? tchecker::dbm::LE_ZERO : tchecker::dbm::db(tchecker::dbm::LT, -Uj));
      modified = true;
    }
  }

  if (modified)
    tchecker::dbm::tighten(dbm, dim);

  assert(tchecker::dbm::is_consistent(dbm, dim));
  assert(tchecker::dbm::is_positive(dbm, dim));
  assert(tchecker::dbm::is_tight(dbm, dim));
}

bool is_alu_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
               tchecker::integer_t const * l, tchecker::integer_t const * u)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm1, dim));
  assert(tchecker::dbm::is_consistent(dbm2, dim));
  assert(tchecker::dbm::is_positive(dbm1, dim));
  assert(tchecker::dbm::is_positive(dbm2, dim));
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  // dbm1 not included in aLU(dbm2) if there is x and y s.t.
  //     dbm1[0x] >= (<= -U(x))
  // &&  dbm2[yx] < dbm1[yx]
  // &&  dbm2[yx] + (< -L(y)) < dbm1[0x]

  for (tchecker::clock_id_t x = 0; x < dim; ++x) {
    tchecker::integer_t Ux = U(x);
    assert(Ux < tchecker::dbm::INF_VALUE);

    // Skip x as 1st condition cannot be satisfied
    if (Ux == -tchecker::dbm::INF_VALUE)
      continue;

    // Check 1st condition
    if (DBM1(0, x) < tchecker::dbm::db(tchecker::dbm::LE, -Ux))
      continue;

    for (tchecker::clock_id_t y = 0; y < dim; ++y) {
      tchecker::integer_t Ly = L(y);
      assert(Ly < tchecker::dbm::INF_VALUE);

      if (x == y)
        continue;

      // Skip y as 3rd condition cannot be satisfied
      if (Ly == -tchecker::dbm::INF_VALUE)
        continue;

      // Check 2nd and 3rd conditions
      if (DBM2(y, x) < DBM1(y, x) && tchecker::dbm::sum(DBM2(y, x), tchecker::dbm::db(tchecker::dbm::LT, -Ly)) < DBM1(0, x))
        return false;
    }
  }

  return true;
}

bool is_am_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
              tchecker::integer_t const * m)
{
  return tchecker::dbm::is_alu_le(dbm1, dbm2, dim, m, m);
}

bool is_g_le_nd(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
                std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf)
{

  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm1, dim));
  assert(tchecker::dbm::is_consistent(dbm2, dim));
  assert(tchecker::dbm::is_positive(dbm1, dim));
  assert(tchecker::dbm::is_positive(dbm2, dim));
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  // From Gdf construct Gdf_u and Gdf_l
  // Gdf_u (resp. Gdf_l) consists of upper (resp. lower) bound constraints
  // Gdf = Gdf_u U Gdf_l
  std::vector<tchecker::typed_simple_clkconstr_expression_t const *> Gdf_u{}, Gdf_l{};

  //collecting diagonal free upper and lower guards
  for (tchecker::typed_simple_clkconstr_expression_t const * nondiag : Gdf)
  {
    if (nondiag->binary_operator() == EXPR_OP_LE ||
        nondiag->binary_operator() == EXPR_OP_LT)
      Gdf_u.push_back(nondiag);
    else if (nondiag->binary_operator() == EXPR_OP_GE ||
             nondiag->binary_operator() == EXPR_OP_GT)
      Gdf_l.push_back(nondiag);
    else
      throw std::invalid_argument("unexpected expression");
  }

  // condition 1
  for (tchecker::typed_simple_clkconstr_expression_t const * phi_u : Gdf_u)
  {
    tchecker::clock_id_t x = tchecker::extract_lvalue_variable_ids(phi_u->clock()).begin();
    x = x+1;//because of reference clock being 0?
    tchecker::integer_t phi_bound = tchecker::const_evaluate(phi_u->bound());

    assert(phi_bound >= 0);
    tchecker::dbm::comparator_t cmp = phi_u->binary_operator() == EXPR_OP_LT ? 
                                      tchecker::dbm::LT : tchecker::dbm::LE;

    if ( (tchecker::dbm::sum(DBM1(0,x), tchecker::dbm::db(cmp, phi_bound)) >=
          tchecker::dbm::LE_ZERO) &&
         (DBM2(0,x) < DBM1(0,x)) ) return false;
  }

  // condition 2
  for (tchecker::typed_simple_clkconstr_expression_t const * phi_l : Gdf_l)
  {
    tchecker::clock_id_t y = tchecker::extract_lvalue_variable_ids(phi_l->clock()).begin();
    y = y + 1;
    tchecker::integer_t phi_bound = tchecker::const_evaluate(phi_l->bound());
    assert(phi_bound >= 0);
    tchecker::dbm::comparator_t cmp = phi_l->binary_operator() == EXPR_OP_GT ? 
                                      tchecker::dbm::LT : tchecker::dbm::LE;

    if ( (tchecker::dbm::sum(DBM2(y,0), tchecker::dbm::db(cmp, -phi_bound)) < tchecker::dbm::LE_ZERO) &&
         (DBM2(y,0) < DBM1(y,0)) ) return false;
  }

  // condition 3
  if (Gdf_u.size() == 0 || Gdf_l.size() == 0) 
    return true;

  for (tchecker::typed_simple_clkconstr_expression_t const * phi_u : Gdf_u)
  {
    tchecker::clock_id_t x = tchecker::extract_lvalue_variable_ids(phi_u->clock()).begin();
    x = x + 1;
    tchecker::dbm::comparator_t cmp1 = phi_u->binary_operator() == EXPR_OP_LT ? tchecker::dbm::LT : tchecker::dbm::LE;
    tchecker::integer_t phi_u_bound = tchecker::const_evaluate(phi_u->bound());
    assert(phi_u_bound >= 0);

    for (tchecker::typed_simple_clkconstr_expression_t const * phi_l : Gdf_l)
    {
      tchecker::clock_id_t y = tchecker::extract_lvalue_variable_ids(phi_l->clock()).begin();
      y = y + 1;

      if (x == y) continue;

      tchecker::integer_t phi_l_bound = tchecker::const_evaluate(phi_l->bound());
      assert(phi_l_bound >= 0);
      
      tchecker::dbm::comparator_t cmp2 = phi_l->binary_operator() == EXPR_OP_GT ? tchecker::dbm::LT : tchecker::dbm::LE;

      if ( (tchecker::dbm::sum(DBM1(0,x), tchecker::dbm::db(cmp1, phi_u_bound)) >= tchecker::dbm::LE_ZERO) &&
          (tchecker::dbm::sum(DBM2(y,x), tchecker::dbm::db(cmp2, -phi_l_bound)) < DBM1(0,x)) &&
          (DBM2(y,x) < DBM1(y,x)) ) 
            return false;
    }
  }

  // finally
  return true;
}

bool is_g_le_star(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim, 
                  std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
                  std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::is_consistent(dbm1, dim));
  assert(tchecker::dbm::is_consistent(dbm2, dim));
  assert(tchecker::dbm::is_positive(dbm1, dim));
  assert(tchecker::dbm::is_positive(dbm2, dim));
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  if (G.empty()) return true; //should return G_le_nd or something like that

  tchecker::typed_diagonal_clkconstr_expression_t const * phi = G.back();
  assert(phi != nullptr);
  G.pop_back(); // phi will be pushed back before exiting this function

  // creating negation of phi
  tchecker::integer_t bound = tchecker::const_evaluate(phi->bound());
  tchecker::binary_operator_t phi_op = phi->binary_operator();
  assert(phi_op != EXPR_OP_EQ && phi_op != EXPR_OP_NEQ);

  // if phi contains > or >= then it needs to be rewritten to contain < or <=
  bool to_be_rewritten = (phi_op == EXPR_OP_GT || phi_op == EXPR_OP_GE) ? 
                          true : false; 

  tchecker::clock_id_t clk1, clk2;
  if (!to_be_rewritten)
  {
    clk1 = tchecker::extract_lvalue_variable_ids(phi->first_clock()).begin();
    clk2 = tchecker::extract_lvalue_variable_ids(phi->second_clock()).begin();
  }
  else
  {
    // flip the clocks and negate the bound
    clk1 = tchecker::extract_lvalue_variable_ids(phi->second_clock()).begin();
    clk2 = tchecker::extract_lvalue_variable_ids(phi->first_clock()).begin();
    bound = -bound;
  }
  tchecker::dbm::comparator_t cmp = (phi_op == EXPR_OP_GE || phi_op == EXPR_OP_LE) ? tchecker::dbm::LT : tchecker::dbm::LE;
  tchecker::dbm::comparator_t cmp_phi = (phi_op == EXPR_OP_GE || phi_op == EXPR_OP_LE) ? tchecker::dbm::LE : tchecker::dbm::LT;

  tchecker::dbm::db_t dbm1_intersection_neg_phi[dim*dim];
  tchecker::dbm::universal(dbm1_intersection_neg_phi, dim);
  tchecker::dbm::intersection(dbm1_intersection_neg_phi, 
                              dbm1_intersection_neg_phi, dbm1, dim);

  if (tchecker::dbm::constrain(dbm1_intersection_neg_phi, dim, clk1, clk2, cmp, bound) == tchecker::dbm::NON_EMPTY)
  {
    if (!is_g_le_star(dbm1_intersection_neg_phi, dbm2, dim, G, Gdf))
    {
      G.push_back(phi);
      return false;
    }
  }

  // create dbm1 intersection phi and dbm2 intersection phi

  // create copies of dbm1, dbm2
  tchecker::dbm::db_t dbm1_intersection_phi[dim*dim],
                      dbm2_intersection_phi[dim*dim];
  tchecker::dbm::universal(dbm1_intersection_phi, dim);
  tchecker::dbm::universal(dbm2_intersection_phi, dim);
  tchecker::dbm::intersection(dbm1_intersection_phi, 
                              dbm1_intersection_phi, dbm1, dim);
  tchecker::dbm::intersection(dbm2_intersection_phi, 
                              dbm2_intersection_phi, dbm2, dim);

  // intersect with phi
  tchecker::dbm::constrain(dbm1_intersection_phi, dim, clk1, clk2, cmp_phi, bound);
  tchecker::dbm::constrain(dbm2_intersection_phi, dim, clk1, clk2, cmp_phi, bound);
  
  bool ans = is_g_le(dbm1_intersection_phi, dbm2_intersection_phi, dim, G, Gdf);
  G.push_back(phi);
  return ans;
}

bool is_g_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
            std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
            std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);

  if (tchecker::dbm::is_empty_0(dbm1, dim)) return true;
  if (tchecker::dbm::is_empty_0(dbm2, dim)) return false;

  assert(tchecker::dbm::is_consistent(dbm1, dim));
  assert(tchecker::dbm::is_consistent(dbm2, dim));
  assert(tchecker::dbm::is_positive(dbm1, dim));
  assert(tchecker::dbm::is_positive(dbm2, dim));
  assert(tchecker::dbm::is_tight(dbm1, dim));
  assert(tchecker::dbm::is_tight(dbm2, dim));

  //checking diagonal free constraints dbm1 <=_Gdf dbm2 //wierd stuff?
  if (!tchecker::dbm::is_g_le_nd(dbm1, dbm2, dim, Gdf)) return false; 
  return is_g_le_star(dbm1, dbm2, dim, G, Gdf);
}


bool is_eca_g_le_nd(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2,
                tchecker::clock_id_t dim,
                std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                const std::unordered_set<tchecker::integer_t> & normal_clock_ids)
{
  /*
    To check whether dbm1<=dbm2, we first check conditions one and two for prophecy clocks.
    Then we check conditions one and two for history clocks
    Finally we check the third condition for four cases i.e. when x and y are both history, 
    x is history y is prophecy
    y is history x is prophecy, and x and y are both prophecy
  */
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::eca_is_consistent(dbm1, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_consistent(dbm2, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm1, dim));
  assert(tchecker::dbm::eca_is_tight(dbm2, dim));

  // From Gdf construct Gdf_u and Gdf_l
  // Gdf_u (resp. Gdf_l) consists of upper (resp. lower) bound constraints
  // Gdf = Gdf_u U Gdf_l
  std::vector<tchecker::typed_simple_clkconstr_expression_t const *> Gdf_u{}, Gdf_l{};

  //collecting diagonal free upper and lower guards: popukate Gdf_u and Gdf_l
  for (tchecker::typed_simple_clkconstr_expression_t const * nondiag : Gdf)
  {
    if (
        (nondiag->binary_operator() == EXPR_OP_LE ||
        nondiag->binary_operator() == EXPR_OP_LT) && 
        (tchecker::extract_lvalue_variable_ids(nondiag->clock()).begin()!=0)
      )
      Gdf_u.push_back(nondiag);
    else if (
             (nondiag->binary_operator() == EXPR_OP_GE ||
             nondiag->binary_operator() == EXPR_OP_GT) &&
             (tchecker::extract_lvalue_variable_ids(nondiag->clock()).begin()!=0)
          )
      Gdf_l.push_back(nondiag);
    else
      throw std::invalid_argument("unexpected expression");
  }


  // condition 1
  for (tchecker::typed_simple_clkconstr_expression_t const * phi_u : Gdf_u)
  {
    tchecker::clock_id_t x = tchecker::extract_lvalue_variable_ids(phi_u->clock()).begin();
    x = x+1;//because of reference clock being 0

    if(DBM2(0,x) < DBM1(0,x)){
      
      if(prophecy_clock_ids.find(x)!=prophecy_clock_ids.end()) return false; //if x is prophecy clock return false

      tchecker::integer_t phi_bound = tchecker::const_evaluate(phi_u->bound());
      assert(phi_bound >= 0);
      
      tchecker::dbm::comparator_t cmp = phi_u->binary_operator() == EXPR_OP_LT ? 
                                        tchecker::dbm::LT : tchecker::dbm::LE;
      
      if (
           (tchecker::dbm::LE_ZERO <= tchecker::dbm::sum(DBM1(0,x), tchecker::dbm::db(cmp, phi_bound))) 
        ) {
          return false; //if x is either a history clock or a normal clock and this condition is true we can safely
          //return false
      }
      
      if (
            history_clock_ids.find(x)!=history_clock_ids.end() &&
            cmp==tchecker::dbm::LT && phi_bound==tchecker::dbm::INF_VALUE &&
            DBM2(0,x)==tchecker::dbm::db(tchecker::dbm::LE, tchecker::dbm::MINUS_INF_VALUE)
         ) return false;
    }

  }

  // condition 2
  for (tchecker::typed_simple_clkconstr_expression_t const * phi_l : Gdf_l)
  {
    tchecker::clock_id_t y = tchecker::extract_lvalue_variable_ids(phi_l->clock()).begin();
    y = y + 1;

    if(DBM2(y,0) < DBM1(y,0)){
      
      if(prophecy_clock_ids.find(y)!=prophecy_clock_ids.end()) return false;

      tchecker::integer_t phi_bound = tchecker::const_evaluate(phi_l->bound());
      assert(phi_bound >= 0);
      tchecker::dbm::comparator_t cmp = phi_l->binary_operator() == EXPR_OP_GT ? 
                                        tchecker::dbm::LT : tchecker::dbm::LE;

      if ( (tchecker::dbm::sum(DBM2(y,0), tchecker::dbm::db(cmp, -1*phi_bound)) < tchecker::dbm::LE_ZERO)
         ) return false;

      if (history_clock_ids.find(y)!=history_clock_ids.end() &&
           cmp==tchecker::dbm::LE && phi_bound==tchecker::dbm::INF_VALUE &&
           DBM1(y,0)==tchecker::dbm::db(tchecker::dbm::LE, tchecker::dbm::INF_VALUE)
         ) return false;
    }
  }
  
  //OPTIMIZATION:
  if (Gdf_u.size() == 0 || Gdf_l.size() == 0) 
    return true;

  //condition3
  for (tchecker::typed_simple_clkconstr_expression_t const * phi_u : Gdf_u)
  {
    tchecker::clock_id_t x = tchecker::extract_lvalue_variable_ids(phi_u->clock()).begin();
    x = x + 1;
    tchecker::dbm::comparator_t cmp1 = phi_u->binary_operator() == EXPR_OP_LT ? tchecker::dbm::LT : tchecker::dbm::LE;
    tchecker::integer_t phi_u_bound = tchecker::const_evaluate(phi_u->bound());
    assert(phi_u_bound >= 0);

    if(
        phi_u_bound==tchecker::dbm::INF_VALUE || 
        phi_u_bound==tchecker::dbm::MINUS_INF_VALUE
    ) continue;
    
    for (tchecker::typed_simple_clkconstr_expression_t const * phi_l : Gdf_l)
    {
      tchecker::clock_id_t y = tchecker::extract_lvalue_variable_ids(phi_l->clock()).begin();
      y = y + 1;
      
      if (x == y) continue;

      tchecker::dbm::comparator_t cmp2 = phi_l->binary_operator() == EXPR_OP_GT ? tchecker::dbm::LT : tchecker::dbm::LE;
      tchecker::integer_t phi_l_bound = tchecker::const_evaluate(phi_l->bound());
      assert(phi_l_bound >= 0);
      
      if(
        phi_l_bound==tchecker::dbm::INF_VALUE || 
        phi_l_bound==tchecker::dbm::MINUS_INF_VALUE
      ) continue;
      


      if (DBM2(y,x) < DBM1(y,x) && 
          DBM2(y,x)!=tchecker::dbm::LE_MINUS_INFINITY &&
          DBM2(y,x)!=tchecker::dbm::LT_INFINITY &&
          DBM2(y,x)!=tchecker::dbm::LE_INFINITY
      )
      if ( tchecker::dbm::LE_ZERO<= (tchecker::dbm::sum(DBM1(0,x), tchecker::dbm::db(cmp1, phi_u_bound))) &&
          (tchecker::dbm::sum(DBM2(y,x), tchecker::dbm::db(cmp2, -1*phi_l_bound)) < DBM1(0,x))) 
            return false;
    }
  }

  
  return true;
}

bool is_eca_g_le_star(tchecker::dbm::db_t const * dbm1, 
                      tchecker::dbm::db_t const * dbm2, 
                      tchecker::clock_id_t dim, 
                      std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
                      std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                      const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                      const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                      const std::unordered_set<tchecker::integer_t> & normal_clock_ids)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);
  assert(tchecker::dbm::eca_is_consistent(dbm1, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_consistent(dbm2, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));

  assert(tchecker::dbm::eca_is_tight(dbm1, dim));
  assert(tchecker::dbm::eca_is_tight(dbm2, dim));


  if (G.empty()){
    bool ans_g_le_nd = is_eca_g_le_nd(dbm1, dbm2, dim, Gdf, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
    return ans_g_le_nd;
  }

  tchecker::typed_diagonal_clkconstr_expression_t const * phi = G.back();
  assert(phi != nullptr);
  G.pop_back(); // phi will be pushed back before exiting this function

  // creating negation of phi
  tchecker::integer_t bound = tchecker::const_evaluate(phi->bound());
  tchecker::binary_operator_t phi_op = phi->binary_operator();

  assert(phi_op != EXPR_OP_EQ && phi_op != EXPR_OP_NEQ);
  assert(phi_op == EXPR_OP_LE || phi_op == EXPR_OP_LT);
  
  tchecker::clock_id_t clk1, clk2;
  
  clk1 = tchecker::extract_lvalue_variable_ids(phi->first_clock()).begin()+1;
  clk2 = tchecker::extract_lvalue_variable_ids(phi->second_clock()).begin()+1;
  
  tchecker::dbm::comparator_t cmp_nphi = (phi_op == EXPR_OP_LE) ? tchecker::dbm::LT : tchecker::dbm::LE;
  tchecker::dbm::comparator_t cmp_phi = (phi_op==EXPR_OP_LT) ? tchecker::dbm::LT : tchecker::dbm::LE;

  
  
  tchecker::dbm::db_t dbm1_intersection_neg_phi[dim*dim];
  tchecker::dbm::eca_universal(dbm1_intersection_neg_phi, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  
  
  tchecker::dbm::eca_intersection(dbm1_intersection_neg_phi, 
                              dbm1_intersection_neg_phi, dbm1, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  
  
  //negating constraint and conjoining it with DBM1  
  //if clock1 and clock2 are both history clocks
  //if exactly one of clocks 1 and 2 is prophecy and exactly one is history
  bool is_prop_clk1 = (prophecy_clock_ids.end()!=prophecy_clock_ids.find(clk1));
  bool is_prop_clk2 = (prophecy_clock_ids.end()!=prophecy_clock_ids.find(clk2));
  
  bool is_hist_clk1 = (history_clock_ids.end()!=history_clock_ids.find(clk1));
  bool is_hist_clk2 = (history_clock_ids.end()!=history_clock_ids.find(clk2));

  bool is_norm_clk1 = (normal_clock_ids.end()!=normal_clock_ids.find(clk1));
  bool is_norm_clk2 = (normal_clock_ids.end()!=normal_clock_ids.find(clk2));
  
  if(is_prop_clk1 && (is_hist_clk2 || is_norm_clk2)){
    //constraint is prophecy-history <| c
    if (tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi, dim, clk2, clk1, cmp_nphi, -1*bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY)
    {
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi, 
                            dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }
  } 
  else if ((is_hist_clk1 || is_norm_clk1) && is_prop_clk2){
    //constraint is history-prophecy <| c
    if (tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi, dim, clk2, clk1, cmp_nphi, -1*bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY)
    {
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi, dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }  
  }
  else if ((is_hist_clk1 || is_norm_clk1) && (is_hist_clk2 || is_norm_clk2)
   && (phi_op!=tchecker::EXPR_OP_LE || bound!=tchecker::dbm::INF_VALUE)){
    //constraint is history-history <| c
    //in zone dbm1, it is either the case that both clk1 and clk2 are equal to infinity or they are finite use that
    tchecker::dbm::db_t dbm1_intersection_neg_phi2[dim*dim];
    tchecker::dbm::eca_universal(dbm1_intersection_neg_phi2, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);

    tchecker::dbm::eca_intersection(dbm1_intersection_neg_phi2, 
                            dbm1_intersection_neg_phi2, dbm1, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids);
    if(
      tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi2, dim, 0, clk1, tchecker::dbm::LE, tchecker::dbm::MINUS_INF_VALUE, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY
      &&
      tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi2, dim, 0, clk2, tchecker::dbm::LE, tchecker::dbm::MINUS_INF_VALUE, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY
    ){
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi2, dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }

    if (tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi, dim, clk2, clk1, cmp_nphi, -1*bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY)
    {
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi, dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }   
    
  }
  else if(is_prop_clk1 && is_prop_clk2 && (phi_op!=tchecker::EXPR_OP_LE || bound!=tchecker::dbm::INF_VALUE)){
    
    tchecker::dbm::db_t dbm1_intersection_neg_phi2[dim*dim];
    tchecker::dbm::eca_universal(dbm1_intersection_neg_phi2, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);

    tchecker::dbm::eca_intersection(dbm1_intersection_neg_phi2, 
                            dbm1_intersection_neg_phi2, dbm1, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);


    if(
      tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi2, dim, clk1, 0, tchecker::dbm::LE, tchecker::dbm::MINUS_INF_VALUE, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY
      &&
      tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi2, dim, clk2, 0, tchecker::dbm::LE, tchecker::dbm::MINUS_INF_VALUE, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY
    ){
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi2, dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }

    if (tchecker::dbm::eca_constrain(dbm1_intersection_neg_phi, dim, clk2, clk1, cmp_nphi, -1*bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids) == tchecker::dbm::NON_EMPTY)
    {
      if (!is_eca_g_le_star(dbm1_intersection_neg_phi, dbm2, dim, G, Gdf,
                            history_clock_ids, 
                            prophecy_clock_ids,
                            normal_clock_ids))
      {
        G.push_back(phi);
        return false;
      }
    }   

  }
  
  
  

  // create dbm1 intersection phi and dbm2 intersection phi

  // create copies of dbm1, dbm2
  tchecker::dbm::db_t dbm1_intersection_phi[dim*dim],
                      dbm2_intersection_phi[dim*dim];
  tchecker::dbm::eca_universal(dbm1_intersection_phi, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  tchecker::dbm::eca_universal(dbm2_intersection_phi, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  
  tchecker::dbm::eca_intersection(dbm1_intersection_phi, 
                              dbm1_intersection_phi, dbm1, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  tchecker::dbm::eca_intersection(dbm2_intersection_phi, 
                              dbm2_intersection_phi, dbm2, dim,history_clock_ids, prophecy_clock_ids, normal_clock_ids);

  // intersect with phi
  auto tmp1 = tchecker::dbm::eca_constrain(dbm1_intersection_phi, dim, clk1, clk2, cmp_phi, bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  auto tmp2 = tchecker::dbm::eca_constrain(dbm2_intersection_phi, dim, clk1, clk2, cmp_phi, bound, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  if(tmp1==tchecker::dbm::NON_EMPTY && tmp2==tchecker::dbm::EMPTY){
    G.push_back(phi);
    return false;
  }
  else if(tmp1==tchecker::dbm::EMPTY){
    G.push_back(phi);
    return true;
  }
  bool ans = is_eca_g_le_star(dbm1_intersection_phi, dbm2_intersection_phi, dim, G, Gdf, history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  G.push_back(phi);
  return ans;
}

bool is_eca_g_le(tchecker::dbm::db_t const * dbm1, tchecker::dbm::db_t const * dbm2, tchecker::clock_id_t dim,
            std::vector<tchecker::typed_diagonal_clkconstr_expression_t const *> & G,
            std::vector<tchecker::typed_simple_clkconstr_expression_t const *> const & Gdf, 
                const std::unordered_set<tchecker::integer_t> & history_clock_ids,
                const std::unordered_set<tchecker::integer_t> & prophecy_clock_ids,
                const std::unordered_set<tchecker::integer_t> & normal_clock_ids)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim >= 1);



  if (tchecker::dbm::is_empty_0(dbm1, dim)) return true;
  if (tchecker::dbm::is_empty_0(dbm2, dim)) return false;

  assert(tchecker::dbm::eca_is_consistent(dbm1, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_consistent(dbm2, dim, history_clock_ids, prophecy_clock_ids, normal_clock_ids));
  assert(tchecker::dbm::eca_is_tight(dbm1, dim));
  assert(tchecker::dbm::eca_is_tight(dbm2, dim));

  bool ret_val = is_eca_g_le_star(dbm1, dbm2, dim, G, Gdf,history_clock_ids, prophecy_clock_ids, normal_clock_ids);
  
  return ret_val;
}


std::size_t hash(tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  std::size_t seed = 0;
  for (tchecker::clock_id_t k = 0; k < dim * dim; ++k)
    boost::hash_combine(seed, tchecker::dbm::hash(dbm[k]));
  return seed;
}

std::ostream & output_matrix(std::ostream & os, tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim)
{
  assert(dbm != nullptr);
  assert(dim >= 1);

  for (tchecker::clock_id_t i = 0; i < dim; ++i) {
    for (tchecker::clock_id_t j = 0; j < dim; ++j) {
      tchecker::dbm::output(os, DBM(i, j));
      os << '\t';
    }
    os << std::endl;
  }

  return os;
}

std::ostream & output(std::ostream & os, tchecker::dbm::db_t const * dbm, tchecker::clock_id_t dim,
                      std::function<std::string(tchecker::clock_id_t)> clock_name)
{
  os << "(";

  // output: x # c (first row/column)
  for (tchecker::clock_id_t j = 1; j < dim; ++j) {
    tchecker::dbm::db_t c0j = DBM(0, j), cj0 = DBM(j, 0);
    if (j > 1)
      os << " & ";
    // xj = k
    
    //changed this if 
    // if (tchecker::dbm::sum(c0j, cj0) == tchecker::dbm::LE_ZERO)
    //   os << clock_name(j) << "=" << tchecker::dbm::value(cj0);
    tchecker::integer_t v0j = tchecker::dbm::value(c0j);
    tchecker::integer_t vj0 = tchecker::dbm::value(cj0);
    if(
        (vj0==-1*v0j  || 
          (v0j==tchecker::dbm::INF_VALUE && 
          vj0==tchecker::dbm::MINUS_INF_VALUE) ||
          (v0j==tchecker::dbm::MINUS_INF_VALUE && 
          vj0==tchecker::dbm::INF_VALUE)
        )&& 
        tchecker::dbm::comparator(c0j)==tchecker::dbm::LE &&
        tchecker::dbm::comparator(cj0)==tchecker::dbm::LE
      )
    {
      os << clock_name(j) << "=";
      if (vj0!=INF_VALUE && vj0!=MINUS_INF_VALUE)
        os << tchecker::dbm::value(cj0);
      else if(v0j==INF_VALUE)
        os << "-inf";
      else if(v0j==MINUS_INF_VALUE)
        os << "inf";
    }
    // k1 <= xj <= k2
    else {
      //changed some cases here also
      if (v0j==tchecker::dbm::INF_VALUE)
        os << "-inf";
      else if (v0j==tchecker::dbm::MINUS_INF_VALUE)
        os << "inf";
      else
        os << -tchecker::dbm::value(c0j);
      
      os << tchecker::dbm::comparator_str(c0j) << clock_name(j);
      os << tchecker::dbm::comparator_str(cj0);
      
      if (tchecker::dbm::value(cj0)==tchecker::dbm::INF_VALUE)
        os << "inf";
      else if (tchecker::dbm::value(cj0)==tchecker::dbm::MINUS_INF_VALUE)
        os << "-inf";
      else
        os << tchecker::dbm::value(cj0);
    }
  }

  // output: x-y # c (other rows/columns)
  for (tchecker::clock_id_t i = 1; i < dim; ++i) {
    for (tchecker::clock_id_t j = i + 1; j < dim; ++j) {
      tchecker::dbm::db_t cij = DBM(i, j), cji = DBM(j, i);
      // xi == xj + k
      tchecker::integer_t vij = tchecker::dbm::value(cij);
      tchecker::integer_t vji = tchecker::dbm::value(cji);
      if((vij==-1*vji || 
            (vij==tchecker::dbm::INF_VALUE && 
            vji==tchecker::dbm::MINUS_INF_VALUE) ||
            (vij==tchecker::dbm::MINUS_INF_VALUE && 
            vji==tchecker::dbm::INF_VALUE)
          ) && 
          tchecker::dbm::comparator(cij)==tchecker::dbm::LE &&
          tchecker::dbm::comparator(cji)==tchecker::dbm::LE){
        
        os << " & ";
        os << clock_name(i) << "=" << clock_name(j);
        
        if (vij == tchecker::dbm::INF_VALUE)
          os << "+inf";
        else if (vij == tchecker::dbm::MINUS_INF_VALUE)
          os << "-inf";
        else if (vij > 0)
          os << "+" << vij;
        else if (vij < 0)
          os << vij;
      }
      //the if below is useless
      // if (tchecker::dbm::sum(cij, cji) == tchecker::dbm::LE_ZERO) {
        
      //   os << " & ";
      //   os << clock_name(i) << "=" << clock_name(j);
      //   tchecker::integer_t vij = tchecker::dbm::value(cij);
      //   if (vij > 0)
      //     os << "+" << tchecker::dbm::value(cij);
      //   else if (vij < 0)
      //     os << "-" << -tchecker::dbm::value(cij);
      // }
      
      
      // k1 <= xi - xj <= k2
      else{
        os << " & ";

        if (vji==tchecker::dbm::MINUS_INF_VALUE)
          os << "inf";
        else if (vji==tchecker::dbm::INF_VALUE)
          os << "-inf";
        else
          os << -vji;
        
        os << tchecker::dbm::comparator_str(cji);
        os << clock_name(i) << "-" << clock_name(j);
        os << tchecker::dbm::comparator_str(cij);

        if (vij==tchecker::dbm::MINUS_INF_VALUE)
          os << "-inf";
        else if (vij==tchecker::dbm::INF_VALUE)
          os << "inf";
        else
          os << vij;
        

      }
      //the else below is useless
      // else if ((cij != tchecker::dbm::LT_INFINITY) || (cji != tchecker::dbm::LT_INFINITY)) {
      //   os << " & ";
        
      //   if (cji != tchecker::dbm::LT_INFINITY)
      //     os << -tchecker::dbm::value(cji) << tchecker::dbm::comparator_str(cji);

      //   os << clock_name(i) << "-" << clock_name(j);

      //   if (cij != tchecker::dbm::LT_INFINITY)
      //     os << tchecker::dbm::comparator_str(cij) << tchecker::dbm::value(cij);
      // }
    }
  }

  os << ")";

  return os;
}



int lexical_cmp(tchecker::dbm::db_t const * dbm1, tchecker::clock_id_t dim1, tchecker::dbm::db_t const * dbm2,
                tchecker::clock_id_t dim2)
{
  assert(dbm1 != nullptr);
  assert(dbm2 != nullptr);
  assert(dim1 >= 1);
  assert(dim2 >= 1);
  return tchecker::lexical_cmp(dbm1, dbm1 + dim1 * dim1, dbm2, dbm2 + dim2 * dim2, tchecker::dbm::db_cmp);
}

} // end of namespace dbm

} // end of namespace tchecker
