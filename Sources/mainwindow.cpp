#include "mainwindow.h"
#include "dialog.h"

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    _model = new QSqlRelationalTableModel(this);
    _model->setTable("orders");
    _model->setRelation(2, QSqlRelation("jobs", "id", "artist"));
    _model->select();

    QGroupBox *artists = createArtistGroupBox();
    QGroupBox *albums = createAlbumGroupBox();
    QGroupBox *details = createDetailsGroupBox();

    _artistView->setCurrentIndex(0);

    connect(_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
            this, SLOT(updateHeader(QModelIndex,int,int)));
    connect(_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
            this, SLOT(updateHeader(QModelIndex,int,int)));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(artists, 0, 0);
    layout->addWidget(albums, 1, 0);
    layout->addWidget(details, 0, 1, 2, 1);
    layout->setColumnStretch(1, 1);
    layout->setColumnMinimumWidth(0, 500);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    setCentralWidget(widget);
    createMenuBar();

    showImageLabel();
    resize(850, 400);
    setWindowTitle(tr("Cinovka"));
}

void MainWindow::changeArtist(int row)
{
    if (row > 0) 
	{
        QModelIndex index = _model->relationModel(2)->index(row, 1);
        _model->setFilter("artist = '" + index.data().toString() + '\'') ;
        showArtistProfile(index);
    } 
	else if (row == 0) 
	{
        _model->setFilter(QString());
        showImageLabel();
    } 
	else 
	{
        return;
    }
}

void MainWindow::showArtistProfile(QModelIndex index)
{
    QSqlRecord record = _model->relationModel(2)->record(index.row());

    QString name = record.value("artist").toString();
    QString count = record.value("albumcount").toString();
    _profileLabel->setText(tr("Artist : %1 \n" \
		"Number of Albums: %2").arg(name).arg(count));

    _profileLabel->show();
    _iconLabel->show();

    _titleLabel->hide();
    _trackList->hide();
    _imageLabel->hide();
}

void MainWindow::showAlbumDetails(QModelIndex index)
{
    QSqlRecord record = _model->record(index.row());

    QString artist = record.value("artist").toString();
    QString title = record.value("title").toString();
    QString year = record.value("year").toString();
    QString albumId = record.value("albumid").toString();

    showArtistProfile(indexOfArtist(artist));
    _titleLabel->setText(tr("Title: %1 (%2)").arg(title).arg(year));
    _titleLabel->show();

    QDomNodeList albums = _albumData.elementsByTagName("album");
    for (int i = 0; i < albums.count(); i++) 
	{
        QDomNode album = albums.item(i);
        if (album.toElement().attribute("id") == albumId) 
		{
            getTrackList(album.toElement());
            break;
        }
    }

    if (!_trackList->count() == 0)
	{
        _trackList->show();
	}
}

void MainWindow::getTrackList(QDomNode album)
{
    _trackList->clear();

    QDomNodeList tracks = album.childNodes();
    QDomNode track;
    QString trackNumber;

    for (int j = 0; j < tracks.count(); j++) 
	{
        track = tracks.item(j);
        trackNumber = track.toElement().attribute("number");

        QListWidgetItem *item = new QListWidgetItem(_trackList);
        item->setText(trackNumber + ": " + track.toElement().text());
    }
}

void MainWindow::addAlbum()
{
    Dialog *dialog = new Dialog(_model, _albumData, this);
    int accepted = dialog->exec();

    if (accepted == 1) 
	{
        int lastRow = _model->rowCount() - 1;
        _albumView->selectRow(lastRow);
        _albumView->scrollToBottom();
        showAlbumDetails(_model->index(lastRow, 0));
    }
}

void MainWindow::deleteAlbum()
{
    QModelIndexList selection = _albumView->selectionModel()->selectedRows(0);

    if (!selection.empty()) 
	{
        QModelIndex idIndex = selection.at(0);
        int id = idIndex.data().toInt();
        QString title = idIndex.sibling(idIndex.row(), 1).data().toString();
        QString artist = idIndex.sibling(idIndex.row(), 2).data().toString();

        QMessageBox::StandardButton button;
        button = QMessageBox::question(this, tr("Delete Album"),
			tr("Are you sure you want to delete '%1' by '%2'?")
			.arg(title, artist), QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) 
		{
            removeAlbumFromFile(id);
            removeAlbumFromDatabase(idIndex);
            decreaseAlbumCount(indexOfArtist(artist));

            showImageLabel();
        }
    }
	else 
	{
        QMessageBox::information(this, tr("Delete Album"),
			tr("Select the album you want to delete."));
    }
}

void MainWindow::removeAlbumFromFile(int id)
{
    QDomNodeList albums = _albumData.elementsByTagName("album");

    for (int i = 0; i < albums.count(); i++) 
	{
        QDomNode node = albums.item(i);
        if (node.toElement().attribute("id").toInt() == id) 
		{
            _albumData.elementsByTagName("archive").item(0).removeChild(node);
            break;
        }
    }
/*
    The following code is commented out since the example uses an in
    memory database, i.e., altering the XML file will bring the data
    out of sync.

    if (!file->open(QIODevice::WriteOnly)) {
        return;
    } else {
        QTextStream stream(file);
        albumData.elementsByTagName("archive").item(0).save(stream, 4);
        file->close();
    }
*/
}

void MainWindow::removeAlbumFromDatabase(QModelIndex index)
{
    _model->removeRow(index.row());
}

void MainWindow::decreaseAlbumCount(QModelIndex artistIndex)
{
    int row = artistIndex.row();
    QModelIndex albumCountIndex = artistIndex.sibling(row, 2);
    int albumCount = albumCountIndex.data().toInt();

    QSqlTableModel *artists = _model->relationModel(2);

    if (albumCount == 1) 
	{
        artists->removeRow(row);
        showImageLabel();
    }
	else 
	{
        artists->setData(albumCountIndex, QVariant(albumCount - 1));
    }
}

QGroupBox* MainWindow::createArtistGroupBox()
{
    _artistView = new QComboBox;
    _artistView->setModel(_model->relationModel(2));
    _artistView->setModelColumn(1);

    connect(_artistView, SIGNAL(currentIndexChanged(int)),
		this, SLOT(changeArtist(int)));

    QGroupBox *box = new QGroupBox(tr("Artist"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(_artistView, 0, 0);
    box->setLayout(layout);

    return box;
}

QGroupBox* MainWindow::createAlbumGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("Album"));

    _albumView = new QTableView;
    _albumView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _albumView->setSortingEnabled(true);
    _albumView->setSelectionBehavior(QAbstractItemView::SelectRows);
    _albumView->setSelectionMode(QAbstractItemView::SingleSelection);
    _albumView->setShowGrid(false);
    _albumView->verticalHeader()->hide();
    _albumView->setAlternatingRowColors(true);
    _albumView->setModel(_model);
    adjustHeader();

    QLocale locale = _albumView->locale();
    locale.setNumberOptions(QLocale::OmitGroupSeparator);
    _albumView->setLocale(locale);

    connect(_albumView, SIGNAL(clicked(QModelIndex)),
		this, SLOT(showAlbumDetails(QModelIndex)));
    connect(_albumView, SIGNAL(activated(QModelIndex)),
		this, SLOT(showAlbumDetails(QModelIndex)));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(_albumView, 0, 0);
    box->setLayout(layout);

    return box;
}

QGroupBox* MainWindow::createDetailsGroupBox()
{
    QGroupBox *box = new QGroupBox(tr("Details"));

    _profileLabel = new QLabel;
    _profileLabel->setWordWrap(true);
    _profileLabel->setAlignment(Qt::AlignBottom);

    _titleLabel = new QLabel;
    _titleLabel->setWordWrap(true);
    _titleLabel->setAlignment(Qt::AlignBottom);

    _iconLabel = new QLabel();
    _iconLabel->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    _iconLabel->setPixmap(QPixmap(":/images/icon.png"));

    _imageLabel = new QLabel;
    _imageLabel->setWordWrap(true);
    _imageLabel->setAlignment(Qt::AlignCenter);
    _imageLabel->setPixmap(QPixmap(":/images/image.png"));

    _trackList = new QListWidget;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(_imageLabel, 0, 0, 3, 2);
    layout->addWidget(_profileLabel, 0, 0);
    layout->addWidget(_iconLabel, 0, 1);
    layout->addWidget(_titleLabel, 1, 0, 1, 2);
    layout->addWidget(_trackList, 2, 0, 1, 2);
    layout->setRowStretch(2, 1);
    box->setLayout(layout);

    return box;
}

void MainWindow::createMenuBar()
{
    QAction *addAction = new QAction(tr("&Add album..."), this);
    QAction *deleteAction = new QAction(tr("&Delete album..."), this);
    QAction *quitAction = new QAction(tr("&Quit"), this);
    QAction *aboutAction = new QAction(tr("&About"), this);
    QAction *aboutQtAction = new QAction(tr("About &Qt"), this);

    addAction->setShortcut(tr("Ctrl+A"));
    deleteAction->setShortcut(tr("Ctrl+D"));
    quitAction->setShortcuts(QKeySequence::Quit);

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(addAction);
    fileMenu->addAction(deleteAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(addAction, SIGNAL(triggered(bool)), this, SLOT(addAlbum()));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteAlbum()));
    connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
    connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
    connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
}

void MainWindow::showImageLabel()
{
    _profileLabel->hide();
    _titleLabel->hide();
    _iconLabel->hide();
    _trackList->hide();

    _imageLabel->show();
}

QModelIndex MainWindow::indexOfArtist(const QString &artist)
{
    QSqlTableModel *artistModel = _model->relationModel(2);

    for (int i = 0; i < artistModel->rowCount(); i++) 
	{
        QSqlRecord record =  artistModel->record(i);
        if (record.value("artist") == artist)
		{
            return artistModel->index(i, 1);
		}
    }

    return QModelIndex();
}

void MainWindow::updateHeader(QModelIndex, int, int)
{
    adjustHeader();
}

void MainWindow::adjustHeader()
{
//    _albumView->hideColumn(0);
//    _albumView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
//    _albumView->resizeColumnToContents(2);
//   _albumView->resizeColumnToContents(3);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Application for cinovka calculation"),
		tr("<p>The <b>Cinovka</b> help to calculate cinovka.</p>"));
}

