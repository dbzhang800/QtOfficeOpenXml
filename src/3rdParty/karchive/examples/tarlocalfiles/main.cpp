/*  This file is part of the KDE project

    Copyright (C) 2013 Maarten De Meyer <de.meyer.maarten@gmail.com>

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
 * TarLocalFiles
 * This example shows how to add local files and directories to a KArchive
 *
 * api: addLocalFile(fileName, destName)
 * api: addLocalDirectory(dirName, destName)
 *
 * Usage: ./tarlocalfiles <file-1> <file-n>
*/

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

#include <ktar.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList files(app.arguments());

    // Create or open an archive
    KTar archive(QStringLiteral("myFiles.tar.gz"));

    // Prepare the archive for writing.
    if (!archive.open(QIODevice::WriteOnly)) {
        // Failed to open file.
        return 1;
    }

    if (files.size() <= 1) {
        // No files given.
        qWarning("Usage: ./tarlocalfiles <file>");
        return 1;
    }

    for (int i = 1; i < files.size(); ++i) {
        QFileInfo localFileOrDir(files.at(i));

        if (localFileOrDir.isFile()) {
            QString name = localFileOrDir.fileName();
            archive.addLocalFile(name, name);
        } else if (localFileOrDir.isDir()) {
            QString name = QDir(files.at(i)).dirName();
            // Add this folder and all its contents
            archive.addLocalDirectory(name, name);
        }
    }

    archive.close();
    return 0;
}
