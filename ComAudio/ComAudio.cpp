#include "ComAudio.h"

ComAudio::ComAudio(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ComAudio)
{
	int resultInitUi;

	if ((resultInitUi = initUi()) != 0)
	{
		// TODO: error msg
		exit(EXIT_FAILURE);
	}
}

ComAudio::~ComAudio()
{
	delete ui;
}

int ComAudio::initUi()
{
	ui->setupUi(this);

	// file browser
	pathLocal = pathLocalInitial;
	fileModel = new QFileSystemModel(this);
	fileModel->setFilter(QDir::NoDotAndDotDot | QDir::Files);
	fileModel->setNameFilters(fileFilter);
	fileModel->setNameFilterDisables(false);
	ui->listView_dir_list->setModel(fileModel);
	setDir();

	connect(ui->lineEditIp, &QLineEdit::textChanged, this, &ComAudio::ipValueChanged);
	connect(ui->lineEditPort, &QLineEdit::textChanged, this, &ComAudio::portValueChanged);


	taskManager = new TaskManager(this, DEFAULT_PORT);
	connect(taskManager, &TaskManager::clientConnectedVoip, this, &ComAudio::clientConnectedVoip);
	connect(taskManager, &TaskManager::clientConnectedFileTransfer, this, &ComAudio::clientConnectedFileTransfer);
	connect(taskManager, &TaskManager::clientConnectedStream, this, &ComAudio::clientConnectedStream);

	connect(taskManager, &TaskManager::connectedToServerFileTransfer, this, &ComAudio::connectedToServerFileTransfer);
	connect(taskManager, &TaskManager::connectedToServerStream, this, &ComAudio::connectedToServerStream);
	connect(taskManager, &TaskManager::connectedToServerVoip, this, &ComAudio::connectedToServerVoip);


	connect(ui->pushButton_tasks_audioStream, &QPushButton::pressed, this, &ComAudio::startStream);
	connect(ui->pushButton_tasks_audioChat, &QPushButton::pressed, this, &ComAudio::startVoip);
	connect(ui->pushButton_tasks_multicast, &QPushButton::pressed, this, &ComAudio::startMulticastTx);
	// temporarily using this to test, we'll have to add another button to handle MulticastRx
	connect(ui->pushButton_tasks_fileTransfer, &QPushButton::pressed, this, &ComAudio::startMulticastRx);
	connect(ui->pushButton_dir_browse, &QPushButton::pressed, this, &ComAudio::selectDir);

	return 0;
}



void ComAudio::connectedToServerVoip(QUdpSocket * udp, QTcpSocket * tcp)
{
	clientVoip = new UDPTask(nullptr, udp, VOICE_STREAM, tcp);
	clientVoip->startVOIP(mAudioOutput, mAudioInput, mFormat);
}

void ComAudio::connectedToServerStream(QTcpSocket * sock)
{
}

void ComAudio::connectedToServerFileTransfer(QTcpSocket * sock)
{
}

void ComAudio::clientConnectedStream(QTcpSocket * sock)
{
}

void ComAudio::clientConnectedFileTransfer(QTcpSocket * sock)
{
}

void ComAudio::clientConnectedVoip(QUdpSocket * udp, QTcpSocket * tcp)
{
	serverVoip = new UDPTask(nullptr, udp, VOICE_STREAM, tcp);
	serverVoip->startVOIP(mAudioOutput, mAudioInput, mFormat);
}

void ComAudio::portValueChanged()
{
	port = ui->lineEditPort->text().toInt();
}

void ComAudio::ipValueChanged()
{
	ipAddr = ui->lineEditIp->text();
}

void ComAudio::startStream()
{
	if (taskManager->ConnectTo(ipAddr, port, TaskType::VOICE_STREAM))
	{
		//grey out other options
	}
}

void ComAudio::startVoip()
{
	if (taskManager->ConnectTo(ipAddr, port, TaskType::VOICE_STREAM))
	{
		//grey out other options
	}
}

void ComAudio::startFileTransfer()
{
	if (taskManager->ConnectTo(ipAddr, port, TaskType::FILE_TRANSFER))
	{
		//grey out other options
	}
}

void ComAudio::startMulticastTx()
{
	serverVoip = new UDPTask(this);
	serverVoip->startMulticastTx();
}

void ComAudio::startMulticastRx()
{
	serverVoip = new UDPTask(this);
	serverVoip->startMulticastRx();
}


void ComAudio::setDir()
{
	// TODO: fix bug -- showing directories on the list view.
	ui->listView_dir_list->setRootIndex(fileModel->setRootPath(pathLocal));
	ui->lineEdit_dir_path->setText(pathLocal);
}

void ComAudio::selectDir()
{
	QString pathTemp = QFileDialog::getExistingDirectory(this, tr("Open Directory"), pathLocal, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	if (!pathTemp.isEmpty())
	{
		pathLocal = pathTemp;
		setDir();
	}
}