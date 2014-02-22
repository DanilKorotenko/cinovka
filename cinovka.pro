HEADERS   = Sources/database.h \
            Sources/dialog.h \
            Sources/mainwindow.h

SOURCES   = Sources/dialog.cpp \
            Sources/main.cpp \
            Sources/mainwindow.cpp

MOC_DIR = moc
OBJECTS_DIR = obj

QT += sql xml widgets

EXAMPLE_FILES = albumdetails.xml

