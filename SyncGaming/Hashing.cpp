#include <QByteArray>
#include <QCryptographicHash>
#include <QFile>
#include <QTextCodec>




QByteArray fileChecksum(QFile& f, QCryptographicHash::Algorithm hashAlgorithm)
{
    if (f.open(QFile::ReadOnly)) {
        QCryptographicHash hash(hashAlgorithm);
        if (hash.addData(&f)) {
            return hash.result();
        }
    }
    return QByteArray();
}

QByteArray pathChecksum(const QString& fileName, QCryptographicHash::Algorithm hashAlgorithm)
{
    QFile f(fileName);
    return fileChecksum(f, hashAlgorithm);
}

QString stringPathChecksum(const QString& fileName, QCryptographicHash::Algorithm hashAlgorithm) {
    QByteArray array = pathChecksum(fileName, hashAlgorithm);
    return QTextCodec::codecForMib(1015)->toUnicode(array.toHex());
}

QString stringFileChecksum(QFile& file, QCryptographicHash::Algorithm hashAlgorithm) {
    QByteArray array = fileChecksum(file, hashAlgorithm);
    return QTextCodec::codecForMib(1015)->toUnicode(array.toHex());
}