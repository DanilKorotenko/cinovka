#include <QtWidgets>

#include "database.h"
#include "mainwindow.h"

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if (!createConnection())
	{
		return 1;
	}

	MainWindow window;
	window.show();

	return app.exec();
}

