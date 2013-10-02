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

#include "qbspinfocore.h"

#include "ui/configdialog.h"

#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <cstdio>
#include <string>

#include <QDebug>

const QByteArray BSP_HEADER_Q3 = "IBSP";
const quint8 BSP_VERSION_Q3 = 0x2e; // 46
const int BSP_LUMP_ENTITIES = 0x00; 
const int BSP_LUMP_TEXTURES = 0x01;

/*!
 *  Lump structure
 */
struct Q3Lump {
	int offaet; // offset at fil
	int length; // lenght of lump
};

/*!
 * Header of the Q3 map.
 */
struct Q3Header {
	char bspHead[4]; 
	int bspVersion;
	Q3Lump lumps[17];
};

/*!
 * The entities lump stores game-related map information, including information about the map name, weapons, health, armor, triggers, spawn points, lights, and .md3 models to be placed in the map. 
 */

struct Q3Entity {
	int size; // Size of the description
	char* description; // Entity descriptions, stored as a string
};

/*!
 * The textures lump stores information about surfaces and volumes, which are in turn associated with faces, brushes, and brushsides. 
 */
struct Q3Texture {
	char texName[64];
	int surfaceFlags;
	int contentFlags;
};

/*
 * QBspinfo constructore
 * \param QObjcet paren object,
 * Create  the core object for linking the components of the application.
 */
QBspInfoCore::QBspInfoCore(QObject *parent) :
    QObject(parent), _conf(new Config(this))
{
	_errorCode = Core::none;
	_conf->setObjectName("config");
	_conf->load();
	connect(_conf, SIGNAL(configurationError()), this, SLOT(slotMessageToUser()));
}

/*!
 * Destructore of the core object
 */
QBspInfoCore::~QBspInfoCore()
{

}

/*
 * Display the app settings dialog, This method call form the main windows.
 */
void QBspInfoCore::showSettings()
{
	ConfigDialog* config = new ConfigDialog(_conf);
	config->exec();
}

/*!
 * Return pointer to storage configuration  objetct.
 */
Config* QBspInfoCore::config() const
{
	return _conf;
}

/*!
 *  Open BSP file, and read the info about entitites nad textures.
 * \param QString Name of the openable file.
 */
bool QBspInfoCore::openBspFile(const QString& filename)
{
	// open file
	std::string fnane = filename.toStdString();
	
	// open file
	FILE* bspFile = fopen(fnane.c_str(), "r+b");
	
	qDebug() << "reading header done ";
	Q3Header header;
	fread(&header, 1, sizeof(Q3Header), bspFile);	
	
	// remove last symbol at began of the bsp header
	QByteArray bsp(header.bspHead, strlen(header.bspHead)-1);
	
	if (bsp != BSP_HEADER_Q3)
	{
		_errorCode = Core::notBspFile;
		slotMessageToUser();
		fclose(bspFile);
		return false;
	}
	
	if (header.bspVersion != BSP_VERSION_Q3)
	{
		_errorCode = Core::notQ3Bsp;
		slotMessageToUser();
		fclose(bspFile);
		return false;
	}
		
	// read texture lump
	int textureLumpNum = header.lumps[BSP_LUMP_TEXTURES].length / sizeof(Q3Texture);
	fseek(bspFile, header.lumps[BSP_LUMP_TEXTURES].offaet, SEEK_SET);
	
	// read each texture entry
	for (int texCounter = 0; texCounter < textureLumpNum; ++texCounter)
	{
		Q3Texture texture;
		fread(&texture, 1, sizeof(Q3Texture), bspFile);
		qDebug() << texture.texName;
		qDebug() << texture.surfaceFlags;
		_resources << texture.texName;
	}

	Q3Entity entity;
	entity.size = header.lumps[BSP_LUMP_ENTITIES].length;
	entity.description = new char[entity.size];
	
	fseek(bspFile, header.lumps[BSP_LUMP_ENTITIES].offaet, SEEK_SET);
	fread(entity.description, entity.size, sizeof(char), bspFile);

	// TODO process entoity parsing, select sounds form it
	qDebug() << "entity " << entity.description;
	_entities = entity.description;
	
	fclose(bspFile);
	return true;
}

/*!
 *  Return pointer tolist of the entities, used in loaded BSP file.
 */
QByteArray QBspInfoCore::entities()
{
	return _entities;
}

/*!
 *  Return pointer tolist of the resources, used in loaded BSP file.
 */
QStringList QBspInfoCore::resources()
{
	return _resources;
}

/*!
 * Generate the message to user, based on the value of "_errorCode" variable. And emiting signal for display message to user.
 */
void QBspInfoCore::slotMessageToUser()
{	
	QString messTitle;
	QString messBody;
	if (sender())
	{
		if (sender()->objectName() == "config")
		{
			_errorCode = Core::notConfigured;
		}
	}

	switch(_errorCode){
		case Core::notBspFile:
		{
			messTitle = tr("Error opening file");
			messBody = tr("This file is not a BSP file.");
			break;
		}
		case Core::notQ3Bsp:
		{
			messTitle = tr("Error opening file");
			messBody = tr("This file is not Quake 3 BSP file.");
			break;
		}
		case Core::notConfigured:
		{
			messTitle = tr("Error");
			messBody = tr("Coinfiguration is not set. Please open the setting dialog, and set you options.");
			break;
		}
		case Core::none:
			break;
		default:
			break;
	}

	Q_EMIT messageToUser(messTitle, messBody);
	_errorCode = Core::none;
}
