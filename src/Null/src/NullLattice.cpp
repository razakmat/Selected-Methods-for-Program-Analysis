#include "NullLattice.h"

void NullLattice::Lub(NullLattice & a, NullLattice & b)
{
    if (a.m_val || b.m_val)
        m_val = true;
    else
        m_val = false;
}

bool NullLattice::operator==(const NullLattice& a) const
{
    return m_val == a.m_val;
}

string NullLattice::GetName()
{
    if (m_val)
        return "⊤";
    else
        return "NN";
}

void NullLattice::Top()
{
    m_val = true;
}


void NullLattice::Bot()
{
    m_val = false;
}

