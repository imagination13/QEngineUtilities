#ifndef QQuickDetailViewPrivate_h__
#define QQuickDetailViewPrivate_h__

#include "private/qquicktreeview_p_p.h"
#include "QQuickTreeViewExPrivate.h"
#include "QQuickDetailView.h"
#include "QQuickDetailViewModel.h"

class QQuickDetailViewPrivate : public QQuickTreeViewExPrivate
{
	Q_DECLARE_PUBLIC(QQuickDetailView)

public:
	QQuickDetailViewPrivate();
	void initItemCallback(int serializedModelIndex, QObject* object) override;
	void updateRequiredProperties(int serializedModelIndex, QObject* object, bool init) override;
private:
	qreal mSpliterPencent = 0.3;
	QList<QObject*> mObjects;
	QQuickDetailViewModel* mModel;
};

QQuickDetailViewPrivate::QQuickDetailViewPrivate()
	:mModel(new QQuickDetailViewModel)
{

}

void QQuickDetailViewPrivate::updateRequiredProperties(int serializedModelIndex, QObject* object, bool init)
{
	Q_Q(QQuickDetailView);
	const QPoint cell = cellAtModelIndex(serializedModelIndex);
	const int row = cell.y();
	const int column = cell.x();
	setRequiredProperty("row", QVariant::fromValue(serializedModelIndex), serializedModelIndex, object, init);
	setRequiredProperty("DetailView", QVariant::fromValue(q), serializedModelIndex, object, init);
	setRequiredProperty("isTreeNode", column == 0, serializedModelIndex, object, init);
	setRequiredProperty("hasChildren", m_treeModelToTableModel.hasChildren(row), serializedModelIndex, object, init);
	setRequiredProperty("expanded", q->isExpanded(row), serializedModelIndex, object, init);
	setRequiredProperty("depth", m_treeModelToTableModel.depthAtRow(row), serializedModelIndex, object, init);
}

#endif // QQuickDetailViewPrivate_h__