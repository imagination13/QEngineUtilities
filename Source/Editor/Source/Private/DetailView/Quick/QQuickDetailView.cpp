#include "QQuickDetailView.h"
#include "private/qqmldata_p.h"
#include <QQmlEngine>
#include <QQmlContext>
#include "private/qquicksplitview_p.h"
#include "QQuickDetailViewPrivate.h"
#include "QQuickDetailViewRow.h"
#include "QQmlHelper.h"

void QQuickDetailViewPrivate::initItemCallback(int serializedModelIndex, QObject* object)
{
    QQuickTreeViewExPrivate::initItemCallback(serializedModelIndex, object);
	auto item = qobject_cast<QQuickItem*>(object);
	if (!item)
		return;
	const QModelIndex& index = m_treeModelToTableModel.mapToModel(serializedModelIndex);;
	IDetailViewRow* node = static_cast<IDetailViewRow*>(index.internalPointer());
	node->setupItem(item);
 //   Q_Q(QQuickDetailView);
}


QQuickDetailView::QQuickDetailView(QQuickItem* parent /*= nullptr*/)
    : QQuickTreeViewEx(*(new QQuickDetailViewPrivate()),parent)
{
    setModel(QVariant::fromValue(d_func()->mModel));
	setReuseItems(false);
	setEditTriggers(QQuickTableView::EditTrigger::DoubleTapped);
}

qreal QQuickDetailView::getSpliterPencent() const
{
    return d_func()->mSpliterPencent;
}

void QQuickDetailView::setSpliterPencent(qreal val)
{
    if(val != d_func()->mSpliterPencent){
		d_func()->mSpliterPencent = val;
		Q_EMIT asSpliterPencentChanged(val);
    }
}

void QQuickDetailView::setObject(QObject* inObject)
{
	if (inObject != d_func()->mModel->getObject()) {
		d_func()->mModel->setObject(inObject);
		Q_EMIT asObjectChanged(inObject);
	}
}

QObject* QQuickDetailView::getObject() const
{
	return d_func()->mModel->getObject();
}

void QQuickDetailView::componentComplete()
{
    QQmlEngine* engine = qmlEngine(this);
	QQmlComponent* delegate = new QQmlComponent(engine, this);
	engine->rootContext()->setContextProperty("helper", QQmlHelper::Get());
	delegate->setData(R"(
        import QtQuick;
        import QtQuick.Controls;
        import QtQuick.Layouts;
		import Qt.DetailView;
        Item {
			 id: DetailDelegate
			 readonly property real indent: 20
             readonly property real padding: 5
			 required property DetailView DetailView
			 required property int row
             required property bool isTreeNode
             required property bool expanded
             required property int hasChildren
             required property int depth
             implicitWidth: DetailView.width
             implicitHeight: 30
		     TapHandler {
                 onTapped: DetailView.toggleExpanded(row)
             }
         })", QUrl("QQuickDetailView.componentComplete"));;
	setDelegate(delegate);
    QQuickTreeViewEx::componentComplete();
}
