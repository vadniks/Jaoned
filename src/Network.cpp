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

Network::SocketListener::SocketListener() {
    mRunning.storeRelaxed(true);
}

void Network::SocketListener::stop() {
    mRunning.storeRelaxed(false);
}

void Network::SocketListener::run() {
    while (mRunning.loadRelaxed() == true) {
//        qDebug() << "listen";
        sleep(1);
    }
}

Network::Network() {
    cInstance = this;

    connect(&mSocket, &QTcpSocket::connected, this, &Network::connected);
    connect(&mSocket, &QTcpSocket::disconnected, this, &Network::disconnected);
    connect(&mSocket, &QTcpSocket::errorOccurred, this, &Network::errorOccurred);

    mSocketListener.start();
}

Network::~Network() {
    mSocketListener.stop();
    mSocketListener.wait();

    mSocket.close();

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

void Network::logOut() {

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
    emit eventOccurred(Event::ERROR_OCCURRED);
    mSocket.disconnectFromHost();
}
