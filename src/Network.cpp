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

struct Message {
    int size;
    ActionFlag flag;
    QVector<uchar> body;

    Message(int size, ActionFlag flag, const QVector<uchar>& body) : size(size), flag(flag), body(body) {}
    explicit Message(ActionFlag flag) : size(0), flag(flag), body(0) {}
    Message() : size(0), flag(ActionFlag::LOG_IN), body(0) {}
};

static const int MESSAGE_HEAD_SIZE = 8;
static const int MAX_MESSAGE_BODY_SIZE = 120;
static const int MAX_MESSAGE_SIZE = 128;

static QVector<uchar> packMessage(const Message& message) {
    assert(!message.body.isEmpty() && message.size > 0 || message.body.isEmpty() && message.size == 0);

    auto bytes = QVector<uchar>(MESSAGE_HEAD_SIZE + message.size);

    memcpy(&(bytes.data()[0]), &(message.size), 4);
    memcpy(&(bytes.data()[4]), &(message.flag), 4);

    if (!message.body.isEmpty())
        memcpy(&(bytes.data()[8]), message.body.data(), message.size);

    return bytes;
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
    Message message(ActionFlag::LOG_IN);
    message.size = MAX_CREDENTIAL_SIZE * 2;

    message.body = QVector<uchar>(MAX_CREDENTIAL_SIZE * 2);
    memcpy(message.body.data(), (uchar[8]) {'a', 'd', 'm', 'i', 'n', 0, 0, 0}, 8);
    memcpy(message.body.data() + 8, (uchar[8]) {'p', 'a', 's', 's', 0, 0, 0, 0}, 8);

    sendMessage(message);
}

void Network::xRegister(const QString& username, const QString& password) {
    Message message(ActionFlag::REGISTER);
    message.size = MAX_CREDENTIAL_SIZE * 2;

    message.body = QVector<uchar>(MAX_CREDENTIAL_SIZE * 2);
    memcpy(message.body.data(), (uchar[8]) {'t', 'e', 's', 't', 0, 0, 0, 0}, 8);
    memcpy(message.body.data() + 8, (uchar[8]) {'p', 'a', 's', 's', 0, 0, 0, 0}, 8);

    sendMessage(message);
}

void Network::shutdown() {
    sendMessage(Message(0, ActionFlag::SHUTDOWN, QVector<uchar>(0)));
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
    assert(mSocket.bytesAvailable() >= MESSAGE_HEAD_SIZE);

    QVector<uchar> buffer(MAX_MESSAGE_SIZE);
    mSocket.read(reinterpret_cast<char*>(buffer.data()), MESSAGE_HEAD_SIZE);

    Message message;

    memcpy(&(message.size), &(buffer.data()[0]), 4);
    memcpy(&(message.flag), &(buffer.data()[4]), 4);

    assert(message.size <= mSocket.bytesAvailable());
    if (message.size > 0) {
        mSocket.read(&(reinterpret_cast<char*>(buffer.data())[8]), message.size);

        message.body = QVector<uchar>(message.size);
        memcpy(message.body.data(), &(buffer.data()[12]), message.size);
    } else
        message.body = QVector<uchar>(0);

    processMessage(message);
}

void Network::bytesWritten(long bytes) {

}

void Network::sendMessage(const Message& message) {
    mSocket.write(reinterpret_cast<const char*>(packMessage(message).data()), MESSAGE_HEAD_SIZE + message.size);
}

void Network::processMessage(const Message& message) {
    qDebug() << message.flag;
    switch (message.flag) {
        case LOG_IN:
            qDebug() << "pm logIn " << (message.size > 0 ? "true" : "false");
            emit logInTried(message.size > 0);
            break;
        case REGISTER:
            qDebug() << "pm register " << (message.size > 0 ? "true" : "false");
            emit registerTried(message.size > 0);
            break;
        case FINISH:
            break;
        case ERROR:
            qDebug() << "error";
            break;
        case SUCCESS:
            break;
        case SHUTDOWN:
            break;
    }
}
