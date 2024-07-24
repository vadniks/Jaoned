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
#include <functional>
#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QProgressBar>

class HomeWidget final : public QWidget {
    Q_OBJECT
private:
    class BoardListItem final : public QWidget {
    private:
        QHBoxLayout mLayout;
        QLabel mColorLabel;
        QLabel mTitleLabel;
        QPushButton mDeleteButton;
        QListWidgetItem mListItem;
        int mId;
        std::function<void (int)> mDeleteItemImpl;
    public:
        BoardListItem(const Board& board, const std::function<void (int)>& deleteItemImpl);
        QListWidgetItem* listItem();
    private:
        void deleteClicked();
    };
private:
    QVBoxLayout mLayout;
    QLabel mBoardsLabel;
    QListWidget mBoardsListWidget;
    QList<BoardListItem*> mBoardListItems; // TODO: add refresh list button
    QWidget mButtonsWidget;
    QHBoxLayout mButtonsLayout;
    QPushButton mNewBoardButton;
    QPushButton mRefreshButton;
    QProgressBar mProgressBar;
public:
    HomeWidget();
    ~HomeWidget() override;
    QSize minimumSizeHint() const override;
    void addBoardToList(const Board& board);
    void clearBoardsList();
private:
    void deleteItem(int id);
    void loading(bool enable);
private slots:
    void boardsListItemClicked(QListWidgetItem* item);
    void newBoardClicked();
public slots:
    void updateContent();
    void boardReceived(const Board& board, bool finished);
    void noBoardsReceived();
    void deleteBoardTried(bool successful);
};
