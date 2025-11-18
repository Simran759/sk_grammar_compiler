

#pragma once
#include <fstream>
#include <string>
#include <map>
#include "ast.hpp"

namespace SK {
class CodeGenerator {
private:
    std::ofstream out_file;
    int label_count = 0;
    int string_literal_count = 0;
    int stack_index = 0;
struct VarInfo {
    int offset;
    std::string type;
};
std::map<std::string, VarInfo> symboltable;
std::map<std::string, std::string> text_values;
std::string new_label();

public:
    std::string getExprType(const ExprNode* expr) const ;
       
    std::string storeConcatString(const std::string& left, const std::string& right);
    std::string getStringValue(const ExprNode* expr);
    explicit CodeGenerator(const std::string& filename);
    void generate(const ProgramNode& root);

 
    void visit(const NumberNode* node);
    void visit(const IdNode* node);
    void visit(const BinOpNode* node);
    void visit(const DeclStmtNode* node);
    void visit(const AssignStmtNode* node);
    void visit(const OutputStmtNode* node);
    void visit(const SK::StringNode* node);  
    void visit(const SK::ConditionNode* node);
    void visit(const SK::IfStmtNode* node);      
    void visit(const SK::WhileStmtNode* node);

};
} 