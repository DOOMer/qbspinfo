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

#ifndef ENTITYMODEL_H
#define ENTITYMODEL_H

#include "core/entityitem.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QByteArray>
#include <QtCore/QStringList>
#include <QtCore/QList>

class EntityModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit EntityModel(QObject *parent = 0);
	~EntityModel();
	
	QVariant data(const QModelIndex &index, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	
	void parseRawData(const char* data);
	void clearData();

private:
	QList<EntityItem*> _entitesItemsList;
	EntityItem *_rootItem;
	
	void createRoortElement();
};

#endif // ENTITYMODEL_H
