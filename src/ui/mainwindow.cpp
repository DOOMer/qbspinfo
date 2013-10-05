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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ui/aboutdialog.h"

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <QDebug>

/*!
 * Create the main window.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow), _core(new QBspInfoCore(this))
{
    _ui->setupUi(this);
	_ui->tabWidget->setCurrentIndex(0);
	
	_ui->treeEntities->setModel(_core->entities());
	
	connect(_ui->butAbout, SIGNAL(clicked(bool)), this, SLOT(slotAboutDialog()));
	connect(_ui->butSettings, SIGNAL(clicked(bool)), _core, SLOT(showSettings()));
	connect(_ui->butOpenBspFile, SIGNAL(clicked(bool)), this, SLOT(slotOpenFile()));
	connect(_core, SIGNAL(messageToUser(QString,QString)), this, SLOT(messageToUser(QString,QString)));
}

/*!
 * Destroy the main window.
 */
MainWindow::~MainWindow()
{
    delete _ui;
}

/*!
 * Disolat rhe dialog for opening BSP file. And call 'openMapFile' method from core object, when file is  selected.
 */
void MainWindow::slotOpenFile()
{
	QString mapsDir = _core->config()->mapsDir();
	
	if (!mapsDir.isEmpty())
	{
		QString openMapFile = QFileDialog::getOpenFileName(this, tr("Open File"), mapsDir, tr("Quake 3 maps (*.bsp)"));
		
		if (!openMapFile.isEmpty())
		{
			if (_core->openBspFile(openMapFile))
			{
				_ui->labOpenedBspFile->setText(openMapFile);

				_ui->treeEntities->resizeColumnToContents(0);

				// TODO replace on the models
				_ui->listResources->clear();
				_ui->listResources->addItems(_core->resources());
			}
		}
	}
}

/*!
 * Dsi[;ay the information message to user.
 * \param title QString Message window title.
 * \param message QString Message text.
 */
void MainWindow::messageToUser(const QString& title, const QString& message)
{
	QMessageBox msg;
	msg.setWindowTitle(title);
	msg.setText(message);
	msg.exec();
}

/*!
 *  Show dialog with info about app.
 */
void MainWindow::slotAboutDialog()
{
	AboutDialog *about = new AboutDialog;
	about->exec();
}
