/*
 * This file is a part of the TChecker project.
 *
 * See files AUTHORS and LICENSE for copyright details.
 *
 */

#include <string>
#include <unordered_set>

#include "tchecker/basictypes.hh"
#include "tchecker/expression/expression.hh"
#include "tchecker/expression/static_analysis.hh"
#include "tchecker/expression/typechecking.hh"
#include "tchecker/expression/typed_expression.hh"
#include "tchecker/parsing/parsing.hh"
#include "tchecker/statement/statement.hh"
#include "tchecker/statement/static_analysis.hh"
#include "tchecker/statement/typechecking.hh"
#include "tchecker/statement/typed_statement.hh"
#include "tchecker/variables/clocks.hh"
#include "tchecker/variables/intvars.hh"
#include "tchecker/clockbounds/solver.hh"

TEST_CASE("extracting constants from expressions", "[extract_constant]")
{
  tchecker::integer_variables_t intvars;
  intvars.declare("i", 1, 0, 5, 0);
  intvars.declare("j", 1, 0, 1, 1);
  intvars.declare("k", 1, -2, 2, 0);

//   tchecker::intvar_id_t i = intvars.id("i");
//   tchecker::intvar_id_t j = intvars.id("j");
//   tchecker::intvar_id_t k = intvars.id("k");

  tchecker::clock_variables_t clocks;
  clocks.declare("x", 1);
  clocks.declare("y", 1);

//   tchecker::clock_id_t x = clocks.id("x");
//   tchecker::clock_id_t y = clocks.id("y");

  SECTION("non-diagonal constraint")
  {
      std::string nondiag{"x < 2"};
      tchecker::expression_t * expr = tchecker::parsing::parse_expression("", nondiag);
      REQUIRE(expr != nullptr);

      tchecker::integer_variables_t lvars;
      tchecker::typed_expression_t * typed_expr = tchecker::typecheck(*expr, lvars, intvars, clocks);
      REQUIRE(typed_expr != nullptr);

      tchecker::integer_t constant = tchecker::amap::extract_constant(*typed_expr);
      REQUIRE(constant == 2);

      delete typed_expr;
      delete expr;
  }

  SECTION("diagonal constraint")
  {
      std::string nondiag{"x - y >= -2"};
      tchecker::expression_t * expr = tchecker::parsing::parse_expression("", nondiag);
      REQUIRE(expr != nullptr);

      tchecker::integer_variables_t lvars;
      tchecker::typed_expression_t * typed_expr = tchecker::typecheck(*expr, lvars, intvars, clocks);
      REQUIRE(typed_expr != nullptr);

      tchecker::integer_t constant = tchecker::amap::extract_constant(*typed_expr);
      REQUIRE(constant == -2);

      delete typed_expr;
      delete expr;
  }
  
  SECTION("conjunction")
  {
      std::string nondiag{"x - y >= -4 && x < 4 && i > 10"};
      tchecker::expression_t * expr = tchecker::parsing::parse_expression("", nondiag);
      REQUIRE(expr != nullptr);

      tchecker::integer_variables_t lvars;
      tchecker::typed_expression_t * typed_expr = tchecker::typecheck(*expr, lvars, intvars, clocks);
      REQUIRE(typed_expr != nullptr);

      tchecker::integer_t constant = tchecker::amap::extract_constant(*typed_expr);
      REQUIRE(constant == 4);

      delete typed_expr;
      delete expr;
  }
}

TEST_CASE("extracting expressions from updates", "[extract_updates]")
{
    tchecker::integer_variables_t intvars;

    tchecker::clock_variables_t clocks;
    clocks.declare("x", 1);
    clocks.declare("y", 1);

    SECTION("clock to positive integer")
    {
        std::string up{"x=10"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z == nullptr);
        REQUIRE(tchecker::const_evaluate(*d) == 10);
    }

    SECTION("clock to negative integer")
    {
        std::string up{"x=-10"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z == nullptr);
        REQUIRE(tchecker::const_evaluate(*d) == -10);
    }

    SECTION("clock to clock")
    {
        std::string up{"x=y"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z->to_string() == "y");
        REQUIRE(d == nullptr);
    }

    SECTION("clock to sum")
    {
        std::string up{"x=-10+y"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z->to_string() == "y");
        REQUIRE(tchecker::const_evaluate(*d) == -10);
    }

    SECTION("no update")
    {
        std::string up{"y=10+x"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z->to_string() == "x");
        REQUIRE(d == nullptr);
    }

    SECTION("sequence of updates")
    {
        std::string up{"y=10+x;x=2"};
        std::string clk_x{"x"};

        tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
        REQUIRE(up_expr != nullptr);

        tchecker::expression_t * x_expr = tchecker::parsing::parse_expression("", clk_x);
        REQUIRE(x_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
        REQUIRE(typed_up != nullptr);
        tchecker::typed_expression_t * typed_x_temp = tchecker::typecheck(*x_expr, lvars, intvars, clocks);
        REQUIRE(typed_x_temp != nullptr);
        REQUIRE(typed_x_temp->type() == tchecker::EXPR_TYPE_CLKVAR);

        tchecker::typed_lvalue_expression_t * typed_x = dynamic_cast<tchecker::typed_lvalue_expression_t *>(typed_x_temp);
        REQUIRE(typed_x != nullptr);

        tchecker::typed_expression_t * z = nullptr, * d = nullptr;
        tchecker::amap::extract_updates(*typed_up, *typed_x, z, d);

        REQUIRE(z == nullptr);
        REQUIRE(tchecker::const_evaluate(*d) == 2);
    }
}

TEST_CASE("constructing up-inverse expression", "[compute_upinverse]")
{
    tchecker::integer_variables_t intvars;
    intvars.declare("i", 1, 0, 5, 0);
    intvars.declare("j", 1, 0, 1, 1);
    intvars.declare("k", 1, -2, 2, 0);

    // tchecker::intvar_id_t i = intvars.id("i");
    // tchecker::intvar_id_t j = intvars.id("j");
    // tchecker::intvar_id_t k = intvars.id("k");

    tchecker::clock_variables_t clocks;
    clocks.declare("x", 1);
    clocks.declare("y", 1);
    clocks.declare("z", 1);
    clocks.declare("w", 1);

    // tchecker::clock_id_t x = clocks.id("x");
    // tchecker::clock_id_t y = clocks.id("y");

    SECTION("non-diagonal constraints")
    {
        SECTION("phi := x < c ; up := x=y")
        {
            std::string phi{"x<2"};
            std::string up{"x=y"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_simple_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_simple_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "y<2");
        }

        SECTION("phi := x >= c ; up := x=1")
        {
            std::string phi{"x>=2"};
            std::string up{"x=1"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_simple_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_simple_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre == nullptr);
        }

        SECTION("phi := x >= c ; up := y=1")
        {
            std::string phi{"x>=2"};
            std::string up{"y=1"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_simple_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_simple_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "x>=2");
        }
    }

    SECTION("diagonal constraints")
    {
        SECTION("phi := x - y < c ; up := x=z")
        {
            std::string phi{"x-y<2"};
            std::string up{"x=z"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "z-y<2");
        }

        SECTION("phi := x - y < c ; up := y=d")
        {
            std::string phi{"x-y<=2"};
            std::string up{"y=-1"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "x<=1");
        }

        SECTION("phi := x - y < c ; up := x=d")
        {
            std::string phi{"x-y<=2"};
            std::string up{"x=3"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "y>=1");
        }

        SECTION("phi := x - y < c ; up := x=d ; upinverse = T")
        {
            std::string phi{"x-y<=2"};
            std::string up{"x=1"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre == nullptr);
        }

        SECTION("phi := x - y >= c ; up := y=d ; upinverse = T")
        {
            std::string phi{"x-y>=2"};
            std::string up{"x=-3"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre == nullptr);
        }

        SECTION("phi := x - y >= c ; up := x=d+w;y=z")
        {
            std::string phi{"x-y>=2"};
            std::string up{"x=3+w;y=z"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "z-w<=1");
        }

        SECTION("phi := x - y >= c ; up := x=3+w;y=w;z=2 ; upinverse = T")
        {
            std::string phi{"x-y>=2"};
            std::string up{"x=3+w;y=w;z=2"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre == nullptr);
        }

        SECTION("phi := x - y >= c ; up := x=4+z;y=1+w ; upinverse = T")
        {
            std::string phi{"x-y>=2"};
            std::string up{"x=4+z;y=1+w"};

            tchecker::typed_expression_t * pre = nullptr;

            tchecker::statement_t * up_expr = tchecker::parsing::parse_statement("", up);
            REQUIRE(up_expr != nullptr);

            tchecker::expression_t * phi_expr = tchecker::parsing::parse_expression("", phi);
            REQUIRE(phi_expr != nullptr);

            tchecker::integer_variables_t lvars;
            tchecker::typed_statement_t * typed_up = tchecker::typecheck(*up_expr, lvars, intvars, clocks);
            REQUIRE(typed_up != nullptr);
            
            tchecker::typed_diagonal_clkconstr_expression_t const * typed_phi = dynamic_cast<tchecker::typed_diagonal_clkconstr_expression_t const *> (tchecker::typecheck(*phi_expr, lvars, intvars, clocks));
            REQUIRE(typed_phi != nullptr);
            
            tchecker::amap::compute_upinverse(typed_phi, *typed_up, pre);

            REQUIRE(pre->to_string() == "w-z<=1");
        }
    }
}

TEST_CASE("guard based optimization", "[guard_based_optimization]")
{
    tchecker::integer_variables_t intvars;
    intvars.declare("i", 1, 0, 5, 0);
    intvars.declare("j", 1, 0, 1, 1);
    intvars.declare("k", 1, -2, 2, 0);

    // tchecker::intvar_id_t i = intvars.id("i");
    // tchecker::intvar_id_t j = intvars.id("j");
    // tchecker::intvar_id_t k = intvars.id("k");

    tchecker::clock_variables_t clocks;
    clocks.declare("x", 1);
    clocks.declare("y", 1);
    clocks.declare("z", 1);
    clocks.declare("w", 1);

    // tchecker::clock_id_t x = clocks.id("x");
    // tchecker::clock_id_t y = clocks.id("y");

    SECTION("row 1 : satisfied")
    {
        std::string upinv{"x<=2"};
        std::string g{"z<=1&&x<3&&y>=2"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 1 : not satisfied")
    {
        std::string upinv{"x<=2"};
        std::string g{"z<=1&&x-y<3&&x>=2"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x<=2");
    }

    SECTION("row 2 : satisfied")
    {
        std::string upinv{"x>2"};
        std::string g{"z<=1&&x<1&&x-y>=3"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x>=1");
    }

    SECTION("row 2 : not satisfied")
    {
        std::string upinv{"x>2"};
        std::string g{"z<=1&&x<=2&&x-y>=3"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x>2");
    }

    SECTION("row 2 : not satisfied")
    {
        std::string upinv{"x>2"};
        std::string g{"z<=1&&y<=2&&x-y<1"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x>2");
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y<=2"};
        std::string g{"z<=1&&x<=1&&x-w<10"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y<=2"};
        std::string g{"z<=1&&x-y<=1&&x-w<10"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y<=2"};
        std::string g{"z<=1&&x-y>=3&&x<3"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : not satisfied")
    {
        std::string upinv{"x-y<=2"};
        std::string g{"x<3&&x-y<=3&&x-y>=1"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x-y<=2");
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y>=2"};
        std::string g{"z<=1&&x<=1&&x-w<10"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y>2"};
        std::string g{"z<=1&&x-y<=1&&x-w<10"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : satisfied")
    {
        std::string upinv{"x-y>=2"};
        std::string g{"z<=1&&x-y>=3&&x<3"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv == nullptr);
    }

    SECTION("row 3 : not satisfied")
    {
        std::string upinv{"x-y>2"};
        std::string g{"x<3&&x-y<=3&&x-y>=1"};

        tchecker::expression_t * upinv_expr = tchecker::parsing::parse_expression("", upinv);
        REQUIRE(upinv_expr != nullptr);

        tchecker::expression_t * g_expr = tchecker::parsing::parse_expression("", g);
        REQUIRE(g_expr != nullptr);

        tchecker::integer_variables_t lvars;
        tchecker::typed_expression_t * typed_upinv = tchecker::typecheck(*upinv_expr, lvars, intvars, clocks);
        REQUIRE(typed_upinv != nullptr);
        tchecker::typed_expression_t * typed_g = tchecker::typecheck(*g_expr, lvars, intvars, clocks);
        REQUIRE(typed_g != nullptr);

        tchecker::amap::guard_based_optimization(typed_upinv, *typed_g);

        REQUIRE(typed_upinv->to_string() == "x-y>2");
    }
}