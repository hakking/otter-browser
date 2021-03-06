/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2010 - 2014 David Rosca <nowrep@gmail.com>
* Copyright (C) 2014 - 2016 Jan Bajer aka bajasoft <jbajer@gmail.com>
* Copyright (C) 2015 - 2016 Michal Dutkiewicz aka Emdek <michal@emdek.pl>

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

#ifndef OTTER_CONTENTBLOCKINGPROFILE_H
#define OTTER_CONTENTBLOCKINGPROFILE_H

#include "ContentBlockingManager.h"

#include <QtCore/QRegularExpression>

namespace Otter
{

class ContentBlockingProfile : public QObject
{
	Q_OBJECT

public:
	enum RuleOption
	{
		NoOption = 0,
		ThirdPartyOption = 1,
		StyleSheetOption = 2,
		ScriptOption = 4,
		ImageOption = 8,
		ObjectOption = 16,
		ObjectSubRequestOption = 32,
		SubDocumentOption = 64,
		XmlHttpRequestOption = 128
	};

	Q_DECLARE_FLAGS(RuleOptions, RuleOption)

	enum ProfileCategory
	{
		OtherCategory = 0,
		AdvertisementsCategory = 1,
		AnnoyanceCategory = 2,
		PrivacyCategory = 4,
		SocialCategory = 8,
		RegionalCategory = 16
	};

	struct ContentBlockingRule
	{
		QStringList blockedDomains;
		QStringList allowedDomains;
		RuleOptions ruleOption;
		RuleOptions exceptionRuleOption;
		bool isException;
		bool needsDomainCheck;
	};

	explicit ContentBlockingProfile(const QList<QString> languages, const QUrl updateUrl, const QDateTime lastUpdate, const QString &name, const QString &title, int updateInterval, const ProfileCategory &category, QObject *parent = NULL);

	void clear();
	void setCategory(const ProfileCategory &category);
	void setTitle(const QString &title);
	void setUpdateInterval(int interval);
	void setUpdateUrl(const QUrl url);
	QString getName() const;
	QString getTitle() const;
	QUrl getUpdateUrl() const;
	QDateTime getLastUpdate() const;
	ContentBlockingManager::CheckResult checkUrl(const QUrl &baseUrl, const QUrl &requestUrl, NetworkManager::ResourceType resourceType);
	QStringList getStyleSheet();
	QStringList getStyleSheetBlackList(const QString &domain);
	QStringList getStyleSheetWhiteList(const QString &domain);
	QList<QLocale::Language> getLanguages() const;
	ProfileCategory getCategory() const;
	int getUpdateInterval() const;
	bool downloadRules();
	bool hasCustomTitle() const;
	bool hasCustomUpdateUrl() const;

protected:
	struct Node
	{
		ContentBlockingRule *rule;
		QChar value;
		QVarLengthArray<Node*, 1> children;

		Node() : rule(NULL), value(0) {}
	};

	QString getPath(bool forceBundled = false) const;
	void loadHeader(const QString &path);
	void parseRuleLine(QString line);
	void parseStyleSheetRule(const QStringList &line, QMultiHash<QString, QString> &list);
	void addRule(ContentBlockingRule *rule, const QString &ruleString);
	void deleteNode(Node *node);
	bool loadRules();
	bool resolveDomainExceptions(const QString &url, const QStringList &ruleList);
	bool checkUrlSubstring(Node *node, const QString &subString, QString currentRule, NetworkManager::ResourceType resourceType);
	bool checkRuleMatch(ContentBlockingRule *rule, const QString &currentRule, NetworkManager::ResourceType resourceType);

protected slots:
	void optionChanged(int identifier, const QVariant &value);
	void replyFinished();

private:
	Node *m_root;
	QNetworkReply *m_networkReply;
	QString m_requestUrl;
	QString m_requestHost;
	QString m_baseUrlHost;
	QString m_name;
	QDateTime m_lastUpdate;
	QRegularExpression m_domainExpression;
	QPair<QString, bool> m_title;
	QPair<QUrl, bool> m_updateUrl;
	QStringList m_styleSheet;
	QList<QLocale::Language> m_languages;
	QMultiHash<QString, QString> m_styleSheetBlackList;
	QMultiHash<QString, QString> m_styleSheetWhiteList;
	ProfileCategory m_category;
	int m_updateInterval;
	bool m_enableWildcards;
	bool m_isUpdating;
	bool m_isEmpty;
	bool m_wasLoaded;

signals:
	void profileModified(const QString &profile);
};

}

#endif
