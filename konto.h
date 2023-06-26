#ifndef KONTO_H
#define KONTO_H

#include <QString>

class Konto
{
    private:
        double kontostand;
        int kontoNr;
        QString inhaberUsername;
    public:
        Konto(int kontoNr, double startkapital, QString inhaberUsername);
        int getKontoNr();
        double getKontostand();
        QString getInhaberUsername();
        void einzahlen(double betrag);
        virtual bool auszahlen(double betrag) = 0;
        virtual QString toString();
};

#endif // KONTO_H
