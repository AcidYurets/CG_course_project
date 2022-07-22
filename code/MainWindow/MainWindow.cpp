#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    ui = new Ui::MainWindowUI();
    ui->setupUi(this);
}

MainWindow::~MainWindow() { 
    delete ui; 
}