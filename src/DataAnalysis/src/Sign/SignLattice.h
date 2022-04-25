#ifndef __SIGNLATTICE_H__
#define __SIGNLATTICE_H__

#include <map>
#include <vector>
#include <string>

using namespace std;

class SignLattice
{
    public:
        SignLattice();
        SignLattice(int);
        void Lub(SignLattice & a, SignLattice & b);
        bool operator==(const SignLattice&) const;
        string GetName();
        static SignLattice plus(SignLattice & i, SignLattice & j);
        static SignLattice minus(SignLattice & i, SignLattice & j);
        static SignLattice times(SignLattice & i, SignLattice & j);
        static SignLattice div(SignLattice & i, SignLattice & j);
        static SignLattice gt(SignLattice & i, SignLattice & j);
        static SignLattice eqq(SignLattice & i, SignLattice & j);
        static SignLattice num(int n);
        void Top();
        void Bot();
        int m_val;
    protected:
            const static int plusTable[5][5];
            const static int minusTable[5][5];
            const static int timesTable[5][5];
            const static int divTable[5][5];
            const static int gtTable[5][5];
            const static int eqqTable[5][5];
};







#endif // __SIGNLATTICE_H__