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

#include "entityitem.h"

EntityItem::EntityItem(const QVector< QByteArray >& data, Entity::itemType type, EntityItem* parent)
{
	_parent = parent;
	_itemData = data;
	_type = type;
}

EntityItem::~EntityItem()
{
	qDeleteAll(_childList);
}

/*!
 *  Add pointer to child item.
 */
void EntityItem::addChild(EntityItem* child)
{
	_childList.append(child);
}

/*!
 * Return pointer to child item by row.
 */
EntityItem* EntityItem::child(int row)
{
	return _childList.value(row);
}

/*!
 * Return pointer to pareint item.
 */
EntityItem* EntityItem::parent()
{
	return _parent;
}

/*!
 * Return data form selected EntityItem.
 */
QVariant EntityItem::data(int colum) const
{
	return QVariant(_itemData.value(colum));
}

/*!
 * Return count of cjld items for paren of selected item.
 * \return int Cound of childs.
 */
int EntityItem::row() const
{
	if (_parent)
	{
		return _parent->_childList.indexOf(const_cast<EntityItem*>(this));
	}
	return 0;
}

int EntityItem::columnCount() const
{
	return _itemData.count();
}


/*!
 * Retun count of child utems for selected item.
 */
int EntityItem::childCount() const
{
	_childList.count();
}


