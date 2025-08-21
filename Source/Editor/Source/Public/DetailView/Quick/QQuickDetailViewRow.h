#ifndef QQuickDetailViewRow_h__
#define QQuickDetailViewRow_h__

#include <QObject>
#include <QMap>
#include <QAbstractItemModel>
#include <QMetaProperty>
#include <QQuickItem>
#include "QPropertyHandle.h"
#include "IQuickPropertyTypeCustomization.h"
#include "IQuickDetailCustomization.h"
#include "QEngineEditorAPI.h"

class QENGINEEDITOR_API IDetailViewRow {
    friend class QQuickDetailViewModel;
public:
    virtual QString name() = 0;
    virtual void setupItem(QQuickItem* inParent){}
    virtual void attachChildren() {}
    virtual void addChild(QSharedPointer<IDetailViewRow> inChild);
    void clear();
protected:
    IDetailViewRow* mParent = nullptr;
    QList<QSharedPointer<IDetailViewRow>> mChildren;
};

class QENGINEEDITOR_API QDetailViewRow_Root : public IDetailViewRow {
	friend class QQuickDetailViewModel;
public:
    virtual QString name() { return "Root"; }
	virtual void setupItem(QQuickItem* inParent) {}
	void addChild(QSharedPointer<IDetailViewRow> inChild);
    void setObject(QObject* inObject);
    void attachChildren();
protected:
    QObject* mObject;
    QSharedPointer<IQuickDetailCustomization> mClassLayoutCustomization;
};

class QENGINEEDITOR_API QDetailViewRow_Property : public IDetailViewRow {
public:
    QDetailViewRow_Property(QPropertyHandle* inHandle);
protected:
    QString name() override { return mHandle->getName(); }
    void setupItem(QQuickItem* inParent) override;
    void attachChildren();
protected:
    QPropertyHandle* mHandle = nullptr;
    QSharedPointer<IQuickPropertyTypeCustomization> mPropertyTypeCustomization;
    QSharedPointer<IQuickDetailCustomization> mClassLayoutCustomization;
};

class QENGINEEDITOR_API QDetailViewRow_Custom : public IDetailViewRow {
public:
    QDetailViewRow_Custom(QQuickItem* inItem);
protected:
	QString name() override { return "Custom"; }
	void setupItem(QQuickItem* inParent) override;
private:
    QQuickItem* mRowItem = nullptr;
};

#endif // QQuickDetailViewRow_h__
