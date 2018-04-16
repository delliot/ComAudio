#include "StreamServe.h"

StreamServe::StreamServe(QTcpSocket* tcp, QString path)
	: QObject(nullptr)
	, tcp(tcp)
	, path(path)
{
}



StreamServe::~StreamServe()
{
}


void StreamServe::sendFile()
{
	outgoing = new QFile(path);
	if (outgoing->open(QIODevice::ReadOnly))
	{
		return;
	}

	while (!outgoing->atEnd())
	{
		QByteArray line = outgoing->readLine();
		tcp->write(line);
	}

	outgoing->close();
}
