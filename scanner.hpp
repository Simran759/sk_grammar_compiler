#ifndef SCANNER_HPP
#define SCANNER_HPP

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

namespace SK {
class Parser;
}

namespace SK {
class SKScanner : public yyFlexLexer {
public:
    SKScanner(std::istream* in) : yyFlexLexer(in) {}
    virtual ~SKScanner() = default;
    

    int yylex(void* yylval); 
};

} 

#endif
