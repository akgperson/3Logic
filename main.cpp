#include <iostream>
#include <memory>
#include <vector>
#include "assert.h"

// #include "cvc4_factory.h"
// #include "smt.h"
// after full installation
#include "smt-switch/cvc4_factory.h"
#include "smt-switch/smt.h"
#include "tcc.h"

using namespace smt;
using namespace std;

int main()
{
  SmtSolver s = CVC4SolverFactory::create(false);
  s->set_opt("produce-models", "true");
  s->set_logic("QF_NIA");
  Sort intsort = s->make_sort(INT);
  Term x = s->make_symbol("x", intsort);
  Term y = s->make_symbol("y", intsort);
  Term z = s->make_symbol("z", intsort);

  Term a = s->make_symbol("a", intsort);
  Term b = s->make_symbol("b", intsort);
  Term c = s->make_symbol("c", intsort);
  Term d = s->make_symbol("d", intsort);

  Term w = s->make_term(Equal, z, s->make_term(Plus, x, y));

  Term w2 = s->make_term(Equal, s->make_term(Div, a, b), s->make_term(Plus, d, a));

  TCCGenerator tccg(s, 1);
  Term tcc1 = tccg.convert(w);
  Term tcc2 = tccg.convert(w2);
//  Term tcc1 = tccg.generate_tcc(t_k);
//  Term tcc2 = tccg.generate_tcc(t_j);

  //checks satisfiability of tcc
  //TODO: make this checking tcc step one tcc at a time, reseting solver between calls
  s->assert_formula(tcc2);
  Result tcc1_res = s->check_sat();
  assert(tcc1_res.is_sat());

  //check that want to check tcc's separately in own passes before checking formulas either in individual passes, again, or together
  s->reset_assertions();
  s->assert_formula(tcc1);
  Result tcc2_res = s->check_sat();
  assert(tcc2_res.is_sat());

  s->reset_assertions();
  s->assert_formula(w);
  s->assert_formula(w2);
  Result res = s->check_sat();
  assert(res.is_sat());

  cout << "Model Values:" << endl;
  for (auto t : TermVec({ x, y, z }))
  {
    cout << "\t" << t << " = " << s->get_value(t) << endl;
  }


  //More Tests
  //TEST: mult div
  Term e1 = s->make_symbol("x1", intsort);
  Term e2 = s->make_symbol("x2", intsort);
  Term e3 = s->make_symbol("x3", intsort);
  Term e4 = s->make_symbol("x4", intsort);
  Term e5 = s->make_term(1, intsort);
  Term w3 = s->make_term(Iff, s->make_term(Equal, s->make_term(Plus, e1, e5), e2), s->make_term(Equal, s->make_term(Div, e1, e3), e4));
  Term tcc3 = tccg.convert(w3);
  //sat of TCC
  s->reset_assertions();
  s->assert_formula(tcc3);
  Result tcc_res = s->check_sat();
  assert(tcc_res.is_sat());
  //sat of formula
  s->reset_assertions();
  s->assert_formula(w3);
  Result res3 = s->check_sat();
  assert(res3.is_sat());

  return 0;
}












