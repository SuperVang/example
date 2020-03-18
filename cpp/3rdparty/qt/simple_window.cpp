#include <qapplication.h>
#include <qmainwindow.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 让滑动更流顺
//    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QWidget *widget = new QMainWindow();

    widget->resize(400, 300);
    widget->show();

    return QApplication::exec();
}