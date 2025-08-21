#include "QQuickDetailViewLayoutBuilder.h"
#include "QQuickDetailViewMananger.h"
#include "QQmlContext"

QQuickDetailViewHeaderRowBuilder::QQuickDetailViewHeaderRowBuilder(QQuickItem* inRootItem)
	: mRootItem(inRootItem)
{
}

QPair<QQuickItem*, QQuickItem*> QQuickDetailViewHeaderRowBuilder::makeNameValueSlot()
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
	QQmlContext* newContext = new QQmlContext(context, mRootItem);
	QQmlComponent rootComp(newContext->engine());
	rootComp.setData(R"(
        import QtQuick;
        import QtQuick.Controls;
        Rectangle{
            anchors.fill: parent
            Text {
                id: indicator
                visible: DetailDelegate.isTreeNode && DetailDelegate.hasChildren
                x: padding + (DetailDelegate.depth * DetailDelegate.indent)
                anchors.verticalCenter: parent.verticalCenter
                text: "?"
                rotation: DetailDelegate.expanded ? 90 : 0
            }
            Item{
                id: nameEditorContent
                anchors.left: parent.left 
                anchors.leftMargin: padding + (DetailDelegate.isTreeNode ? (DetailDelegate.depth + 1) * DetailDelegate.indent : 0)
                anchors.right: splitter.left
            }
            Item{
                id: valueEditorContent
                anchors.left: splitter.left 
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.right: parent.right
            }
            Rectangle{
                id: splitter
                x: DetailView.SpliterPencent * DetailView.width
                height: parent.height
                width: 2
                color: "red"
                MouseArea {
                    id: dragArea
                    hoverEnabled:true
                    cursorShape:containsMouse?Qt.SplitHCursor:Qt.ArrowCursor
                    anchors.fill: parent
                    drag.target: parent
                    drag.axis:Drag.XAxis
                    drag.minimumX: 10
                    drag.maximumX: DetailView.width - 10
                    onPositionChanged: {
                        DetailView.SpliterPencent = splitter.x/DetailView.width
                    }
                }
            }
        }
    )", QUrl());
    QQuickItem* slotItem = qobject_cast<QQuickItem*>(rootComp.create(newContext));
    slotItem->setParentItem(mRootItem);
    return { slotItem->childItems()[1] ,slotItem->childItems()[2] };
}

void QQuickDetailViewHeaderRowBuilder::makePropertyHeader(QPropertyHandle* inHandle)
{
	QQmlEngine* engine = qmlEngine(mRootItem);
	QQmlContext* context = qmlContext(mRootItem);
    QPair<QQuickItem*, QQuickItem*> slotItem = makeNameValueSlot();
	QQuickItem* nameEditor = inHandle->createNameEditor(slotItem.first);
	QQuickItem* valueEditor = inHandle->createValueEditor(slotItem.second);
	context->parentContext()->setContextProperty("heightProxy", valueEditor ? valueEditor : nameEditor);
}

QQuickDetailViewLayoutBuilder::QQuickDetailViewLayoutBuilder(IDetailViewRow* inRow)
	: mRow(inRow)
{
}

QQuickDetailViewLayoutBuilder* QQuickDetailViewLayoutBuilder::addCustomRow(QQuickItem* item)
{
    return nullptr;
}

void QQuickDetailViewLayoutBuilder::addProperty(QPropertyHandle* inPropertyHandle)
{
	QSharedPointer<IDetailViewRow> child(new QDetailViewRow_Property(inPropertyHandle));
	mRow->addChild(child);
	child->attachChildren();
}

void QQuickDetailViewLayoutBuilder::addObject(QObject* inObject)
{
	for (int i = 1; i < inObject->metaObject()->propertyCount(); i++) {
		QMetaProperty prop = inObject->metaObject()->property(i);
		QString propertyPath = prop.name();
		QPropertyHandle* handler = QPropertyHandle::FindOrCreate(inObject, propertyPath);
		if (handler)
			addProperty(handler);
		else
			qWarning() << "property handle is null";
	}
}
