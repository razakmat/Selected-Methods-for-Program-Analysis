#ifndef __TYPE_H__
#define __TYPE_H__

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include <map>

using namespace std;
class ASTNode;


struct Type
{
    virtual ~Type() = default;
    virtual string Print() = 0;
};

struct ProperType : public Type
{
    virtual ~ProperType() = default;
    virtual string Print() = 0;
};

struct TypeVar : public Type
{
    virtual ~TypeVar() = default;
    virtual string Print() = 0;
};

struct TInt : public ProperType
{
    string Print();
};
struct TPointer : public ProperType
{
    TPointer(shared_ptr<Type> t);
    string Print();
    shared_ptr<Type> m_ref;
};
struct TTerm : public TypeVar
{
    TTerm(ASTNode * e);
    string Print();
    ASTNode * m_expr;
};
struct TFunction : public ProperType
{
    string Print();
    vector<shared_ptr<Type>> m_params;
    shared_ptr<Type> m_ret;
};
struct TVar : public TypeVar
{
    TVar(const string & s);
    string Print();
    string m_name;
};

struct TFresh : public TypeVar
{
    public:
        string Print();
        string m_name;
        TFresh();
    private:
        static char m_now1;
        static char m_now2;
};

struct TRecord : public ProperType
{
    public:
        string Print();
        map<string,shared_ptr<Type>> m_fields;
};

struct TAbsentField : public ProperType
{
    public:
        string Print();
};

struct TRecursive : public Type
{
    public:
        string Print();
        shared_ptr<Type> m_fresh;
        shared_ptr<Type> m_inner;
};

#endif // __TYPE_H__