#ifndef GIROKONTO_H
#define GIROKONTO_H

#include "konto.h"

class Girokonto : public Konto
{
    private:
        double dispo;
    public:
        Girokonto(int kontoNr, double startkapital, QString inhaberUsername, double dispo);

        bool auszahlen(double betrag);

        double getDispo();
//        QString toString();
};

#endif // GIROKONTO_H
