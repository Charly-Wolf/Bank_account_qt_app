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
    return false; //TODO Handle case when trying to withdraw more money than the balance for sparkonten -> QMessageBox
}

QDate Sparkonto::getLetzteAuszahlung()
{
    return this-> letzteAuszahlung;
}

QString Sparkonto::toString()
{
    QString kontoNr = QString::number(this->getKontoNr());
    int lastIndex = kontoNr.length() - 1;
    return "Sparkonto " + kontoNr.insert(lastIndex - 1, "-").rightJustified(8, '0'); //"-" vor den 2 letzen Zahlen und "0" links hinzufügen
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
