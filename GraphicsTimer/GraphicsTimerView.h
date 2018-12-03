
// GraphicsTimerView.h: CGraphicsTimerView 类的接口
//

#pragma once
#include "math.h"
#include "stdafx.h"
#include "MapObj.h"

class CGraphicsTimerView : public CView
{
protected: // 仅从序列化创建
	CGraphicsTimerView() noexcept;
	DECLARE_DYNCREATE(CGraphicsTimerView)

// 特性
public:
	CGraphicsTimerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
//属性
public:
	int type;
	CPoint startPoint;//画一段折线的起始点
	CPoint endPoint;//画一段折线的终止点
	bool boolLButtonDown;//标记鼠标左键是否按下
	CArray<CPoint, CPoint> pointList;//多边形顶点列表
	CArray<CPoint, CPoint> pointPrintList;//曲线顶点列表
	CArray<CPoint, CPoint> TempList;//临时顶点列表（中点-顶点-中点）
	CObArray objList;//每画一个图形，一个Obj,链成ObjList
	CObArray curveList;//每画一个多边形的内切bezier曲线，一个curveObj,链成curveList
	int LeftBorder;//视窗的左边界
	int RightBorder;//视窗的右边界
	int TopBorder;//视窗的上边界
	int BottomBorder;//视窗的下边界
	int DirectionFlag[100][100];
// 实现
public:
	virtual ~CGraphicsTimerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	void DDALine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color);
	afx_msg void OnDrawpolygon();
	afx_msg void OnStartmove();
	afx_msg void OnEndmove();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void BezierToPoints();
	CPoint Bezier(double u);
};

#ifndef _DEBUG  // GraphicsTimerView.cpp 中的调试版本
inline CGraphicsTimerDoc* CGraphicsTimerView::GetDocument() const
   { return reinterpret_cast<CGraphicsTimerDoc*>(m_pDocument); }
#endif

