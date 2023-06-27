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

