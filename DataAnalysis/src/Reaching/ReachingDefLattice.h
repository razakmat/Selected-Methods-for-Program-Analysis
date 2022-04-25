#ifndef __REACHINGDEFLATTICE_H__
#define __REACHINGDEFLATTICE_H__

#include "../CFG.h"
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class ReachingDefLattice
{
    public:
        ReachingDefLattice();
        void Lub(ReachingDefLattice & b);
        bool operator==(const ReachingDefLattice&) const;
        string GetName();
        void Top();
        void Bot();
        void Add(CFGAssign * assign);
        void Rem(const string & name);
    protected:
        map<string,CFGAssign*> m_assign;
};

#endif // __REACHINGDEFLATTICE_H__