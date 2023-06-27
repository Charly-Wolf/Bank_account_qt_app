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

        bool setLetzteAuszahlug(QDate letzteAuszahlung);
        //        QString toString();
};

#endif // SPARKONTO_H
