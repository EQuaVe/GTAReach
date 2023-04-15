/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#ifndef TCHECKER_EXPRESSION_HH
#define TCHECKER_EXPRESSION_HH

#include <iostream>
#include <string>

#include <boost/core/noncopyable.hpp>

#include "tchecker/basictypes.hh"

/*!
 \file expression.hh
 \brief Syntax tree for expressions
 */

namespace tchecker {

class expression_visitor_t; // forward declaration

/*!
 \class expression_t
 \brief Base class for expressions
 */
class expression_t : private boost::noncopyable {
public:
  /*!
   \brief Destructor
   */
  virtual ~expression_t() = default;

  /*!
   \brief Assignment operator (DELETED)
   */
  tchecker::expression_t & operator=(tchecker::expression_t const &) = delete;

  /*!
   \brief Move assignment operator (DELETED)
   */
  tchecker::expression_t & operator=(tchecker::expression_t &&) = delete;

  /*!
   \brief Clone
   \return A clone of this
   */
  tchecker::expression_t * clone() const;

  /*!
   \brief Visit
   \param v : visitor
   */
  void visit(tchecker::expression_visitor_t & v) const;

  /*!
   \brief Accessor
   \return this expression as a string
   */
  std::string to_string() const;

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   */
  virtual std::ostream & do_output(std::ostream & os) const = 0;

  /*!
   \brief Clone
   */
  virtual tchecker::expression_t * do_clone() const = 0;

  /*!
   \brief Visit
   \param v : visitor
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const = 0;

  friend std::ostream & operator<<(std::ostream & os, tchecker::expression_t const & expr);
};

/*!
 \brief Output operator for expressions
 \param os : output stream
 \param expr : expression
 \post expr has been output to os
 \return os after expr has been output
 */
std::ostream & operator<<(std::ostream & os, tchecker::expression_t const & expr);

/*!
 \class int_expression_t
 \brief Integer constant
 */
class int_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Constructor
   \param value : integer constant
   */
  explicit int_expression_t(tchecker::integer_t value);

  /*!
   \brief Destructor
   */
  virtual ~int_expression_t() = default;

  /*!
   \brief Accessor
   \return Value
   */
  inline tchecker::integer_t value() const { return _value; }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  tchecker::integer_t _value; /*!< Value */
};

/*!
 \class lvalue_expression_t
 \brief Expression that can serve as left values
 */
class lvalue_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Destructor
   */
  virtual ~lvalue_expression_t() = default;
};

/*!
 \class var_expression_t
 \brief Variable
 */
class var_expression_t : public virtual tchecker::lvalue_expression_t {
public:
  /*!
   \brief Constructor
   \param name : variable name
   \pre name is not empty
   \throw std::invalid_argument : if name is empty
   */
  explicit var_expression_t(std::string const & name);

  /*!
   \brief Destructor
   */
  virtual ~var_expression_t() = default;

  /*!
   \brief Accessor
   \return Name
   */
  inline std::string const & name() const { return _name; }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  std::string const _name; /*!< Variable name */
};

/*!
 \class array_expression_t
 \brief Variable
 */
class array_expression_t : public virtual tchecker::lvalue_expression_t {
public:
  /*!
   \brief Constructor
   \param variable : array variable
   \param offset : offset
   \pre offset != nullptr and variable != nullptr
   \throw std::invalid_argument : if offset == nullptr or name == nullptr
   \note this takes ownership on offset and variable
   */
  array_expression_t(tchecker::var_expression_t const * variable, tchecker::expression_t const * offset);

  /*!
   \brief Destructor
   */
  virtual ~array_expression_t();

  /*!
   \brief Accessor
   \return Variable
   */
  inline tchecker::var_expression_t const & variable() const { return (*_variable); }

  /*!
   \brief Accessor
   \return Offset
   */
  inline tchecker::expression_t const & offset() const { return (*_offset); }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  tchecker::var_expression_t const * _variable; /*!< Array variable */
  tchecker::expression_t const * _offset;       /*!< Offset */
};

/*!
 \class par_expression_t
 \brief Expression inside parenthesis
 */
class par_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Constructor
   \param expr : operand
   \pre expr is not nullptr
   \throw std::invalid_argument : if expr is nullptr
   \note this claims ownership on expr
   */
  par_expression_t(tchecker::expression_t const * expr);

  /*!
   \brief Destructor
   */
  virtual ~par_expression_t();

  /*!
   \brief Accessor
   \return sub expression
   */
  inline tchecker::expression_t const & expr() const { return *_expr; }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  tchecker::expression_t const * _expr; /*!< Expression */
};

/*!
 \brief Type of unary operators
 */
enum unary_operator_t {
  EXPR_OP_NEG,  // unary minus
  EXPR_OP_LNOT, // logical not
};

/*!
 \brief Output operator for unary operators
 \param os : output stream
 \param op : unary operator
 \post op has been output to os
 \return os after op has been output
 */
std::ostream & operator<<(std::ostream & os, enum unary_operator_t op);

/*!
 \class unary_expression_t
 \brief Application of an unary operator to an expression
 */
class unary_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Constructor
   \param op : operator
   \param expr : operand
   \pre expr is not nullptr
   \throw std::invalid_argument : if expr is nullptr
   \note this takes ownership on expr
   */
  unary_expression_t(enum unary_operator_t op, tchecker::expression_t const * expr);

  /*!
   \brief Destructor
   \post the operand expression has been deleted
   */
  virtual ~unary_expression_t();

  /*!
   \brief Accessor
   \return Operand
   */
  inline enum unary_operator_t unary_operator() const { return _operator; }

  /*!
   \brief Accessor
   \return Operand
   */
  inline tchecker::expression_t const & operand() const { return (*_expr); }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  enum unary_operator_t _operator;      /*!< Operator */
  tchecker::expression_t const * _expr; /*!< Operand */
};

/*!
 \brief Type of binary operators
 */
enum binary_operator_t {
  EXPR_OP_LAND,
  EXPR_OP_LT,
  EXPR_OP_LE,
  EXPR_OP_EQ,
  EXPR_OP_NEQ,
  EXPR_OP_GE,
  EXPR_OP_GT,
  EXPR_OP_MINUS,
  EXPR_OP_PLUS,
  EXPR_OP_TIMES,
  EXPR_OP_DIV,
  EXPR_OP_MOD,
};

/*!
 \brief Output operator for binary operators
 \param os : output stream
 \param op : binary operator
 \post op has been output to os
 \return os after op has been output
 */
std::ostream & operator<<(std::ostream & os, enum binary_operator_t op);

/*!
 \brief Predicate check
 \param op : operator
 \return true if op is a predicate (LT, LE, EQ, NEQ, GE, GT), false otherwise
 */
bool predicate(enum tchecker::binary_operator_t op);

/*!
 \class binary_expression_t
 \brief Application of a binary operator to expressions
 */
class binary_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Constructor
   \param op : operaror
   \param left : left operand
   \param right : right operand
   \pre left and right are not nullptr
   \throw std::invalid_argument : if left or right is nullptr
   \note this takes ownership on left and right
   */
  binary_expression_t(enum binary_operator_t op, tchecker::expression_t const * left, tchecker::expression_t const * right);

  /*!
   \brief Destructor
   \post the operand expressions have been deleted
   */
  virtual ~binary_expression_t();

  /*!
   \brief Accessor
   \return Operator
   */
  enum binary_operator_t binary_operator() const { return _op; }

  /*!
   \brief Accessor
   \return Left operand
   */
  inline tchecker::expression_t const & left_operand() const { return (*_left); }

  /*!
   \brief Accessor
   \return Right operand
   */
  inline tchecker::expression_t const & right_operand() const { return (*_right); }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  enum binary_operator_t _op;            /*!< Operator */
  tchecker::expression_t const * _left;  /*!< Left operand */
  tchecker::expression_t const * _right; /*!< Right operand */
};

/*!
 \class ite_expression_t
 \brief Application of if-then-else operator to expressions
 */
class ite_expression_t : public virtual tchecker::expression_t {
public:
  /*!
   \brief Constructor
   \param condition : the decision expression
   \param then_value : the value of the expression if the condition is true
   \param else_value : the value of the expression if the condition is false
   \pre condition, then_value and else_value are not nullptr
   \throw std::invalid_argument : if an argument is nullptr
   \note this takes ownership on its arguments
   */
  ite_expression_t(tchecker::expression_t const * condition, tchecker::expression_t const * then_value,
                   tchecker::expression_t const * else_value);

  /*!
   \brief Destructor
   \post the operand expressions have been deleted
   */
  virtual ~ite_expression_t();

  /*!
   \brief Accessor
   \return the decision expression
   */
  inline tchecker::expression_t const & condition() const { return (*_condition); }

  /*!
   \brief Accessor
   \return the Then value
   */
  inline tchecker::expression_t const & then_value() const { return (*_then_value); }

  /*!
   \brief Accessor
   \return the Else value
   */
  inline tchecker::expression_t const & else_value() const { return (*_else_value); }

protected:
  /*!
   \brief Output the expression
   \param os : output stream
   \post this has been output to os
   \return os after this has been output
   */
  virtual std::ostream & do_output(std::ostream & os) const;

  /*!
   \brief Clone
   \return A clone of this
   */
  virtual tchecker::expression_t * do_clone() const;

  /*!
   \brief Visit
   \param v : visitor
   \post v.visit(*this) has been called
   */
  virtual void do_visit(tchecker::expression_visitor_t & v) const;

  tchecker::expression_t const * _condition;  /*!< guard of the ite */
  tchecker::expression_t const * _then_value; /*!< 'then' part of the expression */
  tchecker::expression_t const * _else_value; /*!< 'else' part of the expression */
};

/*!
 \class expression_visitor_t
 \brief Visitor for expressions
 */
class expression_visitor_t {
public:
  /*!
   \brief Constructor
   */
  expression_visitor_t() = default;

  /*!
   \brief Copy constructor
   */
  expression_visitor_t(tchecker::expression_visitor_t const &) = default;

  /*!
   \brief Move constructor
   */
  expression_visitor_t(tchecker::expression_visitor_t &&) = default;

  /*!
   \brief Destructor
   */
  virtual ~expression_visitor_t() = default;

  /*!
   \brief Assignment operator
   */
  tchecker::expression_visitor_t & operator=(tchecker::expression_visitor_t const &) = default;

  /*!
   \brief Move assignment operator
   */
  tchecker::expression_visitor_t & operator=(tchecker::expression_visitor_t &&) = default;

  /*!
   \brief Visitors
   */
  virtual void visit(tchecker::int_expression_t const & expr) = 0;
  virtual void visit(tchecker::var_expression_t const & expr) = 0;
  virtual void visit(tchecker::array_expression_t const & expr) = 0;
  virtual void visit(tchecker::par_expression_t const & expr) = 0;
  virtual void visit(tchecker::unary_expression_t const & expr) = 0;
  virtual void visit(tchecker::binary_expression_t const & expr) = 0;
  virtual void visit(tchecker::ite_expression_t const & expr) = 0;
};

} // end namespace tchecker

#endif // TCHECKER_EXPRESSION_HH
