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

#include "AsyncActionsThread.hpp"

AsyncActionsThread::AsyncActionsThread() {
    cInstance = this;

    mRunning.storeRelaxed(true);
}

AsyncActionsThread::~AsyncActionsThread() {
    cInstance = nullptr;
}

void AsyncActionsThread::stop() {
    mRunning.storeRelaxed(false);
}

void AsyncActionsThread::schedule(const AsyncActionsThread::Func& action) {
    mActionsMutex.lock();
    mActions.enqueue(action);
    mActionsMutex.unlock();
}

AsyncActionsThread* AsyncActionsThread::instance() {
    assert(cInstance != nullptr);
    return cInstance;
}

void AsyncActionsThread::run() {
    while (mRunning.loadRelaxed() == true) {
        mActionsMutex.lock();

        if (!mActions.isEmpty())
            mActions.dequeue()();

        mActionsMutex.unlock();
    }
}
