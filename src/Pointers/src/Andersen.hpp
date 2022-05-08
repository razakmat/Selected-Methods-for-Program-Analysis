#ifndef __ANDERSEN_H__
#define __ANDERSEN_H__

#include <string>
#include <map>
#include <set>
#include <vector>
#include "../../DataAnalysis/src/AST.h"

using namespace std;

struct Cell{
    virtual ~Cell() = default;
    set<Cell*> m_sol;
    set<Cell*> m_succ;
    map<Cell*,map<Cell*,Cell*>> m_cond;
    string m_name;
};

struct VarCell : public Cell{
    VarCell(const string & str);
};

struct AllocCell : public Cell{
    AllocCell(int num);
};

class CubicSolver{
    public:
        void ConsAddT(Cell * t, Cell * x);
        void ConsAddE(Cell * y, Cell * z);
        void ConsImpl(Cell * t, Cell * x, Cell * y, Cell * z);
    private:
        void AddToken(Cell * t, Cell * x);
        void AddEdge(Cell * x, Cell * y);
        void Propagate();
        vector<pair<Cell*,Cell*>> m_worklist;
};


class Andersen
{
    public:
        Andersen(Program * program);
        ~Andersen();
        void Start();
        void GetResult(map<string,vector<string>> & res);
    private:
        void GetExpr(Expr * expr);
        void FindCells(vector<StmtInNestedBlock*> & block);
        void CreateConstraints(vector<StmtInNestedBlock*> & block);
        map<string,VarCell*> m_varCells;
        set<VarCell*> m_refVars;
        vector<AllocCell*> m_allocCells;
        vector<StmtInNestedBlock*> m_stmts;
        int m_num_alloc = 1;
        CubicSolver solver;

};





#endif // __ANDERSEN_H__