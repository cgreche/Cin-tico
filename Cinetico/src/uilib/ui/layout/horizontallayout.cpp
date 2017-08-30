
//HorizontalLayout
// File: horizontallayout.cpp
// Last edit: 16/06/2017 22:15 (UTC-3)
// Author: CGR

#include "../uibase.h"

//Horizontal Layout
HorizontalLayout::HorizontalLayout()
{
}

HorizontalLayout::~HorizontalLayout()
{
}

void HorizontalLayout::setSize(const Size &size)
{
	m_currentSize = size;

	Point curPos = m_position;
	Size curSize = size;
	curPos.setX(curPos.x()+m_leftMargin);
	curPos.setY(curPos.y()+m_topMargin);
	curSize.setWidth(curSize.width() - (m_leftMargin+m_rightMargin));
	curSize.setHeight(curSize.height() - (m_topMargin+m_bottomMargin));

	calcMinMaxSize();

	LayoutItemList::iterator it;

	//Move and size objects
	for(it = m_childList.begin(); it != m_childList.end(); ++it) {
		LayoutItem &child = **it;

		Layout *layout = child.layout();
		Control *control = child.control();

		Size childSize = child.getRequiredSize();

		if(isSizeTypeMaximum(childSize.width()))
			childSize.setWidth(max(curSize.width()-m_definedSize.width(),0)/m_undefinedWidthCount);
		else if(isSizeTypePercent(childSize.width()))
			childSize.setWidth((curSize.width()*PercentValue(childSize.width()))/100);
		if(isSizeTypeMaximum(childSize.height()))
			childSize.setHeight(curSize.height());
		else if(isSizeTypePercent(childSize.height()))
			childSize.setHeight((curSize.height()*PercentValue(childSize.height()))/100);

		int posX = curPos.x();
		int posY = curPos.y()+(int)((curSize.height()-childSize.height())*m_alignment);
		child.setPosition(Point(posX,posY));
		child.setSize(childSize);

		int spacing = isSizeTypeAuto(child.spacing()) ? m_defSpacing : child.spacing();
		curPos.setX(curPos.x() + childSize.width() + spacing);
	}
}

Size HorizontalLayout::getAutoSize()
{
	Size ret;
	calcMinMaxSize();

	ret.setWidth(m_minSize.width() + m_leftMargin+m_rightMargin);
	ret.setHeight(m_maxSize.height() + m_topMargin+m_bottomMargin);
	return ret;
}