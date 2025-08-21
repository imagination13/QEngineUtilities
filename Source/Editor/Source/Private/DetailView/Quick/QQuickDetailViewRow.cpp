#include "QQuickDetailViewRow.h"
#include "QQuickDetailViewLayoutBuilder.h"
#include "QQuickDetailViewMananger.h"
#include "DetailView/PropertyHandleImpl/QObjectPropertyHandleImpl.h"

#include <QAssociativeIterable>
#include <QSequentialIterable>
#include <QQmlContext>

void IDetailViewRow::addChild(QSharedPointer<IDetailViewRow> inChild)
{
	mChildren << inChild;
	inChild->mParent = this;
}

void IDetailViewRow::clear()
{
	mChildren.clear();
}

QDetailViewRow_Property::QDetailViewRow_Property(QPropertyHandle* inHandle)
    : mHandle(inHandle)
{
    mPropertyTypeCustomization = QQuickDetailViewManager::Get()->getCustomPropertyType(mHandle->getType());
    if (QPropertyHandle::ParseType(mHandle->getType()) == QPropertyHandle::Object) {
        mClassLayoutCustomization = QQuickDetailViewManager::Get()->getCustomDetailLayout(mHandle->getType().metaObject());
    }
}

void QDetailViewRow_Root::addChild(QSharedPointer<IDetailViewRow> inChild)
{
    mChildren << inChild;
}

void QDetailViewRow_Root::setObject(QObject* inObject)
{
    mObject = inObject;
    mClassLayoutCustomization = QQuickDetailViewManager::Get()->getCustomDetailLayout(mObject->metaObject());
}

void QDetailViewRow_Root::attachChildren()
{
	if (mClassLayoutCustomization) {
		QQuickDetailViewLayoutBuilder builder(this);
		mClassLayoutCustomization->customizeDetails(mObject, &builder);
	}
	else {
		for (int i = 1; i < mObject->metaObject()->propertyCount(); i++) {
			QMetaProperty prop = mObject->metaObject()->property(i);
			QString propertyPath = prop.name();
			QPropertyHandle* handler = QPropertyHandle::FindOrCreate(mObject, propertyPath);
			QSharedPointer<IDetailViewRow> child(new QDetailViewRow_Property(handler));
			addChild(child);
			child->attachChildren();
		}
	}
}

void QDetailViewRow_Property::setupItem(QQuickItem* inParent)
{
	QQuickDetailViewHeaderRowBuilder builder(inParent);
    if (mPropertyTypeCustomization) {
        mPropertyTypeCustomization->customizeHeader(mHandle, &builder);
        return;
    }
	builder.makePropertyHeader(mHandle);
}

void QDetailViewRow_Property::attachChildren()
{
	auto type = QPropertyHandle::ParseType(mHandle->getType());
    if (type == QPropertyHandle::Sequential) {
		QVariant varList = mHandle->getValue();
		QSequentialIterable iterable = varList.value<QSequentialIterable>();
		for (int index = 0; index < iterable.size(); index++) {
			QString path = mHandle->getSubPath(QString::number(index));
			QPropertyHandle* handle = QPropertyHandle::FindOrCreate(mHandle->parent(), path);
			if (handle) {
				QSharedPointer<IDetailViewRow> child(new QDetailViewRow_Property(handle));
                addChild(child);
                child->attachChildren();
			}
		}
    }
	else if (type == QPropertyHandle::Associative) {
		QVariant varMap = mHandle->getValue();
		QAssociativeIterable iterable = varMap.value<QAssociativeIterable>();
		for (auto iter = iterable.begin(); iter != iterable.end(); ++iter) {
			QString path = mHandle->getSubPath(iter.key().toString());
			QPropertyHandle* handle = QPropertyHandle::FindOrCreate(mHandle->parent(), path);
			if (handle) {
				QSharedPointer<IDetailViewRow> child(new QDetailViewRow_Property(handle));
				addChild(child);
				child->attachChildren();
			}
		}
	}
	else if (type == QPropertyHandle::Object) {
		auto objectHandle = mHandle->asObject();
		QObject* object = objectHandle->getObject();
		if (object == nullptr)
			return;
        if (mClassLayoutCustomization) {
            QQuickDetailViewLayoutBuilder builder(this);
            mClassLayoutCustomization->customizeDetails(object, &builder);
        }
        else {
			for (int i = 1; i < object->metaObject()->propertyCount(); i++) {
				QMetaProperty prop = object->metaObject()->property(i);
				QString propertyPath = mHandle->getSubPath(prop.name());
				QPropertyHandle* handler = QPropertyHandle::FindOrCreate(mHandle->parent(), propertyPath);
				QSharedPointer<IDetailViewRow> child(new QDetailViewRow_Property(handler));
				addChild(child);
				child->attachChildren();
			}
        }
	}
    else if (type == QPropertyHandle::RawType){
		QQuickDetailViewLayoutBuilder builder(this);
        if(mPropertyTypeCustomization)
            mPropertyTypeCustomization->customizeChildren(mHandle, &builder);
    }
}

QDetailViewRow_Custom::QDetailViewRow_Custom(QQuickItem* inItem)
    : mRowItem(inItem)
{

}

void QDetailViewRow_Custom::setupItem(QQuickItem* inParent)
{
    mRowItem->setParentItem(inParent);
}
