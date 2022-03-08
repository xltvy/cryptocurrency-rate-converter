// Author 1: Altay Acar
// Author 1's ID: 2018400084
// Author 2: Engin Oğuzhan Şenol
// Author 2's ID: 2020400324

#include <QApplication>
#include <QWidget>
#include <QTableWidget>
#include <QTableView>
#include "accessor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget *centralwidget = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout();  //Layout of the application
    Accessor *myAcc = new Accessor();   //Accessor object to be displayed

    vbox->addWidget(myAcc); //Application layout only contains Accessor widget

    centralwidget->setLayout(vbox);
    //Sizing operation for the application window
    int hSizing = myAcc->rowsize+59;
    centralwidget->resize(537, hSizing);
    //Title of the application
    centralwidget->setWindowTitle("Crypto Currency Converter");
    centralwidget->show();  //Makes the application visible

    return app.exec();
}
