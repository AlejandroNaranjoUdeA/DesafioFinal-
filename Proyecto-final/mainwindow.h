#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include "juego.h"
#include <QMediaPlayer>
#include <QAudioOutput>


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

private slots:
    void mostrarMenu();
    void iniciarNivel1();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stack;
    QWidget *menuPrincipal;
    Juego *nivel1;

    QMediaPlayer *musicaMenu;
    QAudioOutput *audioOutput;

};
#endif // MAINWINDOW_H
