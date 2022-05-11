#ifndef __NULLANALYSIS_H__
#define __NULLANALYSIS_H__

#include "NullLattice.h"
#include "../../DataAnalysis/src/CFG.h"
#include "../../DataAnalysis/src/AST.h"
#include <map>
#include <set>
#include <string>
#include <vector>


class NullAnalysis
{
    public:
        NullAnalysis();
        static void Prepare(map<string,vector<string>> & res, vector<CFGNode*> & code);
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, NullAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(NullAnalysis * a);
        bool operator==(const NullAnalysis&) const;
        inline static vector<string> m_decl;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber < b->m_nodeNumber;
            }
        };
    protected:
        NullLattice Eval(Expr * e);
        map<string,NullLattice> m_map;
        inline static map<string,vector<string>> m_res;
        inline static map<int,string> m_allocs;
};





#endif // __NULLANALYSIS_H__