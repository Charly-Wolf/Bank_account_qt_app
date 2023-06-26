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
    QString strKontoNr = QString::number(this->getKontoNr());
        // Ensure the string has at least 8 characters
        while (strKontoNr.length() < 9) {
            strKontoNr = "0" + strKontoNr;
        }
        // Insert "-" at specific positions
        strKontoNr = strKontoNr.insert(3, "-").insert(strKontoNr.size() - 2, "-");

    return strKontoNr;
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
