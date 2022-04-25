#ifndef __SIGNANALYSIS_H__
#define __SIGNANALYSIS_H__

#include "SignLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class SignAnalysis
{
    public:
        SignAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, SignAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(SignAnalysis * a);
        bool operator==(const SignAnalysis&) const;
        inline static vector<string> m_decl;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber < b->m_nodeNumber;
            }
        };
    protected:
        inline static vector<string> m_names;
        SignLattice Eval(Expr * e);
        map<string,SignLattice> m_map;


};



#endif // __SIGNANALYSIS_H__