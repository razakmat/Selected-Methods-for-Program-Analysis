#include "CFG.h"

using namespace std;

void CFGAssign::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"";
    m_left->PrintOut(os);
    os << " = ";
    m_right->PrintOut(os);
    os << ";";
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGAssignP::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"";
    m_left->PrintOut(os);
    os << " = ";
    m_right->PrintOut(os);
    os << ";";
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGOutput::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"output ";
    m_right->PrintOut(os);
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGReturn::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"return ";
    m_right->PrintOut(os);
    os << ";";
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGVar::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"var ";
    for (auto & x : m_vars)
    {
        x->PrintOut(os);
        if (x != m_vars.back())
            os << ", ";
        else
            os << ";";
    }
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGStart::PrintOut(ostream & os)
{
    os << "digraph CFG {" << endl;
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"Fun " << m_funName << " entry";
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGEnd::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"Fun " << m_funName << " exit";
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    os << "}" << endl;
}

void CFGExpr::PrintOut(ostream & os)
{
    os << "n_" << to_string(m_nodeNumber);
    os << "[label=\"";
    m_right->PrintOut(os);
    PrintOutAnalysis(os);
    os << "\"]" << endl;
    PrintOutSucc(os);
}

void CFGNode::PrintOutAnalysis(ostream & os)
{
    if (!m_analysis.empty())
    {
        os << "\\n";
        os << m_analysis;
    }
}



CFGNode::CFGNode(int num)
: m_nodeNumber(num)
{
    m_analysis.clear();
}

void CFGNode::PrintOutSucc(ostream & os)
{
    for (auto & x : m_succ)
        os << "n_" << to_string(m_nodeNumber) << " -> " 
            << "n_" << to_string(x->m_nodeNumber) << endl;
}

CFGAssignP::CFGAssignP(Expr * l,Expr * r, int num)
: CFGNode(num), m_left(l),m_right(r)
{
    
}

CFGAssign::CFGAssign(Identifier * l,Expr * r, int num)
: CFGNode(num), m_left(l),m_right(r)
{
    
}

CFGOutput::CFGOutput(Expr * r, int num)
: CFGNode(num), m_right(r)
{
    
}

CFGReturn::CFGReturn(Expr * r, int num)
: CFGNode(num), m_right(r)
{
    
}

CFGVar::CFGVar(vector<IdentifierDecl*> & v, int num)
: CFGNode(num), m_vars(v)
{
    
}

CFGStart::CFGStart(const string & n, vector<IdentifierDecl*> & v, int num)
: CFGNode(num), m_funName(n), m_vars(v)
{
    
}

CFGEnd::CFGEnd(const string & n, int num)
: CFGNode(num), m_funName(n)
{
    
}

CFGExpr::CFGExpr(Expr * e, int num)
: CFGNode(num), m_right(e)
{
    
}
