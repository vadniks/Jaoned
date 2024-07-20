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

#pragma once

#include "defs.hpp"
#include <QObject>
#include <QTcpSocket>

class Network final : public QObject {
    Q_OBJECT
public:
    enum Event {
        CONNECTED,
        ERROR_OCCURRED,
        DISCONNECTED
    };
private:
    enum Flag : int;
    struct Message;
private:
    static inline Network* cInstance = nullptr;
    QTcpSocket mSocket;
public:
    static const int MESSAGE_HEAD_SIZE = 4 + 4 + 4 + 8 + 4; // 24
    static const int MAX_MESSAGE_SIZE = 0x80; // 128
    static const int MAX_MESSAGE_BODY_SIZE = MAX_MESSAGE_SIZE - MESSAGE_HEAD_SIZE; // 104
    static const int MAX_CREDENTIAL_SIZE = 16;
public:
    Network();
    ~Network() override;

    void connectToHost();
    void disconnectFromHost();
    void logIn(const QString& username, const QString& password);
    void registerUser(const QString& username, const QString& password);

    DISABLE_COPY(Network)
    DISABLE_MOVE(Network)

    static Network* instance();
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError error);
    void readyRead();
private:
    void sendBytes(const QList<char>& bytes, Flag flag);
    void sendMessage(const Message& message);
    void processMessage(const Message& message);
signals: // those are implemented elsewhere
    void eventOccurred(Network::Event event);
};
