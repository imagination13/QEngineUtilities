#include "QApplication"
#include "QQuickView"
#include "QQmlContext"
#include "QQuickItem"

#include "qvariant.h"
#include "qsequentialiterable.h"
#include "qbasicatomic.h"

#include "TestObject.h"
#include "DetailView/Quick/QQuickDetailView.h"

int main(int argc, char** argv) {
	QApplication app(argc, argv);

	TestObject obj;
	qmlRegisterType<QQuickDetailView>("Qt.DetailsView", 1, 0, "DetailsView");
	QQuickView view;
	view.setSource(QUrl::fromLocalFile("../../Examples/DetailViewExample/Main.qml"));
	view.show();
	view.rootObject()->setProperty("Object", QVariant::fromValue(&obj));
	return app.exec();
}