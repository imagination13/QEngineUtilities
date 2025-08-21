#ifndef QQuickDetailViewManager_h__
#define QQuickDetailViewManager_h__

#include <QObject>
#include <QHash>
#include <functional>
#include <QMetaType>
#include <QQmlEngine>
#include <QQuickItem>
#include "IQuickDetailCustomization.h"
#include "IQuickPropertyTypeCustomization.h"
#include "QEngineEditorAPI.h"

class QPropertyHandle;

class QENGINEEDITOR_API QQuickDetailViewManager : public QObject{
public:
	using CustomClassLayoutCreator = std::function<QSharedPointer<IQuickDetailCustomization>()>;
	using CustomPropertyTypeLayoutCreator = std::function<QSharedPointer<IQuickPropertyTypeCustomization>()>;
	using CustomPropertyValueWidgetCreator = std::function<QQuickItem* (QPropertyHandle*, QQuickItem*)>;

	static QQuickDetailViewManager* Get();

	void registerQml();

	template<typename IQuickDetailCustomizationType>
	void registerCustomClassLayout(const QMetaObject* InMetaObject)
	{
		mCustomClassLayoutMap.insert(InMetaObject, []() {
			return QSharedPointer<IQuickDetailCustomizationType>::create();
		});
	}
	void unregisterCustomClassLayout(const QMetaObject* InMetaObject);

	template<typename MetaType, typename IQuickPropertyTypeCustomizationType>
	void registerCustomPropertyTypeLayout() {
		mCustomPropertyTypeLayoutMap.insert(QMetaType::fromType<MetaType>(), []() {
			return QSharedPointer<IQuickPropertyTypeCustomizationType>::create();
			});
	}
	void unregisterCustomPropertyTypeLayout(const QMetaType& InMetaType);

	void registerCustomPropertyValueEditorCreator(const QMetaType& inMetaType, CustomPropertyValueWidgetCreator Creator);
	void unregisterCustomPropertyValueEditorCreator(const QMetaType& inMetaType);

	QQuickItem* createValueEditor(QPropertyHandle* inHandle, QQuickItem* parent);
	QSharedPointer<IQuickDetailCustomization> getCustomDetailLayout(const QMetaObject* InMetaObject);
	QSharedPointer<IQuickPropertyTypeCustomization> getCustomPropertyType(const QMetaType& InMetaType);

protected:
	QQuickDetailViewManager();

private:
	QHash<const QMetaObject*, CustomClassLayoutCreator> mCustomClassLayoutMap;
	QHash<QMetaType, CustomPropertyTypeLayoutCreator> mCustomPropertyTypeLayoutMap;
	QHash<QMetaType, CustomPropertyValueWidgetCreator> mPropertyValueEditorCreatorMap;
};

#endif // QQuickDetailViewManager_h__
