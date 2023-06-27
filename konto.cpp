#include "konto.h"

Konto::Konto(int kontoNr, double startkapital, QString inhaberUsername)
{
    this->kontoNr = kontoNr;
    this->kontostand = startkapital;
    this->inhaberUsername = inhaberUsername;
}

int Konto::getKontoNr()
{
    return this->kontoNr;
}

double Konto::getKontostand()
{
    return this->kontostand;
}

QString Konto::getInhaberUsername()
{
    return this->inhaberUsername;
}

void Konto::einzahlen(double betrag)
{
    this->kontostand += betrag; //TO DO BUG: Does not work with newly created accounts
}

QString Konto::toString()
{
    QString strKontoNr = QString::number(this->getKontoNr());
        // Ensure the string has at least 8 characters
        while (strKontoNr.length() < 9) {
            strKontoNr = "0" + strKontoNr;
        }
        // Insert "-" at specific positions
        strKontoNr = strKontoNr.insert(3, "-").insert(strKontoNr.size() - 2, "-");

    return strKontoNr;
}
