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
#include <Qt3DRender/QPointLight>

#include <Qt3DExtras/QForwardRenderer>

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    stats = new QWidget(this);
    issInfo = new QLabel(tr("Waiting for ISS data..."), stats);
    QFont font("Arial", 12);
    issInfo->setFont(font);
    QVBoxLayout *statsLayout = new QVBoxLayout(stats);
    statsLayout->addWidget(issInfo);

    dateTimeEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    dateTimeEdit->setCalendarPopup(true);

    fetchDateButton = new QPushButton(tr("Show ISS at Selected Time"), this);

    statsLayout->addWidget(dateTimeEdit);
    statsLayout->addWidget(fetchDateButton);

    connect(fetchDateButton, &QPushButton::clicked, this, &MainWindow::fetchISSAtSelectedTime);


    resumeLiveButton = new QPushButton(tr("Resume Live Tracking"), this);
    statsLayout->addWidget(resumeLiveButton);

    connect(resumeLiveButton, &QPushButton::clicked, this, &MainWindow::resumeLiveTracking);


    earthModel = new QWidget(this);
    // earthModel->setStyleSheet("background-color: black; border: 2px solid gray;");
    // QVBoxLayout *earthLayout = new QVBoxLayout(earthModel);
    // earthLayout->setContentsMargins(5, 5, 5, 5);

    earthView = new earthWidget(earthModel);
    // earthLayout->addWidget(earthView);


    // container = new QWidget(this);
    // container->setStyleSheet("background-color: red; border: 2px solid gray;");
    // QVBoxLayout *containerLayout = new QVBoxLayout(container);
    // containerLayout->setContentsMargins(5, 5, 5, 5);
    container = init3DScene();

    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);


    QPushButton *plButton = new QPushButton("PL", stats);
    QPushButton *enButton = new QPushButton("EN", stats);

    plButton->setFixedWidth(40);
    enButton->setFixedWidth(40);


    QHBoxLayout *langButtonLayout = new QHBoxLayout();
    langButtonLayout->addStretch();
    langButtonLayout->addWidget(plButton);
    langButtonLayout->addWidget(enButton);


    statsLayout->insertLayout(0, langButtonLayout);


    connect(plButton, &QPushButton::clicked, this, [=]() { switchLanguage("pl"); });
    connect(enButton, &QPushButton::clicked, this, [=]() { switchLanguage("en"); });


    mainLayout->addWidget(container, 2);
    mainLayout->addWidget(stats, 1);
    setCentralWidget(central);




    //mainLayout->addWidget(earthModel, 2);
    mainLayout->addWidget(container, 2);
    mainLayout->addWidget(stats, 1);

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

    if (issTransform) {
        QVector3D pos = convertLatLonTo3D(latitude, longitude, altitude);
        issTransform->setTranslation(pos);
    }
    updateEarthRotation(timestamp);

}

QWidget* MainWindow::init3DScene() {
    auto *view = new Qt3DExtras::Qt3DWindow();
    // view->defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));
    view->defaultFrameGraph()->setClearColor(Qt::transparent);


    auto *container = QWidget::createWindowContainer(view);
    container->setMinimumSize(400, 400);

    auto *rootEntity = new Qt3DCore::QEntity();

    earthSystemRoot = new Qt3DCore::QEntity(rootEntity);
    earthRotationTransform = new Qt3DCore::QTransform();
    earthSystemRoot->addComponent(earthRotationTransform);

    // Qt3DCore::QEntity *lightEntity = new Qt3DCore::QEntity(rootEntity);
    // auto *light = new Qt3DRender::QPointLight(lightEntity);
    // light->setColor(Qt::white);
    // light->setIntensity(3.0f);
    // light->setConstantAttenuation(0.5f);


    // auto *lightTransform = new Qt3DCore::QTransform();
    // lightTransform->setTranslation(QVector3D(0.0f, 100.0f, 230.0f));  // ustawić na zwrotnik raka

    // lightEntity->addComponent(light);
    // lightEntity->addComponent(lightTransform);


    auto *loader = new Qt3DRender::QSceneLoader(rootEntity);
    QString modelPath = QDir::cleanPath(QCoreApplication::applicationDirPath() + "/../../../repo/aimHigh/aimHigh/models/model.gltf");
    qDebug() << "Model path:" << modelPath;
    qDebug() << "Exists:" << QFile::exists(modelPath);
    loader->setSource(QUrl::fromLocalFile(modelPath));
    rootEntity->addComponent(loader);



    QObject::connect(loader, &Qt3DRender::QSceneLoader::statusChanged, this, [=](Qt3DRender::QSceneLoader::Status status) {
        if (status == Qt3DRender::QSceneLoader::Ready) {
            Qt3DCore::QEntity *issEntity = loader->entity("Sphere.001");

            if (issEntity) {
                this->issTransform = new Qt3DCore::QTransform();
                issEntity->addComponent(this->issTransform);
                this->issTransform->setScale(0.13f);
                qDebug() << "Sphere.001 loaded and transform added.";
            } else {
                qDebug() << "Sphere.001 not found!";
            }
        }
    });

    auto *camera = view->camera();
    camera->lens()->setPerspectiveProjection(45.0f, 16.f/9.f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0, 0, 20));
    camera->setViewCenter(QVector3D(0, 0, 0));

    auto *controller = new Qt3DExtras::QOrbitCameraController(rootEntity);
    controller->setCamera(camera);

    view->setRootEntity(rootEntity);

    return container;
}
QVector3D MainWindow::convertLatLonTo3D(float lat, float lon, float altKm) {
    const float earthRadius = 1.0f;
    const float scale = 1.0f / 6371.0f;

    float latRad = qDegreesToRadians(lat);
    float lonRad = qDegreesToRadians(-lon);
    float offset = 0.5f;
    float trueAlt = earthRadius + (altKm * scale) + offset;

    float x = trueAlt * cos(latRad) * cos(lonRad);
    float y = trueAlt * sin(latRad);
    float z = trueAlt * cos(latRad) * sin(lonRad);

    return QVector3D(x, y, z);
}
void MainWindow::updateEarthRotation(double timestamp) {
    if (!earthRotationTransform) return;

    const int secondsInDay = 86400;
    double dayFraction = fmod(timestamp, secondsInDay) / secondsInDay;
    float angleDeg = static_cast<float>(dayFraction * 360.0f + 180.0f);

    QMatrix4x4 matrix;
    matrix.rotate(angleDeg, QVector3D(0, 1, 0)); // Rotate around Y-axis

    earthRotationTransform->setMatrix(matrix);
}
void MainWindow::fetchISSAtSelectedTime() {
    QDateTime selectedDateTime = dateTimeEdit->dateTime();
    qint64 timestamp = selectedDateTime.toSecsSinceEpoch();

    if (apiHandler) {
        apiHandler->pauseLiveTracking();
        apiHandler->fetchISSAtTimestamp(timestamp);
    }
}

void MainWindow::resumeLiveTracking() {
    if (apiHandler) {
        apiHandler->resumeLiveTracking();
    }
}


void MainWindow::switchLanguage(const QString &languageCode) {
    if (currentLanguage == languageCode)
        return;

    currentLanguage = languageCode;

    qApp->removeTranslator(&translator);

    QFile testPl(":/translations/app_pl.qm");
    QFile testEn(":/translations/app_en.qm");
    qDebug() << "PL file exists in qrc:" << testPl.exists();
    qDebug() << "EN file exists in qrc:" << testEn.exists();

    QString translationFile = QString(":/translations/app_%1.qm").arg(languageCode);
    if (translator.load(translationFile)) {
        qApp->installTranslator(&translator);
        issInfo->setText(tr("Waiting for ISS data..."));
        fetchDateButton->setText(tr("Show ISS at Selected Time"));
        resumeLiveButton->setText(tr("Resume Live Tracking"));
    } else {
        qDebug() << "Translation file not found:" << translationFile;
    }
}

MainWindow::~MainWindow(){
    delete ui;
}
