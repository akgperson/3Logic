#include "tcc.h"
using namespace smt;
using namespace std;

TCCGenerator::TCCGenerator(SmtSolver &solver, bool b) : IdentityWalker(solver, b)
{
  counter = 0;
  Sort int_sort = solver_->make_sort(INT);
  int_zero_ = solver_->make_term(0, int_sort);
};

TCCGenerator::~TCCGenerator() { };

smt::WalkerStepResult TCCGenerator::visit_term(smt::Term &t)
{
  if (!preorder_) { //if current term not being visited for first time (on wind out) [Note: preorder_ true when current term is being visited for the first time]
    Op op = t->get_op();

    if (!op.is_null()) {

      TermVec cached_children;

      for (auto c : t) {
        cached_children.push_back(cache_.at(c));
      }

      cache_[t] = solver_->make_term(op, cached_children);
      cout << "op not null: " << cache_[t] <<endl;

//      if (op.prim_op == Plus) {
//        counter++;
//      }

//      if (op.prim_op == Div) {
        //building up TCC with terms that have division by 0
        //Term condition = solver_->make_term(Distinct, cached_children[1], int_zero_);
//        //cache_[t] = condition;
//      }
    }
    else {
      cache_[t] = t;
      cout << "op is null: " << cache_[t] <<endl;
    }
  }
  return Walker_Continue;
}

smt::Term TCCGenerator::convert(smt::Term &t)
{
  visit(t);
  Term res = cache_.at(t);
  return res;
}

smt::Term TCCGenerator::generate_tcc(smt::Term &t)
{
  //return new_tcc;
  //TCC return true iff formula undefined; look into recursive def and way to build of TCC to understand in general
  cout << t << endl;
  return t;
}
