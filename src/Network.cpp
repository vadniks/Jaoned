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
#include <QDateTime>

enum Network::Flag : int {
    ERROR = 0,
    LOG_IN = 1,
    REGISTER = 2,
    SHUTDOWN = 3,
    POINTS_SET = 4,
    LINE = 5,
    TEXT = 6,
    IMAGE = 7
};

struct Network::Message {
    Flag flag;
    int index;
    int count;
    long timestamp;
    //int bodySize;
    QList<char> body;

    Message() : flag(Flag::ERROR), index(0), count(0), timestamp(0), body() {}
};

Network::Network() {
    assert(cInstance == nullptr);
    cInstance = this;

    connect(&mSocket, &QTcpSocket::connected, this, &Network::connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &Network::disconnected);
    connect(&mSocket, &QTcpSocket::errorOccurred, this, &Network::errorOccurred);
    connect(&mSocket, &QTcpSocket::readyRead, this, &Network::readyRead);
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
    sendUsernameAndPassword(username, password, Flag::LOG_IN);
}

void Network::registerUser(const QString& username, const QString& password) {
    sendUsernameAndPassword(username, password, Flag::REGISTER);
}

void Network::sendPointsSet(const PointsSetDto& pointsSetDto) {
    sendBytes(pointsSetDto.pack(), Flag::POINTS_SET);
}

void Network::sendLine(const LineDto& lineDto) {
    sendBytes(lineDto.pack(), Flag::LINE);
}

void Network::sendText(const TextDto& textDto) {
    sendBytes(textDto.pack(), Flag::TEXT);
}

void Network::sendImage(const ImageDto& imageDto) {
    sendBytes(imageDto.pack(), Flag::IMAGE);
}

Network* Network::instance() {
    assert(cInstance != nullptr);
    return cInstance;
}

long Network::currentTimestamp() {
    return QDateTime::currentMSecsSinceEpoch();
}

void Network::sendUsernameAndPassword(const QString& username, const QString& password, Flag flag) {
    Message message;
    message.flag = flag;
    message.index = 0;
    message.count = 0;
    message.timestamp = currentTimestamp();

    message.body = QList<char>(MAX_CREDENTIAL_SIZE * 2);
    memcpy(message.body.data() + 0, username.toUtf8().data(), username.toUtf8().size());
    memcpy(message.body.data() + MAX_CREDENTIAL_SIZE, password.toUtf8().data(), password.toUtf8().size());

    sendMessage(message);
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
    while (mSocket.bytesAvailable() >= MESSAGE_HEAD_SIZE) {
        QList<char> head(MESSAGE_HEAD_SIZE);
        if (mSocket.read(head.data(), MESSAGE_HEAD_SIZE) < MESSAGE_HEAD_SIZE) return;

        Message message;
        int bodySize;

        memcpy(&(message.flag), &(head.data()[0]), 4);
        memcpy(&(message.index), &(head.data()[4]), 4);
        memcpy(&(message.count), &(head.data()[8]), 4);
        memcpy(&(message.timestamp), &(head.data()[12]), 8);
        memcpy(&bodySize, &(head.data()[20]), 4);

        message.body = QList<char>(bodySize);
        if (mSocket.read(message.body.data(), bodySize) < bodySize) return;

        processMessage(message);
    }
}

void Network::sendBytes(const QList<char>& bytes, Flag flag) {
    int start = 0;
    int index = 0;
    const int count = static_cast<int>(ceilf(static_cast<float>(bytes.size()) / static_cast<float>(MAX_MESSAGE_BODY_SIZE)));
    const long timestamp = currentTimestamp();

    while (true) {
        Message message;
        message.flag = flag;
        message.index = index++;
        message.count = count;
        message.timestamp = timestamp;
        message.body = bytes.mid(start, MAX_MESSAGE_BODY_SIZE);

        if (message.body.isEmpty()) break;
        sendMessage(message);
    }
}

void Network::sendMessage(const Message& message) {
    assert(message.timestamp > 0);

    const int size = static_cast<int>(message.body.size());
    auto head = QList<char>(MESSAGE_HEAD_SIZE);

    memcpy(&(head.data()[0]), &(message.flag), 4);
    memcpy(&(head.data()[4]), &(message.index), 4);
    memcpy(&(head.data()[8]), &(message.count), 4);
    memcpy(&(head.data()[12]), &(message.timestamp), 8);
    memcpy(&(head.data()[20]), &size, 4);

    if (mSocket.write(head.data(), MESSAGE_HEAD_SIZE) < MESSAGE_HEAD_SIZE) return;
    if (size > 0) mSocket.write(message.body.data(), size);
}

void Network::processMessage(const Message& message) {
    qDebug() << message.flag;
    switch (message.flag) {
        case LOG_IN:
            qDebug() << (static_cast<int>(message.body.size()) > 0);
            break;
        case REGISTER:
            qDebug() << (static_cast<int>(message.body.size()) > 0);
            break;
        case ERROR:
            qDebug() << "error";
            emit eventOccurred(Event::ERROR_OCCURRED);
            break;
        case SHUTDOWN:
            break;
    }
}
