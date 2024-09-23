#include "functionserver.h"
#include "QDebug"
#include "QFile"
#include <QBuffer>
#include <QSqlTableModel>
#include "QFileInfo"
#include "database.h"
// #include "qbuffer.h"
#include "sha-384.h"

QByteArray parsing(QByteArray data)//Функция парсинга, получает строку и манипулирует в зависимости от введенной команды.
{
    QString data_from_client = QString::fromUtf8(data).trimmed();
    QStringList clue_words = data_from_client.split(QLatin1Char('&'));
    QString action_type = clue_words.front();
    clue_words.pop_front();
    QByteArray res;
    if (action_type == "auth"){
        res = auth(clue_words.at(0), clue_words.at(1));
    }
    else if (action_type == "reg"){
        res = reg(clue_words.at(0), clue_words.at(1), clue_words.at(2));
    }
    else if (action_type == "users"){
        QByteArray users;
        QByteArray emails;
        res = get_users();
        }
    else if (action_type == "sql"){
        QString query = clue_words.at(0);
        Database::getInstance()->send_query(query, false);
        res = "Exec";
    }
    else if (action_type == "delete"){
        QString username = clue_words.at(0);
        QString query = QString("DELETE FROM Users WHERE username = '%1';").arg(username);
        Database::getInstance()->send_query(query, false);
        res = "Deleted";
        }
    else if (action_type == "stego"){

        QFile image("./image.png");
        if(!image.open(QIODevice::WriteOnly)){
            qDebug() << "Ошибка открытия";
        }
        QByteArray imageData = data.mid(clue_words[0].length()+clue_words[0].length()+2);
        image.write(imageData);
        image.close();
        QImage newImage = stego("./image.png", clue_words.at(0));

        QByteArray new_imageData;
        QBuffer buffer(&new_imageData);
        buffer.open(QIODevice::WriteOnly);
        newImage.save(&buffer, "PNG");

        res = new_imageData;
    }
    else if (action_type == "rsa"){
        int n,e,d;
        generateKeys(n,e,d);
        res = rsa(clue_words.at(0), e, n);
    }

    else{
        res = "There is no such a function";
    }
    res += "\r\n";
    return res;

}

QByteArray rsa(const QString &message, int e, int n)
{
    QString encryptedText;
    for (QChar c : message) {
        int m = c.unicode();
        int encryptedChar = powerMod(m, e, n);
        encryptedText.append(QString::number(encryptedChar) + " ");
    }
    return encryptedText.toUtf8();
}

int modInverse(int e, int phi) {
    e = e % phi;
    for (int x = 1; x < phi; x++) {
        if ((e * x) % phi == 0) {
            return x;
        }
    }
    return 1;
}

int powerMod(int base, int exponent, int mod) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result = (result * base) % mod;
    }
    return result;
}

int gcd(int a, int b) {
    while (b != 0) {
        int t = b;
        b = a % b;
        a = t;
    }
    return a;
}

void generateKeys(int &n, int &e, int &d) {
    int p = 61; // Простой номер
    int q = 53; // Простой номер
    n = p * q;
    int phi = (p - 1) * (q - 1);

    e = 3; // Начальное значение
    while (gcd(e, phi) != 1) {
        e += 2;
    }

    d = modInverse(e, phi);
}

QByteArray reg(QString login, QString password, QString email)//Функция регистрации, создает новую запись с пользователем в БД. Формат: reg&login&password
{
    std::string sha_password = sha384(password.toStdString());
    password = QString::fromUtf8(sha_password.c_str());
    QString query = QString("INSERT INTO Users ('username', 'password', 'email') VALUES ('%1', '%2', '%3');").arg(login, password, email);
    qDebug() << query;
    QStringList res = Database::getInstance()->send_query(query, false);
    if (res[0] == "!0!"){
        return "Error during sending query";
    }
    else if (res[0] == "!1!"){
        return login.toUtf8() + " has been signed up";
    }
}

QByteArray auth(QString login, QString password) //Функция авторизации: возвращает success, если пароли в БД и введенный совпадают. Формат: auth&login&password
{
    password.remove("\r\n");
    QString query = QString("SELECT password FROM Users WHERE username = '%1';").arg(login);
    QStringList res = Database::getInstance()->send_query(query, true);
    if (res.length() == 0)
        return "Error";
    std::string sha_password = sha384(password.toStdString());
    password = QString::fromUtf8(sha_password.c_str());
    if (res[0] != password)
        return "Wrong password";
    else
        if(login == "admin"){
        return "You are admin";
    }
        return "Success";
    }

QImage stego(QString image_path, QString text){
    QFile file(image_path);
    QImage image;
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка открытия файла:" << file.errorString();
        return image;
    }
    QByteArray imageData = file.readAll();
    image.loadFromData(imageData, "PNG");
    file.remove();
    qDebug() << text;

    QImage newImage = image.convertToFormat(QImage::Format_ARGB32);
    // qDebug() << "New Image:" << newImage;
    QByteArray bits = text.toUtf8();
    bits.append('\0'); // Null-terminate the text

    int bitIndex = 0;
    for (int y = 0; y < newImage.height(); ++y) {
        for (int x = 0; x < newImage.width(); ++x) {
            if (bitIndex >= bits.size() * 8) {
                return newImage;
            }

            QRgb pixel = newImage.pixel(x, y);
            int r = qRed(pixel);
            int g = qGreen(pixel);
            int b = qBlue(pixel);

            r = (r & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
            bitIndex++;

            if (bitIndex < bits.size() * 8) {
                g = (g & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
                bitIndex++;
            }

            if (bitIndex < bits.size() * 8) {
                b = (b & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
                bitIndex++;
            }

            newImage.setPixel(x, y, qRgb(r, g, b));
        }
    }
    qDebug() << newImage;
    return newImage;
}

QByteArray get_users(){
    QString query = "SELECT username FROM Users;";
    QStringList users = Database::getInstance()->send_query(query, true);
    query = "SELECT email FROM Users;";
    QStringList email = Database::getInstance()->send_query(query, true);
    // qDebug() << users;
    // qDebug() << email;
    QByteArray res = ("sql;" + users.join(',') + ";" + email.join(',')).toUtf8();
    qDebug() << res;
    return res;
}

