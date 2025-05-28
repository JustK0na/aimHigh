#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <time.h>
#include <QFile>
#include <QDir>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QSceneLoader>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>

#include <Qt3DExtras/QForwardRenderer>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    stats = new QWidget(this);
    issInfo = new QLabel(tr("Waiting for ISS data..."), stats);
    QFont font("Arial", 12);
    issInfo->setFont(font);
    QVBoxLayout *statsLayout = new QVBoxLayout(stats);
    statsLayout->addWidget(issInfo);




    earthModel = new QWidget(this);
    earthModel->setStyleSheet("background-color: black; border: 2px solid gray;");
    QVBoxLayout *earthLayout = new QVBoxLayout(earthModel);
    earthLayout->setContentsMargins(5, 5, 5, 5);

    earthView = new earthWidget(earthModel);
    earthLayout->addWidget(earthView);


   /* container = new QWidget(this);
    container->setStyleSheet("background-color: red; border: 2px solid gray;");
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->setContentsMargins(5, 5, 5, 5);*/
    container = init3DScene();

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);



    mainLayout->addWidget(earthModel, 3);
    mainLayout->addWidget(container, 3);
    mainLayout->addWidget(stats, 1);
    setCentralWidget(central);

    apiHandler = new APIhandler(this);
    connect(apiHandler, &APIhandler::update, this, &MainWindow::updateISSData);
    connect(apiHandler, &APIhandler::update, earthView, &earthWidget::updateISSData);
    apiHandler->startConnection(5000);
}

void MainWindow::updateISSData(double latitude, double longitude, double altitude, double velocity, double timestamp){
    int timestampINT = (int)timestamp;
    //from 1 january 1970
    //+2 h bo UTC.

    struct tm ts;
    char buf[80];
    time_t now = (time_t)timestampINT;

    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);

    QString info = tr("ISS is now here: \nLatitude: %1\nLongitude: %2\nAltitude: %3 km\nVelocity: %4 km\\h\nDate: %5")
                       .arg(latitude, 0, 'f', 2)
                       .arg(longitude, 0, 'f', 2)
                       .arg(altitude, 0, 'f', 2)
                       .arg(velocity, 0, 'f', 2)
                       .arg(buf);
    issInfo->setText(info);
    QFont font("Arial", 12);
    issInfo->setFont(font);
}

QWidget* MainWindow::init3DScene() {
    auto *view = new Qt3DExtras::Qt3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    auto *container = QWidget::createWindowContainer(view);
    container->setMinimumSize(400, 400);  // Can resize

    auto *rootEntity = new Qt3DCore::QEntity();

    auto *loader = new Qt3DRender::QSceneLoader(rootEntity);
    QString modelPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../../repo/aimHigh/aimHigh/models/test1.gltf");
    qDebug() << "Model path:" << modelPath;
    qDebug() << "Exists:" << QFile::exists(modelPath);
    loader->setSource(QUrl::fromLocalFile(modelPath));
    rootEntity->addComponent(loader);

    auto *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.f/9.f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20));
    camera->setViewCenter(QVector3D(0, 0, 0));

    auto *controller = new Qt3DExtras::QOrbitCameraController(rootEntity);
    controller->setCamera(camera);

    view->setRootEntity(rootEntity);

    return container;
}

MainWindow::~MainWindow(){
    delete ui;
}
