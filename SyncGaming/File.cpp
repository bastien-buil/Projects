#include "File.h"
#include <QMessageBox>


File::File(QString path, QString hash, QDateTime date) : m_path(path), m_hash(hash), m_date(date)
{}

QString File::getHash() const {
	return m_hash;
}
QString File::getPath() const {
	return m_path;
}

QDateTime File::getDate() const {
	return m_date;
}

int File::isNewer(File a1, File a2) {//1 newer, 0 egal, older -1
	if (a1.getHash() != a2.getHash()) 
	{
		return a1.getDate() > a2.getDate() ? 1 : -1;
	}
	else 
	{
		return 0;
	}
}

bool operator==(const File& a, const File& b) {
	return a.getPath() == b.getPath();
}