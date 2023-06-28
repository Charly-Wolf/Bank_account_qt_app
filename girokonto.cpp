#include "girokonto.h"
#include <QDebug>

Girokonto::Girokonto(int kontoNr, double startkapital, QString inhaberusername, double dispo) : Konto(kontoNr, startkapital, inhaberusername)
{
    this->dispo = dispo;
}

double Girokonto::auszahlen(double betrag)
{
    qDebug() <<"GIROKONTO.CPP AUSZAHLEN - BETRAG: " << betrag;
    qDebug() << "AKTUELLER KONTOSTAND: " << getKontostand();
    if((getKontostand() + betrag) >= dispo) {
        return einzahlen(-betrag);
        }
    return getKontostand();
}

double Girokonto::getDispo()
{
    return this->dispo;
}

