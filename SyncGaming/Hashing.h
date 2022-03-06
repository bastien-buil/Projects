#pragma once
#include <QByteArray>
#include <QCryptographicHash>

QByteArray pathChecksum(const QString& fileName, QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Md5);
QByteArray fileChecksum(QFile& file, QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Md5);
QString stringPathChecksum(const QString& fileName, QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Md5);
QString stringFileChecksum(QFile& file, QCryptographicHash::Algorithm hashAlgorithm = QCryptographicHash::Md5);