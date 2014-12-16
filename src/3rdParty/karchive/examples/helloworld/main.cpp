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
 * HelloWorld
 *
 * Example to show very basic usage of KArchive with CMake
 *
 * Usage:
 *      mkdir build && cd build
 *      cmake ..
 *      make
 *      ./helloworld
*/

#include <QDebug>
#include <kzip.h>

int main()
{
    //@@snippet_begin(helloworld)
    // Create a zip archive
    KZip archive(QStringLiteral("hello.zip"));

    // Open our archive for writing
    if (archive.open(QIODevice::WriteOnly)) {

        // The archive is open, we can now write data
        archive.writeFile(QStringLiteral("world"),                       // File name
                          QByteArray("The whole world inside a hello."), // Data
                          0100644,                                       // Permissions
                          QStringLiteral("owner"),                       // Owner
                          QStringLiteral("users"));                      // Group

        // Don't forget to close!
        archive.close();
    }

    if (archive.open(QIODevice::ReadOnly)) {
        const KArchiveDirectory *dir = archive.directory();

        const KArchiveEntry *e = dir->entry("world");
    if (!e) {
      qDebug() << "File not found!";
      return -1;
    }
    const KArchiveFile *f = static_cast<const KArchiveFile *>(e);
    QByteArray arr(f->data());
    qDebug() << arr; // the file contents

    // To avoid reading everything into memory in one go, we can use createDevice() instead
    QIODevice *dev = f->createDevice();
    while (!dev->atEnd()) {
        qDebug() << dev->readLine();
    }
    delete dev;
    }
   //@@snippet_end

    return 0;
}
