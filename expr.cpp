
#include <stdio.h>
#include <ctype.h>
#include <string.h>

//#include "datafile.h"
#include "hformulapi.h"
//#include "formula.h"
#include "expr.h"


#define YYSTYPE ITEM
#define BUFSIZE 50

extern LPFORMULAPROC m_lpFormulaProc;
extern HSTTYPEINFO HstTypeInfo[];

char szErrText[128];
const char* szExpr;
int k;

CPtrList* pItemList;
ITEMDATA* pItemData;

POSITION pos;
BOOL bCheck;

FORMULA* pFormula;
int len;

extern ATTRINFO AnaAttrInfo[];
extern ATTRINFO DgtAttrInfo[];
extern ATTRINFO PulAttrInfo[];
#define ANA 257
#define PUL 258
#define DGT 259
#define DGTA 260
#define TIME 261
#define STEP 262
#define NUMBER 263
#define GEQUAL 264
#define LEQUAL 265
#define NEQUAL 266
#define XOR 267
#define MAX 268
#define MIN 269
#define AVE 270
#define SUM 271
#define MAXT 272
#define MINT 273
#define ERR 274
#define UMINUS 275
YYSTYPE yylval, yyval;
#define YYERRCODE 256
  /* start of programs */
extern const char* GetDevName(WORD wStation,BYTE btEquType, WORD wEquNo, WORD wProtNo);

void yyerror( char* szError )
{
	sprintf( szErrText,"%s near or at position %d", szError, k );
}

const char  GetErrText()
{
	return szErrText; 
}

void GetErrPos( int& nLine, int& nPos )
{
	nLine = 0;
	nPos = k; 
} 

BOOL CheckMin( ITEM* pItem )
{
	float fValue = pItem->fValue;
	if ( ITEM_TIME == pItem->btType )
	{
		if ( fValue > 127 || fValue < -128 )
			return FALSE;

		pItem->ItemTime.min = (char)fValue;
		pItem->ItemTime.flag = ITEMTIME_MIN;
	}
	else
	{
		if ( fValue < 0 || fValue > 59 )
			return FALSE;

		pItem->ItemTime.min = (char)fValue;
		pItem->ItemTime.flag = 0;
	}

	return TRUE;
}

BOOL CheckHour( ITEM* pItem )
{
	float fValue = pItem->fValue;
	if ( ITEM_TIME == pItem->btType )
	{
		if ( fValue > 127 || fValue < -128 )
			return FALSE;

		pItem->ItemTime.hour = (char)fValue;
		pItem->ItemTime.flag = ITEMTIME_HOUR;
	}
	else
	{
		if ( fValue < 0 || fValue > 23 )
			return FALSE;

		pItem->ItemTime.hour = (char)fValue;
		pItem->ItemTime.flag = 0;
	}

	return TRUE;
}

BOOL CheckDay( ITEM* pItem )
{
	float fValue = pItem->fValue;
	if ( ITEM_TIME == pItem->btType )
	{
		if ( fValue > 127 || fValue < -128 )
			return FALSE;

		pItem->ItemTime.day = (char)fValue;
		pItem->ItemTime.flag = ITEMTIME_DAY;
	}
	else
	{
		if ( fValue < 1 || fValue > 31 )
			return FALSE;

		pItem->ItemTime.day = (char)fValue;
		pItem->ItemTime.flag = 0;
	}

	return TRUE;
}

BOOL CheckMon( ITEM* pItem )
{
	float fValue = pItem->fValue;
	if ( ITEM_TIME == pItem->btType )
	{
		if ( fValue > 127 || fValue < -128 )
			return FALSE;

		pItem->ItemTime.mon = (char)fValue;
		pItem->ItemTime.flag = ITEMTIME_MON;
	}
	else
	{
		if ( fValue < 1 || fValue > 12 )
			return FALSE;

		pItem->ItemTime.mon = (char)( fValue - 1 );
		pItem->ItemTime.flag = 0;
	}

	return TRUE;
}

BOOL CheckYear( ITEM* pItem )
{
	float fValue = pItem->fValue;
	if ( ITEM_TIME == pItem->btType )
	{
		if ( fValue > 127 || fValue < -128 )
			return FALSE;

		pItem->ItemTime.year = (char)fValue;
		pItem->ItemTime.flag = ITEMTIME_YEAR;
	}
	else
	{
		if ( fValue < 1900 || fValue > 2155 )
			return FALSE;

		pItem->ItemTime.year = (char)( fValue - 1900 );
		pItem->ItemTime.flag = 0;
	}

	return TRUE;
}

BOOL GetFormulaItem( char* pStName, char* pPtName, char* pAttrName, ITEM* pItem )
{
	STATION station;
	if ( NULL == m_lpFormulaProc )
		return FALSE;

	FORMULAPARAMETER Param;
	Param.wStation = (WORD)-1;
	Param.btType = TYPE_NULL;
	Param.pBuffer = &station;

	for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
	{
		if ( !strcmp( pStName, station.szStationName ) )
		{
			pItem->DbWord.wStation = station.wStationID;
			Param.wStation = station.wStationID;

			ANALOGUE analogue;
			DIGITAL digital;
			PULSE pulse;

			Param.pBuffer = &analogue;
			Param.btType = TYPE_ANALOGUE;

			for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
			{
				char szTemp[PTNAMELEN+EQUNAMELEN+1];
				sprintf(szTemp,"%s",analogue.szName);
				if ( !strcmp( pPtName, szTemp ) )
				{
					pItem->btType = ITEM_ANALOGUE;
					pItem->DbWord.wPoint = analogue.wNo;
					pItem->DbWord.wAttrib = ATTR_ANA_VALUE;

					if ( pAttrName != NULL && strlen( pAttrName ) > 0 )
						for ( int i = 0; AnaAttrInfo[i].szAttrib != NULL; i++ )
							if ( !strcmp( AnaAttrInfo[i].szAttrib, pAttrName ) )
								pItem->DbWord.wAttrib = AnaAttrInfo[i].wAttrib;

					return TRUE;
				}
			}

			Param.pBuffer = &digital;
			Param.btType = TYPE_DIGITAL;

			for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
			{
				char szTemp[PTNAMELEN+EQUNAMELEN+1];
				sprintf(szTemp,"%s",digital.szName);
				if ( !strcmp( pPtName, szTemp ) )
				{
					pItem->btType = ITEM_DIGITAL;
					pItem->DbWord.wPoint = digital.wNo;
					pItem->DbWord.wAttrib = ATTR_DGT_VALUE;

					if ( pAttrName != NULL && strlen( pAttrName ) > 0 )
						for ( int i = 0; DgtAttrInfo[i].szAttrib != NULL; i++ )
							if ( !strcmp( DgtAttrInfo[i].szAttrib, pAttrName ) )
								pItem->DbWord.wAttrib = DgtAttrInfo[i].wAttrib;

					return TRUE;
				}
			}

			Param.pBuffer = &pulse;
			Param.btType = TYPE_PULSE;

			for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
			{
				PROTECTDEVICEINFO protdevice;
				FORMULAPARAMETER Param1;
				Param1.wStation = station.wStationID;
				Param1.btType = TYPE_PROTECT;
				Param1.pBuffer = &protdevice;

				for ( Param1.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param1 ); Param1.wPoint++ )
				{
					protdevice.szProtectName[EQUNAMELEN-1] = 0;
					if (protdevice.wProtectID==pulse.wProtID)
						break;
				}
				char szTemp[PTNAMELEN+EQUNAMELEN+1];
				sprintf(szTemp,"%s",pulse.szName);
				if ( !strcmp( pPtName, szTemp ) )
				{
					pItem->btType = ITEM_PULSE;
					pItem->DbWord.wPoint = pulse.wNo;
					pItem->DbWord.wAttrib = ATTR_PUL_COUNTERVALUE;

					if ( pAttrName != NULL && strlen( pAttrName ) > 0 )
						for ( int i = 0; PulAttrInfo[i].szAttrib != NULL; i++ )
							if ( !strcmp( PulAttrInfo[i].szAttrib, pAttrName ) )
								pItem->DbWord.wAttrib = PulAttrInfo[i].wAttrib;

					return TRUE;
				}
			}

			break;
		}
	}

	return FALSE;
}

BOOL GetFormulaItemForGIN( char* pStName/*厂站名*/, char* pPtName/*装置名*/, char* pAttrName/*测点名*/, ITEM* pItem )
{
	if ( NULL == m_lpFormulaProc )
		return FALSE;

	//station;
	STATION station;
	FORMULAPARAMETER Param;
	Param.wStation = (WORD)-1;
	Param.btType = TYPE_NULL;
	Param.pBuffer = &station;

	for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
	{
		if ( !strcmp( pStName, station.szStationName ) )
		{
			pItem->DbWord.wStation = station.wStationID;
			Param.wStation = station.wStationID;
			
			//device
			PROTECTDEVICEINFO protdevice;
			FORMULAPARAMETER Param1;
			Param1.wStation = station.wStationID;
			Param1.btType = TYPE_PROTECT;
			Param1.pBuffer = &protdevice;

			for ( Param1.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param1 ); Param1.wPoint++ )
			{
				if ( !strcmp( pPtName, protdevice.szProtectName ) )
				{

					ANALOGUE analogue;
					DIGITAL digital;
					PULSE pulse;

					Param.pBuffer = &analogue;
					Param.wAttrib = protdevice.wProtectID;
					Param.btType = TYPE_ANALOGUE;
					for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
					{
						char szTemp[PTNAMELEN+EQUNAMELEN+1];
						sprintf(szTemp,"%s",analogue.szName);
						if ( !strcmp( pAttrName, szTemp ) )
						{
							pItem->btType = ITEM_ANALOGUE;
							pItem->DbWord.wStation = station.wStationID;
							pItem->DbWord.wPoint = analogue.wNo;
							pItem->DbWord.wAttrib = protdevice.wProtectID;
							return TRUE;
						}
					}

					Param.pBuffer = &digital;
					Param.btType = TYPE_DIGITAL;

					for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
					{
						char szTemp[PTNAMELEN+EQUNAMELEN+1];
						sprintf(szTemp,"%s",digital.szName);
						if ( !strcmp( pAttrName, szTemp ) )
						{
							pItem->btType = ITEM_DIGITAL;
							pItem->DbWord.wPoint = digital.wNo;
							pItem->DbWord.wAttrib = protdevice.wProtectID;

							return TRUE;
						}
					}

					Param.pBuffer = &pulse;
					Param.btType = TYPE_PULSE;

					for ( Param.wPoint = 0; m_lpFormulaProc( FM_FINDDBINFO, 0, (LPARAM)&Param ); Param.wPoint++ )
					{
						char szTemp[PTNAMELEN+EQUNAMELEN+1];
						sprintf(szTemp,"%s",pulse.szName);
						if ( !strcmp( pAttrName, szTemp ) )
						{
							pItem->btType = ITEM_PULSE;
							pItem->DbWord.wPoint = pulse.wNo;
							pItem->DbWord.wAttrib = protdevice.wProtectID;

							return TRUE;
						}
					}
				}
			}

		}
	}
	return FALSE;
}

void SkipBlank()
{
	/* skip blanks and tabs */
	while ( ' ' == szExpr[ k ] || '\t' == szExpr[ k ] )
		k++;
}

BOOL GetDbItem( ITEM* pItem )
{
	SkipBlank();
	if ( HEADER_SIGN != szExpr[ k ] )
		return FALSE;

	const char *p = szExpr + k, *q, *r;
	char szStName[STNAMELEN], szPtName[PTNAMELEN+EQUNAMELEN+1], szAttrName[MAXATTRNAMELEN] = "";

	q = strchr( p + 1, '.' );
	if ( NULL == q || q - p > STNAMELEN )
		return FALSE;

	strncpy( szStName, p + 1, q - p - 1 );
	szStName[ q - p - 1 ] = '\0';

	r = strchr( q + 1, TAILER_SIGN );
	const char* temp = strchr(q + 1,HEADER_SIGN);
	if ((temp!=NULL) && (temp<r))
	{
		r = strchr(r + 1,TAILER_SIGN);
	}

	if ( NULL == r )
		return FALSE;

	char* pattr = strchr( q + 1, '.' );
	if ( pattr != NULL )
	{
		if ( pattr - q > PTNAMELEN || r - pattr > MAXATTRNAMELEN )
			return FALSE;

		strncpy( szPtName, q + 1, pattr - q - 1 );
		szPtName[ pattr - q - 1 ] = '\0';

		strncpy( szAttrName, pattr + 1, r - pattr - 1 );
		szAttrName[ r - pattr - 1 ] = '\0';
	}
	else
	{
		if ( r - q > PTNAMELEN )
			return FALSE;

		strncpy( szPtName, q + 1, r - q - 1 );
		szPtName[ r - q - 1 ] = '\0';
	}
	//{{Add for GIN
	if(m_pFormulaModule == MODULE_DEVGIN)
	{
		if ( !GetFormulaItemForGIN( szStName, szPtName, szAttrName, pItem ) )
			return FALSE;
	}
	else
	{
		if ( !GetFormulaItem( szStName, szPtName, szAttrName, pItem ) )
			return FALSE;
	}

	k += r - p + 1;
	return TRUE;
}

//对数字量构建一个ITEM，注意数字都是FLOAT
BOOL GetNumberItem( ITEM* pItem )
{
	SkipBlank();
	if ( !isdigit( szExpr[ k ] ) && '.' != szExpr[ k ] )
		return FALSE;

	char buf[ BUFSIZE + 1 ], *cp = buf;
	int dot = 0;

	for ( ; ( cp - buf ) < BUFSIZE; ++cp, ++k )
	{
		*cp = szExpr[ k ];

		if ( isdigit( szExpr[ k ] ) )
			continue;

		if ( '.' == szExpr[ k ] )
		{
			if ( dot++ )
				return FALSE;

			continue;
		}

		break;
	}

	*cp = '\0';
	pItem->btType = ITEM_FLOAT;

	TRY {
		pItem->fValue = (float)atof( buf );
	}
	CATCH(CException, e)
	{
		return FALSE;
	}
	END_CATCH

	if ( pItem->fValue < 0 )
		return FALSE;

	SkipBlank();
	return TRUE;
}

BOOL GetExtendNumberItem( ITEM* pItem )
{
	SkipBlank();
	if ( isdigit( szExpr[ k ] ) || '.' == szExpr[ k ] )
		return GetNumberItem( pItem );

	if ( '$' == szExpr[ k++ ] )
	{
		SkipBlank();
		pItem->fValue = 0.0f;

		char ch = szExpr[ k ];
		if ( '+' == ch || '-' == ch )
		{
			k = k + 1;
			if ( !GetNumberItem( pItem ) )
				return FALSE;
		}

		if ( '-' == ch )
			pItem->fValue = -pItem->fValue;

		pItem->btType = ITEM_TIME;
		SkipBlank();

		return TRUE;
	}

	return FALSE;
}

BOOL GetTimeItem( ITEM* pItem )
{
	ITEM itemYear, itemMon, itemDay, itemHour, itemMin;
	if ( '[' != szExpr[ k++ ] || !GetExtendNumberItem( &itemYear ) ||
		 '/' != szExpr[ k++ ] || !GetExtendNumberItem( &itemMon )  ||
		 '/' != szExpr[ k++ ] || !GetExtendNumberItem( &itemDay )  ||
		 ',' != szExpr[ k++ ] || !GetExtendNumberItem( &itemHour ) ||
		 ':' != szExpr[ k++ ] || !GetExtendNumberItem( &itemMin )  ||
		 ']' != szExpr[ k++ ] )
		 return FALSE;

	if ( !CheckYear( &itemYear ) || !CheckMon( &itemMon ) || !CheckDay( &itemDay ) ||
		 !CheckHour( &itemHour ) || !CheckMin( &itemMin ) )
		 return FALSE;

	pItem->btType = ITEM_TIME;
	pItem->ItemTime.year = itemYear.ItemTime.year;
	pItem->ItemTime.mon = itemMon.ItemTime.mon;
	pItem->ItemTime.day = itemDay.ItemTime.day;
	pItem->ItemTime.hour = itemHour.ItemTime.hour;
	pItem->ItemTime.min = itemMin.ItemTime.min;
	pItem->ItemTime.flag = itemYear.ItemTime.flag |
		itemMon.ItemTime.flag | itemDay.ItemTime.flag |
		itemHour.ItemTime.flag | itemMin.ItemTime.flag;

	return TRUE;
}

yylex()    /* lexical analysis routine */
{
	SkipBlank();
	if ( pItemData != NULL && k == pItemData->nStartPos )
	{
		k = pItemData->nEndPos + 1;
		pItemData = ( NULL == pos ) ? NULL : (ITEMDATA*)pItemList->GetNext( pos );

		yylval = pItemData->item;
		switch ( pItemData->item.btType )
		{
			case ITEM_ANALOGUE:
				return ANA;
			case ITEM_DIGITAL:
				return DGT;
			case ITEM_PULSE:
				return PUL;
			default:
				ASSERT( FALSE );
				return ERR;
		}
	}

	if ( HEADER_SIGN == szExpr[ k ] )
	{
		ITEM item;
		if ( GetDbItem( &item ) )
		{
			yylval = item;
			yylval.btReserve = (BYTE)-1;

			switch ( item.btType )
			{
				case ITEM_ANALOGUE:
					return ANA;

				case ITEM_DIGITAL:
					if ( ATTR_DGT_VALUE == item.DbWord.wAttrib )
						return DGT;
					else
						return DGTA;

				case ITEM_PULSE:
					return PUL;

				default:
					ASSERT( FALSE );
			}

			return ERR;
		}

		if ( '[' != HEADER_SIGN )
			return ERR;
	}

	if ( '[' == szExpr[ k ] )
	{
		const char *p = strchr( szExpr + k, ']' );
		if ( NULL == p )
			return ERR;

		int nLen = p - szExpr - k - 1;
		for ( int i = 0; HstTypeInfo[i].szFace != NULL; i++ )
		{
			if ( nLen >= HstTypeInfo[i].nMinLen && nLen <= strlen( HstTypeInfo[i].szFace ) )
			{
				char szFace[32];
				strcpy( szFace, HstTypeInfo[i].szFace );
				szFace[nLen] = 0;

				if ( !_strnicmp( szFace, szExpr + k + 1, nLen ) )
				{
					yylval.btReserve = HstTypeInfo[i].btHst;
					k += nLen + 2;
					return STEP;
				}
			}
		}

		ITEM item;
		if ( GetTimeItem( &item ) )
		{
			yylval = item;
			return TIME;
		}

		return ERR;
	}

	if ( !_strnicmp( szExpr + k, "MAXT", 4 ) )
	{
		k += 4;
		return MAXT;
	}

	if ( !_strnicmp( szExpr + k, "MINT", 4 ) )
	{
		k += 4;
		return MINT;
	}

	if ( !_strnicmp( szExpr + k, "MAX", 3 ) )
	{
		k += 3;
		return MAX;
	}

	if ( !_strnicmp( szExpr + k, "MIN", 3 ) )
	{
		k += 3;
		return MIN;
	}

	if ( !_strnicmp( szExpr + k, "AVE", 3 ) )
	{
		k += 3;
		return AVE;
	}

	if ( !_strnicmp( szExpr + k, "SUM", 3 ) )
	{
		k += 3;
		return SUM;
	}

	if ( !_strnicmp( szExpr + k, ">=", 2 ) )
	{
		k += 2;
		return GEQUAL;
	}

	if ( !_strnicmp( szExpr + k, "<=", 2 ) )
	{
		k += 2;
		return LEQUAL;
	}

	if ( !_strnicmp( szExpr + k, "!=", 2 ) )
	{
		k += 2;
		return NEQUAL;
	}

	if ( !_strnicmp( szExpr + k, "XOR", 3 ) )
	{
		k += 3;
		return XOR;
	}

	if ( isdigit( szExpr[ k ] ) || '.' == szExpr[ k ] )
	{
		if ( !GetNumberItem( &yylval ) )
			return ERR;

		return NUMBER;
	}

	return szExpr[ k++ ];
}

int Action1( WORD wOp )
{
	if ( len >= FORMULALEN )
	{
		yyerror( "Formula is too long" );
		return 1;
	}

	if ( !bCheck )
		pFormula->wFormula[ len++ ] = wOp;
	else
		len++;

	return 0;
}

int Action2( ITEM& item )
{
	if ( len >= FORMULALEN )
	{
		yyerror( "Formula is too long" );
		return 1;
	}

	if ( !bCheck )
	{
		ITEM* pItem = AddItem( &item );
		if ( NULL == pItem )
		{
			yyerror( "Failed to add item" );
			return 1;
		}

		pFormula->wFormula[ len++ ] = pItem->wNo;
	}
	else
		len++;

	return 0;
}

BOOL _compile_formula( const char* pszFormulaExpr, FORMULA* formula, CPtrList* pList, BOOL check )
{
	ASSERT( pszFormulaExpr != NULL );
	ASSERT( formula != NULL );

	szExpr = pszFormulaExpr;
	k = 0;

	pItemList = pList;
	pItemData = NULL;

	if ( pItemList != NULL )
	{
		pos = pItemList->GetHeadPosition();
		if ( pos != NULL )
			pItemData = (ITEMDATA*)pItemList->GetNext( pos );
	}

	pFormula = formula; len = 0;
	memset( pFormula->wFormula, 0, FORMULALEN * sizeof(WORD) );

	bCheck = check;
	if ( yyparse() )
		return FALSE;

	return TRUE;
}
FILE *yytfilep;
char *yytfilen;
int yytflag = 0;
const int yyexca[] = {
  -1, 1,
  0, -1,
  -2, 0,
  0,
};

#define YYNPROD 67
#define YYLAST 361

const int yyact[] = {
      16,      20,      36,      17,     126,      18,     102,     103,
     125,      42,     120,      39,      21,      22,      34,      42,
      42,     116,      16,      35,     107,      17,     114,      18,
     112,     107,      39,     107,     106,      99,      42,     107,
     117,      16,      37,      37,      17,     115,      18,      29,
      89,     113,      57,      34,      32,     110,      33,      29,
      35,      37,     100,      34,      32,      39,      33,     111,
      35,      42,      88,     105,      56,      24,      25,      23,
      48,      47,      36,      46,      29,      24,      25,      23,
      34,      32,      29,      33,      37,      35,      34,      32,
      45,      33,      15,      35,       1,       0,      19,      94,
      96,      98,      24,      25,      23,       0,       0,      36,
      24,      25,      23,       0,       0,       0,       0,      36,
      19,      93,      95,      97,      34,      32,       0,      33,
      40,      35,       0,     101,     104,      34,      32,      19,
      33,       0,      35,       0,      36,      30,      24,      25,
      23,       0,      36,       0,     127,      30,       0,      24,
       0,      23,       0,      40,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,      30,       0,       0,       0,       0,       0,
      36,      92,       3,       0,       0,       0,       0,       0,
       0,      36,       0,       0,       0,       0,       0,       0,
       0,       0,      50,      51,      53,      55,       0,       0,
       0,      58,      59,      60,      62,      63,      64,      67,
      69,      71,      72,      73,      74,      75,      76,      77,
      79,      82,      84,      86,      87,       0,       0,       0,
       0,       0,       0,       0,       0,     122,       5,       0,
       0,       8,      10,       7,       9,      20,      43,       6,
      44,       0,       0,       0,      11,      12,      13,      14,
      21,      22,       2,       8,      10,       7,       9,      38,
      38,       6,       0,       0,       0,       0,      11,      12,
      13,      14,      90,      91,       7,       9,      38,      41,
       6,       0,       0,       0,       0,      11,      12,      13,
      14,      26,      27,      28,      31,     121,       0,       0,
     124,      26,      27,      28,      31,       0,       0,       0,
       0,      38,      41,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,      26,      27,
      28,      31,       0,       0,      26,      27,      28,       0,
     108,     109,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,     118,     119,      52,       4,
       0,       0,       0,     123,       0,       0,       0,       0,
       0,       0,      26,      27,      28,     123,       0,      49,
       0,       0,      54,      26,      27,       0,       0,       0,
      61,       0,       0,      65,      66,      68,      70,       0,
       0,       0,       0,       0,      78,      80,      81,      83,
      85,
};

const int yypact[] = {
     -40,   -1000,   -1000,      30,      15,   -1000,   -1000,   -1000,
    -260,   -1000,    -260,      40,      27,      25,      24,   -1000,
     -22,     -22,     -22,     -22,   -1000,      20,       2,     -22,
     -22,     -22,     -22,     -22,     -22,     -22,     -22,     -22,
     -22,     -22,     -22,     -22,     -22,     -22,     -22,     -22,
     -22,     -22,     -22,   -1000,   -1000,      -7,      -7,      -7,
      -7,     -12,       9,   -1000,      15,   -1000,     -26,      66,
    -251,    -251,      75,      75,      75,     -33,      75,      75,
      75,     -33,     -26,      66,     -27,      36,     -27,      36,
     -28,     -28,     -92,     -92,   -1000,      75,     -33,      75,
     -33,     -26,      66,     -27,      36,     -27,      36,     -92,
      18,     -13,    -260,    -260,       1,      14,     -17,       0,
     -19,      -4,     -24,   -1000,   -1000,      -9,    -260,    -260,
     -31,   -1000,   -1000,     -22,    -260,    -260,     -22,   -1000,
   -1000,   -1000,   -1000,   -1000,   -1000,   -1000,    -260,    -260,
   -1000,      30,    -258,    -258,      30,   -1000,   -1000,   -1000,
};

const int yypgo[] = {
       0,      84,     161,     318,     213,      58,      82,      40,
       8,
};

const int yyr1[] = {
       0,       1,       1,       1,       1,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       3,       3,
       2,       2,       2,       2,       2,       2,       2,       2,
       2,       2,       2,       2,       2,       2,       2,       2,
       2,       2,       2,       2,       2,       2,       2,       5,
       5,       5,       5,       6,       6,       4,       4,       4,
       8,       7,       7,
};

const int yyr2[] = {
       0,       1,       1,       1,       1,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       3,       3,
       3,       3,       3,       3,       3,       3,       2,       2,
       1,       1,       1,       1,       1,       4,       4,       4,
       4,       1,       3,       3,       3,       3,       3,       3,
       3,       2,       2,       4,       4,       4,       4,       3,
       4,       3,       4,       2,       2,       1,       4,       4,
       1,       3,       3,
};

const int yychk[] = {
   -1000,      -1,     274,      -2,      -3,      -4,     263,     259,
     257,     260,     258,     268,     269,     270,     271,      -6,
      40,      43,      45,     126,     261,     272,     273,      62,
      60,      61,     264,     265,     266,      38,     124,     267,
      43,      45,      42,      47,      94,      61,     266,      38,
     124,     267,      42,      -4,      -4,      40,      40,      40,
      40,      -3,      -2,      -2,      -3,      -2,      -3,      -2,
      40,      40,      -2,      -2,      -2,      -3,      -2,      -2,
      -2,      -3,      -3,      -2,      -3,      -2,      -3,      -2,
      -2,      -2,      -2,      -2,      -2,      -2,      -3,      -2,
      -3,      -3,      -2,      -3,      -2,      -3,      -2,      -2,
      -5,      -7,     257,     258,      -2,      -5,      -7,      -5,
      -7,      -5,      -7,      41,      41,      -5,     257,     258,
      -5,      41,      41,      44,      -4,      -4,      44,      41,
      41,      41,      41,      41,      41,      41,      -4,      -4,
      41,      -2,      -4,      -4,      -2,      -8,     262,      -8,
};

const int yydef[] = {
       0,      -2,       1,       2,       3,       4,      32,      33,
      34,      35,      36,       0,       0,       0,       0,      41,
       0,       0,       0,       0,      61,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,       0,       0,       0,       0,       0,
       0,       0,       0,      59,      60,       0,       0,       0,
       0,       0,       0,      49,       0,      50,      30,      31,
       0,       0,       5,       6,       7,       9,      11,      12,
      13,      15,      19,      21,      23,      25,      27,      29,
      44,      45,      46,      47,      48,       8,      10,      14,
      16,      18,      20,      22,      24,      26,      28,      42,
       0,       0,      34,      36,       0,       0,       0,       0,
       0,       0,       0,      17,      43,       0,       0,       0,
       0,      37,      51,       0,      59,      60,       0,      38,
      52,      39,      53,      40,      54,      62,       0,       0,
      63,      66,      55,      57,      65,      56,      64,      58,
};

const int *yyxi;
/*****************************************************************/
/* PCYACC LALR parser driver routine -- a table driven procedure */
/* for recognizing sentences of a language defined by the        */
/* grammar that PCYACC analyzes. An LALR parsing table is then   */
/* constructed for the grammar and the skeletal parser uses the  */
/* table when performing syntactical analysis on input source    */
/* programs. The actions associated with grammar rules are       */
/* inserted into a switch statement for execution.               */
/*****************************************************************/


#ifndef YYMAXDEPTH
#define YYMAXDEPTH 200
#endif
#ifndef YYREDMAX
#define YYREDMAX 1000
#endif
#define PCYYFLAG -1000
#define WAS0ERR 0
#define WAS1ERR 1
#define WAS2ERR 2
#define WAS3ERR 3
#define yyclearin pcyytoken = -1
#define yyerrok   pcyyerrfl = 0
YYSTYPE yyv[YYMAXDEPTH];     /* value stack, ( YYSTYPE explicit or %union ) */
int pcyyerrct = 0;           /* error count */
int pcyyerrfl = 0;           /* error flag */
int redseq[YYREDMAX];
int redcnt = 0;
int pcyytoken = -1;          /* input token */


int yylex( void );


int yyparse( void )
{
  int statestack[YYMAXDEPTH]; /* state stack */
  int      j, m;              /* working index */
  YYSTYPE *yypvt;
  int      tmpstate, *yyps, n;
  YYSTYPE *yypv;
#ifdef YYDEBUG
  int tmptoken = -1;
#endif


  tmpstate = 0;
  pcyytoken = -1;
  pcyyerrct = 0;
  pcyyerrfl = 0;
  yyps = &statestack[-1];
  yypv = &yyv[-1];


  enstack:    /* push stack */
#ifdef YYDEBUG
    printf("at state %d, next token %d\n", tmpstate, tmptoken);
#endif
    if (++yyps - &statestack[YYMAXDEPTH-1] > 0) {
      yyerror("pcyacc internal stack overflow");
      return(1);
    }
    *yyps = tmpstate;
    ++yypv;
    *yypv = yyval;


  newstate:
    n = yypact[tmpstate];
    if (n <= PCYYFLAG) goto defaultact; /*  a simple state */


    if (pcyytoken < 0) if ((pcyytoken=yylex()) < 0) pcyytoken = 0;
    if ((n += pcyytoken) < 0 || n >= YYLAST) goto defaultact;


    if (yychk[n=yyact[n]] == pcyytoken) { /* a shift */
#ifdef YYDEBUG
      tmptoken  = pcyytoken;
#endif
      pcyytoken = -1;
      yyval = yylval;
      tmpstate = n;
      if (pcyyerrfl > 0) --pcyyerrfl;
      goto enstack;
    }


  defaultact:


    if ((n=yydef[tmpstate]) == -2) {
      if (pcyytoken < 0) if ((pcyytoken=yylex())<0) pcyytoken = 0;
      for (yyxi=yyexca; (*yyxi!= (-1)) || (yyxi[1]!=tmpstate); yyxi += 2);
      while (*(yyxi+=2) >= 0) if (*yyxi == pcyytoken) break;
      if ( (n=yyxi[1]) < 0 ) { /* an accept action */
#ifdef YYTFLAG
          yytfilep = fopen(yytfilen, "w");
          if (yytfilep == NULL) {
            fprintf(stderr, "Can't open t file: %s\n", yytfilen);
            return(0);          }
          for (ti=redcnt-1; ti>=0; ti--) {
            tj = svdprd[redseq[ti]];
            while (strcmp(svdnams[tj], "$EOP"))
              fprintf(yytfilep, "%s ", svdnams[tj++]);
            fprintf(yytfilep, "\n");
          }
          fclose(yytfilep);
#endif
        return (0); /* EOF return ok */

      }
    }


    if (n == 0) {        /* error situation */
      switch (pcyyerrfl) {
        case WAS0ERR:          /* an error just occurred */
          yyerror("syntax error");
            ++pcyyerrct;
        case WAS1ERR:
        case WAS2ERR:           /* try again */
          pcyyerrfl = 3;
	   /* find a state for a legal shift action */
          while (yyps >= statestack) {
	     n = yypact[*yyps] + YYERRCODE;
	     if (n >= 0 && n < YYLAST && yychk[yyact[n]] == YYERRCODE) {
	       tmpstate = yyact[n];  /* simulate a shift of "error" */
	       goto enstack;
            }
	     n = yypact[*yyps];


	     /* the current yyps has no shift on "error", pop stack */
#ifdef YYDEBUG
            printf("error: pop state %d, recover state %d\n", *yyps, yyps[-1]);
#endif
	     --yyps;
	     --yypv;
	   }


	   yyabort:
#ifdef YYTFLAG
              yytfilep = fopen(yytfilen, "w");
              if (yytfilep == NULL) {
                fprintf(stderr, "Can't open t file: %s\n", yytfilen);
                return(1);              }
              for (ti=1; ti<redcnt; ti++) {
                tj = svdprd[redseq[ti]];
                while (strcmp(svdnams[tj], "$EOP"))
                  fprintf(yytfilep, "%s ", svdnams[tj++]);
                fprintf(yytfilep, "\n");
              }
              fclose(yytfilep);
#endif
	     return(1);


	 case WAS3ERR:  /* clobber input char */
#ifdef YYDEBUG
          printf("error: discard token %d\n", pcyytoken);
#endif
          if (pcyytoken == 0) goto yyabort; /* quit */
	   pcyytoken = -1;
	   goto newstate;      } /* switch */
    } /* if */


    /* reduction, given a production n */
#ifdef YYDEBUG
    printf("reduce with rule %d\n", n);
#endif
    if (yytflag && redcnt<YYREDMAX) redseq[redcnt++] = n;
    yyps -= yyr2[n];
    yypvt = yypv;
    yypv -= yyr2[n];
    yyval = yypv[1];
    m = n;
    /* find next state from goto table */
    n = yyr1[n];
    j = yypgo[n] + *yyps + 1;
    if (j>=YYLAST || yychk[ tmpstate = yyact[j] ] != -n) tmpstate = yyact[yypgo[n]];
    switch (m) { /* actions associated with grammar rules */
      
      case 1:{ /* expr : ERR */ yyerror( "syntax error" ); return 1; } break;
      case 2:{ /* expr : arithExpr */ } break;
      case 3:{ /* expr : logicExpr */ } break;
      case 4:{ /* expr : itemTime */ } break;
      case 5:{
      				/* logicExpr : arithExpr '>' arithExpr */
      				if ( Action1( OP_GREATER ) )
      					return 1;
      			} break;
      case 6:{
      				/* logicExpr : arithExpr '<' arithExpr */
      				if ( Action1( OP_LOWER ) )
      					return 1;
      			} break;
      case 7:{
      				/* logicExpr : arithExpr '=' arithExpr */
      				if ( Action1( OP_EQUAL ) )
      					return 1;
      			} break;
      case 8:{
      				/* logicExpr : logicExpr '=' arithExpr */
      				if ( Action1( OP_EQUAL ) )
      					return 1;
      			} break;
      case 9:{
      				/* logicExpr : arithExpr '=' logicExpr */
      				if ( Action1( OP_EQUAL ) )
      					return 1;
      			} break;
      case 10:{
      				/* logicExpr : logicExpr '=' logicExpr */
      				if ( Action1( OP_EQUAL ) )
      					return 1;
      			} break;
      case 11:{
      				/* logicExpr : arithExpr GEQUAL arithExpr */
      				if ( Action1( OP_GEQUAL ) )
      					return 1;
      			} break;
      case 12:{
      				/* logicExpr : arithExpr LEQUAL arithExpr */
      				if ( Action1( OP_LEQUAL ) )
      					return 1;
      			} break;
      case 13:{
      				/* logicExpr : arithExpr NEQUAL arithExpr */
      				if ( Action1( OP_NEQUAL ) )
      					return 1;
      			} break;
      case 14:{
      				/* logicExpr : logicExpr NEQUAL arithExpr */
      				if ( Action1( OP_NEQUAL ) )
      					return 1;
      			} break;
      case 15:{
      				/* logicExpr : arithExpr NEQUAL logicExpr */
      				if ( Action1( OP_NEQUAL ) )
      					return 1;
      			} break;
      case 16:{
      				/* logicExpr : logicExpr NEQUAL logicExpr */
      				if ( Action1( OP_NEQUAL ) )
      					return 1;
      			} break;
      case 17:{ /* logicExpr : '(' logicExpr ')' */ } break;
      case 18:{
      				/* logicExpr : logicExpr AND logicExpr */
      				if ( Action1( OP_AND ) )
      					return 1;
      			} break;
      case 19:{
      				/* logicExpr : logicExpr AND logicExpr */
      				if ( Action1( OP_AND ) )
      					return 1;
      			} break;
      case 20:{
      				/* logicExpr : logicExpr AND logicExpr */
      				if ( Action1( OP_AND ) )
      					return 1;
      			} break;
      case 21:{
      				/* logicExpr : logicExpr AND logicExpr */
      				if ( Action1( OP_AND ) )
      					return 1;
      			} break;
      case 22:{
      				/* logicExpr : logicExpr OR logicExpr */
      				if ( Action1( OP_OR ) )
      					return 1;
      			} break;
      case 23:{
      				/* logicExpr : logicExpr OR logicExpr */
      				if ( Action1( OP_OR ) )
      					return 1;
      			} break;
      case 24:{
      				/* logicExpr : logicExpr OR logicExpr */
      				if ( Action1( OP_OR ) )
      					return 1;
      			} break;
      case 25:{
      				/* logicExpr : logicExpr OR logicExpr */
      				if ( Action1( OP_OR ) )
      					return 1;
      			} break;
      case 26:{
      				/* logicExpr : logicExpr XOR logicExpr */
      				if ( Action1( OP_XOR ) )
      					return 1;
      			} break;
      case 27:{
      				/* logicExpr : logicExpr XOR logicExpr */
      				if ( Action1( OP_XOR ) )
      					return 1;
      			} break;
      case 28:{
      				/* logicExpr : logicExpr XOR logicExpr */
      				if ( Action1( OP_XOR ) )
      					return 1;
      			} break;
      case 29:{
      				/* logicExpr : logicExpr XOR logicExpr */
      				if ( Action1( OP_XOR ) )
      					return 1;
      			} break;
      case 30:{
      				/* logicExpr : NOT logicExpr */
      				if ( Action1( OP_NOT ) )
      					return 1;
      			} break;
      case 31:{
      				/* logicExpr : NOT logicExpr */
      				if ( Action1( OP_NOT ) )
      					return 1;
      			} break;
      case 32:{
      				/* arithExpr : NUMBER */
      				if ( Action2( yypvt[-0] ) )
      					return 1;
      			} break;
      case 33:{
      				/* logicExpr : DGT */
      				if ( Action2( yypvt[-0] ) )
      					return 1;
      			} break;
      case 34:{
      				/* arithExpr : ANA */
      				if ( Action2( yypvt[-0] ) )
      					return 1;
      			} break;
      case 35:{
      				/* arithExpr : DGTA */
      				if ( Action2( yypvt[-0] ) )
      					return 1;
      			} break;
      case 36:{
      				/* arithExpr : PUL */
      				if ( Action2( yypvt[-0] ) )
      					return 1;
      			} break;
      case 37:{
      				/* arithExpr : MAX '(' timedb ')' */
      				if ( Action1( OP_MAX ) )
      					return 1;
      			} break;
      case 38:{
      				/* arithExpr : MIN '(' timedb ')' */
      				if ( Action1( OP_MIN ) )
      					return 1;
      			} break;
      case 39:{
      				/* arithExpr : AVE '(' timedb ')' */
      				if ( Action1( OP_AVE ) )
      					return 1;
      			} break;
      case 40:{
      				/* arithExpr : SUM '(' timedb ')' */
      				if ( Action1( OP_SUM ) )
      					return 1;
      			} break;
      case 41:{
      			/* arithExpr : hstdb */
      		} break;
      case 42:{
      				/* arithExpr : logicExpr '*' arithExpr */
      
				if ( Action1( OP_MULTIPLY ) )
      					return 1;
      			} break;
      case 43:{ /* arithExpr : '(' arithExpr ')' */ } break;
      case 44:{
      				/* arithExpr : arithExpr '+' arithExpr */
      
				if ( Action1( OP_PLUS ) )
      					return 1;
      			} break;
      case 45:{
      				/* arithExpr : arithExpr '-' arithExpr */
      
				if ( Action1( OP_MINUS ) )
      					return 1;
      			} break;
      case 46:{
      				/* arithExpr : arithExpr '*' arithExpr */
      
				if ( Action1( OP_MULTIPLY ) )
      					return 1;
      			} break;
      case 47:{
      				/* arithExpr : arithExpr '/' arithExpr */
      
				if ( Action1( OP_DIVIDE ) )
      					return 1;
      			} break;
      case 48:{
      				/* arithExpr : arithExpr '^' arithExpr */
      
				if ( Action1( OP_POW ) )
      					return 1;
      			} break;
      case 49:{ /* arithExpr : '+' arithExpr %prec UMINUS */ } break;
      case 50:{
      				/* arithExpr : '-' arithExpr %prec UMINUS */
      
				if ( Action1( OP_UMINUS ) )
      					return 1;
      			} break;
      case 51:{
      				/* arithExpr : MAX '(' fields ')' */
      				if ( Action1( OP_MMAX ) || Action1( yypvt[-1].wNo ) )
      					return 1;
      			} break;
      case 52:{
      				/* arithExpr : MIN '(' fields ')' */
      				if ( Action1( OP_MMIN ) || Action1( yypvt[-1].wNo ) )
      					return 1;
      			} break;
      case 53:{
      				/* arithExpr : AVE '(' fields ')' */
      				if ( Action1( OP_MAVE ) || Action1( yypvt[-1].wNo ) )
      					return 1;
      			} break;
      case 54:{
      				/* arithExpr : SUM '(' fields ')' */
      				if ( Action1( OP_MSUM ) || Action1( yypvt[-1].wNo ) )
      					return 1;
      			} break;
      case 55:{
      			/* timedb : ANA itemTime itemTime */
      			yypvt[-2].btType = ITEM_ANATIME;
      			yypvt[-2].btReserve = (BYTE)-1;
      			if ( Action2( yypvt[-2] ) )
      				return 1;
      		} break;
      case 56:{
      			/* timedb : ANA itemTime itemTime itemStep */
      			yypvt[-3].btType = ITEM_ANATIME;
      			yypvt[-3].btReserve = yypvt[-0].btReserve;
      			if ( Action2( yypvt[-3] ) )
      				return 1;
      		} break;
      case 57:{
      			/* timedb : PUL itemTime itemTime */
      			yypvt[-2].btType = ITEM_PULTIME;
      			yypvt[-2].btReserve = (BYTE)-1;
      			if ( Action2( yypvt[-2] ) )
      				return 1;
      		} break;
      case 58:{
      			/* timedb : PUL itemTime itemTime itemStep */
      			yypvt[-3].btType = ITEM_PULTIME;
      			yypvt[-3].btReserve = yypvt[-0].btReserve;
      			if ( Action2( yypvt[-3] ) )
      				return 1;
      		} break;
      case 59:{
      			/* hstdb : ANA itemTime */
      			yypvt[-1].btType = ITEM_ANAHST;
      			if ( Action2( yypvt[-1] ) )
      				return 1;
      		} break;
      case 60:{
      			/* hstdb : PUL itemTime */
      			yypvt[-1].btType = ITEM_PULHST;
      			if ( Action2( yypvt[-1] ) )
      				return 1;
      		} break;
      case 61:{
      			/* itemTime : ITEMTIME */
      			if ( Action2( yypvt[-0] ) )
      				return 1;
      		} break;
      case 62:{
      				/* itemTime : MAXT '(' timedb ')' */
      				if ( Action1( OP_MAXT ) )
      					return 1;
      			} break;
      case 63:{
      				/* itemTime : MINT '(' timedb ')' */
      				if ( Action1( OP_MINT ) )
      					return 1;
      			} break;
      case 64:{
      			/* itemStep : STEP */
      			yyval = yypvt[-0];
      		} break;
      case 65:{
      				/* fields : arithExpr ',' arithExpr */
      				yyval.wNo = 2;
      			} break;
      case 66:{
      				/* fields : fields ',' arithExpr */
      				yyval.wNo = yypvt[-2].wNo + 1;
      			} break;    }
    goto enstack;
}
