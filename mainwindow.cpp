#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_board = new Board(this);
    m_gameplaySettings = new gameplaySettings(this);
    m_gameplaySettings->setBoard(m_board);
}

MainWindow::~MainWindow()
{
    delete m_gameplaySettings;
    delete m_board;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_gameplaySettings->exec();
}

