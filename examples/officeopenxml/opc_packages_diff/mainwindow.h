#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSharedPointer>

class QProcess;
class QFile;
class QTemporaryDir;

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
    void onFileOpenActionTriggered();
    void onAboutActionTriggered();
    void onFileOpenButtonClicked();
    void onRunButtonClicked();
    void onRunProcessFinished();
    void onRunProcessError();
    void onRunProcessReadyRead();

private:
    void closeEvent(QCloseEvent *evt);
    void loadSettings();
    void saveSettings() const;
    bool extractOpcPackage(const QString &packageName, const QString &dir);
    void copyRawDataToFile(QIODevice *srcDevice, QFile &desFile);
    void copyFormattedXmlDataToFile(QIODevice *srcDevice, QFile &desFile);
    bool startDiffProcess(const QString &name, const QStringList &args);

    Ui::MainWindow *ui;
    QString m_lastPath;
    QSharedPointer<QTemporaryDir> m_dir1;
    QSharedPointer<QTemporaryDir> m_dir2;
    QProcess *m_process;
};

#endif // MAINWINDOW_H
