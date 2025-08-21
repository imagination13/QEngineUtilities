#ifndef QEnumPropertyHandleImpl_h__
#define QEnumPropertyHandleImpl_h__

#include "IPropertyHandleImpl.h"

class QEnumPropertyHandleImpl: public IPropertyHandleImpl {
public:
	QEnumPropertyHandleImpl(QPropertyHandle* InHandle);

protected:
	QWidget* generateValueWidget() override;
	QQuickItem* createValueEditor(QQuickItem* inParent) override;

private:
	QHash<QString, int> mNameToValueMap;
	QList<QString> mKeys;
};


#endif // QEnumPropertyHandleImpl_h__
