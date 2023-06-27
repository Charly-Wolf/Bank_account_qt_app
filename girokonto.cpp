#include "girokonto.h"

Girokonto::Girokonto(int kontoNr, double startkapital, QString inhaberusername, double dispo) : Konto(kontoNr, startkapital, inhaberusername)
{
    this->dispo = dispo;
}

bool Girokonto::auszahlen(double betrag)
{
    if((getKontostand() + betrag) >= -dispo) {
        einzahlen(betrag);
        return true;
        }
    return false;
}

double Girokonto::getDispo()
{
    return this->dispo;
}

//QString Girokonto::toString()
//{
//    QString strKontoNr = QString::number(this->getKontoNr());
//        // Ensure the string has at least 8 characters
//        while (strKontoNr.length() < 9) {
//            strKontoNr = "0" + strKontoNr;
//        }
//        // Insert "-" at specific positions
//        strKontoNr = strKontoNr.insert(3, "-").insert(strKontoNr.size() - 2, "-");

//    return strKontoNr;
//}

