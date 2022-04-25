#ifndef __VERYBUSYEXPRLATTICE_H__
#define __VERYBUSYEXPRLATTICE_H__

//NOT USED


#include "../AST.h"
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class VeryBusyExprLattice
{
    public:
        VeryBusyExprLattice();
        void Lub(VeryBusyExprLattice & b);
        bool operator==(const VeryBusyExprLattice&) const;
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

#endif // __VERYBUSYEXPRLATTICE_H__