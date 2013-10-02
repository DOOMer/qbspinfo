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

#include "aboutdialog.h"
#include "ui_aboutdialog.h"

/*!
 * Create about window, Setyp the infoemation labels.
 */
AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    _ui(new Ui::AboutDialog)
{
    _ui->setupUi(this);
	QString version = "v" + QString(VERSION);
	QString buildDate = "Built date: " + QString(__DATE__) + ",  " + QString(__TIME__);
	QString infoQt = QString("Built with Qt ") + QT_VERSION_STR + QString(", running with Qt ") + qVersion();
	QString description = tr("Get info about Quake 3 BSP files");
	QString copyright = "Copyright (c) 2013, Artem 'DOOMer' Galichkin";
	
	_ui->labVersion->setText(version);
	_ui->labBuildDate->setText(buildDate);
	_ui->labQtVersions->setText(infoQt);
	_ui->labDescription->setText(description);
	_ui->labCopyright->setText(copyright);
	
	connect(_ui->butAboutQt, SIGNAL(clicked(bool)), qApp, SLOT(aboutQt()));
	connect(_ui->butClose, SIGNAL(clicked(bool)), this, SLOT(close()));
}

/*!
 * About windows destructor.
 */
AboutDialog::~AboutDialog()
{
    delete _ui;
}
