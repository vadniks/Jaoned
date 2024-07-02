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

#include "Board.hpp"
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>

class HomeWidget final : public QWidget {
    Q_OBJECT
private:
    class BoardListItem final : public QWidget {
    private:
        QHBoxLayout mLayout;
        QLabel mColorLabel;
        QLabel mTitleLabel;
        QListWidgetItem mListItem;
    public:
        explicit BoardListItem(const Board& board);
        QListWidgetItem* listItem();
    };
private:
    QVBoxLayout mLayout;
    QLabel mBoardsLabel;
    QListWidget mBoardsListWidget;
    QList<BoardListItem*> mBoardListItems;
public:
    HomeWidget();
    ~HomeWidget() override;
    QSize minimumSizeHint() const override;
    void refillList();
    void addBoardToList(const Board& board);
    void clearBoardsList();
private slots:
    void boardsListItemClicked(QListWidgetItem* item);
};
