#ifndef __CONSTANALYSIS_H__
#define __CONSTANALYSIS_H__

#include "ConstLattice.h"
#include "../CFG.h"
#include "../AST.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

class ConstAnalysis
{
    public:
        ConstAnalysis();
        static void PrevNodes(CFGNode * node, vector<CFGNode*> & v);
        static void NextNodes(CFGNode * node, vector<CFGNode*> & v);
        static void TransferFun(CFGNode *, ConstAnalysis * lat);
        void SaveToStr(CFGNode * node);
        void Bot();
        void Lub(ConstAnalysis * a);
        bool operator==(const ConstAnalysis&) const;
        inline static vector<string> m_decl;
        struct cmp {
            bool operator()(CFGNode* const& a,CFGNode* const& b) const
            {
                return a->m_nodeNumber < b->m_nodeNumber;
            }
        };
    protected:
        inline static vector<string> m_names;
        ConstLattice Eval(Expr * e);
        map<string,ConstLattice> m_map;


};

#endif // __CONSTANALYSIS_H__