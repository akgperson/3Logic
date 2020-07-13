#include "smt-switch/include/identity_walker.h"

class TCCGenerator : smt::IdentityWalker
{
  public:
    TCCGenerator(smt::SmtSolver &solver, bool b);
    ~TCCGenerator();
    smt::WalkerStepResult visit_term(smt::Term &t) override; //check Term and Term what input and what output? math to function overriding? New ones?
    smt::Term generate_tcc(smt::Term &t); //in parenthesis? what feed to my function?
    smt::Term convert(smt::Term &t);

    int counter; //counter for no. of +'s in a given input term
    std::vector<smt::Term> cached_children;

    using TermVec = std::vector<smt::Term>;
    smt::Term int_zero_;

//    using UnorderedTermMap = std::unordered_map<smt::Term, smt::Term>;
//    smt::UnorderedTermMap cache2;
};

