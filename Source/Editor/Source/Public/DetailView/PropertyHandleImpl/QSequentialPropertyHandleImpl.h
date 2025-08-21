#ifndef QSequentialPropertyHandle_h__
#define QSequentialPropertyHandle_h__

#include "QMetaContainer"
#include "IPropertyHandleImpl.h"

class QSequentialPropertyHandleImpl: public IPropertyHandleImpl
{
public:
	QSequentialPropertyHandleImpl(QPropertyHandle* InHandle);

	int itemCount();
	void appendItem(QVariant InVar);
	void moveItem(int InSrcIndex, int InDstIndex);
	void removeItem(int InIndex);

protected:
	void generateChildrenRow(QRowLayoutBuilder* Builder) override;
	QWidget* generateValueWidget() override;

	virtual QQuickItem* createValueEditor(QQuickItem* inParent) override;

	QPropertyHandle* createChildHandle(const QString& inSubName) override;

private:
	QMetaSequence mMetaSequence;
};

#endif // QSequentialPropertyHandle_h__
