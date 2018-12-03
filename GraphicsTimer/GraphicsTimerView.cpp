
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

void CGraphicsTimerView::OnDraw(CDC* /*pDC*/)
{
	CGraphicsTimerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
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
