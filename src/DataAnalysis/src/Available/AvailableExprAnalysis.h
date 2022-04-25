#ifndef __AVAILABLEEXPRANALYSIS_H__
#define __AVAILABLEEXPRANALYSIS_H__

#include "AvailableExprLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class AvailableExprAnalysis
{
    public:
        AvailableExprAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, AvailableExprAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(AvailableExprAnalysis * a);
        bool operator==(const AvailableExprAnalysis&) const;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber < b->m_nodeNumber;
            }
        };
    protected:
        void Eval(Expr * e);
        AvailableExprLattice m_lat;
        bool m_input;


};

#endif // __AVAILABLEEXPRANALYSIS_H__