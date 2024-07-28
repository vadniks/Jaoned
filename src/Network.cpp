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
    CREATE_BOARD = 4,
    GET_BOARD = 5,
    GET_BOARDS = 6,
    DELETE_BOARD = 7,
    POINTS_SET = 8,
    LINE = 9,
    TEXT = 10,
    IMAGE = 11,
    UNDO = 12,
    SELECT_BOARD = 13,
    GET_BOARD_ELEMENTS = 14
};

enum ElementType : int {
    POINTS_SET_TYPE = 0,
    LINE_TYPE = 1,
    TEXT_TYPE = 2,
    IMAGE_TYPE = 3
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

Network::Network() : mConnected(false) {
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

void Network::sendUndo() {
    Message message;
    message.flag = Flag::UNDO;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();
    sendMessage(message);
}

void Network::selectBoard(int id) {
    Message message;
    message.flag = Flag::SELECT_BOARD;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();

    message.body = QList<char>(4);
    memcpy(message.body.data(), &id, 4);

    sendMessage(message);
}

void Network::boardElements() {
    Message message;
    message.flag = Flag::GET_BOARD_ELEMENTS;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();
    sendMessage(message);
}

void Network::createBoard(const Board& board) {
    Message message;
    message.flag = Flag::CREATE_BOARD;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();
    message.body = board.pack();
    sendMessage(message);
}

void Network::getBoard(int id) {
    Message message;
    message.flag = Flag::GET_BOARD;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();

    message.body = QList<char>(4);
    memcpy(message.body.data(), &id, 4);

    sendMessage(message);
}

void Network::getBoards() {
    Message message;
    message.flag = Flag::GET_BOARDS;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();
    sendMessage(message);
}

void Network::deleteBoard(int id) {
    Message message;
    message.flag = Flag::DELETE_BOARD;
    message.index = 0;
    message.count = 1;
    message.timestamp = currentTimestamp();

    message.body = QList<char>(4);
    memcpy(message.body.data(), &id, 4);

    sendMessage(message);
}

bool Network::connectedToHost() {
    return mConnected;
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
    message.count = 1;
    message.timestamp = currentTimestamp();

    message.body = QList<char>(MAX_CREDENTIAL_SIZE * 2);
    memcpy(message.body.data() + 0, username.toUtf8().data(), username.toUtf8().size());
    memcpy(message.body.data() + MAX_CREDENTIAL_SIZE, password.toUtf8().data(), password.toUtf8().size());

    sendMessage(message);
}

void Network::connected() {
    mConnected = true;
    emit eventOccurred(Event::CONNECTED);
}

void Network::disconnected() {
    mConnected = false;
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

        if (bodySize > 0) {
            message.body = QList<char>(bodySize);
            if (mSocket.read(message.body.data(), bodySize) < bodySize) return;
        }

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

        start += static_cast<int>(message.body.size());

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
    switch (message.flag) {
        case Flag::LOG_IN:
            qDebug() << "logIn " << (static_cast<int>(message.body.size()) > 0);
            emit logInTried(static_cast<int>(message.body.size()) > 0);
            break;
        case Flag::REGISTER:
            qDebug() << "register " << (static_cast<int>(message.body.size()) > 0);
            emit registerTried(static_cast<int>(message.body.size()) > 0);
            break;
        case Flag::ERROR:
            qDebug() << "error";
            emit eventOccurred(Event::ERROR_OCCURRED);
            break;
        case Flag::SHUTDOWN:
            assert(false);
        case Flag::CREATE_BOARD:
            emit createBoardTried(static_cast<int>(message.body.size()) > 0);
            break;
        case Flag::GET_BOARD:
            {
                auto board = Board::unpack(message.body);
                qDebug() << "getBoard " << board.id() << ' ' << board.color() << ' ' << board.title();
            }
            break;
        case Flag::GET_BOARDS:
            processBoards(message);
            break;
        case Flag::DELETE_BOARD:
            emit deleteBoardTried(static_cast<int>(message.body.size()) > 0);
            break;
        case Flag::POINTS_SET:
            mPendingMessages[message.timestamp].enqueue(message);
            if (message.index == message.count - 1)
                processPointsSet(message.timestamp);
            break;
        case Flag::LINE:
            {
                const auto line = LineDto::unpack(message.body);
                qDebug() << "line received" << line.start().x() << ' ' << line.start().y() << ' ' << line.end().x() << ' ' << line.end().y() << ' ' << line.width() << ' ' << line.color();
                emit lineReceived(line);
            }
            break;
        case Flag::TEXT:
            mPendingMessages[message.timestamp].enqueue(message);
            if (message.index == message.count - 1)
                processText(message.timestamp);
            break;
        case Flag::IMAGE:
            mPendingMessages[message.timestamp].enqueue(message);
            if (message.index == message.count - 1)
                processImage(message.timestamp);
            break;
        case Flag::UNDO:
            emit undoReceived();
            break;
        case Flag::SELECT_BOARD:
            assert(false);
        case Flag::GET_BOARD_ELEMENTS:
            processBoardElements(message);
            break;
    }
}

void Network::processBoards(const Message& message) {
    if (message.count == 1 && message.body.isEmpty()) {
        noBoardsReceived();
        return;
    }

    mPendingMessages[message.timestamp].enqueue(message);

    if (message.index == message.count - 1) {
        while (!mPendingMessages[message.timestamp].empty()) {
            const auto xMessage = mPendingMessages[message.timestamp].dequeue();
            boardReceived(Board::unpack(xMessage.body), xMessage.index == xMessage.count - 1);
        }
        mPendingMessages.remove(message.timestamp);
    }
}

void Network::processBoardElements(const Message& message) {
    if (message.body.isEmpty()) {
        qDebug() << "pbe finish";
        emit boardElementsReceiveFinished();
        return;
    }

    mPendingMessages[message.timestamp].enqueue(message);
    if (message.index < message.count - 1) return;

    QList<char> bytes;

    while (!mPendingMessages[message.timestamp].empty())
        bytes.append(mPendingMessages[message.timestamp].dequeue().body);
    mPendingMessages.remove(message.timestamp);

    ElementType type;
    memcpy(&type, bytes.data(), 4);
    qDebug() << "pbe " << type;
}

void Network::processPointsSet(long timestamp) {
    QList<char> bytes;

    while (!mPendingMessages[timestamp].empty())
        bytes.append(mPendingMessages[timestamp].dequeue().body);
    mPendingMessages.remove(timestamp);

    const auto pointsSet = PointsSetDto::unpack(bytes);
    qDebug() << "pointsSet received" << pointsSet.erase() << ' ' << pointsSet.width() << ' ' << pointsSet.color() << ' ' << pointsSet.points().size();
    emit pointsSetReceived(pointsSet);
}

void Network::processText(long timestamp) {
    QList<char> bytes;

    while (!mPendingMessages[timestamp].empty())
        bytes.append(mPendingMessages[timestamp].dequeue().body);
    mPendingMessages.remove(timestamp);

    const auto text = TextDto::unpack(bytes);
    qDebug() << "text received" << text.pos().x() << ' ' << text.pos().y() << ' ' << text.fontSize() << ' ' << text.color() << ' ' << text.text();
    emit textReceived(text);
}

void Network::processImage(long timestamp) {
    QList<char> bytes;

    while (!mPendingMessages[timestamp].empty())
        bytes.append(mPendingMessages[timestamp].dequeue().body);
    mPendingMessages.remove(timestamp);

    const auto image = ImageDto::unpack(bytes);
    qDebug() << "image received" << image.pos().x() << ' ' << image.pos().y() << ' ' << image.width() << ' ' << image.height() << ' ' << image.texture().size();
    emit imageReceived(image);
}
