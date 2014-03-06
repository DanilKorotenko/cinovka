#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QSqlRelationalTableModel *albums, QWidget *parent = 0);

private slots:
    void revert();
    void submit();

private:
    int addNewAlbum(const QString &title, int artistId);
    int addNewArtist(const QString &name);
    void addTracks(int albumId, QStringList tracks);
    QDialogButtonBox *createButtons();
    QGroupBox *createInputWidgets();
    int findArtistId(const QString &artist);
    int generateAlbumId();
    int generateArtistId();
    void increaseAlbumCount(QModelIndex artistIndex);
    QModelIndex indexOfArtist(const QString &artist);

    QSqlRelationalTableModel *model;

    QLineEdit *artistEditor;
    QLineEdit *titleEditor;
    QSpinBox *yearEditor;
    QLineEdit *tracksEditor;
};

#endif

