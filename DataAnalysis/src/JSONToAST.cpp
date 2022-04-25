#include "JSONToAST.h"

#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;


Program * JSONToAST::convert(ifstream & file)
{
    try{
        json j = json::parse(file);
        if (j.begin().key() != "Program")
            throw ParserException();
        return JSONProgram(j.begin().value());
    }catch(...){
        throw ParserException();
    }
}

StmtInNestedBlock * JSONToAST::convertStmtInNestedBlock(json & j)
{
    if (j.begin().key() == "AssignStmt")
        return JSONAssignStmt(j.begin().value());   
    if (j.begin().key() == "IfStmt")
        return JSONIfStmt(j.begin().value());
    if (j.begin().key() == "WhileStmt")
        return JSONWhileStmt(j.begin().value());
    if (j.begin().key() == "OutputStmt")
        return JSONOutputStmt(j.begin().value());

    throw ParserException();
}

Expr * JSONToAST::convertExpr(json & j)
{
    if (j.begin().key() == "Null")
        return JSONNull(j.begin().value());
    if (j.begin().key() == "Number")
        return JSONNumber(j.begin().value());
    if (j.begin().key() == "Identifier")
        return JSONIdentifier(j.begin().value());
    if (j.begin().key() == "BinaryOp")
        return JSONBinaryOp(j.begin().value());
    if (j.begin().key() == "CallFuncExpr")
        return JSONCallFuncExpr(j.begin().value());
    if (j.begin().key() == "Input")
        return JSONInput(j.begin().value());
    if (j.begin().key() == "Alloc")
        return JSONAlloc(j.begin().value());
    if (j.begin().key() == "VarRef")
        return JSONVarRef(j.begin().value());
    if (j.begin().key() == "Deref")
        return JSONDeref(j.begin().value());
    if (j.begin().key() == "Record")
        return JSONRecord(j.begin().value());
    if (j.begin().key() == "FieldAccess")
        return JSONFieldAccess(j.begin().value());

    throw ParserException();
}


Loc JSONToAST::JSONLoc(json & j)
{
    json js = j["Loc"];
    string line = js["line"];
    string col = js["col"];

    return Loc{stoi(line),stoi(col)};
}


Program * JSONToAST::JSONProgram(json & j)
{
    vector<FunDecl*> funs;
    json js = j["funs"];
    for (auto it = js.begin(); it != js.end(); it++){
        funs.push_back(JSONFunDecl((*it)["FunDecl"]));
    }
    return new Program(funs,JSONLoc(j["loc"]));
}

BinaryOp * JSONToAST::JSONBinaryOp(json & j)
{
    string op = j["operator"].begin().key();
    BinaryOperator * Binop = nullptr;
    if (op == "Plus")
        Binop = new Plus();
    else if (op == "Minus")
        Binop = new Minus();
    else if (op == "Times")
        Binop = new Times();
    else if (op == "Divide")
        Binop = new Divide();
    else if (op == "GreatThan")
        Binop = new GreaterThan();
    else if (op == "Equal")
        Binop = new Equal();

    return new BinaryOp(Binop,convertExpr(j["left"]),convertExpr(j["right"]),JSONLoc(j["loc"]));
}

Null * JSONToAST::JSONNull(json & j)
{
    return new Null(JSONLoc(j["loc"]));
}

Number * JSONToAST::JSONNumber(json & j)
{
    string value = j["value"];
    return new Number(stoi(value),JSONLoc(j["loc"]));
}

Identifier * JSONToAST::JSONIdentifier(json & j)
{
    string name = j["name"];
    return new Identifier(name,JSONLoc(j["loc"]));
}

CallFuncExpr * JSONToAST::JSONCallFuncExpr(json & j)
{
    json js = j["args"];
    vector<Expr*> arg;
    for (auto it = js.begin(); it != js.end(); it++)
        arg.push_back(convertExpr(*it));
    return new CallFuncExpr(convertExpr(j["targetFun"]),arg,JSONLoc(j["loc"]));
}

Input * JSONToAST::JSONInput(json & j)
{
    return new Input(JSONLoc(j["loc"]));
}

Alloc * JSONToAST::JSONAlloc(json & j)
{
    return new Alloc(convertExpr(j["expr"]),JSONLoc(j["loc"]));
}

VarRef * JSONToAST::JSONVarRef(json & j)
{
    json js = j["id"];
    return new VarRef(JSONIdentifier(js["Identifier"]),JSONLoc(j["loc"]));
}

Deref * JSONToAST::JSONDeref(json & j)
{
    return new Deref(convertExpr(j["pointer"]),JSONLoc(j["loc"]));
}

RecordField * JSONToAST::JSONRecordField(json & j)
{
    return new RecordField(j["name"],convertExpr(j["expr"]),JSONLoc(j["loc"]));
}

Record * JSONToAST::JSONRecord(json & j)
{
    json js = j["fields"];
    vector<RecordField*> field;
    for (auto it = js.begin(); it != js.end(); it++)
        field.push_back(JSONRecordField((*it)["RecordField"]));
    return new Record(field,JSONLoc(j["loc"]));
}

FieldAccess * JSONToAST::JSONFieldAccess(json & j)
{
    return new FieldAccess(convertExpr(j["record"]),j["field"],JSONLoc(j["loc"]));
}

AssignStmt * JSONToAST::JSONAssignStmt(json & j)
{
    return new AssignStmt(convertExpr(j["left"]),convertExpr(j["right"]),JSONLoc(j["loc"]));
}

NestedBlockStmt * JSONToAST::JSONNestedBlockStmt(json & j)
{
    json js = j["body"];
    vector<StmtInNestedBlock*> body;
    for (auto it = js.begin(); it != js.end(); it++)
        body.push_back(convertStmtInNestedBlock(*it));
    return new NestedBlockStmt(body,JSONLoc(j["loc"]));
}

IdentifierDecl * JSONToAST::JSONIdentifierDecl(json & j)
{
    return new IdentifierDecl(j["name"],JSONLoc(j["loc"]));
}

VarStmt * JSONToAST::JSONVarStmt(json & j)
{
    json js = j["decls"];
    vector<IdentifierDecl*> decls;
    for (auto it = js.begin(); it != js.end(); it++)
        decls.push_back(JSONIdentifierDecl((*it)["IdentifierDecl"]));
    return new VarStmt(decls,JSONLoc(j["loc"]));
}

ReturnStmt * JSONToAST::JSONReturnStmt(json & j)
{
    return new ReturnStmt(convertExpr(j["expr"]),JSONLoc(j["loc"]));
}

FunBlockStmt * JSONToAST::JSONFunBlockStmt(json & j)
{
    json js = j["vars"];
    vector<VarStmt*> vars;
    for (auto it = js.begin(); it != js.end(); it++)
        vars.push_back(JSONVarStmt((*it)["VarStmt"]));
    json st = j["stmts"];
    vector<StmtInNestedBlock*> stmt;
    for (auto it = st.begin(); it != st.end(); it++)
        stmt.push_back(convertStmtInNestedBlock(*it));

    return new FunBlockStmt(vars,stmt,JSONReturnStmt(j["ret"]["ReturnStmt"]),JSONLoc(j["loc"]));
}

IfStmt * JSONToAST::JSONIfStmt(json & j)
{
    Expr * expr = convertExpr(j["guard"]);
    json jj = j["thenBranch"];
    NestedBlockStmt * stmtThen;
    if (jj.begin().key() != "NestedBlockStmt"){
        vector<StmtInNestedBlock*> body;
        body.push_back(convertStmtInNestedBlock(jj));
        stmtThen = new NestedBlockStmt(body,JSONLoc(jj.begin().value()["loc"]));
    }
    else{
        stmtThen = JSONNestedBlockStmt(j["thenBranch"]["NestedBlockStmt"]);
    }
    json js = j["elseBranch"];
    NestedBlockStmt * stmtElse = nullptr;
    if (js.begin().key() != "None")
    {
        json jj = js["Some"]["value"];
        if (jj.begin().key() != "NestedBlockStmt"){
            vector<StmtInNestedBlock*> body;
            body.push_back(convertStmtInNestedBlock(jj));
            stmtElse = new NestedBlockStmt(body,JSONLoc(jj.begin().value()["loc"]));
        }
        else 
            stmtElse = JSONNestedBlockStmt(js["Some"]["value"]["NestedBlockStmt"]);
    }

    return new IfStmt(expr,stmtThen,stmtElse,JSONLoc(j["loc"]));
}

WhileStmt * JSONToAST::JSONWhileStmt(json & j)
{
    return new WhileStmt(convertExpr(j["guard"]),JSONNestedBlockStmt(j["block"]["NestedBlockStmt"]),JSONLoc(j["loc"]));
}

OutputStmt * JSONToAST::JSONOutputStmt(json & j)
{
    return new OutputStmt(convertExpr(j["expr"]),JSONLoc(j["loc"]));
}

FunDecl * JSONToAST::JSONFunDecl(json & j)
{
    json js = j["params"];

    vector<IdentifierDecl*> params;
    for (auto it = js.begin(); it != js.end(); it++)
        params.push_back(JSONIdentifierDecl((*it)["IdentifierDecl"]));

    return new FunDecl(params,JSONFunBlockStmt(j["block"]["FunBlockStmt"]),j["name"],JSONLoc(j["loc"]));
}
