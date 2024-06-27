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
#include <functional>
#include <QThread>
#include <QAtomicInt>
#include <QMutex>
#include <QQueue>

class AsyncActionsThread final : public QThread {
public:
    using Func = std::function<void ()>;
private:
    QAtomicInt mRunning;
    QMutex mActionsMutex;
    QQueue<Func> mActions;
public:
    AsyncActionsThread();
    void stop();
    void schedule(const Func& action);

    DISABLE_COPY(AsyncActionsThread)
    DISABLE_MOVE(AsyncActionsThread)
protected:
    void run() override;
};
