// expr.h : header file
//
#include "hformulapi.h"
char*  GetErrText();
void GetErrPos( int& nLine, int& nPos );

int yyparse( void );
bool _compile_formula( const char* pszFormulaExpr, FORMULA* formula, QList<ITEMDATA*>* pList, bool check );
