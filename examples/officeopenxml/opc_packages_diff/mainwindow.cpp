#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opcpackage.h>
#include <opcpackagepart.h>
#include <mcexmlstreamreader.h>

#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QScopedPointer>
#include <QFileDialog>
#include <QCloseEvent>
#include <QRegularExpression>
#include <QProcess>
#include <QDebug>
#include <QTemporaryDir>
#include <QMessageBox>
#include <QInputDialog>

using namespace QtOfficeOpenXml;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_process = 0;

    connect(ui->actionOpen, SIGNAL(triggered()), SLOT(onFileOpenActionTriggered()));
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(onAboutActionTriggered()));
    connect(ui->file1Button, SIGNAL(clicked()), SLOT(onFileOpenButtonClicked()));
    connect(ui->file2Button, SIGNAL(clicked()), SLOT(onFileOpenButtonClicked()));
    connect(ui->diffExceuableFileButton, SIGNAL(clicked()), SLOT(onDiffFileOpenButtonClicked()));
    connect(ui->diffCommandAddButton, SIGNAL(clicked()), SLOT(onDiffAddButtonClicked()));
    connect(ui->diffCommandRemoveButton, SIGNAL(clicked()), SLOT(onDiffRemoveButtonClicked()));
    connect(ui->diffConfigComboBox, SIGNAL(currentIndexChanged(int)), SLOT(onDiffConfigChanged(int)));
    connect(ui->diffCommandEdit, SIGNAL(editingFinished()), SLOT(onDiffConfigEditedByUser()));
    connect(ui->diffArgumentsEdit, SIGNAL(editingFinished()), SLOT(onDiffConfigEditedByUser()));
    connect(ui->diffRunButton, SIGNAL(clicked()), SLOT(onRunButtonClicked()));
    loadSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onFileOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open opc pakcage"), m_lastPath, tr("Office files (*.xlsx *.docx *.pptx);;All files(*.*)"));
    if (fileName.isEmpty())
        return;
    m_lastPath = fileName;

    if (sender() == ui->file1Button)
        ui->fileName1Edit->setText(fileName);
    else
        ui->fileName2Edit->setText(fileName);

    ui->outputEdit->clear();
}

void MainWindow::onDiffFileOpenButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Diff Excutable"), m_lastPath);
    if (fileName.isEmpty())
        return;
    ui->diffCommandEdit->setText(fileName);

    onDiffConfigEditedByUser();
}

void MainWindow::onDiffAddButtonClicked()
{
    QString name = QInputDialog::getText(this, "Diff Config", "New diff config name", QLineEdit::Normal);
    if (name.isEmpty())
        return;
    ui->diffConfigComboBox->addItem(name, QStringList()<<name<<QString()<<"%1 %2");
    ui->diffConfigComboBox->setCurrentIndex(ui->diffConfigComboBox->count()-1);
}

void MainWindow::onDiffRemoveButtonClicked()
{
    ui->diffConfigComboBox->removeItem(ui->diffConfigComboBox->currentIndex());
}

void MainWindow::onDiffConfigChanged(int /*index*/)
{
    QStringList data = ui->diffConfigComboBox->currentData().toStringList();
    if (data.isEmpty()) {
        ui->diffCommandEdit->clear();
        ui->diffArgumentsEdit->clear();
        return;
    }
    ui->diffCommandEdit->setText(data[1]);
    ui->diffArgumentsEdit->setText(data[2]);
}

void MainWindow::onDiffConfigEditedByUser()
{
    ui->diffConfigComboBox->setItemData(ui->diffConfigComboBox->currentIndex(),
                                        QStringList() << ui->diffConfigComboBox->currentText()
                                        << ui->diffCommandEdit->text()
                                        << ui->diffArgumentsEdit->text());
}

void MainWindow::onFileOpenActionTriggered()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Select two opc pakcages that you want to diff"), m_lastPath, tr("Office files (*.xlsx *.docx *.pptx);;All files(*.*)"));
    if (fileNames.size() != 2)
        return;
    m_lastPath = fileNames[1];

    ui->fileName1Edit->setText(fileNames[0]);
    ui->fileName2Edit->setText(fileNames[1]);

    ui->outputEdit->clear();
}

void MainWindow::onAboutActionTriggered()
{
    QMessageBox::information(this, tr("About"), tr("This is a debug tool that can be used to find difference of the two opc packages."));
}

void MainWindow::onRunButtonClicked()
{
    ui->outputEdit->clear();

    QString fileName1 = ui->fileName1Edit->text();
    QString fileName2 = ui->fileName2Edit->text();
    if (fileName1.isEmpty() || fileName2.isEmpty()) {
        ui->outputEdit->appendPlainText("Error: Package name is empty.");
        return;
    }

    m_dir1 = QSharedPointer<QTemporaryDir>(new QTemporaryDir);
    m_dir2 = QSharedPointer<QTemporaryDir>(new QTemporaryDir);
    if (!(extractOpcPackage(fileName1, m_dir1->path()) && extractOpcPackage(fileName2, m_dir2->path()))) {
        //ui->outputEdit->appendPlainText("Error: ");
        return;
    }

    QStringList args = ui->diffArgumentsEdit->text().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    for (int i=0; i<args.size(); ++i) {
        if (args[i] == QLatin1String("%1"))
            args[i] = m_dir1->path();
        else if (args[i] == QLatin1String("%2"))
            args[i] = m_dir2->path();
    }

    startDiffProcess(ui->diffCommandEdit->text(), args);
}

void MainWindow::onRunProcessFinished()
{
    m_process->deleteLater();
    m_process = 0;
    ui->diffRunButton->setEnabled(true);
    qDebug("run process finished.");
}

void MainWindow::onRunProcessError()
{
    m_process->deleteLater();
    ui->outputEdit->appendPlainText(m_process->errorString());

    m_process = 0;
    ui->diffRunButton->setEnabled(true);
}

void MainWindow::onRunProcessReadyRead()
{
    if (!m_process)
        return;
    ui->outputEdit->moveCursor(QTextCursor::End);
    QString data = QString::fromLocal8Bit(m_process->readAll());

    //Relace the temp path with the real opc pakcage path.
    data.replace(m_dir1->path(), ui->fileName1Edit->text());
    data.replace(m_dir2->path(), ui->fileName2Edit->text());

    ui->outputEdit->insertPlainText(data);
}

void MainWindow::closeEvent(QCloseEvent *evt)
{
    saveSettings();
    evt->accept();
}

void MainWindow::loadSettings()
{
    QSettings settings;
    m_lastPath = settings.value("lastPath").toString();

    ui->diffCommandEdit->setText(settings.value("diff", QString("diff")).toString());
    ui->diffArgumentsEdit->setText(settings.value("diffArgs", QString("-r -c3 %1 %2")).toString());

    QList<QVariant> diffs = settings.value("diffs").toList();
    int diffIndex = settings.value("diffIndex", 0).toInt();
    if (diffs.isEmpty()) {
        diffs.append(QStringList()<<"Gnu Diff"<<"diff"<<"-r -c3 %1 %2");
        diffs.append(QStringList()<<"Kde Kompare"<<"kompare"<<"%1 %2");
        diffs.append(QStringList()<<"Gnome Meld"<<"meld"<<"%1 %2");
#ifdef Q_OS_WIN
        diffs.append(QStringList()<<"WinMerge"<<"WinMergeU"<<"/xq /r %1 %2");
#endif
    }
    foreach (QVariant diff, diffs)
        ui->diffConfigComboBox->addItem(diff.toStringList()[0], diff);
    if (diffIndex >= 0 && diffIndex < diffs.count())
        ui->diffConfigComboBox->setCurrentIndex(diffIndex);

    restoreGeometry(settings.value("geometry").toByteArray());
}

void MainWindow::saveSettings() const
{
    QSettings settings;
    settings.setValue("lastPath", m_lastPath);

    QList<QVariant> diffs;
    for (int i = 0; i < ui->diffConfigComboBox->count(); ++i)
        diffs.append(ui->diffConfigComboBox->itemData(i));
    settings.setValue("diffs", diffs);
    settings.setValue("diffIndex", ui->diffConfigComboBox->currentIndex());

    settings.setValue("geometry", saveGeometry());
}

bool MainWindow::extractOpcPackage(const QString &packageName, const QString &dir)
{
    QScopedPointer<Opc::Package> package(Opc::Package::open(packageName, QIODevice::ReadOnly));

    if (!package) {
        ui->outputEdit->appendPlainText(QString("Error: Can not open package %1").arg(packageName));
        return false;
    }

    foreach (Opc::PackagePart *part, package->parts()) {
        QString filePath = dir + part->partName();
        QDir().mkpath(QFileInfo(filePath).absolutePath());
        QFile desFile(filePath);
        if (!desFile.open(QIODevice::WriteOnly)) {
            ui->outputEdit->appendPlainText(QString("Error: Can not create files %1").arg(filePath));
            ui->outputEdit->appendPlainText(desFile.errorString());
            return false;
        }

        QIODevice *srcDevice = part->getDevice();
        if (part->contentType().endsWith("+xml")
                || part->contentType() == "text/xml"
                || part->contentType() == "application/xml"
                || part->contentType() == "text/xml-external-parsed-entity"
                || part->contentType() == "application/xml-external-parsed-entity"
                || part->contentType() == "application/xml-dtd" ) {
            copyFormattedXmlDataToFile(srcDevice, desFile);
        } else {
            copyRawDataToFile(srcDevice, desFile);
        }
        part->releaseDevice();

        desFile.close();
    }
    return true;
}

void MainWindow::copyRawDataToFile(QIODevice *srcDevice, QFile &desFile)
{
    const qint64 blockSize = 1024 * 1024;
    QByteArray block(blockSize, Qt::Uninitialized);
    qint64 remainingSize = srcDevice->size();
    while (remainingSize > 0) {
        const qint64 currentSize = qMin(blockSize, remainingSize);
        srcDevice->read(block.data(), currentSize);
        desFile.write(block.data(), currentSize);
        remainingSize -= currentSize;
    }
}

void MainWindow::copyFormattedXmlDataToFile(QIODevice *srcDevice, QFile &desFile)
{
    QXmlStreamReader reader(srcDevice);

    //QXmlStreamWriter used here to format the xml file data.
    QXmlStreamWriter writer(&desFile);
    writer.setAutoFormatting(true);

    while(!reader.atEnd()) {
        reader.readNext();
        if (!reader.isWhitespace() && reader.tokenType() != QXmlStreamReader::Invalid)
            Mce::writeCurrentToken(writer, reader);
    }
}

bool MainWindow::startDiffProcess(const QString &name, const QStringList &args)
{
    ui->diffRunButton->setEnabled(false);

    m_process = new QProcess(this);
    connect(m_process, SIGNAL(error(QProcess::ProcessError)), SLOT(onRunProcessError()));
    connect(m_process, SIGNAL(finished(int)), SLOT(onRunProcessFinished()));
    connect(m_process, SIGNAL(readyRead()), SLOT(onRunProcessReadyRead()));
    m_process->start(name, args);
    qDebug()<<QString("Run %1 with arguments").arg(name)<<args;

    return true;
}
