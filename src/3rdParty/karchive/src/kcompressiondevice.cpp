/* This file is part of the KDE libraries
   Copyright (C) 2000 David Faure <faure@kde.org>
   Copyright (C) 2011 Mario Bensi <mbensi@ipsquad.net>

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

#include "kcompressiondevice.h"
#include <config-compression.h>
#include "kfilterbase.h"
#include <QtCore/QFile>
#include <stdio.h> // for EOF
#include <stdlib.h>
#include <assert.h>

#include "kgzipfilter.h"
#include "knonefilter.h"

#if HAVE_BZIP2_SUPPORT
#include "kbzip2filter.h"
#endif
#if HAVE_XZ_SUPPORT
#include "kxzfilter.h"
#endif

#define BUFFER_SIZE 8*1024

#include <QDebug>

class KCompressionDevice::Private
{
public:
    Private() : bNeedHeader(true), bSkipHeaders(false),
        bOpenedUnderlyingDevice(false),
        bIgnoreData(false),
        result(KFilterBase::Result()), filter(nullptr), type(KCompressionDevice::None)  {}
    bool bNeedHeader;
    bool bSkipHeaders;
    bool bOpenedUnderlyingDevice;
    bool bIgnoreData;
    QByteArray buffer; // Used as 'input buffer' when reading, as 'output buffer' when writing
    QByteArray origFileName;
    KFilterBase::Result result;
    KFilterBase *filter;
    KCompressionDevice::CompressionType type;
};

KFilterBase *KCompressionDevice::filterForCompressionType(KCompressionDevice::CompressionType type)
{
    switch (type) {
    case KCompressionDevice::GZip:
        return new KGzipFilter;
        break;
    case KCompressionDevice::BZip2:
#if HAVE_BZIP2_SUPPORT
        return new KBzip2Filter;
#else
        return 0;
#endif
        break;
    case KCompressionDevice::Xz:
#if HAVE_XZ_SUPPORT
        return new KXzFilter;
#else
        return 0;
#endif
        break;
    case KCompressionDevice::None:
        return new KNoneFilter;
    }
    return 0;
}

KCompressionDevice::KCompressionDevice(QIODevice *inputDevice, bool autoDeleteInputDevice, CompressionType type)
    : d(new Private)
{
    assert(inputDevice);
    d->filter = filterForCompressionType(type);
    if (d->filter) {
        d->type = type;
        d->filter->setDevice(inputDevice, autoDeleteInputDevice);
    }
}

KCompressionDevice::KCompressionDevice(const QString &fileName, CompressionType type)
    : d(new Private)
{
    QFile *f = new QFile(fileName);
    d->filter = filterForCompressionType(type);
    if (d->filter) {
        d->type = type;
        d->filter->setDevice(f, true);
    } else {
        delete f;
    }
}

KCompressionDevice::~KCompressionDevice()
{
    if (isOpen()) {
        close();
    }
    delete d->filter;
    delete d;
}

KCompressionDevice::CompressionType KCompressionDevice::compressionType() const
{
    return d->type;
}

bool KCompressionDevice::open(QIODevice::OpenMode mode)
{
    if (isOpen()) {
        //qWarning() << "KCompressionDevice::open: device is already open";
        return true; // QFile returns false, but well, the device -is- open...
    }
    //qDebug() << mode;
    if (mode == QIODevice::ReadOnly) {
        d->buffer.resize(0);
    } else {
        d->buffer.resize(BUFFER_SIZE);
        d->filter->setOutBuffer(d->buffer.data(), d->buffer.size());
    }
    d->bNeedHeader = !d->bSkipHeaders;
    d->filter->setFilterFlags(d->bSkipHeaders ? KFilterBase::NoHeaders : KFilterBase::WithHeaders);
    if (!d->filter->init(mode)) {
        return false;
    }
    d->bOpenedUnderlyingDevice = !d->filter->device()->isOpen();
    bool ret = d->bOpenedUnderlyingDevice ? d->filter->device()->open(mode) : true;
    d->result = KFilterBase::Ok;

    if (!ret) {
        //qWarning() << "KCompressionDevice::open: Couldn't open underlying device";
    } else {
        setOpenMode(mode);
    }

    return ret;
}

void KCompressionDevice::close()
{
    if (!isOpen()) {
        return;
    }
    if (d->filter->mode() == QIODevice::WriteOnly) {
        write(0L, 0);    // finish writing
    }
    //qDebug() << "Calling terminate().";

    if (!d->filter->terminate()) {
        //qWarning() << "KCompressionDevice::close: terminate returned an error";
    }
    if (d->bOpenedUnderlyingDevice) {
        d->filter->device()->close();
    }
    setOpenMode(QIODevice::NotOpen);
}

bool KCompressionDevice::seek(qint64 pos)
{
    qint64 ioIndex = this->pos(); // current position
    if (ioIndex == pos) {
        return true;
    }

    //qDebug() << "seek(" << pos << ") called";

    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);

    if (pos == 0) {
        // We can forget about the cached data
        d->bNeedHeader = !d->bSkipHeaders;
        d->result = KFilterBase::Ok;
        d->filter->setInBuffer(0L, 0);
        d->filter->reset();
        QIODevice::seek(pos);
        return d->filter->device()->reset();
    }

    if (ioIndex > pos) { // we can start from here
        pos = pos - ioIndex;
    } else {
        // we have to start from 0 ! Ugly and slow, but better than the previous
        // solution (KTarGz was allocating everything into memory)
        if (!seek(0)) { // recursive
            return false;
        }
    }

    //qDebug() << "reading " << pos << " dummy bytes";
    QByteArray dummy(qMin(pos, (qint64)3 * BUFFER_SIZE), 0);
    d->bIgnoreData = true;
    bool result = (read(dummy.data(), pos) == pos);
    d->bIgnoreData = false;
    QIODevice::seek(pos);
    return result;
}

bool KCompressionDevice::atEnd() const
{
    return (d->type == KCompressionDevice::None || d->result == KFilterBase::End)
           && QIODevice::atEnd() // take QIODevice's internal buffer into account
           && d->filter->device()->atEnd();
}

qint64 KCompressionDevice::readData(char *data, qint64 maxlen)
{
    Q_ASSERT(d->filter->mode() == QIODevice::ReadOnly);
    //qDebug() << "maxlen=" << maxlen;
    KFilterBase *filter = d->filter;

    uint dataReceived = 0;

    // We came to the end of the stream
    if (d->result == KFilterBase::End) {
        return dataReceived;
    }

    // If we had an error, return -1.
    if (d->result != KFilterBase::Ok) {
        return -1;
    }

    qint64 outBufferSize;
    if (d->bIgnoreData) {
        outBufferSize = qMin(maxlen, (qint64)3 * BUFFER_SIZE);
    } else {
        outBufferSize = maxlen;
    }
    outBufferSize -= dataReceived;
    qint64 availOut = outBufferSize;
    filter->setOutBuffer(data, outBufferSize);

    while (dataReceived < maxlen) {
        if (filter->inBufferEmpty()) {
            // Not sure about the best size to set there.
            // For sure, it should be bigger than the header size (see comment in readHeader)
            d->buffer.resize(BUFFER_SIZE);
            // Request data from underlying device
            int size = filter->device()->read(d->buffer.data(),
                                              d->buffer.size());
            //qDebug() << "got" << size << "bytes from device";
            if (size) {
                filter->setInBuffer(d->buffer.data(), size);
            } else {
                // Not enough data available in underlying device for now
                break;
            }
        }
        if (d->bNeedHeader) {
            (void) filter->readHeader();
            d->bNeedHeader = false;
        }

        d->result = filter->uncompress_();

        if (d->result == KFilterBase::Error) {
            //qWarning() << "KCompressionDevice: Error when uncompressing data";
            break;
        }

        // We got that much data since the last time we went here
        uint outReceived = availOut - filter->outBufferAvailable();
        //qDebug() << "avail_out = " << filter->outBufferAvailable() << " result=" << d->result << " outReceived=" << outReceived;
        if (availOut < (uint)filter->outBufferAvailable()) {
            //qWarning() << " last availOut " << availOut << " smaller than new avail_out=" << filter->outBufferAvailable() << " !";
        }

        dataReceived += outReceived;
        if (!d->bIgnoreData) {  // Move on in the output buffer
            data += outReceived;
            availOut = maxlen - dataReceived;
        } else if (maxlen - dataReceived < outBufferSize) {
            availOut = maxlen - dataReceived;
        }
        if (d->result == KFilterBase::End) {
            //qDebug() << "got END. dataReceived=" << dataReceived;
            break; // Finished.
        }
        filter->setOutBuffer(data, availOut);
    }

    return dataReceived;
}

qint64 KCompressionDevice::writeData(const char *data /*0 to finish*/, qint64 len)
{
    KFilterBase *filter = d->filter;
    Q_ASSERT(filter->mode() == QIODevice::WriteOnly);
    // If we had an error, return 0.
    if (d->result != KFilterBase::Ok) {
        return 0;
    }

    bool finish = (data == 0L);
    if (!finish) {
        filter->setInBuffer(data, len);
        if (d->bNeedHeader) {
            (void)filter->writeHeader(d->origFileName);
            d->bNeedHeader = false;
        }
    }

    uint dataWritten = 0;
    uint availIn = len;
    while (dataWritten < len || finish) {

        d->result = filter->compress_(finish);

        if (d->result == KFilterBase::Error) {
            //qWarning() << "KCompressionDevice: Error when compressing data";
            // What to do ?
            break;
        }

        // Wrote everything ?
        if (filter->inBufferEmpty() || (d->result == KFilterBase::End)) {
            // We got that much data since the last time we went here
            uint wrote = availIn - filter->inBufferAvailable();

            //qDebug() << " Wrote everything for now. avail_in=" << filter->inBufferAvailable() << "result=" << d->result << "wrote=" << wrote;

            // Move on in the input buffer
            data += wrote;
            dataWritten += wrote;

            availIn = len - dataWritten;
            //qDebug() << " availIn=" << availIn << "dataWritten=" << dataWritten << "pos=" << pos();
            if (availIn > 0) {
                filter->setInBuffer(data, availIn);
            }
        }

        if (filter->outBufferFull() || (d->result == KFilterBase::End) || finish) {
            //qDebug() << " writing to underlying. avail_out=" << filter->outBufferAvailable();
            int towrite = d->buffer.size() - filter->outBufferAvailable();
            if (towrite > 0) {
                // Write compressed data to underlying device
                int size = filter->device()->write(d->buffer.data(), towrite);
                if (size != towrite) {
                    //qWarning() << "KCompressionDevice::write. Could only write " << size << " out of " << towrite << " bytes";
                    return 0; // indicate an error (happens on disk full)
                }
                //else
                //qDebug() << " wrote " << size << " bytes";
            }
            if (d->result == KFilterBase::End) {
                Q_ASSERT(finish); // hopefully we don't get end before finishing
                break;
            }
            d->buffer.resize(BUFFER_SIZE);
            filter->setOutBuffer(d->buffer.data(), d->buffer.size());
        }
    }

    return dataWritten;
}

void KCompressionDevice::setOrigFileName(const QByteArray &fileName)
{
    d->origFileName = fileName;
}

void KCompressionDevice::setSkipHeaders()
{
    d->bSkipHeaders = true;
}

KFilterBase *KCompressionDevice::filterBase()
{
    return d->filter;
}
