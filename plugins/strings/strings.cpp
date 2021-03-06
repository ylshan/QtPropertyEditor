/***************************************************************************
 *   This file is part of the propertyEditor project                       *
 *   Copyright (C) 2008 by BogDan Vatra                                    *
 *   bog_dan_ro@yahoo.com                                                  *
 **                   GNU General Public License Usage                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 *                                                                         *
 **                  GNU Lesser General Public License                    **
 *                                                                         *
 *   This library is free software: you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        * 
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library.                                      * 
 *   If not, see <http://www.gnu.org/licenses/>.                           *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ****************************************************************************/

#include <QtCore>
#include <QMetaProperty>
#include <QMetaEnum>

#include <QtWidgets/QLineEdit>

#include "strings.h"

Strings::Strings(QObject* parent, QObject* object, int property, const PropertyModel * propertyModel): PropertyInterface(parent, object, property, propertyModel)
{
}


QWidget* Strings::createEditor(QWidget * parent, const QModelIndex & index)
{
	Q_UNUSED(index);
	QLineEdit *le = new QLineEdit(parent);
	if (validator(QVariant::String))
	{
		validator(QVariant::String)->setObject(object());
		validator(QVariant::String)->setProperty(objectPropertyName());
		le->setValidator(validator(QVariant::String));
	}
	le->setText(value().toString());
	connect(le, SIGNAL(textChanged(const QString&)), this, SLOT(setValue(const QString&)));
	return le;
}

void Strings::setValue(const QString& s)
{
	PropertyInterface::setValue(s);
}

QVariant Strings::data(const QModelIndex & index)
{
	if (!index.isValid() || !object() || -1 == objectProperty())
		return QVariant();

	switch (index.column())
	{
		case 0:
			return object()->metaObject()->property(objectProperty()).name();
		case 1:
			return value();
	}
	return QVariant();
}

bool Strings::setData(QVariant data, const QModelIndex & index)
{
	Q_UNUSED(index);
	return PropertyInterface::setValue(data);
}

void Strings::setEditorData(QWidget * editor, const QModelIndex & /*index*/)
{
	dynamic_cast<QLineEdit *>(editor)->setText(value().toString());
}


bool Strings::canHandle(QObject * object, int property)const
{
	if (object->metaObject()->property(property).isEnumType() || object->metaObject()->property(property).isFlagType())
		return false;

	switch (object->property(object->metaObject()->property(property).name()).type())
	{
		case QVariant::String:
			return true;
		default:
			return false;
	}
}

PropertyInterface* Strings::createInstance(QObject * object, int property, const PropertyModel * propertyModel) const
{
	return new Strings(parent(), object, property, propertyModel);
}

//Q_EXPORT_PLUGIN2(strings, Strings);
