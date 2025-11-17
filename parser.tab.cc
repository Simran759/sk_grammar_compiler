#include "parser.tab.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> 
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



#if YYDEBUG

# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)        \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();               \
  } while (false)

#else 

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif 

#define yyerrok       (yyerrstatus_ = 0)
#define yyclearin     (yyla.clear ())

#define YYACCEPT      goto yyacceptlab
#define YYABORT       goto yyabortlab
#define YYERROR       goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 4 "parser.ypp"
namespace SK {
#line 118 "parser.tab.cc"

  Parser::Parser (SK::SKScanner &scanner_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value (that.value)
  {}


  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t)
    : Base (t)
    , value ()
  {}

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, YY_RVREF (value_type) v)
    : Base (t)
    , value (YY_MOVE (v))
  {}


  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    value = YY_MOVE (s.value);
  }

  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.value))
  {
#if 201103L <= YY_CPLUSPLUS
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.value))
  {
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    YY_USE (yysym.kind ());
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif 

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    int yylen = 0;

    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    symbol_type yyla;

    int yyresult;

#if YY_EXCEPTIONS
    try
#endif 
      {
    YYCDEBUG << "Starting parse\n";


    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  yybackup:
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif 
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, scanner));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif 
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    if (yyerrstatus_)
      --yyerrstatus_;

    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;


      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif 
        {
          switch (yyn)
            {
  case 2: 
#line 39 "parser.ypp"
                 { (yylhs.value.program) = new SK::Program(std::unique_ptr<SK::StmtList>((yystack_[0].value.stmt_list))); }
#line 575 "parser.tab.cc"
    break;

  case 3: 
#line 43 "parser.ypp"
               { (yylhs.value.stmt_list) = new SK::StmtList(); (yylhs.value.stmt_list)->stmts.emplace_back((yystack_[0].value.stmt)); }
#line 581 "parser.tab.cc"
    break;

  case 4: 
#line 44 "parser.ypp"
                      { (yylhs.value.stmt_list) = (yystack_[0].value.stmt_list); (yylhs.value.stmt_list)->stmts.insert((yystack_[0].value.stmt_list)->stmts.begin(), std::unique_ptr<SK::Stmt>((yystack_[1].value.stmt))); }
#line 587 "parser.tab.cc"
    break;

  case 5: 
#line 48 "parser.ypp"
    { (yylhs.value.stmt) = (yystack_[0].value.stmt); }
#line 593 "parser.tab.cc"
    break;

  case 6: 
#line 49 "parser.ypp"
    { (yylhs.value.stmt) = (yystack_[0].value.stmt); }
#line 599 "parser.tab.cc"
    break;

  case 7: 
#line 50 "parser.ypp"
    { (yylhs.value.stmt) = (yystack_[0].value.stmt); }
#line 605 "parser.tab.cc"
    break;

  case 8: 
#line 51 "parser.ypp"
    { (yylhs.value.stmt) = (yystack_[0].value.stmt); }
#line 611 "parser.tab.cc"
    break;

  case 9: 
#line 55 "parser.ypp"
                                     { (yylhs.value.stmt) = new SK::DeclStmt("num", *(yystack_[3].value.str_val), std::unique_ptr<SK::Expr>((yystack_[1].value.expr))); delete (yystack_[3].value.str_val); }
#line 617 "parser.tab.cc"
    break;

  case 10: 
#line 56 "parser.ypp"
                                      { (yylhs.value.stmt) = new SK::DeclStmt("text", *(yystack_[3].value.str_val), std::unique_ptr<SK::Expr>((yystack_[1].value.expr))); delete (yystack_[3].value.str_val); }
#line 623 "parser.tab.cc"
    break;

  case 11: 
#line 57 "parser.ypp"
                               { (yylhs.value.stmt) = new SK::DeclStmt("num", *(yystack_[1].value.str_val)); delete (yystack_[1].value.str_val); }
#line 629 "parser.tab.cc"
    break;

  case 12: 
#line 58 "parser.ypp"
                                { (yylhs.value.stmt) = new SK::DeclStmt("text", *(yystack_[1].value.str_val)); delete (yystack_[1].value.str_val); }
#line 635 "parser.tab.cc"
    break;

  case 13: 
#line 62 "parser.ypp"
                               { (yylhs.value.stmt) = new SK::AssignStmt(*(yystack_[3].value.str_val), std::unique_ptr<SK::Expr>((yystack_[1].value.expr))); delete (yystack_[3].value.str_val); }
#line 641 "parser.tab.cc"
    break;

  case 14: 
#line 66 "parser.ypp"
                               { auto s = new SK::InputStmt(); s->ids.push_back(*(yystack_[2].value.str_val)); (yylhs.value.stmt) = s; delete (yystack_[2].value.str_val); }
#line 647 "parser.tab.cc"
    break;

  case 15: 
#line 70 "parser.ypp"
                                { auto s = new SK::OutputStmt(); s->exprs.emplace_back((yystack_[2].value.expr)); (yylhs.value.stmt) = s; }
#line 653 "parser.tab.cc"
    break;

  case 16: 
#line 74 "parser.ypp"
             { (yylhs.value.expr) = (yystack_[0].value.expr); }
#line 659 "parser.tab.cc"
    break;

  case 17: 
#line 75 "parser.ypp"
                       { (yylhs.value.expr) = new SK::BinaryExpr(std::unique_ptr<SK::Expr>((yystack_[2].value.expr)), "+", std::unique_ptr<SK::Expr>((yystack_[0].value.expr))); }
#line 665 "parser.tab.cc"
    break;

  case 18: 
#line 76 "parser.ypp"
                       { (yylhs.value.expr) = new SK::BinaryExpr(std::unique_ptr<SK::Expr>((yystack_[2].value.expr)), "-", std::unique_ptr<SK::Expr>((yystack_[0].value.expr))); }
#line 671 "parser.tab.cc"
    break;

  case 19: 
#line 77 "parser.ypp"
                       { (yylhs.value.expr) = new SK::BinaryExpr(std::unique_ptr<SK::Expr>((yystack_[2].value.expr)), "*", std::unique_ptr<SK::Expr>((yystack_[0].value.expr))); }
#line 677 "parser.tab.cc"
    break;

  case 20: 
#line 78 "parser.ypp"
                       { (yylhs.value.expr) = new SK::BinaryExpr(std::unique_ptr<SK::Expr>((yystack_[2].value.expr)), "/", std::unique_ptr<SK::Expr>((yystack_[0].value.expr))); }
#line 683 "parser.tab.cc"
    break;

  case 21: 
#line 82 "parser.ypp"
         { (yylhs.value.expr) = new SK::IdExpr(*(yystack_[0].value.str_val)); delete (yystack_[0].value.str_val); }
#line 689 "parser.tab.cc"
    break;

  case 22: 
#line 83 "parser.ypp"
                { (yylhs.value.expr) = new SK::NumberExpr(std::stod(*(yystack_[0].value.str_val))); delete (yystack_[0].value.str_val); }
#line 695 "parser.tab.cc"
    break;

  case 23: 
#line 84 "parser.ypp"
                { (yylhs.value.expr) = new SK::StringExpr(*(yystack_[0].value.str_val)); delete (yystack_[0].value.str_val); }
#line 701 "parser.tab.cc"
    break;

  case 24: 
#line 85 "parser.ypp"
                     { (yylhs.value.expr) = (yystack_[1].value.expr); }
#line 707 "parser.tab.cc"
    break;


#line 711 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif 
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  yyerrlab:
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        std::string msg = YY_("syntax error");
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    goto yyerrlab1;


  yyerrorlab:
    
    if (false)
      YYERROR;

    
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  yyerrlab1:
    yyerrstatus_ = 3;  
    
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif 
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif 









  const signed char Parser::yypact_ninf_ = -17;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
      4,   -16,   -10,    -2,    -1,    29,   -17,     4,   -17,   -17,
    -17,   -17,    -3,    27,    28,    30,    -3,   -17,   -17,   -17,
    -17,   -17,    -3,     5,    -9,   -14,     1,     6,     7,     8,
    -17,    -3,    -3,    -3,    -3,    -3,   -17,    -3,   -17,    11,
     12,   -17,   -17,   -17,   -17,   -17,    13,    14,   -17,   -17,
    -17,   -17
  };

  const signed char
  Parser::yydefact_[] =
  {
      0,     0,     0,     0,     0,     0,     2,     3,     5,     6,
      7,     8,     0,     0,     0,     0,     0,     1,     4,    21,
     22,    23,     0,     0,    16,     0,     0,     0,     0,     0,
     13,     0,     0,     0,     0,     0,    11,     0,    12,     0,
      0,    24,    17,    18,    19,    20,     0,     0,    14,    15,
      9,    10
  };

  const signed char
  Parser::yypgoto_[] =
  {
    -17,   -17,    35,   -17,   -17,   -17,   -17,   -17,   -13,   -17
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      0,     5,     6,     7,     8,     9,    10,    11,    23,    24
  };

  const signed char
  Parser::yytable_[] =
  {
     19,    20,    21,    28,    13,    14,    12,     1,    35,    29,
      2,     3,     4,    36,    31,    32,    33,    34,    42,    43,
     44,    45,    46,    37,    47,    22,    15,    16,    38,    17,
     25,    26,    30,    27,     0,    39,    40,    41,    48,    49,
     50,    51,    18
  };

  const signed char
  Parser::yycheck_[] =
  {
      3,     4,     5,    16,    14,    15,    22,     3,    22,    22,
      6,     7,     8,    27,    23,    24,    25,    26,    31,    32,
     33,    34,    35,    22,    37,    28,    28,    28,    27,     0,
      3,     3,    27,     3,    -1,    29,    29,    29,    27,    27,
     27,    27,     7
  };

  const signed char
  Parser::yystos_[] =
  {
      0,     3,     6,     7,     8,    31,    32,    33,    34,    35,
     36,    37,    22,    14,    15,    28,    28,     0,    32,     3,
      4,     5,    28,    38,    39,     3,     3,     3,    38,    38,
     27,    23,    24,    25,    26,    22,    27,    22,    27,    29,
     29,    29,    38,    38,    38,    38,    38,    38,    27,    27,
     27,    27
  };

  const signed char
  Parser::yyr1_[] =
  {
      0,    30,    31,    32,    32,    33,    33,    33,    33,    34,
     34,    34,    34,    35,    36,    37,    38,    38,    38,    38,
     38,    39,    39,    39,    39
  };

  const signed char
  Parser::yyr2_[] =
  {
      0,     2,     1,     1,     2,     1,     1,     1,     1,     6,
      6,     4,     4,     4,     5,     5,     1,     3,     3,     3,
      3,     1,     1,     1,     3
  };


#if YYDEBUG
  
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUMBER",
  "STRING", "MAKE", "ASK", "SHOW", "CHECK", "OTHERWISE", "DURING", "DO",
  "GIVE", "NUMTYPE", "TEXTTYPE", "EQ", "NEQ", "GE", "LE", "GT", "LT",
  "ASSIGN", "'+'", "'-'", "'*'", "'/'", "';'", "'('", "')'", "$accept",
  "program", "stmt_list", "stmt", "decl_stmt", "assign_stmt", "input_stmt",
  "output_stmt", "expr", "term", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const signed char
  Parser::yyrline_[] =
  {
      0,    39,    39,    43,    44,    48,    49,    50,    51,    55,
     56,    57,    58,    62,    66,    70,    74,    75,    76,    77,
     78,    82,    83,    84,    85
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
            i = yystack_.begin (),
            i_end = yystack_.end ();
          i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
                << " (line " << yylno << "):\n";
    
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                        yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif 

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    
    static
    const signed char
    translate_table[] =
    {
      0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     28,    29,    25,    23,     2,    24,     2,    26,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,    27,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
      5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     15,    16,    17,    18,    19,    20,    21,    22
    };
    
    const int code_max = 277;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 4 "parser.ypp"
} 
#line 1089 "parser.tab.cc"

#line 88 "parser.ypp"

#include "scanner.hpp"
int SK::Parser::yylex(SK::Parser::semantic_type* yylval, SK::SKScanner& scanner) {
    return scanner.yylex(yylval);
}