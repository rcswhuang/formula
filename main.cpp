#include <QApplication>
#include <QFile>
#include <QTextStream>
int main(int argc,char *argv[])
{

   // Q_INIT_RESOURCE(styleqss);
    QApplication app(argc,argv);

    QFile file(":/qss/qpushbutton.qss");
    file.open(QFile::ReadOnly);
    //QTextStream filetext(&file);
        //QString stylesheet = filetext.readAll();
     //app.setStyleSheet(file.readAll());
     file.close();

    //QString strBtnQss = ":/qss/qpushbutton.qss";
    //app.setStyleSheet(strBtnQss);
    //HFormulaDlg dlg;
    //dlg.exec();
    return app.exec();
}
