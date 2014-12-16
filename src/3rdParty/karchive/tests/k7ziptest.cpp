/*
 *  Copyright (C) 2011 Mario Bensi <mbensi@ipsquad.net>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License version 2 as published by the Free Software Foundation;
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#include "k7zip.h"
#include <stdio.h>
#include <QtCore/QDebug>

void recursive_print(const KArchiveDirectory *dir, const QString &path)
{
    QStringList l = dir->entries();
    l.sort();
    QStringList::ConstIterator it = l.constBegin();
    for (; it != l.constEnd(); ++it) {
        const KArchiveEntry *entry = dir->entry((*it));
        printf("mode=%07o %s %s %s %s%s %lld isdir=%d\n", entry->permissions(), entry->date().toString("yyyy-MM-dd hh:mm:ss").toLatin1().constData(),
               entry->user().toLatin1().constData(), entry->group().toLatin1().constData(), path.toLatin1().constData(), (*it).toLatin1().constData(),
               entry->isFile() ? static_cast<const KArchiveFile *>(entry)->size() : 0,
               entry->isDirectory());
        if (!entry->symLinkTarget().isEmpty()) {
            printf("  (symlink to %s)\n", qPrintable(entry->symLinkTarget()));
        }
        if (entry->isDirectory()) {
            recursive_print((KArchiveDirectory *)entry, path + (*it) + '/');
        }
        if (entry->isFile()) {
            const KArchiveFile *f = static_cast<const KArchiveFile *>(entry);
            QByteArray arr(f->data());
            qDebug() << "data" << arr;

            QIODevice *dev = f->createDevice();
            QByteArray contents = dev->readAll();
            qDebug() << "contents" << contents;
            delete dev;
        }
    }
}

// See karchivetest.cpp for the unittest that covers K7Zip.

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("\n"
               " Usage :\n"
               " ./k7ziptest /path/to/existing_file.7z       tests listing an existing .7z\n");
        return 1;
    }

    K7Zip k7z(argv[1]);

    if (!k7z.open(QIODevice::ReadOnly)) {
        printf("Could not open %s for reading\n", argv[1]);
        return 1;
    }

    const KArchiveDirectory *dir = k7z.directory();

    //printf("calling recursive_print\n");
    recursive_print(dir, "");
    //printf("recursive_print called\n");

    k7z.close();

    return 0;
}

