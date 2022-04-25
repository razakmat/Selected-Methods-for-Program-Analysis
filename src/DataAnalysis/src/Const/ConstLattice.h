#ifndef __CONSTLATTICE_H__
#define __CONSTLATTICE_H__

#include <string>

using namespace std;

class ConstLattice
{
    public:
        ConstLattice();
        ConstLattice(int);
        void Lub(ConstLattice & a, ConstLattice & b);
        bool operator==(const ConstLattice&) const;
        string GetName();
        static ConstLattice plus(ConstLattice & i, ConstLattice & j);
        static ConstLattice minus(ConstLattice & i, ConstLattice & j);
        static ConstLattice times(ConstLattice & i, ConstLattice & j);
        static ConstLattice div(ConstLattice & i, ConstLattice & j);
        static ConstLattice gt(ConstLattice & i, ConstLattice & j);
        static ConstLattice eqq(ConstLattice & i, ConstLattice & j);
        static ConstLattice num(int n);
        void Top();
        void Bot();
        int m_val;
        bool m_bot = false;
        bool m_top = false;
    protected:
        static ConstLattice Apply(ConstLattice & i, ConstLattice & j);
};

#endif // __CONSTLATTICE_H__