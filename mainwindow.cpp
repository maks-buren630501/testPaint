#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"myrect.h"
#include<iostream>
#include"myellipce.h"
#include"mypolygon.h"
#include"myline.h"
#include <QFileDialog>
#include <QLineF>
#include <QTextStream>
#include<iphlpapi.h>
#include <typeinfo>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     mode = 1;
     timer = new QTimer();
     connect(timer, &QTimer::timeout, this, &MainWindow::updateScene);
     timer->start(10);
     scene   = new MyScene();

     ui->graphicsView->setScene(scene);  /// set scene into graphicsView
     ui->graphicsView->setRenderHint(QPainter::Antialiasing);
     ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     scene->setSceneRect(0,0,500,500);

     QCursor cursor = QCursor();
     ui->graphicsView->setCursor(cursor);

     ui->graphicsView->setMouseTracking(true);

     tempShape = nullptr;
     movedShape = nullptr;
     tempLine = nullptr;

     connect(scene, &MyScene::signalTargetCoordinate, this, &MainWindow::slotTarget);
     connect(scene, &MyScene::signalMouseClick, this, &MainWindow::clickTarget);


}

MainWindow::~MainWindow()
{
    delete ui;
}

MyShape *MainWindow::createShape(int mode)
{
    switch (mode)
    {
    case 1:
    {
        return new MyRect();
        break;
    }
    case 2:
    {
        return new MyEllipce();
        break;
    }
    case 3:
    {
        return new MyPolygon();
        break;
    }
    default:
    {
        return new MyShape();
        break;
    }
    }
}

void MainWindow::clearTempShape()
{
    scene->removeItem(tempShape);
    if(tempShape != nullptr)
    {
        delete tempShape;
    }
    tempShape = nullptr;
    scene->removeItem(movedShape);
    if(movedShape != nullptr)
    {
        delete movedShape;
    }
    movedShape = nullptr;
}

void MainWindow::shapeToFile(MyShape *shape, QTextStream &stream)
{
    if(shape->getName().split("/").at(0).toInt() == 3)
    {
        stream << shape->getName() << "|" <<QString().number(shape->x()) << "|" << QString().number(shape->y())<<"|"<<shape->getCenter().x() <<"|"<<shape->getCenter().y()<<"|";
        auto polygon = shape->getPolygon();
        for(auto point : polygon)
        {
            stream << point.x() << "|"<<point.y()<<"|";
        }
        stream << "\n";
    }
    else
    {
        stream << shape->getName() << "|" <<QString().number(shape->x()) << "|" << QString().number(shape->y()) << "|" << shape->getPointStart().x() <<"|"  << shape->getPointStart().y() <<"|"<<shape->getPointEnd().x() <<"|"  << shape->getPointEnd().y()<<"|" <<shape->getCenter().x() <<"|"<<shape->getCenter().y()<<"\n";
    }
}

void MainWindow::lineToFile(MyLine *line, QTextStream &stream)
{
    stream << line->getPointStart()->getName() << "|"<<line->getPointEnd()->getName()<<"\n";
}

void MainWindow::setShapeFromTemp(MyShape *shape)
{
    QString name;
    shape->setPointStart(tempShape->getPointStart());
    shape->setPointEnd(tempShape->getPointEnd());
    shape->setPolygon(tempShape->getPolygon());
    shape->setPos(tempShape->x(),tempShape->y());
    shape->setCenter(tempShape->getCenter().x(),tempShape->getCenter().y());
    shape->setPainted(true);
    name += QString().number(mode) + "/" +QString().number(shapes.size()+1);
    shape->setName(name);
}

void MainWindow::setShapeFromString(MyShape *shape, QString string)
{
    auto vStr = string.split("|");
    auto vName = vStr[0].split("/");
    shape->setName(vStr[0]);
    shape->setX(vStr[1].toInt());
    shape->setY(vStr[2].toInt());
    if(vName[0].toInt() == 3)
    {
        shape->setCenter(vStr[3].toInt(),vStr[4].toInt());
        for(int i = 5; i < vStr.size()-1; i+= 2)
        {
            shape->setPolygon(shape->getPolygon() << QPoint(vStr[i].toInt(),vStr[i+1].toInt()));
        }

    }
    else
    {
        shape->setPointStart(QPoint(vStr[3].toInt(),vStr[4].toInt()));
        shape->setPointEnd(QPoint(vStr[5].toInt(),vStr[6].toInt()));
        shape->setCenter(vStr[7].toInt(),vStr[8].toInt());
    }
    shape->setPainted(true);
}

void MainWindow::setLineFromString(MyLine *line, QString string)
{
    auto vStr = string.split("|");
    for(auto shape : shapes)
    {
        if(shape->getName() == vStr[0])
        {
            line->setPointStart(shape);
        }
        if(shape->getName() == vStr[1])
        {
            line->setPointEnd(shape);
        }
    }
    line->setPainted(true);
}

bool MainWindow::neerPoints(QPoint p1, QPoint p2,int accuracy)
{
    if(abs(p1.x() - p2.x())<accuracy && abs(p1.y() - p2.y())<accuracy)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void MainWindow::updateScene()
{
    scene->update();
    if(tempShape != nullptr)
    {
        return;
    }
    tempShape = createShape(mode);
    if(tempShape != nullptr)
    {
        tempShape->setPos(0,0);
        scene->addItem(tempShape);
        connect(scene, &MyScene::signalTargetCoordinate, tempShape, &MyShape::slotTarget);
        connect(tempShape, &MyShape::addShapeSignal, this, &MainWindow::addShape);
    }
}

void MainWindow::addShape()
{

    if(tempShape == nullptr)
    {
        return;
    }
    MyShape *shape;
    shape = createShape(mode);
    if(mode < 4)
    {
        setShapeFromTemp(shape);
        scene->addItem(shape);
        shapes.push_back(shape);
        clearTempShape();
     }
    else
    {
        return;
    }
}

void MainWindow::slotTarget(QPointF point)
{
    if(mode == 4)
    {
        if(movedShape == nullptr && GetAsyncKeyState(VK_LBUTTON))
        {
            for(auto shape : shapes)
            {
                if(neerPoints(shape->getCenter(),point.toPoint(),30))
                {
                    movedShape = shape;
                    break;
                }
            }
        }
        if(movedShape != nullptr && GetAsyncKeyState(VK_LBUTTON))
        {
            movedShape->setPontEndStartOffset(point.x(),point.y());
        }
        if(movedShape != nullptr && !GetAsyncKeyState(VK_LBUTTON))
        {
            movedShape = nullptr;
        }
    }
}

void MainWindow::clickTarget(QPointF point)
{
    if(mode == 5)
    {
        if(tempLine == nullptr)
        {
            for(auto shape : shapes)
            {
                if(neerPoints(shape->getCenter(),point.toPoint(),30))
                {
                    tempLine = new MyLine;
                    connect(scene, &MyScene::signalTargetCoordinate, tempLine, &MyShape::slotTarget);
                    tempLine->setPointStart(shape);
                    scene->addItem(tempLine);
                    return;
                }
            }
        }
        if(tempLine != nullptr)
        {
            for(auto shape : shapes)
            {
                if(neerPoints(shape->getCenter(),point.toPoint(),30))
                {
                    tempLine->setPointEnd(shape);
                    tempLine->setPainted(true);
                    lines.push_back(tempLine);
                    tempLine = nullptr;
                    return;
                }
            }
        }
    }
}
void MainWindow::on_rectButtton_clicked()
{
    mode = 1;
    clearTempShape();
}

void MainWindow::on_ellipsButton_clicked()
{
    mode = 2;
    clearTempShape();
}

void MainWindow::on_anngleButton_clicked()
{
    mode = 3;
    clearTempShape();
}

void MainWindow::on_moveButton_clicked()
{
    mode = 4;
    clearTempShape();
}

void MainWindow::on_linkButton_clicked()
{
    mode = 5;
    clearTempShape();
}

void MainWindow::on_saveButton_clicked()
{
    auto fileName = QFileDialog::getSaveFileName(this,  "test files (*.test)");
    fileName += ".test";
    QFile file(fileName);
    file.open(QFile::WriteOnly);
    QTextStream write(&file);
    for(auto shape : shapes)
    {
        shapeToFile(shape,write);
    }
    write << "---------------------------------------------------\n";
    for(auto line : lines)
    {
        lineToFile(line,write);
    }
    file.close();
}

void MainWindow::on_loadButton_clicked()
{
    lines.clear();
    shapes.clear();
    mode = 0;
    clearTempShape();
    scene->clear();
    fl = 0;
    auto fileName = QFileDialog::getOpenFileName(this,
                tr("Save install file"), "",
                tr("install file (*.*);;All Files (*)"));
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QTextStream read(&file);
    int flw = 0;
    while(!read.atEnd())
    {
        auto str = read.readLine();
        if(str.indexOf("-----------") != -1)
        {
            flw = 1;
            continue;
        }
        if(flw == 0)
        {
            auto vStr = str.split("|");
            auto vName = vStr[0].split("/");
            MyShape *myShape = createShape(vName[0].toInt());
            setShapeFromString(myShape,str);
            shapes.push_back(myShape);
            scene->addItem(myShape);
        }
        else
        {
            MyLine *line = new MyLine();
            setLineFromString(line,str);
            lines.push_back(line);
            scene->addItem(line);
        }
    }
    mode = 1;
}
