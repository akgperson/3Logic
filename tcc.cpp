#include "tcc.h"
using namespace smt;
using namespace std;

TCCGenerator::TCCGenerator(SmtSolver &solver, bool b) : IdentityWalker(solver, b)
{
  //constructor
  counter = 0;
};

TCCGenerator::~TCCGenerator() { };

smt::WalkerStepResult TCCGenerator::visit_term(smt::Term &t)
{
  if (!preorder_) { //if current term not being visited for first time (on wind out) [Note: preorder_ true when current term is being visited for the first time]
    Op op = t->get_op();
    //need to do more w cached_children etc to make sure proper traversal
    if (op.prim_op == Plus) {
      //counting +'s in given formula
      counter++;
    }
   // if (op.prim_op == Div) {
      //building up TCC with terms that have division by 0
     // Term condition = solver_->make_term(Equal, cached_children[1], int_zero_); //int_zero? //cached_children?
      //cache_[t] = condition; //check what cache is? TCC set? adding right thing?
    //}
  }
  return Walker_Continue;
}

smt::Term TCCGenerator::convert(smt::Term &t)
{
  visit(t);
  //Term res = cache_.at(t); //don't totally get how cache work or what this line is actually doing
  //return res;
  cout << counter << endl;
  return t;
}

smt::Term TCCGenerator::generate_tcc(smt::Term &t)
{
  //generate the TCC given the formula
  //return new_tcc;
  //TCC return true iff formula undefined; look into recursive def and way to build of TCC to understand in general
  return t;
}
