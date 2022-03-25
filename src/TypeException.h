#ifndef __TYPEEXCEPTION_H__
#define __TYPEEXCEPTION_H__

#include <string>
#include "AST.h"

using namespace std;

class TypeException : public exception
{
    public:
        TypeException(const string & msg,const Loc & loc)
        {
           m_msg = "Type Error[" + to_string(loc.m_line) + "," + to_string(loc.m_col) + "] : " + msg;
        }
        TypeException(const string & msg)
        {
           m_msg = "Type Error: " + msg;
        }
    
        virtual const char* what() const noexcept override
        {
            return m_msg.c_str();
        }
    private:
        string m_msg;
}; 



#endif // __TYPEEXCEPTION_H__