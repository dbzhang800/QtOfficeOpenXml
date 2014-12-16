/* This file is part of the KDE project
   Copyright (C) 2009 Pino Toscano <pino@kde.org>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef KLIMITEDIODEVICETEST_H
#define KLIMITEDIODEVICETEST_H

#include <QByteArray>
#include <QBuffer>
#include <QList>
#include <QObject>

struct ChunkData {
    QByteArray data;
    int offset;
};

class KLimitedIODeviceTest : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void initTestCase();

    void testReadChunks_data();
    void testReadChunks();
    void testSeeking();

private:
    void addChunk(const QByteArray &chunk);

    QByteArray m_data;
    QBuffer m_buffer;
    QList<ChunkData> m_chunks;
};

#endif
