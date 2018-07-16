#ifndef HFORMULAEX_H
#define HFORMULAEX_H
#include "hformulapi.h"

//定义相关功能函数 内部使用，不对外开放
bool insertFormula(FORMULA* pFormula);
bool insertItem(ITEM* pItem,QList<ITEM*>* pItemList);
ushort getNextFormulaNo();
ushort getNextItemNo();
ITEM* getItem(ushort wNo,QList<ITEM*>* pItemList);
bool isItemEqual(ITEM* pItem1,ITEM* pItem2);
ITEM* findItem(ITEM* pItem);
ITEM* addItem(ITEM* pItem);
bool isItemValid(ITEM* pItem);
bool isFormulaValid(FORMULA* pFormula);
void freeFormulaData(int nType); //删除ITEM和FORMULA
void freeRunFormulaData();
void freeFormulaData();
int getMaxFormulaNo();
bool loadRunFormulaList();
bool loadFormulaData(QList<FORMULA *>* pFormulaList, QList<ITEM *>* pItemList);
bool checkFormulaTime(int nType);
int fOperator(ushort wSymbol);
int gOperator(ushort wSymbol);
void formatFloatString(char *szText);//对带浮点数的字符串进行格式化
bool getWordString(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string);
bool getWordStringForGIN(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string);
bool getItemString(ushort wNo,QString& string,bool bValue);
bool getItemStringForGIN(ushort wNo,QString& string,bool bValue);
bool getPointNameForGIN(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string);//wPoint 是GIN号
bool getPointName(ushort wStation,uchar btType,ushort wPoint,QString& string);//wPoint 是点号
void deleteFormulaMem(ushort wNo);
ushort getDays(int year,int mon);//获得某年某月的天数
void adjustTime(ITEM* pItem,int year,int mon,int day,int hour,int min);
void ItemTimeToTime(struct tm* ptm,ITEM* pItem);
void TimeToItemTime(struct tm* ptm,ITEM* pItem);
bool matchPulAttrib(ITEM* pItem,ushort& wAttrib);
FORMULARUN *getFormulaRun(ushort wNo);
void getInputList(FORMULARUN* pFormulaRun,QList<FORMULACONDITION*>* pList,int nDBID);
void getRuleFailList(QList<FORMULACONDITION*> *pCondList,QStringList* pStringList,bool bGrade = false);
bool getVirtualYxFailList(ushort wStation,uchar btType,ushort wPoint,uchar btValue,QStringList& list);

#endif // HFORMULAEX_H
