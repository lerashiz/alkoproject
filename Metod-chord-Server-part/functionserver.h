#ifndef FUNCTIONSERVER_H
#define FUNCTIONSERVER_H
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QLatin1Char>
#include <QImage>

QByteArray parsing(QByteArray data);

QByteArray reg(QString login, QString password, QString email);

QByteArray auth(QString login, QString password);

QImage stego(QString image_path, QString text);

QByteArray rsa(const QString &message, int e, int n);    // шифрование
int gcd(int a, int b);
int modInverse(int e, int phi);
int powerMod(int base, int exponent, int mod);
void generateKeys(int &n, int &e, int &d);
QByteArray get_users();


#endif // FUNCTIONSERVER_H
