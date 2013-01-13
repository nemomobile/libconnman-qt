#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QQuickView *view = new QQuickView;
    view->setSource(QUrl::fromLocalFile("main.qml"));
    view->setGeometry(0, 0, 800, 480);
    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->show();

    return a.exec();
}
