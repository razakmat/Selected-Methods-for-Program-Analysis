#ifndef __CFG_H__
#define __CFG_H__

#include "AST.h"
#include <vector>
#include <string>
using namespace std;


class CFGNode
{
    public:
        CFGNode(int num);
        virtual void PrintOut(ostream & os) = 0;
        vector<CFGNode*> m_prev;
        vector<CFGNode*> m_succ;
        int m_nodeNumber;
        string m_analysis;
    protected:
        void PrintOutSucc(ostream & os);
        void PrintOutAnalysis(ostream & os);
};

// for flow-sensitive pointer analysis
class CFGAssignP : public CFGNode
{
    public:
        CFGAssignP(Expr * l, Expr * r, int num);
        virtual void PrintOut(ostream & os);
        Expr * m_left;
        Expr * m_right;
};

class CFGAssign : public CFGNode
{
    public:
        CFGAssign(Identifier * l,Expr * r, int num);
        virtual void PrintOut(ostream & os);
        Identifier * m_left;
        Expr * m_right;
};

class CFGOutput : public CFGNode
{
    public:
        CFGOutput(Expr * r, int num);
        virtual void PrintOut(ostream & os);
        Expr * m_right;
};

class CFGReturn : public CFGNode
{
    public:
        CFGReturn(Expr * r, int num);
        virtual void PrintOut(ostream & os);
        Expr * m_right;
};

class CFGVar : public CFGNode
{
    public:
        CFGVar(vector<IdentifierDecl*> & v, int num);
        virtual void PrintOut(ostream & os);
        vector<IdentifierDecl*> m_vars;
};

class CFGStart : public CFGNode
{
    public:
        CFGStart(const string & n, vector<IdentifierDecl*> & v, int num);
        virtual void PrintOut(ostream & os);
        string m_funName;
        vector<IdentifierDecl*> m_vars;
};

class CFGEnd : public CFGNode
{
    public:
        CFGEnd(const string & n, int num);
        virtual void PrintOut(ostream & os);
        string m_funName;
};


class CFGExpr : public CFGNode
{
    public:
        CFGExpr(Expr * e, int num);
        virtual void PrintOut(ostream & os);
        Expr * m_right;
};

#endif // __CFG_H__