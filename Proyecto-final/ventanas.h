#ifndef VENTANAS_H
#define VENTANAS_H

#include <QWidget>
#include <QStackedWidget>
#include <QPushButton>
#include "juego.h"  // O el juego que estés desarrollando

class Ventanas : public QWidget {
    Q_OBJECT

public:
    explicit Ventanas(QWidget *parent = nullptr);

private slots:
    void mostrarMenu();
    void iniciarNivel1();
    // Agregás más niveles si querés

private:
    QStackedWidget *stack;
    QWidget *menuPrincipal;
    Juego *nivel1;
};

#endif // VENTANAS_H
