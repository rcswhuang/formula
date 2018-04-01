// expr.h : header file
//

const char  GetErrText();
void GetErrPos( int& nLine, int& nPos );

int yyparse( void );
BOOL _compile_formula( const char* pszFormulaExpr, FORMULA* formula, CPtrList* pList, BOOL check );
