#ifndef QQuickDetailViewLayoutBuilder_h__
#define QQuickDetailViewLayoutBuilder_h__

#include "QQuickDetailViewRow.h"

class QENGINEEDITOR_API QQuickDetailViewHeaderRowBuilder {
public:
	QQuickDetailViewHeaderRowBuilder(QQuickItem* inRootItem);
	QPair<QQuickItem*, QQuickItem*> makeNameValueSlot();
	void makePropertyHeader(QPropertyHandle* inHandle);
private:
	QQuickItem* mRootItem = nullptr;
};

class  QENGINEEDITOR_API QQuickDetailViewLayoutBuilder {
public:
	QQuickDetailViewLayoutBuilder(IDetailViewRow* inRow);
	QQuickDetailViewLayoutBuilder* addCustomRow(QQuickItem* item);
	void addProperty(QPropertyHandle* inPropertyHandle);
	void addObject(QObject* inObject);
private:
	IDetailViewRow* mRow = nullptr;
};

#endif // QQuickDetailViewLayoutBuilder_h__
