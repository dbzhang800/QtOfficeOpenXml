/*  This file is part of the KDE project

    Copyright (C) 2014 Maarten De Meyer <de.meyer.maarten@gmail.com>

    You may use this file under the terms of the BSD license as follows:

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

    1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
    IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
    OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
    IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
    NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
    THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * bzip2gzip
 * This example shows the usage of KCompressionDevice.
 * It converts BZip2 files to GZip archives.
 *
 * api: KCompressionDevice(QIODevice * inputDevice, bool autoDeleteInputDevice, CompressionType type)
 * api: KCompressionDevice(const QString & fileName, CompressionType type)
 * api: QIODevice::readAll()
 * api: QIODevice::read(qint64 maxSize)
 * api: QIODevice::write(const QByteArray &data)
 *
 * Usage: ./bzip2gzip <archive.bz2>
*/

#include <QCoreApplication>
#include <QStringList>
#include <QFile>
#include <QFileInfo>

#include <KCompressionDevice>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList args(app.arguments());

    if (args.size() != 2) {
        qWarning("Usage: ./bzip2gzip <archive.bz2>");
        return 1;
    }

    QString inputFile = args.at(1);
    QFile file(inputFile);
    QFileInfo info(inputFile);

    if (info.suffix() != QStringLiteral("bz2")) {
        qCritical("Error: not a valid BZip2 file!");
        return 1;
    }

    //@@snippet_begin(kcompressiondevice_example)
    // Open the input archive
    KCompressionDevice input(&file, false, KCompressionDevice::BZip2);
    input.open(QIODevice::ReadOnly);

    QString outputFile = (info.completeBaseName() + QStringLiteral(".gz"));

    // Open the new output file
    KCompressionDevice output(outputFile, KCompressionDevice::GZip);
    output.open(QIODevice::WriteOnly);

    while(!input.atEnd()) {
        // Read and uncompress the data
        QByteArray data = input.read(512);

        // Write data like you would to any other QIODevice
        output.write(data);
    }

    input.close();
    output.close();
    //@@snippet_end

    return 0;
}
