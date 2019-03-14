// A Bison parser, made by GNU Bison 3.3.1.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "spl_parser.tab.h"


// Unqualified %code blocks.
#line 48 "spl_parser.y" // lalr1.cc:435

    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>
    #include <vector>
   
    /* include for all driver functions */
    #include "spl_driver.hpp"

#undef yylex
#define yylex scanner.yylex

#line 59 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:435


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
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
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 25 "spl_parser.y" // lalr1.cc:510
namespace SPL {
#line 154 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:510

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  SPL_Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  SPL_Parser::SPL_Parser (SPL_Scanner &scanner_yyarg, SPL_Driver  &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  SPL_Parser::~SPL_Parser ()
  {}

  SPL_Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  SPL_Parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
    , location (std::move (that.location))
  {
    switch (this->type_get ())
    {
      case 46: // CHAR
        value.move< char > (std::move (that.value));
        break;

      case 45: // REAL
        value.move< double > (std::move (that.value));
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.move< int > (std::move (that.value));
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.move< std::string > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  SPL_Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->type_get ())
    {
      case 46: // CHAR
        value.copy< char > (YY_MOVE (that.value));
        break;

      case 45: // REAL
        value.copy< double > (YY_MOVE (that.value));
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  SPL_Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  SPL_Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 46: // CHAR
        value.move< char > (YY_MOVE (s.value));
        break;

      case 45: // REAL
        value.move< double > (YY_MOVE (s.value));
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.move< std::string > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_type.
  SPL_Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  SPL_Parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  SPL_Parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  SPL_Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  void
  SPL_Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  void
  SPL_Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  int
  SPL_Parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }


  // by_state.
  SPL_Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  SPL_Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  SPL_Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  SPL_Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  SPL_Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  SPL_Parser::symbol_number_type
  SPL_Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  SPL_Parser::stack_symbol_type::stack_symbol_type ()
  {}

  SPL_Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 46: // CHAR
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
        break;

      case 45: // REAL
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  SPL_Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.type_get ())
    {
      case 46: // CHAR
        value.move< char > (YY_MOVE (that.value));
        break;

      case 45: // REAL
        value.move< double > (YY_MOVE (that.value));
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  SPL_Parser::stack_symbol_type&
  SPL_Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 46: // CHAR
        value.move< char > (that.value);
        break;

      case 45: // REAL
        value.move< double > (that.value);
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        value.move< int > (that.value);
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  SPL_Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  SPL_Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  SPL_Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  SPL_Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  SPL_Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  SPL_Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  SPL_Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  SPL_Parser::debug_level_type
  SPL_Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  SPL_Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  SPL_Parser::state_type
  SPL_Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  SPL_Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  SPL_Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  SPL_Parser::operator() ()
  {
    return parse ();
  }

  int
  SPL_Parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 46: // CHAR
        yylhs.value.emplace< char > ();
        break;

      case 45: // REAL
        yylhs.value.emplace< double > ();
        break;

      case 39: // SYS_CON
      case 40: // SYS_FUNCT
      case 41: // SYS_PROC
      case 42: // SYS_TYPE
      case 44: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case 47: // STRING
      case 48: // NAME
      case 49: // ID
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 147 "spl_parser.y" // lalr1.cc:919
    {}
#line 811 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 3:
#line 151 "spl_parser.y" // lalr1.cc:919
    {}
#line 817 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 4:
#line 154 "spl_parser.y" // lalr1.cc:919
    {}
#line 823 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 5:
#line 158 "spl_parser.y" // lalr1.cc:919
    {}
#line 829 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 6:
#line 162 "spl_parser.y" // lalr1.cc:919
    {}
#line 835 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 7:
#line 166 "spl_parser.y" // lalr1.cc:919
    {}
#line 841 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 8:
#line 170 "spl_parser.y" // lalr1.cc:919
    {}
#line 847 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 9:
#line 171 "spl_parser.y" // lalr1.cc:919
    {}
#line 853 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 10:
#line 175 "spl_parser.y" // lalr1.cc:919
    {}
#line 859 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 11:
#line 176 "spl_parser.y" // lalr1.cc:919
    {}
#line 865 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 12:
#line 180 "spl_parser.y" // lalr1.cc:919
    {}
#line 871 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 13:
#line 181 "spl_parser.y" // lalr1.cc:919
    {}
#line 877 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 14:
#line 182 "spl_parser.y" // lalr1.cc:919
    {}
#line 883 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 15:
#line 183 "spl_parser.y" // lalr1.cc:919
    {}
#line 889 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 16:
#line 184 "spl_parser.y" // lalr1.cc:919
    {}
#line 895 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 17:
#line 188 "spl_parser.y" // lalr1.cc:919
    {}
#line 901 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 18:
#line 189 "spl_parser.y" // lalr1.cc:919
    {}
#line 907 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 19:
#line 193 "spl_parser.y" // lalr1.cc:919
    {}
#line 913 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 20:
#line 194 "spl_parser.y" // lalr1.cc:919
    {}
#line 919 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 21:
#line 198 "spl_parser.y" // lalr1.cc:919
    {}
#line 925 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 22:
#line 202 "spl_parser.y" // lalr1.cc:919
    {}
#line 931 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 23:
#line 203 "spl_parser.y" // lalr1.cc:919
    {}
#line 937 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 24:
#line 204 "spl_parser.y" // lalr1.cc:919
    {}
#line 943 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 25:
#line 208 "spl_parser.y" // lalr1.cc:919
    {}
#line 949 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 26:
#line 209 "spl_parser.y" // lalr1.cc:919
    {}
#line 955 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 27:
#line 210 "spl_parser.y" // lalr1.cc:919
    {}
#line 961 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 28:
#line 211 "spl_parser.y" // lalr1.cc:919
    {}
#line 967 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 29:
#line 212 "spl_parser.y" // lalr1.cc:919
    {}
#line 973 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 30:
#line 213 "spl_parser.y" // lalr1.cc:919
    {}
#line 979 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 31:
#line 214 "spl_parser.y" // lalr1.cc:919
    {}
#line 985 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 32:
#line 218 "spl_parser.y" // lalr1.cc:919
    {}
#line 991 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 33:
#line 222 "spl_parser.y" // lalr1.cc:919
    {}
#line 997 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 34:
#line 226 "spl_parser.y" // lalr1.cc:919
    {}
#line 1003 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 35:
#line 227 "spl_parser.y" // lalr1.cc:919
    {}
#line 1009 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 36:
#line 231 "spl_parser.y" // lalr1.cc:919
    {}
#line 1015 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 37:
#line 235 "spl_parser.y" // lalr1.cc:919
    {}
#line 1021 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 38:
#line 236 "spl_parser.y" // lalr1.cc:919
    {}
#line 1027 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 39:
#line 240 "spl_parser.y" // lalr1.cc:919
    {}
#line 1033 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 40:
#line 241 "spl_parser.y" // lalr1.cc:919
    {}
#line 1039 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 41:
#line 245 "spl_parser.y" // lalr1.cc:919
    {}
#line 1045 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 42:
#line 246 "spl_parser.y" // lalr1.cc:919
    {}
#line 1051 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 43:
#line 250 "spl_parser.y" // lalr1.cc:919
    {}
#line 1057 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 44:
#line 254 "spl_parser.y" // lalr1.cc:919
    {}
#line 1063 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 45:
#line 255 "spl_parser.y" // lalr1.cc:919
    {}
#line 1069 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 46:
#line 256 "spl_parser.y" // lalr1.cc:919
    {}
#line 1075 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 47:
#line 257 "spl_parser.y" // lalr1.cc:919
    {}
#line 1081 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 48:
#line 258 "spl_parser.y" // lalr1.cc:919
    {}
#line 1087 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 49:
#line 262 "spl_parser.y" // lalr1.cc:919
    {}
#line 1093 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 50:
#line 266 "spl_parser.y" // lalr1.cc:919
    {}
#line 1099 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 51:
#line 270 "spl_parser.y" // lalr1.cc:919
    {}
#line 1105 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 52:
#line 274 "spl_parser.y" // lalr1.cc:919
    {}
#line 1111 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 53:
#line 278 "spl_parser.y" // lalr1.cc:919
    {}
#line 1117 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 54:
#line 279 "spl_parser.y" // lalr1.cc:919
    {}
#line 1123 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 55:
#line 283 "spl_parser.y" // lalr1.cc:919
    {}
#line 1129 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 56:
#line 284 "spl_parser.y" // lalr1.cc:919
    {}
#line 1135 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 57:
#line 288 "spl_parser.y" // lalr1.cc:919
    {}
#line 1141 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 58:
#line 289 "spl_parser.y" // lalr1.cc:919
    {}
#line 1147 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 59:
#line 293 "spl_parser.y" // lalr1.cc:919
    {}
#line 1153 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 60:
#line 297 "spl_parser.y" // lalr1.cc:919
    {}
#line 1159 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 61:
#line 301 "spl_parser.y" // lalr1.cc:919
    {}
#line 1165 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 62:
#line 305 "spl_parser.y" // lalr1.cc:919
    {}
#line 1171 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 63:
#line 309 "spl_parser.y" // lalr1.cc:919
    {}
#line 1177 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 64:
#line 310 "spl_parser.y" // lalr1.cc:919
    {}
#line 1183 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 65:
#line 314 "spl_parser.y" // lalr1.cc:919
    {}
#line 1189 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 66:
#line 315 "spl_parser.y" // lalr1.cc:919
    {}
#line 1195 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 67:
#line 319 "spl_parser.y" // lalr1.cc:919
    {}
#line 1201 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 68:
#line 320 "spl_parser.y" // lalr1.cc:919
    {}
#line 1207 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 69:
#line 321 "spl_parser.y" // lalr1.cc:919
    {}
#line 1213 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 70:
#line 322 "spl_parser.y" // lalr1.cc:919
    {}
#line 1219 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 71:
#line 323 "spl_parser.y" // lalr1.cc:919
    {}
#line 1225 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 72:
#line 324 "spl_parser.y" // lalr1.cc:919
    {}
#line 1231 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 73:
#line 325 "spl_parser.y" // lalr1.cc:919
    {}
#line 1237 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 74:
#line 326 "spl_parser.y" // lalr1.cc:919
    {}
#line 1243 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 75:
#line 327 "spl_parser.y" // lalr1.cc:919
    {}
#line 1249 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 76:
#line 331 "spl_parser.y" // lalr1.cc:919
    {}
#line 1255 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 77:
#line 332 "spl_parser.y" // lalr1.cc:919
    {}
#line 1261 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 78:
#line 333 "spl_parser.y" // lalr1.cc:919
    {}
#line 1267 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 79:
#line 337 "spl_parser.y" // lalr1.cc:919
    {}
#line 1273 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 80:
#line 338 "spl_parser.y" // lalr1.cc:919
    {}
#line 1279 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 81:
#line 339 "spl_parser.y" // lalr1.cc:919
    {}
#line 1285 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 82:
#line 340 "spl_parser.y" // lalr1.cc:919
    {}
#line 1291 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 83:
#line 341 "spl_parser.y" // lalr1.cc:919
    {}
#line 1297 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 84:
#line 345 "spl_parser.y" // lalr1.cc:919
    {}
#line 1303 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 85:
#line 349 "spl_parser.y" // lalr1.cc:919
    {}
#line 1309 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 86:
#line 350 "spl_parser.y" // lalr1.cc:919
    {}
#line 1315 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 87:
#line 354 "spl_parser.y" // lalr1.cc:919
    {}
#line 1321 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 88:
#line 358 "spl_parser.y" // lalr1.cc:919
    {}
#line 1327 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 89:
#line 362 "spl_parser.y" // lalr1.cc:919
    {}
#line 1333 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 90:
#line 366 "spl_parser.y" // lalr1.cc:919
    {}
#line 1339 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 91:
#line 367 "spl_parser.y" // lalr1.cc:919
    {}
#line 1345 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 92:
#line 371 "spl_parser.y" // lalr1.cc:919
    {}
#line 1351 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 93:
#line 375 "spl_parser.y" // lalr1.cc:919
    {}
#line 1357 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 94:
#line 376 "spl_parser.y" // lalr1.cc:919
    {}
#line 1363 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 95:
#line 380 "spl_parser.y" // lalr1.cc:919
    {}
#line 1369 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 96:
#line 381 "spl_parser.y" // lalr1.cc:919
    {}
#line 1375 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 97:
#line 385 "spl_parser.y" // lalr1.cc:919
    {}
#line 1381 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 98:
#line 389 "spl_parser.y" // lalr1.cc:919
    {}
#line 1387 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 99:
#line 390 "spl_parser.y" // lalr1.cc:919
    {}
#line 1393 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 100:
#line 394 "spl_parser.y" // lalr1.cc:919
    {}
#line 1399 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 101:
#line 395 "spl_parser.y" // lalr1.cc:919
    {}
#line 1405 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 102:
#line 396 "spl_parser.y" // lalr1.cc:919
    {}
#line 1411 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 103:
#line 397 "spl_parser.y" // lalr1.cc:919
    {}
#line 1417 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 104:
#line 398 "spl_parser.y" // lalr1.cc:919
    {}
#line 1423 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 105:
#line 399 "spl_parser.y" // lalr1.cc:919
    {}
#line 1429 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 106:
#line 400 "spl_parser.y" // lalr1.cc:919
    {}
#line 1435 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 107:
#line 404 "spl_parser.y" // lalr1.cc:919
    {}
#line 1441 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 108:
#line 405 "spl_parser.y" // lalr1.cc:919
    {}
#line 1447 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 109:
#line 406 "spl_parser.y" // lalr1.cc:919
    {}
#line 1453 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 110:
#line 407 "spl_parser.y" // lalr1.cc:919
    {}
#line 1459 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 111:
#line 411 "spl_parser.y" // lalr1.cc:919
    {}
#line 1465 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 112:
#line 412 "spl_parser.y" // lalr1.cc:919
    {}
#line 1471 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 113:
#line 413 "spl_parser.y" // lalr1.cc:919
    {}
#line 1477 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 114:
#line 414 "spl_parser.y" // lalr1.cc:919
    {}
#line 1483 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 115:
#line 415 "spl_parser.y" // lalr1.cc:919
    {}
#line 1489 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 116:
#line 419 "spl_parser.y" // lalr1.cc:919
    {}
#line 1495 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 117:
#line 420 "spl_parser.y" // lalr1.cc:919
    {}
#line 1501 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 118:
#line 421 "spl_parser.y" // lalr1.cc:919
    {}
#line 1507 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 119:
#line 422 "spl_parser.y" // lalr1.cc:919
    {}
#line 1513 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 120:
#line 423 "spl_parser.y" // lalr1.cc:919
    {}
#line 1519 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 121:
#line 424 "spl_parser.y" // lalr1.cc:919
    {}
#line 1525 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 122:
#line 425 "spl_parser.y" // lalr1.cc:919
    {}
#line 1531 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 123:
#line 426 "spl_parser.y" // lalr1.cc:919
    {}
#line 1537 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 124:
#line 427 "spl_parser.y" // lalr1.cc:919
    {}
#line 1543 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 125:
#line 428 "spl_parser.y" // lalr1.cc:919
    {}
#line 1549 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 126:
#line 432 "spl_parser.y" // lalr1.cc:919
    {}
#line 1555 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;

  case 127:
#line 433 "spl_parser.y" // lalr1.cc:919
    {}
#line 1561 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
    break;


#line 1565 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:919
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
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
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
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  SPL_Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

  // Generate an error message.
  std::string
  SPL_Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short SPL_Parser::yypact_ninf_ = -180;

  const signed char SPL_Parser::yytable_ninf_ = -1;

  const short
  SPL_Parser::yypact_[] =
  {
     -13,   -16,    35,  -180,    -3,  -180,   -42,    51,    67,  -180,
    -180,  -180,  -180,  -180,    61,    81,     6,    68,    78,    79,
      92,    85,  -180,    86,    94,    85,  -180,    85,    80,    83,
      82,     1,  -180,    87,  -180,  -180,  -180,  -180,  -180,  -180,
    -180,  -180,  -180,   163,    90,   104,    79,  -180,    97,   -11,
      85,    85,  -180,   103,  -180,  -180,  -180,  -180,   107,   -29,
      85,  -180,    22,    27,   193,  -180,   162,  -180,   182,   232,
     165,    85,    85,   284,    85,    85,    85,   121,  -180,   109,
     163,   108,  -180,  -180,    19,    97,  -180,   124,   127,   -11,
    -180,   112,  -180,   113,  -180,  -180,    85,    85,    85,   132,
     265,    59,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,   258,    85,   258,   -18,
     285,   126,  -180,   285,   285,   -12,   205,   189,  -180,   129,
     163,   139,    97,  -180,   149,    97,   151,   158,  -180,  -180,
    -180,   174,   108,  -180,   168,   168,  -180,  -180,  -180,  -180,
      31,    32,   239,  -180,  -180,   164,   185,   122,  -180,    27,
      27,    27,    27,    27,    27,   193,   193,   193,  -180,  -180,
    -180,  -180,   188,   230,   285,  -180,  -180,    85,  -180,  -180,
      85,   245,    85,  -180,   187,   140,    -8,  -180,    57,   202,
      36,   163,  -180,  -180,   197,   -19,   191,  -180,   199,    51,
     201,  -180,  -180,  -180,   258,   258,  -180,  -180,  -180,  -180,
      85,   258,  -180,   285,   285,    85,   285,    21,   207,  -180,
    -180,   108,  -180,  -180,  -180,  -180,    97,   208,   -44,  -180,
     211,   213,   140,  -180,  -180,  -180,   214,   236,   176,  -180,
     285,   163,  -180,   280,   244,   208,  -180,   -19,   140,   140,
    -180,  -180,  -180,   258,  -180,   108,  -180,  -180,  -180,  -180,
    -180,  -180
  };

  const unsigned char
  SPL_Parser::yydefact_[] =
  {
       0,     0,     0,     7,     0,     1,     0,     0,     9,     3,
       2,    64,     4,    61,     0,    18,     0,     0,     8,     0,
      40,     0,    62,     0,     0,     0,    64,     0,    81,     0,
       0,    79,    69,     0,    66,    67,    68,    70,    71,    72,
      73,    74,    75,     0,     0,     0,    17,    20,     0,    48,
       0,     0,    16,   118,    12,    13,    14,    15,   116,     0,
       0,   120,     0,   106,   110,   115,     0,    97,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    63,     0,
       0,     0,    19,    38,     0,    39,    42,     0,     0,     6,
      46,     0,    47,     0,   123,   122,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,     0,    65,    76,   127,     0,     0,     0,    11,     0,
       0,     0,     0,    25,    26,     0,     0,     0,    22,    23,
      24,     0,     0,    41,    54,    54,    44,    45,     7,     7,
       0,     0,     0,   125,   121,     0,     0,     0,    94,   100,
     101,   102,   103,   104,   105,   107,   108,   109,   111,   112,
     113,   114,     0,    86,    87,    88,    82,     0,    83,    80,
       0,     0,     0,    10,     0,     0,     0,    35,     0,     0,
       0,     0,    21,    37,     0,     0,     0,    52,     0,     0,
       0,   119,   117,   124,     0,     0,    92,    93,    91,    90,
       0,     0,    84,    98,   126,     0,    78,     0,     0,    33,
      34,     0,    31,    27,    28,    43,     0,    60,     0,    56,
       0,     0,     0,    49,     5,    51,     0,     0,     0,    85,
      77,     0,    29,     0,     0,    59,    53,     0,     0,     0,
      50,    96,    95,     0,    30,     0,    36,    55,    57,    58,
      89,    32
  };

  const short
  SPL_Parser::yypgoto_[] =
  {
    -180,  -180,  -180,  -180,   160,   307,  -180,  -180,  -180,   -43,
    -180,  -180,   266,  -139,  -179,  -180,  -180,  -180,   125,  -124,
    -180,  -180,   228,  -180,   234,  -180,   235,  -180,   181,  -180,
      84,  -180,  -180,   130,    -5,   302,  -104,   257,  -180,  -180,
    -180,  -180,  -180,  -180,  -180,  -180,  -180,  -180,   175,  -180,
    -180,   -20,   180,     2,   -28,    24
  };

  const short
  SPL_Parser::yydefgoto_[] =
  {
      -1,     2,     3,     6,   198,   199,     8,    15,    18,    61,
      20,    46,    47,   137,   138,   139,   140,   186,   187,    84,
      49,    85,    86,    89,    90,    91,    92,    93,   196,   228,
     229,   230,   231,    12,    32,    16,    33,    34,    35,    36,
      37,   212,    38,    39,    40,   210,    41,   157,   158,    42,
     119,   124,    63,    64,    65,   125
  };

  const unsigned short
  SPL_Parser::yytable_[] =
  {
      79,    62,    13,   194,    74,    68,   218,    70,   188,    87,
     219,   190,   173,   246,   175,     1,    88,   226,    11,    21,
     247,    10,    94,    95,    22,    23,   241,    24,    25,    98,
      83,   108,   109,     4,    99,     5,    26,   129,   136,   176,
     100,    83,   177,    27,   121,   179,   101,    28,   180,    29,
      30,   120,   110,   250,   123,    31,   126,    75,   156,    76,
      52,     9,   188,    11,    77,    54,    55,    56,    57,   258,
     259,   227,   102,   103,   104,   105,   106,   107,   152,   141,
     142,    14,   244,   168,   169,   170,   171,   184,   201,   202,
      50,   180,   180,   223,    51,   172,   141,   174,    52,   136,
     236,   237,   245,    54,    55,    56,    57,   239,   155,    17,
     165,   166,   167,   130,   156,    19,   261,   141,   221,   131,
     150,   151,    43,   227,    52,    53,    44,    45,    48,    54,
      55,    56,    57,    58,    59,    66,    71,   132,    67,    72,
     206,    60,   136,    73,    80,   130,    83,    52,   224,   260,
     133,    78,    54,    55,    56,    57,   134,   213,    81,    96,
     214,    52,   216,    97,   135,   115,    54,    55,    56,    57,
     127,   155,   144,   128,   242,   145,   148,   149,   136,    52,
     118,   153,   133,   178,    54,    55,    56,    57,   134,   136,
     238,   253,   182,   183,    13,   240,   135,   185,   254,   111,
     112,   113,    52,   114,   208,   136,   136,    54,    55,    56,
      57,   189,   136,   191,   116,   102,   103,   104,   105,   106,
     107,   209,   192,   193,   195,   204,   102,   103,   104,   105,
     106,   107,   102,   103,   104,   105,   106,   107,   102,   103,
     104,   105,   106,   107,    11,    21,   205,   211,   215,   217,
     222,    23,   232,    24,    25,   102,   103,   104,   105,   106,
     107,   225,    26,   233,   181,   235,   243,   117,   141,    27,
      11,    21,   248,    28,   249,    29,    30,    23,   251,    24,
      25,    31,   159,   160,   161,   162,   163,   164,    26,   102,
     103,   104,   105,   106,   107,    27,    11,    21,   203,    28,
     252,    29,    30,    23,   255,    24,    25,    31,   256,   200,
       7,   220,    82,   143,    26,   102,   103,   104,   105,   106,
     107,    27,   154,   146,   147,    28,   197,    29,    69,   234,
     122,   257,   207,    31,     0,   102,   103,   104,   105,   106,
     107
  };

  const short
  SPL_Parser::yycheck_[] =
  {
      43,    21,     7,   142,     3,    25,   185,    27,   132,    20,
      18,   135,   116,    57,   118,    28,    27,    36,    12,    13,
      64,    63,    50,    51,    18,    19,     5,    21,    22,    58,
      49,     4,     5,    49,    63,     0,    30,    80,    81,    57,
      60,    49,    60,    37,    72,    57,    24,    41,    60,    43,
      44,    71,    25,   232,    74,    49,    76,    56,   101,    58,
      39,    64,   186,    12,    63,    44,    45,    46,    47,   248,
     249,   195,    50,    51,    52,    53,    54,    55,    98,    60,
      61,    14,   221,   111,   112,   113,   114,   130,    57,    57,
       5,    60,    60,    57,     9,   115,    60,   117,    39,   142,
     204,   205,   226,    44,    45,    46,    47,   211,    49,    48,
     108,   109,   110,     5,   157,    34,   255,    60,    61,    11,
      96,    97,    54,   247,    39,    40,    48,    48,    36,    44,
      45,    46,    47,    48,    49,    49,    56,    29,    44,    56,
      18,    56,   185,    61,    54,     5,    49,    39,   191,   253,
      42,    64,    44,    45,    46,    47,    48,   177,    54,    56,
     180,    39,   182,    56,    56,     3,    44,    45,    46,    47,
      49,    49,    48,    64,   217,    48,    64,    64,   221,    39,
      15,    49,    42,    57,    44,    45,    46,    47,    48,   232,
     210,    15,     3,    64,   199,   215,    56,    58,   241,     6,
       7,     8,    39,    10,    16,   248,   249,    44,    45,    46,
      47,    62,   255,    62,    32,    50,    51,    52,    53,    54,
      55,    33,    64,    49,    56,    61,    50,    51,    52,    53,
      54,    55,    50,    51,    52,    53,    54,    55,    50,    51,
      52,    53,    54,    55,    12,    13,    61,    17,     3,    62,
      48,    19,    61,    21,    22,    50,    51,    52,    53,    54,
      55,    64,    30,    64,    59,    64,    59,    35,    60,    37,
      12,    13,    61,    41,    61,    43,    44,    19,    64,    21,
      22,    49,   102,   103,   104,   105,   106,   107,    30,    50,
      51,    52,    53,    54,    55,    37,    12,    13,    59,    41,
      64,    43,    44,    19,    24,    21,    22,    49,    64,   149,
       3,   186,    46,    85,    30,    50,    51,    52,    53,    54,
      55,    37,    57,    89,    89,    41,   145,    43,    26,   199,
      73,   247,   157,    49,    -1,    50,    51,    52,    53,    54,
      55
  };

  const unsigned char
  SPL_Parser::yystos_[] =
  {
       0,    28,    66,    67,    49,     0,    68,    70,    71,    64,
      63,    12,    98,    99,    14,    72,   100,    48,    73,    34,
      75,    13,    18,    19,    21,    22,    30,    37,    41,    43,
      44,    49,    99,   101,   102,   103,   104,   105,   107,   108,
     109,   111,   114,    54,    48,    48,    76,    77,    36,    85,
       5,     9,    39,    40,    44,    45,    46,    47,    48,    49,
      56,    74,   116,   117,   118,   119,    49,    44,   116,   100,
     116,    56,    56,    61,     3,    56,    58,    63,    64,    74,
      54,    54,    77,    49,    84,    86,    87,    20,    27,    88,
      89,    90,    91,    92,   119,   119,    56,    56,    58,    63,
     116,    24,    50,    51,    52,    53,    54,    55,     4,     5,
      25,     6,     7,     8,    10,     3,    32,    35,    15,   115,
     116,   119,   102,   116,   116,   120,   116,    49,    64,    74,
       5,    11,    29,    42,    48,    56,    74,    78,    79,    80,
      81,    60,    61,    87,    48,    48,    89,    91,    64,    64,
     120,   120,   116,    49,    57,    49,    74,   112,   113,   117,
     117,   117,   117,   117,   117,   118,   118,   118,   119,   119,
     119,   119,   116,   101,   116,   101,    57,    60,    57,    57,
      60,    59,     3,    64,    74,    58,    82,    83,    84,    62,
      84,    62,    64,    49,    78,    56,    93,    93,    69,    70,
      69,    57,    57,    59,    61,    61,    18,   113,    16,    33,
     110,    17,   106,   116,   116,     3,   116,    62,    79,    18,
      83,    61,    48,    57,    74,    64,    36,    84,    94,    95,
      96,    97,    61,    64,    98,    64,   101,   101,   116,   101,
     116,     5,    74,    59,    78,    84,    57,    64,    61,    61,
      79,    64,    64,    15,    74,    24,    64,    95,    79,    79,
     101,    78
  };

  const unsigned char
  SPL_Parser::yyr1_[] =
  {
       0,    65,    66,    67,    68,    69,    70,    71,    72,    72,
      73,    73,    74,    74,    74,    74,    74,    75,    75,    76,
      76,    77,    78,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    80,    81,    82,    82,    83,    84,    84,    85,
      85,    86,    86,    87,    88,    88,    88,    88,    88,    89,
      90,    91,    92,    93,    93,    94,    94,    95,    95,    96,
      97,    98,    99,   100,   100,   101,   101,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   103,   103,   103,   104,
     104,   104,   104,   104,   105,   106,   106,   107,   108,   109,
     110,   110,   111,   112,   112,   113,   113,   114,   115,   115,
     116,   116,   116,   116,   116,   116,   116,   117,   117,   117,
     117,   118,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   119,   119,   119,   120,   120
  };

  const unsigned char
  SPL_Parser::yyr2_[] =
  {
       0,     2,     3,     3,     2,     2,     5,     0,     2,     0,
       5,     4,     1,     1,     1,     1,     1,     2,     0,     2,
       1,     4,     1,     1,     1,     1,     1,     3,     3,     4,
       5,     3,     6,     3,     2,     1,     4,     3,     1,     2,
       0,     2,     1,     4,     2,     2,     1,     1,     0,     4,
       5,     4,     3,     3,     0,     3,     1,     3,     3,     2,
       1,     1,     3,     3,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     6,     5,     1,
       4,     1,     4,     4,     5,     2,     0,     4,     4,     8,
       1,     1,     5,     2,     1,     4,     4,     2,     3,     1,
       3,     3,     3,     3,     3,     3,     1,     3,     3,     3,
       1,     3,     3,     3,     3,     1,     1,     4,     1,     4,
       1,     3,     2,     2,     4,     3,     3,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const SPL_Parser::yytname_[] =
  {
  "$end", "error", "$undefined", "ASSIGN", "PLUS", "MINUS", "MUL", "DIV",
  "MOD", "NOT", "AND", "ARRAY", "BEGIN", "CASE", "CONST", "DO", "DOWNTO",
  "ELSE", "END", "FOR", "FUNCTION", "GOTO", "IF", "IN", "OF", "OR",
  "PACKED", "PROCEDURE", "PROGRAM", "RECORD", "REPEAT", "SET", "THEN",
  "TO", "TYPE", "UNTIL", "VAR", "WHILE", "WITH", "SYS_CON", "SYS_FUNCT",
  "SYS_PROC", "SYS_TYPE", "READ", "INTEGER", "REAL", "CHAR", "STRING",
  "NAME", "ID", "GE", "GT", "LE", "LT", "EQUAL", "UNEQUAL", "LP", "RP",
  "LB", "RB", "COMMA", "COLON", "DOTDOT", "DOT", "SEMI", "$accept",
  "program", "program_head", "routine", "sub_routine", "routine_head",
  "label_part", "const_part", "const_expr_list", "const_value",
  "type_part", "type_decl_list", "type_definition", "type_decl",
  "simple_type_decl", "array_type_decl", "record_type_decl",
  "field_decl_list", "field_decl", "name_list", "var_part",
  "var_decl_list", "var_decl", "routine_part", "function_decl",
  "function_head", "procedure_decl", "procedure_head", "parameters",
  "para_decl_list", "para_type_list", "var_para_list", "val_para_list",
  "routine_body", "compound_stmt", "stmt_list", "stmt", "non_label_stmt",
  "assign_stmt", "proc_stmt", "if_stmt", "else_clause", "repeat_stmt",
  "while_stmt", "for_stmt", "direction", "case_stmt", "case_expr_list",
  "case_expr", "goto_stmt", "expression_list", "expression", "expr",
  "term", "factor", "args_list", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short
  SPL_Parser::yyrline_[] =
  {
       0,   147,   147,   151,   154,   158,   162,   166,   170,   171,
     175,   176,   180,   181,   182,   183,   184,   188,   189,   193,
     194,   198,   202,   203,   204,   208,   209,   210,   211,   212,
     213,   214,   218,   222,   226,   227,   231,   235,   236,   240,
     241,   245,   246,   250,   254,   255,   256,   257,   258,   262,
     266,   270,   274,   278,   279,   283,   284,   288,   289,   293,
     297,   301,   305,   309,   310,   314,   315,   319,   320,   321,
     322,   323,   324,   325,   326,   327,   331,   332,   333,   337,
     338,   339,   340,   341,   345,   349,   350,   354,   358,   362,
     366,   367,   371,   375,   376,   380,   381,   385,   389,   390,
     394,   395,   396,   397,   398,   399,   400,   404,   405,   406,
     407,   411,   412,   413,   414,   415,   419,   420,   421,   422,
     423,   424,   425,   426,   427,   428,   432,   433
  };

  // Print the state stack on the debug stream.
  void
  SPL_Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  SPL_Parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  SPL_Parser::token_number_type
  SPL_Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64
    };
    const unsigned user_token_number_max_ = 319;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 25 "spl_parser.y" // lalr1.cc:1242
} // SPL
#line 2206 "C:/Users/DELL/Desktop/course/compiler/SPLCompiler/cmake-build-debug/spl_parser.tab.c" // lalr1.cc:1242
#line 437 "spl_parser.y" // lalr1.cc:1243


// TODO: error detection

