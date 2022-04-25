#ifndef __LIVEVARANALYSIS_H__
#define __LIVEVARANALYSIS_H__

#include "LiveVarLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class LiveVarAnalysis
{
    public:
        LiveVarAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, LiveVarAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(LiveVarAnalysis * a);
        bool operator==(const LiveVarAnalysis&) const;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber > b->m_nodeNumber;
            }
        };
    protected:
        void Eval(Expr * e);
        LiveVarLattice m_lat;


};


#endif // __LIVEVARANALYSIS_H__