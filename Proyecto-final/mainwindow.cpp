#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    // ================= Menú Principal =================
    menuPrincipal = new QWidget;
    menuPrincipal->setStyleSheet("background-image: url(:/imagenes/fondo menu.jpg); background-repeat: no-repeat; background-position: center; background-size: cover;");
    QVBoxLayout *layoutMenu = new QVBoxLayout(menuPrincipal);

    musicaMenu = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    musicaMenu->setAudioOutput(audioOutput);
    musicaMenu->setSource(QUrl("qrc:/sonidos/goku_dragon-ball-z-cha-la-head-cha-la.mp3"));
    musicaMenu->setLoops(QMediaPlayer::Infinite);
    audioOutput->setVolume(70);
    musicaMenu->play();

    QLabel *titulo = new QLabel("Menú Principal");
    QPushButton *botonJugar = new QPushButton("Jugar");

    layoutMenu->addWidget(titulo);
    layoutMenu->addWidget(botonJugar);

    // ================= Pantalla de Autenticación =================
    pantallaAuth = new QWidget;
    QVBoxLayout *layoutAuth = new QVBoxLayout(pantallaAuth);
    QPushButton *botonLogin = new QPushButton("Iniciar sesión");
    QPushButton *botonRegistro = new QPushButton("Registrarse");

    layoutAuth->addWidget(new QLabel("Bienvenido"));
    layoutAuth->addWidget(botonLogin);
    layoutAuth->addWidget(botonRegistro);

    // ================= Formulario Login =================
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

    // ================= Formulario Registro =================
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

    // ================= Selector de Niveles =================
    selectorNiveles = new QWidget;
    QVBoxLayout* layoutSelector = new QVBoxLayout(selectorNiveles);
    layoutSelector->addWidget(new QLabel("Selecciona un nivel:"));

    btnNivel1 = new QPushButton;
    btnNivel1->setIcon(QIcon(":/imagenes/goku vs dodoria y zarbon.png"));
    btnNivel1->setIconSize(QSize(320, 220));
    btnNivel1->setFixedSize(320, 220);
    btnNivel1->setStyleSheet("border: none;");

    btnNivel2 = new QPushButton;
    btnNivel2->setIcon(QIcon(":/imagenes/goku vs sabarman.png"));
    btnNivel2->setIconSize(QSize(320, 220));
    btnNivel2->setFixedSize(320, 220);
    btnNivel2->setStyleSheet("border: none;");

    btnNivel3 = new QPushButton;
    btnNivel3->setIcon(QIcon(":/imagenes/goku vs freezer.png"));
    btnNivel3->setIconSize(QSize(320, 220));
    btnNivel3->setFixedSize(320, 220);
    btnNivel3->setStyleSheet("border: none;");

    QVBoxLayout *botonesLayout = new QVBoxLayout;
    botonesLayout->addWidget(btnNivel1);
    botonesLayout->addWidget(btnNivel2);
    botonesLayout->addWidget(btnNivel3);
    layoutSelector->addLayout(botonesLayout);

    // ================= Agregar pantallas al stack =================
    stack->addWidget(menuPrincipal);
    stack->addWidget(pantallaAuth);
    stack->addWidget(formularioLogin);
    stack->addWidget(formularioRegistro);
    stack->addWidget(selectorNiveles);

    // ================= Conexiones =================
    connect(botonJugar, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(pantallaAuth);
    });

    connect(botonLogin, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioLogin);
    });

    connect(botonRegistro, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(formularioRegistro);
    });

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
            usuarioActivo = usuario;
            musicaMenu->stop();
            stack->setCurrentWidget(selectorNiveles);
        } else {
            QMessageBox::warning(this, "Error", "Usuario o contraseña incorrectos.");
        }
    });

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
