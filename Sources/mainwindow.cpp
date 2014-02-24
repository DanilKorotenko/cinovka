#include "mainwindow.h"

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
//	_model = new QSqlRelationalTableModel(this);
//	_model->setTable("orders");
//	_model->setRelation(2, QSqlRelation("jobs", "id", "artist"));
//	_model->select();

	_orderTable = new QTableView;
//	_albumView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//	_albumView->setSortingEnabled(true);
//	_albumView->setSelectionBehavior(QAbstractItemView::SelectRows);
//	_albumView->setSelectionMode(QAbstractItemView::SingleSelection);
//	_albumView->setShowGrid(false);
//	_albumView->verticalHeader()->hide();
//	_albumView->setAlternatingRowColors(true);
//	_albumView->setModel(_model);

//	connect(_albumView, SIGNAL(clicked(QModelIndex)),
//		this, SLOT(showAlbumDetails(QModelIndex)));
//	connect(_albumView, SIGNAL(activated(QModelIndex)),
//		this, SLOT(showAlbumDetails(QModelIndex)));

	_widthField = new QLineEdit;
	_heightField = new QLineEdit;

	_jobTable = new QTableView;

//	QGridLayout *layout = new QGridLayout;
//	layout->addWidget(_profileLabel, 0, 0);
//	layout->addWidget(_iconLabel, 0, 1);
//	layout->addWidget(_titleLabel, 1, 0, 1, 2);
//	layout->addWidget(_trackList, 2, 0, 1, 2);
//	layout->setRowStretch(2, 1);
//	box->setLayout(layout);

//	connect(_model, SIGNAL(rowsInserted(QModelIndex,int,int)),
//			this, SLOT(updateHeader(QModelIndex,int,int)));
//	connect(_model, SIGNAL(rowsRemoved(QModelIndex,int,int)),
//			this, SLOT(updateHeader(QModelIndex,int,int)));

	QPushButton *addOrderButton = new QPushButton(tr("Add Order"));
	connect(addOrderButton, SIGNAL(clicked()), this, SLOT(addOrder()));

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(_orderTable, 0, 0, 1, 2);
	layout->addWidget(_jobTable, 0, 2, 1, 2);
	layout->addWidget(new QLabel(tr("Width:")), 1, 0);
	layout->addWidget(_widthField, 1, 1);
	layout->addWidget(new QLabel(tr("Height:")), 2, 0);
	layout->addWidget(_heightField, 2, 1);
	layout->addWidget(addOrderButton, 3, 0);
//	layout->setColumnStretch(1, 1);
//	layout->setColumnMinimumWidth(0, 500);

	QWidget *widget = new QWidget;
	widget->setLayout(layout);
	setCentralWidget(widget);
	createMenuBar();

	resize(850, 400);
	setWindowTitle(tr("Cinovka"));
}

void MainWindow::changeArtist(int row)
{
	if (row > 0)
	{
		QModelIndex index = _model->relationModel(2)->index(row, 1);
		_model->setFilter("artist = '" + index.data().toString() + '\'') ;
//		showArtistProfile(index);
	}
	else if (row == 0)
	{
		_model->setFilter(QString());
	}
	else
	{
		return;
	}
}

//void MainWindow::showArtistProfile(QModelIndex index)
//{
/*	QSqlRecord record = _model->relationModel(2)->record(index.row());

	QString name = record.value("artist").toString();
	QString count = record.value("albumcount").toString();
	_profileLabel->setText(tr("Artist : %1 \n" \
		"Number of Albums: %2").arg(name).arg(count));

	_profileLabel->show();

	_trackList->hide();*/
//}

void MainWindow::showAlbumDetails(QModelIndex index)
{
	QSqlRecord record = _model->record(index.row());

	QString artist = record.value("artist").toString();
	QString title = record.value("title").toString();
	QString year = record.value("year").toString();
	QString albumId = record.value("albumid").toString();

//	showArtistProfile(indexOfArtist(artist));

	QDomNodeList albums = _albumData.elementsByTagName("album");
	for (int i = 0; i < albums.count(); i++)
	{
		QDomNode album = albums.item(i);
		if (album.toElement().attribute("id") == albumId)
		{
//			getTrackList(album.toElement());
			break;
		}
	}

//	if (!_trackList->count() == 0)
//	{
//		_trackList->show();
//	}
}

//void MainWindow::getTrackList(QDomNode album)
//{
/*	_trackList->clear();

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
*/
//}

void MainWindow::addOrder()
{
/*
	Dialog *dialog = new Dialog(_model, _albumData, this);
	int accepted = dialog->exec();

	if (accepted == 1)
	{
		int lastRow = _model->rowCount() - 1;
		_albumView->selectRow(lastRow);
		_albumView->scrollToBottom();
		showAlbumDetails(_model->index(lastRow, 0));
	}
*/
}

/*
#ifndef DIALOG_H
#define DIALOG_H

#include <QtWidgets>
#include <QtSql>
#include <QtXml>

class Dialog : public QDialog
{
	Q_OBJECT

public:
	Dialog(QSqlRelationalTableModel *albums, QDomDocument details,
		QWidget *parent = 0);

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
	QDomDocument albumDetails;

	QLineEdit *artistEditor;
	QLineEdit *titleEditor;
	QSpinBox *yearEditor;
	QLineEdit *tracksEditor;
};

#endif

#include "dialog.h"

int uniqueAlbumId;
int uniqueArtistId;

Dialog::Dialog(QSqlRelationalTableModel *albums, QDomDocument details,
	QWidget *parent) : QDialog(parent)
{
	model = albums;
	albumDetails = details;

	QGroupBox *inputWidgetBox = createInputWidgets();
	QDialogButtonBox *buttonBox = createButtons();

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(inputWidgetBox);
	layout->addWidget(buttonBox);
	setLayout(layout);

	setWindowTitle(tr("Add Album"));
}

void Dialog::submit()
{
	QString artist = artistEditor->text();
	QString title = titleEditor->text();

	if (artist.isEmpty() || title.isEmpty()) {
		QString message(tr("Please provide both the name of the artist "
						   "and the title of the album."));
		QMessageBox::information(this, tr("Add Album"), message);
	} else {
		int artistId = findArtistId(artist);
		int albumId = addNewAlbum(title, artistId);

		QStringList tracks;
		tracks = tracksEditor->text().split(',', QString::SkipEmptyParts);
		addTracks(albumId, tracks);

		increaseAlbumCount(indexOfArtist(artist));
		accept();
	}
}

int Dialog::findArtistId(const QString &artist)
{
	QSqlTableModel *artistModel = model->relationModel(2);
	int row = 0;

	while (row < artistModel->rowCount()) {
		QSqlRecord record = artistModel->record(row);
		if (record.value("artist") == artist)
			return record.value("id").toInt();
		else
			row++;
	}
	return addNewArtist(artist);
}


int Dialog::addNewArtist(const QString &name)
{
	QSqlTableModel *artistModel = model->relationModel(2);
	QSqlRecord record;

	int id = generateArtistId();

	QSqlField f1("id", QVariant::Int);
	QSqlField f2("artist", QVariant::String);
	QSqlField f3("albumcount", QVariant::Int);

	f1.setValue(QVariant(id));
	f2.setValue(QVariant(name));
	f3.setValue(QVariant(0));
	record.append(f1);
	record.append(f2);
	record.append(f3);

	artistModel->insertRecord(-1, record);
	return id;
}

int Dialog::addNewAlbum(const QString &title, int artistId)
{
	int id = generateAlbumId();
	QSqlRecord record;

	QSqlField f1("albumid", QVariant::Int);
	QSqlField f2("title", QVariant::String);
	QSqlField f3("artistid", QVariant::Int);
	QSqlField f4("year", QVariant::Int);

	f1.setValue(QVariant(id));
	f2.setValue(QVariant(title));
	f3.setValue(QVariant(artistId));
	f4.setValue(QVariant(yearEditor->value()));
	record.append(f1);
	record.append(f2);
	record.append(f3);
	record.append(f4);

	model->insertRecord(-1, record);
	return id;
}

void Dialog::addTracks(int albumId, QStringList tracks)
{
	QDomElement albumNode = albumDetails.createElement("album");
	albumNode.setAttribute("id", albumId);

	for (int i = 0; i < tracks.count(); i++) {
		QString trackNumber = QString::number(i);
		if (i < 10)
			trackNumber.prepend("0");

		QDomText textNode = albumDetails.createTextNode(tracks.at(i));

		QDomElement trackNode = albumDetails.createElement("track");
		trackNode.setAttribute("number", trackNumber);
		trackNode.appendChild(textNode);

		albumNode.appendChild(trackNode);
	}

	QDomNodeList archive = albumDetails.elementsByTagName("archive");
	archive.item(0).appendChild(albumNode);
}

void Dialog::increaseAlbumCount(QModelIndex artistIndex)
{
	QSqlTableModel *artistModel = model->relationModel(2);

	QModelIndex albumCountIndex;
	albumCountIndex = artistIndex.sibling(artistIndex.row(), 2);

	int albumCount = albumCountIndex.data().toInt();
	artistModel->setData(albumCountIndex, QVariant(albumCount + 1));
}

void Dialog::revert()
{
	artistEditor->clear();
	titleEditor->clear();
	yearEditor->setValue(QDate::currentDate().year());
	tracksEditor->clear();
}

QGroupBox *Dialog::createInputWidgets()
{
	QGroupBox *box = new QGroupBox(tr("Add Album"));

	QLabel *artistLabel = new QLabel(tr("Artist:"));
	QLabel *titleLabel = new QLabel(tr("Title:"));
	QLabel *yearLabel = new QLabel(tr("Year:"));
	QLabel *tracksLabel = new QLabel(tr("Tracks (separated by comma):"));

	artistEditor = new QLineEdit;
	titleEditor = new QLineEdit;

	yearEditor = new QSpinBox;
	yearEditor->setMinimum(1900);
	yearEditor->setMaximum(QDate::currentDate().year());
	yearEditor->setValue(yearEditor->maximum());
	yearEditor->setReadOnly(false);

	tracksEditor = new QLineEdit;

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(artistLabel, 0, 0);
	layout->addWidget(artistEditor, 0, 1);
	layout->addWidget(titleLabel, 1, 0);
	layout->addWidget(titleEditor, 1, 1);
	layout->addWidget(yearLabel, 2, 0);
	layout->addWidget(yearEditor, 2, 1);
	layout->addWidget(tracksLabel, 3, 0, 1, 2);
	layout->addWidget(tracksEditor, 4, 0, 1, 2);
	box->setLayout(layout);

	return box;
}

QDialogButtonBox *Dialog::createButtons()
{
	QPushButton *closeButton = new QPushButton(tr("&Close"));
	QPushButton *revertButton = new QPushButton(tr("&Revert"));
	QPushButton *submitButton = new QPushButton(tr("&Submit"));

	closeButton->setDefault(true);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(revertButton, SIGNAL(clicked()), this, SLOT(revert()));
	connect(submitButton, SIGNAL(clicked()), this, SLOT(submit()));

	QDialogButtonBox *buttonBox = new QDialogButtonBox;
	buttonBox->addButton(submitButton, QDialogButtonBox::ResetRole);
	buttonBox->addButton(revertButton, QDialogButtonBox::ResetRole);
	buttonBox->addButton(closeButton, QDialogButtonBox::RejectRole);

	return buttonBox;
}

QModelIndex Dialog::indexOfArtist(const QString &artist)
{
	QSqlTableModel *artistModel = model->relationModel(2);

	for (int i = 0; i < artistModel->rowCount(); i++) {
		QSqlRecord record =  artistModel->record(i);
		if (record.value("artist") == artist)
			return artistModel->index(i, 1);
	}

	return QModelIndex();
}

int Dialog::generateArtistId()
{
	uniqueArtistId += 1;
	return uniqueArtistId;
}

int Dialog::generateAlbumId()
{
	uniqueAlbumId += 1;
	return uniqueAlbumId;
}
*/

void MainWindow::deleteAlbum()
{
/*
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
		}
	}
	else
	{
		QMessageBox::information(this, tr("Delete Album"),
			tr("Select the album you want to delete."));
	}
*/
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
	}
	else
	{
		artists->setData(albumCountIndex, QVariant(albumCount - 1));
	}
}

void MainWindow::createMenuBar()
{
	QAction *quitAction = new QAction(tr("&Quit"), this);
	QAction *aboutAction = new QAction(tr("&About"), this);
	QAction *aboutQtAction = new QAction(tr("About &Qt"), this);

	quitAction->setShortcuts(QKeySequence::Quit);

	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(quitAction);

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);

	connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));
	connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(about()));
	connect(aboutQtAction, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
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

void MainWindow::about()
{
	QMessageBox::about(this, tr("Application for cinovka calculation"),
		tr("<p>The <b>Cinovka</b> help to calculate cinovka.</p>"));
}
