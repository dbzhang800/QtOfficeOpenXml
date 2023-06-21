/* This file is part of the KDE libraries
   Copyright (C) 2000-2005 David Faure <faure@kde.org>
   Copyright (C) 2003 Leo Savernik <l.savernik@aon.at>

   Moved from ktar.h by Roberto Teixeira <maragato@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/
#ifndef KARCHIVEENTRY_H
#define KARCHIVEENTRY_H

#include <sys/stat.h>
#include <sys/types.h>

#include <karchive_export.h>
#include <QDateTime>

#ifdef Q_OS_WIN
#include <qplatformdefs.h> // mode_t
#endif

class KArchiveDirectory;
class KArchiveFile;

class KArchiveEntryPrivate;
/**
 * A base class for entries in an KArchive.
 * @short Base class for the archive-file's directory structure.
 *
 * @see KArchiveFile
 * @see KArchiveDirectory
 */
class KARCHIVE_EXPORT KArchiveEntry
{
public:
    /**
     * Creates a new entry.
     * @param archive the entries archive
     * @param name the name of the entry
     * @param access the permissions in unix format
     * @param date the date (in seconds since 1970)
     * @param user the user that owns the entry
     * @param group the group that owns the entry
     * @param symlink the symlink, or QString()
     */
    KArchiveEntry(KArchive *archive, const QString &name, int access, const QDateTime &date,
                  const QString &user, const QString &group,
                  const QString &symlink);

    virtual ~KArchiveEntry();

    /**
     * Creation date of the file.
     * @return the creation date
     */
    QDateTime date() const;

    /**
     * Name of the file without path.
     * @return the file name without path
     */
    QString name() const;
    /**
     * The permissions and mode flags as returned by the stat() function
     * in st_mode.
     * @return the permissions
     */
    mode_t permissions() const;
    /**
     * User who created the file.
     * @return the owner of the file
     */
    QString user() const;
    /**
     * Group of the user who created the file.
     * @return the group of the file
     */
    QString group() const;

    /**
     * Symlink if there is one.
     * @return the symlink, or QString()
     */
    QString symLinkTarget() const;

    /**
     * Checks whether the entry is a file.
     * @return true if this entry is a file
     */
    virtual bool isFile() const;

    /**
     * Checks whether the entry is a directory.
     * @return true if this entry is a directory
     */
    virtual bool isDirectory() const;

protected:
    KArchive *archive() const;

protected:
    virtual void virtual_hook(int id, void *data);
private:
    KArchiveEntryPrivate *const d;
};

#endif
