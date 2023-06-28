#include "sparkonto.h"

Sparkonto::Sparkonto(int kontoNr, QString inhaberUsername, double startkapital) : Konto(kontoNr, startkapital, inhaberUsername)
{
}

double Sparkonto::auszahlen(double betrag)
{
    QDate heute = QDate::currentDate();
    if(this->letzteAuszahlung < heute) {
        if((getKontostand()+betrag) >= 0) {
            setLetzteAuszahlug(QDate::currentDate());
            return einzahlen(-betrag);
        }
    }
    return getKontostand();
}

QDate Sparkonto::getLetzteAuszahlung()
{
    return this-> letzteAuszahlung;
}

bool Sparkonto::setLetzteAuszahlug(QDate letzteAuszahlung)
{
    QDate heute = QDate::currentDate();
    if (letzteAuszahlung <= heute) { //Überprüfen ob heute schon eine Auszahlung/Überweisung durcheführt wurde
        this->letzteAuszahlung = letzteAuszahlung;
        return true;
    }
    return false;
}
