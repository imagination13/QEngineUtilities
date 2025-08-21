#ifndef IPropertyHandleImpl_h__
#define IPropertyHandleImpl_h__

#include "QEngineEditorAPI.h"
#include "QQuickItem"

class QRowLayoutBuilder;
class QENGINEEDITOR_API IPropertyHandleImpl{
	friend class QPropertyHandle;
protected:
	IPropertyHandleImpl(QPropertyHandle* InHandle){
		mHandle = InHandle;
	}
	virtual QPropertyHandle* findChildHandle(const QString& inSubName);
	virtual QPropertyHandle* createChildHandle(const QString& inSubName);
	virtual QWidget* generateNameWidget();
	virtual QWidget* generateValueWidget();
	virtual void generateChildrenRow(QRowLayoutBuilder* Builder) {}

	virtual QQuickItem* createNameEditor(QQuickItem* inParent);
	virtual QQuickItem* createValueEditor(QQuickItem* inParent);

protected:
	QPropertyHandle* mHandle;
};

#endif // IPropertyHandleImpl_h__
