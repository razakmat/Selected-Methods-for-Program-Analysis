#include "LiveVarAnalysis.h"

LiveVarAnalysis::LiveVarAnalysis()
{
    
}


void LiveVarAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}


void LiveVarAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}


void LiveVarAnalysis::TransferFun(CFGNode * node, LiveVarAnalysis * lat)
{
    if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        lat->m_lat.Rem(n->m_left->m_name);
        lat->Eval(n->m_right);
    }
    else if (CFGVar * n = dynamic_cast<CFGVar*>(node))
    {
        for (auto & x : n->m_vars)
            lat->m_lat.Rem(x->m_name);
    }
    else if (CFGExpr * n = dynamic_cast<CFGExpr*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (CFGOutput * n = dynamic_cast<CFGOutput*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (CFGReturn * n = dynamic_cast<CFGReturn*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (dynamic_cast<CFGEnd*>(node))
    {
        lat->Bot();
    }
}

void LiveVarAnalysis::SaveToStr(CFGNode * node)
{
    string str = m_lat.GetName();
    node->m_analysis = str;
}

void LiveVarAnalysis::Bot()
{
    m_lat.Bot();
}

void LiveVarAnalysis::Lub(LiveVarAnalysis * a)
{
    m_lat.Lub(a->m_lat);
}

bool LiveVarAnalysis::operator==(const LiveVarAnalysis& a) const
{
    return m_lat == a.m_lat;
}

void LiveVarAnalysis::Eval(Expr * e)
{
    if (dynamic_cast<Number*>(e))
    {
    }
    else if (Identifier * n = dynamic_cast<Identifier*>(e))
    {
        m_lat.Add(n->m_name);
    }
    else if (BinaryOp * n = dynamic_cast<BinaryOp*>(e))
    {
        Eval(n->m_left);
        Eval(n->m_right);         
    }
    else if (dynamic_cast<Input*>(e))
    {
    }
    else
        throw "Unsupported expr";
}
