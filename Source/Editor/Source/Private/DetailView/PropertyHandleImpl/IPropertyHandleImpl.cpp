#include "DetailView/PropertyHandleImpl/IPropertyHandleImpl.h"
#include "DetailView/QPropertyHandle.h"
#include "DetailView/Widget/QDetailViewManager.h"
#include "QBoxLayout"
#include "Widgets/QElideLabel.h"

QPropertyHandle* IPropertyHandleImpl::findChildHandle(const QString& inSubName) {
	return QPropertyHandle::Find(mHandle->parent(), mHandle->getSubPath(inSubName));
}

QPropertyHandle* IPropertyHandleImpl::createChildHandle(const QString& inSubName) {
	return nullptr;
}

QWidget* IPropertyHandleImpl::generateNameWidget() {
	return new QElideLabel(mHandle->getName());
}

QWidget* IPropertyHandleImpl::generateValueWidget() {
	QWidget* valueContent = new QWidget;
	valueContent->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	QHBoxLayout* valueContentLayout = new QHBoxLayout(valueContent);
	valueContentLayout->setAlignment(Qt::AlignLeft);
	valueContentLayout->setContentsMargins(10, 2, 10, 2);
	valueContentLayout->setSpacing(2);
	valueContentLayout->addWidget(QDetailViewManager::Instance()->getCustomPropertyValueWidget(mHandle));
	mHandle->generateAttachButtonWidget(valueContentLayout);
	return valueContent;
}

QQuickItem* IPropertyHandleImpl::createNameEditor(QQuickItem* inParent)
{
	QQmlEngine* engine = qmlEngine(inParent);
	QQmlContext* context = qmlContext(inParent);
	QQmlComponent nameComp(engine);
	nameComp.setData(R"(
		import QtQuick;
		import QtQuick.Controls;
		Item{
			implicitHeight: 25
			width: parent.width
			Text {
				anchors.fill: parent
				verticalAlignment: Text.AlignVCenter
				clip: true
				elide: Text.ElideRight
				text: model.name
			Component.onCompleted: {
				console.log("----Component.onCompleted",model.name)
			}
			Component.onDestruction: {
				console.log("----Component.onDestruction",model.name)
			}
			}
		}
   )", QUrl());
	QVariantMap initialProperties;
	initialProperties["parent"] = QVariant::fromValue(inParent);
	auto nameEditor = qobject_cast<QQuickItem*>(nameComp.createWithInitialProperties(initialProperties, context));
	nameEditor->setParentItem(inParent);
	return nameEditor;
}

QQuickItem* IPropertyHandleImpl::createValueEditor(QQuickItem* inParent)
{
	return nullptr;
}
