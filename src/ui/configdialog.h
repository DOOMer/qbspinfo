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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "core/config.h"

#include <QDialog>

namespace Ui {
class ConfigDialog;
}

class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(Config *config, QWidget *parent = 0);
    ~ConfigDialog();

private:
    Ui::ConfigDialog *ui;
	Config *_conf;
	
	void _createModList(const QString& gamePath);
	
private Q_SLOTS:
	bool close();
	void selectGameDir();
};

#endif // CONFIGDIALOG_H
