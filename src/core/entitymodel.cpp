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

#include "entitymodel.h"

#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QPair>

#include <QDebug>

EntityModel::EntityModel(QObject *parent) :
    QAbstractItemModel(parent)
{	
	QVector<QByteArray> root;
	root << tr("Entity (or property)").toUtf8() << tr("value").toUtf8();
	_rootItem = new EntityItem(root, Entity::entityGroup);
	_entitesItemsList << _rootItem;
}

EntityModel::~EntityModel()
{
	clearData();
}

QVariant EntityModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	
	EntityItem *item = static_cast<EntityItem*>(index.internalPointer());
	return item->data(index.column());
}

Qt::ItemFlags EntityModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}


QVariant EntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}


QModelIndex EntityModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
	
	EntityItem *parentItem;
	if (!parent.isValid())
	{
		parentItem = _rootItem;
	}
	else
	{
		parentItem = static_cast<EntityItem*>(parent.internalPointer());
	}
	
	EntityItem *childItem = parentItem->child(row);
	if (childItem)
	{
		return createIndex(row, column, childItem);
	}
	else
	{
		QModelIndex();
	}
}

QModelIndex EntityModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
		return QModelIndex();
	
	EntityItem *childItem = static_cast<EntityItem*>(index.internalPointer());
	EntityItem *parentItem = childItem->parent();
	
	if (parentItem == _rootItem)
	{
		return QModelIndex();
	}
	
	return createIndex(parentItem->row(), 0, parentItem);
}

/*!
 * Returns the number of rows under the given parent. 
 */
int EntityModel::rowCount(const QModelIndex& parent) const
{
	EntityItem *parentItem;
	if (parent.column() > 0)
	{
		return 0;
	}
	
	if (!parent.isValid())
	{
		parentItem = _rootItem;
	}
	else
	{
		parentItem = static_cast<EntityItem*>(parent.internalPointer());
	}
	
	return parentItem->childCount();
}

/*!
 * Return the number of columns for the children of the given parent.
 * \param QModelIndex paren Parent item modelindex. If its ni valid, return 'columnCount' for root item.
 */
int EntityModel::columnCount(const QModelIndex& parent) const
{
	if (parent.isValid())
	{
		return static_cast<EntityItem*>(parent.internalPointer())->columnCount();
	}
	else
	{
		return _rootItem->columnCount();
	}
}

/*!
 * Parse raw chr* string with entite description.
 */
void EntityModel::parseRawData(const char* data)
{	
	QStringList tempEntList = QString(data).mid(1, QString(data).size() - 3).split("}\n{");
	QStringList tempPropertyList;
	QMap<QString, int> tempClassnamesList;
	QMap<QString, QString> tempPropertyMap;
	QVector<QByteArray> itemdata;
	
	for (int i = 0; i < tempEntList.count(); ++i)
	{
		tempEntList[i] = tempEntList[i].trimmed();
		
		// create temp propery list
		tempPropertyList = tempEntList.value(i).split("\n");
		
		// clear map with entiy properties
		tempPropertyMap.clear();
		
		for (int j = 0; j < tempPropertyList.count(); ++j)
		{
// 			qDebug() << tempPropertyList.at(j);
			if (!tempPropertyList.value(j).isEmpty())
			{	
				// create pair for propery (first - key. second - value)
				QStringList s = tempPropertyList.value(j).split("\" \"");

				// remove quotes
				s[0] = s[0].mid(1,s[0].size() - 1);
				s[1] = s[1].mid(0,s[1].size() - 1);
				tempPropertyMap.insert(s[0], s[1]);
			}
		}
// 		qDebug() << tempPropertyMap;
		
		QString classname = tempPropertyMap.value("classname");
		int ci = tempClassnamesList.keys().indexOf(classname);
		
		EntityItem *item = nullptr;
		EntityItem *group = nullptr;
		
		if (ci != -1)
		{
			// get group identifier
			group = _entitesItemsList.at(ci);
		}
		else
		{
			// create group identifier
			itemdata << classname.toLatin1();
			group = new EntityItem(itemdata, Entity::entityGroup, _rootItem);
			_rootItem->addChild(group);
			_entitesItemsList.append(group);
			Q_EMIT layoutChanged();
			itemdata.clear();
			
			// get index for added group item, and add it into temp list
			int grIndex = _entitesItemsList.indexOf(group);
			tempClassnamesList.insert(classname, grIndex);
		}
// 		QVector<QBitArray> data;
		itemdata << classname.toLatin1();
		item = new EntityItem(itemdata, Entity::entityItem, group);
		group->addChild(item);
		_entitesItemsList.append(item);
		Q_EMIT layoutChanged();
		
		itemdata.clear();
		
		// TODO add properies from tempPropertyMap
	}
}

/*!
 *  Clear data, when load new BSP file.
 */
void EntityModel::clearData()
{
	qDeleteAll(_entitesItemsList);
}
