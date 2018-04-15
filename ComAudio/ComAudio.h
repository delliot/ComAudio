#pragma once

#include <QtWidgets/QMainWindow>
#include <QDialog>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QUdpSocket>
#include <QLineEdit>
#include "TaskManager.h"
#include "ui_ComAudio.h"

class ComAudio : public QMainWindow
{
	Q_OBJECT

public:
	ComAudio(QWidget *parent = Q_NULLPTR);
	~ComAudio();

	public slots:
	void setDir();
	void selectDir();

public slots:
	void connectedToServerVoip(QUdpSocket * sock);
	void connectedToServerStream(QTcpSocket * sock);
	void connectedToServerFileTransfer(QTcpSocket * sock);

	void clientConnectedStream(QTcpSocket * );
	void clientConnectedFileTransfer(QTcpSocket * );
	void clientConnectedVoip(QUdpSocket * );

	void portValueChanged();
	void ipValueChanged();

private:
	int initUi(); // initializes UI components

	Ui::ComAudio *ui;
	QFileSystemModel *dirModel;
	QFileSystemModel *fileModel;
	QString pathLocal;
	QString pathFile;
	short port;
	QString ipAddr;

	TaskManager * taskManager;

	const QString pathLocalInitial = QDir::currentPath();
	const QStringList fileFilter = QStringList{ "*.aac" ,"*.wmv" ,"*.avi" ,"*.mpeg" ,"*.mov" ,"*.3gp" ,"*.flv" ,"*.mp3" };
};
