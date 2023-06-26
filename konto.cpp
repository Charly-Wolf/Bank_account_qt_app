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
    this->kontostand += betrag; //TODO BUG: Does not work with newly created accounts
}

QString Konto::toString()
{

}
