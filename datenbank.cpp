#include "datenbank.h"

Datenbank::Datenbank(QString dbDatei)
{
    this->ready = false;
    this->db = (QSqlDatabase::addDatabase("QSQLITE"));
    this->db.setDatabaseName(dbDatei);
    this->ready = db.open();
    this->letzterError = db.lastError().text();
    this->db.close();
}

Datenbank::~Datenbank()
{
    this->db.close();
}

QVector<Konto *> Datenbank::alleKontenHolenAusDB()
{   qDebug() << "---------------\nHOLE ALLE KONTEN AUS DB";
    QVector<Konto*> konten;

    bool ok = false;
    QSqlQuery* qry = this->abfrage("SELECT tblKonten.KontoNr, tblKonten.Kontostand, tblKonten.Kontotyp, tblKonten.InhaberUsername, "
                                    "tblGirokonten.dispo, tblSparkonten.LetzteAuszahlung "
                                    "FROM tblKonten "
                                    "LEFT JOIN tblGirokonten ON tblKonten.KontoNr = tblGirokonten.KontoNr "
                                    "LEFT JOIN tblSparkonten ON tblKonten.KontoNr = tblSparkonten.KontoNr ");
    if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
    qDebug() << "SELECT QUERY";
    if(qry) {
        if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
        ok = qry->first();
        qDebug() << "QUERY -> FIRST";
        while(ok) {
            int kontoNr = qry->value("KontoNr").toInt();
            qDebug() << "KONTO NR: " << kontoNr;
            double kontostand = qry->value("Kontostand").toDouble();
            qDebug() << "KONTOSTAND: " << kontostand;
            QString kontotyp = qry->value("Kontotyp").toString();
            qDebug() << "KONTOTYP: " << kontotyp;
            QString inhaberUsername = qry->value("InhaberUsername").toString();
            qDebug() << "INHABER USERNAME: " << inhaberUsername;
            if(kontotyp == "Girokonto") {
                double dispo = qry->value("Dispo").toDouble();
                qDebug() << "DISPO: " << dispo;
                Girokonto* gk = new Girokonto(kontoNr, kontostand, inhaberUsername, dispo);
                qDebug() << "NEW GIROKONTO OBJEKT  CREATED " ;
                konten.append(gk);
                qDebug() << "APENDED TO KONTEN AT INDEX: " << konten.indexOf(gk);

            }
            else {
                QDate letzteAuszahlung = QDate::fromString(qry->value("LetzteAuszahlung").toString(), "yyyy-MM-dd");
                qDebug() << "LETZTE-AUSZAHLUNG DATE: " << letzteAuszahlung;
                Sparkonto* sk = new Sparkonto(kontoNr, inhaberUsername, kontostand);
                qDebug() << "NEW SPARKONTO OBJECT CREATED" ;
                sk->setLetzteAuszahlug(letzteAuszahlung);
                konten.append(sk);
                qDebug() << "APENDED TO KONTEN AT INDEX: " << konten.indexOf(sk);
            }
            ok = qry->next();
            if (ok) qDebug() << "NEXT ROW IN DB!\n----------";
            if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
        } //While
        delete qry;
       qDebug() << "-----------------\nQRY DELETED\n----------------";
    } //If
    return konten;
}

void Datenbank::kontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername, QString kontotyp)
{
    QSqlQuery* qry;
    QString sql = "INSERT INTO tblKonten (KontoNr, Kontostand, Kontotyp, InhaberUsername)";
    sql += "VALUES(" + QString::number(kontoNr) + ", " + QString::number(startkapital) + ", '"+ kontotyp +"', '" + inhaberUsername +"');";
    qry = this->abfrage(sql);
    delete qry;
}

QSqlQuery *Datenbank::abfrage(QString sql)
{
    this->ready = this->db.open();
    QSqlQuery *qry = new QSqlQuery(this->db);
    this->letzterError = this->db.lastError().text();
    if (this->ready) {
        this->ready = qry->exec(sql);
        this->letzterError = qry->lastError().text();
    }
    return qry;
}

//QVector<Konto *> Datenbank::loggedUserKontenHolenAusDB(QString username)
//{
//    QVector<Konto*> konten;

//    bool ok = false;
//    QSqlQuery* qry = this->abfrage("SELECT tblKonten.KontoNr, tblKonten.Kontostand, tblKonten.Kontotyp, "
//                                    "tblGirokonten.dispo, tblSparkonten.LetzteAuszahlung "
//                                    "FROM tblKonten "
//                                    "LEFT JOIN tblGirokonten ON tblKonten.KontoNr = tblGirokonten.KontoNr "
//                                    "LEFT JOIN tblSparkonten ON tblKonten.KontoNr = tblSparkonten.KontoNr "
//                                    "WHERE InhaberUsername = '" + username + "';");
//    if(qry) {
//        ok = qry->first();
//        while(ok) {
//            int kontoNr = qry->value("KontoNr").toInt();
//            double kontostand = qry->value("Kontostand").toDouble();
//            QString kontotyp = qry->value("Kontotyp").toString();
//            QString inhaberUsername = qry->value("InhaberUsername").toString();
//            if(kontotyp == "Girokonto") {
//                double dispo = qry->value("Dispo").toDouble();
//                Girokonto* gk = new Girokonto(kontoNr, kontostand, inhaberUsername, dispo);
//                konten.append(gk);

//            }
//            else {
//                QDate letzteAuszahlung = QDate::fromString(qry->value("LetzteAuszahlung").toString(), "yyyy-MM-dd");
//                Sparkonto* sk = new Sparkonto(kontoNr, inhaberUsername, kontostand);
//                sk->setLetzteAuszahlug(letzteAuszahlung);
//                konten.append(sk);
//            }
//            ok = qry->next();
//        } //While
//        delete qry;
//    } //If
//    return konten;
//}

QVector<User *> Datenbank::alleUsersHolenAusDB()
{   qDebug() << "---------------\nHOLE ALLE USERS AUS DB";
    QVector<User*>users;

    bool ok;
    QSqlQuery* qry;
    qry = this->abfrage("SELECT * FROM tblUsers");
    if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
    qDebug() << "SELECT QUERY";
    if (qry) {
        if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
        User *u;
        int id;
        QString username;
        QString vorname;
        QString nachname;
        QString pass;

        ok = qry->first();
        qDebug() << "QUERY -> FIRST";
        while (ok) {
            if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
            id = qry->value("Id").toInt();
            qDebug() << "ID: " << id;
            username = qry->value("Username").toString();
            qDebug() << "USERNAME: " << username;
            nachname = qry->value("Nachname").toString();
            qDebug() << "NACHNAME: " << nachname;
            vorname = qry->value("Vorname").toString();
            qDebug() << "VORNAME: " << vorname;
            pass = qry->value("Pass").toString();
            qDebug() << "PASS: " << pass;
            u = new User(username, vorname, nachname, pass);
            if(u) qDebug() << "USER CREATED";
            u->setId(id);
            users.append(u);
            qDebug() << "USER APPENDED AT INDEX: " << users.indexOf(u);
            ok = qry->next();
            if (ok) qDebug() << "NEXT ROW IN DB!\n----------";
            if(db.lastError().isValid()) qDebug() << "DB ERROR: " << letzterError;
        } //while
        delete qry;
        qDebug() << "-----------------\nQRY DELETED\n----------------";
    } //if qry
    return users;
}

void Datenbank::girokontoEintragenInDB(int kontoNr, double startkapital, double dispo, QString inhaberUsername)
{
    kontoEintragenInDB(kontoNr, startkapital, inhaberUsername, "Girokonto");

    QSqlQuery* qry;
    QString sql = "INSERT INTO tblGirokonten (KontoNr, Dispo) ";
    sql+= "VALUES(" + QString::number(kontoNr) + ", " + QString::number(dispo) + ");";
    qry = this->abfrage(sql);
    delete qry;
}

void Datenbank::sparkontoEintragenInDB(int kontoNr, double startkapital, QString inhaberUsername)
{
    kontoEintragenInDB(kontoNr, startkapital, inhaberUsername, "Sparkonto");

    QSqlQuery* qry;
    QString sql = "INSERT INTO tblSparkonten (KontoNr) ";
    sql+= "VALUES(" + QString::number(kontoNr) + ");";
    qry = this->abfrage(sql);
    delete qry;
}

bool Datenbank::letzteAuszahlungAendernInDB(QDate letzteAuszahlung, int KontoNr)
{
    QSqlQuery* qry;
    QString letzteAuszahlungStr = letzteAuszahlung.toString("yyyy-MM-dd");
    QString KontoNrStr = QString::number(KontoNr);
    QString sql = "UPDATE tblSparkonten SET LetzteAuszahlung = '" + letzteAuszahlungStr + "' ";
    sql += "WHERE KontoNr = " + KontoNrStr;
    qry = this->abfrage(sql);

    if(qry->lastError().isValid()) return false;
    return true;
    delete qry;
}

bool Datenbank::kontostandAendernInDB(double kontostand, int KontoNr)
{
    QSqlQuery* qry;
    QString kontostandStr = QString::number(kontostand);
    QString KontoNrStr = QString::number(KontoNr);
    QString sql = "UPDATE tblKonten SET Kontostand = '" + kontostandStr + "' ";
    sql += "WHERE KontoNr = " + KontoNrStr;
    qry = this->abfrage(sql);
    if(qry->lastError().isValid()) return false;
    return true;
    delete qry;
}


User *Datenbank::userHolenAusDB(QString username)
{
    User* u = NULL;
    QSqlQuery *qry;
    qry = this->abfrage("SELECT * FROM tblUsers "
            "WHERE Username = '" + username + "'");
    if(qry->first()) {
        int id = qry->value("Id").toInt();
        QString vorname = qry->value("Vorname").toString();
        QString nachname = qry->value("Nachname").toString();
        QString pass = qry->value("Pass").toString();
        u = new User(username, vorname, nachname, pass);
        u->setId(id);
    }
    delete qry;
    return u;
}

void Datenbank::userEintragenInDB(User *user)
{
    QSqlQuery* qry;
    QString sql = "INSERT INTO tblUsers (Username, Vorname, Nachname, Pass) ";
    sql += "VALUES('" + user->getUsername() + "', '" + user->getVorname() + "', '";
    sql += user->getNachname() + "', '" + user->getPass() + "')";

    qry = this->abfrage(sql);
    delete qry;
}

QString Datenbank::getLetzterError()
{
    return letzterError; // DEBUG
}
