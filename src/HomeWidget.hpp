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
#include <QListView>
#include <QAbstractListModel>

class HomeWidget final : public QWidget {
    Q_OBJECT
private:
    class BoardsListModel final : public QAbstractListModel {
    public:
        enum Roles {
            Title = Qt::ItemDataRole::UserRole,
            Color = Qt::ItemDataRole::UserRole + 1
        };
    private:
        QList<Board> mBoards;
        QHash<int, QByteArray> mRoles;
    public:
        BoardsListModel();
        int rowCount(const QModelIndex& parent) const override;
        int columnCount(const QModelIndex& parent) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QHash<int, QByteArray> roleNames() const override;
    };
private:
    QLabel mBoardsLabel;
    BoardsListModel mBoardsListModel;
    QListView mBoardsListView;
public:
    HomeWidget();
};
