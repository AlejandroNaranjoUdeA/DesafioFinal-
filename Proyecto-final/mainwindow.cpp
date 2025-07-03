#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    // ================== Menú principal ==================
    menuPrincipal = new QWidget;
    menuPrincipal->setStyleSheet("background-image: url(:/imagenes/fondo menu.jpg); background-size: cover;");
    QVBoxLayout *layoutMenu = new QVBoxLayout(menuPrincipal);
    QLabel *titulo = new QLabel("Menú Principal");
    QPushButton *botonNivel1 = new QPushButton("Jugar");
    layoutMenu->addWidget(titulo);
    layoutMenu->addWidget(botonNivel1);

    // ================== Música del menú ==================
    musicaMenu = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicaMenu->setAudioOutput(audioOutput);
    musicaMenu->setSource(QUrl("qrc:/sonidos/goku_dragon-ball-z-cha-la-head-cha-la.mp3"));
    musicaMenu->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(70);
    musicaMenu->play();

    // ================== Pantalla de autenticación ==================
    pantallaAuth = new QWidget;
    QVBoxLayout *layoutAuth = new QVBoxLayout(pantallaAuth);
    QPushButton *botonLogin = new QPushButton("Iniciar sesión");
    QPushButton *botonRegistro = new QPushButton("Registrarse");
    layoutAuth->addWidget(new QLabel("Bienvenido"));
    layoutAuth->addWidget(botonLogin);
    layoutAuth->addWidget(botonRegistro);

    // ================== Formulario login ==================
    formularioLogin = new QWidget;
    QVBoxLayout *layoutLogin = new QVBoxLayout(formularioLogin);
    entradaUsuarioLogin = new QLineEdit();
    entradaContrasenaLogin = new QLineEdit();
    entradaContrasenaLogin->setEchoMode(QLineEdit::Password);
    QPushButton *botonEntrar = new QPushButton("Entrar");
    layoutLogin->addWidget(new QLabel("Usuario:"));
    layoutLogin->addWidget(entradaUsuarioLogin);
    layoutLogin->addWidget(new QLabel("Contraseña:"));
    layoutLogin->addWidget(entradaContrasenaLogin);
    layoutLogin->addWidget(botonEntrar);

    // ================== Formulario registro ==================
    formularioRegistro = new QWidget;
    QVBoxLayout *layoutRegistro = new QVBoxLayout(formularioRegistro);
    entradaUsuarioRegistro = new QLineEdit();
    entradaContrasenaRegistro = new QLineEdit();
    entradaContrasenaRegistro->setEchoMode(QLineEdit::Password);
    QPushButton *botonCrear = new QPushButton("Crear cuenta");
    layoutRegistro->addWidget(new QLabel("Nuevo usuario:"));
    layoutRegistro->addWidget(entradaUsuarioRegistro);
    layoutRegistro->addWidget(new QLabel("Contraseña:"));
    layoutRegistro->addWidget(entradaContrasenaRegistro);
    layoutRegistro->addWidget(botonCrear);

    // ================== Pantalla selección de nivel ==================
    pantallaSeleccionNivel = new QWidget;
    QVBoxLayout *layoutNiveles = new QVBoxLayout(pantallaSeleccionNivel);
    QLabel *texto = new QLabel("Seleccione un nivel");
    QPushButton *btnNivel1 = new QPushButton("Nivel 1");
    QPushButton *btnNivel2 = new QPushButton("Nivel 2");
    QPushButton *btnNivel3 = new QPushButton("Nivel 3");
    layoutNiveles->addWidget(texto);
    layoutNiveles->addWidget(btnNivel1);
    layoutNiveles->addWidget(btnNivel2);
    layoutNiveles->addWidget(btnNivel3);

    // ================== Agregar widgets al stack ==================
    stack->addWidget(menuPrincipal);         // 0
    stack->addWidget(pantallaAuth);          // 1
    stack->addWidget(formularioLogin);       // 2
    stack->addWidget(formularioRegistro);    // 3
    stack->addWidget(pantallaSeleccionNivel);// 4

    // ================== Conexiones navegación ==================
    connect(botonNivel1, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(pantallaAuth);
    });

    connect(botonLogin, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioLogin);
    });

    connect(botonRegistro, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioRegistro);
    });

    // ================== Crear cuenta ==================
    connect(botonCrear, &QPushButton::clicked, this, [this]() {
        QString usuario = entradaUsuarioRegistro->text();
        QString contrasena = entradaContrasenaRegistro->text();

        if (usuario.isEmpty() || contrasena.isEmpty()) {
            QMessageBox::warning(this, "Error", "Debe llenar todos los campos.");
            return;
        }

        QFile archivo("usuarios.txt");
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
            return;
        }

        QTextStream in(&archivo);
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(",");
            if (partes.size() == 2 && partes[0] == usuario) {
                QMessageBox::warning(this, "Error", "El usuario ya existe.");
                archivo.close();
                return;
            }
        }
        archivo.close();

        if (!archivo.open(QIODevice::Append | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo para escribir.");
            return;
        }

        QTextStream out(&archivo);
        out << usuario << "," << contrasena << "\n";
        archivo.close();

        QMessageBox::information(this, "Éxito", "Usuario registrado con éxito.");
        entradaUsuarioRegistro->clear();
        entradaContrasenaRegistro->clear();
        stack->setCurrentWidget(pantallaAuth);
    });

    // ================== Iniciar sesión ==================
    connect(botonEntrar, &QPushButton::clicked, this, [this]() {
        QString usuario = entradaUsuarioLogin->text();
        QString contrasena = entradaContrasenaLogin->text();

        if (usuario.isEmpty() || contrasena.isEmpty()) {
            QMessageBox::warning(this, "Error", "Debe llenar todos los campos.");
            return;
        }

        QFile archivo("usuarios.txt");
        if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Error", "No se pudo abrir el archivo.");
            return;
        }

        QTextStream in(&archivo);
        bool encontrado = false;
        while (!in.atEnd()) {
            QString linea = in.readLine();
            QStringList partes = linea.split(",");
            if (partes.size() == 2 && partes[0] == usuario && partes[1] == contrasena) {
                encontrado = true;
                break;
            }
        }
        archivo.close();

        if (encontrado) {
            QMessageBox::information(this, "Éxito", "Inicio de sesión exitoso.");
            entradaUsuarioLogin->clear();
            entradaContrasenaLogin->clear();

            usuarioActivo = usuario;
            musicaMenu->stop();
            stack->setCurrentWidget(pantallaSeleccionNivel);
        } else {
            QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        }
    });

    // ================== Iniciar nivel desde pantalla de niveles ==================
    connect(btnNivel1, &QPushButton::clicked, this, [this]() {
        nivel1 = new Juego(1);
        nivel1->mostrarNombreUsuario(usuarioActivo);
        stack->addWidget(nivel1);
        stack->setCurrentWidget(nivel1);
    });

    connect(btnNivel2, &QPushButton::clicked, this, [this]() {
        nivel2 = new Juego(2);
        nivel2->mostrarNombreUsuario(usuarioActivo);
        stack->addWidget(nivel2);
        stack->setCurrentWidget(nivel2);
    });

    connect(btnNivel3, &QPushButton::clicked, this, [this]() {
        nivel3 = new Juego(3);
        nivel3->mostrarNombreUsuario(usuarioActivo);
        stack->addWidget(nivel3);
        stack->setCurrentWidget(nivel3);
    });

    mostrarMenu();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::mostrarMenu() {
    stack->setCurrentWidget(menuPrincipal);
}
