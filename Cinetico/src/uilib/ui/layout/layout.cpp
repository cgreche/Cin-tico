
//Layout
// File: layout.cpp
// Last edit: 25/09/2017 03:22 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	float Layout::left_align(0.0f);
	float Layout::center_align(0.5f);
	float Layout::right_align(1.0f);



	//LayoutItem

	Layout *LayoutItem::layout()
	{
		return NULL;
	}

	Control *LayoutItem::control()
	{
		return NULL;
	}

	Size LayoutItem::getRequiredSize()
	{
		Size wantedSize = this->size();
		Size retSize = wantedSize;

		//Firstly, process the width
		//Check if size is defined as constant value
		if (isSizeTypeItem(wantedSize.width())) {
			retSize.setWidth(currentSize().width());
		}
		else if (isSizeTypeAuto(wantedSize.width())) {
			//Check if size if defined as auto
			Size autoSize = getAutoSize();
			if (layout()) {
				if (layout()->undefinedWidthCount() == 0)
					retSize.setWidth(autoSize.width());
				else {
					retSize.setWidth(SizeTypeMax);
				}
			}
			else {
				retSize.setWidth(autoSize.width());
			}
		}
		else {
			retSize.setWidth(wantedSize.width());
		}

		//height
		if (isSizeTypeItem(wantedSize.height())) {
			retSize.setHeight(currentSize().height());
		}
		else if (isSizeTypeAuto(size().height())) {
			//Check if size if defined as auto
			Size autoSize = getAutoSize();
			if (layout()) {
				if (layout()->undefinedHeightCount() == 0)
					retSize.setHeight(autoSize.height());
				else {
					retSize.setHeight(SizeTypeMax);
				}
			}
			else {
				retSize.setHeight(autoSize.height());
			}
		}
		else {
			retSize.setHeight(wantedSize.height());
		}

		return retSize;
	}

	//LayoutItemControl

	void LayoutItemControl::setParent(Control *parent) { m_control->setParent(parent); }
	void LayoutItemControl::setPosition(const Point &position) { m_control->setPosition(position); }
	void LayoutItemControl::setSize(const Size &size) { m_control->setSize(size); }
	void LayoutItemControl::setVisible(bool visible) { m_control->setVisible(visible); }
	void LayoutItemControl::setEnabled(bool enabled) { m_control->setEnabled(enabled); }
	Point LayoutItemControl::position() const { return m_control->position(); }
	Size LayoutItemControl::currentSize() const { return m_control->size(); }
	Size LayoutItemControl::getAutoSize() { return m_control->getAutoSize(); }
	Control* LayoutItemControl::control() { return m_control; }


	//LayoutItemLayout

	void LayoutItemLayout::setParent(Control *parent) { m_layout->setParent(parent); }
	void LayoutItemLayout::setPosition(const Point &position) { m_layout->setPosition(position); }
	void LayoutItemLayout::setSize(const Size &size) { m_layout->setSize(size); }
	void LayoutItemLayout::setVisible(bool visible) { m_layout->setVisible(visible); }
	void LayoutItemLayout::setEnabled(bool enabled) { m_layout->setEnabled(enabled); }
	Point LayoutItemLayout::position() const { return m_layout->position(); }
	Size LayoutItemLayout::currentSize() const { return m_layout->size(); }
	Size LayoutItemLayout::getAutoSize() { return m_layout->getAutoSize(); }
	Layout* LayoutItemLayout::layout() { return m_layout; }






	//Layout

	Layout::Layout()
		: m_topMargin(0), m_leftMargin(0), m_bottomMargin(0), m_rightMargin(0),
		m_defSpacing(5), m_alignment(0.0f),
		m_parentControl(NULL), m_parentLayout(NULL),
		m_position(0, 0), m_currentSize(0, 0)
	{
	}

	Layout::~Layout()
	{
		//	setVisible(false); //Don't show controls being disappended from the windows
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.control())
				child.control()->m_parentLayout = NULL;
			if (child.layout())
				child.layout()->m_parentLayout = NULL;
			delete &child;
		}
	}

	void Layout::setParent(Control *parent)
	{
		if (parent == m_parentControl)
			return;
		m_parentControl = parent;
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setParent(parent);
		}
	}

	void Layout::setPosition(const Point &position)
	{
		Point oldPos = m_position;
		m_position = position;

		if (m_childList.empty())
			return;

		Point diffPos = position - oldPos;

		LayoutItemList::iterator it;
		//Move objects
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;

			Point childPos = child.position() + diffPos;
			child.setPosition(childPos);
		}
	}

	void Layout::setSize(const Size &size)
	{
	}

	void Layout::setVisible(bool visible)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setVisible(visible);
		}
	}

	void Layout::setEnabled(bool enabled)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setEnabled(enabled);
		}
	}

	Point Layout::position() const
	{
		return m_position;
	}

	Size Layout::size() const
	{
		return m_currentSize;
	}

	Layout *Layout::layout()
	{
		return this;
	}

	void Layout::setMargin(int margin)
	{
		m_topMargin = m_leftMargin = m_bottomMargin = m_rightMargin = margin;
	}

	void Layout::setMargin(int vMargin, int hMargin)
	{
		m_topMargin = m_bottomMargin = vMargin;
		m_leftMargin = m_rightMargin = hMargin;
	}

	void Layout::setMargin(int topMargin, int leftMargin, int bottomMargin, int rightMargin)
	{
		m_topMargin = topMargin;
		m_leftMargin = leftMargin;
		m_bottomMargin = bottomMargin;
		m_rightMargin = rightMargin;
	}

	void Layout::setTopMargin(int margin)
	{
		m_topMargin = margin;
	}

	void Layout::setLeftMargin(int margin)
	{
		m_leftMargin = margin;
	}

	void Layout::setBottomMargin(int margin)
	{
		m_bottomMargin = margin;
	}

	void Layout::setRightMargin(int margin)
	{
		m_rightMargin = margin;
	}

	void Layout::setDefaultSpacing(int spacing)
	{
		m_defSpacing = spacing;
	}

	void Layout::getMargins(int *pTopMargin, int *pLeftMargin, int *pBottomMargin, int *pRightMargin)
	{
		if (pTopMargin) *pTopMargin = m_topMargin;
		if (pLeftMargin) *pLeftMargin = m_leftMargin;
		if (pBottomMargin) *pBottomMargin = m_bottomMargin;
		if (pRightMargin) *pRightMargin = m_rightMargin;
	}

	void Layout::setAlignment(float alignment)
	{
		m_alignment = max(0.0f, min(1.0f, alignment));
	}


	void Layout::append(Control& control)
	{
		Size controlSize = control.size();
		//todo: change the way it is decided to be the AutoSize?
		if (controlSize == Size(0, 0))
			controlSize = AutoSize;
		else
			controlSize = ItemSize;
		append(control, controlSize, SizeTypeAuto);
	}

	void Layout::append(Control& control, const Size &size, int spacing)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.control() && child.control() == &control)
				return;
		}

		m_childList.push_back(new LayoutItemControl(control, size, spacing));
		control.setParent(parentControl());
		control.m_parentLayout = this; //must be set after setParent()
	}

	void Layout::remove(Control &control)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.control() && child.control() == &control) {
				child.control()->m_parentLayout = NULL; //must be set before setParent()
				child.control()->setParent(NULL);
				delete &child;
				m_childList.erase(it);
				return;
			}
		}
	}

	void Layout::append(Layout& layout)
	{
		append(layout, AutoSize, SizeTypeAuto);
	}

	void Layout::append(Layout& layout, const Size &size, int spacing)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.layout() && child.layout() == &layout)
				return;
		}

		m_childList.push_back(new LayoutItemLayout(layout, size, spacing));
		layout.setParent(parentControl());
		layout.m_parentLayout = this;
	}

	void Layout::remove(Layout &layout)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.layout() && child.layout() == &layout) {
				child.layout()->m_parentLayout = NULL; //must be set before setParent()
				child.layout()->setParent(NULL);
				delete &child;
				m_childList.erase(it);
				return;
			}
		}
	}

	Size Layout::getAutoSize()
	{
		Size ret;
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			Size autoSize = child.getAutoSize();
			Point pos = child.position();
			int width = pos.x() + autoSize.width();
			int height = pos.y() + autoSize.height();
			if (width > ret.width())
				ret.setWidth(width);
			if (height > ret.height())
				ret.setHeight(height);
		}
		return ret;
	}

	Control *Layout::parentControl() const
	{
		return m_parentControl;
	}

}