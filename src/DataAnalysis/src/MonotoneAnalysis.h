#ifndef __MONOTONEANALYSIS_H__
#define __MONOTONEANALYSIS_H__

#include "CFG.h"
#include "AST.h"
#include <vector>
#include <string>
#include <map>
#include "Sign/SignAnalysis.h"
#include "Const/ConstAnalysis.h"
#include "Live/LiveVarAnalysis.h"
#include "Available/AvailableExprAnalysis.h"
#include "VeryBusy/VeryBusyExprAnalysis.h"
#include "Reaching/ReachingDefAnalysis.h"

using namespace std;


template<typename T>
class MonotoneAnalysis
{
    public:
        MonotoneAnalysis(vector<CFGNode*> & cfg);
        ~MonotoneAnalysis();
        void Worklist();
    protected:
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber < b->m_nodeNumber;
            }
        };
        T * Fun(CFGNode * node);
        T * Join(CFGNode * node);
        void Save();
        vector<CFGNode*> & m_nodes;
        map<CFGNode*,T*,cmp> m_map3;
};
        
#include "MonotoneAnalysis.cpp"







#endif // __MONOTONEANALYSIS_H__