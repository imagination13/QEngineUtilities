#include "DetailView/PropertyHandleImpl/QEnumPropertyHandleImpl.h"
#include <QComboBox>
#include "DetailView/QPropertyHandle.h"
#include <QMetaEnum>
#include <QWidget>
#include <QBoxLayout>

QEnumPropertyHandleImpl::QEnumPropertyHandleImpl(QPropertyHandle* InHandle)
	:IPropertyHandleImpl(InHandle) {
	const QMetaObject* metaObj = mHandle->getType().metaObject();
	if (metaObj){
		const QMetaEnum& metaEnum = metaObj->enumerator(metaObj->indexOfEnumerator(QString(mHandle->getType().name()).split("::").last().toLocal8Bit()));
		for (int i = 0; i < metaEnum.keyCount(); i++) {
			mNameToValueMap[metaEnum.key(i)] = metaEnum.value(i);
			mKeys << metaEnum.key(i);
		}
	}
}

QWidget* QEnumPropertyHandleImpl::generateValueWidget() {
	QComboBox* comboBox = new QComboBox();
	for(auto enumPair :mNameToValueMap.asKeyValueRange()){
		comboBox->addItem(enumPair.first);
	}
	mHandle->bind(
		comboBox,
		&QComboBox::currentTextChanged,
		[comboBox, this]() {
		return mNameToValueMap.value(comboBox->currentText());
	},
		[comboBox, this](QVariant var) {
		comboBox->setCurrentText(mNameToValueMap.key(var.toInt()));
	}
	);
	QWidget* valueContent = new QWidget;
	valueContent->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
	QHBoxLayout* valueContentLayout = new QHBoxLayout(valueContent);
	valueContentLayout->setAlignment(Qt::AlignLeft);
	valueContentLayout->setContentsMargins(10, 2, 10, 2);
	valueContentLayout->setSpacing(2);
	valueContentLayout->addWidget(comboBox);
	mHandle->generateAttachButtonWidget(valueContentLayout);
	return valueContent;
}

QQuickItem* QEnumPropertyHandleImpl::createValueEditor(QQuickItem* inParent)
{
	QQmlEngine* engine = qmlEngine(inParent);
	QQmlContext* context = qmlContext(inParent);
	QQmlComponent comp(engine);
	comp.setData(R"(
				import QtQuick;
				import QtQuick.Controls;
				import "qrc:/Resources/Qml"
				TextComboBox{
					width: parent.width
				}
			)", QUrl());
	QVariantMap initialProperties;
	initialProperties["parent"] = QVariant::fromValue(inParent);
	auto valueEditor = qobject_cast<QQuickItem*>(comp.createWithInitialProperties(initialProperties, context));
	qDebug() << comp.errorString();
	valueEditor->setParentItem(inParent);
	valueEditor->setProperty("value", mHandle->getValue());
	valueEditor->setProperty("model", mKeys);
	QObject::connect(valueEditor, SIGNAL(asValueChanged(QVariant)), mHandle, SLOT(setVar(QVariant)));
	QObject::connect(mHandle, SIGNAL(asRequestRollback(QVariant)), valueEditor, SLOT(setValue(QVariant)));
	return valueEditor;
}
