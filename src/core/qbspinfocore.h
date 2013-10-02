/***************************************************************************
* Copyright (C) 2013 by Artem 'DOOMer' Galichkin *
* doomer3d@gmail.com *
* *
* This program is free software; you can redistribute it and/or modify *
* it under the terms of the GNU General Public License as published by *
* the Free Software Foundation; either version 2 of the License, or *
* (at your option) any later version. *
* *
* This program is distributed in the hope that it will be useful, *
* but WITHOUT ANY WARRANTY; without even the implied warranty of *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the *
* GNU General Public License for more details. *
* *
* You should have received a copy of the GNU General Public License *
* along with this program; if not, write to the *
* Free Software Foundation, Inc., *
* 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA. *
***************************************************************************/

#ifndef QBSPINFOCORE_H
#define QBSPINFOCORE_H

#include <QtCore/QObject>
#include <QtCore/QStringList>

#include "core/config.h"

namespace Core {
	enum ErrorCode {
		none = 0, 
		notBspFile = 1,
		notQ3Bsp = 2, 
		notConfigured = 12
	};
};

class QBspInfoCore : public QObject
{
    Q_OBJECT
public:
    explicit QBspInfoCore(QObject *parent = 0);
	~QBspInfoCore();
	Config* config() const;
	bool openBspFile(const QString& filename);
	// TODO replace QByteArray on the Model
	QByteArray entities();
	// TODO replace QStringList on the Model
	QStringList resources();

public Q_SLOTS:
	void showSettings();

private Q_SLOTS:
	void slotMessageToUser();
	
Q_SIGNALS:
	void messageToUser(const QString& title, const QString& message);
	
private:
	Config *_conf;
	Core::ErrorCode _errorCode;
	
	// TODO replace QStringList on the Model
	QStringList _resources;
	
	// TODO replace QByteArray on the Model
	QByteArray _entities; 
};

#endif // QBSPINFOCORE_H
