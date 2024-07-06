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

#include "Network.hpp"

// int size - prefix
struct Message {
    Network::ActionFlag flag;
    int index;
    int count;
    QList<uchar> body;

    Message() : flag(Network::ActionFlag::ERROR), index(0), count(0), body() {}

    Message(Network::ActionFlag flag, int index, int count, const QList<uchar>& body) :
        flag(flag), index(index), count(count), body(body)
    {}
};

static const int MESSAGE_HEAD_SIZE = 12;
static const int MAX_MESSAGE_SIZE = 128;
static const int MAX_MESSAGE_BODY_SIZE = MAX_MESSAGE_SIZE - MESSAGE_HEAD_SIZE; // 116
static const int MESSAGE_PREFIX_SIZE = 4;

Network::Network() : mSocket(nullptr) {
    assert(cInstance == nullptr);
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

void Network::disconnectFromHost() {
    mSocket.disconnectFromHost();
}

void Network::logIn(const QString& username, const QString& password) {

}

void Network::xRegister(const QString& username, const QString& password) {

}

void Network::shutdown() {

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
    while (mSocket.bytesAvailable() >= MESSAGE_PREFIX_SIZE + MESSAGE_HEAD_SIZE) {
        int messageSize;
        if (mSocket.read(reinterpret_cast<char*>(&messageSize), MESSAGE_PREFIX_SIZE) < MESSAGE_PREFIX_SIZE)
            return;

        const int bodySize = messageSize - MESSAGE_HEAD_SIZE;

        QList<uchar> head(MESSAGE_HEAD_SIZE);
        if (mSocket.read(reinterpret_cast<char*>(head.data()), MESSAGE_HEAD_SIZE) < MESSAGE_HEAD_SIZE)
            return;

        Message message;

        memcpy(&(message.flag), &(head.data()[0]), 4);
        memcpy(&(message.index), &(head.data()[4]), 4);
        memcpy(&(message.count), &(head.data()[8]), 4);

        if (bodySize > 0) {
            QList<uchar> body(bodySize);
            if (mSocket.read(reinterpret_cast<char*>(body.data()), bodySize) < bodySize)
                return;

            message.body = body;
        } else
            message.body = QList<uchar>(0);

        processMessage(message);
    }
}

void Network::bytesWritten(long bytes) {

}

void Network::sendMessage(const Message& message) {
    const int size = static_cast<int>(message.body.size());
    auto head = QList<uchar>(MESSAGE_HEAD_SIZE);

    memcpy(&(head.data()[0]), &(message.flag), 4);
    memcpy(&(head.data()[4]), &(message.index), 4);
    memcpy(&(head.data()[8]), &(message.count), 4);

    if (mSocket.write(reinterpret_cast<const char*>(&size), MESSAGE_PREFIX_SIZE) < MESSAGE_PREFIX_SIZE)
        return;

    if (mSocket.write(reinterpret_cast<const char*>(head.data()), MESSAGE_HEAD_SIZE) < MESSAGE_HEAD_SIZE)
        return;

    if (size > 0)
        mSocket.write(reinterpret_cast<const char*>(message.body.data()), size);
}

void Network::processMessage(const Message& message) {
    qDebug() << message.flag;
    switch (message.flag) {
        case LOG_IN:
            emit logInTried(static_cast<int>(message.body.size()) > 0);
            break;
        case REGISTER:
            emit registerTried(static_cast<int>(message.body.size()) > 0);
            break;
        case ERROR:
            qDebug() << "error";
            emit eventOccurred(Event::ERROR_OCCURRED);
            break;
        case SHUTDOWN:
            break;
    }
}
