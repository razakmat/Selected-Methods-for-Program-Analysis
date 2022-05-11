#ifndef __NULLLATTICE_H__
#define __NULLLATTICE_H__

#include <string>

using namespace std;

class NullLattice
{
    public:
        void Lub(NullLattice & a, NullLattice & b);
        bool operator==(const NullLattice&) const;
        string GetName();
        void Top();
        void Bot();
        bool m_val;
    protected:
};






#endif // __NULLLATTICE_H__