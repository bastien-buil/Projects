#pragma once
#include <QString>
#include <QDate>


class File
{
public:
	File(QString path, QString hash, QDateTime date);
	QString getHash() const;
	QString getPath() const;
	QDateTime getDate() const;


	static int isNewer(File a1, File a2);

private:
	QString m_path;
	QString m_hash;
	QDateTime m_date;

};


bool operator==(const File& a, const File& b);
