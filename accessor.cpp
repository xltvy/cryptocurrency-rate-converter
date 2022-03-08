// Author 1: Altay Acar
// Author 1's ID: 2018400084
// Author 2: Engin Oğuzhan Şenol
// Author 2's ID: 2020400324

#include "accessor.h"

//currencies vector stores the name of currencies given in input file input.txt
QVector<QString> currencies;

/**
 * @brief Accessor::Accessor custom QTableWidget built to display conversion rates of crypto coins.
 * Coins given in an input file, and the input file is accessed through the environment variable
 * "MYCRYPTOCONVERT".
 */
Accessor::Accessor() : QTableWidget()
{
    //Objects that accesses the URL and gives a signal to the destination after accessing
    archiver = new QNetworkAccessManager(this);
    manager = new QNetworkAccessManager(this);

    //Signaling operation to receive json data about desired coins
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));

    //Accesses the environment variable MYCRYPTOCONVERT that stores the path of the input file
    const QByteArray inputpath = qgetenv("MYCRYPTOCONVERT");
    QFile file(inputpath);  //Opens the file for input operations

    //Reads the file and inserts every crypto currency name in the currencies vector
    if (file.open(QIODevice::ReadOnly))
    {
       QTextStream in(&file);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          currencies.append(line);
       }
       file.close();    //Closes the file
    }

    //Number of rows equals to the number of currency names given
    this->setRowCount(currencies.size());
    this->rowsize = currencies.size()*30;   //Each row's height is 30
    //Number of columns is fixed with 4 (one column for crypto, three columns for conversions)
    this->setColumnCount(4);
    //Sets the names of the headers
    QStringList hLabels;
    hLabels << "Cyrpto Currency" << "USD" << "EUR" << "GBP";
    this->setHorizontalHeaderLabels(hLabels);
    this->setColumnWidth(0, 200);   //Column that contains the names of the crypto coins is wider

    //Signaling operation to receive list of every possible coin in the URL's database
    connect(archiver, SIGNAL(finished(QNetworkReply*)), this, SLOT(dictAcquired(QNetworkReply*)));
    archiver->get(QNetworkRequest(QUrl("https://api.coingecko.com/api/v3/coins/list?include_platform=true")));

}

/**
 * @brief Accessor::replyFinished Builds the table according to the data received from the URL
 * @param reply Stores the reply from the URL
 */
void Accessor::replyFinished(QNetworkReply *reply)
{
    //Creates the QJsonObject according to the reply given from the URL
    QByteArray result = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result);
    //QJsonObject that has coin names as keys and another QJsonObject for currency-rate pairs
    QJsonObject obj = jsonResponse.object();

    /* Fills the table according to the coin names and their conversion rates
     * If the coin name given in the input txt file is not an id for the coingecko.com,
     * then the conversion rates for that coin will be 0. If the coin name given in the
     * input txt file is an id for the coingecko.com, then the conversion rates for
     * that coin is placed in appropriate cells in the table
    */
    for (int i=0; i < currencies.size(); i++) {
        QTableWidgetItem *item; //QTableWidgetItem that fills the specified table cell
        for (int j=0; j< 4; j++) {
            item = new QTableWidgetItem;
            //First column
            if (j==0) {
                //First column only contains the names of the given coin names
                item->setText(currencies.at(i));
            }
            //Second column
            if (j==1) {
                //Second column contains the USD conversion rates for the given coin
                QJsonObject temp = obj[name_dict[currencies.at(i)]].toObject();
                //Retrieves the conversion rate data for USD from temporary QJsonObject temp
                double dtemp = temp["usd"].toDouble();
                QString stemp = QString::number(dtemp);
                item->setText(stemp);   //Sets the item accordingly
            }
            //Third column
            if (j==2) {
                //Third column contains the EUR conversion rates for the given coin
                QJsonObject temp = obj[name_dict[currencies.at(i)]].toObject();
                //Retrieves the conversion rate data for EUR from temporary QJsonObject temp
                double dtemp = temp["eur"].toDouble();
                QString stemp = QString::number(dtemp);
                item->setText(stemp);   //Sets the item accordingly
            }
            //Fourth column
            if (j==3) {
                //Fourth column contains the GBP conversion rates for the given coin
                QJsonObject temp = obj[name_dict[currencies.at(i)]].toObject();
                //Retrieves the conversion rate data for GBP from temporary QJsonObject temp
                double dtemp = temp["gbp"].toDouble();
                QString stemp = QString::number(dtemp);
                item->setText(stemp);   //Sets the item accordingly
            }
            this->setItem(i, j, item);  //Fills the specified cell (ith row, jth column) with item
        }
    }
}
/**
 * @brief Accessor::dictAcquired Fills the databases about every coin's name, id, and symbol
 * @param reply Stores the reply from the URL
 */
void Accessor::dictAcquired(QNetworkReply *reply)
{
    //Creates the QList<QVariant> that stores every coin's data fields as a map
    QString result = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(result.toUtf8());
    QVariant temp = jsonResponse.toVariant();
    QList<QVariant> archive = temp.toList();
    //From the data given in the list "archive", fills the QMap<QString, QString> objects symbol_dict, id_dict, name_dict
    for (int i=0; i<archive.size(); i++) {
        QMap<QString, QVariant> entry = archive.at(i).toMap();  //Every entry in the list is a QMap
        QString symbol = entry["symbol"].toString();    //symbol of the ith coin in the list
        QString id = entry["id"].toString();    //id of the ith coin in the list
        QString name = entry["name"].toString();    //name of the ith coin in the list
        symbol_dict[symbol] = id;   //symbol-id as key-value
        name_dict[name] = id;   //name-id as key-value
        id_dict[id] = name; //id-name as key-value
    }

    //Checks the coins given in the input file
    for (int i=0; i<currencies.size(); i++) {
        //If it is a symbol given in the input file, replaces it with it's id
        if (symbol_dict.keys().contains(currencies.at(i))) {
            currencies.replace(i, symbol_dict[currencies.at(i)]);
        }
        //If it is a name given in the input file, replaces it with it's id
        if (name_dict.keys().contains(currencies.at(i))) {
            currencies.replace(i, name_dict[currencies.at(i)]);
        }
    }

    //Builds the URL that contains the json object for crypto coins given in input appropriate to the website
    QString url = "https://api.coingecko.com/api/v3/simple/price?ids=";
    int count = 1;
    for (int i=0; i<currencies.size(); i++) {
        if (currencies.size()==count) {
            url += currencies.at(i);
            break;
        }
        url += currencies.at(i) + ",";
        count++;
    }
    url += "&vs_currencies=usd,eur,gbp";
    //Updates the currency list for writing to the table operation, as their names are written, not the ids
    for (int i=0; i<currencies.size(); i++) {
        currencies.replace(i, id_dict[currencies.at(i)]);
    }

    manager->get(QNetworkRequest(QUrl(url)));   //Network request operation

}
