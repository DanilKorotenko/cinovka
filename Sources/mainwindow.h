#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDomDocument>
#include <QMainWindow>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
class QComboBox;
class QGroupBox;
class QLabel;
class QListWidget;
class QSqlRelationalTableModel;
class QTableView;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(const QString &artistTable, const QString &albumTable, 
		QWidget *parent = 0);

private slots:
    void about();
    void addAlbum();
    void changeArtist(int row);
    void deleteAlbum();
    void showAlbumDetails(QModelIndex index);
    void showArtistProfile(QModelIndex index);
    void updateHeader(QModelIndex, int, int);

private:
    void adjustHeader();
    QGroupBox *createAlbumGroupBox();
    QGroupBox *createArtistGroupBox();
    QGroupBox *createDetailsGroupBox();
    void createMenuBar();
    void decreaseAlbumCount(QModelIndex artistIndex);
    void getTrackList(QDomNode album);
    QModelIndex indexOfArtist(const QString &artist);
    void removeAlbumFromDatabase(QModelIndex album);
    void removeAlbumFromFile(int id);
    void showImageLabel();

    QTableView *albumView;
    QComboBox *artistView;
    QListWidget *trackList;

    QLabel *iconLabel;
    QLabel *imageLabel;
    QLabel *profileLabel;
    QLabel *titleLabel;

    QSqlRelationalTableModel *model;
};

#endif

