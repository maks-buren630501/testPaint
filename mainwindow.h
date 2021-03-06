#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointF>
#include <QTextStream>
#include "myline.h"
#include"myscene.h"
#include"myshape.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyShape *createShape(int mode);
    void clearTempShape();
    void setTempShapeNull();
    void shapeToFile(MyShape *shape, QTextStream &stream);
    void lineToFile(MyLine *line, QTextStream &stream);
    void setShapeFromTemp(MyShape *shape);
    void setShapeFromString(MyShape *shape, QString string);
    void setLineFromString(MyLine *line, QString string);

    static bool neerPoints(QPoint p1, QPoint p2,int accuracy);
public slots:
    void updateScene();

    void addShape();

    virtual void slotTarget(QPointF point);

    virtual void clickTarget(QPointF point);

private slots:
    void on_rectButtton_clicked();

    void on_ellipsButton_clicked();

    void on_anngleButton_clicked();

    void on_moveButton_clicked();

    void on_linkButton_clicked();

    void on_saveButton_clicked();

    void on_loadButton_clicked();

private:
    QVector<MyShape*> shapes;
    QVector<MyLine*> lines;
    MyShape *tempShape;
    MyLine *tempLine;
    MyShape* movedShape;
    Ui::MainWindow *ui;
    MyScene  *scene;    // We declare a graphic scene
    QTimer *timer;
    int mode;
    int fl;
    int x,y;
};
#endif // MAINWINDOW_H
