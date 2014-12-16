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
 * Unzipper
 * This example shows how to extract all files from a zip archive.
 *
 * api: KArchive::directory()
 * api: KArchiveDirectory::copyTo(QString destination, bool recursive)
 *
 * Usage: ./unzipper <archive>
*/

#include <QCoreApplication>
#include <QDir>

#include <kzip.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QStringList args(app.arguments());

    if (args.size() != 2) {
        // Too many or too few arguments
        qWarning("Usage: ./unzipper <archive.zip>");
        return 1;
    }

    QString file = args.at(1);
    KZip archive(file);

    // Open the archive
    if (!archive.open(QIODevice::ReadOnly)) {
        qWarning("Cannot open " + file.toLatin1());
        qWarning("Is it a valid zip file?");
        return 1;
    }

    // Take the root folder from the archive and create a KArchiveDirectory object.
    // KArchiveDirectory represents a directory in a KArchive.
    const KArchiveDirectory *root = archive.directory();

    // We can extract all contents from a KArchiveDirectory to a destination.
    // recursive true will also extract subdirectories.
    QString destination = QDir::currentPath();
    bool recursive = true;
    root->copyTo(destination, recursive);

    archive.close();
    return 0;
}
