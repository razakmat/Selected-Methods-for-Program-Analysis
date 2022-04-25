#ifndef __ASTTOCFG_H__
#define __ASTTOCFG_H__

#include "AST.h"
#include "CFG.h"
#include <string>
using namespace std;

class ASTToCFG
{
    public:
        CFGNode * CreateCFG(Program * AST);
        vector<CFGNode*> & GetVectorNodes();
        vector<string> & GetVectorNames();
    protected:
        void CreateVars(FunBlockStmt * funBlock);
        void CreateStmts(vector<StmtInNestedBlock*> & block);
        void CreateStmt(StmtInNestedBlock * stmt);
        void CreateReturnAndEnd(ReturnStmt * ret, const string & name);
        void WritePrev(CFGNode * now);
        vector<CFGNode*> m_prev;
        int m_counter = 0;
        vector<CFGNode*> m_nodes;
        vector<string> m_names;
};


#endif // __ASTTOCFG_H__