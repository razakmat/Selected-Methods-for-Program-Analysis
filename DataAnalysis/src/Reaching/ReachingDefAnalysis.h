#ifndef __REACHINGDEFANALYSIS_H__
#define __REACHINGDEFANALYSIS_H__

#include "ReachingDefLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class ReachingDefAnalysis
{
    public:
        ReachingDefAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, ReachingDefAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(ReachingDefAnalysis * a);
        bool operator==(const ReachingDefAnalysis&) const;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber > b->m_nodeNumber;
            }
        };
    protected:
        ReachingDefLattice m_lat;


};

#endif // __REACHINGDEFANALYSIS_H__