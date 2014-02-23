#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QComboBox;
class QFile;
class QGroupBox;
class QLabel;
class QListWidget;
class QSqlRelationalTableModel;
class QTableView;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

private slots:
    void about();
    void addOrder();
    void changeArtist(int row);
    void deleteAlbum();
    void showAlbumDetails(QModelIndex index);
//    void showArtistProfile(QModelIndex index);

private:
    QGroupBox *createAlbumGroupBox();
    QGroupBox *createArtistGroupBox();
    QGroupBox *createDetailsGroupBox();
    void createMenuBar();
    void decreaseAlbumCount(QModelIndex artistIndex);
//    void getTrackList(QDomNode album);
    QModelIndex indexOfArtist(const QString &artist);
    void readAlbumData();
    void removeAlbumFromDatabase(QModelIndex album);
    void removeAlbumFromFile(int id);

    QTableView *_orderTable;
    QTableView *_jobTable;
	QLineEdit *_widthField;
	QLineEdit *_heightField;

    QDomDocument _albumData;
    QSqlRelationalTableModel *_model;
};

#endif

