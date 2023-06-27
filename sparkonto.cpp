#include "sparkonto.h"

Sparkonto::Sparkonto(int kontoNr, QString inhaberUsername, double startkapital) : Konto(kontoNr, startkapital, inhaberUsername)
{
}

bool Sparkonto::auszahlen(double betrag)
{
    QDate heute = QDate::currentDate();
    if(this->letzteAuszahlung < heute) {
        if((getKontostand()+betrag) >= 0) {
            einzahlen(betrag);
            setLetzteAuszahlug(QDate::currentDate());
            return true;
        }
    }
    return false; //TO DO Handle case when trying to withdraw more money than the balance for sparkonten -> QMessageBox
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
