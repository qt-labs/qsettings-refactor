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
#include "cocoasettings.h"
#include <private/qcore_mac_p.h>
#include <Foundation/NSUserDefaults.h>

CocoaSettings::CocoaSettings(QObject *parent) : Settings(parent)
{
}

CocoaSettings::~CocoaSettings()
{
}

void CocoaSettings::remove(const QString &key)
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults removeObjectForKey: QCFString::toNSString(key)];
}

void CocoaSettings::set(const QString &key, const QVariant &value)
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    [defaults setObjectForKey: QCFString::toNSString(key)];
}

bool CocoaSettings::get(const QString &key, QVariant *value) const
{
    return false;
}

QStringList CocoaSettings::children(const QString &prefix, ChildSpec spec) const
{
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary *dict = [defaults dictionaryRepresentation];

    QStringList lst;
    for (id key in [dict allKeys])
        lst += QCFString::toQString(key);
    return lst;
}

void CocoaSettings::clear()
{
}

void CocoaSettings::sync()
{
    [[NSUserDefaults standardUserDefaults] synchronize];
}

void CocoaSettings::flush()
{
    sync();
}

bool CocoaSettings::isWritable() const
{
    return false;
}

QString CocoaSettings::fileName() const
{
    return QString();
}
