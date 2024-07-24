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
#include "Consts.hpp"
#include "Network.hpp"
#include <QMessageBox>

HomeWidget::BoardListItem::BoardListItem(const Board& board, const std::function<void ()>& parentUpdater) :
    mLayout(this),
    mId(board.id()),
    mParentUpdater(parentUpdater)
{
    QPixmap pixmap(25, 25);
    pixmap.fill(board.color());

    mColorLabel.setPixmap(pixmap);
    mLayout.addWidget(&mColorLabel);

    QFont font(mTitleLabel.font());
    font.setPointSize(14);

    mTitleLabel.setText(board.title());
    mTitleLabel.setFont(font);
    mLayout.addWidget(&mTitleLabel);

    mDeleteButton.setText("Delete");
    mDeleteButton.setFixedWidth(static_cast<int>(static_cast<float>(Consts::MIN_WINDOW_WIDTH) * 0.1f));
    connect(&mDeleteButton, &QPushButton::clicked, this, &HomeWidget::BoardListItem::deleteClicked);
    mLayout.addWidget(&mDeleteButton);

    mListItem.setSizeHint(QWidget::sizeHint());
}

QListWidgetItem* HomeWidget::BoardListItem::listItem() {
    return &mListItem;
}

void HomeWidget::BoardListItem::deleteClicked() {
    QMessageBox box(this);
    box.setModal(true);
    box.setText("Delete this board?");
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

    switch (box.exec()) {
        case QMessageBox::Yes:
            qDebug() << "y";
            Network::instance()->deleteBoard(mId);
            mParentUpdater();
            break;
        case QMessageBox::No:
            qDebug() << "n";
            break;
    }
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
    connect(&mNewBoardButton, &QPushButton::clicked, this, &HomeWidget::newBoardClicked);
    mLayout.addWidget(&mNewBoardButton, 0, Qt::AlignCenter);

    connect(Network::instance(), &Network::boardReceived, this, &HomeWidget::boardReceived);
    connect(Network::instance(), &Network::noBoardsReceived, this, &HomeWidget::noBoardsReceived);
}

HomeWidget::~HomeWidget() {
    for (auto i : mBoardListItems)
        delete i;

    disconnect(Network::instance(), &Network::boardReceived, this, &HomeWidget::boardReceived);
    disconnect(Network::instance(), &Network::noBoardsReceived, this, &HomeWidget::noBoardsReceived);
}

QSize HomeWidget::minimumSizeHint() const {
    return {Consts::MIN_WINDOW_WIDTH, Consts::MIN_WINDOW_HEIGHT};
}

void HomeWidget::addBoardToList(const Board& board) {
    auto boardListItem = new BoardListItem(board, [this](){ updateContent(); });
    mBoardsListWidget.addItem(boardListItem->listItem());
    mBoardsListWidget.setItemWidget(boardListItem->listItem(), boardListItem);
    mBoardListItems.push_back(boardListItem);
}

void HomeWidget::clearBoardsList() {
    for (int i = 0; i < mBoardsListWidget.count(); i++)
        mBoardsListWidget.takeItem(i);

    for (auto i : mBoardListItems) {
        qDebug() << i;
        delete i;
    }

    mBoardListItems.clear();
    mBoardsListWidget.clear();
}

void HomeWidget::boardsListItemClicked(QListWidgetItem* item) {
    mBoardsListWidget.clearSelection();
}

void HomeWidget::newBoardClicked() {

}

void HomeWidget::updateContent() {
    clearBoardsList();
    Network::instance()->getBoards();
}

void HomeWidget::boardReceived(const Board& board, bool oneOfMany) {
    addBoardToList(board);
}

void HomeWidget::noBoardsReceived(bool oneOfMany) {

}
