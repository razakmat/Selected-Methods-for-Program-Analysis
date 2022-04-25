#ifndef __LIVEVARLATTICE_H__
#define __LIVEVARLATTICE_H__

#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

class LiveVarLattice
{
    public:
        LiveVarLattice();
        void Lub(LiveVarLattice & b);
        bool operator==(const LiveVarLattice&) const;
        string GetName();
        void Top();
        void Bot();
        void Add(const string & name);
        void Rem(const string & name);
    protected:
        set<string> m_var;
};

#endif // __LIVEVARLATTICE_H__