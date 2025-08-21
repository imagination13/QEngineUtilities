#include "DetailView/Widget/QDetailLayoutBuilder.h"
#include "DetailView/Widget/QDetailView.h"
#include "DetailView/Widget/QDetailViewRow.h"


QDetailViewRow* QDetailLayoutBuilder::newChildRow() {
	return mDetailView->addTopLevelRow();
}

QDetailViewRow* QRowLayoutBuilder::newChildRow() {
	return mRow->addChildRow();
}
