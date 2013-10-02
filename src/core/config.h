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

#ifndef CONFING_H
#define CONFING_H

#include <QObject>
#include <QtCore/QVariantMap>

const QString KEY_GAME_PATH = "gamePath";
const QString KEY_SELECTED_MOD = "selectedMod";

class Config : public QObject
{
    Q_OBJECT
public:
    explicit Config(QObject *parent = 0);
	void load();
	void save();
	QString mapsDir();
	QVariant value(const QString key);
	void setValue(const QString key, const QVariant& val);
	QString configFilePath();
	QString getSysLang();
	
Q_SIGNALS:
	void configurationError();

private:
	QVariantMap _confData;
	QString configDir();
};

#endif // CONFING_H
