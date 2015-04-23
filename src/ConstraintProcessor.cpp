#include "ConstraintProcessor.h"
#include <list>

using std::list;
namespace SSARI {
ConstraintProcessor::ConstraintProcessor(CVarMath *math) : mathProc(math)
{

}


bool ConstraintProcessor::satisfyConstaint(const shared_ptr<Constraint> constraint, RegisterFile &rf) {

    shared_ptr<SymbolicVar> sVar = this->genConstraint(constraint, rf);
    if(!sVar)
        throw runtime_error("Satifying Equation was NULL");

    // Determine Satisfiablity
    return this->mathProc->isSat(sVar);

}

shared_ptr<SymbolicVar> ConstraintProcessor::genConstraint(shared_ptr<Constraint> constraint, RegisterFile &rf) {
    // Temp add to register file
    if(constraint == nullptr)
        throw runtime_error("ConstraintProcessor: Null Input Constraint");


    CVar constraintVar("const_8465486");
    rf.setVar(constraintVar, constraint);
    shared_ptr<SymbolicVar> sVar = this->processConstraint(constraintVar, rf);

    return sVar;
}


shared_ptr<SymbolicVar> ConstraintProcessor::processConstraint(CVar var, RegisterFile &rf) {
    // Determine if function is already set
    /*
    auto iter = this->funcFile.find(var);
    if(iter != funcFile.end())
        return iter->second;*/
    // Fetch Constraint Associated with variable
    shared_ptr<Constraint> constraint = rf.getVar(var);


    if(!constraint)
    {
        cout << "CVar: " << var.debugInfo() << endl;
        cout << rf.dumpRegister() << endl;
        throw runtime_error("ConstraintProcessor: Error Null Constraint for " + var.toString());
    }
    // Fetch Operands
    list<shared_ptr<SymbolicVar> > operands;
    for(auto iter = constraint->cbegin(); iter != constraint->cend(); iter++)
    {
        // Determine if Operand is a CVar, could be constant
        shared_ptr<SymbolicVar> symVar;


        if(shared_ptr<CVar> opVar = dynamic_pointer_cast<CVar>(*iter)){
            symVar = this->processConstraint(*opVar, rf);
            if(symVar == nullptr)
                throw runtime_error((*iter)->toString() + " could not be found in register file");
        }
        else if(shared_ptr<CConstant> opVar = dynamic_pointer_cast<CConstant>(*iter)){
            symVar = this->mathProc->get(opVar);
            if(symVar == nullptr)
                throw runtime_error((*iter)->toString() + " constant was not generated from MathProcessor");
        }
        else if(*iter == nullptr)
            throw runtime_error("Operand is a nullptr");
        else
            throw runtime_error((*iter)->toString() + " unknown type");


        operands.push_back(symVar);


    }

    // Process Constraint
    shared_ptr<SymbolicVar> outSymbol;
    string op = constraint->getOperator().getOperator();
    if(op == "=")
    {
        // Determine if set to constant or another var
        if(operands.size() != 1)
            throw runtime_error(var.toString() + " = operation has too many operands");
        outSymbol = this->mathProc->set(var, operands.front());
    }
    else
    {
        if(operands.size() != 2)
           throw runtime_error(var.toString() + " " + op + " operation has incorrect number of operands");

        // Check Operator
        if(op == "+")
            outSymbol = this->mathProc->add(operands.front(), operands.back());
        else if(op == "-")
            outSymbol = this->mathProc->sub(operands.front(), operands.back());
        else if(op == "/")
            outSymbol = this->mathProc->div(operands.front(), operands.back());
        else if(op == "*")
            outSymbol = this->mathProc->mul(operands.front(), operands.back());
        else if(op == "<")
            outSymbol = this->mathProc->lt(operands.front(), operands.back());
        else if(op == "<=")
            outSymbol = this->mathProc->lte(operands.front(), operands.back());
        else if(op == ">")
            outSymbol = this->mathProc->gt(operands.front(), operands.back());
        else if(op == ">=")
            outSymbol = this->mathProc->gte(operands.front(), operands.back());
        else if(op == "==")
            outSymbol = this->mathProc->eq(operands.front(), operands.back());
        else if(op == "!=")
            outSymbol = this->mathProc->neq(operands.front(), operands.back());
        else if(op == "||")
            outSymbol = this->mathProc->logOr(operands.front(), operands.back());
        else if(op == "&&")
            outSymbol = this->mathProc->logAnd(operands.front(), operands.back());
        // This is currently incorrect
        else if(op == "!")
            outSymbol = this->mathProc->logNot(operands.front());
        else if(op == "|")
            outSymbol = this->mathProc->boolOr(operands.front(), operands.back());
        else if(op == "&")
            outSymbol = this->mathProc->boolAnd(operands.front(), operands.back());
        // This is also incorrect, constaints can be unary
        else if(op == "~")
            outSymbol = this->mathProc->boolNot(operands.front());
        else
            throw runtime_error("Invalid Operator: " + op);
    }

    // Update Function and return
    this->funcFile[var] = outSymbol;

    return outSymbol;

}


ConstraintProcessor::~ConstraintProcessor()
{

}

}
