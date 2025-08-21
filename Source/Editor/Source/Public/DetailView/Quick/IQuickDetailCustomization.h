#ifndef IQUICKDETAILCUSTOMIZATION_H
#define IQUICKDETAILCUSTOMIZATION_H

#include <QSharedPointer>
#include "QEngineEditorAPI.h"

class QQuickDetailViewLayoutBuilder;
class QENGINEEDITOR_API IQuickDetailCustomization : public QEnableSharedFromThis<IQuickDetailCustomization> {
public:
	virtual ~IQuickDetailCustomization(){}

	virtual void customizeDetails(const QObject* inObject, QQuickDetailViewLayoutBuilder* inBuilder) = 0;
};

#endif // IQUICKDETAILCUSTOMIZATION_H
