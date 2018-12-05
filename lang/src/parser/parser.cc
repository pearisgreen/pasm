/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
/************ Begin %include sections from the grammar ************************/
#line 1 "./parser/grammar.y"

#include "token_types.h"
#include "cassert"
#include "lexer.h"
#line 33 "./parser/out/grammar.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ImplParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ImplParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ImplParseARG_SDECL     A static variable declaration for the %extra_argument
**    ImplParseARG_PDECL     A parameter declaration for the %extra_argument
**    ImplParseARG_PARAM     Code to pass %extra_argument as a subroutine parameter
**    ImplParseARG_STORE     Code to store %extra_argument into yypParser
**    ImplParseARG_FETCH     Code to extract %extra_argument from yypParser
**    ImplParseCTX_*         As ImplParseARG_ except for %extra_context
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYNTOKEN           Number of terminal symbols
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
**    YY_MIN_REDUCE      Minimum value for reduce actions
**    YY_MAX_REDUCE      Maximum value for reduce actions
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 93
#define YYACTIONTYPE unsigned short int
#define ImplParseTOKENTYPE token*
typedef union {
  int yyinit;
  ImplParseTOKENTYPE yy0;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ImplParseARG_SDECL  void *state ;
#define ImplParseARG_PDECL , void *state 
#define ImplParseARG_PARAM ,state 
#define ImplParseARG_FETCH  void *state =yypParser->state ;
#define ImplParseARG_STORE yypParser->state =state ;
#define ImplParseCTX_SDECL
#define ImplParseCTX_PDECL
#define ImplParseCTX_PARAM
#define ImplParseCTX_FETCH
#define ImplParseCTX_STORE
#define YYNSTATE             69
#define YYNRULE              84
#define YYNTOKEN             64
#define YY_MAX_SHIFT         68
#define YY_MIN_SHIFTREDUCE   132
#define YY_MAX_SHIFTREDUCE   215
#define YY_ERROR_ACTION      216
#define YY_ACCEPT_ACTION     217
#define YY_NO_ACTION         218
#define YY_MIN_REDUCE        219
#define YY_MAX_REDUCE        302
/************* End control #defines *******************************************/
#define YY_NLOOKAHEAD ((int)(sizeof(yy_lookahead)/sizeof(yy_lookahead[0])))

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X.
**
** The formulas above are for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (220)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */   261,  217,    8,   38,  261,   38,   37,   37,   44,   43,
 /*    10 */    57,   56,   54,   37,   37,   44,   41,  155,  156,  157,
 /*    20 */   158,  159,  160,  161,  162,  163,  164,   48,  233,   21,
 /*    30 */    38,   48,   38,   37,   37,   44,   43,   57,   56,   54,
 /*    40 */   221,  222,   27,  224,   30,   19,   20,  235,   11,   46,
 /*    50 */    29,   37,   37,   40,   47,  260,   33,   66,   38,  260,
 /*    60 */    38,   37,   37,   44,   43,   57,   56,   54,  289,   68,
 /*    70 */    37,   37,   44,   43,   57,   56,   54,  288,   64,   37,
 /*    80 */    37,   44,   43,   57,   56,   54,  231,   28,  287,  153,
 /*    90 */    37,   37,   44,   43,   57,   56,   54,   58,  170,   37,
 /*   100 */    37,   44,   43,   57,   56,   54,  198,  189,  188,   37,
 /*   110 */    37,   44,   43,   57,   53,  150,  167,  153,   39,   30,
 /*   120 */    12,   67,   39,   39,   18,   46,   67,  175,  176,  177,
 /*   130 */    45,   30,   35,   35,   45,   45,   51,   46,   32,  198,
 /*   140 */    65,   30,   29,    7,   65,   65,   62,   46,  256,   25,
 /*   150 */    60,   59,  192,  179,  180,  181,   52,   23,   24,  256,
 /*   160 */     3,   67,    2,   37,   37,   44,   43,   55,  183,  184,
 /*   170 */   185,   37,   37,   42,    5,    4,   22,  228,   26,  229,
 /*   180 */     6,  138,   13,   31,   13,   36,   36,   17,   16,  168,
 /*   190 */   168,   67,   34,  238,  227,  239,    1,  226,  193,  182,
 /*   200 */    10,  149,   49,   50,   14,    9,  178,   15,   61,  191,
 /*   210 */   195,  197,  196,  154,  186,   63,  153,   12,  219,   32,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    80,   64,   65,   83,   84,   85,   86,   87,   88,   89,
 /*    10 */    90,   91,   92,   86,   87,   88,   89,   17,   18,   19,
 /*    20 */    20,   21,   22,   23,   24,   25,   26,   80,   67,   68,
 /*    30 */    83,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*    40 */    66,   67,   68,   69,   70,   71,   73,   75,   48,   76,
 /*    50 */    50,   86,   87,   88,   82,   80,   46,   57,   83,   84,
 /*    60 */    85,   86,   87,   88,   89,   90,   91,   92,   84,    2,
 /*    70 */    86,   87,   88,   89,   90,   91,   92,   84,   33,   86,
 /*    80 */    87,   88,   89,   90,   91,   92,   67,   68,   84,   44,
 /*    90 */    86,   87,   88,   89,   90,   91,   92,   84,   29,   86,
 /*   100 */    87,   88,   89,   90,   91,   92,   11,   12,   13,   86,
 /*   110 */    87,   88,   89,   90,   91,   51,   47,   44,   69,   70,
 /*   120 */    28,   57,   73,   74,   57,   76,   57,   58,   59,   60,
 /*   130 */    69,   70,   86,   87,   73,   74,   63,   76,   46,   11,
 /*   140 */    69,   70,   50,   48,   73,   74,   48,   76,   67,   68,
 /*   150 */    52,   53,   57,   11,   12,   13,   61,   40,   41,   78,
 /*   160 */    79,   57,   81,   86,   87,   88,   89,   90,   11,   12,
 /*   170 */    13,   86,   87,   88,   38,   39,   33,   72,   34,   72,
 /*   180 */    44,   47,   77,   33,   77,   86,   87,   36,   37,   46,
 /*   190 */    46,   57,   33,   68,   68,   68,   33,   68,   51,   57,
 /*   200 */    62,   51,   57,   50,   35,   34,   47,   45,   11,   49,
 /*   210 */    49,   57,   57,   49,   57,   11,   44,   28,    0,   46,
 /*   220 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   230 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   240 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   250 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   260 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   270 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   280 */    93,   93,   93,   93,
};
#define YY_SHIFT_COUNT    (68)
#define YY_SHIFT_MIN      (0)
#define YY_SHIFT_MAX      (218)
static const unsigned short int yy_shift_ofst[] = {
 /*     0 */   220,   95,   95,   95,  128,  128,  128,  128,   67,  128,
 /*    10 */     0,    0,    0,   69,  128,  128,  128,  128,   92,  143,
 /*    20 */   144,   10,  104,  128,  128,   10,  104,   10,  220,   64,
 /*    30 */   134,  104,  104,  142,  157,  136,  136,  136,   98,   73,
 /*    40 */   117,  151,  117,  151,  117,   45,  150,  159,  163,  147,
 /*    50 */   145,  153,  138,  169,  171,  162,  169,  162,  160,  154,
 /*    60 */   155,  161,  197,  164,  204,  172,  173,  189,  218,
};
#define YY_REDUCE_COUNT (32)
#define YY_REDUCE_MIN   (-80)
#define YY_REDUCE_MAX   (129)
static const short yy_reduce_ofst[] = {
 /*     0 */   -63,  -80,  -53,  -25,  -16,   -7,    4,   13,  -26,   23,
 /*    10 */    49,   61,   71,   81,   77,  -73,  -35,   85,  -27,  105,
 /*    20 */   107,  -28,  -39,   46,   99,  -28,   19,  -28,  -28,  125,
 /*    30 */   126,  127,  129,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   220,  216,  216,  216,  216,  216,  216,  216,  216,  216,
 /*    10 */   216,  216,  216,  216,  216,  216,  216,  216,  216,  216,
 /*    20 */   216,  232,  216,  216,  216,  258,  216,  223,  230,  216,
 /*    30 */   216,  216,  216,  216,  216,  292,  291,  290,  274,  216,
 /*    40 */   295,  297,  294,  296,  293,  216,  216,  216,  216,  216,
 /*    50 */   216,  216,  216,  301,  302,  299,  300,  298,  216,  216,
 /*    60 */   216,  216,  216,  216,  216,  234,  216,  216,  216,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ImplParseARG_SDECL                /* A place to hold %extra_argument */
  ImplParseCTX_SDECL                /* A place to hold %extra_context */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ImplParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#if defined(YYCOVERAGE) || !defined(NDEBUG)
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  /*    0 */ "$",
  /*    1 */ "WHITESPACE",
  /*    2 */ "END",
  /*    3 */ "OTHER",
  /*    4 */ "TAB",
  /*    5 */ "NEW_LINE",
  /*    6 */ "CARRIAGE_RETURN",
  /*    7 */ "CHAR",
  /*    8 */ "MULTICHAR",
  /*    9 */ "LINE_COMMENT",
  /*   10 */ "MULTILINE_COMMENT",
  /*   11 */ "INTEGER",
  /*   12 */ "FLOAT",
  /*   13 */ "STRING",
  /*   14 */ "USE",
  /*   15 */ "AS",
  /*   16 */ "RET",
  /*   17 */ "U8",
  /*   18 */ "U16",
  /*   19 */ "U32",
  /*   20 */ "U64",
  /*   21 */ "I8",
  /*   22 */ "I16",
  /*   23 */ "I32",
  /*   24 */ "I64",
  /*   25 */ "F32",
  /*   26 */ "F64",
  /*   27 */ "EXCALAMATIONMARK_EQUALS",
  /*   28 */ "COLON",
  /*   29 */ "SEMICOLON",
  /*   30 */ "EXCLAMATIONMARK",
  /*   31 */ "QUESTIONMARK",
  /*   32 */ "EXP",
  /*   33 */ "COMMA",
  /*   34 */ "PIPE",
  /*   35 */ "CIRCUMFLEX",
  /*   36 */ "DOUBLE_LESS_THAN",
  /*   37 */ "DOUBLE_GREATER_THAN",
  /*   38 */ "SLASH",
  /*   39 */ "PERCENT",
  /*   40 */ "MINUS",
  /*   41 */ "PLUS",
  /*   42 */ "EXCLAMATION_MARK",
  /*   43 */ "TILDE",
  /*   44 */ "ASTERIX",
  /*   45 */ "AMPERSAND",
  /*   46 */ "L_C_BRACKET",
  /*   47 */ "R_C_BRACKET",
  /*   48 */ "L_S_BRACKET",
  /*   49 */ "R_S_BRACKET",
  /*   50 */ "L_R_BRACKET",
  /*   51 */ "R_R_BRACKET",
  /*   52 */ "DOT",
  /*   53 */ "ARROW",
  /*   54 */ "DOUBLE_COLON",
  /*   55 */ "POINTER_TYPE",
  /*   56 */ "VOID",
  /*   57 */ "ID",
  /*   58 */ "PUSH",
  /*   59 */ "MOV",
  /*   60 */ "ADD",
  /*   61 */ "CAST",
  /*   62 */ "LESS_THAN",
  /*   63 */ "GREATER_THAN",
  /*   64 */ "package",
  /*   65 */ "package_opt",
  /*   66 */ "function_def",
  /*   67 */ "var_inst",
  /*   68 */ "var_decl",
  /*   69 */ "struct_def",
  /*   70 */ "struct_def_opt",
  /*   71 */ "function_def_opt",
  /*   72 */ "block_statement",
  /*   73 */ "function_type",
  /*   74 */ "type",
  /*   75 */ "literal",
  /*   76 */ "function_type_opt",
  /*   77 */ "block_statement_opt",
  /*   78 */ "statement",
  /*   79 */ "u_mnemonic",
  /*   80 */ "operand",
  /*   81 */ "d_mnemonic",
  /*   82 */ "literal_opt",
  /*   83 */ "postfix_operand",
  /*   84 */ "scalar_operand",
  /*   85 */ "primary_operand",
  /*   86 */ "scalar_operand_int",
  /*   87 */ "multiplicative_operand",
  /*   88 */ "additive_operand",
  /*   89 */ "shift_operand",
  /*   90 */ "and_operand",
  /*   91 */ "exclusive_or_operand",
  /*   92 */ "inclusive_or_operand",
};
#endif /* defined(YYCOVERAGE) || !defined(NDEBUG) */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "package ::= package_opt END",
 /*   1 */ "package_opt ::=",
 /*   2 */ "package_opt ::= package_opt function_def",
 /*   3 */ "package_opt ::= package_opt var_inst",
 /*   4 */ "package_opt ::= package_opt var_decl",
 /*   5 */ "package_opt ::= package_opt struct_def",
 /*   6 */ "struct_def ::= struct_def_opt R_C_BRACKET",
 /*   7 */ "struct_def_opt ::= ID L_C_BRACKET var_decl",
 /*   8 */ "struct_def_opt ::= struct_def_opt var_decl",
 /*   9 */ "function_def ::= function_def_opt block_statement",
 /*  10 */ "function_def ::= ID function_type block_statement",
 /*  11 */ "function_def_opt ::= ID function_type PIPE var_decl",
 /*  12 */ "function_def_opt ::= ID function_type PIPE var_inst",
 /*  13 */ "function_def_opt ::= function_def_opt COMMA var_decl",
 /*  14 */ "function_def_opt ::= function_def_opt COMMA var_inst",
 /*  15 */ "var_decl ::= ID COLON type",
 /*  16 */ "var_inst ::= var_decl literal",
 /*  17 */ "function_type ::= function_type_opt R_R_BRACKET",
 /*  18 */ "function_type ::= L_R_BRACKET R_R_BRACKET",
 /*  19 */ "function_type_opt ::= L_R_BRACKET var_decl",
 /*  20 */ "function_type_opt ::= function_type_opt COMMA var_decl",
 /*  21 */ "type ::= type ASTERIX",
 /*  22 */ "type ::= L_S_BRACKET type COMMA INTEGER R_S_BRACKET",
 /*  23 */ "type ::= U8",
 /*  24 */ "type ::= U16",
 /*  25 */ "type ::= U32",
 /*  26 */ "type ::= U64",
 /*  27 */ "type ::= I8",
 /*  28 */ "type ::= I16",
 /*  29 */ "type ::= I32",
 /*  30 */ "type ::= I64",
 /*  31 */ "type ::= F32",
 /*  32 */ "type ::= F64",
 /*  33 */ "type ::= function_type",
 /*  34 */ "type ::= struct_def",
 /*  35 */ "block_statement ::= block_statement_opt R_C_BRACKET",
 /*  36 */ "block_statement_opt ::= L_C_BRACKET",
 /*  37 */ "block_statement_opt ::= block_statement_opt statement",
 /*  38 */ "statement ::= SEMICOLON",
 /*  39 */ "statement ::= var_decl",
 /*  40 */ "statement ::= var_inst",
 /*  41 */ "statement ::= u_mnemonic operand",
 /*  42 */ "statement ::= d_mnemonic operand COMMA operand",
 /*  43 */ "u_mnemonic ::= PUSH",
 /*  44 */ "d_mnemonic ::= MOV",
 /*  45 */ "d_mnemonic ::= ADD",
 /*  46 */ "literal ::= literal_opt R_C_BRACKET",
 /*  47 */ "literal_opt ::= L_C_BRACKET INTEGER",
 /*  48 */ "literal_opt ::= L_C_BRACKET FLOAT",
 /*  49 */ "literal_opt ::= L_C_BRACKET STRING",
 /*  50 */ "literal_opt ::= L_C_BRACKET ID",
 /*  51 */ "literal_opt ::= literal_opt COMMA INTEGER",
 /*  52 */ "literal_opt ::= literal_opt COMMA FLOAT",
 /*  53 */ "literal_opt ::= literal_opt COMMA STRING",
 /*  54 */ "literal_opt ::= literal_opt COMMA ID",
 /*  55 */ "operand ::= postfix_operand",
 /*  56 */ "operand ::= STRING",
 /*  57 */ "operand ::= FLOAT",
 /*  58 */ "operand ::= scalar_operand",
 /*  59 */ "operand ::= L_S_BRACKET scalar_operand R_S_BRACKET",
 /*  60 */ "primary_operand ::= ID",
 /*  61 */ "primary_operand ::= CAST LESS_THAN type GREATER_THAN L_R_BRACKET ID R_R_BRACKET",
 /*  62 */ "postfix_operand ::= primary_operand",
 /*  63 */ "postfix_operand ::= postfix_operand L_S_BRACKET INTEGER R_S_BRACKET",
 /*  64 */ "postfix_operand ::= postfix_operand DOT ID",
 /*  65 */ "postfix_operand ::= postfix_operand ARROW ID",
 /*  66 */ "scalar_operand_int ::= INTEGER",
 /*  67 */ "multiplicative_operand ::= scalar_operand_int",
 /*  68 */ "multiplicative_operand ::= multiplicative_operand ASTERIX scalar_operand",
 /*  69 */ "multiplicative_operand ::= multiplicative_operand SLASH scalar_operand",
 /*  70 */ "multiplicative_operand ::= multiplicative_operand PERCENT scalar_operand",
 /*  71 */ "additive_operand ::= multiplicative_operand",
 /*  72 */ "additive_operand ::= additive_operand PLUS multiplicative_operand",
 /*  73 */ "additive_operand ::= additive_operand MINUS multiplicative_operand",
 /*  74 */ "shift_operand ::= additive_operand",
 /*  75 */ "shift_operand ::= shift_operand DOUBLE_LESS_THAN additive_operand",
 /*  76 */ "shift_operand ::= shift_operand DOUBLE_GREATER_THAN additive_operand",
 /*  77 */ "and_operand ::= shift_operand",
 /*  78 */ "and_operand ::= and_operand AMPERSAND shift_operand",
 /*  79 */ "exclusive_or_operand ::= and_operand",
 /*  80 */ "exclusive_or_operand ::= exclusive_or_operand CIRCUMFLEX and_operand",
 /*  81 */ "inclusive_or_operand ::= exclusive_or_operand",
 /*  82 */ "inclusive_or_operand ::= inclusive_or_operand PIPE exclusive_or_operand",
 /*  83 */ "scalar_operand ::= inclusive_or_operand",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ImplParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ImplParseInit(void *yypRawParser ImplParseCTX_PDECL){
  yyParser *yypParser = (yyParser*)yypRawParser;
  ImplParseCTX_STORE
#ifdef YYTRACKMAXSTACKDEPTH
  yypParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  yypParser->yytos = NULL;
  yypParser->yystack = NULL;
  yypParser->yystksz = 0;
  if( yyGrowStack(yypParser) ){
    yypParser->yystack = &yypParser->yystk0;
    yypParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  yypParser->yytos = yypParser->yystack;
  yypParser->yystack[0].stateno = 0;
  yypParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  yypParser->yystackEnd = &yypParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef ImplParse_ENGINEALWAYSONSTACK
/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to ImplParse and ImplParseFree.
*/
void *ImplParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE) ImplParseCTX_PDECL){
  yyParser *yypParser;
  yypParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( yypParser ){
    ImplParseCTX_STORE
    ImplParseInit(yypParser ImplParseCTX_PARAM);
  }
  return (void*)yypParser;
}
#endif /* ImplParse_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ImplParseARG_FETCH
  ImplParseCTX_FETCH
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ImplParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef ImplParse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ImplParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ImplParseFinalize(p);
  (*freeProc)(p);
}
#endif /* ImplParse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ImplParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/* This array of booleans keeps track of the parser statement
** coverage.  The element yycoverage[X][Y] is set when the parser
** is in state X and has a lookahead token Y.  In a well-tested
** systems, every element of this matrix should end up being set.
*/
#if defined(YYCOVERAGE)
static unsigned char yycoverage[YYNSTATE][YYNTOKEN];
#endif

/*
** Write into out a description of every state/lookahead combination that
**
**   (1)  has not been used by the parser, and
**   (2)  is not a syntax error.
**
** Return the number of missed state/lookahead combinations.
*/
#if defined(YYCOVERAGE)
int ImplParseCoverage(FILE *out){
  int stateno, iLookAhead, i;
  int nMissed = 0;
  for(stateno=0; stateno<YYNSTATE; stateno++){
    i = yy_shift_ofst[stateno];
    for(iLookAhead=0; iLookAhead<YYNTOKEN; iLookAhead++){
      if( yy_lookahead[i+iLookAhead]!=iLookAhead ) continue;
      if( yycoverage[stateno][iLookAhead]==0 ) nMissed++;
      if( out ){
        fprintf(out,"State %d lookahead %s %s\n", stateno,
                yyTokenName[iLookAhead],
                yycoverage[stateno][iLookAhead] ? "ok" : "missed");
      }
    }
  }
  return nMissed;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static YYACTIONTYPE yy_find_shift_action(
  YYCODETYPE iLookAhead,    /* The look-ahead token */
  YYACTIONTYPE stateno      /* Current state number */
){
  int i;

  if( stateno>YY_MAX_SHIFT ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
#if defined(YYCOVERAGE)
  yycoverage[stateno][iLookAhead] = 1;
#endif
  do{
    i = yy_shift_ofst[stateno];
    assert( i>=0 );
    /* assert( i+YYNTOKEN<=(int)YY_NLOOKAHEAD ); */
    assert( iLookAhead!=YYNOCODE );
    assert( iLookAhead < YYNTOKEN );
    i += iLookAhead;
    if( i>=YY_NLOOKAHEAD || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          j<(int)(sizeof(yy_lookahead)/sizeof(yy_lookahead[0])) &&
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static YYACTIONTYPE yy_find_reduce_action(
  YYACTIONTYPE stateno,     /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ImplParseARG_FETCH
   ImplParseCTX_FETCH
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ImplParseARG_STORE /* Suppress warning about unused %extra_argument var */
   ImplParseCTX_STORE
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState, const char *zTag){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%s%s '%s', go to state %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%s%s '%s', pending reduce %d\n",
         yyTracePrompt, zTag, yyTokenName[yypParser->yytos->major],
         yyNewState - YY_MIN_REDUCE);
    }
  }
}
#else
# define yyTraceShift(X,Y,Z)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  YYACTIONTYPE yyNewState,      /* The new state to shift in */
  YYCODETYPE yyMajor,           /* The major token to shift in */
  ImplParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState, "Shift");
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  {   64,   -2 }, /* (0) package ::= package_opt END */
  {   65,    0 }, /* (1) package_opt ::= */
  {   65,   -2 }, /* (2) package_opt ::= package_opt function_def */
  {   65,   -2 }, /* (3) package_opt ::= package_opt var_inst */
  {   65,   -2 }, /* (4) package_opt ::= package_opt var_decl */
  {   65,   -2 }, /* (5) package_opt ::= package_opt struct_def */
  {   69,   -2 }, /* (6) struct_def ::= struct_def_opt R_C_BRACKET */
  {   70,   -3 }, /* (7) struct_def_opt ::= ID L_C_BRACKET var_decl */
  {   70,   -2 }, /* (8) struct_def_opt ::= struct_def_opt var_decl */
  {   66,   -2 }, /* (9) function_def ::= function_def_opt block_statement */
  {   66,   -3 }, /* (10) function_def ::= ID function_type block_statement */
  {   71,   -4 }, /* (11) function_def_opt ::= ID function_type PIPE var_decl */
  {   71,   -4 }, /* (12) function_def_opt ::= ID function_type PIPE var_inst */
  {   71,   -3 }, /* (13) function_def_opt ::= function_def_opt COMMA var_decl */
  {   71,   -3 }, /* (14) function_def_opt ::= function_def_opt COMMA var_inst */
  {   68,   -3 }, /* (15) var_decl ::= ID COLON type */
  {   67,   -2 }, /* (16) var_inst ::= var_decl literal */
  {   73,   -2 }, /* (17) function_type ::= function_type_opt R_R_BRACKET */
  {   73,   -2 }, /* (18) function_type ::= L_R_BRACKET R_R_BRACKET */
  {   76,   -2 }, /* (19) function_type_opt ::= L_R_BRACKET var_decl */
  {   76,   -3 }, /* (20) function_type_opt ::= function_type_opt COMMA var_decl */
  {   74,   -2 }, /* (21) type ::= type ASTERIX */
  {   74,   -5 }, /* (22) type ::= L_S_BRACKET type COMMA INTEGER R_S_BRACKET */
  {   74,   -1 }, /* (23) type ::= U8 */
  {   74,   -1 }, /* (24) type ::= U16 */
  {   74,   -1 }, /* (25) type ::= U32 */
  {   74,   -1 }, /* (26) type ::= U64 */
  {   74,   -1 }, /* (27) type ::= I8 */
  {   74,   -1 }, /* (28) type ::= I16 */
  {   74,   -1 }, /* (29) type ::= I32 */
  {   74,   -1 }, /* (30) type ::= I64 */
  {   74,   -1 }, /* (31) type ::= F32 */
  {   74,   -1 }, /* (32) type ::= F64 */
  {   74,   -1 }, /* (33) type ::= function_type */
  {   74,   -1 }, /* (34) type ::= struct_def */
  {   72,   -2 }, /* (35) block_statement ::= block_statement_opt R_C_BRACKET */
  {   77,   -1 }, /* (36) block_statement_opt ::= L_C_BRACKET */
  {   77,   -2 }, /* (37) block_statement_opt ::= block_statement_opt statement */
  {   78,   -1 }, /* (38) statement ::= SEMICOLON */
  {   78,   -1 }, /* (39) statement ::= var_decl */
  {   78,   -1 }, /* (40) statement ::= var_inst */
  {   78,   -2 }, /* (41) statement ::= u_mnemonic operand */
  {   78,   -4 }, /* (42) statement ::= d_mnemonic operand COMMA operand */
  {   79,   -1 }, /* (43) u_mnemonic ::= PUSH */
  {   81,   -1 }, /* (44) d_mnemonic ::= MOV */
  {   81,   -1 }, /* (45) d_mnemonic ::= ADD */
  {   75,   -2 }, /* (46) literal ::= literal_opt R_C_BRACKET */
  {   82,   -2 }, /* (47) literal_opt ::= L_C_BRACKET INTEGER */
  {   82,   -2 }, /* (48) literal_opt ::= L_C_BRACKET FLOAT */
  {   82,   -2 }, /* (49) literal_opt ::= L_C_BRACKET STRING */
  {   82,   -2 }, /* (50) literal_opt ::= L_C_BRACKET ID */
  {   82,   -3 }, /* (51) literal_opt ::= literal_opt COMMA INTEGER */
  {   82,   -3 }, /* (52) literal_opt ::= literal_opt COMMA FLOAT */
  {   82,   -3 }, /* (53) literal_opt ::= literal_opt COMMA STRING */
  {   82,   -3 }, /* (54) literal_opt ::= literal_opt COMMA ID */
  {   80,   -1 }, /* (55) operand ::= postfix_operand */
  {   80,   -1 }, /* (56) operand ::= STRING */
  {   80,   -1 }, /* (57) operand ::= FLOAT */
  {   80,   -1 }, /* (58) operand ::= scalar_operand */
  {   80,   -3 }, /* (59) operand ::= L_S_BRACKET scalar_operand R_S_BRACKET */
  {   85,   -1 }, /* (60) primary_operand ::= ID */
  {   85,   -7 }, /* (61) primary_operand ::= CAST LESS_THAN type GREATER_THAN L_R_BRACKET ID R_R_BRACKET */
  {   83,   -1 }, /* (62) postfix_operand ::= primary_operand */
  {   83,   -4 }, /* (63) postfix_operand ::= postfix_operand L_S_BRACKET INTEGER R_S_BRACKET */
  {   83,   -3 }, /* (64) postfix_operand ::= postfix_operand DOT ID */
  {   83,   -3 }, /* (65) postfix_operand ::= postfix_operand ARROW ID */
  {   86,   -1 }, /* (66) scalar_operand_int ::= INTEGER */
  {   87,   -1 }, /* (67) multiplicative_operand ::= scalar_operand_int */
  {   87,   -3 }, /* (68) multiplicative_operand ::= multiplicative_operand ASTERIX scalar_operand */
  {   87,   -3 }, /* (69) multiplicative_operand ::= multiplicative_operand SLASH scalar_operand */
  {   87,   -3 }, /* (70) multiplicative_operand ::= multiplicative_operand PERCENT scalar_operand */
  {   88,   -1 }, /* (71) additive_operand ::= multiplicative_operand */
  {   88,   -3 }, /* (72) additive_operand ::= additive_operand PLUS multiplicative_operand */
  {   88,   -3 }, /* (73) additive_operand ::= additive_operand MINUS multiplicative_operand */
  {   89,   -1 }, /* (74) shift_operand ::= additive_operand */
  {   89,   -3 }, /* (75) shift_operand ::= shift_operand DOUBLE_LESS_THAN additive_operand */
  {   89,   -3 }, /* (76) shift_operand ::= shift_operand DOUBLE_GREATER_THAN additive_operand */
  {   90,   -1 }, /* (77) and_operand ::= shift_operand */
  {   90,   -3 }, /* (78) and_operand ::= and_operand AMPERSAND shift_operand */
  {   91,   -1 }, /* (79) exclusive_or_operand ::= and_operand */
  {   91,   -3 }, /* (80) exclusive_or_operand ::= exclusive_or_operand CIRCUMFLEX and_operand */
  {   92,   -1 }, /* (81) inclusive_or_operand ::= exclusive_or_operand */
  {   92,   -3 }, /* (82) inclusive_or_operand ::= inclusive_or_operand PIPE exclusive_or_operand */
  {   84,   -1 }, /* (83) scalar_operand ::= inclusive_or_operand */
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
**
** The yyLookahead and yyLookaheadToken parameters provide reduce actions
** access to the lookahead token (if any).  The yyLookahead will be YYNOCODE
** if the lookahead token has already been consumed.  As this procedure is
** only called from one place, optimizing compilers will in-line it, which
** means that the extra parameters have no performance impact.
*/
static YYACTIONTYPE yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno,       /* Number of the rule by which to reduce */
  int yyLookahead,             /* Lookahead token, or YYNOCODE if none */
  ImplParseTOKENTYPE yyLookaheadToken  /* Value of the lookahead token */
  ImplParseCTX_PDECL                   /* %extra_context */
){
  int yygoto;                     /* The next state */
  YYACTIONTYPE yyact;             /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ImplParseARG_FETCH
  (void)yyLookahead;
  (void)yyLookaheadToken;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    if( yysize ){
      fprintf(yyTraceFILE, "%sReduce %d [%s], go to state %d.\n",
        yyTracePrompt,
        yyruleno, yyRuleName[yyruleno], yymsp[yysize].stateno);
    }else{
      fprintf(yyTraceFILE, "%sReduce %d [%s].\n",
        yyTracePrompt, yyruleno, yyRuleName[yyruleno]);
    }
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      /* The call to yyStackOverflow() above pops the stack until it is
      ** empty, causing the main parser loop to exit.  So the return value
      ** is never used and does not matter. */
      return 0;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        /* The call to yyStackOverflow() above pops the stack until it is
        ** empty, causing the main parser loop to exit.  So the return value
        ** is never used and does not matter. */
        return 0;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
      default:
      /* (0) package ::= package_opt END */ yytestcase(yyruleno==0);
      /* (1) package_opt ::= */ yytestcase(yyruleno==1);
      /* (2) package_opt ::= package_opt function_def */ yytestcase(yyruleno==2);
      /* (3) package_opt ::= package_opt var_inst */ yytestcase(yyruleno==3);
      /* (4) package_opt ::= package_opt var_decl */ yytestcase(yyruleno==4);
      /* (5) package_opt ::= package_opt struct_def */ yytestcase(yyruleno==5);
      /* (6) struct_def ::= struct_def_opt R_C_BRACKET */ yytestcase(yyruleno==6);
      /* (7) struct_def_opt ::= ID L_C_BRACKET var_decl */ yytestcase(yyruleno==7);
      /* (8) struct_def_opt ::= struct_def_opt var_decl */ yytestcase(yyruleno==8);
      /* (9) function_def ::= function_def_opt block_statement */ yytestcase(yyruleno==9);
      /* (10) function_def ::= ID function_type block_statement */ yytestcase(yyruleno==10);
      /* (11) function_def_opt ::= ID function_type PIPE var_decl */ yytestcase(yyruleno==11);
      /* (12) function_def_opt ::= ID function_type PIPE var_inst */ yytestcase(yyruleno==12);
      /* (13) function_def_opt ::= function_def_opt COMMA var_decl */ yytestcase(yyruleno==13);
      /* (14) function_def_opt ::= function_def_opt COMMA var_inst */ yytestcase(yyruleno==14);
      /* (15) var_decl ::= ID COLON type */ yytestcase(yyruleno==15);
      /* (16) var_inst ::= var_decl literal */ yytestcase(yyruleno==16);
      /* (17) function_type ::= function_type_opt R_R_BRACKET */ yytestcase(yyruleno==17);
      /* (18) function_type ::= L_R_BRACKET R_R_BRACKET */ yytestcase(yyruleno==18);
      /* (19) function_type_opt ::= L_R_BRACKET var_decl */ yytestcase(yyruleno==19);
      /* (20) function_type_opt ::= function_type_opt COMMA var_decl */ yytestcase(yyruleno==20);
      /* (21) type ::= type ASTERIX */ yytestcase(yyruleno==21);
      /* (22) type ::= L_S_BRACKET type COMMA INTEGER R_S_BRACKET */ yytestcase(yyruleno==22);
      /* (23) type ::= U8 */ yytestcase(yyruleno==23);
      /* (24) type ::= U16 */ yytestcase(yyruleno==24);
      /* (25) type ::= U32 */ yytestcase(yyruleno==25);
      /* (26) type ::= U64 */ yytestcase(yyruleno==26);
      /* (27) type ::= I8 */ yytestcase(yyruleno==27);
      /* (28) type ::= I16 */ yytestcase(yyruleno==28);
      /* (29) type ::= I32 */ yytestcase(yyruleno==29);
      /* (30) type ::= I64 */ yytestcase(yyruleno==30);
      /* (31) type ::= F32 */ yytestcase(yyruleno==31);
      /* (32) type ::= F64 */ yytestcase(yyruleno==32);
      /* (33) type ::= function_type (OPTIMIZED OUT) */ assert(yyruleno!=33);
      /* (34) type ::= struct_def (OPTIMIZED OUT) */ assert(yyruleno!=34);
      /* (35) block_statement ::= block_statement_opt R_C_BRACKET */ yytestcase(yyruleno==35);
      /* (36) block_statement_opt ::= L_C_BRACKET */ yytestcase(yyruleno==36);
      /* (37) block_statement_opt ::= block_statement_opt statement */ yytestcase(yyruleno==37);
      /* (38) statement ::= SEMICOLON */ yytestcase(yyruleno==38);
      /* (39) statement ::= var_decl */ yytestcase(yyruleno==39);
      /* (40) statement ::= var_inst (OPTIMIZED OUT) */ assert(yyruleno!=40);
      /* (41) statement ::= u_mnemonic operand */ yytestcase(yyruleno==41);
      /* (42) statement ::= d_mnemonic operand COMMA operand */ yytestcase(yyruleno==42);
      /* (43) u_mnemonic ::= PUSH */ yytestcase(yyruleno==43);
      /* (44) d_mnemonic ::= MOV */ yytestcase(yyruleno==44);
      /* (45) d_mnemonic ::= ADD */ yytestcase(yyruleno==45);
      /* (46) literal ::= literal_opt R_C_BRACKET */ yytestcase(yyruleno==46);
      /* (47) literal_opt ::= L_C_BRACKET INTEGER */ yytestcase(yyruleno==47);
      /* (48) literal_opt ::= L_C_BRACKET FLOAT */ yytestcase(yyruleno==48);
      /* (49) literal_opt ::= L_C_BRACKET STRING */ yytestcase(yyruleno==49);
      /* (50) literal_opt ::= L_C_BRACKET ID */ yytestcase(yyruleno==50);
      /* (51) literal_opt ::= literal_opt COMMA INTEGER */ yytestcase(yyruleno==51);
      /* (52) literal_opt ::= literal_opt COMMA FLOAT */ yytestcase(yyruleno==52);
      /* (53) literal_opt ::= literal_opt COMMA STRING */ yytestcase(yyruleno==53);
      /* (54) literal_opt ::= literal_opt COMMA ID */ yytestcase(yyruleno==54);
      /* (55) operand ::= postfix_operand */ yytestcase(yyruleno==55);
      /* (56) operand ::= STRING */ yytestcase(yyruleno==56);
      /* (57) operand ::= FLOAT */ yytestcase(yyruleno==57);
      /* (58) operand ::= scalar_operand (OPTIMIZED OUT) */ assert(yyruleno!=58);
      /* (59) operand ::= L_S_BRACKET scalar_operand R_S_BRACKET */ yytestcase(yyruleno==59);
      /* (60) primary_operand ::= ID */ yytestcase(yyruleno==60);
      /* (61) primary_operand ::= CAST LESS_THAN type GREATER_THAN L_R_BRACKET ID R_R_BRACKET */ yytestcase(yyruleno==61);
      /* (62) postfix_operand ::= primary_operand (OPTIMIZED OUT) */ assert(yyruleno!=62);
      /* (63) postfix_operand ::= postfix_operand L_S_BRACKET INTEGER R_S_BRACKET */ yytestcase(yyruleno==63);
      /* (64) postfix_operand ::= postfix_operand DOT ID */ yytestcase(yyruleno==64);
      /* (65) postfix_operand ::= postfix_operand ARROW ID */ yytestcase(yyruleno==65);
      /* (66) scalar_operand_int ::= INTEGER */ yytestcase(yyruleno==66);
      /* (67) multiplicative_operand ::= scalar_operand_int (OPTIMIZED OUT) */ assert(yyruleno!=67);
      /* (68) multiplicative_operand ::= multiplicative_operand ASTERIX scalar_operand */ yytestcase(yyruleno==68);
      /* (69) multiplicative_operand ::= multiplicative_operand SLASH scalar_operand */ yytestcase(yyruleno==69);
      /* (70) multiplicative_operand ::= multiplicative_operand PERCENT scalar_operand */ yytestcase(yyruleno==70);
      /* (71) additive_operand ::= multiplicative_operand */ yytestcase(yyruleno==71);
      /* (72) additive_operand ::= additive_operand PLUS multiplicative_operand */ yytestcase(yyruleno==72);
      /* (73) additive_operand ::= additive_operand MINUS multiplicative_operand */ yytestcase(yyruleno==73);
      /* (74) shift_operand ::= additive_operand */ yytestcase(yyruleno==74);
      /* (75) shift_operand ::= shift_operand DOUBLE_LESS_THAN additive_operand */ yytestcase(yyruleno==75);
      /* (76) shift_operand ::= shift_operand DOUBLE_GREATER_THAN additive_operand */ yytestcase(yyruleno==76);
      /* (77) and_operand ::= shift_operand */ yytestcase(yyruleno==77);
      /* (78) and_operand ::= and_operand AMPERSAND shift_operand */ yytestcase(yyruleno==78);
      /* (79) exclusive_or_operand ::= and_operand */ yytestcase(yyruleno==79);
      /* (80) exclusive_or_operand ::= exclusive_or_operand CIRCUMFLEX and_operand */ yytestcase(yyruleno==80);
      /* (81) inclusive_or_operand ::= exclusive_or_operand */ yytestcase(yyruleno==81);
      /* (82) inclusive_or_operand ::= inclusive_or_operand PIPE exclusive_or_operand */ yytestcase(yyruleno==82);
      /* (83) scalar_operand ::= inclusive_or_operand */ yytestcase(yyruleno==83);
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  yymsp += yysize+1;
  yypParser->yytos = yymsp;
  yymsp->stateno = (YYACTIONTYPE)yyact;
  yymsp->major = (YYCODETYPE)yygoto;
  yyTraceShift(yypParser, yyact, "... then shift");
  return yyact;
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ImplParseARG_FETCH
  ImplParseCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  ImplParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ImplParseCTX_STORE
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ImplParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ImplParseARG_FETCH
  ImplParseCTX_FETCH
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
#line 63 "./parser/grammar.y"

    printf("syntax error\n");
#line 1304 "./parser/out/grammar.c"
/************ End %syntax_error code ******************************************/
  ImplParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ImplParseCTX_STORE
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ImplParseARG_FETCH
  ImplParseCTX_FETCH
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
  ImplParseARG_STORE /* Suppress warning about unused %extra_argument variable */
  ImplParseCTX_STORE
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ImplParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void ImplParse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ImplParseTOKENTYPE yyminor       /* The value for the token */
  ImplParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  YYACTIONTYPE yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser = (yyParser*)yyp;  /* The parser */
  ImplParseCTX_FETCH
  ImplParseARG_STORE

  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif

  yyact = yypParser->yytos->stateno;
#ifndef NDEBUG
  if( yyTraceFILE ){
    if( yyact < YY_MIN_REDUCE ){
      fprintf(yyTraceFILE,"%sInput '%s' in state %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact);
    }else{
      fprintf(yyTraceFILE,"%sInput '%s' with pending reduce %d\n",
              yyTracePrompt,yyTokenName[yymajor],yyact-YY_MIN_REDUCE);
    }
  }
#endif

  do{
    assert( yyact==yypParser->yytos->stateno );
    yyact = yy_find_shift_action((YYCODETYPE)yymajor,yyact);
    if( yyact >= YY_MIN_REDUCE ){
      yyact = yy_reduce(yypParser,yyact-YY_MIN_REDUCE,yymajor,
                        yyminor ImplParseCTX_PARAM);
    }else if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,(YYCODETYPE)yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      break;
    }else if( yyact==YY_ACCEPT_ACTION ){
      yypParser->yytos--;
      yy_accept(yypParser);
      return;
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
      if( yymajor==YYNOCODE ) break;
      yyact = yypParser->yytos->stateno;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      break;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      break;
#endif
    }
  }while( yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}

/*
** Return the fallback token corresponding to canonical token iToken, or
** 0 if iToken has no fallback.
*/
int ImplParseFallback(int iToken){
#ifdef YYFALLBACK
  if( iToken<(int)(sizeof(yyFallback)/sizeof(yyFallback[0])) ){
    return yyFallback[iToken];
  }
#else
  (void)iToken;
#endif
  return 0;
}
