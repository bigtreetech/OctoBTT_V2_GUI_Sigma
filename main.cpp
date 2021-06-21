#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>
#include <QQmlContext>
#include "./headers/userloginmodule.h"
#include "./headers/websocketmodule.h"
#include "./headers/functionmodule.h"
#include "./headers/cameramodule.h"
#include "./headers/viewprovider.h"

AccessControl *globalAccess = new AccessControl();
WebSocketModule *myWebsoket = new WebSocketModule;
FunctionModule *functionModule = new FunctionModule();
ViewProvider *viewProvider = new ViewProvider();
//CameraModule *cameraModule = new CameraModule(globalAccess->MainUrl());
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QFont font;
    font.setPointSize(14);
    font.setFamily("qrc:/font/SimSun.ttf");

    //注册模块
    qmlRegisterType<UserLoginModule>("UserLoginModule",1,0,"UserLoginModule");
    qmlRegisterType<CameraModule>("MyCameraModule",1,0,"MyCameraModule");
//    qmlRegisterType<UserLoginModule>("UserInfo",1,0,"UserInfo");

    QGuiApplication app(argc, argv);

    app.setFont(font);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.rootContext()->setContextProperty("globalAccess",globalAccess);
    engine.rootContext()->setContextProperty("myWebsoket",myWebsoket);
    engine.rootContext()->setContextProperty("functionModule",functionModule);
    engine.addImageProvider(QLatin1String("camera"), viewProvider);

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
