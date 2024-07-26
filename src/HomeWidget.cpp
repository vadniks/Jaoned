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
#include "BoardCreationDialog.hpp"
#include <QMessageBox>
#include <QTimer>

HomeWidget::BoardListItem::BoardListItem(const Board& board, const std::function<void (int)>& deleteItemImpl) :
    mLayout(this),
    mId(board.id()),
    mDeleteItemImpl(deleteItemImpl)
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

    if (box.exec() == QMessageBox::Yes)
        mDeleteItemImpl(mId);
}

HomeWidget::HomeWidget() : mLayout(this), mButtonsLayout(&mButtonsWidget) {
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
    mButtonsLayout.addWidget(&mNewBoardButton);

    mRefreshButton.setText("Refresh");
    connect(&mRefreshButton, &QPushButton::clicked, this, &HomeWidget::updateContent);
    mButtonsLayout.addWidget(&mRefreshButton);

    mLayout.addWidget(&mButtonsWidget, 0, Qt::AlignCenter);

    mProgressBar.setMaximum(0);
    mProgressBar.setMinimum(0);
    mProgressBar.setValue(0);
    mProgressBar.setFixedSize(16 * 20, 9 * 5);
    mProgressBar.setVisible(false);
    mLayout.addWidget(&mProgressBar);

    connect(Network::instance(), &Network::boardReceived, this, &HomeWidget::boardReceived);
    connect(Network::instance(), &Network::noBoardsReceived, this, &HomeWidget::noBoardsReceived);
    connect(Network::instance(), &Network::deleteBoardTried, this, &HomeWidget::deleteBoardTried);
}

HomeWidget::~HomeWidget() {
    for (auto i : mBoardListItems)
        delete i;

    disconnect(Network::instance(), &Network::boardReceived, this, &HomeWidget::boardReceived);
    disconnect(Network::instance(), &Network::noBoardsReceived, this, &HomeWidget::noBoardsReceived);
    disconnect(Network::instance(), &Network::deleteBoardTried, this, &HomeWidget::deleteBoardTried);
}

QSize HomeWidget::minimumSizeHint() const {
    return {Consts::MIN_WINDOW_WIDTH, Consts::MIN_WINDOW_HEIGHT};
}

void HomeWidget::addBoardToList(const Board& board) {
    auto boardListItem = new BoardListItem(board, [this](int id){ deleteItem(id); });
    mBoardsListWidget.addItem(boardListItem->listItem());
    mBoardsListWidget.setItemWidget(boardListItem->listItem(), boardListItem);
    mBoardListItems.push_back(boardListItem);
}

void HomeWidget::clearBoardsList() {
    for (int i = 0; i < mBoardsListWidget.count(); i++)
        mBoardsListWidget.takeItem(i);

    for (auto i : mBoardListItems)
        delete i;

    mBoardListItems.clear();
    mBoardsListWidget.clear();
}

void HomeWidget::deleteItem(int id) {
    loading(true);
    Network::instance()->deleteBoard(id);
}

void HomeWidget::loading(bool enable) {
    mProgressBar.setVisible(enable);
    mBoardsListWidget.setEnabled(!enable);
    mNewBoardButton.setEnabled(!enable);
    mRefreshButton.setEnabled(!enable);
}

void HomeWidget::boardsListItemClicked(QListWidgetItem* item) {
    mBoardsListWidget.clearSelection();
}

void HomeWidget::newBoardClicked() {
    BoardCreationDialog().exec();
}

void HomeWidget::updateContent() {
    loading(true);
    clearBoardsList();
    Network::instance()->getBoards();
}

void HomeWidget::boardReceived(const Board& board, bool finished) {
    addBoardToList(board);
    if (finished) loading(false);
}

void HomeWidget::noBoardsReceived() {
    loading(false);
}

void HomeWidget::deleteBoardTried(bool successful) {
    updateContent();
}
