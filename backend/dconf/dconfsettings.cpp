/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is prototype code for the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "dconfsettings.h"
#include <qvariant.h>
#include <qdebug.h>
extern "C" {
#include <dconf/dconf.h>
}

DConfSettings::DConfSettings(QObject *parent) : Settings(parent)
{
    client = dconf_client_new();
    g_object_ref(client);

    g_signal_connect(G_OBJECT(client), "changed", G_CALLBACK(changeCallback), this);

    dconf_client_watch_fast(client, "/org/qt-project/"); // TODO
}

DConfSettings::~DConfSettings()
{
    dconf_client_unwatch_fast(client, "/org/qt-project/"); // TODO

    g_object_unref(client);
}

void DConfSettings::remove(const QString &key)
{
    set(key, QVariant());
}

void DConfSettings::set(const QString &key, const QVariant &value)
{
    GVariant *var = toGVariant(value);
    dconf_client_write_fast(client, key.toUtf8(), var, NULL);
}

bool DConfSettings::get(const QString &key, QVariant *value) const
{
    GVariant *var = dconf_client_read(client, key.toUtf8());
    if (var) {
        if (value)
            *value = toQVariant(var);
        g_variant_unref(var);
        return true;
    }
    return false;
}

QStringList DConfSettings::children(const QString &prefix, ChildSpec spec) const
{
    QString pfx = prefix;
    if (!pfx.endsWith(QLatin1Char('/')))
        pfx.append(QLatin1Char('/'));

    QStringList lst;
    gint len = 0;

    gchar **arr = dconf_client_list(client, pfx.toUtf8(), &len);
    for (int i = 0; i < len; ++i) {
        const QString value = QString::fromUtf8(arr[i]);
        const QByteArray path = QString(pfx + value).toUtf8();
        if (spec == AllKeys || (spec == ChildKeys && dconf_is_key(path, NULL)) || (spec == ChildGroups && dconf_is_dir(path, NULL)))
            lst += value;
    }
    g_strfreev(arr);
    return lst;
}

void DConfSettings::clear()
{
}

void DConfSettings::sync()
{
    dconf_client_sync(client);
}

void DConfSettings::flush()
{
    sync();
}

bool DConfSettings::isWritable() const
{
    return dconf_client_is_writable(client, NULL);
}

QString DConfSettings::fileName() const
{
    return QString();
}

QVariant DConfSettings::toQVariant(GVariant *value)
{
    if (!value)
        return QVariant();

    switch (g_variant_classify(value)) {
    case G_VARIANT_CLASS_BOOLEAN:
        return QVariant::fromValue(g_variant_get_boolean(value));
    case G_VARIANT_CLASS_BYTE:
        return QVariant::fromValue(g_variant_get_byte(value));
    case G_VARIANT_CLASS_INT16:
        return QVariant::fromValue(g_variant_get_int16(value));
    case G_VARIANT_CLASS_UINT16:
        return QVariant::fromValue(g_variant_get_uint16(value));
    case G_VARIANT_CLASS_INT32:
        return QVariant::fromValue(g_variant_get_int32(value));
    case G_VARIANT_CLASS_UINT32:
        return QVariant::fromValue(g_variant_get_uint32(value));
    case G_VARIANT_CLASS_INT64:
        return QVariant::fromValue(g_variant_get_int64(value));
    case G_VARIANT_CLASS_UINT64:
        return QVariant::fromValue(g_variant_get_uint64(value));
    case G_VARIANT_CLASS_DOUBLE:
        return QVariant::fromValue(g_variant_get_double(value));
    case G_VARIANT_CLASS_STRING:
    case G_VARIANT_CLASS_OBJECT_PATH:
    case G_VARIANT_CLASS_SIGNATURE:
        return QString::fromUtf8(g_variant_get_string(value, NULL));
    // TODO:
    case G_VARIANT_CLASS_HANDLE:
    case G_VARIANT_CLASS_VARIANT:
    case G_VARIANT_CLASS_MAYBE:
    case G_VARIANT_CLASS_ARRAY:
    case G_VARIANT_CLASS_TUPLE:
    case G_VARIANT_CLASS_DICT_ENTRY:
    default:
        return QVariant();
    }
}

GVariant* DConfSettings::toGVariant(const QVariant& value)
{
    if (!value.isValid())
        return NULL;

    switch (value.type()) {
    case QVariant::Invalid:
        return NULL;
    case QVariant::Bool:
        return g_variant_new_boolean(value.toBool());
    case QVariant::Int:
        return g_variant_new_int32(value.toInt());
    case QVariant::UInt:
        return g_variant_new_uint32(value.toUInt());
    case QVariant::LongLong:
        return g_variant_new_int64(value.toLongLong());
    case QVariant::ULongLong:
        return g_variant_new_uint64(value.toULongLong());
    case QVariant::Double:
        return g_variant_new_double(value.toDouble());
    case QVariant::Char:
        return g_variant_new_byte(value.toChar().toLatin1());
    case QVariant::String:
        return g_variant_new_string(value.toString().toUtf8());
    case QVariant::ByteArray:
        return g_variant_new_bytestring(value.toByteArray());
    case QVariant::Map:
    case QVariant::List:
    case QVariant::StringList:
    case QVariant::BitArray:
    case QVariant::Date:
    case QVariant::Time:
    case QVariant::DateTime:
    case QVariant::Url:
    case QVariant::Locale:
    case QVariant::Rect:
    case QVariant::RectF:
    case QVariant::Size:
    case QVariant::SizeF:
    case QVariant::Line:
    case QVariant::LineF:
    case QVariant::Point:
    case QVariant::PointF:
    case QVariant::RegExp:
    case QVariant::RegularExpression:
    case QVariant::Hash:
    case QVariant::EasingCurve:
    case QVariant::Uuid:
    case QVariant::ModelIndex:

    case QVariant::Font:
    case QVariant::Pixmap:
    case QVariant::Brush:
    case QVariant::Color:
    case QVariant::Palette:
    case QVariant::Image:
    case QVariant::Polygon:
    case QVariant::Region:
    case QVariant::Bitmap:
    case QVariant::Cursor:
    case QVariant::KeySequence:
    case QVariant::Pen:
    case QVariant::TextLength:
    case QVariant::TextFormat:
    case QVariant::Matrix:
    case QVariant::Transform:
    case QVariant::Matrix4x4:
    case QVariant::Vector2D:
    case QVariant::Vector3D:
    case QVariant::Vector4D:
    case QVariant::Quaternion:
    case QVariant::PolygonF:
    case QVariant::Icon:

    case QVariant::SizePolicy:

    default:
        return NULL;
    }
}

void DConfSettings::changeCallback(DConfClient *client, char *prefix, char **changes, char *tag, void *userData)
{
    Q_UNUSED(client);
    Q_UNUSED(tag);

    DConfSettings *settings = static_cast<DConfSettings*>(userData);
    while (settings && changes && changes[0]) {
        const QString key = QString::fromUtf8(changes[0]);
        QVariant value;
        settings->get(prefix, &value);
        qDebug() << Q_FUNC_INFO << prefix << key << value;
        Q_EMIT settings->changed(key, value);
        ++changes;
    }
}
