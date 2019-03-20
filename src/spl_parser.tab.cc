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





#include "spl_parser.tab.hh"


// Unqualified %code blocks.
#line 51 "spl_parser.y" // lalr1.cc:435

    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>
    #include <vector>
   
    /* include for all driver functions */
    #include "spl_driver.hpp"

#undef yylex
#define yylex scanner.yylex

#line 59 "spl_parser.tab.cc" // lalr1.cc:435


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

#line 28 "spl_parser.y" // lalr1.cc:510
namespace SPL {
#line 154 "spl_parser.tab.cc" // lalr1.cc:510

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
      case 52: // CHAR
        value.move< char > (std::move (that.value));
        break;

      case 51: // REAL
        value.move< double > (std::move (that.value));
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.move< int > (std::move (that.value));
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        value.copy< char > (YY_MOVE (that.value));
        break;

      case 51: // REAL
        value.copy< double > (YY_MOVE (that.value));
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        value.move< char > (YY_MOVE (s.value));
        break;

      case 51: // REAL
        value.move< double > (YY_MOVE (s.value));
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.move< int > (YY_MOVE (s.value));
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        value.YY_MOVE_OR_COPY< char > (YY_MOVE (that.value));
        break;

      case 51: // REAL
        value.YY_MOVE_OR_COPY< double > (YY_MOVE (that.value));
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        value.move< char > (YY_MOVE (that.value));
        break;

      case 51: // REAL
        value.move< double > (YY_MOVE (that.value));
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.move< int > (YY_MOVE (that.value));
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        value.move< char > (that.value);
        break;

      case 51: // REAL
        value.move< double > (that.value);
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        value.move< int > (that.value);
        break;

      case 53: // STRING
      case 54: // ID
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
      case 52: // CHAR
        yylhs.value.emplace< char > ();
        break;

      case 51: // REAL
        yylhs.value.emplace< double > ();
        break;

      case 45: // SYS_CON
      case 46: // SYS_FUNCT
      case 48: // SYS_PROC
      case 49: // SYS_TYPE
      case 50: // INTEGER
        yylhs.value.emplace< int > ();
        break;

      case 53: // STRING
      case 54: // ID
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
#line 150 "spl_parser.y" // lalr1.cc:919
    {}
#line 804 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 3:
#line 154 "spl_parser.y" // lalr1.cc:919
    {}
#line 810 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 4:
#line 157 "spl_parser.y" // lalr1.cc:919
    {}
#line 816 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 5:
#line 161 "spl_parser.y" // lalr1.cc:919
    {}
#line 822 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 6:
#line 165 "spl_parser.y" // lalr1.cc:919
    {}
#line 828 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 7:
#line 169 "spl_parser.y" // lalr1.cc:919
    {}
#line 834 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 8:
#line 173 "spl_parser.y" // lalr1.cc:919
    {}
#line 840 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 9:
#line 174 "spl_parser.y" // lalr1.cc:919
    {}
#line 846 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 10:
#line 178 "spl_parser.y" // lalr1.cc:919
    {}
#line 852 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 11:
#line 179 "spl_parser.y" // lalr1.cc:919
    {}
#line 858 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 12:
#line 183 "spl_parser.y" // lalr1.cc:919
    {}
#line 864 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 13:
#line 184 "spl_parser.y" // lalr1.cc:919
    {}
#line 870 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 14:
#line 185 "spl_parser.y" // lalr1.cc:919
    {}
#line 876 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 15:
#line 186 "spl_parser.y" // lalr1.cc:919
    {}
#line 882 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 16:
#line 187 "spl_parser.y" // lalr1.cc:919
    {}
#line 888 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 17:
#line 191 "spl_parser.y" // lalr1.cc:919
    {}
#line 894 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 18:
#line 192 "spl_parser.y" // lalr1.cc:919
    {}
#line 900 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 19:
#line 196 "spl_parser.y" // lalr1.cc:919
    {}
#line 906 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 20:
#line 197 "spl_parser.y" // lalr1.cc:919
    {}
#line 912 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 21:
#line 201 "spl_parser.y" // lalr1.cc:919
    {}
#line 918 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 22:
#line 205 "spl_parser.y" // lalr1.cc:919
    {}
#line 924 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 23:
#line 206 "spl_parser.y" // lalr1.cc:919
    {}
#line 930 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 24:
#line 207 "spl_parser.y" // lalr1.cc:919
    {}
#line 936 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 25:
#line 211 "spl_parser.y" // lalr1.cc:919
    {}
#line 942 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 26:
#line 212 "spl_parser.y" // lalr1.cc:919
    {}
#line 948 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 27:
#line 213 "spl_parser.y" // lalr1.cc:919
    {}
#line 954 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 28:
#line 214 "spl_parser.y" // lalr1.cc:919
    {}
#line 960 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 29:
#line 215 "spl_parser.y" // lalr1.cc:919
    {}
#line 966 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 30:
#line 216 "spl_parser.y" // lalr1.cc:919
    {}
#line 972 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 31:
#line 217 "spl_parser.y" // lalr1.cc:919
    {}
#line 978 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 32:
#line 221 "spl_parser.y" // lalr1.cc:919
    {}
#line 984 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 33:
#line 225 "spl_parser.y" // lalr1.cc:919
    {}
#line 990 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 34:
#line 229 "spl_parser.y" // lalr1.cc:919
    {}
#line 996 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 35:
#line 230 "spl_parser.y" // lalr1.cc:919
    {}
#line 1002 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 36:
#line 234 "spl_parser.y" // lalr1.cc:919
    {}
#line 1008 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 37:
#line 238 "spl_parser.y" // lalr1.cc:919
    {}
#line 1014 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 38:
#line 239 "spl_parser.y" // lalr1.cc:919
    {}
#line 1020 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 39:
#line 243 "spl_parser.y" // lalr1.cc:919
    {}
#line 1026 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 40:
#line 244 "spl_parser.y" // lalr1.cc:919
    {}
#line 1032 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 41:
#line 248 "spl_parser.y" // lalr1.cc:919
    {}
#line 1038 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 42:
#line 249 "spl_parser.y" // lalr1.cc:919
    {}
#line 1044 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 43:
#line 253 "spl_parser.y" // lalr1.cc:919
    {}
#line 1050 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 44:
#line 257 "spl_parser.y" // lalr1.cc:919
    {}
#line 1056 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 45:
#line 258 "spl_parser.y" // lalr1.cc:919
    {}
#line 1062 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 46:
#line 259 "spl_parser.y" // lalr1.cc:919
    {}
#line 1068 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 47:
#line 260 "spl_parser.y" // lalr1.cc:919
    {}
#line 1074 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 48:
#line 261 "spl_parser.y" // lalr1.cc:919
    {}
#line 1080 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 49:
#line 265 "spl_parser.y" // lalr1.cc:919
    {}
#line 1086 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 50:
#line 269 "spl_parser.y" // lalr1.cc:919
    {}
#line 1092 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 51:
#line 273 "spl_parser.y" // lalr1.cc:919
    {}
#line 1098 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 52:
#line 277 "spl_parser.y" // lalr1.cc:919
    {}
#line 1104 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 53:
#line 281 "spl_parser.y" // lalr1.cc:919
    {}
#line 1110 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 54:
#line 282 "spl_parser.y" // lalr1.cc:919
    {}
#line 1116 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 55:
#line 286 "spl_parser.y" // lalr1.cc:919
    {}
#line 1122 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 56:
#line 287 "spl_parser.y" // lalr1.cc:919
    {}
#line 1128 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 57:
#line 291 "spl_parser.y" // lalr1.cc:919
    {}
#line 1134 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 58:
#line 292 "spl_parser.y" // lalr1.cc:919
    {}
#line 1140 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 59:
#line 296 "spl_parser.y" // lalr1.cc:919
    {}
#line 1146 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 60:
#line 300 "spl_parser.y" // lalr1.cc:919
    {}
#line 1152 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 61:
#line 304 "spl_parser.y" // lalr1.cc:919
    {}
#line 1158 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 62:
#line 308 "spl_parser.y" // lalr1.cc:919
    {}
#line 1164 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 63:
#line 312 "spl_parser.y" // lalr1.cc:919
    {}
#line 1170 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 64:
#line 313 "spl_parser.y" // lalr1.cc:919
    {}
#line 1176 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 65:
#line 317 "spl_parser.y" // lalr1.cc:919
    {}
#line 1182 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 66:
#line 318 "spl_parser.y" // lalr1.cc:919
    {}
#line 1188 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 67:
#line 322 "spl_parser.y" // lalr1.cc:919
    {}
#line 1194 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 68:
#line 323 "spl_parser.y" // lalr1.cc:919
    {}
#line 1200 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 69:
#line 324 "spl_parser.y" // lalr1.cc:919
    {}
#line 1206 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 70:
#line 325 "spl_parser.y" // lalr1.cc:919
    {}
#line 1212 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 71:
#line 326 "spl_parser.y" // lalr1.cc:919
    {}
#line 1218 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 72:
#line 327 "spl_parser.y" // lalr1.cc:919
    {}
#line 1224 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 73:
#line 328 "spl_parser.y" // lalr1.cc:919
    {}
#line 1230 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 74:
#line 329 "spl_parser.y" // lalr1.cc:919
    {}
#line 1236 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 75:
#line 330 "spl_parser.y" // lalr1.cc:919
    {}
#line 1242 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 76:
#line 334 "spl_parser.y" // lalr1.cc:919
    {}
#line 1248 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 77:
#line 335 "spl_parser.y" // lalr1.cc:919
    {}
#line 1254 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 78:
#line 336 "spl_parser.y" // lalr1.cc:919
    {}
#line 1260 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 79:
#line 340 "spl_parser.y" // lalr1.cc:919
    {}
#line 1266 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 80:
#line 341 "spl_parser.y" // lalr1.cc:919
    {}
#line 1272 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 81:
#line 342 "spl_parser.y" // lalr1.cc:919
    {}
#line 1278 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 82:
#line 343 "spl_parser.y" // lalr1.cc:919
    {}
#line 1284 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 83:
#line 344 "spl_parser.y" // lalr1.cc:919
    {}
#line 1290 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 84:
#line 348 "spl_parser.y" // lalr1.cc:919
    {}
#line 1296 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 85:
#line 352 "spl_parser.y" // lalr1.cc:919
    {}
#line 1302 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 86:
#line 353 "spl_parser.y" // lalr1.cc:919
    {}
#line 1308 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 87:
#line 357 "spl_parser.y" // lalr1.cc:919
    {}
#line 1314 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 88:
#line 361 "spl_parser.y" // lalr1.cc:919
    {}
#line 1320 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 89:
#line 365 "spl_parser.y" // lalr1.cc:919
    {}
#line 1326 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 90:
#line 369 "spl_parser.y" // lalr1.cc:919
    {}
#line 1332 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 91:
#line 370 "spl_parser.y" // lalr1.cc:919
    {}
#line 1338 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 92:
#line 374 "spl_parser.y" // lalr1.cc:919
    {}
#line 1344 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 93:
#line 378 "spl_parser.y" // lalr1.cc:919
    {}
#line 1350 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 94:
#line 379 "spl_parser.y" // lalr1.cc:919
    {}
#line 1356 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 95:
#line 383 "spl_parser.y" // lalr1.cc:919
    {}
#line 1362 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 96:
#line 384 "spl_parser.y" // lalr1.cc:919
    {}
#line 1368 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 97:
#line 388 "spl_parser.y" // lalr1.cc:919
    {}
#line 1374 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 98:
#line 392 "spl_parser.y" // lalr1.cc:919
    {}
#line 1380 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 99:
#line 393 "spl_parser.y" // lalr1.cc:919
    {}
#line 1386 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 100:
#line 397 "spl_parser.y" // lalr1.cc:919
    {}
#line 1392 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 101:
#line 398 "spl_parser.y" // lalr1.cc:919
    {}
#line 1398 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 102:
#line 399 "spl_parser.y" // lalr1.cc:919
    {}
#line 1404 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 103:
#line 400 "spl_parser.y" // lalr1.cc:919
    {}
#line 1410 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 104:
#line 401 "spl_parser.y" // lalr1.cc:919
    {}
#line 1416 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 105:
#line 402 "spl_parser.y" // lalr1.cc:919
    {}
#line 1422 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 106:
#line 403 "spl_parser.y" // lalr1.cc:919
    {}
#line 1428 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 107:
#line 407 "spl_parser.y" // lalr1.cc:919
    {}
#line 1434 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 108:
#line 408 "spl_parser.y" // lalr1.cc:919
    {}
#line 1440 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 109:
#line 409 "spl_parser.y" // lalr1.cc:919
    {}
#line 1446 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 110:
#line 410 "spl_parser.y" // lalr1.cc:919
    {}
#line 1452 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 111:
#line 414 "spl_parser.y" // lalr1.cc:919
    {}
#line 1458 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 112:
#line 415 "spl_parser.y" // lalr1.cc:919
    {}
#line 1464 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 113:
#line 416 "spl_parser.y" // lalr1.cc:919
    {}
#line 1470 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 114:
#line 417 "spl_parser.y" // lalr1.cc:919
    {}
#line 1476 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 115:
#line 418 "spl_parser.y" // lalr1.cc:919
    {}
#line 1482 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 116:
#line 422 "spl_parser.y" // lalr1.cc:919
    {}
#line 1488 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 117:
#line 423 "spl_parser.y" // lalr1.cc:919
    {}
#line 1494 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 118:
#line 424 "spl_parser.y" // lalr1.cc:919
    {}
#line 1500 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 119:
#line 425 "spl_parser.y" // lalr1.cc:919
    {}
#line 1506 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 120:
#line 426 "spl_parser.y" // lalr1.cc:919
    {}
#line 1512 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 121:
#line 427 "spl_parser.y" // lalr1.cc:919
    {}
#line 1518 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 122:
#line 428 "spl_parser.y" // lalr1.cc:919
    {}
#line 1524 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 123:
#line 429 "spl_parser.y" // lalr1.cc:919
    {}
#line 1530 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 124:
#line 430 "spl_parser.y" // lalr1.cc:919
    {}
#line 1536 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 125:
#line 431 "spl_parser.y" // lalr1.cc:919
    {}
#line 1542 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 126:
#line 435 "spl_parser.y" // lalr1.cc:919
    {}
#line 1548 "spl_parser.tab.cc" // lalr1.cc:919
    break;

  case 127:
#line 436 "spl_parser.y" // lalr1.cc:919
    {}
#line 1554 "spl_parser.tab.cc" // lalr1.cc:919
    break;


#line 1558 "spl_parser.tab.cc" // lalr1.cc:919
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


  const short SPL_Parser::yypact_ninf_ = -176;

  const signed char SPL_Parser::yytable_ninf_ = -1;

  const short
  SPL_Parser::yypact_[] =
  {
      16,   -40,    61,  -176,    22,  -176,    31,    89,    74,  -176,
    -176,  -176,  -176,  -176,    55,    86,   181,   123,    77,    84,
      98,    70,  -176,    91,    96,    70,  -176,    70,    97,    99,
      93,     5,  -176,   100,  -176,  -176,  -176,  -176,  -176,  -176,
    -176,  -176,  -176,    37,   164,   169,    84,  -176,   132,   -17,
      70,    70,  -176,   130,  -176,  -176,  -176,  -176,    -3,    70,
    -176,   274,    88,    53,  -176,   188,  -176,   256,   108,   288,
      70,    70,   226,    70,    70,    70,   138,  -176,   133,    37,
     115,  -176,  -176,   -11,   132,  -176,   141,   144,   -17,  -176,
     136,  -176,   139,  -176,  -176,    70,    70,    70,   149,    19,
      70,    70,    70,    70,    70,    70,   277,    70,    70,    70,
      70,    70,    70,    70,    70,   194,    70,   194,   152,   -44,
     325,  -176,   325,   325,    76,    13,   209,  -176,   153,    37,
     158,   132,  -176,   157,   132,   160,   156,  -176,  -176,  -176,
     171,   115,  -176,   172,   172,  -176,  -176,  -176,  -176,    83,
     116,    33,  -176,  -176,    88,    88,    88,    88,    88,    88,
     166,   173,   225,  -176,    53,    53,    53,  -176,  -176,  -176,
    -176,   249,   208,   325,  -176,  -176,  -176,    70,  -176,    70,
     231,    70,  -176,   175,   129,    -8,  -176,    44,   184,   131,
      37,  -176,  -176,   176,    -7,   180,  -176,   186,    89,   190,
    -176,  -176,  -176,   194,   194,  -176,  -176,  -176,  -176,    70,
     194,  -176,   325,   325,    70,   325,    60,   185,  -176,  -176,
     115,  -176,  -176,  -176,  -176,   132,   192,    51,  -176,   187,
     211,   129,  -176,  -176,  -176,   224,   227,   304,  -176,   325,
      37,  -176,   258,   228,   192,  -176,    -7,   129,   129,  -176,
    -176,  -176,   194,  -176,   115,  -176,  -176,  -176,  -176,  -176,
    -176
  };

  const unsigned char
  SPL_Parser::yydefact_[] =
  {
       0,     0,     0,     7,     0,     1,     0,     0,     9,     3,
       2,    64,     4,    61,     0,    18,     0,     0,     8,     0,
      40,     0,    62,     0,     0,     0,    64,     0,     0,    81,
       0,    79,    69,     0,    66,    67,    68,    70,    71,    72,
      73,    74,    75,     0,     0,     0,    17,    20,     0,    48,
       0,     0,    16,   118,    12,    13,    14,    15,   116,     0,
     120,     0,   106,   110,   115,     0,    97,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,     0,     0,
       0,    19,    38,     0,    39,    42,     0,     0,     6,    46,
       0,    47,     0,   122,   123,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      99,    65,    76,   127,     0,     0,     0,    11,     0,     0,
       0,     0,    25,    26,     0,     0,     0,    22,    23,    24,
       0,     0,    41,    54,    54,    44,    45,     7,     7,     0,
       0,     0,   125,   121,   104,   105,   100,   101,   102,   103,
       0,     0,     0,    94,   109,   107,   108,   114,   111,   112,
     113,     0,    86,    87,    88,    83,    82,     0,    80,     0,
       0,     0,    10,     0,     0,     0,    35,     0,     0,     0,
       0,    21,    37,     0,     0,     0,    52,     0,     0,     0,
     119,   117,   124,     0,     0,    92,    93,    91,    90,     0,
       0,    84,    98,   126,     0,    78,     0,     0,    33,    34,
       0,    31,    27,    28,    43,     0,    60,     0,    56,     0,
       0,     0,    49,     5,    51,     0,     0,     0,    85,    77,
       0,    29,     0,     0,    59,    53,     0,     0,     0,    50,
      96,    95,     0,    30,     0,    36,    55,    57,    58,    89,
      32
  };

  const short
  SPL_Parser::yypgoto_[] =
  {
    -176,  -176,  -176,  -176,   145,   289,  -176,  -176,  -176,   -43,
    -176,  -176,   255,  -137,  -175,  -176,  -176,  -176,   117,  -128,
    -176,  -176,   219,  -176,   216,  -176,   218,  -176,   163,  -176,
      62,  -176,  -176,   111,    -5,   291,  -104,   246,  -176,  -176,
    -176,  -176,  -176,  -176,  -176,  -176,  -176,  -176,   159,  -176,
    -176,   -20,   238,   -75,   -32,   -31
  };

  const short
  SPL_Parser::yydefgoto_[] =
  {
      -1,     2,     3,     6,   197,   198,     8,    15,    18,    60,
      20,    46,    47,   136,   137,   138,   139,   185,   186,    83,
      49,    84,    85,    88,    89,    90,    91,    92,   195,   227,
     228,   229,   230,    12,    32,    16,    33,    34,    35,    36,
      37,   211,    38,    39,    40,   209,    41,   162,   163,    42,
     119,   123,    62,    63,    64,   124
  };

  const unsigned short
  SPL_Parser::yytable_[] =
  {
      78,    61,    13,   187,   193,    67,   189,    69,    73,   217,
      86,   172,   176,   174,     4,   177,    87,   218,    93,    94,
     100,   101,   102,   103,   104,   105,   100,   101,   102,   103,
     104,   105,   164,   165,   166,   225,   128,   135,   118,    99,
     100,   101,   102,   103,   104,   105,    82,    82,   140,   141,
       1,   120,    96,   122,    97,   125,   249,   187,   110,    98,
      74,     5,    75,   161,   149,   150,   226,    76,   111,   112,
     113,   180,   257,   258,   240,   153,    50,   151,   167,   168,
     169,   170,    52,   243,    51,     9,   183,    54,    55,    56,
      57,   202,   107,    10,   171,    14,   173,   244,   135,   235,
     236,   108,   109,   140,   220,    52,   238,   245,    11,    17,
      54,    55,    56,    57,   246,    52,    53,   260,   226,   161,
      54,    55,    56,    57,    58,    59,    19,    11,    21,   129,
      43,    44,   178,   130,    23,   179,    24,    25,    45,   200,
      48,   135,   179,   129,    26,    65,    66,   223,   259,   116,
     131,    27,    70,    72,    71,    28,    29,   212,    30,   213,
      52,   215,    31,    77,   132,    54,    55,    56,    57,   133,
     134,    79,   201,   241,    52,   179,    80,   135,   132,    54,
      55,    56,    57,   133,   134,    95,    82,   222,   135,   237,
     140,   114,   126,    13,   239,   143,   127,   253,   144,   147,
      11,    21,   148,   152,   135,   135,    22,    23,   175,    24,
      25,   135,   181,    11,    21,   184,   182,    26,   188,   191,
      23,   190,    24,    25,    27,   192,   203,   194,    28,    29,
      26,    30,   210,   204,   214,    31,   216,    27,   221,   224,
     231,    28,    29,   242,    30,    11,    21,   247,    31,   232,
     205,   140,    23,   234,    24,    25,   100,   101,   102,   103,
     104,   105,    26,   100,   101,   102,   103,   104,   105,    27,
      52,   248,   207,    28,    29,    54,    55,    56,    57,   160,
      31,   100,   101,   102,   103,   104,   105,   250,   208,   254,
     251,   255,     7,   199,   115,   100,   101,   102,   103,   104,
     105,    81,   219,   142,   145,   106,   146,   196,   256,   233,
     117,   100,   101,   102,   103,   104,   105,    68,   121,     0,
       0,   206,    52,     0,     0,     0,   252,    54,    55,    56,
      57,   160,   100,   101,   102,   103,   104,   105,   154,   155,
     156,   157,   158,   159
  };

  const short
  SPL_Parser::yycheck_[] =
  {
      43,    21,     7,   131,   141,    25,   134,    27,     3,   184,
      27,   115,    56,   117,    54,    59,    33,    25,    50,    51,
       7,     8,     9,    10,    11,    12,     7,     8,     9,    10,
      11,    12,   107,   108,   109,    42,    79,    80,    70,    59,
       7,     8,     9,    10,    11,    12,    54,    54,    59,    60,
      34,    71,    55,    73,    57,    75,   231,   185,     5,    62,
      55,     0,    57,   106,    95,    96,   194,    62,    15,    16,
      17,    58,   247,   248,    14,    56,     6,    97,   110,   111,
     112,   113,    45,   220,    14,    63,   129,    50,    51,    52,
      53,    58,     4,    62,   114,    21,   116,   225,   141,   203,
     204,    13,    14,    59,    60,    45,   210,    56,    19,    54,
      50,    51,    52,    53,    63,    45,    46,   254,   246,   162,
      50,    51,    52,    53,    54,    55,    40,    19,    20,    14,
       7,    54,    56,    18,    26,    59,    28,    29,    54,    56,
      42,   184,    59,    14,    36,    54,    50,   190,   252,    41,
      35,    43,    55,    60,    55,    47,    48,   177,    50,   179,
      45,   181,    54,    63,    49,    50,    51,    52,    53,    54,
      55,     7,    56,   216,    45,    59,     7,   220,    49,    50,
      51,    52,    53,    54,    55,    55,    54,    56,   231,   209,
      59,     3,    54,   198,   214,    54,    63,   240,    54,    63,
      19,    20,    63,    54,   247,   248,    25,    26,    56,    28,
      29,   254,     3,    19,    20,    57,    63,    36,    61,    63,
      26,    61,    28,    29,    43,    54,    60,    55,    47,    48,
      36,    50,    24,    60,     3,    54,    61,    43,    54,    63,
      60,    47,    48,    58,    50,    19,    20,    60,    54,    63,
      25,    59,    26,    63,    28,    29,     7,     8,     9,    10,
      11,    12,    36,     7,     8,     9,    10,    11,    12,    43,
      45,    60,    23,    47,    48,    50,    51,    52,    53,    54,
      54,     7,     8,     9,    10,    11,    12,    63,    39,    31,
      63,    63,     3,   148,    38,     7,     8,     9,    10,    11,
      12,    46,   185,    84,    88,    31,    88,   144,   246,   198,
      22,     7,     8,     9,    10,    11,    12,    26,    72,    -1,
      -1,   162,    45,    -1,    -1,    -1,    22,    50,    51,    52,
      53,    54,     7,     8,     9,    10,    11,    12,   100,   101,
     102,   103,   104,   105
  };

  const unsigned char
  SPL_Parser::yystos_[] =
  {
       0,    34,    65,    66,    54,     0,    67,    69,    70,    63,
      62,    19,    97,    98,    21,    71,    99,    54,    72,    40,
      74,    20,    25,    26,    28,    29,    36,    43,    47,    48,
      50,    54,    98,   100,   101,   102,   103,   104,   106,   107,
     108,   110,   113,     7,    54,    54,    75,    76,    42,    84,
       6,    14,    45,    46,    50,    51,    52,    53,    54,    55,
      73,   115,   116,   117,   118,    54,    50,   115,    99,   115,
      55,    55,    60,     3,    55,    57,    62,    63,    73,     7,
       7,    76,    54,    83,    85,    86,    27,    33,    87,    88,
      89,    90,    91,   118,   118,    55,    55,    57,    62,   115,
       7,     8,     9,    10,    11,    12,    31,     4,    13,    14,
       5,    15,    16,    17,     3,    38,    41,    22,   118,   114,
     115,   101,   115,   115,   119,   115,    54,    63,    73,    14,
      18,    35,    49,    54,    55,    73,    77,    78,    79,    80,
      59,    60,    86,    54,    54,    88,    90,    63,    63,   119,
     119,   115,    54,    56,   116,   116,   116,   116,   116,   116,
      54,    73,   111,   112,   117,   117,   117,   118,   118,   118,
     118,   115,   100,   115,   100,    56,    56,    59,    56,    59,
      58,     3,    63,    73,    57,    81,    82,    83,    61,    83,
      61,    63,    54,    77,    55,    92,    92,    68,    69,    68,
      56,    56,    58,    60,    60,    25,   112,    23,    39,   109,
      24,   105,   115,   115,     3,   115,    61,    78,    25,    82,
      60,    54,    56,    73,    63,    42,    83,    93,    94,    95,
      96,    60,    63,    97,    63,   100,   100,   115,   100,   115,
      14,    73,    58,    77,    83,    56,    63,    60,    60,    78,
      63,    63,    22,    73,    31,    63,    94,    78,    78,   100,
      77
  };

  const unsigned char
  SPL_Parser::yyr1_[] =
  {
       0,    64,    65,    66,    67,    68,    69,    70,    71,    71,
      72,    72,    73,    73,    73,    73,    73,    74,    74,    75,
      75,    76,    77,    77,    77,    78,    78,    78,    78,    78,
      78,    78,    79,    80,    81,    81,    82,    83,    83,    84,
      84,    85,    85,    86,    87,    87,    87,    87,    87,    88,
      89,    90,    91,    92,    92,    93,    93,    94,    94,    95,
      96,    97,    98,    99,    99,   100,   100,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   102,   102,   102,   103,
     103,   103,   103,   103,   104,   105,   105,   106,   107,   108,
     109,   109,   110,   111,   111,   112,   112,   113,   114,   114,
     115,   115,   115,   115,   115,   115,   115,   116,   116,   116,
     116,   117,   117,   117,   117,   117,   118,   118,   118,   118,
     118,   118,   118,   118,   118,   118,   119,   119
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
  "\"end of file\"", "error", "$undefined", "ASSIGN", "OR", "AND", "NOT",
  "EQUAL", "UNEQUAL", "GE", "GT", "LE", "LT", "PLUS", "MINUS", "MUL",
  "DIV", "MOD", "ARRAY", "_BEGIN", "CASE", "CONST", "DO", "DOWNTO", "ELSE",
  "_END", "FOR", "FUNCTION", "GOTO", "IF", "IN", "OF", "PACKED",
  "PROCEDURE", "PROGRAM", "RECORD", "REPEAT", "SET", "THEN", "TO", "TYPE",
  "UNTIL", "VAR", "WHILE", "WITH", "SYS_CON", "SYS_FUNCT", "READ",
  "SYS_PROC", "SYS_TYPE", "INTEGER", "REAL", "CHAR", "STRING", "ID", "LP",
  "RP", "LB", "RB", "COMMA", "COLON", "DOTDOT", "DOT", "SEMI", "$accept",
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
       0,   150,   150,   154,   157,   161,   165,   169,   173,   174,
     178,   179,   183,   184,   185,   186,   187,   191,   192,   196,
     197,   201,   205,   206,   207,   211,   212,   213,   214,   215,
     216,   217,   221,   225,   229,   230,   234,   238,   239,   243,
     244,   248,   249,   253,   257,   258,   259,   260,   261,   265,
     269,   273,   277,   281,   282,   286,   287,   291,   292,   296,
     300,   304,   308,   312,   313,   317,   318,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   334,   335,   336,   340,
     341,   342,   343,   344,   348,   352,   353,   357,   361,   365,
     369,   370,   374,   378,   379,   383,   384,   388,   392,   393,
     397,   398,   399,   400,   401,   402,   403,   407,   408,   409,
     410,   414,   415,   416,   417,   418,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   431,   435,   436
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
      55,    56,    57,    58,    59,    60,    61,    62,    63
    };
    const unsigned user_token_number_max_ = 318;
    const token_number_type undef_token_ = 2;

    if (static_cast<int> (t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

#line 28 "spl_parser.y" // lalr1.cc:1242
} // SPL
#line 2199 "spl_parser.tab.cc" // lalr1.cc:1242
#line 440 "spl_parser.y" // lalr1.cc:1243


// TODO: error detection

void SPL::SPL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "spl.exe: error: " << err_message << " at " << l << "\n";
}
