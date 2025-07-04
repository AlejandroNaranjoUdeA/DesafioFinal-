#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QLineEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include "juego.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void mostrarMenu();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stack;
    QWidget *menuPrincipal;
    QWidget *pantallaAuth;
    QWidget *formularioLogin;
    QWidget *formularioRegistro;
    QWidget *selectorNiveles;

    QLineEdit *entradaUsuarioLogin;
    QLineEdit *entradaContrasenaLogin;
    QLineEdit *entradaUsuarioRegistro;
    QLineEdit *entradaContrasenaRegistro;

    QPushButton *btnNivel1;
    QPushButton *btnNivel2;
    QPushButton *btnNivel3;

    QMediaPlayer *musicaMenu;
    QAudioOutput *audioOutput;

    QString usuarioActivo;

    Juego *nivel1;
    Juego *nivel2;
    Juego *nivel3;
};

#endif // MAINWINDOW_H
