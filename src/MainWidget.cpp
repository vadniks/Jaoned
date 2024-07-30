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

#include "MainWidget.hpp"
#include "Network.hpp"
#include <QResizeEvent>

MainWidget::MainWidget() :
    mLayout(this),
    mInfoLayout(&mInfoWidget),
    mBoardWidget(new BoardWidget([this](){ mControlsWidget.updateMode(); })),
    mControlsWidget(mBoardWidget),
    mBoard(-1, QColor(), "Board")
{
    mTitleLabel.setText(mBoard.title());
    mTitleLabel.setFont(QFont("Roboto", 18));
    mInfoLayout.addWidget(&mTitleLabel);

    mInfoLayout.addStretch();

    mProgressBar.setMaximum(0);
    mProgressBar.setMinimum(0);
    mProgressBar.setValue(0);
    mProgressBar.setFixedSize(16 * 20, 9 * 5);
    mProgressBar.setVisible(false);
    mInfoLayout.addWidget(&mProgressBar);

    mInfoLayout.addStretch();

    mExitButton.setText("Exit");
    mInfoLayout.addWidget(&mExitButton);

    mLayout.addWidget(&mInfoWidget, 0, Qt::AlignTop);
    mLayout.addWidget(&mControlsWidget, 0, Qt::AlignTop);
    mLayout.addWidget(mBoardWidget, 0, Qt::AlignVCenter);
    mLayout.addStretch();

    loading(true);

    connect(&mControlsWidget, &ControlsWidget::updated, this, &MainWidget::controlsWidgetUpdated);

    connect(mBoardWidget, &BoardWidget::pointsSetAdded, this, &MainWidget::pointsSetAdded);
    connect(mBoardWidget, &BoardWidget::lineAdded, this, &MainWidget::lineAdded);
    connect(mBoardWidget, &BoardWidget::textAdded, this, &MainWidget::textAdded);
    connect(mBoardWidget, &BoardWidget::imageAdded, this, &MainWidget::imageAdded);
    connect(mBoardWidget, &BoardWidget::lastElementRemoved, this, &MainWidget::lastElementRemoved);

    connect(Network::instance(), &Network::boardElementsReceiveFinished, this, &MainWidget::boardElementsReceiveFinished);
    connect(Network::instance(), &Network::pointsSetReceived, this, &MainWidget::pointsSetReceived);
    connect(Network::instance(), &Network::lineReceived, this, &MainWidget::lineReceived);
    connect(Network::instance(), &Network::textReceived, this, &MainWidget::textReceived);
    connect(Network::instance(), &Network::imageReceived, this, &MainWidget::imageReceived);
}

MainWidget::~MainWidget() {
    disconnect(Network::instance(), &Network::boardElementsReceiveFinished, this, &MainWidget::boardElementsReceiveFinished);

    delete mBoardWidget;
}

void MainWidget::setBoard(const Board& board) {
    mBoard = board;
    mTitleLabel.setText(board.title());
}

void MainWidget::updateContent() {
    loading(true);
    Network::instance()->boardElements();
}

void MainWidget::resizeEvent(QResizeEvent* event) {
    resizeBoardWidget();
    QWidget::resizeEvent(event);
}

void MainWidget::resizeBoardWidget() {
    const auto size = this->size();
    const auto probe = mControlsWidget.size();
    const auto margin = (size.width() - probe.width()) / 2;

    mBoardWidget->resize(probe.width(), size.height() - probe.height() - margin * 3);
}

void MainWidget::loading(bool enabled) {
    mProgressBar.setVisible(enabled);

    mExitButton.setEnabled(!enabled);

    mControlsWidget.setEnabled(!enabled);
    mBoardWidget->setEnabled(!enabled);
}

void MainWidget::controlsWidgetUpdated() {
    resizeBoardWidget();
}

void MainWidget::boardElementsReceiveFinished() {
    loading(false);
}

void MainWidget::pointsSetReceived(const PointsSetDto& pointsSetDto) {
    mBoardWidget->addPointsSet(pointsSetDto);
}

void MainWidget::lineReceived(const LineDto& lineDto) {
    mBoardWidget->addLine(lineDto);
}

void MainWidget::textReceived(const TextDto& textDto) {
    mBoardWidget->addText(textDto);
}

void MainWidget::imageReceived(const ImageDto& imageDto) {
    mBoardWidget->addImage(imageDto);
}

void MainWidget::undoReceived() {
    mBoardWidget->removeLastElement();
}

void MainWidget::pointsSetAdded(const PointsSetDto& pointsSetDto) {
    qDebug() << "pointsSet sent" << pointsSetDto.erase() << ' ' << pointsSetDto.width() << ' ' << pointsSetDto.color() << ' ' << pointsSetDto.points().size();
    Network::instance()->sendPointsSet(pointsSetDto);
}

void MainWidget::lineAdded(const LineDto& lineDto) {
    qDebug() << "line sent" << lineDto.start().x() << ' ' << lineDto.start().y() << ' ' << lineDto.end().x() << ' ' << lineDto.end().y() << ' ' << lineDto.width() << ' ' << lineDto.color();
    Network::instance()->sendLine(lineDto);
}

void MainWidget::textAdded(const TextDto& textDto) {
    qDebug() << "text sent" << textDto.pos().x() << ' ' << textDto.pos().y() << ' ' << textDto.fontSize() << ' ' << textDto.color() << ' ' << textDto.text();
    Network::instance()->sendText(textDto);
}

void MainWidget::imageAdded(const ImageDto& imageDto) {
    qDebug() << "image sent" << imageDto.pos().x() << ' ' << imageDto.pos().y() << ' ' << imageDto.width() << ' ' << imageDto.height() << ' ' << imageDto.texture().size();
    Network::instance()->sendImage(imageDto);
}

void MainWidget::lastElementRemoved() {
    qDebug() << "undo sent";
    Network::instance()->sendUndo();
}
