#ifndef SPARKONTO_H
#define SPARKONTO_H

#include "konto.h"
#include <QDate>

class Sparkonto : public Konto
{
    private:
        QDate letzteAuszahlung;
    public:
        Sparkonto(int kontoNr, QString inhaberUsername, double startkapital);
        bool auszahlen(double betrag);
        QDate getLetzteAuszahlung();
        QString toString();
        bool setLetzteAuszahlug(QDate letzteAuszahlung);
};

#endif // SPARKONTO_H
