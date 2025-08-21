#ifndef QAssociativePropertyHandle_h__
#define QAssociativePropertyHandle_h__

#include "QMetaContainer"
#include "IPropertyHandleImpl.h"

class QRowLayoutBuilder;
class QAssociativePropertyHandleImpl: public IPropertyHandleImpl {
public:
	QAssociativePropertyHandleImpl(QPropertyHandle* InHandle);

	void appendItem(QString inKey, QVariant inValue);
	bool renameItem(QString inSrc, QString inDst);
	void removeItem(QString inKey);

protected:
	void generateChildrenRow(QRowLayoutBuilder* Builder)  override;
	QWidget* generateValueWidget() override;
	QQuickItem* createValueEditor(QQuickItem* inParent) override;
	QPropertyHandle* createChildHandle(const QString& inSubName) override;

private:
	QMetaAssociation mMetaAssociation;
};


#endif // QAssociativePropertyHandle_h__

