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

#include "LooperThread.hpp"

LooperThread::LooperThread() {
    mRunning.storeRelaxed(true);
}

void LooperThread::stop() {
    mRunning.storeRelaxed(false);
}

void LooperThread::schedule(const LooperThread::Func& action) {
    mActionsMutex.lock();
    mActions.enqueue(action);
    mActionsMutex.unlock();
}

void LooperThread::run() {
    while (mRunning.loadRelaxed() == true) {
        mActionsMutex.lock();

        if (!mActions.isEmpty())
            mActions.dequeue()();

        mActionsMutex.unlock();
    }
}
