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
#include <QThread>
#include <QAtomicInt>

struct Message;

class Network final : public QObject {
    Q_OBJECT
public:
    enum Event {
        CONNECTED,
        ERROR_OCCURRED,
        DISCONNECTED
    };
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
    void xRegister(const QString& username, const QString& password);
    void shutdown();

    DISABLE_COPY(Network)
    DISABLE_MOVE(Network)

    static Network* instance();
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError error);
    void readyRead();
    void bytesWritten(long bytes);
private:
    void sendMessage(const Message& message);
    void processMessage(const Message& message);
signals: // those are implemented elsewhere
    void eventOccurred(Network::Event event);
    void logInTried(bool successful);
    void registerTried(bool successful);
};
