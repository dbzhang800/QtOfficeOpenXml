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

#include "klimitediodevicetest.h"

#include "klimitediodevice_p.h"

#include <QtTest/QtTest>

QTEST_MAIN(KLimitedIODeviceTest)

void KLimitedIODeviceTest::addChunk(const QByteArray &chunk)
{
    ChunkData cd;
    cd.data = chunk;
    cd.offset = m_chunks.isEmpty() ? 0 : m_chunks.last().offset + m_chunks.last().data.size();
    m_chunks.append(cd);
    m_data.append(chunk);
}

void KLimitedIODeviceTest::initTestCase()
{
    addChunk("Test of string");
    addChunk("second part of the large buffer");
    addChunk("... which will be used to test the KLimitedIODevice");

    m_buffer.setBuffer(&m_data);
    m_buffer.open(QIODevice::ReadOnly);
}

void KLimitedIODeviceTest::testReadChunks_data()
{
    QTest::addColumn<int>("index");

    for (int i = 0; i < m_chunks.count(); ++i) {
        const ChunkData &d = m_chunks.at(i);
        QTest::newRow(d.data.constData()) << i;
    }
}

void KLimitedIODeviceTest::testReadChunks()
{
    QFETCH(int, index);

    const ChunkData &chunk = m_chunks.at(index);

    KLimitedIODevice dev(&m_buffer, chunk.offset, chunk.data.size());
    QVERIFY(dev.isOpen());
    QCOMPARE(dev.readAll(), chunk.data);
}

void KLimitedIODeviceTest::testSeeking()
{
    const ChunkData &chunk = m_chunks.at(2);

    KLimitedIODevice dev(&m_buffer, chunk.offset, chunk.data.size());
    QVERIFY(dev.seek(dev.size() - 16));
    QCOMPARE(dev.readAll(), chunk.data.right(16));
    QVERIFY(dev.seek(0));
    QCOMPARE(dev.readAll(), chunk.data);
}

