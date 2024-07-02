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

#include "HomeWidget.hpp"
#include "defs.hpp"

HomeWidget::BoardListItem::BoardListItem(const Board& board) : mLayout(this) {
    QPixmap pixmap(25, 25);
    pixmap.fill(board.color());

    mColorLabel.setPixmap(pixmap);
    mLayout.addWidget(&mColorLabel);

    QFont font(mTitleLabel.font());
    font.setPointSize(14);

    mTitleLabel.setText(board.title());
    mTitleLabel.setFont(font);
    mLayout.addWidget(&mTitleLabel);

    mListItem.setSizeHint(QWidget::sizeHint());
}

QListWidgetItem* HomeWidget::BoardListItem::listItem() {
    return &mListItem;
}

HomeWidget::HomeWidget() : mLayout(this) {
    QFont font(mBoardsLabel.font());
    font.setPointSize(14);

    mBoardsLabel.setText("Boards");
    mBoardsLabel.setFont(font);
    mLayout.addWidget(&mBoardsLabel, 0, Qt::AlignCenter);

    mBoardsListWidget.setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    connect(&mBoardsListWidget, &QListWidget::itemClicked, this, &HomeWidget::boardsListItemClicked);
    mLayout.addWidget(&mBoardsListWidget);

    mNewBoardButton.setText("New");
    mNewBoardButton.setFixedWidth(100);
    mLayout.addWidget(&mNewBoardButton, 0, Qt::AlignCenter);

    // TODO: test only
    addBoardToList(Board("Test 1", QColor(255, 255, 255)));
    addBoardToList(Board("Test 2", QColor(170, 170, 170)));
    addBoardToList(Board("Test 3", QColor(85, 85, 85)));
    refillList();
}

HomeWidget::~HomeWidget() {
    for (auto i : mBoardListItems)
        delete i;
}

QSize HomeWidget::minimumSizeHint() const {
    return {Consts::MIN_WINDOW_WIDTH, Consts::MIN_WINDOW_HEIGHT};
}

void HomeWidget::refillList() {
    mBoardsListWidget.clear();

    for (auto boardListItem : mBoardListItems) {
        const auto item = boardListItem->listItem();
        mBoardsListWidget.addItem(item);
        mBoardsListWidget.setItemWidget(item, boardListItem);
    }
}

void HomeWidget::addBoardToList(const Board& board) {
    mBoardListItems.push_back(new BoardListItem(board));
}

void HomeWidget::clearBoardsList() {
    for (auto i : mBoardListItems)
        delete i;

    mBoardListItems.clear();
}

void HomeWidget::boardsListItemClicked(QListWidgetItem* item) {
    mBoardsListWidget.clearSelection();
}
