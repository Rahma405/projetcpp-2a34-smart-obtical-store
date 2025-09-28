#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<QStringList> listeClients;

    // Méthodes pour les boutons
    void ajouterClient();
    void modifierClient();
    void afficherClients();
    void reinitialiserFormulaire();
    void rechercherClient();
    void trierClients();
    void exporterPDF();
    void afficherStatistiques();
};
#endif // MAINWINDOW_H
