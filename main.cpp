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
//  s->assert_formula(w);

  Term w2 = s->make_term(Equal, s->make_term(Div, a, b), s->make_term(Plus, d, a));
  //Term w2 = s->make_term(Equal, s->make_term(Div, a, s->make_term(0, intsort)), s->make_term(Plus, d, a));
//  s->assert_formula(w2);

  TCCGenerator tccg(s, 1);
  Term tcc1 = tccg.convert(w);
  Term tcc2 = tccg.convert(w2);
//  Term tcc1 = tccg.generate_tcc(t_k);
//  Term tcc2 = tccg.generate_tcc(t_j);
  s->assert_formula(tcc1);
  s->assert_formula(tcc2);

  //checks satisfiability of tcc
  //TODO: make this checking tcc step one tcc at a time, reseting solver between calls
  Result r = s->check_sat();
  assert(r.is_sat());
  //TODO output satisfiability of tcc and maybe tcc itself here

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
  return 0;
}
