#if defined (_MSC_VER) && (_MSC_VER >=1600)
#pragma execution_character_set("utf-8")
#endif

#include "hformulaex.h"
#include <QList>
#include <QVector>
using namespace std;
extern QList<FORMULA*> m_FormulaList;
extern QList<ITEM*> m_ItemList;
extern QVector<FORMULARUN*> m_FormulaRunList;
extern bool m_bFormula;
extern bool m_bModified;
extern ushort m_wStation;
extern uchar m_btType;
extern uchar m_btFormulaModule;
extern QString m_strFormulaText;
extern LPFORMULAPROC m_lpFormulaProc;
extern ATTRINFO AnaAttrInfo[];
extern ATTRINFO DgtAttrInfo[];
extern ATTRINFO PulAttrInfo[];

//定义相关功能函数 内部使用，不对外开放
bool insertFormula(FORMULA* pFormula)
{
    if(NULL == pFormula)
        return false;
    ushort wNo = pFormula->wNo;
    QList<FORMULA*>::iterator cur = m_FormulaList.begin();
    for(;cur != m_FormulaList.end();++cur)
    {
        FORMULA* pCurFormula = *cur;
        if(wNo == pCurFormula->wNo)
            return false;
        if(wNo < pCurFormula->wNo)
        {
            m_FormulaList.insert(cur,pFormula);
            return true;
        }
    }
    m_FormulaList.append(pFormula);
    return true;
}

bool insertItem(ITEM* pItem,QList<ITEM*>* pItemList)
{
    if(NULL == pItemList)
        pItemList = &m_ItemList;

    ushort wNo = pItem->wNo;
    QList<ITEM*>::iterator cur = pItemList->begin();
    for(;cur != pItemList->end(); ++cur)
    {
        //QList<ITEM*>::iterator sav = cur;
        ITEM* pCurItem = *cur;
        if(wNo == pCurItem->wNo)
            return false;
        if(wNo < pCurItem->wNo)
        {
            pItemList->insert(cur,pItem);
            return true;
        }
    }
    pItemList->append(pItem);
    return true;
}

ushort getNextFormulaNo()
{
    ushort wNo = 0;
    for(int i = 0; i < m_FormulaList.count();i++)
    {
        FORMULA* pFormula = (FORMULA*)m_FormulaList[i];
        if(NULL == pFormula)
            continue;
        if(wNo == pFormula->wNo)
            wNo++;
        else
            break;
    }
    return wNo;
}

ushort getNextItemNo()
{
    ushort wNo = 1;
    for(int i = 0; i < m_ItemList.count(); i++)
    {
        ITEM* item = (ITEM*)m_ItemList[i];
        if(NULL == item)
            continue;
        if(wNo == item->wNo)
            wNo++;
        else
            break;
    }
    return wNo;
}

ITEM* getItem(ushort wNo,QList<ITEM*>* pItemList)
{
    if(NULL == pItemList)
        pItemList = &m_ItemList;
    for(int i = 0; i < pItemList->count();i++)
    {
        ITEM* item = (ITEM*)pItemList->at(i);
        if(wNo == item->wNo)
            return item;
    }
    return NULL;
}

bool isItemEqual(ITEM* pItem1,ITEM* pItem2)
{
    if(NULL == pItem1 || NULL == pItem2)
        return false;
    if(pItem1->btType != pItem2->btType)
        return false;
    switch (pItem1->btType) {
    case ITEM_ANATIME:
    case ITEM_PULTIME:
        if(pItem1->btReserver != pItem2->btReserver)
            break;
    case ITEM_ANALOGUE:
    case ITEM_ANAHST:
    case ITEM_DIGITAL:
    case ITEM_PULSE:
    case ITEM_PULHST:
        if(pItem1->DbWord.wStation == pItem2->DbWord.wStation &&
           pItem1->DbWord.wPoint == pItem2->DbWord.wPoint &&
           pItem1->DbWord.wAttrib == pItem2->DbWord.wAttrib)
            return true;
        break;
    case ITEM_FLOAT:
        if(pItem1->fValue == pItem2->fValue)
            return true;
        break;
    case ITEM_BOOLEAN:
        if(pItem1->bValue == pItem2->bValue)
            return true;
        break;
    case ITEM_TIME:
        if(pItem1->ItemTime.year == pItem2->ItemTime.year &&
           pItem1->ItemTime.mon == pItem2->ItemTime.mon &&
           pItem1->ItemTime.day == pItem2->ItemTime.day &&
           pItem1->ItemTime.hour == pItem2->ItemTime.hour &&
           pItem1->ItemTime.min == pItem2->ItemTime.min &&
           pItem1->ItemTime.flag == pItem2->ItemTime.flag)
            return true;
        break;
    default:
        break;
    }
    return false;
}

ITEM* findItem(ITEM* pItem)
{
    if(NULL == pItem)
        return NULL;
    for(int i = 0; i < m_ItemList.count(); i++)
    {
        ITEM* curItem = (ITEM*)m_ItemList[i];
        if(isItemEqual(pItem,curItem))
            return curItem;
    }

    return NULL;
}

ITEM* addItem(ITEM* pItem)
{
    ITEM* item = findItem(pItem);
    if(NULL == item)
    {
        item = new ITEM;
        if(NULL == item)
            return NULL;
        *item = *pItem;
        item->wNo = getNextItemNo();

        if(!insertItem(item,&m_ItemList))
        {
            delete item;
            return NULL;
        }
    }
    return item;
}

bool isItemValid(ITEM* pItem)
{
    if(NULL == pItem)
        return false;
    return true;
}

bool isFormulaValid(FORMULA* pFormula)
{
    if(NULL == pFormula)
        return false;
    int i = 0;
    while(i < FORMULALEN && ISFORMULAITEM(pFormula->wFormula[i]))
    {
        ITEM* item = getItem(pFormula->wFormula[i++],&m_ItemList);
        if(!isItemValid(item))
            return false;
    }
    return true;
}

#define FTYPE_FORMULA 0x01
#define FTYPE_ITEM    0x02
void freeFormulaData(int nType) //删除ITEM和FORMULA
{
    if(FTYPE_FORMULA)
    {
        while(!m_FormulaList.isEmpty())
            delete (FORMULA*)m_FormulaList.takeFirst();
    }
    else if(FTYPE_ITEM)
    {
        while(!m_ItemList.isEmpty())
            delete (ITEM*)m_ItemList.takeFirst();
    }
}

void freeRunFormulaData()
{
    while(!m_FormulaRunList.isEmpty())
    {
        delete (FORMULARUN*)m_FormulaRunList.takeFirst();
    }
    m_FormulaRunList.clear();
}

void freeFormulaData()
{
     freeFormulaData(FTYPE_FORMULA);
     freeFormulaData(FTYPE_ITEM);
     freeRunFormulaData();
}

int getMaxFormulaNo()
{
    int wMax = 0;
    for(int i = 0; i < m_FormulaList.count(); i++)
    {
        FORMULA* pFormula = (FORMULA*)m_FormulaList[i];
        if(NULL != pFormula && wMax < pFormula->wNo)
            wMax = pFormula->wNo;
    }
    return wMax;
}

bool loadRunFormulaList()
{
    freeRunFormulaData();
    for(int i = 0; i < m_FormulaList.count();i++)
    {
        FORMULA* pFormula = m_FormulaList[i];
        if(NULL != pFormula)
        {
            ushort wNo = pFormula->wNo;
            if(wNo > 0)
            {
                FORMULARUN* pFormulaRun = new FORMULARUN;
                memset(pFormulaRun,0,sizeof(FORMULARUN));
                int k = 0;
                while(k < FORMULALEN && pFormula->wFormula[k] != 0)
                {
                    ushort wItemNo = pFormula->wFormula[k];
                    ITEM* item = getItem(wItemNo,&m_ItemList);
                    pFormulaRun->pItem[k] = item;
                    k++;
                }
                pFormulaRun->pFormula = pFormula;
                m_FormulaRunList.insert(i,pFormulaRun);
            }
        }
    }
    return true;
}

//从文件中读取的规则表和规则项表
bool loadFormulaData(QList<FORMULA *>*  pFormulaList, QList<ITEM *>* pItemList)
{
    if(NULL == pFormulaList || NULL == pItemList)
        return false;
    QList<FORMULA*>::iterator fit = pFormulaList->begin();
    for(;fit != pFormulaList->end();++fit)
    {
        FORMULA* pFormula = (FORMULA*)*fit;
        if(NULL == pFormula) continue;
        FORMULA* pNewFormula = new FORMULA;
        memcpy(pNewFormula,pFormula,sizeof(FORMULA));
        insertFormula(pNewFormula);
    }

    QList<ITEM*>::iterator it = pItemList->begin();
    for(;it != pItemList->end();++it)
    {
        ITEM* pItem = (ITEM*)*it;
        if(NULL == pItem) continue;
        ITEM* pNewItem = new ITEM;
        memcpy(pNewItem,pItem,sizeof(ITEM));
        insertItem(pNewItem,&m_ItemList);
    }

    if(!loadRunFormulaList()) return false;
    return true;
}

bool checkFormulaTime(int nType)
{
    return true;
}

int fOperator(ushort wSymbol)
{
    if(ISFORMULAITEM(wSymbol) || ISFUNCTION(wSymbol))
        return 100;
    switch (wSymbol) {
    case OP_MMAX:
    case OP_MMIN:
        return 20;
    case OP_UMINUS:
    case OP_UPLUS:
        return 18;
    case OP_POW:
        return 16;
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return 14;
    case OP_PLUS:
    case OP_MINUS:
        return 12;
    case OP_NOT:
        return 10;
    case OP_XOR:
        return 9;
    case OP_AND:
        return 8;
    case OP_OR:
        return 6;
    case OP_GREATER:
    case OP_LOWER:
    case OP_EQUAL:
    case OP_GEQUAL:
    case OP_LEQUAL:
    case OP_NEQUAL:
        return 4;
    default:
        break;
    }
    return 0;
}

int gOperator(ushort wSymbol)
{
    switch (wSymbol) {
    case OP_UMINUS:
    case OP_UPLUS:
        return 19;
    case OP_POW:
        return 16;
    case OP_MULTIPLY:
    case OP_DIVIDE:
        return 14;
    case OP_PLUS:
    case OP_MINUS:
        return 12;
    case OP_NOT:
        return 10;
    case OP_XOR:
        return 9;
    case OP_AND:
        return 8;
    case OP_OR:
        return 6;
    case OP_GREATER:
    case OP_LOWER:
    case OP_EQUAL:
    case OP_GEQUAL:
    case OP_LEQUAL:
    case OP_NEQUAL:
        return 4;
    default:
        break;
    }
    return 0;
}

//1.20000->1.2
void formatFloatString(char *szText)//对带浮点数的字符串进行格式化
{
    char* p = strchr(szText,'.');//取.20000
    if(NULL != p)
    {
        char *q = szText + strlen(szText) - 1;
        while('0' == *q && q > p)
        {
            *q = '\0';
            q--;
        }
        if(p == q)
            *q = '\0';
    }
}

bool getWordString(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string)
{
    if(NULL == m_lpFormulaProc)
        return false;
    int nDBID = 0;
    FORMULAPARAMETER parameter;
    parameter.wStation = wStation;
    parameter.btType = TYPE_NULL;
    parameter.wPoint = wPoint;

    STATION station;
    parameter.pBuffer = &station;
    if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID))
        return false;
    parameter.btType = btType;
    char szText[128] = "";
    switch (btType) {
    case TYPE_ANALOGUE:
    {
        ANALOGUE analogue;
        parameter.pBuffer = &analogue;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID))
            return false;
        int i = 0;
        for(; AnaAttrInfo[i].szAttrib != NULL;i++)
            if(AnaAttrInfo[i].wAttrib == wAttrib)
                break;
        if(NULL == AnaAttrInfo[i].szAttrib)
            return false;
        sprintf(szText,"%c%s.%s.%s%c",'[',station.szStationName,analogue.szAnalogueName,AnaAttrInfo[i].szAttrib,']');
    }
        break;

    case TYPE_DIGITAL:
    {
        DIGITAL digital;
        parameter.pBuffer = &digital;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID))
            return false;
        int i = 0;
        for(; DgtAttrInfo[i].szAttrib != NULL;i++)
            if(DgtAttrInfo[i].wAttrib == wAttrib)
                break;
        if(NULL == DgtAttrInfo[i].szAttrib)
            return false;
        sprintf(szText,"%c%s.%s.%s%c",'[',station.szStationName,digital.szDigitalName,DgtAttrInfo[i].szAttrib,']');
    }
        break;

    case TYPE_PULSE:
       /* PULSE analogue;
        parameter.pBuffer = &analogue;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter));
            return false;
        int i = 0;
        for(; AnaAttrInfo[i].szAttrib != NULL;i++)
            if(AnaAttrInfo[i].wAttrib == wAttrib)
                break;
        if(NULL == AnaAttrInfo[i].szAttrib)
            return false;
        sprintf(szText,"%c%s.%s.%s%c",'[',station.szStationName,analogue.szAnalogueName,AnaAttrInfo[i].szAttrib,']');
        break;*/
        break;
    default:
        break;
    }
    string = szText;
    return true;
}

bool getWordStringForGIN(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string)
{
    return false;
}

bool getItemString(ushort wNo,QString& string,bool bValue)
{
    ITEM* pItem = getItem(wNo,&m_ItemList);
    if(NULL == pItem)
        return false;
    int nDBID = 0;
    ITEM item;
    FORMULAPARAMETER parameter;
    parameter.wStation = pItem->DbWord.wStation;
    parameter.wPoint = pItem->DbWord.wPoint;
    parameter.wAttrib = pItem->DbWord.wAttrib;
    parameter.pBuffer = &item;

    string = "";

    switch (pItem->btType) {
    case ITEM_ANAHST:
    case ITEM_ANALOGUE:
    case ITEM_ANATIME:
    case ITEM_PULHST:
    case ITEM_PULSE:
    case ITEM_PULTIME:
    {
        uchar btType = (ITEM_ANAHST == pItem->btType || ITEM_ANALOGUE == pItem->btType || ITEM_ANATIME == pItem->btType)?TYPE_ANALOGUE:TYPE_PULSE;
        if(!getWordString(pItem->DbWord.wStation,btType,pItem->DbWord.wPoint,pItem->DbWord.wAttrib,string))
            return false;
        if(bValue && NULL != m_lpFormulaProc)
        {
            parameter.btType = btType;
            if(m_lpFormulaProc(FM_GETDBATTR,0,(LPARAM)&item,nDBID))
            {
                char szText[64] = "";
                if(ITEM_FLOAT == item.btType)
                {
                    sprintf(szText,"%f",item.fValue);
                    formatFloatString(szText);
                }
                string += szText;
            }
        }
    }
        break;
    case ITEM_DIGITAL:
    {
        uchar btType = TYPE_DIGITAL;
        if(!getWordString(pItem->DbWord.wStation,btType,pItem->DbWord.wPoint,pItem->DbWord.wAttrib,string))
            return false;
        if(bValue && NULL != m_lpFormulaProc)
        {
            parameter.btType = btType;
            if(m_lpFormulaProc(FM_GETDBATTR,0,(LPARAM)&item,nDBID))
            {
                char szText[64] = "";
                if(ITEM_FLOAT == item.btType)
                {
                    sprintf(szText,"%f",item.fValue);
                    formatFloatString(szText);
                }
                else if(ITEM_BOOLEAN == item.btType)
                    sprintf(szText,"%d",item.bValue);
                string += szText;
            }
        }
    }
        break;
    case ITEM_FLOAT:
    {
        char szText[64]="";
        try
        {
            sprintf(szText,"%f",pItem->fValue);
            formatFloatString(szText);
        }catch(std::exception &e)
        {
            szText[0] = 0;
        }
        string = szText;
    }
        break;
    default:
        break;
    }
    return true;
}

bool getItemStringForGIN(ushort wNo,QString& string,bool bValue)
{
    return false;
}

bool getPointNameForGIN(ushort wStation,uchar btType,ushort wPoint,ushort wAttrib,QString& string)//wPoint 是GIN号
{
    return false;
}

bool getPointName(ushort wStation,uchar btType,ushort wPoint,QString& string)//wPoint 是点号
{
    if(NULL == m_lpFormulaProc)
        return false;
    int nDBID = 0;
    FORMULAPARAMETER parameter;
    parameter.wStation = wStation;
    parameter.btType = TYPE_NULL;
    parameter.wPoint = wPoint;

    STATION station;
    parameter.pBuffer = &station;
    if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID));
        return false;
    parameter.btType = btType;
    char szText[128] = "";
    switch (btType) {
    case TYPE_ANALOGUE:
    {
        ANALOGUE analogue;
        parameter.pBuffer = &analogue;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID));
            return false;

        sprintf(szText,"%s%s",station.szStationName,analogue.szAnalogueName);
    }
        break;

    case TYPE_DIGITAL:
    {
        DIGITAL digital;
        parameter.pBuffer = &digital;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter,nDBID));
            return false;

        sprintf(szText,"%s%s",station.szStationName,digital.szDigitalName);
    }
        break;

    case TYPE_PULSE:
       /* PULSE analogue;
        parameter.pBuffer = &analogue;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&parameter));
            return false;
        sprintf(szText,"%c%s.%s.%s%c",'[',station.szStationName,analogue.szAnalogueName,AnaAttrInfo[i].szAttrib,']');
        break;*/
    default:
        break;
    }
    string = szText;
    return true;
}



void deleteFormulaMem(ushort wNo)
{
    FORMULA* pFormula = getFormula(wNo);
    if(NULL == pFormula)
        return;
    m_FormulaList.removeOne(pFormula);
    m_bModified = true;
    delete pFormula;
    pFormula = NULL;
}

ushort getDays(int year,int mon)//获得某年某月的天数
{
    ushort wDay = 0;
    bool bFlags = (0 == year%400 || (0 == year%4 && 0 != year%100));
    switch(mon)
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        wDay = 31;
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        wDay = 30;
    case 2:
        wDay = bFlags?(ushort)29:(ushort)28;
        break;
    default:
        break;
    }
    return wDay;
}

void adjustTime(ITEM* pItem,int year,int mon,int day,int hour,int min)
{
    if(year != 0)
        pItem->ItemTime.year += year;
    if(mon != 0)
        pItem->ItemTime.mon += mon;

    while(pItem->ItemTime.mon < 0)
    {
        pItem->ItemTime.mon += 12;
        pItem->ItemTime.year--;
    }

    while(pItem->ItemTime.mon > 11)
    {
        pItem->ItemTime.mon -= 12;
        pItem->ItemTime.year++;
    }

    int days = getDays(pItem->ItemTime.year+1900,pItem->ItemTime.mon+1);
    if(pItem->ItemTime.day > days)
        pItem->ItemTime.day = days;
    if(day != 0 || hour != 0 || min != 0)
    {
        struct tm tm;
        memset(&tm,0,sizeof(struct tm));
        tm.tm_year = pItem->ItemTime.year;
        tm.tm_mon = pItem->ItemTime.mon;
        tm.tm_mday = pItem->ItemTime.day;
        tm.tm_hour = pItem->ItemTime.hour;
        tm.tm_min = pItem->ItemTime.min;

        time_t lTime = mktime(&tm);
        lTime += day*86400+hour*3600+min*60;
        tm = *(localtime(&lTime));

        pItem->ItemTime.year = tm.tm_year;
        pItem->ItemTime.mon = tm.tm_mon;
        pItem->ItemTime.day = tm.tm_mday;
        pItem->ItemTime.hour = tm.tm_hour;
        pItem->ItemTime.min = tm.tm_min;
    }

}

void ItemTimeToTime(struct tm* ptm,ITEM* pItem)
{
    memset(&ptm,0,sizeof(struct tm));
    ptm->tm_year = pItem->ItemTime.year;
    ptm->tm_mon = pItem->ItemTime.mon;
    ptm->tm_mday = pItem->ItemTime.day;
    ptm->tm_hour = pItem->ItemTime.hour;
    ptm->tm_min = pItem->ItemTime.min;
}

void TimeToItemTime(struct tm* ptm,ITEM* pItem)
{
    pItem->ItemTime.year = ptm->tm_year;
    pItem->ItemTime.mon = ptm->tm_mon;
    pItem->ItemTime.day = ptm->tm_mday;
    pItem->ItemTime.hour = ptm->tm_hour;
    pItem->ItemTime.min = ptm->tm_min;
}

bool matchPulAttrib(ITEM* pItem,ushort& wAttrib)
{
    return false;
}

FORMULARUN *getFormulaRun(ushort wNo)
{
    if(!m_bFormula)
        return NULL;
    if(wNo > m_FormulaRunList.count()) return NULL;
    if(wNo < 0) return NULL;
    return (FORMULARUN*)m_FormulaRunList[wNo];
}

void getInputList(FORMULARUN* pFormulaRun,QList<FORMULACONDITION*>* pList,int nDBID)
{
    if(NULL == pFormulaRun || NULL == pList)
        return;
    FORMULA *pFormula = pFormulaRun->pFormula;
    if(NULL == pFormula && pFormula->btType != FORMULATYPE_TWO) return;

    INPUTITEM ItemInput[FORMULALEN];
    QList<INPUTITEM*> ItemGroup[FORMULALEN];
    QList<INPUTITEM*> ItemBeforeOR;

    int k = 0;
    int top = 0;
    int ItemCount = 0; //量测的个数
    int nGroupNo = 1; //组合号
    uchar btUnitCount = 0; //AND之前的单元个数
    int nGroupIndex = 0;
    bool bFirstOr = true;

    while(k < FORMULALEN && pFormula->wFormula[k] != 0)
    {
        ushort wNo = pFormula->wFormula[k++];
        if(ISFORMULAITEM(wNo))
        {
            ITEM* pItem = pFormulaRun->pItem[k-1];//取前一个ITEM
            if(NULL == pItem)
                return;
            if((pItem->btType == ITEM_ANALOGUE) || (pItem->btType == ITEM_DIGITAL))
            {
                ITEM item;
                FORMULAPARAMETER param;
                param.wStation = pItem->DbWord.wStation;
                param.wPoint = pItem->DbWord.wPoint;
                param.wAttrib = pItem->DbWord.wAttrib;
                param.pItem = &item;
                param.btType = TYPE_ANALOGUE;
                ItemInput[top].btType = 0;//yc
                ItemInput[top].wStation = param.wStation;
                if(ITEM_DIGITAL == pItem->btType)
                {
                    ItemInput[top].btType = 1;//yx
                    param.btType = TYPE_DIGITAL;
                }
                ItemInput[top].wPoint = pItem->DbWord.wPoint;
                if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&param,nDBID))
                    return;
                if(item.btType == ITEM_BOOLEAN)
                    ItemInput[top].fValue = (float)pItem->bValue;
                else
                    ItemInput[top].fValue = (float)pItem->fValue;
            }
            if((pItem->btType == ITEM_FLOAT) || (pItem->btType == ITEM_BOOLEAN))
            {
                ItemInput[top].btType = 2; //常量
                if(pItem->btType == ITEM_BOOLEAN)
                    ItemInput[top].fValue= (float)pItem->bValue;
                else
                    ItemInput[top].fValue = (float)pItem->fValue;
            }
            btUnitCount++;
            ItemInput[top].nGroup = nGroupNo;
            ItemInput[top].bAlone = true;
            ItemGroup[nGroupIndex].append(&ItemInput[top]);
            ItemBeforeOR.append(&ItemInput[0]);
            top++;
            ItemCount++;
            continue;
        }

        if(wNo == OP_AND)  //过个and
        {
            if(btUnitCount == 2 || 3 == btUnitCount)
            {
                int nCount = ItemGroup[nGroupIndex].count();
                int nNum;
                INPUTITEM* pItemInput = NULL;
                if(nCount >= 2)
                {
                    nNum = ItemGroup[nGroupIndex].count()-1;
                    pItemInput = (INPUTITEM*)ItemGroup[nGroupIndex].value(nNum);
                    if(pItemInput)
                    {
                        ItemGroup[nGroupIndex+1].append(pItemInput);
                        ItemGroup[nGroupIndex].removeOne(pItemInput);
                        if(!pItemInput->bAlone)
                        {
                            nNum = ItemGroup[nGroupIndex].count()-1;
                            pItemInput = (INPUTITEM*)ItemGroup[nGroupIndex].value(nNum);
                            if(pItemInput)
                            {
                                ItemGroup[nGroupIndex+1].append(pItemInput);
                                ItemGroup[nGroupIndex].removeOne(pItemInput);
                            }
                        }
                    }
                }
                nGroupIndex++;
            }
            btUnitCount = 0;
        }

        if(wNo == OP_OR)
        {
            nGroupNo++;
            ushort wLastFormulaNo = pFormula->wFormula[k-2];
            if(wLastFormulaNo != OP_AND && wLastFormulaNo != OP_OR)
            {
                ItemInput[top-1].nGroup = nGroupNo;
                if(!ItemInput[top-1].bAlone)
                    ItemInput[top-2].nGroup = nGroupNo;
                nGroupIndex++;
            }
            else if(wLastFormulaNo == OP_AND)
            {
                for(int i = 0; i < ItemGroup[nGroupIndex].count();i++)
                {
                    INPUTITEM* inputItem = (INPUTITEM*)ItemGroup[nGroupIndex][i];
                    if(inputItem) inputItem->nGroup = nGroupNo;
                }
            }
            for(int i = 0; i < ItemBeforeOR.count();i++)
            {
                INPUTITEM* inputItem = (INPUTITEM*)ItemBeforeOR[i];
                if(inputItem) inputItem->nGroup++;
            }
            nGroupNo++;
            btUnitCount = 0;
            if(bFirstOr)
                bFirstOr = false;
            ItemBeforeOR.clear();//不能删里面的元素
        }

        if((wNo >= OP_GREATER) && (wNo <= OP_NEQUAL))
        {
            btUnitCount--;
            ItemInput[top-1].bAlone = false;
            continue;
        }
    }

    k = 0;
    top = 0;
    while(k < FORMULALEN && pFormula->wFormula[k] != 0)
    {
        ushort wNo = pFormula->wFormula[k++];
        if(ISFORMULAITEM(wNo))
        {
            top++;
            continue;
        }

        if(wNo == OP_NOT)
        {
            if(ItemInput[top-1].btType = 1 || ItemInput[top-1].btType == 0)
            {
                FORMULACONDITION* pCond = new FORMULACONDITION;
                pCond->btType = CONDITIONTYPE_DIGITAL;
                pCond->wStationA = ItemInput[top-1].wStation;
                pCond->wPointA = ItemInput[top-1].wPoint;
                pCond->wAttribA = ItemInput[top-1].wAttrib;
                pCond->fValueA = ItemInput[top-1].fValue;
                pCond->wOp = wNo;
                pCond->wGroupNo = ItemInput[top-1].nGroup;
                if((ItemInput[top-1].fValue - 0) < 0.00001)
                    pCond->bResult = true;
                else
                    pCond->bResult = false;
                pList->append(pCond);
                ItemInput[top-1].fValue = (float)pCond->bResult;
                ItemInput[top-1].btType = 3;
            }
            continue;
        }

        if((wNo >= OP_GREATER) &&(wNo <= OP_NEQUAL))
        {
            if(ItemInput[top-2].btType == 0 || ItemInput[top-2].btType == 1)//---huangw
            {
                FORMULACONDITION* pCond = new FORMULACONDITION;
                pCond->btType = CONDITIONTYPE_ANALOGUE;
                pCond->wStationA = ItemInput[top-2].wStation;
                pCond->wPointA = ItemInput[top-2].wPoint;
                pCond->wAttribA = ItemInput[top-2].wAttrib;
                pCond->fValueA = ItemInput[top-2].fValue;
                pCond->wOp = wNo;
                pCond->wGroupNo = ItemInput[top-2].nGroup;
                if(ItemInput[top-1].btType == 0)
                {
                    pCond->btPointTypeB = POINTBTYPE_ANALOGUE;
                    pCond->wStationB = ItemInput[top-1].wStation;
                    pCond->wPointB = ItemInput[top-1].wPoint;
                    pCond->wAttribB = ItemInput[top-1].wAttrib;
                    pCond->fValueB = ItemInput[top-1].fValue;
                }
                if(ItemInput[top-1].btType == 2)
                {
                    pCond->btPointTypeB = POINTBTYPE_CONST;
                    pCond->fValueB = ItemInput[top-1].fValue;
                }

                switch (wNo) {
                case OP_GREATER:
                    pCond->bResult = (pCond->fValueA > pCond->fValueB);
                    break;
                case OP_LOWER:
                    pCond->bResult = (pCond->fValueA < pCond->fValueB);
                    break;
                case OP_EQUAL:
                    pCond->bResult = (pCond->fValueA == pCond->fValueB);
                    break;
                case OP_GEQUAL:
                    pCond->bResult = (pCond->fValueA >= pCond->fValueB);
                    break;
                case OP_LEQUAL:
                    pCond->bResult = (pCond->fValueA <= pCond->fValueB);
                    break;
                case OP_NEQUAL:
                    pCond->bResult = (pCond->fValueA != pCond->fValueB);
                    break;
                default:
                    break;
                }
                pList->append(pCond);
                ItemInput[top-2].fValue = (float)(pCond->bResult);
                ItemInput[top-2].btType = 3;
            }
            continue;
        }
    }

    for(int i = 0; i < ItemCount;i++)
    {
        if(ItemInput[i].btType == 1)
        {
            FORMULACONDITION* pCond = new FORMULACONDITION;
            pCond->btType = CONDITIONTYPE_DIGITAL;
            pCond->wStationA = ItemInput[i].wStation;
            pCond->wPointA = ItemInput[i].wPoint;
            pCond->wAttribA = ItemInput[i].wAttrib;
            pCond->fValueA = ItemInput[i].fValue;
            pCond->wOp = 0;
            pCond->wGroupNo = ItemInput[top-1].nGroup;
            if((ItemInput[top-1].fValue - 0) < 0.00001)
                pCond->bResult = false;
            else
                pCond->bResult = true;
            pList->append(pCond);
        }
    }

}

void getRuleFailList(QList<FORMULACONDITION*> *pCondList,QStringList* pStrErrorList,bool bGrade )
{
    if(pCondList->count() < 1) return;
    int wMaxGroupNo = 0;
    for(int i = 0; i < pCondList->count() ;i++)
    {
        FORMULACONDITION* pFormulaCond = (FORMULACONDITION*)pCondList->at(i);
        if(NULL == pFormulaCond) continue;
        if(wMaxGroupNo < pFormulaCond->wGroupNo)
            wMaxGroupNo = pFormulaCond->wGroupNo;
    }

    int nGroupIndex;
    int nIndex = 1;
    for(nGroupIndex = 1; nGroupIndex < wMaxGroupNo;nGroupIndex++)
    {
        bool bFirstItem = true;
        for(int i = 0; i < pCondList->count();i++)
        {
            FORMULACONDITION* pFormulaCond = (FORMULACONDITION*)pCondList->at(i);
            if(!pFormulaCond || pFormulaCond->bResult == 1) continue;

            if(nGroupIndex == pFormulaCond->wGroupNo)
            {
                if(bGrade && bFirstItem)
                {
                    QString strError;
                    strError = QString("第%1组条件").arg(nIndex++);
                    pStrErrorList->append(strError);
                    bFirstItem = false;
                }
            }
            else
                continue;

            if(CONDITIONTYPE_DIGITAL == pFormulaCond->btType)
            {
                QString strPointErr;
                QStringList list;
                bool bVirtualYx = false;
                if(m_btFormulaModule == MODULE_GIN)
                {

                }
                else
                {
                    if(!getPointName(pFormulaCond->wStationA,TYPE_DIGITAL,pFormulaCond->wPointA,strPointErr))
                    {
                        strPointErr = QStringLiteral("未知遥信点");
                    }
                    else
                    {
                        bVirtualYx = true;
                    }
                }

                //取错误的规则列表，正确的规则列表已经过滤
                if(pFormulaCond->wOp != OP_NOT)
                {
                    strPointErr += "实际状态为分，要求状态为合";
                }
                else // 1 == 0
                {
                    strPointErr += "实际状态为合，要求状态为分";
                }
                pStrErrorList->append(strPointErr);
                if(bVirtualYx)
                {
                    //获取虚装置的错误列表
                    for(int i = 0; i < list.count();i++)
                    {
                        QString strVirtual = "       ";
                        QString strTemp = list[i];
                        strVirtual += strTemp;
                        pStrErrorList->append(strVirtual);
                    }
                    list.clear();
                }
            }
            else if(CONDITIONTYPE_ANALOGUE == pFormulaCond->btType)
            {
                QString strItem1,strItem2;
                if(m_btFormulaModule == MODULE_GIN)
                {

                }
                else
                {
                    if(!getPointName(pFormulaCond->wStationA,TYPE_ANALOGUE,pFormulaCond->wPointA,strItem1))
                    {
                        strItem1 = QStringLiteral("未知遥信点");
                    }
                }

                if(pFormulaCond->btPointTypeB == POINTBTYPE_CONST)
                {
                    strItem2 = QString("%1").arg(pFormulaCond->fValueB);
                }
                else if(pFormulaCond->btPointTypeB == POINTBTYPE_ANALOGUE)
                {
                    if(m_btFormulaModule == MODULE_GIN)
                    {

                    }
                    else
                    {
                        if(!getPointName(pFormulaCond->wStationB,TYPE_ANALOGUE,pFormulaCond->wPointB,strItem2))
                        {
                            strItem2 = QStringLiteral("未知遥信点");
                        }
                    }
                }
                else
                    continue;
                QString strOp;
                switch (pFormulaCond->wOp) {
                case OP_GREATER:
                    strOp = ">";
                    break;
                case OP_LOWER:
                    strOp = "<";
                    break;
                case OP_EQUAL:
                    strOp = "==";
                    break;
                case OP_GEQUAL:
                    strOp = ">=";
                    break;
                case OP_LEQUAL:
                    strOp = "<=";
                    break;
                case OP_NEQUAL:
                    strOp = "!=";
                    break;
                default:
                    break;
                }
                QString strError = strItem1 + strOp + strItem2 + "不满足";
                pStrErrorList->append(strError);
            }
        }
    }
}

bool getVirtualYxFailList(ushort wStation,uchar btType,ushort wPoint,uchar btValue,QStringList& list)
{
    list.clear();
    if(0 != btValue) return false;
    if(NULL == m_lpFormulaProc) return false;
    int nDBID = 0;
    FORMULAPARAMETER param;
    param.wStation = wStation;
    param.wPoint = wPoint;
    param.btType = TYPE_NULL;
    STATION station;
    param.pBuffer = &station;
    if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&param,nDBID))
        return false;
    param.btType = btType;
    if(TYPE_DIGITAL == btType)
    {
        DIGITAL digital;
        param.pBuffer = &digital;
        if(!m_lpFormulaProc(FM_GETDBINFO,0,(LPARAM)&param,nDBID)) return false;
        ushort wFormulaNo = digital.wFormulaID;
        if(wFormulaNo != 0 && wFormulaNo != (ushort)-1)
        {
            ITEM item;
            QList<FORMULACONDITION*> pList;
            bool bResult = doFormula(wFormulaNo,&item,false,NULL,FORMULATYPE_TWO,&pList);
            if(!bResult)
            {
                QString strErr;
                strErr.arg("%1%2计算错误，请检查公式").arg(station.szStationName).arg(digital.szDigitalName);
                list.append(strErr);
                return true;
            }
        }
    }
    return false;
}
