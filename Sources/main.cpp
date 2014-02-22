#include <QtWidgets>

#include "database.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!createConnection())
        return 1;

    QFile *albumDetails = new QFile("albumdetails.xml");
    MainWindow window("artists", "albums", albumDetails);
    window.show();
    return app.exec();
}

