/*
 * Jaoned - an OpenGL & QT based drawing board
 * Copyright (C) 2024 Vadim Nikolaev (https://github.com/vadniks).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include "Network.hpp"

enum ActionFlag : int {
    LOG_IN = 0,
    REGISTER = 1,
    FINISH = 2,
    ERROR = 3,
    SUCCESS = 4,
    SHUTDOWN = 5
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"
struct Message {
    int size;
    ActionFlag flag;
    int from;
    QVector<uchar> body;
};
#pragma clang diagnostic pop

static const int MESSAGE_HEAD_SIZE = 12;
static const int MAX_MESSAGE_BODY_SIZE = 116;
static const int MAX_MESSAGE_SIZE = 128;

static QVector<uchar> packMessage(const Message& message) {
    assert(!message.body.isEmpty() && message.size > 0 || message.body.isEmpty() && message.size == 0);

    auto bytes = QVector<uchar>(MESSAGE_HEAD_SIZE + message.size);

    memcpy(&(bytes.data()[0]), &(message.size), 4);
    memcpy(&(bytes.data()[4]), &(message.flag), 4);
    memcpy(&(bytes.data()[8]), &(message.from), 4);

    if (!message.body.isEmpty())
        memcpy(&(bytes.data()[12]), message.body.data(), message.size);

    return bytes;
}

static Message unpackMessage(const QVector<uchar>& bytes) {
    Message message;

    memcpy(&(message.size), &(bytes.data()[0]), 4);
    memcpy(&(message.flag), &(bytes.data()[4]), 4);
    memcpy(&(message.from), &(bytes.data()[8]), 4);

    if (message.size > 0) {
        message.body = QVector<uchar>(message.size);
        memcpy(message.body.data(), &(bytes.data()[12]), message.size);
    } else
        message.body = QVector<uchar>(0);

    return message;
}

Network::Network() : mSocket(nullptr) {
    cInstance = this;

    connect(&mSocket, &QTcpSocket::connected, this, &Network::connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &Network::disconnected);
    connect(&mSocket, &QTcpSocket::errorOccurred, this, &Network::errorOccurred);
    connect(&mSocket, &QTcpSocket::readyRead, this, &Network::readyRead);
    connect(&mSocket, &QTcpSocket::bytesWritten, this, &Network::bytesWritten);
}

Network::~Network() {
    cInstance = nullptr;
}

void Network::connectToHost() {
    mSocket.connectToHost("127.0.0.1", 8080);
}

void Network::logIn(const QString& username, const QString& password) {
    emit logInTried(false);
}

void Network::xRegister(const QString& username, const QString& password) {
    emit registerTried(false);
}

Network* Network::instance() {
    assert(cInstance != nullptr);
    return cInstance;
}

void Network::connected() {
    emit eventOccurred(Event::CONNECTED);
}

void Network::disconnected() {
    emit eventOccurred(Event::DISCONNECTED);
}

void Network::errorOccurred(QAbstractSocket::SocketError error) {
    if (error != QAbstractSocket::SocketError::RemoteHostClosedError)
        emit eventOccurred(Event::ERROR_OCCURRED);
    mSocket.disconnectFromHost();
}

void Network::readyRead() {
    qDebug() << mSocket.bytesAvailable();
    if (mSocket.bytesAvailable() < 1000000)
        return;

    qDebug() << mSocket.read(5);
    mSocket.write(QByteArray(1000000, 1));
}

void Network::bytesWritten(long bytes) {

}
