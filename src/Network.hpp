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

class Network final : public QObject {
    Q_OBJECT
public:
    enum ActionFlag : int {
        ERROR = 0,
        LOG_IN = 1,
        REGISTER = 2,
        SHUTDOWN = 3
    };
private:
    static inline Network* cInstance = nullptr;
public:
    Network();
    ~Network() override;

    DISABLE_COPY(Network)
    DISABLE_MOVE(Network)

    static Network* instance();
//private slots:
//
//signals: // those are implemented elsewhere

};
