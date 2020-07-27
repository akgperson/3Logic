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
      TermVec cached_tcc;

      for (auto c : t) {
        cached_children.push_back(c);
        cached_tcc.push_back(cache_.at(c));
      }

      if (op.prim_op == Or) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == And) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Ite) {
        Term condition = solver_->make_term(And, cached_tcc[0], solver_->make_term(Ite, cached_children[0], cached_tcc[1], cached_tcc[2]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Implies) {
        Term condition = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        cache_[t] = condition;
      }
      else if (op.prim_op == Not) {
        Term condition = solver_->make_term(Not, cached_tcc[0]);
        cache_[t] = condition;
      }
      else if (op.prim_op == Iff) {
        Term subcondition1 = solver_->make_term(And, cached_children[0], cached_children[1]);
        Term subcondition2 = solver_->make_term(And, solver_->make_term(Not, cached_children[0]), solver_->make_term(Not, cached_children[1]));
        Term subcondition3 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], solver_->make_term(Not, cached_children[0])), solver_->make_term(And, cached_tcc[1], solver_->make_term(Not, cached_children[1])), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        Term subcondition4 = solver_->make_term(Or, solver_->make_term(And, cached_tcc[0], cached_children[0]), solver_->make_term(And, cached_tcc[1], cached_children[1]), solver_->make_term(And, cached_tcc[0], cached_tcc[1]));
        Term condition = solver_->make_term(Or, solver_->make_term(And, subcondition1, subcondition3), solver_->make_term(And, subcondition4, subcondition2), solver_->make_term(And, subcondition3, subcondition4));
        cache_[t] = condition;
      }

      else if (op.prim_op == Mod) { //could merge w Div
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition;
      }
      else if (op.prim_op == Div) {
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition; //change
      }
      else if (op.prim_op == IntDiv) { //could merge w Div
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1], solver_->make_term(Distinct, cached_children[1], int_zero_));
        cache_[t] = condition; //change
      }

      //Quantifiers
      //TODO add exists; fix issue where get 'undeclared identifier: Exists' error
      //else if (op.prim_op == Exists) {
        //Term condition = solver_->make_term(Or, solver_->make_term(Exists, cached_children[0], solver_->make_term(And, cached_children[1], cached_tcc[1]), solver_->make_term(Forall, cached_children[0], cached_tcc[1])));
        //cache_[t] = condition;
      //}
      //TODO add Forall
      //else if (op.prim_op == Exists) {
      //}

      else { //op w/o undefined behavior
        Term condition = solver_->make_term(And, cached_tcc[0], cached_tcc[1]);
//        will need loop for non binary ops?
        cache_[t] = condition;
      }
    }
    else { //at leaf
        cache_[t] = solver_->make_term(true); //change
    }
  }
  return Walker_Continue;
}

smt::Term TCCGenerator::convert(smt::Term &t)
{
  visit(t);
  Term res = cache_.at(t);
  cout << "res = " << res << endl;
  return res;
}

smt::Term TCCGenerator::generate_tcc(smt::Term &t)
{
  //return new_tcc;
  //TCC return true iff formula undefined; look into recursive def and way to build of TCC to understand in general
  cout << t << endl;
  return t;
}
