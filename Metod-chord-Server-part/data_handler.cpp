// #include "data_handler.h"
// #include <QFile>
// #include <QFileInfo>
// #include <QDebug>

// DataHandler::DataHandler(QObject *parent) : QObject(parent)
// {
// }

// QByteArray DataHandler::handleRequest(const QByteArray &request) {
//     QString reqStr = QString::fromUtf8(request);
//     QStringList reqParts = reqStr.split("&");

//     QByteArray imageData = request.at(1);
//     QString text = request.at(2);
//     handleImage(imageData, text);
//     return "";
//     }
// }

// QByteArray DataHandler::handleText(const QString &text) {
//     qDebug() << "Received text:" << text;
//     return "Text received";
// }

// void DataHandler::handleImage(const QByteArray &imageData, const QString &text) {
//     QImage image;
//     if (image.loadFromData(imageData)) {
//         QImage imageWithText = embedTextInImage(image, text);
//         QByteArray byteArray;
//         QBuffer buffer(&byteArray);
//         buffer.open(QIODevice::WriteOnly);
//         imageWithText.save(&buffer, "PNG");
//         buffer.close();

//         // Отправляем изображение обратно клиенту
//         qDebug() << "Sending image back to client. Size:" << byteArray.size();
//         socket->write(byteArray);
//         socket->flush();
//         socket->waitForBytesWritten();
//         qDebug() << "Image sent back to client.";
//     } else {
//         qDebug() << "Failed to load image data";
//         socket->write("Failed to load image data");
//     }
// }

// QImage DataHandler::embedTextInImage(const QImage &image, const QString &text) {
//     QImage newImage = image.convertToFormat(QImage::Format_ARGB32);
//     QByteArray bits = text.toUtf8();
//     bits.append('\0'); // Null-terminate the text

//     int bitIndex = 0;
//     for (int y = 0; y < newImage.height(); ++y) {
//         for (int x = 0; x < newImage.width(); ++x) {
//             if (bitIndex >= bits.size() * 8) {
//                 return newImage;
//             }

//             QRgb pixel = newImage.pixel(x, y);
//             int r = qRed(pixel);
//             int g = qGreen(pixel);
//             int b = qBlue(pixel);

//             r = (r & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
//             bitIndex++;

//             if (bitIndex < bits.size() * 8) {
//                 g = (g & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
//                 bitIndex++;
//             }

//             if (bitIndex < bits.size() * 8) {
//                 b = (b & 0xFE) | ((bits[bitIndex / 8] >> (7 - (bitIndex % 8))) & 1);
//                 bitIndex++;
//             }

//             newImage.setPixel(x, y, qRgb(r, g, b));
//         }
//     }
//     return newImage;
// }
