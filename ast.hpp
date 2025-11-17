#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace SK {

class CodeGenerator;


class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void generate_code(CodeGenerator& generator) const = 0;
};


class ExprNode : public ASTNode {};

class NumberNode : public ExprNode {
public:
    int value;
    explicit NumberNode(int val) : value(val) {}
    void generate_code(CodeGenerator& generator) const override;
};


class StringNode : public ExprNode {
public:
    std::string value;
    explicit StringNode(std::string val) : value(std::move(val)) {}
    void generate_code(CodeGenerator& generator) const override;
};

class IdNode : public ExprNode {
public:
    std::string name;
    explicit IdNode(std::string n) : name(std::move(n)) {}
    void generate_code(CodeGenerator& generator) const override;
};

class BinOpNode : public ExprNode {
public:
    std::unique_ptr<ExprNode> left;
    int op; 
    std::unique_ptr<ExprNode> right;

    BinOpNode(std::unique_ptr<ExprNode> l, int o, std::unique_ptr<ExprNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    void generate_code(CodeGenerator& generator) const override;
};


class ConditionNode : public ASTNode {
public:
    std::unique_ptr<ExprNode> left;
    int op;
    std::unique_ptr<ExprNode> right;

    ConditionNode(std::unique_ptr<ExprNode> l, int o, std::unique_ptr<ExprNode> r)
        : left(std::move(l)), op(o), right(std::move(r)) {}
    void generate_code(CodeGenerator& generator) const override;
};



class StmtNode : public ASTNode {};


using StmtList = std::vector<std::unique_ptr<StmtNode>>;

class DeclStmtNode : public StmtNode {
public:
    std::string type;
    std::string id_name;
    std::unique_ptr<ExprNode> expr;

    DeclStmtNode(std::string t, std::string id, std::unique_ptr<ExprNode> e)
        : type(std::move(t)), id_name(std::move(id)), expr(std::move(e)) {}
    void generate_code(CodeGenerator& generator) const override;
};

class AssignStmtNode : public StmtNode {
public:
    std::string id_name;
    std::unique_ptr<ExprNode> exprs;

    AssignStmtNode(std::string id, std::unique_ptr<ExprNode> e)
        : id_name(std::move(id)), exprs(std::move(e)) {}
    void generate_code(CodeGenerator& generator) const override;
};

class OutputStmtNode : public StmtNode {
public:
    std::vector<std::unique_ptr<ExprNode>> exprs;
    explicit OutputStmtNode(std::vector<std::unique_ptr<ExprNode>> e)
        : exprs(std::move(e)) {}
    void generate_code(CodeGenerator& generator) const override;
};



class IfStmtNode : public StmtNode {
public:
    std::unique_ptr<ConditionNode> condition;
    StmtList if_block;
    StmtList else_block; 

    IfStmtNode(std::unique_ptr<ConditionNode> cond, StmtList if_b, StmtList else_b)
        : condition(std::move(cond)), if_block(std::move(if_b)), else_block(std::move(else_b)) {}
    void generate_code(CodeGenerator& generator) const override;
};


class WhileStmtNode : public StmtNode {
public:
    std::unique_ptr<ConditionNode> condition;
    StmtList body;

    WhileStmtNode(std::unique_ptr<ConditionNode> cond, StmtList b)
        : condition(std::move(cond)), body(std::move(b)) {}
    void generate_code(CodeGenerator& generator) const override;
};


class ReturnStmtNode : public StmtNode {
public:
    std::unique_ptr<ExprNode> expr;
    explicit ReturnStmtNode(std::unique_ptr<ExprNode> e) : expr(std::move(e)) {}
    void generate_code(CodeGenerator& generator) const override;
};


class FunctionDefNode : public StmtNode {
public:
    std::string name;
    std::vector<std::pair<std::string, std::string>> params; 
    StmtList body;
    FunctionDefNode(const std::string& n,
                    std::vector<std::pair<std::string, std::string>> par,
                    StmtList b)
        : name(n), params(std::move(par)), body(std::move(b)) {}
    void generate_code(CodeGenerator& generator) const override;
};

class CallNode : public ExprNode {
public:
    std::string name;
    std::vector<std::unique_ptr<ExprNode>> args;
    CallNode(const std::string& n, std::vector<std::unique_ptr<ExprNode>> a)
        : name(n), args(std::move(a)) {}
    void generate_code(CodeGenerator& generator) const override;
};


class ProgramNode : public ASTNode {
public:
    StmtList statements;
    std::vector<std::unique_ptr<FunctionDefNode>> functions;
    void generate_code(CodeGenerator& generator) const override;
};


} 

