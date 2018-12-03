
// GraphicsTimerView.cpp: CGraphicsTimerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "GraphicsTimer.h"
#endif

#include "GraphicsTimerDoc.h"
#include "GraphicsTimerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGraphicsTimerView

IMPLEMENT_DYNCREATE(CGraphicsTimerView, CView)

BEGIN_MESSAGE_MAP(CGraphicsTimerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGraphicsTimerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DrawPolygon, &CGraphicsTimerView::OnDrawpolygon)
	ON_COMMAND(ID_StartMove, &CGraphicsTimerView::OnStartmove)
	ON_COMMAND(ID_EndMove, &CGraphicsTimerView::OnEndmove)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CGraphicsTimerView 构造/析构

CGraphicsTimerView::CGraphicsTimerView() noexcept
{
	// TODO: 在此处添加构造代码

}

CGraphicsTimerView::~CGraphicsTimerView()
{
}

BOOL CGraphicsTimerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGraphicsTimerView 绘图

void CGraphicsTimerView::OnDraw(CDC* pDC)
{
	CGraphicsTimerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	for (int i = 0; i < objList.GetSize(); i++)
	{
		
		MapObj* obj = (MapObj*)objList.GetAt(i);
		if (obj->type == 1)
		{
			int pointsize = obj->points.GetSize();
			for (int j = 0; j < pointsize; j++)
			{
				CPoint p1 = obj->points.GetAt(j);
				CPoint p2 = obj->points.GetAt((j + 1) % pointsize);
				DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));
			}
		}
		
	}
}


// CGraphicsTimerView 打印


void CGraphicsTimerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGraphicsTimerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGraphicsTimerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGraphicsTimerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGraphicsTimerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGraphicsTimerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGraphicsTimerView 诊断

#ifdef _DEBUG
void CGraphicsTimerView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphicsTimerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphicsTimerDoc* CGraphicsTimerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphicsTimerDoc)));
	return (CGraphicsTimerDoc*)m_pDocument;
}
#endif //_DEBUG


// CGraphicsTimerView 消息处理程序


void CGraphicsTimerView::DDALine(CDC *pDC, int x1, int y1, int x2, int y2, COLORREF color)
{
	// TODO: 在此处添加实现代码.
	double dx, dy, e, x, y;
	dx = x2 - x1;
	dy = y2 - y1;
	e = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
	dx /= e;
	dy /= e;
	x = x1;
	y = y1;
	pDC->MoveTo(x1, y1);
	for (int i = 1; i <= e; i++)
	{
		pDC->SetPixel((int)(x + 0.5), (int)(y + 0.5), color);
		x += dx;
		y += dy;
	}
}


void CGraphicsTimerView::OnDrawpolygon()
{
	// TODO: 在此添加命令处理程序代码
	type = 1;//1表示绘制多边形区域
}


void CGraphicsTimerView::OnStartmove()
{
	// TODO: 在此添加命令处理程序代码
}


void CGraphicsTimerView::OnEndmove()
{
	// TODO: 在此添加命令处理程序代码
}


void CGraphicsTimerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	boolLButtonDown = true;
	if (type == 1)
	{
		this->SetCapture();//捕捉鼠标
		//设置开始点和终止点，此时为同一点
		startPoint = point;
		endPoint = point;
		pointList.Add(point);
	}


	CView::OnLButtonDown(nFlags, point);
}


void CGraphicsTimerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	boolLButtonDown = false;
	ReleaseCapture();//释放鼠标


	CView::OnLButtonUp(nFlags, point);
}

void CGraphicsTimerView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (type == 1 && pointList.GetSize() != 0)
	{
		CDC* pDC = this->GetDC();//构造设备环境对象
		pDC->SetROP2(R2_NOTXORPEN);//设置绘图模式为R2_NOT
		//重新绘制前一个鼠标移动消息处理函数绘制的直线段,因为绘图模式的原因，结果就是擦除了该线段
		DDALine(pDC, startPoint.x, startPoint.y, endPoint.x, endPoint.y, RGB(0, 0, 0));
		//绘制新的直线段
		DDALine(pDC, startPoint.x, startPoint.y, point.x, point.y, RGB(0, 0, 0));
		//保存新的直线段终点
		endPoint = point;
	}

	CView::OnMouseMove(nFlags, point);
}



void CGraphicsTimerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (type == 1)
	{
		ReleaseCapture();//释放鼠标
		boolLButtonDown = FALSE;
		MapObj* obj = new MapObj();
		obj->type = 1;
		for (int i = 0; i < pointList.GetSize(); i++)
		{
			obj->points.Add(pointList.GetAt(i));
		}
		objList.Add(obj);//将此次图形的所有点保存下来
		pointList.RemoveAll();//清空当前图形顶点列表，为下个图形做准备

		CDC* pDC = this->GetDC();
		int pointsize = obj->points.GetSize();
		for (int j = 0; j < pointsize; j++)
		{
			CPoint p1 = obj->points.GetAt(j);
			CPoint p2 = obj->points.GetAt((j + 1) % pointsize);
			DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));
		}
	}


	CView::OnLButtonDblClk(nFlags, point);

}


