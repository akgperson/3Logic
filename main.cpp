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

  s->assert_formula(s->make_term(Equal, z, s->make_term(Plus, x, y)));

  TCCGenerator tccg;
  Term t_f = tccg->convert(s);
  //Term tcc = tccg.generate_tcc(t_f);

  Result r = s->check_sat();
  assert(r.is_sat());

  cout << "Model Values:" << endl;
  for (auto t : TermVec({ x, y, z }))
  {
    cout << "\t" << t << " = " << s->get_value(t) << endl;
  }
  return 0;
}
