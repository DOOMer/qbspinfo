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

#include "config.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

const float configVersion = 0.99;
const QString JSON_KEY_VERSION = "version";
const QString JSON_KEY_GAME = "game";

const QByteArray APP_CONFIG_STORAGE_FILE = "qbspinfo.json";

#include <QDebug>

Config::Config(QObject *parent) :
    QObject(parent)
{
}

/*!
 * Load settings from the storage file.
 */
void Config::load()
{
	qDebug() << "load conf";
	QString configFilePath =  Config::configFilePath();
	
	QString in;
	
	QFile f(configFilePath);
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "read file ";
		in = f.readAll();
		f.close();
	}
	
	if (!in.isEmpty())
	{
		QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8());
		QJsonArray configJSON = doc.array();
		QJsonObject main = configJSON.at(2).toObject();
		_confData = main.value("main").toObject().toVariantMap();
	}
}

/*!
 * Save settings from the storage file.
 */
void Config::save()
{
	qDebug() << "save conf";
	QString configFilePath =  Config::configFilePath();
	
	// JSONed	
	QJsonValue game(QString("Quake 3"));
	
	QJsonObject confVersion;
	confVersion.insert(JSON_KEY_VERSION, QJsonValue(configVersion));
	QJsonObject gameName;
	gameName.insert(JSON_KEY_GAME, game);
	
	// main settings
	QJsonObject main;
	QJsonObject mainData;
	mainData = QJsonObject::fromVariantMap(_confData);
	main.insert("main", mainData);

	QJsonArray configJSON;
	configJSON.append(confVersion);
	configJSON.append(gameName);
	configJSON.append(main);
	
	// create JSON document and write it to the byte-array
	QJsonDocument doc(configJSON);
	QByteArray out = doc.toJson(QJsonDocument::Indented);
	
	QFile f(configFilePath);
	if (f.open(QIODevice::WriteOnly))
	{
		qDebug() << "write file ";
		f.write(out);
		f.close();
	}
}

/*!
 * Get path to the directory with maps. 
 * \return QString Return value create based on the "game directory" and "selected mod" configuration params.
 */
QString Config::mapsDir()
{
	QString dir;
	QString gameDir = _confData[KEY_GAME_PATH].toString();
	QString modDir = _confData[KEY_SELECTED_MOD].toString();
	dir = gameDir + QDir::toNativeSeparators(QDir::separator()) + modDir;
	dir += QDir::toNativeSeparators(QDir::separator()) + "maps";
	
	if (gameDir.isEmpty() || modDir.isEmpty())
	{
		qDebug() << "game dir error";
		Q_EMIT configurationError();
		return QString();
	}
	
	return dir;
}

/*!
 * Set the configuration parameter.
 * \param key QString Name of the parameter.
 * \param val QVariant Value of the parameter.
 */
void Config::setValue(const QString key, const QVariant& val)
{
	if (_confData.keys().indexOf(key) == -1)
	{
		_confData.insert(key, val);
	}
	else
	{
		_confData[key] = val;
	}
}

/*!
 * Get the configuration parameter. If key isn't exists, then return empty QVariant ojcet.'
 * \param key QString Name of the parameter.
 * \return QVariant Value of the parameter.
 */
QVariant Config::value(const QString key)
{
	if (_confData.keys().indexOf(key) == -1)
	{
		return QVariant();
	}
	else
	{
		return _confData[key];
	}
}

/*!
 * Get path to the file file with settings. 
 * \return QString Return value is 'ConfigDir" value and file name.'
 */
QString Config::configFilePath()
{
	QString cf = configDir() + APP_CONFIG_STORAGE_FILE;
	return cf;
}

/*!
 * Get path to the directory for storage file with settings. 
 * \return QString Return value based on the XDG_CONFIG_HOME env variable and yje app name..
 */
QString Config::configDir()
{
	QString confDir;
	
	confDir = qgetenv("XDG_CONFIG_HOME");
        
	// Ubuntu hack -- if XDG_CONFIG_HOME is missing
    if (confDir.isEmpty() == true)
    {
		confDir = QDir::homePath();
        confDir += QDir::separator();
        confDir += ".config";
	}
        
	confDir.append(QDir::separator());
	confDir.append("qbspinfo");
	confDir.append(QDir::separator());
	
	// check exists conf directory
	if (!QFile::exists(confDir))
	{
		QDir mkDir(confDir);
		mkDir.mkpath(mkDir.path());
	}
	
	return confDir;
}

/*!
 * Get system language (locale)
 * \return QString Return locale value.
 */
QString Config::Config::getSysLang()
{
// #ifdef Q_WS_X11
    QByteArray lang = qgetenv("LC_ALL");

    if (lang.isEmpty())
    {
        lang = qgetenv("LC_MESSAGES");
    }
    if (lang.isEmpty())
    {
        lang = qgetenv("LANG");
    }
    if (!lang.isEmpty())
    {
        return QLocale (lang).name();
    }
    else
    {
        return QLocale::system().name();
    }
// #endif
// #ifdef Q_WS_WIN
//     return QLocale::system().name();
// #endif
}
