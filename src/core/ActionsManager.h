/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2013 - 2014 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

#ifndef OTTER_ACTIONSMANAGER_H
#define OTTER_ACTIONSMANAGER_H

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtWidgets/QShortcut>

namespace Otter
{

class ActionsManager : public QObject
{
	Q_OBJECT

public:
	~ActionsManager();

	static void createInstance(QObject *parent = NULL);
	static void loadProfiles();
	static void registerAction(QWidget *window, const QLatin1String &name, const QString &text, const QIcon &icon = QIcon());
	static void registerAction(QWidget *window, QAction *action);
	static void registerWindow(QWidget *window, QList<QAction*> actions);
	static void triggerAction(const QString &action);
	static void setupLocalAction(QAction *localAction, const QLatin1String &globalAction, bool connectTrigger = false);
	static void setupWindowActions(QWidget *window);
	static QAction* getAction(const QString &action);
	static QList<QKeySequence> getShortcuts(const QLatin1String &action);
	static QStringList getIdentifiers();
	static bool hasShortcut(const QKeySequence &shortcut, const QLatin1String &excludeAction);

protected:
	void timerEvent(QTimerEvent *event);

protected slots:
	void optionChanged(const QString &option);
	void removeWindow(QObject *window);
	void triggerMacro();

private:
	explicit ActionsManager(QObject *parent = NULL);

	int m_reloadTimer;

	static ActionsManager *m_instance;
	static QHash<QWidget*, QHash<QString, QAction*> > m_windowActions;
	static QHash<QString, QAction*> m_applicationActions;
	static QHash<QString, QStringList> m_applicationMacros;
	static QHash<QString, QList<QKeySequence> > m_profileShortcuts;
	static QHash<QString, QKeySequence> m_nativeShortcuts;
};

}

#endif
