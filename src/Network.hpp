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
private:
    enum Event : int;
    enum Flag : int;
    struct Message;
private:
    static inline Network* cInstance = nullptr;
    QTcpSocket mSocket;
public:
    static inline const int MAX_CREDENTIAL_SIZE = 8;
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
