#ifndef __AVAILABLEEXPRLATTICE_H__
#define __AVAILABLEEXPRLATTICE_H__

#include "../AST.h"
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class AvailableExprLattice
{
    public:
        AvailableExprLattice();
        void Lub(AvailableExprLattice & b);
        bool operator==(const AvailableExprLattice&) const;
        string GetName();
        void Top();
        void Bot();
        void Add(Expr * expr);
        void Rem(const string & name);
    protected:
        bool FindName(const string & name, Expr * expr);
        bool m_all = false;
        map<string,Expr*> m_var;
};

#endif // __AVAILABLEEXPRLATTICE_H__