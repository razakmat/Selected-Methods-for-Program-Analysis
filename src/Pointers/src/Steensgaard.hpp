#ifndef __STEENSGAARD_H__
#define __STEENSGAARD_H__

#include <string>
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include "../../DataAnalysis/src/AST.h"

using namespace std;

struct Term{
    virtual ~Term() = default;
    string m_name;
};

struct AllocTerm : public Term{
    AllocTerm(int num);
};

struct VarTerm : public Term{
    VarTerm(const string & name);
};

struct PointerTerm : public Term{
    PointerTerm(Term * ref);
    Term * m_ref;
};

struct FreshTerm : public Term{
    FreshTerm();
    static int m_num;
};

class Unification
{
    public:
        void Unify(Term * t1, Term * t2);
        void Close();
        Term * GetSecond(Term * t);
    private:
        void MakeSet(Term * t);
        Term * Find(Term * t);
        void Union(Term * t1, Term * t2);
        unordered_map<Term*,Term*> m_map;

};



class Steensgaard
{
    public:
        Steensgaard(Program * program);
        ~Steensgaard();
        void Start();
        void GetResult(map<string,vector<string>> & res);
    private:
        void CreateConstraints(vector<StmtInNestedBlock*> & block);
        map<string,VarTerm*> m_vars;
        vector<PointerTerm*> m_pointers; 
        vector<StmtInNestedBlock*> m_stmts;
        Unification solver;
        int m_num_alloc = 1;
        map<string,Term*> m_res;
};




using namespace std;



#endif // __STEENSGAARD_H__