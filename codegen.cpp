#include "codegen.hpp"
#include "parser.tab.hpp" 
#include <stdexcept>


using namespace SK;
class DeclStmtNode : public StmtNode {
public:
    std::string type;
    std::string id_name;
    std::unique_ptr<ExprNode> expr;

    DeclStmtNode(std::string t, std::string id, std::unique_ptr<ExprNode> e)
        : type(std::move(t)), id_name(std::move(id)), expr(std::move(e)) {}

    void generate_code(CodeGenerator& generator) const override;
};
std::string CodeGenerator::storeConcatString(const std::string& left, const std::string& right) {
    std::string new_label = "S" + std::to_string(string_literal_count++);
   
    out_file << "section .data\n";
    out_file << new_label << " db \"" << left << right << "\", 0\n";
    out_file << "section .text\n";
    return new_label;
}

std::string CodeGenerator::getExprType(const ExprNode* expr) const {
    if (auto numNode = dynamic_cast<const NumberNode*>(expr)) return "num";
    if (auto strNode = dynamic_cast<const StringNode*>(expr)) return "text";
    if (auto idNode = dynamic_cast<const IdNode*>(expr)) {
        if (symboltable.count(idNode->name)) return symboltable.at(idNode->name).type;
    }
    if (auto binOpNode = dynamic_cast<const BinOpNode*>(expr)) {
        std::string left = getExprType(binOpNode->left.get());
        std::string right = getExprType(binOpNode->right.get());
        if (binOpNode->op == SK::Parser::token::PLUS) {
            if (left == "text" && right == "text") return "text";
            if (left == "num" && right == "num") return "num";
            return "unknown";
        }
        
        if (left == "text" || right == "text") {
            
            return "invalid_text_op";
        }
        if ((binOpNode->op == SK::Parser::token::MINUS ||
             binOpNode->op == SK::Parser::token::MUL ||
             binOpNode->op == SK::Parser::token::DIV) && left=="num" && right=="num")
            return "num";
        return "unknown";
    }
    if (auto callNode = dynamic_cast<const CallNode*>(expr)) {
        
        auto it = functionTypeTable.find(callNode->name);
        if (it != functionTypeTable.end()) {
            return it->second; 
        }
        return "unknown"; 
    }
    return "unknown";
}



SK::CodeGenerator::CodeGenerator(const std::string& filename) {
    out_file.open(filename);
    if (!out_file.is_open()) {
        throw std::runtime_error("Could not open output file.");
    }
}


std::string CodeGenerator::new_label() {
    return "L" + std::to_string(label_count++);
}
std::string CodeGenerator::getStringValue(const ExprNode* expr) {
    if (auto strNode = dynamic_cast<const SK::StringNode*>(expr))
        return strNode->value;
    if (auto idNode = dynamic_cast<const SK::IdNode*>(expr)) {
        if (text_values.count(idNode->name)) return text_values.at(idNode->name);
        return "";
    }
    if (auto binOpNode = dynamic_cast<const SK::BinOpNode*>(expr)) {
        if (binOpNode->op == SK::Parser::token::PLUS) {
            return getStringValue(binOpNode->left.get()) + getStringValue(binOpNode->right.get());
        }
    }
    return "";
}

void CodeGenerator::generate(const SK::ProgramNode& root) {
   
    out_file << "section .data\n";
    out_file << "    format_num db \"%d\", 10, 0\n";
    out_file << "    format_str db \"%s\", 10, 0\n\n"; 
    out_file << "section .text\n";
    out_file << "global main\n";
    out_file << "extern printf\n\n";
    out_file << "main:\n";
    out_file << "    push rbp\n";
    out_file << "    mov rbp, rsp\n";
    out_file << "    sub rsp, 64 ; Allocate stack space for variables\n\n";

    
    root.generate_code(*this);

    
    out_file << "\n.exit_program:\n";
    out_file << "    mov rax, 60     ; syscall for exit\n";
    out_file << "    mov rdi, 0      ; exit code 0\n";
    out_file << "    syscall\n";
}


void SK::NumberNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::StringNode::generate_code(CodeGenerator& gen) const { gen.visit(this); } 
void SK::IdNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::BinOpNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::ConditionNode::generate_code(CodeGenerator& gen) const { gen.visit(this); } 
void SK::DeclStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::AssignStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::OutputStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::IfStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); }
void SK::WhileStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); } 
void SK::ReturnStmtNode::generate_code(CodeGenerator& gen) const { gen.visit(this); } 



void CodeGenerator::visit(const SK::NumberNode* node) {
    out_file << "    mov rax, " << node->value << "\n";
}

void CodeGenerator::visit(const SK::StringNode* node) {
    std::string str_label = "S" + std::to_string(string_literal_count++);
    
    out_file << "section .data\n";
    out_file << str_label << " db \"" << node->value << "\", 0\n";
   
    out_file << "section .text\n";
    out_file << "    mov rax, " << str_label << "\n";
}

void CodeGenerator::visit(const SK::IdNode* node) {
    if (symboltable.find(node->name) == symboltable.end()) {
        throw std::runtime_error("Semantic Error: Use of undeclared variable '" + node->name + "'");
    }
    int offset = symboltable.at(node->name).offset; 
    out_file << "    mov rax, [rbp - " << offset << "]\n";
}



void CodeGenerator::visit(const SK::BinOpNode* node) {
    std::string leftType = getExprType(node->left.get());
    std::string rightType = getExprType(node->right.get());

    if (node->op == SK::Parser::token::PLUS) {
        
        if (leftType == "num" && rightType == "num") {
            node->right->generate_code(*this);
            out_file << "    push rax\n";
            node->left->generate_code(*this);
            out_file << "    pop rbx\n";
            out_file << "    add rax, rbx\n";
            return;
        }
       
        if (leftType == "text" && rightType == "text") {
    std::string concat = getStringValue(node);
    std::string label = storeConcatString(concat, ""); 
    out_file << "    mov rax, " << label << "\n";
    return;
}

        throw std::runtime_error("Semantic Error: Cannot add string and number.");
    }



    
    if (node->op == SK::Parser::token::MINUS ||
        node->op == SK::Parser::token::MUL ||
        node->op == SK::Parser::token::DIV) {
        if (leftType == "num" && rightType == "num") {
            node->right->generate_code(*this);
            out_file << "    push rax\n";
            node->left->generate_code(*this);
            out_file << "    pop rbx\n";
            switch (node->op) {
                case SK::Parser::token::MINUS: out_file << "    sub rax, rbx\n"; break;
                case SK::Parser::token::MUL: out_file << "    imul rax, rbx\n"; break;
                case SK::Parser::token::DIV:
                    out_file << "    cqo\n";
                    out_file << "    idiv rbx\n";
                    break;
            }
            return;
        }
        throw std::runtime_error("Semantic Error: Operation only allowed for num types.");
    }

    
    throw std::runtime_error("Semantic Error: Unsupported binary operation.");
}


void CodeGenerator::visit(const SK::DeclStmtNode* node) {
    if (symboltable.count(node->id_name))
        throw std::runtime_error("Redefinition...");
    
    stackindex += 8;
    symboltable[node->id_name] = VarInfo{stackindex, node->type};

    if (node->expr) {
        std::string exprType = getExprType(node->expr.get());
        if (exprType == "invalid_text_op") {
    throw std::runtime_error("Operator only '+' (concatenation) is allowed for text. You used an invalid operator.");
}
if (node->type != exprType) {
    throw std::runtime_error("Type Error: Cannot initialize " + node->type + " with " + exprType);
}

        
        if (node->type == "text") {
            text_values[node->id_name] = getStringValue(node->expr.get());
        }
        node->expr->generate_code(*this);
        out_file << "    mov [rbp - " << stackindex << "], rax ; Declare " << node->id_name << "\n";
    }
}


void CodeGenerator::visit(const SK::AssignStmtNode* node) {
    if (!symboltable.count(node->id_name)) {
        throw std::runtime_error("Semantic Error: Assignment to undeclared variable '" + node->id_name + "'");
    }
    std::string varType = symboltable.at(node->id_name).type;
    std::string exprType = getExprType(node->exprs.get());
    if (exprType == "invalid_text_op") {
        throw std::runtime_error("Operator only '+' (concatenation) is allowed for text in assignment.");
    }
    if (varType != exprType) {
        throw std::runtime_error("Type Error: Cannot assign " + exprType + " to " + varType);
    }
    node->exprs->generate_code(*this);
    int offset = symboltable.at(node->id_name).offset; 
    out_file << "    mov [rbp - " << offset << "], rax ; Assign to " << node->id_name << "\n";
}


void CodeGenerator::visit(const SK::OutputStmtNode* node) {
    for (const auto& expr : node->exprs) {
        std::string exprType = getExprType(expr.get());
        if (exprType == "invalid_text_op") {
            throw std::runtime_error("Operator only '+' (concatenation) is allowed for text in show.");
        }
        
        const SK::IdNode* idNode = dynamic_cast<const SK::IdNode*>(expr.get());
        bool isString = false;
        if (idNode && symboltable.count(idNode->name)) {
            isString = (symboltable.at(idNode->name).type == "text");
        } else {
            isString = dynamic_cast<const SK::StringNode*>(expr.get()) != nullptr;
        }
        expr->generate_code(*this);
        if (isString)
            out_file << "    mov rdi, format_str\n";
        else
            out_file << "    mov rdi, format_num\n";
        out_file << "    mov rsi, rax\n    mov rax, 0\n    call printf\n";
    }
}



void CodeGenerator::visit(const SK::ConditionNode* node) {
    std::string leftType = getExprType(node->left.get());
    std::string rightType = getExprType(node->right.get());
    if (leftType != "num" || rightType != "num") {
        throw std::runtime_error("Condition Error: Comparisons allowed only between num types.");
    }
    node->right->generate_code(*this);
    out_file << "    push rax\n";
    node->left->generate_code(*this);
    out_file << "    pop rbx\n";
    out_file << "    cmp rax, rbx\n";
}

void CodeGenerator::visit(const SK::IfStmtNode* node) {
    
    std::string leftType = getExprType(node->condition->left.get());
    std::string rightType = getExprType(node->condition->right.get());
    if (leftType != "num" || rightType != "num") {
        throw std::runtime_error("If Statement Condition Error: Only num types are allowed in 'check' conditions.");
    }

    std::string else_label = new_label();
    std::string end_if_label = new_label();
    node->condition->generate_code(*this);

    std::string jump_instruction;
    switch (node->condition->op) {
        case SK::Parser::token::EQ: jump_instruction = "jne"; break;
        case SK::Parser::token::NE: jump_instruction = "je"; break;
        case SK::Parser::token::LT: jump_instruction = "jge"; break;
        case SK::Parser::token::LE: jump_instruction = "jg"; break;
        case SK::Parser::token::GT: jump_instruction = "jle"; break;
        case SK::Parser::token::GE: jump_instruction = "jl"; break;
        default: throw std::runtime_error("Unsupported condition operator in 'check'.");
    }

    out_file << "    " << jump_instruction << " " << (node->else_block.empty() ? end_if_label : else_label) << "\n";
    for (const auto& stmt : node->if_block) { stmt->generate_code(*this); }
    if (!node->else_block.empty()) {
        out_file << "    jmp " << end_if_label << "\n";
        out_file << else_label << ":\n";
        for (const auto& stmt : node->else_block) { stmt->generate_code(*this); }
    }
    out_file << end_if_label << ":\n";
}


void CodeGenerator::visit(const SK::WhileStmtNode* node) {
    
    std::string leftType = getExprType(node->condition->left.get());
    std::string rightType = getExprType(node->condition->right.get());
    if (leftType != "num" || rightType != "num") {
        throw std::runtime_error("During Loop Condition Error: Only num types are allowed in 'during' conditions.");
    }

    std::string loop_start_label = new_label();
    std::string loop_end_label = new_label();
    
    out_file << loop_start_label << ":\n";
    node->condition->generate_code(*this);

    
    std::string jump_instruction;
    switch (node->condition->op) {
        case SK::Parser::token::EQ: jump_instruction = "jne"; break;
        case SK::Parser::token::NE: jump_instruction = "je"; break;
        case SK::Parser::token::LT: jump_instruction = "jge"; break;
        case SK::Parser::token::LE: jump_instruction = "jg"; break;
        case SK::Parser::token::GT: jump_instruction = "jle"; break;
        case SK::Parser::token::GE: jump_instruction = "jl"; break;
        default: throw std::runtime_error("Unsupported condition operator in 'during'.");
    }

    out_file << "    " << jump_instruction << " " << loop_end_label << "\n";
    for (const auto& stmt : node->body) { stmt->generate_code(*this); }
    out_file << "    jmp " << loop_start_label << "\n";
    out_file << loop_end_label << ":\n";
}



void CodeGenerator::visit(const SK::ReturnStmtNode* node) {
    node->expr->generate_code(*this);
   
    out_file << "    ret\n";
}

void ProgramNode::generate_code(CodeGenerator& gen) const {
    for (const auto& func : functions) func->generate_code(gen);
    for (const auto& stmt : statements) stmt->generate_code(gen);
}

void FunctionDefNode::generate_code(CodeGenerator& gen) const {
    gen.visit(this);
}

void CodeGenerator::visit(const FunctionDefNode* node) {
    
    out_file << node->name << ":\n";
    out_file << "    push rbp\n";
    out_file << "    mov rbp, rsp\n";
   
    std::map<std::string, VarInfo> local_symboltable;
    int param_offset = 8;
    for (const auto& param : node->params) {
        local_symboltable[param.second] = VarInfo{param_offset, param.first};
        param_offset += 8;
    }
    auto saved_symboltable = symboltable;
    symboltable = local_symboltable;

    
    functionTypeTable[node->name] = "num";

    
    for (const auto& stmt : node->body) stmt->generate_code(*this);

    symboltable = saved_symboltable;
    out_file << "    pop rbp\n";
    out_file << "    ret\n";
}


void CallNode::generate_code(CodeGenerator& gen) const {
    gen.visit(this);
}


void CodeGenerator::visit(const CallNode* node) {
    
    if (functionTypeTable.find(node->name) == functionTypeTable.end()) {
        throw std::runtime_error("Semantic Error: Function '" + node->name + "' is called but not defined.");
    }

    
    node->args[0]->generate_code(*this);
    out_file << "    mov rdi, rax\n";
    node->args[1]->generate_code(*this);
    out_file << "    mov rsi, rax\n";
    out_file << "    call " << node->name << "\n";
   
}
