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

    // Testtreiber:
//    bool testKontenAnlegen();
    void testUsersAnlegen();

    // Widget-Sichtbarkeit (visible, enabled):
    void initialWidgetsState();
    void kontoAnlegenGuiDeaktivieren();
    void toggleBtnSichtbarkeitWennGroesserAlsNull(double value, QWidget* button);
    void toggleSichtbarkeitWennUsernameOderPassNotEmpty(QString username, QString pass, QWidget* button); // TO DO: CHECK IF Name of Function is correct (sichtbarkeit oder enabled?)
    void operationModusDeaktivieren();
    void operationModusAktivieren();
    // IN PROGRESS


    void enterLoginModus();
    void exitLoginModus();
    void enterHomeModus();
    void exitHomeModus();
    void transitionLogout();
//    void enterKontoAnlegenModus(); // MAYBE NOT NECESSARY
//    void exitKontoAnlegenModus(); // MAYBE NOT NECESSARY

    // TO DO: Formatierung... maybe I need to delete these methods and use toString directly from the classes (Models)
    QString kontoStandFormatieren(double kontoStand);
    QString letzteAuszahlungFormatieren(QDate letzteAuszahlung);

    // Widgets manipulieren
    void empfaengerKontenLaden(int MarkiertesKontoNr);
    void kontenAnzeigen();
    void opInHistorieHinzufuegen(QString kontoNrString, QString operation, double betrag, double kontostand);
    bool meldungenBeimAuszahlen(double betrag);
    bool erfolgMeldungBeimAuszahlen(bool reply);
    bool setTabellenWidth();

    // Animationen
    void fadeInGuiElement(QWidget* qWidget, double duration);
    void fadeOutGuiElement(QWidget* qWidget, double duration);
    void animateBankNameFadeIn();
    void animateHeaderBtnsFadeIn();
    void animateUserNameLbl();

    // Buttons Event handlers
    void on_btn_OkKontoAnlegen_clicked();
    void on_btnUeberw_clicked();
    void on_btnAbbrechen_clicked();
    void on_btnEinzahlung_clicked();
    void on_btnAuszahlung_clicked();
    void on_radBtnGiro_clicked();
    void on_radBtnSpar_clicked();
    void on_btnClearHistorie_clicked();
    void on_btnLogin_clicked();
    void on_btnAusloggen_clicked();
    void on_btnUserVerwalten_clicked();
    void on_btnBankkonten_clicked();
    void on_btnNeuerUser_clicked();
    void on_btnAbbUserAnlegen_clicked();
    void on_btnUserAnlegen_clicked();
    void on_btnLoginDebugOhneUsername_clicked();
    void on_btnCsvExport_clicked();
    void on_btn_OkOperation_clicked();

    // Values-changed Event handlers
    void on_tabWidgetOperationen_currentChanged(int index);
    void on_tableGirokonten_itemSelectionChanged();
    void on_tableSparkonten_itemSelectionChanged();
    void on_lePassAnlegen_textChanged(const QString &arg1);
    void on_lePass_textChanged(const QString &arg1);
    void on_leUserName_textChanged(const QString &arg1);
    void on_cBoxEmpfaenger_currentIndexChanged(const QString &arg1);
    void on_sbDispoAnlegen_valueChanged(double arg1);
    void on_tabWidgetKonten_currentChanged(int index);

    // Tastatur Event handlers
    void keyPressEvent(QKeyEvent* event) override;
    void on_leUserName_returnPressed();
    void on_lePass_returnPressed();

private:
    Ui::FrmMain *ui;

    KontenListe* konten;
    UsersListe* users;

    Girokonto* markierteGirokonto;
    Sparkonto* markierteSparkonto;
    int markierteKontoNr; // TO DO: DO I REALLY NEED THIS? MAYBE DIRECTLY A MARKIERTEKONTO OBJECT

    User* loggedUser;

    int empfKontoNr; // TO DO: DO I NEED IT AS GLOBAL VARIABLE?

    QWidget* activeWidget; // TO DO: USE IT OR DELETE IT, bcs for now it does not serve any clear purpose, it is halfway used

    enum opModus {
        Einzahlung,
        Auszahlung,
        Ueberweisung,
    };
    opModus opModus;
};
#endif // FRMMAIN_H
