#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <camerafilter/camerafilter.h>  //和qzxing有冲突
#include <mainwork.h>
#include <sqlite/easytablemodel.h>
#include <QQmlContext>
//#include "QZXing.h"
#include <QFont>
#include <QFontDatabase>

QString loadFontFromFile(QString path)
{
    bool fileExists = QFile::exists(path);     //qrc资源好像不能包含字体
    qDebug()<<"字体文件存在"<<fileExists;

    static QString font;
    int loadedFontID = QFontDatabase::addApplicationFont(path);
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if(!loadedFontFamilies.empty()) font = loadedFontFamilies.at(0);

    return font;
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QString fontName = loadFontFromFile("assets:/jdhtj.ttf");     //NotoSans-SemiBold.ttf   //wrzht.ttf
    if(fontName != "")
    {
        QFont font(fontName);
        qDebug()<<fontName<<"字体名称";
        app.setFont(font);
    }

    QQmlApplicationEngine engine;
    //QZXing::registerQMLTypes();
    QQmlContext *context = engine.rootContext();

    mainwork mw;
    qmlRegisterType<EasyTableModel>("EasyModel", 1, 0, "EasyTableModel");
    context->setContextProperty("mw", &mw);

    CameraFilter Camerafilter;
    Camerafilter.QrImgWidth = 640;
    context->setContextProperty("Camerafilter", &Camerafilter);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
