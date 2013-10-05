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
	createRoortElement();
}

EntityModel::~EntityModel()
{
	clearData();
}

/*!
 * Returns the data stored under the DislpayRole.
 * \return QVariant Call data() method with the index of column.
 */
QVariant EntityModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole)
		return QVariant();
	
	EntityItem *item = static_cast<EntityItem*>(index.internalPointer());
	return item->data(index.column());
}

/*!
 * Returns the item flags for the given index.
 */
Qt::ItemFlags EntityModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

/*!
 * Returns the data for the section in the header with horizontal orientation only. 
 * \return QVariant Text data from root element.
 */
QVariant EntityModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

/*!
 * Returns the index of the item in the model specified by the given row, column and parent index.
 * \param QModelIndex parent Paren element for current.
 * \return QModelIndex Calculated index.
 */
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
		return QModelIndex();
	}
}

/*!
 * Returns the parent of the model item with the given index. 
 * If given index is a index of root element, then return empty QModelIndex.
 * \param QModelIndex index Infex of element, for which the need calculate parent..
 * \return QModelIndex Calculated index.
 */
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
 * Returns the number of columns for the children of the given parent.
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
	createRoortElement();
	
	QStringList tempEntList = QString(data).mid(1, QString(data).size() - 3).split("}\n{");
	QStringList tempPropertyList;
	QMap<QString, int> tempClassnamesList;
	QMap<QString, QString> tempPropertyMap;
	QVector<QByteArray> itemdata;

	EntityItem *item = nullptr;
	EntityItem *group = nullptr;
	EntityItem *property = nullptr;
	
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
		
		QString classname = tempPropertyMap.value("classname");
		int ci = tempClassnamesList.value(classname);

		if (ci != 0)
		{
			// get group identifier
			group = _entitesItemsList.at(ci);
		}
		else
		{
			// create group identifier
			itemdata << classname.toUtf8() << "";
			group = new EntityItem(itemdata, Entity::entityGroup, _rootItem);
			_rootItem->addChild(group);
			_entitesItemsList.append(group);
			itemdata.clear();
			
			// get index for added group item, and add it into temp list
			int grIndex = _entitesItemsList.indexOf(group);
			tempClassnamesList.insert(classname, grIndex);
		}

		itemdata << classname.toLatin1() << "";
		item = new EntityItem(itemdata, Entity::entityItem, group);
		group->addChild(item);
		_entitesItemsList.append(item);
		itemdata.clear();

		// create subitems for each entity prepoery
		for (int p = 0; p < tempPropertyMap.keys().count(); p++)
		{
			itemdata << tempPropertyMap.keys().value(p).toUtf8();
			itemdata << tempPropertyMap.value(itemdata.value(0)).toUtf8();
			property = new EntityItem(itemdata, Entity::entityProperty, item);
			item->addChild(property);
			_entitesItemsList.append(property);
			itemdata.clear();
		}
	}
	Q_EMIT layoutChanged(); // send signal for update view
}

/*!
 *  Clear data, when load new BSP file.
 */
void EntityModel::clearData()
{
	qDeleteAll(_entitesItemsList);
}

void EntityModel::createRoortElement()
{
	QVector<QByteArray> root;
	root << tr("Entity (or property)").toUtf8() << tr("Value").toUtf8();
	_rootItem = new EntityItem(root, Entity::entityGroup);
	_entitesItemsList << _rootItem;
}
