#include "hformulapi.h"
#include "hformuladlg.h"
#include "hformulaex.h"
#include "hformuladlg.h"
#include <QList>
#include <QVector>
#include <math.h>
QList<FORMULA*> m_FormulaList;
QList<ITEM*> m_ItemList;
QVector<FORMULARUN*> m_FormulaRunList;
bool m_bFormula = false;//是否加载公式
bool m_bModified = false; //是否修改
ushort m_wStation = (ushort)-1;
uchar m_btType = TYPE_NULL;
uchar m_btFormulaModule = MODULE_ID;
QString m_strFormulaText;
LPFORMULAPROC m_lpFormulaProc = NULL;
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

//初始化Formula
bool FORMULA_EXPORT initFormula(LPFORMULAPROC lpFormulaProc,uchar btModuleType /*= MODULE_ID*/)
{
    if(m_bFormula)
        return true;
    if(NULL == lpFormulaProc)
        return false;
    m_lpFormulaProc = lpFormulaProc;
    if(btModuleType == MODULE_GIN)
        btModuleType = MODULE_GIN;
    else
        btModuleType = MODULE_ID;

    return true;
}

void FORMULA_EXPORT exitFormula()
{
    if(m_bModified && saveFormulaData())
        m_bModified = false;

}

FORMULA* FORMULA_EXPORT getFormula(ushort wNo)
{
    if(!m_bFormula)
        return NULL;
    QList<FORMULA*>::iterator i;
    for(i = m_FormulaList.begin(); i != m_FormulaList.end();i++)
    {
        FORMULA* pFormula = *i;
        if(wNo == pFormula->wNo)
            return pFormula;
    }
    return NULL;
}

bool FORMULA_EXPORT loadFormulaData(QList<FORMULA*> pFormulaList,QList<ITEM*> pItemList)
{
    if(!loadFormulaData(&pFormulaList,&pItemList)) return false;
    m_bFormula = true;
    return true;
}

bool FORMULA_EXPORT saveFormulaData()
{
    if(!m_bModified)
        return true;
    FORMULAITEMLIST param;
    param.pFormulaList = &m_FormulaList;
    param.pItemList = &m_ItemList;
    if(!m_lpFormulaProc(FM_SAVEFORMULALIST,0,(LPARAM)&param,0))//huangw
    {
        m_bModified = false;
        return true;
    }
    return false;
}

void FORMULA_EXPORT setFormulaModified(bool modified)
{
    m_bModified = modified;
}

bool FORMULA_EXPORT getFormulaModified()
{
    return m_bModified;
}

bool FORMULA_EXPORT addFormula(FORMULA* pFormula,ushort wNo,uchar btType /*= FORMULATYPE_NORMAL*/)
{
    if(!m_bFormula || NULL == pFormula)
        return false;

    FORMULA* pNewFormula = NULL;
    if((ushort)wNo != -1)
        pNewFormula = getFormula(wNo);
    if(NULL == pNewFormula)
    {
        pNewFormula = new FORMULA;
        if(NULL == pNewFormula)
            return false;

        ushort wNewNo = getMaxFormulaNo() + 1;
        if(wNewNo < 1)
            wNewNo = 1;
        pNewFormula->wNo = wNewNo;
        pNewFormula->wMessage = (ushort)-1;
        pNewFormula->btType = btType;

        if(!insertFormula(pNewFormula))
        {
            delete pNewFormula;
            pNewFormula = NULL;
        }
    }
    //pNewFormula是插入的公式里面的对象，pFormula是另外一个对象，pFormula和pNewFormula是一模一样的两个对象
    memcpy(pNewFormula->wFormula,pFormula->wFormula,sizeof(ushort)*FORMULALEN);
    pNewFormula->btType = btType;
    pFormula->wNo = pNewFormula->wNo;

    m_bModified = true;
    if(!loadRunFormulaList()) return false;
    return true;
}

void FORMULA_EXPORT deleteFormula(ushort wNo)
{
    FORMULA* pFormula = getFormula(wNo);
    if(NULL == pFormula)
        return;
    int index = m_FormulaList.indexOf(pFormula);
    m_FormulaList.removeAt(index);
    m_bModified = false;
    delete pFormula;
}

bool FORMULA_EXPORT createFormula(FORMULA* pFormula,ushort wNo)//创建某个测点wNo的公式
{
    if(!m_bFormula || NULL == pFormula)
        return false;
    if((ushort)-1 != wNo)
    {
        FORMULA* pTemp = getFormula(wNo);
        if(NULL != pTemp)
        {
            *pFormula = *pTemp;//why???----huangw 这是浅拷贝，直接将值赋过去，因为结构没有指针
        }
    }

    HFormulaDlg dlg(pFormula);
    dlg.setStation(m_wStation);
    dlg.setType(m_btType);
    if(QDialog::Accepted == dlg.exec())
    {
        m_wStation = dlg.getStation();
        m_btType = dlg.getType();
        return true;
    }

    return false;
}

bool FORMULA_EXPORT compileFormula(const char* szFormula,FORMULA* pFormula)//编译公式
{

}

QString FORMULA_EXPORT getFormulaText(FORMULA* pFormula,bool bValue)
{
    if(!m_bFormula || NULL == pFormula)
        return NULL;
    QString string1,string2;
    //ops结构存放的是解析后的表达式最后变成1个完成的长度，操作符也是最后一个终极操作符
    /*  规则应该是一个中序排序存放在pFormlua里面的wFormula数组内
     *           *
     *       +       +
     *     1   2   3   4
     * 适合二元运算符
     */
    struct {
        int nCount;
        ushort wOperator;
    }ops[FORMULALEN];

    m_strFormulaText.clear();
    if(0 == pFormula->wFormula[0])
        return m_strFormulaText;

    uchar btHstType = (uchar)-1;
    int top = 0,k = 0;
    //注意公式是栈模式操作，栈内(数组)从后面开始入栈，一直往前放数据
    while(k < FORMULALEN && pFormula->wFormula[k] != 0)//逐个进行分析
    {
        ushort wNo = pFormula->wFormula[k++];
        if(ISFORMULAITEM(wNo))//0-9 ( ) .等 具体公式项
        {
            if(m_btFormulaModule == MODULE_GIN)
            {
                if(!getItemStringForGIN(wNo,string1,bValue))
                    string1.clear();
            }
            else
            {
                if(!getItemString(wNo,string1,bValue))
                    string2.clear();
            }

            ITEM* pItem = getItem(wNo,&m_ItemList);
            //好像读取遥测和遥脉的历史值字符串  ？？？？？---huangw
            if(NULL != pItem && (ITEM_ANAHST == pItem->btType || ITEM_PULHST == pItem->btType))
            {
                if(top <= 0)
                    continue;
            }
            else
            {
                //读取数字等ITEM的字符串信息
                m_strFormulaText = m_strFormulaText + string1;
                if(NULL != pItem && (ITEM_ANATIME == pItem->btType || ITEM_PULTIME == pItem->btType))
                    btHstType = pItem->btReserver;
                ops[top].nCount = string1.length();
                ops[top++].wOperator = wNo;
            }
            continue;
        }

        if(ISONEOPERATOR(wNo)) //一元运算符，表示是对前一个数据进行一元操作
        {
            if(0 == top || OP_UPLUS == wNo)
                continue;

            string1 = m_strFormulaText.right(ops[top-1].nCount);
            m_strFormulaText = m_strFormulaText.left(m_strFormulaText.length() - ops[top-1].nCount);

            //如果top-1也是操作符，就要判断操作符优先级
            //如果top-1的操作符的优先级 < wNo的优先级，则前面所有的部分就要加()
            if(fOperator(ops[top-1].wOperator) < gOperator(wNo))
            {
                string1 = "( " + string1 + " )";
                ops[top-1].nCount += 4;
            }
            switch (wNo) {
            case OP_UMINUS:
                string1 = "-"+string1;
                ops[top-1].nCount++;
                break;
            case OP_NOT:
                string1 = "~"+string1;
                ops[top-1].nCount++;
                break;
            default:
                break;
            }
            m_strFormulaText = m_strFormulaText + string1;
            ops[top-1].wOperator = wNo;
            continue;
        }

        //对于二元运算符 就是将二元运算左右的部分合并成一个ops对象存放，wOperator是左右合并的运算符
        if(ISTWOOPERATOR(wNo)) //二元运算符，表示对前面两个数据进行二元运算
        {
            if(top <= 1)
                continue;
            top--;
            string2 = m_strFormulaText.right(ops[top].nCount);//1+2 ---wOperator= +
            m_strFormulaText = m_strFormulaText.left(m_strFormulaText.length() - ops[top].nCount);
            string1 = m_strFormulaText.right(ops[top-1].nCount);//3+4 ---Operator = +
            m_strFormulaText = m_strFormulaText.left(m_strFormulaText.length() - ops[top-1].nCount);
            //wNo = *  由于左右两边的运算符优先级都比较低 所以加括号
            if(fOperator(ops[top].wOperator) < gOperator(wNo))
            {
                string2 = "( " + string2 + " )";
                ops[top].nCount += 4;
            }

            if(fOperator(ops[top-1].wOperator) < gOperator(wNo))
            {
                string1 = "( " + string1 + " )";
                ops[top-1].nCount += 4;
            }

            int nCount = 0;
            switch (wNo) {
            case OP_PLUS:
                string1 = string1 + " + ";
                nCount = 3;
                break;
            case OP_MINUS:
                string1 = string1 + " - ";
                nCount = 3;
                break;
            case OP_MULTIPLY:
                string1 = string1 + " * ";
                nCount = 3;
                break;
            case OP_DIVIDE:
                string1 = string1 + " / ";
                nCount = 3;
                break;
            case OP_POW:
                string1 = string1 + " ^ ";
                nCount = 3;
                break;
            case OP_GREATER:
                string1 = string1 + " > ";
                nCount = 3;
                break;
            case OP_LOWER:
                string1 = string1 + " < ";
                nCount = 3;
                break;
            case OP_EQUAL:
                string1 = string1 + " = ";
                nCount = 3;
                break;
            case OP_GEQUAL:
                string1 = string1 + " >= ";
                nCount = 4;
                break;
            case OP_LEQUAL:
                string1 = string1 + " <= ";
                nCount = 4;
                break;
            case OP_NEQUAL:
                string1 = string1 + " != ";
                nCount = 4;
                break;
            case OP_AND:
                string1 = string1 + " & ";
                nCount = 3;
                break;
            case OP_OR:
                string1 = string1 + " | ";
                nCount = 3;
                break;
            case OP_XOR:
                string1 = string1 + " XOR ";
                nCount = 5;
                break;
            default:
                break;
            }
            string1 = string1 + string2;
            m_strFormulaText = m_strFormulaText + string1;
            ops[top-1].nCount += nCount;
            ops[top-1].wOperator = wNo;
            continue;
        }

        //对于MAX类的运算符 注意MAX(A,B) 要搞清楚这类结构是如何存入pFormula里面的 中序？后序？
        if(ISFUNCTION(wNo))
        {
            if(top <=2)
                continue;
            int nCount1= ops[top-1].nCount;
            int nCount2 = ops[top-2].nCount + ops[top-3].nCount;

            string1 = m_strFormulaText.right(nCount1);//A
            m_strFormulaText = m_strFormulaText.left(m_strFormulaText.length() - nCount1);

            string2 = m_strFormulaText.right(nCount2);
            m_strFormulaText = m_strFormulaText.left(m_strFormulaText.length() - nCount2);
            //???? huangw
            switch (wNo) {
            case OP_MAX:
                string1 = "MAX( " + string1 + string2 + " )";//MAX( A,B ),string1=A string2=B
                break;
            case OP_MIN:
                string1 = "MIN( " + string1 + string2 + " )";
                break;
            case OP_AVE:
                string1 = "AVE( " + string1 + string2 + " )";
                break;
            case OP_MAXT:
                string1 = "MAXT( "+ string1 + string2 + " )";
                break;
            case OP_MINT:
                string1 = "MINT( " + string1 + string2 + " )";
                break;
            default:
                break;
            }
            m_strFormulaText = m_strFormulaText + string1;
            top-=3;
            ops[top].nCount = string1.length();
            ops[top].wOperator = wNo;
            continue;
        }
    }
    return m_strFormulaText;
}

//在pFormula的里面寻找item,如果找到pOld,就用pNew替换,ITEM必须是数字 字符等
void FORMULA_EXPORT replaceFormulaItem(FORMULA* pFormula,ITEM* pOld,ITEM* pNew)
{
    if(NULL == pFormula || NULL == pOld || NULL == pNew)
        return;
    for(int i = 0;  pFormula->wFormula[i] != 0 && i < FORMULALEN;i++)
    {
        ushort wNo = pFormula->wFormula[i];
        if(ISFORMULAITEM(wNo))
        {
            ITEM* item = getItem(wNo,&m_ItemList);
            if(!item && item->btType == pOld->btType) //要判断类型是不是一样 这是关键
            {
                //将item的相关项拷贝到pOld和pNew里面 其实这个不需要 只要pOld pNew里面设置好就可以了。
                switch(item->btType)
                {
                case ITEM_ANATIME:
                case ITEM_PULTIME:
                    if((uchar)-1 == pOld->btReserver)
                        pOld->btReserver = item->btReserver;
                    if((uchar)-1 == pNew->btReserver)
                        pNew->btReserver = item->btReserver;
                    break;
                case ITEM_ANALOGUE:
                case ITEM_DIGITAL:
                case ITEM_PULSE:
                case ITEM_ANAHST:
                case ITEM_PULHST:
                    if((ushort)-1 == pOld->DbWord.wAttrib)
                        pOld->DbWord.wAttrib = item->DbWord.wAttrib;
                    if((ushort)-1 == pNew->DbWord.wAttrib)
                        pNew->DbWord.wAttrib = item->DbWord.wAttrib;
                    break;

                }
                if(isItemEqual(item,pOld))//找到和pOld相等的Item了，替换
                {
                    ITEM* tmpItem = addItem(pNew);
                    if(NULL != tmpItem)
                        pFormula->wFormula[i] = tmpItem->wNo;
                }
            }
        }
    }
}

void FORMULA_EXPORT onFormulaIdle()
{
    if(m_bModified)
        return;
}

bool FORMULA_EXPORT doFormula(ushort wNo,ITEM* item,bool bHst,struct tm* ptm,uchar btEType,QList<FORMULACONDITION*>* pList,int nDBID)
{
    if(!m_bFormula || NULL == item || bHst || NULL == ptm || NULL == m_lpFormulaProc)
        return false;
    FORMULARUN *pFormulaRun = getFormulaRun(wNo);
    if(NULL == pFormulaRun)
        return false;
    FORMULA *pFormula = pFormulaRun->pFormula;
    if(NULL == pFormula)
        return false;

    //---huangw
    time_t lTime = time(NULL);
    struct tm tm = *localtime(&lTime);
    ITEM ItemValue[FORMULALEN];
    memset(ItemValue,0,sizeof(ITEM)*FORMULALEN);
    int top = 0,k=0;
    while(k < FORMULALEN && pFormula->wFormula[k] != 0)
    {
        ushort wNo = pFormula->wFormula[k++];
        if(ISFORMULAITEM(wNo))
        {
            ITEM *pItem = pFormulaRun->pItem[k-1];
            if(NULL == pItem)
                return false;
            switch(pItem->btType)
            {
            case ITEM_ANALOGUE:
            case ITEM_DIGITAL:
            case ITEM_PULSE:
            {
                FORMULAPARAMETER param;
                param.wStation = pItem->DbWord.wStation;
                param.wPoint = pItem->DbWord.wPoint;
                param.wAttrib = pItem->DbWord.wAttrib;
                param.pItem = &ItemValue[top];
                param.ptm1 = ptm;
                param.btType = TYPE_ANALOGUE;
                if(ITEM_DIGITAL == pItem->btType)
                    param.btType = TYPE_DIGITAL;
                else if(ITEM_PULSE == pItem->btType)
                    param.btType = TYPE_PULSE;
                if(!m_lpFormulaProc((bHst?FM_GETHSTATTR:FM_GETDBATTR),0,(LPARAM)&param,nDBID))
                    return false;
                top++;
            }
                break;

            case ITEM_ANAHST:
            case ITEM_PULHST:
            {
                if(ItemValue[top-1].btType != ITEM_TIME)
                    return false;
                struct tm tmHst;
                memset(&tmHst,0,sizeof(struct tm));
                tmHst.tm_year = ItemValue[top-1].ItemTime.year;
                tmHst.tm_mon = ItemValue[top-1].ItemTime.mon;
                tmHst.tm_min = ItemValue[top-1].ItemTime.min;
                tmHst.tm_hour = ItemValue[top-1].ItemTime.hour;
                tmHst.tm_min = ItemValue[top-1].ItemTime.min;

                FORMULAPARAMETER param;
                param.wStation = pItem->DbWord.wStation;
                param.wPoint = pItem->DbWord.wPoint;
                param.wAttrib = pItem->DbWord.wAttrib;
                param.pItem = &ItemValue[top-1];
                param.ptm1 = &tmHst;
                param.btType = TYPE_ANALOGUE;
                if(TYPE_PULSE == pItem->btType)
                    param.btType = TYPE_PULSE;
                if(!m_lpFormulaProc(FM_GETHSTATTR,0,(LPARAM)&param,nDBID))
                    return false;

                break;
            }
            case ITEM_TIME:
             {
                ItemValue[top++] = *pItem;
                bool bAdjusted =false;
                int year = 0,mon = 0,day = 0,hour=0,min=0;
                if(pItem->ItemTime.flag & ITEMTIME_YEAR)
                {
                    ItemValue[top-1].ItemTime.year = bHst?pItem->ItemTime.year:tm.tm_year;//ItemTime.year是历史时间，tm是当前时间
                    year = pItem->ItemTime.year;
                    bAdjusted = true;
                }
                if(pItem->ItemTime.flag & ITEMTIME_MON)
                {
                    ItemValue[top-1].ItemTime.mon = bHst?pItem->ItemTime.mon:tm.tm_mon;//ItemTime.year是历史时间，tm是当前时间
                    mon = pItem->ItemTime.mon;
                    bAdjusted = true;
                }
                if(pItem->ItemTime.flag & ITEMTIME_DAY)
                {
                    ItemValue[top-1].ItemTime.day = bHst?pItem->ItemTime.day:tm.tm_mday;//ItemTime.year是历史时间，tm是当前时间
                    day = pItem->ItemTime.day;
                    bAdjusted = true;
                }
                if(pItem->ItemTime.flag & ITEMTIME_HOUR)
                {
                    ItemValue[top-1].ItemTime.hour = bHst?pItem->ItemTime.hour:tm.tm_hour;//ItemTime.year是历史时间，tm是当前时间
                    hour = pItem->ItemTime.hour;
                    bAdjusted = true;
                }
                if(pItem->ItemTime.flag & ITEMTIME_MIN)
                {
                    ItemValue[top-1].ItemTime.min = bHst?pItem->ItemTime.min:tm.tm_min;//ItemTime.year是历史时间，tm是当前时间
                    min = pItem->ItemTime.min;
                    bAdjusted = true;
                }
                if(bAdjusted)
                    adjustTime(&ItemValue[top-1],year,mon,day,hour,min);
                break;
            }
            case ITEM_FLOAT:
            case ITEM_BOOLEAN:
            {
                ItemValue[top++] = *pItem;
                break;
            }

            case ITEM_ANATIME:
            case ITEM_PULTIME:
            {
                ItemValue[top++] = *pItem;
                break;
            }
            default:
                return false;
            }
            continue;
        }
        if(ISONEOPERATOR(wNo))
        {
            if(top<=0)
                continue;
            if(OP_UPLUS == wNo)
                continue;
            switch (wNo) {
            case OP_UMINUS:
                if(ITEM_FLOAT == ItemValue[top-1].btType)
                {
                    if(INVALID_FLOAT != ItemValue[top-1].fValue)
                        ItemValue[top-1].fValue = -ItemValue[top-1].fValue;
                }
                else
                    return false;
                break;
            case OP_NOT:
                if(ITEM_FLOAT == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].fValue = (uchar)(ItemValue[top-1].fValue + 0.5f);
                    ItemValue[top-1].btType = ITEM_BOOLEAN;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].bValue = !ItemValue[top-1].bValue;
                }
                else
                    return false;
                break;
            default:
                return false;
            }
            continue;
        }
        if(ISTWOOPERATOR(wNo))
        {
            if(top<=1)
                continue;
            switch (wNo) {
            case OP_PLUS:      //top-2应该是A top-1应该是B 结果A=A+B
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    if(INVALID_FLOAT == ItemValue[top-1].fValue || INVALID_FLOAT == ItemValue[top-2].fValue)
                        ItemValue[top-2].fValue = INVALID_FLOAT;
                    else
                        ItemValue[top-2].fValue += ItemValue[top-1].fValue;
                }
                else
                    return false;
                break;
            case OP_MINUS:  //top-2应该是A top-1应该是B 结果A=A-B
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    if(INVALID_FLOAT == ItemValue[top-1].fValue || INVALID_FLOAT == ItemValue[top-2].fValue)
                        ItemValue[top-2].fValue = INVALID_FLOAT;
                    else
                        ItemValue[top-2].fValue -= ItemValue[top-1].fValue;
                }
                else
                    return false;
                break;
            case OP_MULTIPLY:
                if(ITEM_BOOLEAN == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].fValue = (float)ItemValue[top-2].bValue;
                    ItemValue[top-2].btType = ITEM_FLOAT;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].fValue = (float)ItemValue[top-1].bValue;
                    ItemValue[top-1].btType = ITEM_FLOAT;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    if(INVALID_FLOAT == ItemValue[top-1].fValue || INVALID_FLOAT == ItemValue[top-2].fValue)
                        ItemValue[top-2].fValue = INVALID_FLOAT;
                    else
                        ItemValue[top-2].fValue *= ItemValue[top-1].fValue;
                }
                else
                    return false;
                break;
            case OP_DIVIDE:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType && 0!= ItemValue[top-1].fValue)
                {
                    if(INVALID_FLOAT == ItemValue[top-1].fValue || INVALID_FLOAT == ItemValue[top-2].fValue)
                        ItemValue[top-2].fValue = INVALID_FLOAT;
                    else
                        ItemValue[top-2].fValue /= ItemValue[top-1].fValue;
                }
                else
                    return false;
                break;
            case OP_POW:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    if(INVALID_FLOAT == ItemValue[top-1].fValue || INVALID_FLOAT == ItemValue[top-2].fValue)
                        ItemValue[top-2].fValue = INVALID_FLOAT;
                    else
                        ItemValue[top-2].fValue = (float)pow(ItemValue[top-2].fValue,ItemValue[top-1].fValue);
                }
                else
                    return false;
                break;
            case OP_GREATER:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue > ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_LOWER:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue < ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_EQUAL:
                if(ITEM_BOOLEAN == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].fValue = (float)ItemValue[top-2].bValue;
                    ItemValue[top-2].btType = ITEM_FLOAT;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].fValue = (float)ItemValue[top-1].bValue;
                    ItemValue[top-1].btType = ITEM_FLOAT;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue == ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_GEQUAL:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue >= ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_LEQUAL:
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue <= ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_NEQUAL:
                if(ITEM_BOOLEAN == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].fValue = (float)ItemValue[top-2].bValue;
                    ItemValue[top-2].btType = ITEM_FLOAT;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].fValue = (float)ItemValue[top-1].bValue;
                    ItemValue[top-1].btType = ITEM_FLOAT;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType && ITEM_FLOAT == ItemValue[top-2].btType &&
                   INVALID_FLOAT != ItemValue[top-1].fValue && ITEM_FLOAT != ItemValue[top-2].fValue)
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].fValue != ItemValue[top-2].fValue);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                else
                    return false;
                break;
            case OP_AND:
                if(ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].bValue = (uchar)(ItemValue[top-2].fValue+0.5f);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].bValue = (uchar)(ItemValue[top-1].bValue+0.5f);
                    ItemValue[top-1].btType = ITEM_BOOLEAN;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType && ITEM_BOOLEAN == ItemValue[top-2].btType )
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].bValue && ItemValue[top-2].bValue);
                }
                else
                    return false;
                break;
            case OP_OR:
                if(ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].bValue = (uchar)(ItemValue[top-2].fValue+0.5f);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].bValue = (uchar)(ItemValue[top-1].bValue+0.5f);
                    ItemValue[top-1].btType = ITEM_BOOLEAN;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType && ITEM_BOOLEAN == ItemValue[top-2].btType )
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].bValue || ItemValue[top-2].bValue);
                }
                else
                    return false;
                break;
            case OP_XOR:
                if(ITEM_FLOAT == ItemValue[top-2].btType)
                {
                    ItemValue[top-2].fValue = (uchar)(ItemValue[top-2].fValue+0.5f);
                    ItemValue[top-2].btType = ITEM_BOOLEAN;
                }
                if(ITEM_FLOAT == ItemValue[top-1].btType)
                {
                    ItemValue[top-1].fValue = (uchar)(ItemValue[top-1].bValue+0.5f);
                    ItemValue[top-1].btType = ITEM_BOOLEAN;
                }
                if(ITEM_BOOLEAN == ItemValue[top-1].btType && ITEM_BOOLEAN == ItemValue[top-2].btType )
                {
                    ItemValue[top-2].bValue = (ItemValue[top-1].bValue && !(ItemValue[top-2].bValue))||(ItemValue[top-2].bValue && !(ItemValue[top-1].bValue));
                }
                else
                    return false;
                break;
            default:
                return false;
            }
            top--;
            continue;
        }
        if(ISFUNCTION(wNo))
        {

        }
        if(ISMULTIFUNCTION(wNo))//Max(A,B) //2个
        {
            if(k > FORMULALEN || 0 == pFormula->wFormula[k])
                return false;
            ushort wNum = pFormula->wFormula[k++];
            if((ushort)top < wNum)//读取存放Max,Min等里面个数  栈内存放位置:A,B,MAX,2 -->top = 2,wNum = 2
                return false;
            int i = top - wNum;//top此时已经移动到MAX这个位置
            for(int j = i; j < top;j++)
                if(ITEM_FLOAT != ItemValue[j].btType)
                    return false;
            switch(wNo)
            {
            case OP_MMAX:
                for(int t = i+1; t < top;t++)//最后要通过移动top到i来完成ItemValue从t到top的释放，保证ItemValue[i]存放最后结果
                    if(INVALID_FLOAT != ItemValue[t].btType && ItemValue[t].fValue > ItemValue[i].fValue)
                        ItemValue[i].fValue = ItemValue[t].fValue;
                break;
            case OP_MMIN:
                for(int t = i+1; t < top;t++)//最后要通过移动top到i来完成ItemValue从t到top的释放，保证ItemValue[i]存放最后结果
                    if(INVALID_FLOAT != ItemValue[t].btType && ItemValue[t].fValue < ItemValue[i].fValue)
                        ItemValue[i].fValue = ItemValue[t].fValue;
                break;
            case OP_MSUM:
            case OP_MAVE:
                float sum;
                int num;
                for(int j = i; j < top;j++)
                {
                    if(INVALID_FLOAT != ItemValue[j].btType)
                    {
                        sum += ItemValue[j].fValue;
                        num++;
                    }
                }
                if(num > 0)
                    ItemValue[i].fValue = (OP_MSUM == wNo)?sum:sum/num;
                break;
            default:
                return false;
            }
            top = top - wNum + 1;
            continue;
        }
        return false;//规则表达式不能有错，如果过程中有不对ITEM_XXX就要返回错误
    }

    if(1 != top)
        return false;

    *item = ItemValue[0];
    switch (btEType)
    {
    case ITEM_FLOAT:
        if(item->btType == ITEM_BOOLEAN)
        {
            item->btType = ITEM_FLOAT;
            item->fValue = (float)item->bValue;
        }
        break;
    case ITEM_BOOLEAN:
        if(item->btType == ITEM_FLOAT)
        {
            item->btType = ITEM_BOOLEAN;
            item->bValue = (uchar)(item->fValue + 0.5f);
        }
        break;
    }
    return true;
}

bool FORMULA_EXPORT doRuleFormula(ushort wNo,QStringList* pList,int nDBID )
{
    QList<FORMULACONDITION*> pCondList;
    ITEM item;
    memset(&item,0,sizeof(ITEM));
    bool bValue = false;
    bool bResult = doFormula(wNo,&item,false,NULL,FORMULATYPE_TWO,&pCondList);
    if(!bResult)
    {
        QString szMsg = QStringLiteral("规则执行失败，请检查规则组态");
        if(pList)
            pList->append(szMsg);
        bValue = false;
    }
    else
    {
        if(!item.bValue && pList != NULL)
        {
            //

        }
        bValue = item.bValue;
    }
    while(!pCondList.isEmpty())
        delete (FORMULACONDITION*)pCondList.takeFirst();
    return bValue;
}

bool FORMULA_EXPORT checkRuleFormulaConflict(ushort wNo,ushort wStation,ushort wDigitalNo)
{
    if(!m_bFormula || NULL == m_lpFormulaProc)
        return false;
    FORMULARUN* pFormulaRun = getFormulaRun(wNo);
    if(NULL == pFormulaRun)
        return false;
    FORMULA* pFormula = pFormulaRun->pFormula;
    if(NULL == pFormula)
        return false;
    QList<ITEM*> pItemList;
    int k = 0;
    while(k < FORMULALEN && pFormula->wFormula[k] != 0)
    {
        ITEM* item = pFormulaRun->pItem[k];
        pItemList.append(item);
        k++;
    }

    for(k = 0; k < pItemList.count();k++)
    {
        ITEM* item = (ITEM*)pItemList[k];
        if(NULL == item) continue;
        if(ITEM_DIGITAL != item->btType) continue;
        if(item->DbWord.wStation == (ushort)-1 || item->DbWord.wPoint == (ushort)-1)
            continue;
        if(wStation == item->DbWord.wStation && wDigitalNo == item->DbWord.wPoint)
            return true;
    }
    return false;
}










