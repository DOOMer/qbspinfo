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

#include "configdialog.h"
#include "ui_configdialog.h"

#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtWidgets/QFileDialog>

#include <QDebug>

/*!
 * Create the configuration dialog.
 * \param config Qonfig& Pointer to the app configuration object.
 */
ConfigDialog::ConfigDialog(Config* config, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::ConfigDialog), _conf(config)
{
    ui->setupUi(this);
	
	ui->editPathToGame->setText(_conf->value(KEY_GAME_PATH).toString());
	_createModList(ui->editPathToGame->text());
	int selectedMod = ui->cbxGameMod->findText(_conf->value(KEY_SELECTED_MOD).toString());
	
	if (selectedMod == -1)
	{
		ui->cbxGameMod->setCurrentIndex(0);
	}
	else
	{
		ui->cbxGameMod->setCurrentIndex(selectedMod);
	}
	
	connect(ui->butClose, SIGNAL(clicked(bool)), this, SLOT(close()));
	connect(ui->butPathToGame, SIGNAL(clicked(bool)), this, SLOT(selectGameDir()));
}

/*!
 * Destroy the configuration windows. 
 */
ConfigDialog::~ConfigDialog()
{
	_conf = nullptr;
    delete ui;
}

/*!
 * Close the dialog and save the  selected settings.
 */
bool ConfigDialog::close()
{
	_conf->setValue(KEY_GAME_PATH, QVariant(ui->editPathToGame->text()));
	_conf->setValue(KEY_SELECTED_MOD, QVariant(ui->cbxGameMod->currentText()));
	
	_conf->save();
	return QDialog::close();
}

/*!
 * Display dialog for the select game directory. 
 */
void ConfigDialog::selectGameDir()
{
	QString startDir = QDir::homePath();
	QString dirName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), startDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	if (!dirName.isEmpty())
	{
		ui->editPathToGame->setText(dirName);
		_createModList(dirName);
		ui->cbxGameMod->setCurrentIndex(0);
	}
}

/*!
 * Create list fo the  game modificantions (based on the subdirectories in the game directory). And set default mod (base game) as selected mod.
 * \param gamePath QString Path to the game directory.
 */
void ConfigDialog::_createModList(const QString& gamePath)
{
	ui->cbxGameMod->clear();
	QDir gameDir(gamePath);	
	QStringList modsList = gameDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

	int baseMod = modsList.indexOf("baseq3");
	if (baseMod == -1)
	{
		ui->cbxGameMod->addItem("baseq3");
	}
	else
	{
		QString bmod = modsList.takeAt(baseMod);
		ui->cbxGameMod->addItem(bmod);
	}
		
	ui->cbxGameMod->addItems(modsList);
}
