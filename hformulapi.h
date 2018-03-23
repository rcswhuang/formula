#ifndef FORMULAPI_H
#define FORMULAPI_H
#include "hformulaexport.h"
#include "publicdata.h"
#include <QList>
typedef unsigned int WPARAM;
typedef long LPARAM;

//定义相关类型 最好移到publicdata.h

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

        struct _tagDbWord
        {
          ushort wStation;
          ushort wPoint;
          ushort wAttrib;
        }DbWord;

        struct _tagItemTime
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

//各种属性结构
HSTTYPEINFO HstTypeInfo[]=
{  
	 //btHst          szFace        minLen
	{HSTTYPE_MONTH,    "1mon",        3},
	{HSTTYPE_1MINUTE,  "1min",        2},
	{HSTTYPE_5MINUTE,  "5min",        2},
	{HSTTYPE_15MINUTE, "15min",       3},
	{HSTTYPE_HOUR,     "1hour",       2},
	{HSTTYPE_DAY,      "1day",        2},
	{(uchar)-1,         NULL,         0}
};

ATTRINFO AnaAttrInfo[]=
{
  //wAttrib                       szAttrib
  {ATTR_ANA_VALUE,                  "工程值"},
  {ATTR_ANA_DAYMAXVALUE,            "日最大值"},
  {ATTR_ANA_DAYMINVALUE,            "日最小值"},
  {ATTR_ANA_DAYAVEVALUE,            "日平均值"},
  {ATTR_ANA_MONMAXVALUE,            "月最大值"}, 
  {ATTR_ANA_MONMINVALUE,            "月最大值"}, 
  {ATTR_ANA_MONAVEVALUE,            "月最大值"}, 
  {ATTR_VOLTAGE_DAYNORMALTIME,		"日正常时间"},
  {ATTR_VOLTAGE_DAYLOWTIME, 		"日越下限时间"},
  {ATTR_VOLTAGE_DAYHIGHTIME,		"日越上限时间"},
  {ATTR_VOLTAGE_MONNORMALTIME,	    "月正常时间"},	
  {ATTR_VOLTAGE_MONLOWTIME, 	    "月越下限时间"},
  {ATTR_VOLTAGE_MONHIGHTIME,        "月越上限时间"},	
  {ATTR_VOLTAGE_DAYQUALIFIEDRATE,   "日合格率"},
  {ATTR_VOLTAGE_MONQUALIFIEDRATE,   "月合格率"},
  {0,                                NULL}	
};

//遥信属性
ATTRINFO DgtAttrInfo[]=
{
   {ATTR_DGT_VALUE,                 "工程值"},
   {ATTR_DGT_TOTALNORMALCLOSE,      "正常合闸总数"},
   {ATTR_DGT_TOTALNORMALOPEN,       "正常分闸总数"},
   {ATTR_DGT_TOTALFAULTSWITCH,      "事故变位总数"},
   {ATTR_DGT_DAYNORMALCLOSE,        "日正常合闸次数"},
   {ATTR_DGT_DAYNORMALOPEN,         "日正常分闸次数"},
   {ATTR_DGT_DAYFAULTSWITCH,		"日事故变位次数"},
   {ATTR_DGT_MONNORMALCLOSE,        "月正常合闸次数"},
   {ATTR_DGT_MONNORMALOPEN,         "月正常分闸次数"},
   {ATTR_DGT_MONFAULTSWITCH,		"月事故变位次数"},
   {ATTR_DGT_YEARNORMALCLOSE,       "年正常合闸次数"},
   {ATTR_DGT_YEARNORMALOPEN,        "年正常分闸次数"},
   {ATTR_DGT_YEARFAULTSWITCH,       "年事故变位次数"},
   {0,                                NULL}
};

//遥脉属性
ATTRINFO PulAttrInfo[]=
{
    {ATTR_PUL_RAW,             "原始值"},
    {ATTR_PUL_COUNTERVALUE,    "工程值"},
	{ATTR_PUL_COUNTERMIN,      "分钟电量"},
	{ATTR_PUL_COUNTERHOUR,     "小时电量"},
	{ATTR_PUL_COUNTERDAY,      "日总电量"},
	{ATTR_PUL_COUNTERMON,      "月总电量"},
	{ATTR_POWER_DAYPEKVALUE,   "日峰电量"},
	{ATTR_POWER_DAYVOLVALUE,   "日谷电量"},
	{ATTR_POWER_DAYPINVALUE,   "日平电量"},
	{ATTR_POWER_MONPEKVALUE,   "月峰电量"},
	{ATTR_POWER_MONVOLVALUE,   "月谷电量"},
	{ATTR_POWER_MONPINVALUE,   "月平电量"},
	{0,                          NULL   },
};

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
typedef bool (FORMULA_EXPORT *LPFORMULAPROC)(int nMsgType,WPARAM wParam,LPARAM lParam,int nDBID = 0);
LPFORMULAPROC m_lpFormulaProc = NULL;

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

const char* FORMULA_EXPORT getFormulaText(FORMULA* formula,bool bValue);
void FORMULA_EXPORT replaceFormulaItem(FORMULA* pFormula,ITEM* pOld,ITEM* pNew);
void FORMULA_EXPORT onFormulaIdle();

bool FORMULA_EXPORT doFormula(ushort wNo,ITEM* item,bool bHst,struct tm* ptm,uchar btEType = ITEM_NULL,QList<FORMULACONDITION*>* &pList = NULL,int nDBID = 0);
bool FORMULA_EXPORT doRuleFormula(ushort wNo,QStringList* pList = NULL,int nDBID = 0);
bool FORMULA_EXPORT checkRuleFormulaConflict(ushort wNo,ushort wStation,ushort wDigitalNo);

#endif // FORMULAPI_H