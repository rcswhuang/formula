#include "hformuladlg.h"
#include "ui_formula.h"
#include "hformulapi.h"

extern ATTRINFO AnaAttrInfo[];
extern ATTRINFO DgtAttrInfo[];
extern ATTRINFO PulAttrInfo[];
extern LPFORMULAPROC m_lpFormulaProc;
ATTRINFOS AttrInfos[]=
{
  {TYPE_ANALOGUE,"遥测量",AnaAttrInfo},
  {TYPE_DIGITAL, "遥信量",DgtAttrInfo},
  {TYPE_PULSE,   "遥脉量",PulAttrInfo},
  {TYPE_NULL,     NULL,    NULL}
};

HFormulaDlg::HFormulaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Formula)
{
    ui->setupUi(this);

    wStation = (ushort)-1;
    wProtect = (ushort)-1;
    btType = TYPE_NULL;
}

HFormulaDlg::HFormulaDlg(FORMULA* pFormula,QWidget *parent):
    QDialog(parent),
    ui(new Ui::Formula)
{
    ui->setupUi(this);

    wStation = (ushort)-1;
    wProtect = (ushort)-1;
    btType = TYPE_NULL;
    m_pFormula = pFormula;
}

HFormulaDlg::~HFormulaDlg()
{
    delete ui;
}

void HFormulaDlg::init()
{
    connect(ui->IDC_STATION,SIGNAL(currentIndexChanged(int)),this,SLOT(onStationChanged_clicked()));
    connect(ui->IDC_COMBOMODE,SIGNAL(currentIndexChanged(int)),this,SLOT(onIntervalChanged_clicked()));
    connect(ui->IDC_TYPE,SIGNAL(currentIndexChanged(int)),this,SLOT(onTypeChanged_clicked()));
    connect(ui->IDC_ATTRIB,SIGNAL(currentIndexChanged(int)),this,SLOT(onAttribChanged_clicked()));
    connect(ui->IDC_LIST,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListWidget_doubleClicked()));

    ui->IDC_STATION->clear();
    ui->IDC_COMBOMODE->clear();//间隔
    ui->IDC_STATION->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->IDC_COMBOMODE->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    STATION station;
    FORMULAPARAMETER Param;
    Param.wStation = (ushort)-1;
    Param.btType = TYPE_NULL;
    Param.pBuffer = &station;

    for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,(LPARAM)&Param);Param.wPoint++)
    {
        ui->IDC_STATION->addItem(station.szStationName,station.wStationID);
        if(station.wStationID == wStation)
        {
            EQUIPMENTGROUP equipGroup;
            FORMULAPARAMETER Param1;
            Param1.wStation = wStation;
            Param1.btType = TYPE_GROUP;
            Param1.pBuffer = &equipGroup;
            for(Param1.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,&Param1);Param1.wPoint++)
            {
                ui->IDC_COMBOMODE->addItem(equipGroup.szGroupName,equipGroup.wGroupID);
                ui->IDC_COMBOMODE->setCurrentIndex(0);
            }

            setPointList(wStation,btType);
        }
    }

    //设置属性
    for(int i = 0; AttrInfos[i].btType != 0;i++)
    {
        ui->IDC_TYPE->addItem(AttrInfos[i].szType,AttrInfos[i].btType);

    }
    int index = ui->IDC_TYPE->findData(btType);
    if(index != -1)
    {
        ui->IDC_TYPE->setCurrentIndex(index);
        updateAttrib();
    }

    ui->IDC_FORMULA->setChecked(Qt::Checked);
    ui->IDC_INFO->setEnabled(false);

}

void HFormulaDlg::initConnect()
{
    connect(ui->IDC_0,SIGNAL(clicked(bool)),this,SLOT(idc0_clicked()));
    connect(ui->IDC_1,SIGNAL(clicked(bool)),this,SLOT(idc1_clicked()));
    connect(ui->IDC_2,SIGNAL(clicked(bool)),this,SLOT(idc2_clicked()));
    connect(ui->IDC_3,SIGNAL(clicked(bool)),this,SLOT(idc3_clicked()));
    connect(ui->IDC_4,SIGNAL(clicked(bool)),this,SLOT(idc4_clicked()));
    connect(ui->IDC_5,SIGNAL(clicked(bool)),this,SLOT(idc5_clicked()));
    connect(ui->IDC_6,SIGNAL(clicked(bool)),this,SLOT(idc6_clicked()));
    connect(ui->IDC_7,SIGNAL(clicked(bool)),this,SLOT(idc7_clicked()));
    connect(ui->IDC_8,SIGNAL(clicked(bool)),this,SLOT(idc8_clicked()));
    connect(ui->IDC_9,SIGNAL(clicked(bool)),this,SLOT(idc9_clicked()));
    connect(ui->IDC_10,SIGNAL(clicked(bool)),this,SLOT(idc10_clicked()));
    connect(ui->IDC_DOT,SIGNAL(clicked(bool)),this,SLOT(idcDot_clicked()));

    // + - * /
    connect(ui->IDC_PLUS,SIGNAL(clicked(bool)),this,SLOT(idcPlus_clicked()));
    connect(ui->IDC_MINUS,SIGNAL(clicked(bool)),this,SLOT(idcMinus_clicked()));
    connect(ui->IDC_MUTIPLY,SIGNAL(clicked(bool)),this,SLOT(idcMutiply_clicked()));
    connect(ui->IDC_DIDVDE,SIGNAL(clicked(bool)),this,SLOT(idcDidvde_clicked()));

    // > < = != >= <=
    connect(ui->IDC_GREATE,SIGNAL(clicked(bool)),this,SLOT(idcGreate_clicked()));
    connect(ui->IDC_LOWER,SIGNAL(clicked(bool)),this,SLOT(idcLower_clicked()));
    connect(ui->IDC_EQUAL,SIGNAL(clicked(bool)),this,SLOT(idcEqual_clicked()));
    connect(ui->IDC_NEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcNequal_clicked()));
    connect(ui->IDC_GEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcGequal_clicked()));
    connect(ui->IDC_LEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcLequal_clicked()));

    // （ ）
    connect(ui->IDC_LPARENTHESIS,SIGNAL(clicked(bool)),this,SLOT(idcLparenthesis_clicked()));
    connect(ui->IDC_RPARENTHESIS,SIGNAL(clicked(bool)),this,SLOT(idcRparenthesis_clicked()));

    //Dat power and or xor not
    connect(ui->IDC_DAT,SIGNAL(clicked(bool)),this,SLOT(idcDat_clicked()));
    connect(ui->IDC_POWER,SIGNAL(clicked(bool)),this,SLOT(idcPower_clicked()));
    connect(ui->IDC_AND,SIGNAL(clicked(bool)),this,SLOT(idcAnd_clicked()));
    connect(ui->IDC_OR,SIGNAL(clicked(bool)),this,SLOT(idcOr_clicked()));
    connect(ui->IDC_XOR,SIGNAL(clicked(bool)),this,SLOT(idcXor_clicked()));
    connect(ui->IDC_NOT,SIGNAL(clicked(bool)),this,SLOT(idcNot_clicked()));

    //max min ave sum maxT minT
    connect(ui->IDC_MAX,SIGNAL(clicked(bool)),this,SLOT(idcMax_clicked()));
    connect(ui->IDC_MIN,SIGNAL(clicked(bool)),this,SLOT(idcMin_clicked()));
    connect(ui->IDC_AVE,SIGNAL(clicked(bool)),this,SLOT(idcAve_clicked()));
    connect(ui->IDC_SUM,SIGNAL(clicked(bool)),this,SLOT(idcSum_clicked()));
    connect(ui->IDC_MAXT,SIGNAL(clicked(bool)),this,SLOT(idcMaxT_clicked()));
    connect(ui->IDC_MINT,SIGNAL(clicked(bool)),this,SLOT(idcMinT_clicked()));

    //1min 5min 15min 1hour 1day 1mon
    connect(ui->IDC_1MIN,SIGNAL(clicked(bool)),this,SLOT(idc1Min_clicked()));
    connect(ui->IDC_5MIN,SIGNAL(clicked(bool)),this,SLOT(idc5Min_clicked()));
    connect(ui->IDC_15MIN,SIGNAL(clicked(bool)),this,SLOT(idc15Min_clicked()));
    connect(ui->IDC_1HOUR,SIGNAL(clicked(bool)),this,SLOT(idc1Hour_clicked()));
    connect(ui->IDC_1DAY,SIGNAL(clicked(bool)),this,SLOT(idc1Day_clicked()));
    connect(ui->IDC_1MON,SIGNAL(clicked(bool)),this,SLOT(idc1Mon_clicked()));

    redoAct = new QAction();
    connect(redoAct,SIGNAL(toggled(bool)),this,SLOT(redo()));
    redoAct->setShortcut(Qt::Key_Redo);

    undoAct = new QAction();
    connect(undoAct,SIGNAL(toggled(bool)),this,SLOT(undo()));
    undoAct->setShortcut(Qt::Key_Undo);

}

void HFormulaDlg::idc0_clicked()
{
    //获取当前鼠标的位置，如果当前焦点不在edit上，自动获取鼠标位置在尾部
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
    cursor.insertText(tr("0"));
    ui->IDC_EDIT->setCursor(cursor);
}

void HFormulaDlg::idc1_clicked()
{

}

void HFormulaDlg::idc2_clicked()
{

}

void HFormulaDlg::idc3_clicked()
{

}

void HFormulaDlg::idc4_clicked()
{

}

void HFormulaDlg::idc5_clicked()
{

}

void HFormulaDlg::idc6_clicked()
{

}

void HFormulaDlg::idc7_clicked()
{

}

void HFormulaDlg::idc8_clicked()
{

}

void HFormulaDlg::idc9_clicked()
{

}

void HFormulaDlg::idc10_clicked()//$
{

}

void HFormulaDlg::idcDot_clicked()//.
{

}

void HFormulaDlg::idcPlus_clicked() //+
{

}

void HFormulaDlg::idcMinus_clicked() //-
{

}

void HFormulaDlg::idcMutiply_clicked() //*
{

}

void HFormulaDlg::idcDidvde_clicked() // /
{

}

void HFormulaDlg::idcGreate_clicked() // >
{

}

void HFormulaDlg::idcLower_clicked() // <
{

}

void HFormulaDlg::idcEqual_clicked() // =
{

}

void HFormulaDlg::idcGequal_clicked() // >=
{

}

void HFormulaDlg::idcLequal_clicked() // <=
{

}

void HFormulaDlg::idcNequal_clicked() // !=
{

}

void HFormulaDlg::idcLparenthesis_clicked() //（
{

}

void HFormulaDlg::idcRparenthesis_clicked() // ）
{

}

void HFormulaDlg::idcDat_clicked()//Dat
{

}

void HFormulaDlg::idcPower_clicked()//Power
{

}

void HFormulaDlg::idcAnd_clicked()//And
{

}

void HFormulaDlg::idcOr_clicked()//Or
{

}

void HFormulaDlg::idcXor_clicked()//Xor
{

}

void HFormulaDlg::idcNot_clicked()//Not
{

}

void HFormulaDlg::idcMax_clicked()// Max()
{

}

void HFormulaDlg::idcMin_clicked()// Min()
{

}

void HFormulaDlg::idcAve_clicked()// Ave()
{

}

void HFormulaDlg::idcSum_clicked()// Sum()
{

}

void HFormulaDlg::idcMaxT_clicked()// MaxT()
{

}

void HFormulaDlg::idcMinT_clicked()// MinT()
{

}

void HFormulaDlg::idc1Min_clicked()// 1Min()
{

}

void HFormulaDlg::idc5Min_clicked()//5Min()
{

}

void HFormulaDlg::idc15Min_clicked()//15Min()
{

}

void HFormulaDlg::idc1Hour_clicked()//1Hour()
{

}

void HFormulaDlg::idc1Day_clicked()//1Day()
{

}

void HFormulaDlg::idc1Mon_clicked()//1Month()
{

}

void HFormulaDlg::redo()
{

}

void HFormulaDlg::undo()
{

}

void HFormulaDlg::onStationChanged_clicked()
{
    int index = ui->IDC_STATION->currentIndex();
    if(-1 == index)
        return;
    ushort wStationID = ui->IDC_STATION->currentData().toUInt();
    wStation = wStationID;

    wProtect = (ushort)-1;
    ui->IDC_COMBOMODE->clear();
    EQUIPMENTGROUP equipGroup;
    FORMULAPARAMETER Param;
    Param.wStation = wStation;
    Param.btType = TYPE_GROUP;
    Param.pBuffer = &equipGroup;
    for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,&Param);Param.wPoint++)
    {
        ui->IDC_COMBOMODE->addItem(equipGroup.szGroupName,equipGroup.wGroupID);
        ui->IDC_COMBOMODE->setCurrentIndex(0);
    }

    btType = ui->IDC_TYPE->currentData().toUInt();

    if(-1 != btType)
    {
        setPointList(wStation,btType);
    }

}

void HFormulaDlg::onIntervalChanged_clicked()
{
    int index = ui->IDC_COMBOMODE->currentIndex();
    if(-1 == index)
        return;
    ushort wProtectID = ui->IDC_COMBOMODE->currentData().toUInt();
    if(wProtectID == wProtect)
        return;
    wProtect = wProtectID;
    index = ui->IDC_TYPE->currentIndex();
    if(-1 == index)
        return;
    ushort btTypeID = ui->IDC_TYPE->currentData().toUInt();
    btType = btTypeID;
    updateAttrib();
    if(-1 != btTypeID)
        setPointList(ui->IDC_STATION->currentData().toUInt(),btType);
}

void HFormulaDlg::onTypeChanged_clicked()
{
    int index = ui->IDC_TYPE->currentIndex();
    if(-1 == index)
        return;
    uchar btTypeID = ui->IDC_TYPE->currentData().toUInt();
    if(btTypeID == btType)
        return;
    btType = btTypeID;
    updateAttrib();
    if(-1 != ui->IDC_STATION->currentIndex())
        setPointList(ui->IDC_STATION->currentData().toUInt(),btType);
}

void HFormulaDlg::onAttribChanged_clicked()
{

}

void HFormulaDlg::onListWidget_doubleClicked()
{
    QString strStation;
    QString strPoint;
    QString strAttrib;
    if(-1 == ui->IDC_STATION->currentIndex())
        return;
    strStation = ui->IDC_STATION->currentText();
    if(-1 == ui->IDC_COMBOMODE->currentIndex())
        return;
    if(-1 == ui->IDC_TYPE->currentIndex())
        return;

    strPoint = ui->IDC_LIST->currentItem()->text();

    bool bfind = false;
    for(int i = 0; AttrInfos[i].pAttrInfo != NULL; i++)
    {
        if(AttrInfos[i].btType == btType)
        {
            bfind = true;
            break;
        }
    }
    if(!bfind) return;

    if(-1 == ui->IDC_ATTRIB->currentIndex())
        return;
    strAttrib = ui->IDC_ATTRIB->currentText();
    QString strText;
    if(strAttrib.isEmpty())
        strText = QString("[")+strStation+QString(".")+strPoint+QString("]");
    else
        strText = QString("[")+strStation+QString(".")+strPoint+QString(".")+strAttrib+QString("]");

    //插入文字
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.beginEditBlock();
    cursor.insertText(strText);
    cursor.endEditBlock();

    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();

    ui->IDC_EDIT->setFocus();

}

void HFormulaDlg::setPointList(ushort wStation, uchar btType)
{
    ui->IDC_LIST->clear();
    ANALOGUE analogue;
    DIGITAL digital;

    FORMULAPARAMETER Param;
    Param.wStation = wStation;
    Param.btType = btType;
    Param.pBuffer = NULL;
    switch(btType)
    {
    case TYPE_ANALOGUE:
        Param.pBuffer = &analogue;
        for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,&Param);Param.wPoint++)
        {
            if(analogue.wGroupID == wProtect){
                QListWidgetItem* item = new QListWidgetItem(ui->IDC_LIST);
                item->setText(analogue.szAnalogueName);
                item->setData(Qt::UserRole,analogue.wAnalogueID);
                ui->IDC_LIST->addItem(item);
            }
        }
        break;
    case TYPE_DIGITAL:
        Param.pBuffer = &digital;
        for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,&Param);Param.wPoint++)
        {
            if(digital.wGroupID == wProtect){
                QListWidgetItem* item = new QListWidgetItem(ui->IDC_LIST);
                item->setText(digital.szDigitalName);
                item->setData(Qt::UserRole,digital.wDigitalID);
                ui->IDC_LIST->addItem(item);
            }
        }
        break;
    }
}

void HFormulaDlg::updateAttrib()
{
    ui->IDC_ATTRIB->clear();
    for(int i = 0; AttrInfos[i].pAttrInfo != NULL;i++ )
    {
        if(AttrInfos[i].btType == btType)
        {
            ATTRINFO *attrInfo = AttrInfos[i].pAttrInfo;
            for(int j = 0; j < attrInfo[j].szAttrib != NULL;j++)
            {
                ui->IDC_ATTRIB->addItem(attrInfo[j].szAttrib,attrInfo[j].wAttrib);
            }
        }
    }
    if(ui->IDC_TYPE->count()>0)
        ui->IDC_TYPE->setCurrentIndex(0);
}