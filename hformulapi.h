#ifndef FORMULAPI_H
#define FORMULAPI_H
#include "hformulaexport.h"
#include "publicdata.h"
#include <QList>
#include <QString>
typedef unsigned int WPARAM;
typedef long LPARAM;

//定义相关类型 最好移到publicdata.h
#define MAXATTRNAMELEN	32

// for 公式数据
#define MODULE_ID  0 //监控ID
#define MODULE_GIN 1 //GIN号
#define FORMULALEN 256
#define FORMULATYPE_NORMAL	0	//一般公式
#define FORMULATYPE_TWO 	1	//两极公式
#define CONDITIONTYPE_DIGITAL  0 //遥信比较
#define CONDITIONTYPE_ANALOGUE 1 //遥测比较
#define POINTBTYPE_CONST    0 //遥信比较的常量/遥测比较的常量
#define POINTBTYPE_ANALOGUE 1 //遥测量

#define HSTTYPEFIRST     0x00
#define HSTTYPE_1MINUTE  0x00//,  "1min",        2},
#define HSTTYPE_5MINUTE  0x01//,  "5min",        2},
#define HSTTYPE_15MINUTE 0x02//, "15min",       3},
#define HSTTYPE_HOUR     0x03//,     "1hour",       2},
#define HSTTYPE_DAY      0x04//,      "1day",        2},
#define HSTTYPE_MONTH    0x05
#define HSTTYPE_YEAR     0x06
#define HSTTYPELAST      0x06

#define HEADER_SIGN '['
#define TAILER_SIGN ']'

//公式具体内容
typedef struct _tagFORMULA
{
    ushort wNo; //公式
    ushort wMessage; //保留
    ushort wFormula[FORMULALEN]; //存放公式(数字号、操作符、四遥点号等)
    uchar btType;		//公式类型

} FORMULA;

//历史类型信息
typedef struct _tagHSTTYPEINFO
{
    uchar btHst;
    char* szFace;
    int nMinLen;

}HSTTYPEINFO;

//遥脉电量
typedef struct _tagPULSAVEINFO
{
    unsigned long dwFlag;
    ushort wAttrib;
}PULSAVEINFO;

//属性信息
typedef struct _tagATTRINFO
{
    ushort wAttrib;//TYPE_NULL.....
    char* szAttrib;//0,"遥信量".....
}ATTRINFO;

//属性组信息
typedef struct _tagATTRINFOS
{
    uchar btType;
    char* szType;
    ATTRINFO* pAttrInfo;
}ATTRINFOS;

//公式中每一项解析之后存放的结构
//数字 + - * / 等每一个都对应一个结构存放
//对于不同类型的ITEM 都有需要存放的值
//遥测遥信不要存放值，存放站号点号属性等信息
//时间值就需要存放时间，
typedef struct _tagITEM
{
    ushort wNo;
    uchar btType;
    uchar btReserver; //HSTYPEINFO里面的btHst

    union{
        bool bValue;
        float fValue;

        struct
        {
          ushort wStation;
          ushort wPoint;
          ushort wAttrib;
        }DbWord;

        struct
        {
          char year;
          char mon;
          char day;
          char hour;
          char min;
          char flag;
        }ItemTime;
    };
}ITEM;


typedef struct _tagFORMULAPARAMETER
{
    ushort wStation;
    uchar btType;
    ushort wPoint;
    ushort wAttrib;

    void* pBuffer;
    ITEM* pItem;
    uchar btFunc;
    uchar btHst;

    struct tm* ptm1;
    struct tm* ptm2;

    time_t lTime;

}FORMULAPARAMETER;

typedef struct
{
  FORMULA* pFormula;
  ITEM* pItem[FORMULALEN];
}FORMULARUN;

typedef struct _tagFORMULACONDITION
{
    uchar btType;//类型 遥信或者遥测
    ushort wStationA;
    ushort wPointA;
    ushort wAttribA;
    float fValueA;
    ushort wOp; //运算标志

    uchar btPointTypeB;//常量(遥信)或者遥测量
    ushort wStationB;
    ushort wPointB;
    ushort wAttribB;
    float fValueB;

    ushort wGroupNo;//条件组号
    bool bResult;
}FORMULACONDITION;

typedef struct _tagINPUTITEM
{
    uchar btType; // 0-yc 1-yx 2-const 3-calc
    ushort wStation;
    ushort wPoint;
    ushort wAttrib;
    float fValue;
    bool bAlone;//独立和组合
    int nGroup; //组号
}INPUTITEM;

typedef struct _tagFORMULAITEMLIST
{
    QList<FORMULA*> *pFormulaList;
    QList<ITEM*> *pItemList;
}FORMULAITEMLIST;

typedef struct _tagITEMDATA
{
    int nStartPos;
    int nEndPos;

    ITEM item;
}ITEMDATA;

#define INVALID_FLOAT 3.4e+38F  //超大值
#define INVALID_TIME 0
//回调消息类型
#define FM_GETDBINFO       0 //获取点或者厂站信息
#define FM_FINDDBINFO      1 //根据index获取点或者厂站信息
#define FM_GETDBATTR       2 //获取点值
#define FM_GETHSTATTR      3 //获取点历史值
#define FM_GETHSTFUNC      4 //获取点历史功能值
#define FM_SAVEFORMULALIST 5 //提示保存公式项

#define ITEMTIME_YEAR  0x01
#define ITEMTIME_MON   0x02
#define ITEMTIME_DAY   0x03
#define ITEMTIME_HOUR  0x04
#define ITEMTIME_MIN   0x05

#define OP_UMINUS       0x8000 //+正值
#define OP_UPLUS        0x8001 //-负值
#define OP_NOT          0x8002 //取非

#define OP_PLUS         0x8010
#define OP_MINUS        0x8011
#define OP_MULTIPLY     0x8012
#define OP_DIVIDE       0x8013
#define OP_POW          0x8014
#define OP_GREATER      0x8015
#define OP_LOWER        0x8016
#define OP_EQUAL        0x8017
#define OP_GEQUAL       0x8018
#define OP_LEQUAL       0x8019
#define OP_NEQUAL       0x801A
#define OP_AND          0x801B
#define OP_OR           0x801C
#define OP_XOR          0x801D

#define OP_MAX          0x8020
#define OP_MIN          0x8021
#define OP_AVE          0x8022
#define OP_SUM          0x8023
#define OP_MAXT         0x8024
#define OP_MINT         0x8025

#define OP_MMAX         0x8030
#define OP_MMIN         0x8031
#define OP_MAVE         0x8032
#define OP_MSUM         0x8033

#define ISFORMULAITEM(n)     (((ushort)n) > 0 && ((ushort)n) < 0x8000)
#define ISONEOPERATOR(n)     (((ushort)n) >= 0x8000 && ((ushort)n) < 0x8010)
#define ISTWOOPERATOR(n)     (((ushort)n) >= 0x8010 && ((ushort)n) < 0x8020)
#define ISFUNCTION(n)        (((ushort)n) >= 0x8020 && ((ushort)n) < 0x8030)
#define ISMULTIFUNCTION(n)   (((ushort)n) >= 0x8030)



//定义公式项类型
#define ITEM_NULL      0
#define ITEM_ANALOGUE  1
#define ITEM_DIGITAL   2
#define ITEM_PULSE     3
#define ITEM_FLOAT     4
#define ITEM_BOOLEAN   5
#define ITEM_TIME      6//时间项
#define ITEM_ANAHST    7 //遥测历史项
#define ITEM_PULHST    8 //遥脉历史项
#define ITEM_ANATIME   9 //遥测时间项
#define ITEM_PULTIME   10 //遥脉时间项
#define ITEM_RELAY     11 //遥控项
#define ITEM_TUNE      12 //
//回调函数
typedef bool (FORMULA_EXPORT *LPFORMULAPROC)(int nMsgType,WPARAM wParam,LPARAM lParam,int nDBID);

bool FORMULA_EXPORT initFormula(LPFORMULAPROC lpFormulaProc,uchar btModuleType = MODULE_ID);
void FORMULA_EXPORT exitFormula();

FORMULA* FORMULA_EXPORT getFormula(ushort wNo);

bool FORMULA_EXPORT loadFormulaData(QList<FORMULA*> pFormulaList,QList<ITEM*> pItemList);
bool FORMULA_EXPORT saveFormulaData();

void FORMULA_EXPORT setFormulaModified(bool modified);
bool FORMULA_EXPORT getFormulaModified();

bool FORMULA_EXPORT addFormula(FORMULA* pFromula,ushort wNo,uchar btType = FORMULATYPE_NORMAL);
void FORMULA_EXPORT deleteFormula(ushort wNo);

bool FORMULA_EXPORT createFormula(FORMULA* pFormula,ushort wNo);//创建某个测点wNo的公式
bool FORMULA_EXPORT compileFormula(const char* szFormula,FORMULA* pFormula);//编译公式

QString FORMULA_EXPORT getFormulaText(FORMULA* formula,bool bValue);
void FORMULA_EXPORT replaceFormulaItem(FORMULA* pFormula,ITEM* pOld,ITEM* pNew);
void FORMULA_EXPORT onFormulaIdle();

bool FORMULA_EXPORT doFormula(ushort wNo,ITEM* item,bool bHst,struct tm* ptm,uchar btEType = ITEM_NULL,QList<FORMULACONDITION*>* pList = NULL,int nDBID = 0);
bool FORMULA_EXPORT doRuleFormula(ushort wNo,QStringList* pList = NULL,int nDBID = 0);
bool FORMULA_EXPORT checkRuleFormulaConflict(ushort wNo,ushort wStation,ushort wDigitalNo);

#endif // FORMULAPI_H
