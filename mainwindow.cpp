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
     scene   = new MyScene();    // Init custom scene

     ui->graphicsView->setScene(scene);  /// set scene into graphicsView
     ui->graphicsView->setRenderHint(QPainter::Antialiasing);
     ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

     scene->setSceneRect(0,0,500,500);

     // Create Custom Cursor
     QCursor cursor = QCursor();
     ui->graphicsView->setCursor(cursor);    // Set cursor intoe QGraphicsView

     ui->graphicsView->setMouseTracking(true);

     stateMove = false;
     tempShape = nullptr;
     movedShape = nullptr;

     fl = 0;

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

void MainWindow::clearShape()
{
    scene->removeItem(tempShape);
    if(tempShape != nullptr)
    {
        delete tempShape;
    }
    tempShape = nullptr;
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
        tempShape->setPos(250,250);
        scene->addItem(tempShape);
        connect(scene, &MyScene::signalTargetCoordinate, tempShape, &MyShape::slotTarget);
        connect(tempShape, &MyShape::addShapeSignal, this, &MainWindow::addShape);
    }
}

void MainWindow::addShape()
{
    QString name;
    if(tempShape == nullptr)
    {
        return;
    }
    MyShape *shape;
    shape = createShape(mode);
    if(mode < 4)
    {
        shape->setPointStart(tempShape->getPointStart());
        shape->setPointEnd(tempShape->getPointEnd());
        shape->setPolygon(tempShape->getPolygon());
        shape->setPos(tempShape->x(),tempShape->y());
        shape->setCenter(tempShape->getCenter().x(),tempShape->getCenter().y());
        shape->setPainted(true);
        name += QString().number(mode) + "/" +QString().number(shapes.size()+1);
        shape->setName(name);
        scene->removeItem(tempShape);
        scene->addItem(shape);
        shapes.push_back(shape);
        std::cout<<shapes.size()<<std::endl;
        tempShape = nullptr;
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
        if(!stateMove && GetAsyncKeyState(VK_LBUTTON))
        {
            for(auto shape : shapes)
            {
                auto pointCenter = shape->getCenter();
                if(abs(pointCenter.x() - (point.x())) < 10 && abs(pointCenter.y() - (point.y())) < 10)
                {
                    x = point.x();
                    y = point.y();
                    X = shape->x();
                    Y = shape->y();
                    cx = pointCenter.x();
                    cy = pointCenter.y();
                    movedShape = shape;
                    stateMove = true;
                    break;
                }
            }
        }
        if(stateMove && GetAsyncKeyState(VK_LBUTTON))
        {
            int dx = point.x() - x;
            int dy = point.y() - y;

            movedShape->setX(X + dx);
            movedShape->setY(Y + dy);
            movedShape->setCenter(cx + dx,cy + dy);
        }
        if(stateMove && !GetAsyncKeyState(VK_LBUTTON))
        {
            stateMove = false;
            movedShape = nullptr;
        }
    }

}

void MainWindow::clickTarget(QPointF point)
{
    if(mode == 5)
    {
        if(fl == 0)
        {
            for(auto shape : shapes)
            {
                auto pointCenter = shape->getCenter();
                if(abs(pointCenter.x() - (point.x())) < 30 && abs(pointCenter.y() - (point.y())) < 30)
                {
                    line = new MyLine;
                    line->setPointStart(shape);
                    fl = 1;
                    return;
                }
            }
        }
        if(fl == 1)
        {
            for(auto shape : shapes)
            {
                auto pointCenter = shape->getCenter();
                if(abs(pointCenter.x() - (point.x())) < 30 && abs(pointCenter.y() - (point.y())) < 30)
                {
                    line->setPointEnd(shape);
                    fl = 0;
                    scene->addItem(line);
                    lines.push_back(line);
                    return;
                }
            }
        }
    }
}
void MainWindow::on_rectButtton_clicked()
{
    mode = 1;
    clearShape();

}

void MainWindow::on_ellipsButton_clicked()
{
    mode = 2;
    clearShape();
}

void MainWindow::on_anngleButton_clicked()
{
    mode = 3;
    clearShape();
}

void MainWindow::on_moveButton_clicked()
{
    mode = 4;
    clearShape();
}

void MainWindow::on_linkButton_clicked()
{
    mode = 5;
    clearShape();
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
    if(tempShape != nullptr)
    {
        delete tempShape;
        tempShape = nullptr;
    }
    if(movedShape != nullptr)
    {
        delete movedShape;
        movedShape = nullptr;
    }
    scene->clear();
    stateMove = false;
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
            myShape->setName(vStr[0]);
            myShape->setX(vStr[1].toInt());
            myShape->setY(vStr[2].toInt());
            if(vName[0].toInt() == 3)
            {
                myShape->setCenter(vStr[3].toInt(),vStr[4].toInt());
                for(int i = 5; i < vStr.size()-1; i+= 2)
                {
                    myShape->setPolygon(myShape->getPolygon() << QPoint(vStr[i].toInt(),vStr[i+1].toInt()));
                }

            }
            else
            {
                myShape->setPointStart(QPoint(vStr[3].toInt(),vStr[4].toInt()));
                myShape->setPointEnd(QPoint(vStr[5].toInt(),vStr[6].toInt()));
                myShape->setCenter(vStr[7].toInt(),vStr[8].toInt());
            }
            myShape->setPainted(true);
            shapes.push_back(myShape);
            scene->addItem(myShape);
        }
        else
        {
            MyLine *line = new MyLine();
            auto vStr = str.split("|");
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
                lines.push_back(line);
                scene->addItem(line);
            }
        }
    }
    mode = 1;
}
