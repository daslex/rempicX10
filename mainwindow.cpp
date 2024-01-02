#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <filesystem>
#include <QDebug>
//#include <fstream>
#include <string>
#include <algorithm>
#include <cwctype>
#include <QPixmap>
#include <QKeyEvent>

using namespace std;

namespace fs=std::filesystem;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    ui->statusbar->showMessage("");
    const wstring PATH = ui->lineEdit->text().toStdWString() + L"\\progress_x10";

   start();
}

void MainWindow::start(){
    allSrc.clear();
    ui->statusbar->showMessage("Старт");
    set<wstring> picExtensions = getExtension();
    getFileList(picExtensions);

    if (!allSrc.empty()){
        current = allSrc.begin();
        draw(current->src);
    }

}

set<wstring> MainWindow::getExtension(){
    set<wstring> extensions;
    wstring S;

    wifstream f("picExtensions.txt");
        while (getline(f,S)){
            extensions.insert(S);
        }
    f.close();



    return extensions;
}

void MainWindow::getFileList(set<wstring>& PicExtension){


    if (ui->lineEdit->text().isEmpty()){
        ui->statusbar->showMessage("Пустой адрес");
        return;
    }

    const wstring PATH = ui->lineEdit->text().toStdWString();
    for (const auto &i: fs::recursive_directory_iterator(PATH)){
        wstring ext = i.path().extension().wstring();
        transform(ext.begin(),ext.end(),ext.begin(),::tolower);
        if (PicExtension.find(ext) != PicExtension.end()){
            /*тут вставить проверку для обновления*/
            ContainerSrc currentSrc;
            currentSrc.status.SetNormal();
            currentSrc.src = i.path().wstring();
            wstring currentExtension = i.path().extension();
            if (PicExtension.find(currentExtension) != PicExtension.end()){
                allSrc.insert(currentSrc);
            }

            /*конец проверки для обновления*/
        }
    }
}

void MainWindow::stepRightNormal()
{
    while (next(current) != allSrc.end()){
        current = next(current);
        if (current->status.getValue() == 2){
            draw(current->src);
            return;
        }
    }
}

void MainWindow::stepLefttNormal()
{
    while (current != allSrc.begin()){
        current = prev(current);
        if (current->status.getValue() == 2){
            draw(current->src);
            return;
        }
    }
}

void MainWindow::draw(const wstring src)
{
    ui->statusbar->showMessage("Отображаю изображение");
    QPixmap pic;
    pic.load(QString::fromStdWString(src));
    pic = pic.scaled(this->size()*0.90,Qt::KeepAspectRatio);

    ui->label->setPixmap(pic);
    ui->label->setFocus();
    ui->statusbar->showMessage(QString::fromStdWString(current->src));
}

void MainWindow::save()
{
    if (allSrc.empty()){
        return;
    }

    wstring FNAME = ui->lineEdit->text().toStdWString() + L"\\data.txt";
    wofstream f(FNAME, ios::binary);

    auto it = allSrc.begin();
    qDebug() << allSrc.size();
    qDebug() << current->src;
    f << current->src << L'\n';

    for (const auto &i: allSrc){
        qDebug() << i.src;
        f << i.src << L'\n';
        qDebug() << i.status.getValue();
        f << i.status.getValue() << L'\n';
    }

    f.close();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
        case Qt::Key_Right:
            stepRightNormal();
        break;
        case Qt::Key_Left:
            stepLefttNormal();
        break;
        case Qt::Key_Delete:
            current->status.SetBad();
            stepRightNormal();
        break;
    }
}




void MainWindow::on_pushButton_5_clicked()
{
    auto position = allSrc.begin();

    while (position != allSrc.end()){
        qDebug() << position->src << ": " << position->status.getValue();
        position = next(position);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
   save();
}

