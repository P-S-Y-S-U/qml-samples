#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlEngine>
#include <QDebug>

#define SOME_TEST_DEFINES

static QHash<void*, QRect> TestHash;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    QQuickView view;
    view.setMinimumSize( QSize{ 1920, 1080 } );
#if 0
    view.setTextRenderType(QQuickWindow::TextRenderType::QtTextRendering);
#else
    view.setTextRenderType(QQuickWindow::TextRenderType::NativeTextRendering);
#endif

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

    view.connect(view.engine(), &QQmlEngine::quit, &app, &QCoreApplication::quit);


    qWarning() << "Test Log";
    qWarning() << "Hash Empty : " << TestHash.empty();
    qWarning() << "Hash: " << TestHash;

    void* ptrFormat = &format;
    QRect sample{};
    TestHash[ptrFormat] = sample;

    qWarning() << "Hash: " << TestHash;
    qWarning() << "Hash Empty : " << TestHash.empty();

    view.setSource(url);
    if (view.status() == QQuickView::Error)
        return -1;
    view.setResizeMode(QQuickView::SizeViewToRootObject);
    view.show();

    return app.exec();
}
