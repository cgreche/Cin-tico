
//Layout
// File: uibase.h
// Last edit: 25/09/2017 03:22 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __LAYOUT_H__
#define __LAYOUT_H__

#include <vector>
#include <list>

enum SizeType
{
	SizeTypeItem = 0xfdffffff,
	SizeTypeAuto = 0xfeffffff,
	SizeTypeMax  = 0xffffffff,
	SizeTypePercent = 0xfcffffff,
};

#define PercentSize(x) ((0xfc<<24)|(x&0xffffff))

#define ItemSize Size(SizeTypeItem,SizeTypeItem)
#define AutoSize Size(SizeTypeAuto,SizeTypeAuto)
#define MaximumSize Size(SizeTypeMax,SizeTypeMax)
#define MakePercentType(x) ((0xfc<<24)|((x&0xff)<<16))
#define PercentValue(x) ((x>>16)&0xff)

inline bool isSizeItem(Size& size) { return size == ItemSize; }
inline bool isSizeAuto(Size& size) { return size == AutoSize; }
inline bool isSizeMaximum(Size& size) { return size == MaximumSize; }
inline bool isSizeTypeItem(u32 size) { return ((size>>24) & 0xff) == 0xfd; }
inline bool isSizeTypeAuto(u32 size) { return ((size>>24) & 0xff) == 0xfe; }
inline bool isSizeTypeMaximum(u32 size) { return ((size>>24) & 0xff) == 0xff; }
inline bool isSizeTypePercent(u32 size) { return ((size>>24) & 0xff) == 0xfc; }

class Control;

class LayoutItem;
class Layout;
class LayoutControlItem;
typedef std::vector<LayoutItem*> LayoutItemList;
typedef std::list<Layout*> LayoutList;

class LayoutItem
{
protected:
	Size m_size;
	int m_spacing;

public:
	friend class Layout;
	LayoutItem(Size size, int spacing):m_size(size),m_spacing(spacing) { }
	virtual void setParent(Control *parent) = 0;
	virtual void setPosition(const Point &position) = 0;
	virtual void setSize(const Size &size) = 0;
	virtual void setVisible(bool visible) = 0;
	virtual void setEnabled(bool enabled) = 0;
	virtual Point position() const = 0;
	virtual Size currentSize() const = 0;
	virtual Size getAutoSize() = 0;

	virtual Layout* layout();
	virtual Control* control();

	Size getRequiredSize();

	Size size() const { return m_size; }
	int spacing() const { return m_spacing; }
};

class LayoutItemControl : public LayoutItem
{
	Control* m_control;

public:
	friend class Layout;
	LayoutItemControl(Control& control, Size size, int spacing): LayoutItem(size,spacing), m_control(&control) { }

	virtual void setParent(Control *parent);
	virtual void setPosition(const Point &position);
	virtual void setSize(const Size &size);
	virtual void setVisible(bool visible);
	virtual void setEnabled(bool enabled);
	virtual Point position() const;
	virtual Size currentSize() const;
	virtual Size getAutoSize();

	virtual Control* control();
};

class LayoutItemLayout : public LayoutItem
{
	Layout *m_layout;

public:
	LayoutItemLayout(Layout& layout, Size size, int spacing): LayoutItem(size,spacing), m_layout(&layout) { }
	virtual void setParent(Control *parent);
	virtual void setPosition(const Point &position);
	virtual void setSize(const Size &size);
	virtual void setVisible(bool visible);
	virtual void setEnabled(bool enabled);
	virtual Point position() const;
	virtual Size currentSize() const;
	virtual Size getAutoSize();

	virtual Layout* layout();
};



class Layout
{
protected:
	int m_defSpacing;
	int m_topMargin;
	int m_leftMargin;
	int m_bottomMargin;
	int m_rightMargin;
	float m_alignment;
	LayoutItemList m_childList;
	Control *m_parentControl;
	Layout *m_parentLayout;

	Point m_position;
	Size m_currentSize;

	uint m_undefinedWidthCount;
	uint m_undefinedHeightCount;

	Size m_minSize;
	Size m_maxSize;
	Size m_definedSize;
	Size m_biggestControlSize;

	string m_tag; //for debugging purposes

public:
	static float left_align;
	static float center_align;
	static float right_align;

	Layout();
	virtual ~Layout();

	virtual void setParent(Control *parent);
	virtual void setPosition(const Point &position);
	virtual void setSize(const Size &size);
	virtual void setVisible(bool visible);
	virtual void setEnabled(bool enabled);
	virtual Point position() const;
	virtual Size size() const;
	virtual Size getAutoSize();

	virtual Layout* layout();
	
	void setMargin(int margin);
	void setMargin(int vMargin, int hMargin);
	void setMargin(int topMargin, int leftMargin, int bottomMargin, int rightMargin);
	void setTopMargin(int margin);
	void setLeftMargin(int margin);
	void setBottomMargin(int margin);
	void setRightMargin(int margin);
	void setDefaultSpacing(int spacing);
	void setAlignment(float alignment);

	void getMargins(int *pTopMargin, int *pLeftMargin, int *pBottomMargin, int *pRightMargin);

	void append(Control &control);
	void append(Control &control, const Size &size, int spacing = SizeTypeAuto);
	void remove(Control &control);
	void append(Layout &layout);
	void append(Layout &layout, const Size &size, int spacing = SizeTypeAuto);
	void remove(Layout &layout);

	Control *parentControl() const;

	void setLayoutTag(const string &tag) { m_tag = tag; }
	uint undefinedWidthCount() const { return m_undefinedWidthCount; }
	uint undefinedHeightCount() const { return m_undefinedHeightCount; }	
};

#endif