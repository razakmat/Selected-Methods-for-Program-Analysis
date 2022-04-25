#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <memory>
#include <ostream>
using namespace std;

class Visitor;

struct Loc
{
    int m_line;
    int m_col;
};


class BinaryOperator
{
public:
    virtual int Apply(int left, int right) = 0;
    virtual void PrintOut(ostream & os) = 0;
};
class Plus : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};
class Minus : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};
class Times : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};
class Divide : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};
class Equal : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};
class GreaterThan : public BinaryOperator
{
public:
    int Apply(int left, int right);
    virtual void PrintOut(ostream & os);
};


class ASTNode
{
public:
    ASTNode(const Loc & loc);
    virtual ~ASTNode() = default;
    virtual ASTNode * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
    Loc m_loc;
};


class Expr : public ASTNode
{
public:
    Expr(const Loc & loc);
    virtual ~Expr() = default;
    virtual Expr * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
};

class Stmt : public ASTNode
{
public:
    Stmt(const Loc & loc);
    virtual ~Stmt() = default;
    virtual Stmt * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
};

class Decl : public ASTNode
{
public:
    Decl(const string & name, const Loc & loc);
    virtual ~Decl() = default;
    virtual Decl * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
    string m_name;
};


class StmtInNestedBlock : public Stmt
{
public:
    StmtInNestedBlock(const Loc & loc);
    virtual ~StmtInNestedBlock() = default;
    virtual StmtInNestedBlock * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
};

class Null : public Expr
{
public:
    Null(const Loc & loc);
    ~Null();
    Null * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
};

class Number : public Expr
{
public:
    Number(int value, const Loc & loc);
    ~Number();
    Number * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    int m_value;
};

class Identifier : public Expr
{
public:
    Identifier(const string & name, const Loc & loc);
    ~Identifier();
    Identifier * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    string m_name;
};

class BinaryOp : public Expr
{
public:
    BinaryOp(BinaryOperator * op, Expr * left, Expr * right,const Loc & loc);
    ~BinaryOp();
    BinaryOp * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    BinaryOperator * m_operator;
    Expr * m_left;
    Expr * m_right;
};

class CallFuncExpr : public Expr
{
public:
    CallFuncExpr(Expr * targetFun, const vector<Expr*> arg, const Loc & loc);
    ~CallFuncExpr();
    CallFuncExpr * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_targetFun;
    vector<Expr*> m_arg;
};


class Input : public Expr
{
public:
    Input(const Loc & loc);
    ~Input();
    Input * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
};

class Alloc : public Expr
{
public:
    Alloc(Expr * expr, const Loc & loc);
    ~Alloc();
    Alloc * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_expr;
};


class VarRef : public Expr
{
public:
    VarRef(Identifier * id, const Loc & loc);
    ~VarRef();
    VarRef * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Identifier * m_id;
};

class Deref : public Expr
{
public:
    Deref(Expr * pointer, const Loc & loc);
    ~Deref();
    Deref * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_pointer;
};

class RecordField : public ASTNode
{
public:
    RecordField(const string & name, Expr * expr, const Loc & loc);
    ~RecordField();
    RecordField * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    string m_name;
    Expr * m_expr;
};

class Record : public Expr
{
public:
    Record(const vector<RecordField*> & fields,const Loc & loc);
    ~Record();
    Record * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<RecordField*> m_fields;
};


class FieldAccess : public Expr
{
public:
    FieldAccess(Expr * record, const string & field, const Loc & loc);
    ~FieldAccess();
    FieldAccess * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_record;
    string m_field;
};

class AssignStmt : public StmtInNestedBlock
{
public:
    AssignStmt(Expr * left, Expr * right, const Loc & loc);
    ~AssignStmt();
    AssignStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_left;
    Expr * m_right;
};


class Block : public Stmt
{
public:
    Block(const Loc & loc);
    virtual ~Block() = default;
    virtual  Block * Accept(Visitor & v) = 0;
    virtual void PrintOut(ostream & os) = 0;
};


class NestedBlockStmt : public Block
{
public:
    NestedBlockStmt(const vector<StmtInNestedBlock*> & body, const Loc & loc);
    ~NestedBlockStmt();
    NestedBlockStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<StmtInNestedBlock*> m_body;
};


class IdentifierDecl : public Decl
{
public:
    IdentifierDecl(const string & name, const Loc & loc);
    ~IdentifierDecl();
    IdentifierDecl * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
};

class VarStmt : public Stmt
{
public:
    VarStmt(const vector<IdentifierDecl*> & decls, const Loc & loc);
    ~VarStmt();
    VarStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<IdentifierDecl*> m_decls;
};


class ReturnStmt : public Stmt
{
public:
    ReturnStmt(Expr * expr, const Loc & loc);
    ~ReturnStmt();
    ReturnStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_expr;
};

class FunBlockStmt : public Block
{
public:
    FunBlockStmt(const vector<VarStmt*> & vars,
                 const vector<StmtInNestedBlock*> & stmts, 
                 ReturnStmt * ret,const Loc & loc);
    ~FunBlockStmt();
    FunBlockStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<VarStmt*> m_vars;
    vector<StmtInNestedBlock*> m_stmts;
    ReturnStmt * m_ret;
};

class IfStmt : public StmtInNestedBlock
{
public:
    IfStmt(Expr * guard, NestedBlockStmt * thenBranch, 
           NestedBlockStmt * elseBranch,const Loc & loc);
    ~IfStmt();
    IfStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_guard;
    NestedBlockStmt * m_thenBranch;
    NestedBlockStmt * m_elseBranch;
};

class WhileStmt : public StmtInNestedBlock
{
public:
    WhileStmt(Expr * guard, NestedBlockStmt * block, const Loc & loc);
    ~WhileStmt();
    WhileStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_guard;
    NestedBlockStmt * m_block;
};

class OutputStmt : public StmtInNestedBlock
{
public:
    OutputStmt(Expr * expr, const Loc & loc);
    ~OutputStmt();
    OutputStmt * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    Expr * m_expr;
};


class FunDecl : public Decl
{
public:
    FunDecl(const vector<IdentifierDecl*> params, FunBlockStmt * block,
            const string & name, const Loc & loc);
    ~FunDecl();
    FunDecl * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<IdentifierDecl*> m_params;
    FunBlockStmt * m_block;
};

class Program : public ASTNode
{
public:
    Program(vector<FunDecl*> & funs,const Loc & loc);
    ~Program();
    Program * Accept(Visitor & v);
    virtual void PrintOut(ostream & os);
    vector<FunDecl*> m_funs;
};

#endif