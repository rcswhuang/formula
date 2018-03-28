#ifndef HFORMULA_H
#define HFORMULA_H

#include <QDialog>
//#include "hformulapi.h"
namespace Ui {
class Formula;
}

class HFormulaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HFormulaDlg(QWidget *parent = 0);
    //HFormulaDlg(FORMULA* pFormula,QWidget *parent);
    ~HFormulaDlg();

    void init();
    void initConnect();
    void setPointList(ushort wStation,uchar btType);
    void updateAttrib();

    void setStation(ushort station);
    ushort getStation();
    void setType(uchar type);
    uchar getType();

public slots:
    void idc0_clicked();
    void idc1_clicked();
    void idc2_clicked();
    void idc3_clicked();
    void idc4_clicked();
    void idc5_clicked();
    void idc6_clicked();
    void idc7_clicked();
    void idc8_clicked();
    void idc9_clicked();
    void idc10_clicked();//$
    void idcDot_clicked();//.

    void idcPlus_clicked(); //+
    void idcMinus_clicked(); //-
    void idcMutiply_clicked(); //*
    void idcDidvde_clicked(); // /

    void idcGreate_clicked(); // >
    void idcLower_clicked(); // <
    void idcEqual_clicked(); // =
    void idcGequal_clicked(); // >=
    void idcLequal_clicked(); // <=
    void idcNequal_clicked(); // !=

    void idcLparenthesis_clicked(); //（
    void idcRparenthesis_clicked(); // ）

    void idcDat_clicked();//Dat
    void idcPower_clicked();//Power
    void idcAnd_clicked();//And
    void idcOr_clicked();//Or
    void idcXor_clicked();//Xor
    void idcNot_clicked();//Not

    void idcMax_clicked();// Max()
    void idcMin_clicked();// Min()
    void idcAve_clicked();// Ave()
    void idcSum_clicked();// Sum()
    void idcMaxT_clicked();// MaxT()
    void idcMinT_clicked();// MinT()

    void idc1Min_clicked();// 1Min()
    void idc5Min_clicked();//5Min()
    void idc15Min_clicked();//15Min()
    void idc1Hour_clicked();//1Hour()
    void idc1Day_clicked();//1Day()
    void idc1Mon_clicked();//1Month()

    void redo();
    void undo();

    void onStationChanged_clicked();
    void onIntervalChanged_clicked();
    void onTypeChanged_clicked();
    void onAttribChanged_clicked();

    void onListWidget_doubleClicked();

protected:
    ushort wStation;
    ushort wProtect;
    uchar btType;
    //FORMULA* m_pFormula;

private:
    Ui::Formula *ui;
    QAction *redoAct;
    QAction *undoAct;
};

#endif // HFORMULA_H
