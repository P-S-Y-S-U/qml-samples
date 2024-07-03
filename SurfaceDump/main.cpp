#include <QMetaType>
#include <QTextStream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQuickWindow>
#include <QQmlEngine>
#include <QImage>
#include <QFile>
#include <QQmlContext>

#include "TexReader.h"
#include "SurfaceDumper.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QQuickView view;
    view.setMinimumSize( QSize{ 1920, 1080 } );

    SurfaceDumper dumper;

    QSurfaceFormat format;
    format.setRedBufferSize(8);
    format.setGreenBufferSize(8);
    format.setBlueBufferSize(8);
    format.setAlphaBufferSize(8);
    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGLES);
    format.setMajorVersion(2);
    format.setMinorVersion(0);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);

    view.setFormat(format);

    view.engine()->rootContext()->setContextProperty("surfacedumper", &dumper);

    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);
    view.connect(&view, &QQuickView::afterRendering, &dumper, &SurfaceDumper::ReadAndDump);

    view.setSource(url);
    if (view.status() == QQuickView::Error)
        return -1;
    view.setResizeMode(QQuickView::SizeViewToRootObject);
    view.show();

    return app.exec();
}
