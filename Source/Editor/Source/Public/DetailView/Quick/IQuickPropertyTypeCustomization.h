#ifndef IQuickPropertyTypeCustomization_h__
#define IQuickPropertyTypeCustomization_h__

#include <QSharedPointer>
#include "QEngineEditorAPI.h"

class QPropertyHandle;
class QQuickDetailViewHeaderRowBuilder;
class QQuickDetailViewLayoutBuilder;

class QENGINEEDITOR_API IQuickPropertyTypeCustomization :public  QEnableSharedFromThis<IQuickPropertyTypeCustomization>
{
public:
	virtual void customizeHeader(QPropertyHandle* inPropertyHandle, QQuickDetailViewHeaderRowBuilder* inBuilder) = 0;
	virtual void customizeChildren(QPropertyHandle* inPropertyHandle, QQuickDetailViewLayoutBuilder* inBuilder) {}
};

#endif // IQuickPropertyTypeCustomization_h__
