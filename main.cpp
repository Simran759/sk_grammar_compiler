#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip> 
#include "parser.tab.hpp" 
#include "ast.hpp"
#include "codegen.hpp"


extern FILE* yyin;


extern int yylex(); 
extern SK::Parser::value_type yylval; 
const char* getTokenName(int token) {
    switch(token) {
        case SK::Parser::token::NUMBER: return "NUMBER";
        case SK::Parser::token::ID: return "ID";
        case SK::Parser::token::TYPE: return "TYPE";
        case SK::Parser::token::STRING: return "STRING";
        case SK::Parser::token::MAKE: return "MAKE";
        case SK::Parser::token::SHOW: return "SHOW";
        case SK::Parser::token::CHECK: return "CHECK";
        case SK::Parser::token::OTHERWISE: return "OTHERWISE";
        case SK::Parser::token::DURING: return "DURING";
        case SK::Parser::token::DO: return "DO";
        case SK::Parser::token::USING: return "USING";
        case SK::Parser::token::GIVE: return "GIVE";
        case SK::Parser::token::ASK: return "ASK";
        case SK::Parser::token::LPAREN: return "LPAREN";
        case SK::Parser::token::RPAREN: return "RPAREN";
        case SK::Parser::token::LBRACE: return "LBRACE";
        case SK::Parser::token::RBRACE: return "RBRACE";
        case SK::Parser::token::SEMI: return "SEMI";
        case SK::Parser::token::COMMA: return "COMMA";
        case SK::Parser::token::ASSIGN: return "ASSIGN";
        case SK::Parser::token::PLUS: return "PLUS";
        case SK::Parser::token::MINUS: return "MINUS";
        case SK::Parser::token::MUL: return "MUL";
        case SK::Parser::token::DIV: return "DIV";
        case SK::Parser::token::EQ: return "EQ";
        case SK::Parser::token::NE: return "NE";
        case SK::Parser::token::GT: return "GT";
        case SK::Parser::token::LT: return "LT";
        case SK::Parser::token::GE: return "GE";
        case SK::Parser::token::LE: return "LE";
        case 0: return "EOF";
        default: return "UNKNOWN";
    }
}

const char* getOpName(int op) {
    switch(op) {
        case SK::Parser::token::PLUS: return "+";
        case SK::Parser::token::MINUS: return "-";
        case SK::Parser::token::MUL: return "*";
        case SK::Parser::token::DIV: return "/";
        case SK::Parser::token::EQ: return "==";
        case SK::Parser::token::NE: return "!=";
        case SK::Parser::token::GT: return ">";
        case SK::Parser::token::LT: return "<";
        case SK::Parser::token::GE: return ">=";
        case SK::Parser::token::LE: return "<=";
        default: return "?";
    }
}

void printASTNode(const SK::ASTNode* node, const std::string& prefix);

void printStatementList(const SK::StmtList& statements, const std::string& prefix) {
    for (const auto& stmt : statements) {
        printASTNode(stmt.get(), prefix);
    }
}


void printExpressionList(const std::vector<std::unique_ptr<SK::ExprNode>>& exprs, const std::string& prefix) {
    for (const auto& expr : exprs) {
        printASTNode(expr.get(), prefix);
    }
}


void printASTNode(const SK::ASTNode* node, const std::string& prefix) {
    if (!node) {
        std::cout << prefix << "└── (null)" << std::endl;
        return;
    }

    
    if (auto n = dynamic_cast<const SK::ProgramNode*>(node)) {
        std::cout << prefix << "ProgramNode" << std::endl;
        std::string funcPrefix = prefix + "├── Functions (" + std::to_string(n->functions.size()) + "):";
        std::string stmtPrefix = prefix + "└── Statements (" + std::to_string(n->statements.size()) + "):";

        if (n->functions.empty()) {
            funcPrefix = prefix + "├── Functions (0)";
        }
        std::cout << funcPrefix << std::endl;
        for (const auto& func : n->functions) {
            printASTNode(func.get(), prefix + "│   ");
        }
        
        if (n->statements.empty()) {
            stmtPrefix = prefix + "└── Statements (0)";
        }
        std::cout << stmtPrefix << std::endl;
        printStatementList(n->statements, prefix + "    ");

    } else if (auto n = dynamic_cast<const SK::DeclStmtNode*>(node)) {
        std::cout << prefix << "DeclStmtNode (Type: " << n->type << ", ID: " << n->id_name << ")" << std::endl;
        if (n->expr) {
            std::cout << prefix << "└── Expr:" << std::endl;
            printASTNode(n->expr.get(), prefix + "    ");
        }
    } else if (auto n = dynamic_cast<const SK::AssignStmtNode*>(node)) {
        std::cout << prefix << "AssignStmtNode (ID: " << n->id_name << ")" << std::endl;
        std::cout << prefix << "└── Expr:" << std::endl;
        printASTNode(n->exprs.get(), prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::OutputStmtNode*>(node)) {
        std::cout << prefix << "OutputStmtNode" << std::endl;
        std::cout << prefix << "└── Exprs:" << std::endl;
        printExpressionList(n->exprs, prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::IfStmtNode*>(node)) {
        std::cout << prefix << "IfStmtNode" << std::endl;
        std::cout << prefix << "├── Condition:" << std::endl;
        printASTNode(n->condition.get(), prefix + "│   ");
        std::cout << prefix << "├── If Block:" << std::endl;
        printStatementList(n->if_block, prefix + "│   ");
        if (!n->else_block.empty()) {
            std::cout << prefix << "└── Else Block:" << std::endl;
            printStatementList(n->else_block, prefix + "    ");
        }
    } else if (auto n = dynamic_cast<const SK::WhileStmtNode*>(node)) {
        std::cout << prefix << "WhileStmtNode" << std::endl;
        std::cout << prefix << "├── Condition:" << std::endl;
        printASTNode(n->condition.get(), prefix + "│   ");
        std::cout << prefix << "└── Body:" << std::endl;
        printStatementList(n->body, prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::ReturnStmtNode*>(node)) {
        std::cout << prefix << "ReturnStmtNode" << std::endl;
        std::cout << prefix << "└── Expr:" << std::endl;
        printASTNode(n->expr.get(), prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::FunctionDefNode*>(node)) {
        std::cout << prefix << "FunctionDefNode (Name: " << n->name << ")" << std::endl;
        std::cout << prefix << "├── Params:" << std::endl;
        for(const auto& p : n->params) {
            std::cout << prefix << "│   " << "Param (Type: " << p.first << ", ID: " << p.second << ")" << std::endl;
        }
        std::cout << prefix << "└── Body:" << std::endl;
        printStatementList(n->body, prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::ConditionNode*>(node)) {
        std::cout << prefix << "ConditionNode (Op: " << getOpName(n->op) << ")" << std::endl;
        std::cout << prefix << "├── Left:" << std::endl;
        printASTNode(n->left.get(), prefix + "│   ");
        std::cout << prefix << "└── Right:" << std::endl;
        printASTNode(n->right.get(), prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::BinOpNode*>(node)) {
        std::cout << prefix << "BinOpNode (Op: " << getOpName(n->op) << ")" << std::endl;
        std::cout << prefix << "├── Left:" << std::endl;
        printASTNode(n->left.get(), prefix + "│   ");
        std::cout << prefix << "└── Right:" << std::endl;
        printASTNode(n->right.get(), prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::CallNode*>(node)) {
        std::cout << prefix << "CallNode (Name: " << n->name << ")" << std::endl;
        std::cout << prefix << "└── Args:" << std::endl;
        printExpressionList(n->args, prefix + "    ");
    } else if (auto n = dynamic_cast<const SK::IdNode*>(node)) {
        std::cout << prefix << "IdNode (Name: " << n->name << ")" << std::endl;
    } else if (auto n = dynamic_cast<const SK::NumberNode*>(node)) {
        std::cout << prefix << "NumberNode (Value: " << n->value << ")" << std::endl;
    } else if (auto n = dynamic_cast<const SK::StringNode*>(node)) {
        std::cout << prefix << "StringNode (Value: \"" << n->value << "\")" << std::endl;
    } else {
        std::cout << prefix << "UnknownNode" << std::endl;
    }
}

void printParseTree(const SK::ProgramNode* node) {
    printASTNode(node, "");
}


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file.sk> [-v]\n";
        std::cerr << "  -v : Verbose mode (show lexer tokens and parse tree)\n";
        return 1;
    }

    bool verbose = (argc > 2 && std::string(argv[2]) == "-v");

    std::string input_filename = argv[1];
    if (input_filename.find(".sk") == std::string::npos) {
        std::cerr << "Error: Input file must have a .sk extension.\n";
        return 1;
    }
    
    std::string base_filename = input_filename.substr(0, input_filename.rfind('.'));
    std::string output_filename = base_filename + ".asm";

    if (verbose) {
        std::cout << "\n=== LEXER ANALYSIS ===\n";
        FILE* lex_input = fopen(argv[1], "r");
        if (lex_input) {
            yyin = lex_input; 
            int token;
            int token_count = 0;
            
            std::cout << "Tokens:\n";
           
            while ((token = yylex()) != 0) {
                token_count++;
                std::cout << "  [" << std::setw(3) << token_count << "] " << std::setw(10) << std::left << getTokenName(token);
                
                if (token == SK::Parser::token::NUMBER) {
                    std::cout << " (value: " << yylval.int_val << ")";
                } else if (token == SK::Parser::token::ID || token == SK::Parser::token::TYPE || 
                           token == SK::Parser::token::STRING) {
                    if (yylval.str_val) {
                        std::cout << " (value: \"" << *yylval.str_val << "\")";
                        
                       
                        delete yylval.str_val;
                        yylval.str_val = nullptr; 
                    }
                }
                std::cout << "\n";
            }
            std::cout << "Total tokens (excluding EOF): " << token_count << "\n";
            fclose(lex_input);
        }
    }

   
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        std::cerr << "Error: Could not reopen file for parsing.\n";
        return 1;
    }

   
    SK::ProgramNode* ast_root = nullptr;
    
   
    SK::Parser parser(ast_root);

    std::cout << (verbose ? "\n=== PARSING ===" : "") << "\n";
    std::cout << "1. Parsing..." << std::endl;
    int result = parser.parse();
    fclose(yyin);

    if (result != 0 || ast_root == nullptr) {
        std::cerr << "Compilation failed during parsing.\n";
        return 1;
    }

    if (verbose) {
        std::cout << "\n=== ABSTRACT SYNTAX TREE ===\n";
        printParseTree(ast_root);
        std::cout << "\n";
    }

    std::cout << "2. Generating Assembly Code..." << std::endl;
    try {
        
        SK::CodeGenerator generator(output_filename);
        generator.generate(*ast_root);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error during code generation: " << e.what() << std::endl;
        delete ast_root;
        return 1;
    }

    
    if (verbose) {
        std::cout << "\n=== GENERATED ASSEMBLY (" << output_filename << ") ===\n";
        std::ifstream asm_file(output_filename);
        if (asm_file) {
            std::stringstream buffer;
            buffer << asm_file.rdbuf();
            std::cout << buffer.str() << std::endl;
            asm_file.close();
        } else {
            std::cerr << "Warning: Could not read back assembly file for verbose output.\n";
        }
    }

    delete ast_root;
    
    std::cout << "Compilation successful! Assembly written to " << output_filename << std::endl;
    if (verbose) {
        std::cout << "To assemble and link, run:\n";
        std::cout << "   nasm -f elf64 " << output_filename << " -o " << base_filename << ".o\n";
        std::cout << "   gcc " << base_filename << ".o -o " << base_filename << " -no-pie\n";
        std::cout << "   ./" << base_filename << "\n";
    }
    return 0;
}