#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include "kontenliste.h"
#include "usersliste.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FrmMain; }
QT_END_NAMESPACE

class FrmMain : public QMainWindow
{
    Q_OBJECT

public:
    FrmMain(QWidget *parent = nullptr);
    ~FrmMain();

private slots:

//    bool testKontenAnlegen();
    bool testUsersAnlegen();

    QString kontoNrFormatieren(QString kontoStr);
    QString kontoStandFormatieren(double kontoStand);
    QString letzteAuszahlungFormatieren(QDate letzteAuszahlung);
    void operationModusDeaktivieren();
    void operationModusAktivieren();
    void empfaengerKontenLaden(int MarkiertesKontoNr);
    void kontoAnlegenGuiDeaktivieren();
    void kontenAnzeigen();
    void opInHistorieHinzufuegen(int kontoNr, QString operation, double betrag, double kontostand);
    void fadeInGuiElement(QWidget* qWidget, double duration);
    void fadeOutGuiElement(QWidget* qWidget, double duration);
    void initialWidgetsState();
    void toggleBtnSichtbarkeitWennGroesserAlsNull(double value, QWidget* button);
    void toggleSichtbarkeitWennUsernameOderPassNotEmpty(QString username, QString pass, QWidget* button); // TODO CHECK IF Name of Function is correct (sichtbarkeit oder enabled?)
    void userNameUndWillkommenAnimieren();
    bool meldungenBeimAuszahlen(double betrag);

    void debugMessage(QString debugString);
    void on_btnUeberw_clicked();
    void on_btnAbbrechen_clicked();
    void on_btnEinzahlung_clicked();
    void on_btnAuszahlung_clicked();
    void on_btnOk_clicked();
    void on_btnKontoAnlegen_clicked();
    void on_radBtnGiro_clicked();
    void on_radBtnSpar_clicked();
//    void on_btnAbbKontoAnl_clicked();
    void on_cBoxEmpfaenger_currentIndexChanged(const QString &arg1);
    void on_btnClearHistorie_clicked();
    void on_btnLogin_clicked();
    void on_btnAusloggen_clicked();
    void on_btnUserVerwalten_clicked();
    void on_btnBankkonten_clicked();
    void on_btnNeuerUser_clicked();
    void on_btnAbbUserAnlegen_clicked();
    void on_btnUserAnlegen_clicked();
    void on_btnLoginDebugOhneUsername_clicked();
    void on_btnPassAendern_clicked();
    void on_btnAbbPassAendern_clicked();
    void on_btnOkPassAendern_clicked();
    void on_lePass_returnPressed();
    void on_leUserName_returnPressed();
    void keyPressEvent(QKeyEvent* event) override;
    void on_btnCsvExport_clicked();
//    void on_sbStartKapAnlegen_valueChanged(double arg1);
    void on_sbDispoAnlegen_valueChanged(double arg1);
    void on_tabWidgetKonten_currentChanged(int index);
    void on_leUserName_textChanged(const QString &arg1);
    void on_lePass_textChanged(const QString &arg1);
    void on_lePassAnlegen_textChanged(const QString &arg1);
    void on_tabWidgetOperationen_currentChanged(int index);
    void on_tableGirokonten_itemSelectionChanged();
    void on_tableSparkonten_itemSelectionChanged();

private:
    Ui::FrmMain *ui;

    QVector<User*> testUsers;

    KontenListe* konten;
    UsersListe* users;
    Girokonto* markierteGirokonto;
    Sparkonto* markierteSparkonto;
    User* loggedUser;
    int markierteKontoNr;
    int empfKontoNr;

    QWidget* activeWidget;

    enum opModus {
        Einzahlung,
        Auszahlung,
        Ueberweisung,
    };
    opModus opModus;
};
#endif // FRMMAIN_H
