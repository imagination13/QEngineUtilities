#ifndef QQuickDetailView_h__
#define QQuickDetailView_h__

#include "QQuickTreeViewEx.h"

class QQuickDetailViewPrivate;

class QENGINEEDITOR_API QQuickDetailView: public QQuickTreeViewEx {
	Q_OBJECT
	QML_NAMED_ELEMENT(DetailView)
	Q_DISABLE_COPY(QQuickDetailView)
	Q_DECLARE_PRIVATE(QQuickDetailView)
	Q_PROPERTY(qreal SpliterPencent READ getSpliterPencent WRITE setSpliterPencent NOTIFY asSpliterPencentChanged FINAL)
	Q_PROPERTY(QObject* Object READ getObject WRITE setObject NOTIFY asObjectChanged FINAL)
public:
	QQuickDetailView(QQuickItem* parent = nullptr);
	qreal getSpliterPencent() const;
	void setSpliterPencent(qreal val);
	Q_INVOKABLE void setObject(QObject* inObject);
	QObject* getObject() const;
Q_SIGNALS:
	void asSpliterPencentChanged(qreal);
	void asObjectChanged(QObject*);
protected:
	void componentComplete() override;
};

#endif // QQuickDetailView_h__