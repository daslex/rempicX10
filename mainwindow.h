#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>
#include <set>
#include <fstream>

using namespace std;

struct ContainerSrc {

    class Status{
        unsigned value;
    public:
        inline void SetNormal(){value = 2;}
        inline void SetBad() {value = 0;}
        inline void SetGood(){value = 1;}

        inline unsigned getValue() const {return value;}
    };

    Status mutable status;
    wstring src;

    const bool operator< (const ContainerSrc &rvalue) const{
        wstring x = this->src;
        wstring y = rvalue.src;
        return lexicographical_compare(x.begin(),x.end(),y.begin(),y.end());
    }
};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;

    void start();
    set<wstring> getExtension();
    void getFileList(set<wstring>& PicExtension);
    set<ContainerSrc> allSrc;
    set<ContainerSrc>::iterator current;

    void stepRightNormal();
    void stepLefttNormal();

    void draw(const wstring src);
    void save();
    void keyPressEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
