#ifndef __VERYBUSYEXPRANALYSIS_H__
#define __VERYBUSYEXPRANALYSIS_H__

#include "../Available/AvailableExprLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class VeryBusyExprAnalysis
{
    public:
        VeryBusyExprAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, VeryBusyExprAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(VeryBusyExprAnalysis * a);
        bool operator==(const VeryBusyExprAnalysis&) const;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber > b->m_nodeNumber;
            }
        };
    protected:
        void Eval(Expr * e);
        AvailableExprLattice m_lat;
        bool m_input;


};

#endif // __VERYBUSYEXPRANALYSIS_H__
