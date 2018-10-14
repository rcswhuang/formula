#if defined (_MSC_VER) && (_MSC_VER >=1600)
#pragma execution_character_set("utf-8")
#endif
#include "hformuladlg.h"
#include "ui_formula.h"
#include "expr.h"
#include <QTextCodec>


extern LPFORMULAPROC m_lpFormulaProc;
extern ATTRINFO AnaAttrInfo[];
extern ATTRINFO DgtAttrInfo[];
extern ATTRINFO PulAttrInfo[];
ATTRINFOS AttrInfos[]=
{
  {TYPE_ANALOGUE,"遥测量",AnaAttrInfo},
  {TYPE_DIGITAL, "遥信量",DgtAttrInfo},
  {TYPE_PULSE,   "遥脉量",PulAttrInfo},
  {TYPE_NULL,     NULL,    NULL}
};


QString strNum = "QPushButton { \
        background-color:#16A085;padding: 4px;min-width: 5px;min-height: 5px;border:none;color:#ffffff} \
        QPushButton:hover {background-color: #999;} \
        QPushButton:pressed {background-color: #333;color: #AAA;} \
        QPushButton:disabled {color: #333333;}";
QString strOper = "QPushButton { \
        background-color:#D35400;border:none;color:#ffffff;padding: 4px;min-width: 5px;min-height: 5px;} \
        QPushButton:hover {background-color: #999;} \
        QPushButton:pressed {background-color: #333;color: #AAA;} \
        QPushButton:disabled {color: #333333;}";

QString strComp = "QPushButton { \
        background-color:#104E8B;border:none;color:#ffffff;padding: 4px;min-width: 5px;min-height: 5px;} \
        QPushButton:hover {background-color: #999;} \
        QPushButton:pressed {background-color: #333;border-color: #555;color: #AAA;} \
        QPushButton:disabled {color: #333333;}";

QString strLabel = "QLabel{background-color:#00008B;color:#ffffff;}";

QString LineEdit = "QTextEdit{ \
        font-family:宋体;font-size:16px;height:50px;border-radius:5px \
        }";

QString ComboBox = "QComboBox{ \
        font-family:宋体;font-size:20px;height:50px;border-radius:5px \
        }";

HFormulaDlg::HFormulaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Formula)
{
    ui->setupUi(this);

    wStation = (ushort)-1;
    wProtect = (ushort)-1;
    //btType = TYPE_NULL;
    m_pFormula = NULL;
    initConnect();
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
    //initConnect();
    //init();
    connect(ui->okBtn,SIGNAL(clicked(bool)),this,SLOT(onOk()));
    connect(ui->cancleBtn,SIGNAL(clicked(bool)),this,SLOT(onCancle()));
    setWindowTitle("公式计算");
    setWindowIcon(QIcon(":/images/calculator2.png"));
}

HFormulaDlg::~HFormulaDlg()
{
    delete ui;
}

void HFormulaDlg::init()
{
    ui->IDC_STATION->clear();
    ui->IDC_COMBOMODE->clear();//间隔
    ui->IDC_STATION->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    ui->IDC_COMBOMODE->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    //先设置类型
    ui->IDC_STATION->addItem("");
    ui->IDC_TYPE->addItem("");
    ui->IDC_COMBOMODE->addItem("");
    ui->IDC_ATTRIB->addItem("");
    ui->IDC_STATION->setCurrentIndex(0);
    ui->IDC_COMBOMODE->setCurrentIndex(0);
    ui->IDC_TYPE->setCurrentIndex(0);
    ui->IDC_ATTRIB->setCurrentIndex(0);
    for(int i = 0; AttrInfos[i].btType != 0;i++)
    {
        ui->IDC_TYPE->addItem(AttrInfos[i].szType,AttrInfos[i].btType);

    }

    STATION station;
    FORMULAPARAMETER Param;
    Param.wStation = (ushort)-1;
    Param.btType = TYPE_NULL;
    Param.pBuffer = &station;

    for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,(HLPARAM)&Param,0);Param.wPoint++)//读取厂站的信息，从0开始，注意wPoint此时是厂站的ID
    {
        ui->IDC_STATION->addItem(station.szStationName,station.wStationID);
    }

    /*int index = ui->IDC_TYPE->findData(btType);
    if(index != -1)
    {
        ui->IDC_TYPE->setCurrentIndex(index);
        updateAttrib();
    }*/

    //ui->IDC_FORMULA->setChecked(Qt::Checked);
    //ui->IDC_INFO->setEnabled(false);
    if(m_pFormula->wFormula[0]!=0)
    {
        ui->IDC_EDIT->clear();
        QString text = getFormulaText(m_pFormula,false);
        if(!text.isEmpty())
        {
            ui->IDC_EDIT->setFocus();
            QTextCursor cursor = ui->IDC_EDIT->textCursor();
            cursor.clearSelection();
            cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
            cursor.insertText(text);
            ui->IDC_EDIT->setTextCursor(cursor);
        }
    }

    initConnect();
    connect(ui->IDC_STATION,SIGNAL(currentIndexChanged(int)),this,SLOT(onStationChanged_clicked(int)));
    connect(ui->IDC_COMBOMODE,SIGNAL(currentIndexChanged(int)),this,SLOT(onIntervalChanged_clicked(int)));
    connect(ui->IDC_TYPE,SIGNAL(currentIndexChanged(int)),this,SLOT(onTypeChanged_clicked(int)));
    connect(ui->IDC_ATTRIB,SIGNAL(currentIndexChanged(int)),this,SLOT(onAttribChanged_clicked(int)));
    connect(ui->IDC_LIST,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(onListWidget_doubleClicked()));
}

void HFormulaDlg::initConnect()
{
    //ui->IDC_STATION->setStyleSheet(ComboBox);
    //ui->IDC_TYPE->setStyleSheet(ComboBox);
    //ui->IDC_COMBOMODE->setStyleSheet(ComboBox);
    //ui->IDC_ATTRIB->setStyleSheet(ComboBox);
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
    ui->IDC_0->setStyleSheet(strNum);
    ui->IDC_1->setStyleSheet(strNum);
    ui->IDC_2->setStyleSheet(strNum);
    ui->IDC_3->setStyleSheet(strNum);
    ui->IDC_4->setStyleSheet(strNum);
    ui->IDC_5->setStyleSheet(strNum);
    ui->IDC_6->setStyleSheet(strNum);
    ui->IDC_7->setStyleSheet(strNum);
    ui->IDC_8->setStyleSheet(strNum);
    ui->IDC_9->setStyleSheet(strNum);
    ui->IDC_10->setStyleSheet(strNum);
    ui->IDC_DOT->setStyleSheet(strNum);

    // + - * /
    connect(ui->IDC_PLUS,SIGNAL(clicked(bool)),this,SLOT(idcPlus_clicked()));
    connect(ui->IDC_MINUS,SIGNAL(clicked(bool)),this,SLOT(idcMinus_clicked()));
    connect(ui->IDC_MUTIPLY,SIGNAL(clicked(bool)),this,SLOT(idcMutiply_clicked()));
    connect(ui->IDC_DIDVDE,SIGNAL(clicked(bool)),this,SLOT(idcDidvde_clicked()));
    ui->IDC_PLUS->setStyleSheet(strOper);
    ui->IDC_MINUS->setStyleSheet(strOper);
    ui->IDC_MUTIPLY->setStyleSheet(strOper);
    ui->IDC_DIDVDE->setStyleSheet(strOper);
    // > < = != >= <=
    connect(ui->IDC_GREATE,SIGNAL(clicked(bool)),this,SLOT(idcGreate_clicked()));
    connect(ui->IDC_LOWER,SIGNAL(clicked(bool)),this,SLOT(idcLower_clicked()));
    connect(ui->IDC_EQUAL,SIGNAL(clicked(bool)),this,SLOT(idcEqual_clicked()));
    connect(ui->IDC_NEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcNequal_clicked()));
    connect(ui->IDC_GEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcGequal_clicked()));
    connect(ui->IDC_LEQUAL,SIGNAL(clicked(bool)),this,SLOT(idcLequal_clicked()));
    ui->IDC_GREATE->setStyleSheet(strComp);
    ui->IDC_LOWER->setStyleSheet(strComp);
    ui->IDC_EQUAL->setStyleSheet(strComp);
    ui->IDC_NEQUAL->setStyleSheet(strComp);
    ui->IDC_GEQUAL->setStyleSheet(strComp);
    ui->IDC_LEQUAL->setStyleSheet(strComp);
    // （ ）
    connect(ui->IDC_LPARENTHESIS,SIGNAL(clicked(bool)),this,SLOT(idcLparenthesis_clicked()));
    connect(ui->IDC_RPARENTHESIS,SIGNAL(clicked(bool)),this,SLOT(idcRparenthesis_clicked()));
    ui->IDC_LPARENTHESIS->setStyleSheet(strOper);
    ui->IDC_RPARENTHESIS->setStyleSheet(strOper);
    //Dat power and or xor not
    connect(ui->IDC_DAT,SIGNAL(clicked(bool)),this,SLOT(idcDat_clicked()));
    connect(ui->IDC_POWER,SIGNAL(clicked(bool)),this,SLOT(idcPower_clicked()));
    ui->IDC_DAT->setStyleSheet(strOper);
    ui->IDC_POWER->setStyleSheet(strOper);

    connect(ui->IDC_AND,SIGNAL(clicked(bool)),this,SLOT(idcAnd_clicked()));
    connect(ui->IDC_OR,SIGNAL(clicked(bool)),this,SLOT(idcOr_clicked()));
    connect(ui->IDC_XOR,SIGNAL(clicked(bool)),this,SLOT(idcXor_clicked()));
    connect(ui->IDC_NOT,SIGNAL(clicked(bool)),this,SLOT(idcNot_clicked()));
    ui->IDC_AND->setStyleSheet(strComp);
    ui->IDC_OR->setStyleSheet(strComp);
    ui->IDC_XOR->setStyleSheet(strComp);
    ui->IDC_NOT->setStyleSheet(strComp);

    //max min ave sum maxT minT
    /*
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
*/
    redoAct = new QAction();
    connect(redoAct,SIGNAL(toggled(bool)),this,SLOT(redo()));
    redoAct->setShortcut(Qt::Key_Redo);

    undoAct = new QAction();
    connect(undoAct,SIGNAL(toggled(bool)),this,SLOT(undo()));
    undoAct->setShortcut(Qt::Key_Undo);

    ui->IDC_EDIT->setStyleSheet(LineEdit);
    ui->label->setStyleSheet(strLabel);
    ui->label_2->setStyleSheet(strLabel);
    ui->label_3->setStyleSheet(strLabel);
    ui->label_4->setStyleSheet(strLabel);

}

void HFormulaDlg::idc0_clicked()
{
    //获取当前鼠标的位置，如果当前焦点不在edit上，自动获取鼠标位置在尾部
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("0"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc1_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("1"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc2_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("2"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc3_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("3"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc4_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("4"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc5_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("5"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc6_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("6"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc7_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("7"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc8_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("8"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc9_clicked()
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("9"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idc10_clicked()//$
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("$"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcDot_clicked()//.
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("."));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcPlus_clicked() //+
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" + "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcMinus_clicked() //-
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" - "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcMutiply_clicked() //*
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" * "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcDidvde_clicked() // /
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" / "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcGreate_clicked() // >
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" > "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcLower_clicked() // <
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" < "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcEqual_clicked() // =
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" = "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcGequal_clicked() // >=
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" >= "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcLequal_clicked() // <=
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" <= "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcNequal_clicked() // !=
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" != "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcLparenthesis_clicked() //（
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" ( "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcRparenthesis_clicked() // ）
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" ) "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcDat_clicked()//Dat
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr("0"));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcPower_clicked()//Power
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" ^ "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcAnd_clicked()//And
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" & "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcOr_clicked()//Or
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" | "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcXor_clicked()//Xor
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" XOR "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::idcNot_clicked()//Not
{
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(tr(" ~ "));
    ui->IDC_EDIT->setTextCursor(cursor);
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
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
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
}

void HFormulaDlg::undo()
{
    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->undo();
}

void HFormulaDlg::setStation(ushort station)
{
    wStation = station;
}

ushort HFormulaDlg::getStation()
{
    return wStation;
}

void HFormulaDlg::setType(uchar type)
{
    btType = type;
}

uchar HFormulaDlg::getType()
{
    return btType;
}

void HFormulaDlg::onStationChanged_clicked(int index)
{
    if((int)-1 == index)
        return;
    index = ui->IDC_STATION->currentIndex();
    if((int)-1 == index)
        return;
    ushort wStationID = ui->IDC_STATION->currentData().toUInt();
    wStation = wStationID;

    wProtect = (ushort)-1;
    ui->IDC_COMBOMODE->blockSignals(true);
    ui->IDC_COMBOMODE->clear();
    ui->IDC_COMBOMODE->blockSignals(false);
    EQUIPMENTGROUP equipGroup;
    FORMULAPARAMETER Param;
    Param.wStation = wStation;
    Param.btType = TYPE_GROUP;
    Param.pBuffer = &equipGroup;
    for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,(HLPARAM)&Param,0);Param.wPoint++)
    {
        ui->IDC_COMBOMODE->addItem(equipGroup.szGroupName,equipGroup.wGroupID);
    }
    ui->IDC_COMBOMODE->setCurrentIndex(0);

    btType = ui->IDC_TYPE->currentData().toUInt();
    if(-1 != btType)
    {
        setPointList(wStation,btType);
    }

}

void HFormulaDlg::onIntervalChanged_clicked(int index)
{
    if((int)-1 == index)
        return;
    index = ui->IDC_COMBOMODE->currentIndex();
    if((int)-1 == index)
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

void HFormulaDlg::onTypeChanged_clicked(int index)
{
    //int index = ui->IDC_TYPE->currentIndex();
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

void HFormulaDlg::onAttribChanged_clicked(int index)
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
    ui->IDC_EDIT->setFocus();
    QTextCursor cursor = ui->IDC_EDIT->textCursor();
    cursor.clearSelection();
    cursor.movePosition(QTextCursor::NoMove, QTextCursor::KeepAnchor);
    cursor.insertText(strText);
    ui->IDC_EDIT->setTextCursor(cursor);

    QTextDocument* doc = ui->IDC_EDIT->document();
    doc->redo();
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
        for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,(HLPARAM)&Param,0);Param.wPoint++)
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
        for(Param.wPoint = 0;m_lpFormulaProc(FM_FINDDBINFO,0,(HLPARAM)&Param,0);Param.wPoint++)
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
            for(int j = 0; attrInfo[j].szAttrib != NULL;j++)
            {
                ui->IDC_ATTRIB->addItem(attrInfo[j].szAttrib,attrInfo[j].wAttrib);
            }
        }
    }
    //if(ui->IDC_TYPE->count()>0)
    //    ui->IDC_TYPE->setCurrentIndex(0);
}

void HFormulaDlg::onOk()
{
    QString strFormulaText = ui->IDC_EDIT->toPlainText();
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //strFormulaText.remove(QRegExp("\\s")); //不能预先去掉所有空格，遥测遥测名都是加空格的组合名，除非比较时，都去掉空格
    if(!_compile_formula(strFormulaText.toLocal8Bit().data(),m_pFormula,NULL,false))
    {
        const char* pszFormula = strFormulaText.toLocal8Bit().data();
        while(' ' == *pszFormula || '\t' == *pszFormula)
            pszFormula++;
        if(0 != pszFormula)
        {
            int nline,npos;
            getErrPos(nline,npos);
            ui->IDC_EDIT->setFocus();
            QTextCursor cursor = ui->IDC_EDIT->textCursor();
            cursor.setPosition(npos);
            ui->IDC_EDIT->setTextCursor(cursor);
            m_pFormula->wFormula[0] = 0;
            return;
        }
    }
    QDialog::accept();
}

void HFormulaDlg::onCancle()
{
    QDialog::reject();
}
