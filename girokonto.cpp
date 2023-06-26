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

QString Girokonto::toString()
{
    QString kontoNr = QString::number(this->getKontoNr());
    int lastIndex = kontoNr.length() - 1;
    return "Girokonto " + kontoNr.insert(lastIndex - 1, "-").rightJustified(8, '0'); //"-" vor den 2 letzen Zahlen und "0" links hinzufügen
}

