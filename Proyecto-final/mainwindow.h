#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QMediaPlayer>
#include <QAudioOutput>
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
    QWidget *pantallaSeleccionNivel;

    QLineEdit *entradaUsuarioLogin;
    QLineEdit *entradaContrasenaLogin;
    QLineEdit *entradaUsuarioRegistro;
    QLineEdit *entradaContrasenaRegistro;

    QMediaPlayer *musicaMenu;
    QAudioOutput *audioOutput;

    Juego *nivel1 = nullptr;
    Juego *nivel2 = nullptr;
    Juego *nivel3 = nullptr;

    QString usuarioActivo;
};

#endif // MAINWINDOW_H
