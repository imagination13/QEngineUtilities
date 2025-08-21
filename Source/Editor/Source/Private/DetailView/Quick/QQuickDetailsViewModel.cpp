#include "QQuickDetailViewModel.h"
#include "QQuickDetailViewRow.h"
#include "QQuickDetailViewLayoutBuilder.h"

QQuickDetailViewModel::QQuickDetailViewModel(QObject* parent)
	: QAbstractItemModel(parent) 
    , mRoot(new QDetailViewRow_Root)
{
}

QVariant QQuickDetailViewModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();
    IDetailViewRow* node = static_cast<IDetailViewRow*>(index.internalPointer());
    return node->name();
}

Qt::ItemFlags QQuickDetailViewModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex QQuickDetailViewModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    if (!parent.isValid()) {
        return createIndex(row, column, mRoot->mChildren[row].get());
    }
    return createIndex(row, column, mRoot->mChildren[parent.row()]->mChildren[row].get());
}

QModelIndex QQuickDetailViewModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();
    IDetailViewRow* node = static_cast<IDetailViewRow*>(index.internalPointer());
    if (node->mParent) {
        return createIndex(mRoot->mChildren.indexOf(node->mParent), 0, node->mParent);
    }
    return QModelIndex();
}

int QQuickDetailViewModel::rowCount(const QModelIndex& parent) const {
    if (!parent.isValid())
        return mRoot->mChildren.size();
    IDetailViewRow* node = static_cast<IDetailViewRow*>(parent.internalPointer());
    return node->mChildren.count();
}

void QQuickDetailViewModel::setObject(QObject* inObject)
{
    mObject = inObject;
    rebuildNode();
}

QObject* QQuickDetailViewModel::getObject() const
{
    return mObject;
}

void QQuickDetailViewModel::rebuildNode()
{
    beginResetModel();
    mRoot->clear();
    mRoot->setObject(mObject);
    mRoot->attachChildren();
    endResetModel();
}

int QQuickDetailViewModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QHash<int, QByteArray> QQuickDetailViewModel::roleNames() const {
    return {
        { Roles::name,"name" },
    };
}
