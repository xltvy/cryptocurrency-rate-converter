// Author 1: Altay Acar
// Author 1's ID: 2018400084
// Author 2: Engin Oğuzhan Şenol
// Author 2's ID: 2020400324

#ifndef ACCESSOR_H
#define ACCESSOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QVector>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QMap>
#include <QVBoxLayout>

/* Accessor class inherits QTableWidget for table operations.
 * The built-in QNetworkAccessManager object "manager" retrieves
 * the json data from the URL created throughout the process.
 * It also has the replyFinished method that performs an operation
 * to fill the data table after receiving the signal from the URL.
*/
class Accessor : public QTableWidget
{
    Q_OBJECT
public:
    Accessor();
    QMap<QString, QString> symbol_dict; //Stores crypto coin symbols and ids as key-value pairs respectively
    QMap<QString, QString> name_dict;   //Stores crypto coin names and ids as key-value pairs respectively
    QMap<QString, QString> id_dict; //Stores crypto coin ids and names as key-value pairs respectively
    int rowsize;    //Stores the size of the rows for sizing operation in mainwindow

public slots:
    //Fuction to build table after getting a reply from the URL
    void replyFinished(QNetworkReply *reply);
    //Function to get data about every possible coins' name, id, and symbol
    void dictAcquired(QNetworkReply *reply);

private:
    //QNetworkAccessManager object to access the json file in given URL
    QNetworkAccessManager *manager;
    //QNetworkAccessManager object to access the list of every possible crypto coin and their ids, names, and symbols
    QNetworkAccessManager *archiver;
};


#endif // ACCESSOR_H
