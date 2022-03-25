#ifndef JSONTOAST_H
#define JSONTOAST_H

#include <fstream>
#include "nlohmann/json.hpp"
#include "AST.h"

using namespace std;
using json = nlohmann::json;

struct ParserException : public exception
{
    const char* what () const throw()
    {
        return "Error parsing JSON";
    }
};

class JSONToAST
{
    public:
        Program * convert(ifstream & file);
    private:
        Expr * convertExpr(json & j);
        StmtInNestedBlock * convertStmtInNestedBlock(json & j);
        Program * JSONProgram(json & j);
        BinaryOp * JSONBinaryOp(json & j);
        Null * JSONNull(json & j);
        Number * JSONNumber(json & j);
        Identifier * JSONIdentifier(json & j);
        CallFuncExpr * JSONCallFuncExpr(json & j);
        Input * JSONInput(json & j);
        Alloc * JSONAlloc(json & j);
        VarRef * JSONVarRef(json & j);
        Deref * JSONDeref(json & j);
        RecordField * JSONRecordField(json & j);
        Record * JSONRecord(json & j);
        FieldAccess * JSONFieldAccess(json & j);
        AssignStmt * JSONAssignStmt(json & j);
        NestedBlockStmt * JSONNestedBlockStmt(json & j);
        IdentifierDecl * JSONIdentifierDecl(json & j);
        VarStmt * JSONVarStmt(json & j);
        ReturnStmt * JSONReturnStmt(json & j);
        FunBlockStmt * JSONFunBlockStmt(json & j);
        IfStmt * JSONIfStmt(json & j);
        WhileStmt * JSONWhileStmt(json & j);
        OutputStmt * JSONOutputStmt(json & j);
        FunDecl * JSONFunDecl(json & j);

        Loc JSONLoc(json & j);
};


#endif
