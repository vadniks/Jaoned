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
#include "dto.hpp"
#include "Board.hpp"
#include <QObject>
#include <QTcpSocket>
#include <QQueue>
#include <QMap>

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
    QMap<long, QQueue<Message>> mPendingMessages;
    bool mConnected;
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

    void sendPointsSet(const PointsSetDto& pointsSetDto);
    void sendLine(const LineDto& lineDto);
    void sendText(const TextDto& textDto);
    void sendImage(const ImageDto& imageDto);
    void sendUndo();
    void sendClear();

    void selectBoard(int id);
    void boardElements();

    void createBoard(const Board& board);
    void getBoard(int id);
    void getBoards();
    void deleteBoard(int id);

    bool connectedToHost();

    DISABLE_COPY(Network)
    DISABLE_MOVE(Network)

    static Network* instance();
private:
    static long currentTimestamp();
    void sendUsernameAndPassword(const QString& username, const QString& password, Flag flag);
private slots:
    void connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError error);
    void readyRead();
private:
    void sendBytes(const QList<char>& bytes, Flag flag);
    void sendMessage(const Message& message);
    void processMessage(const Message& message);

    void processBoards(const Message& message);
    void processBoardElements(const Message& message);

    void processPointsSet(long timestamp);
    void processText(long timestamp);
    void processImage(long timestamp);
signals: // those are implemented elsewhere
    void eventOccurred(Network::Event event);

    void logInTried(bool successful);
    void registerTried(bool successful);

    void createBoardTried(bool successful);
    void boardReceived(const Board& board, bool finished);
    void noBoardsReceived();
    void deleteBoardTried(bool successful);

    void pointsSetReceived(const PointsSetDto& pointsSetDto);
    void lineReceived(const LineDto& lineDto);
    void textReceived(const TextDto& textDto);
    void imageReceived(const ImageDto& imageDto);
    void undoReceived();
    void clearReceived();

    void boardElementsReceiveFinished();
};
