/****************************************************************************
**
** Copyright (C) 2014-2015 Debao Zhang <hello@debao.me>
**
** This file is part of the QtOfficeOpenXml library.
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPLv2 included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
****************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opcpackage.h>
#include <opcpackagepart.h>

#include <QStringList>
#include <QMainWindow>
#include <QHash>
#include <QSet>

using namespace QtOfficeOpenXml;

class QTextDocument;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onActionOpenTriggered();
    void onActionRecentFileTriggered();
    void onActionCloseTriggered();
    void onActionSaveTriggered();
    void onActionSaveAsTriggered();
    void onActionAboutTriggered();
    void onPartItemChanged(const QString &partName);
    void onPartModificationChanged(bool changed);

private:
    void closeEvent(QCloseEvent *evt);

    void createActions();
    void createMenuBar();
    void updateRecentFilesMenu();
    void loadSettings();
    void saveSettings();
    bool openPackage(const QString &name);
    int getUserCloseOption();
    void closePackage();
    void doSavePackage(const QString &name, bool closeOld, bool openNew);

    Ui::MainWindow *ui;
    QAction *m_actionOpen;
    QAction *m_actionClose;
    QAction *m_actionSave;
    QAction *m_actionSaveAs;
    QAction *m_actionQuit;
    QAction *m_actionAbout;
    QMenu *m_menuRecentFiles;

    Opc::Package *m_package;
    QHash<QString, QTextDocument*> m_partDocumentHash;

    QStringList m_recentFilesList;
};

#endif // MAINWINDOW_H
