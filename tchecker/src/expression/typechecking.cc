/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <tuple>

#include "tchecker/expression/type_inference.hh"
#include "tchecker/expression/typechecking.hh"

namespace tchecker {

namespace details {

/*!
 \class expression_typechecker_t
 \brief Expression typechecking visitor
 */
class expression_typechecker_t : public tchecker::expression_visitor_t {
public:
  /*!
   \brief Constructor
   \param intvars : integer variables
   \param clocks : clock variables
   \param log : logging facility
   */
  expression_typechecker_t(tchecker::integer_variables_t const & localvars, tchecker::integer_variables_t const & intvars,
                           tchecker::clock_variables_t const & clocks, std::function<void(std::string const &)> error)
      : _typed_expr(nullptr), _localvars(localvars), _intvars(intvars), _clocks(clocks), _error(error)
  {
  }

  /*!
   \brief Copy constructor (DELETED)
   */
  expression_typechecker_t(tchecker::details::expression_typechecker_t const &) = delete;

  /*!
   \brief Move constructor (DELETED)
   */
  expression_typechecker_t(tchecker::details::expression_typechecker_t &&) = delete;

  /*!
   \brief Destructor
   */
  virtual ~expression_typechecker_t() { delete _typed_expr; }

  /*!
   \brief Assignment operator (DELETED)
   */
  tchecker::details::expression_typechecker_t & operator=(tchecker::details::expression_typechecker_t const &) = delete;

  /*!
   \brief Move assignment operator (DELETED)
   */
  tchecker::details::expression_typechecker_t & operator=(tchecker::details::expression_typechecker_t &&) = delete;

  /*!
   \brief Accessor
   \return typed expression computed by this visitor
   \note the expression is released by the call, and should be handled by the
   caller
   */
  tchecker::typed_expression_t * release()
  {
    auto p = _typed_expr;
    _typed_expr = nullptr;
    return p;
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::int_expression_t const & expr)
  {
    _typed_expr = new tchecker::typed_int_expression_t(tchecker::EXPR_TYPE_INTTERM, expr.value());
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::var_expression_t const & expr)
  {
    // variable type, id and size
    auto type_id_size = typecheck_variable(expr.name());
    enum tchecker::expression_type_t const type = std::get<0>(type_id_size);
    tchecker::variable_id_t const id = std::get<1>(type_id_size);
    tchecker::variable_size_t const size = std::get<2>(type_id_size);

    // bounded integer variable
    if ((type == tchecker::EXPR_TYPE_LOCALINTVAR) || (type == tchecker::EXPR_TYPE_LOCALINTARRAY)) {
      auto const & infos = _localvars.info(id);
      _typed_expr = new tchecker::typed_bounded_var_expression_t(type, expr.name(), id, size, infos.min(), infos.max());
    }
    else if ((type == tchecker::EXPR_TYPE_INTVAR) || (type == tchecker::EXPR_TYPE_INTARRAY)) {
      auto const & infos = _intvars.info(id);
      _typed_expr = new tchecker::typed_bounded_var_expression_t(type, expr.name(), id, size, infos.min(), infos.max());
    }
    // clock variable
    else if ((type == tchecker::EXPR_TYPE_CLKVAR) || (type == tchecker::EXPR_TYPE_CLKARRAY))
      _typed_expr = new tchecker::typed_var_expression_t(type, expr.name(), id, size);
    // otherwise (BAD)
    else
      _typed_expr = new tchecker::typed_var_expression_t(tchecker::EXPR_TYPE_BAD, expr.name(), id, size);

    if (type == tchecker::EXPR_TYPE_BAD)
      _error("in expression " + expr.to_string() + ", undeclared variable");
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   \note array expression on variables of size 1 are well typed
   \note out-of-bounds access are not checked
   */
  virtual void visit(tchecker::array_expression_t const & expr)
  {
    // Typecheck variable
    expr.variable().visit(*this);
    auto const * const typed_variable = dynamic_cast<tchecker::typed_var_expression_t const *>(this->release());
    auto const variable_type = typed_variable->type();

    // Typecheck offset
    expr.offset().visit(*this);
    auto const * const typed_offset = this->release();
    auto const offset_type = typed_offset->type();

    // Typed expression
    enum tchecker::expression_type_t expr_type;

    if (integer_dereference(variable_type) && integer_valued(offset_type)) {
      if ((variable_type == tchecker::EXPR_TYPE_LOCALINTVAR) || (variable_type == tchecker::EXPR_TYPE_LOCALINTARRAY))
        expr_type = tchecker::EXPR_TYPE_LOCALINTLVALUE;
      else
        expr_type = tchecker::EXPR_TYPE_INTLVALUE;
    }
    else if (clock_dereference(variable_type) && integer_valued(offset_type))
      expr_type = tchecker::EXPR_TYPE_CLKLVALUE;
    else
      expr_type = tchecker::EXPR_TYPE_BAD;

    _typed_expr = new tchecker::typed_array_expression_t(expr_type, typed_variable, typed_offset);

    // Report bad type
    if (expr_type != tchecker::EXPR_TYPE_BAD)
      return;

    if (offset_type != tchecker::EXPR_TYPE_BAD) {
      if (!integer_valued(offset_type))
        _error("in expression " + expr.to_string() + ", array subscript " + expr.offset().to_string() +
               " does not have an integral value");
      else
        _error("in expression " + expr.to_string() + ", invalid array variable " + expr.variable().to_string());
    }
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::par_expression_t const & expr)
  {
    // Sub expression
    expr.expr().visit(*this);
    tchecker::typed_expression_t * typed_sub_expr = this->release();

    // Typed expression
    enum tchecker::expression_type_t expr_type = type_par(typed_sub_expr->type());

    _typed_expr = new tchecker::typed_par_expression_t(expr_type, typed_sub_expr);

    // Report bad type
    if (expr_type != tchecker::EXPR_TYPE_BAD)
      return;

    if (typed_sub_expr->type() != tchecker::EXPR_TYPE_BAD)
      _error("in expression " + expr.to_string() + ", invalid parentheses around " + expr.expr().to_string());
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::binary_expression_t const & expr)
  {
    // Operands
    expr.left_operand().visit(*this);
    tchecker::typed_expression_t * typed_left_operand = this->release();

    expr.right_operand().visit(*this);
    tchecker::typed_expression_t * typed_right_operand = this->release();

    // Typed expression
    enum tchecker::expression_type_t expr_type =
        type_binary(expr.binary_operator(), typed_left_operand->type(), typed_right_operand->type());

    switch (expr_type) {
    case tchecker::EXPR_TYPE_CLKCONSTR_SIMPLE:
      _typed_expr = new tchecker::typed_simple_clkconstr_expression_t(expr_type, expr.binary_operator(), typed_left_operand,
                                                                      typed_right_operand);
      break;
    case tchecker::EXPR_TYPE_CLKCONSTR_DIAGONAL:
      _typed_expr = new tchecker::typed_diagonal_clkconstr_expression_t(expr_type, expr.binary_operator(), typed_left_operand,
                                                                        typed_right_operand);
      break;
    default:
      _typed_expr =
          new tchecker::typed_binary_expression_t(expr_type, expr.binary_operator(), typed_left_operand, typed_right_operand);
      break;
    }

    // Report bad type
    if (expr_type != tchecker::EXPR_TYPE_BAD)
      return;

    if ((typed_left_operand->type() != tchecker::EXPR_TYPE_BAD) && (typed_right_operand->type() != tchecker::EXPR_TYPE_BAD))
      _error("in expression " + expr.to_string() + ", invalid composition of expressions " + expr.left_operand().to_string() +
             " and " + expr.right_operand().to_string());
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::unary_expression_t const & expr)
  {
    // Operand
    expr.operand().visit(*this);
    tchecker::typed_expression_t * typed_operand = this->release();

    // Typed expression
    enum tchecker::expression_type_t expr_type = type_unary(expr.unary_operator(), typed_operand->type());

    _typed_expr = new tchecker::typed_unary_expression_t(expr_type, expr.unary_operator(), typed_operand);

    // Report bad type
    if (expr_type != tchecker::EXPR_TYPE_BAD)
      return;

    if (typed_operand->type() != tchecker::EXPR_TYPE_BAD)
      _error("in expression " + expr.to_string() + ", invalid operand " + expr.operand().to_string());
  }

  /*!
   \brief Visitor
   \param expr : expression
   \post _typed_expr points to a typed clone of expr
   */
  virtual void visit(tchecker::ite_expression_t const & expr)
  {
    // Operands
    expr.condition().visit(*this);
    tchecker::typed_expression_t * typed_condition = this->release();

    expr.then_value().visit(*this);
    tchecker::typed_expression_t * typed_then_value = this->release();

    expr.else_value().visit(*this);
    tchecker::typed_expression_t * typed_else_value = this->release();

    // Typed expression
    enum tchecker::expression_type_t expr_type =
        type_ite(typed_condition->type(), typed_then_value->type(), typed_else_value->type());
    if (expr_type == tchecker::EXPR_TYPE_INTTERM)
      _typed_expr = new tchecker::typed_ite_expression_t(expr_type, typed_condition, typed_then_value, typed_else_value);

    // Report bad type
    if (expr_type != tchecker::EXPR_TYPE_BAD)
      return;

    if ((typed_then_value->type() != tchecker::EXPR_TYPE_BAD) && (typed_else_value->type() != tchecker::EXPR_TYPE_BAD))
      _error("in expression " + expr.to_string() + ", invalid composition of expressions " + expr.then_value().to_string() +
             " and " + expr.else_value().to_string());
  }

protected:
  /*!
   \brief Accessor
   \param name : variable name
   \return
   tchecker::EXPR_TYPE_LOCALINTARRAY if name is an array of integer
   variables,
   tchecker::EXPR_TYPE_LOCALINTVAR if name is an integer variable of size 1,
   tchecker::EXPR_TYPE_INTARRAY if name is an array of integer
   variables,
   tchecker::EXPR_TYPE_INTVAR if name is an integer variable of size 1,
   tchecker::EXPR_TYPE_CLKARRAY if name is an array of clock variables,
   tchecker::EXPR_TYPE_CLKVAR if name is a clock variable of size 1
   tchecker::EXPR_TYPE_BAD otherwise (name is not a declared variable)
   \pre name is a declared integer or clock variable
   */
  std::tuple<enum tchecker::expression_type_t, tchecker::variable_id_t, tchecker::variable_size_t>
  typecheck_variable(std::string const & name)
  {
    // Local integer variable ?
    try {
      auto id = _localvars.id(name);
      auto size = _localvars.info(id).size();
      if (size > 1)
        return std::make_tuple(tchecker::EXPR_TYPE_LOCALINTARRAY, id, size);
      else
        return std::make_tuple(tchecker::EXPR_TYPE_LOCALINTVAR, id, size);
    }
    catch (...) {
    }

    // Integer variable ?
    try {
      auto id = _intvars.id(name);
      auto size = _intvars.info(id).size();
      if (size > 1)
        return std::make_tuple(tchecker::EXPR_TYPE_INTARRAY, id, size);
      else
        return std::make_tuple(tchecker::EXPR_TYPE_INTVAR, id, size);
    }
    catch (...) {
    }

    // Clock variable ?
    try {
      auto id = _clocks.id(name);
      auto size = _clocks.info(id).size();
      if (size > 1)
        return std::make_tuple(tchecker::EXPR_TYPE_CLKARRAY, id, size);
      else
        return std::make_tuple(tchecker::EXPR_TYPE_CLKVAR, id, size);
    }
    catch (...) {
    }

    // Not a variable name
    return std::make_tuple(tchecker::EXPR_TYPE_BAD, std::numeric_limits<tchecker::variable_id_t>::max(), 1);
  }

  tchecker::typed_expression_t * _typed_expr;       /*!< Typed expression */
  tchecker::integer_variables_t const & _localvars; /*!< Local variables */
  tchecker::integer_variables_t const & _intvars;   /*!< Integer variables */
  tchecker::clock_variables_t const & _clocks;      /*!< Clock variables */
  std::function<void(std::string const &)> _error;  /*!< Error logging func */
};

} // end of namespace details

tchecker::typed_expression_t * typecheck(tchecker::expression_t const & expr, tchecker::integer_variables_t const & localvars,
                                         tchecker::integer_variables_t const & intvars,
                                         tchecker::clock_variables_t const & clocks,
                                         std::function<void(std::string const &)> error)
{
  tchecker::details::expression_typechecker_t v(localvars, intvars, clocks, error);
  expr.visit(v);
  return v.release();
}

} // end of namespace tchecker
