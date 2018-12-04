
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
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGraphicsTimerView 构造/析构

CGraphicsTimerView::CGraphicsTimerView() noexcept
{
	// TODO: 在此处添加构造代码
	
	for (int i = 0; i < 100; i++)
	{
		for (int d = 0; d < 100; d++)//防止超过边界，转换方向,每个图形都重新初始化一下这个DirectionFlag数组
		{
			DirectionFlag[i][d] = 1;
		}
	}
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

	/*
	CRect rect;
	this->GetClientRect(rect);
	pDC->FillSolidRect(&rect, RGB(255, 255, 255));
	*/
	


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
				pDC->MoveTo(p1);
				pDC->LineTo(p2);
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
	this->SetTimer(1, 50, NULL);
}


void CGraphicsTimerView::OnEndmove()
{
	// TODO: 在此添加命令处理程序代码
	this->KillTimer(1);
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
		//DDALine(pDC, startPoint.x, startPoint.y, endPoint.x, endPoint.y, RGB(0, 0, 0));
		pDC->MoveTo(startPoint);
		pDC->LineTo(endPoint);
		//绘制新的直线段
		//DDALine(pDC, startPoint.x, startPoint.y, point.x, point.y, RGB(0, 0, 0));
		pDC->MoveTo(startPoint);
		pDC->LineTo(point);
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

			pDC->MoveTo(p1);
			pDC->LineTo(p2);
			//DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));

		}



		
		//开始求封闭多边形对应的bezier曲线上的离散点
		pointPrintList.RemoveAll();//使用前清空列表
		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		oldpen = pDC->SelectObject(&pen);
		for (int j = 0; j < pointsize; j++)
		{
			CPoint p1 = obj->points.GetAt(j);
			CPoint p2 = obj->points.GetAt((j + 1) % pointsize);
			CPoint p3 = obj->points.GetAt((j + 2) % pointsize);
			CPoint mid1;
			CPoint mid2;
			mid1.x = (p1.x + p2.x) / 2;
			mid1.y = (p1.y + p2.y) / 2;
			mid2.x = (p2.x + p3.x) / 2;
			mid2.y = (p2.y + p3.y) / 2;
			TempList.RemoveAll();//使用前清空列表
			TempList.Add(mid1);
			TempList.Add(p2);
			TempList.Add(mid2);
			BezierToPoints();
		}

		//开始画封闭多边形对应的bezier曲线
		pDC->MoveTo(pointPrintList.GetAt(0));
		for (int i = 1; i < pointPrintList.GetSize(); i++)
		{
			pDC->LineTo(pointPrintList.GetAt(i));
		}
		pDC->SelectObject(oldpen);

		//存取此次封闭多边形对应的bezier曲线
		MapObj* curveobj = new MapObj();
		curveobj->type = 2;
		for (int i = 0; i < pointPrintList.GetSize(); i++)
		{
			curveobj->points.Add(pointPrintList.GetAt(i));
		}
		curveList.Add(curveobj);//将此次图形的所有点保存下来
		pointPrintList.RemoveAll();//清空当前图形顶点列表，为下个图形做准备


	}


	CView::OnLButtonDblClk(nFlags, point);

}




void CGraphicsTimerView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rect;//边界值
	this->GetClientRect(rect);
	LeftBorder = 0;
	RightBorder = rect.right;
	TopBorder = 0;
	BottomBorder = rect.bottom;


	if (nIDEvent == 1) //对1号计时器到期时，进行操作
	{
		CDC* pDC = this->GetDC();
		
		/********************************************/
		this->GetClientRect(rect);
		pDC->FillSolidRect(&rect, RGB(255, 255, 255));
		/********************************************/


		for (int i = 0; i < objList.GetSize(); i++)//最多可以拥有一百个图形
		{
			MapObj* obj = (MapObj*)objList.GetAt(i);//得到一个图形
			MapObj* newObj = new MapObj();//准备一个新的图形，用于替换原有的图形顶点
			newObj->type = 1;
			if (obj->type == 1)//开始替换，首先计算每个顶点的新坐标
			{
				int pointsize = obj->points.GetSize();
				for (int j = 0; j < pointsize; j++)//遍历该图形的每一个顶点
				{
					CPoint p1 = obj->points.GetAt(j);//得到该图形的一个顶点，在points中，顶点从0开始计数，而实际应该从1开始计数，所以是判断j+1的奇偶性
					CPoint p2;//用于承接新的顶点
					if ((j + 1) % 2 == 0)//顶点为偶数编号
					{
						
						if ((3 * j*DirectionFlag[i][j] + p1.y <= TopBorder)|| (3 * j*DirectionFlag[i][j] + p1.y>=BottomBorder))
						{
							DirectionFlag[i][j] = -DirectionFlag[i][j];
						}
						p2.x = p1.x;
						p2.y = 3*(j+1)*DirectionFlag[i][j]+p1.y;
					}
					else if ((j + 1) % 2 == 1)//顶点为奇数编号
					{
						if ((3 * j*DirectionFlag[i][j] + p1.x<= LeftBorder) || (3 * j*DirectionFlag[i][j] + p1.x>=RightBorder))
						{
							DirectionFlag[i][j] = -DirectionFlag[i][j];
						}
						p2.x = 3*(j+1)*DirectionFlag[i][j]+p1.x;
						p2.y =p1.y;
					}
					newObj->points.Add(p2);
				}
			}

			/*
			pDC->SetROP2(R2_NOTXORPEN);//设置绘图模式为R2_NOT
			for (int j = 0; j < obj->points.GetSize(); j++)
			{
				CPoint p1 = obj->points.GetAt(j);
				CPoint p2 = obj->points.GetAt((j + 1) % obj->points.GetSize());
				DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));
			}
			*/
			
			for (int j = 0; j < newObj->points.GetSize(); j++)
			{
				CPoint p1 = newObj->points.GetAt(j);
				CPoint p2 = newObj->points.GetAt((j + 1) % newObj->points.GetSize());
				pDC->MoveTo(p1);
				pDC->LineTo(p2);
				//DDALine(pDC, p1.x, p1.y, p2.x, p2.y, RGB(0, 0, 0));
			}

			objList.SetAt(i, newObj);//用新的顶点替换原来的顶点


			//开始画bezier曲线
			/******************************************************************/
			CPen pen, *oldpen;
			pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			oldpen = pDC->SelectObject(&pen);


			/*
			//开始清除上一个多边形对应的bezier曲线
			MapObj* curveobj = (MapObj*)curveList.GetAt(i);//得到一个图形
			pDC->MoveTo(curveobj->points.GetAt(0));
			for (int i = 1; i < curveobj->points.GetSize(); i++)
			{
				pDC->LineTo(curveobj->points.GetAt(i));
			}
			*/
			
			
			//开始求新的封闭多边形对应的bezier曲线上的离散点
			pointPrintList.RemoveAll();//使用前清空列表
			int pointsize = newObj->points.GetSize();
			for (int j = 0; j < pointsize; j++)
			{
				CPoint p1 = newObj->points.GetAt(j);
				CPoint p2 = newObj->points.GetAt((j + 1) % pointsize);
				CPoint p3 = newObj->points.GetAt((j + 2) % pointsize);
				CPoint mid1;
				CPoint mid2;
				mid1.x = (p1.x + p2.x) / 2;
				mid1.y = (p1.y + p2.y) / 2;
				mid2.x = (p2.x + p3.x) / 2;
				mid2.y = (p2.y + p3.y) / 2;
				TempList.RemoveAll();//使用前清空列表
				TempList.Add(mid1);
				TempList.Add(p2);
				TempList.Add(mid2);
				BezierToPoints();
			}

			//存取新的封闭多边形对应的bezier曲线
			MapObj* newcurveobj = new MapObj();
			newcurveobj->type = 2;
			for (int i = 0; i < pointPrintList.GetSize(); i++)
			{
				newcurveobj->points.Add(pointPrintList.GetAt(i));
			}
			curveList.SetAt(i,newcurveobj);//将此次图形的所有点保存下来
			pointPrintList.RemoveAll();//清空当前图形顶点列表，为下个图形做准备

			//画新的封闭多边形对应的bezier曲线
			pDC->MoveTo(newcurveobj->points.GetAt(0));
			for (int i = 1; i < newcurveobj->points.GetSize(); i++)
			{
				pDC->LineTo(newcurveobj->points.GetAt(i));
			}
			pDC->SelectObject(oldpen);
			/******************************************************************/

		}
		
		this->ReleaseDC(pDC);
	}





	CView::OnTimer(nIDEvent);
}


void CGraphicsTimerView::BezierToPoints()
{
	// TODO: 在此处添加实现代码
	int nPoints = 50;//用50个点画出Bezier曲线
	double u = 0;
	double delt = 1 / (double)(nPoints);
	for (int i = 0; i < nPoints + 1; i++)
	{
		pointPrintList.Add(Bezier(u));
		u += delt;
	}
}


CPoint CGraphicsTimerView::Bezier(double u)
{
	// TODO: 在此处添加实现代码.
	int N = 2;
	int m, i;
	CPoint *R = new CPoint[N + 1];
	CPoint *Q = new CPoint[N + 1];
	CPoint P;
	/****************************************************/
	for (int i = 0; i < TempList.GetSize(); i++)
	{
		R[i] = TempList.GetAt(i);
	}
	/****************************************************/
	for (int m = N; m > 0; m--)
	{
		for (int i = 0; i < m; i++)
		{
			Q[i].x = R[i].x + (R[i + 1].x - R[i].x)*u;
			Q[i].y = R[i].y + (R[i + 1].y - R[i].y)*u;
		}
		for (int i = 0; i < m; i++)
		{
			R[i] = Q[i];
		}
	}
	P = R[0];
	delete R;
	delete Q;
	return P;
}
