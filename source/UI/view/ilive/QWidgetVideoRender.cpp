
#include "QWidgetVideoRender.h"
#include "VideoService.h"
#include "Context.h"

#include <QMouseEvent>
#include <QAction>


#define VIEW_BKG_COLOR 0x60	//默认背景颜色值

#define FPS_Count 10 //每渲染10次，更新一次fps

QWidgetVideoRender *QWidgetVideoRender::main_view_ = nullptr;
extern QWidget * mainWindow;
extern QWidget * mainSWindow;

static void _CopyBits2Tex_None_0(uint8* pDst, uint8* pSrc, uint32 uLen, const SIZE & size)
{
	memcpy(pDst, pSrc, uLen);
}

static void _CopyBits2Tex_None_90(uint8* pDst, uint8* pSrc, uint32 uLen, const SIZE & size)
{
	int n = 0;
	int linesize = size.cx * 3;

	for (int x = 0; x < size.cx; x++)
	{
		for (int y = size.cy - 1; y >= 0; y--)
		{
			memcpy(pDst + n, pSrc + linesize * y + 3 * x, 3);
			n += 3;
		}
	}
}

static void _CopyBits2Tex_None_180(uint8* pDst, uint8* pSrc, uint32 uLen, const SIZE & size)
{
	int n = 0;
	int linesize = size.cx * 3;

	for (int x = 0; x < size.cx; x++)
	{
		for (int y = 0; y < size.cy; y++)
		{
			memcpy(pDst + linesize * (size.cy - y - 1) + 3 * x, pSrc + linesize * y + 3 * (size.cx - x - 1), 3);
		}
	}
}

static void _CopyBits2Tex_None_270(uint8* pDst, uint8* pSrc, uint32 uLen, const SIZE & size)
{
	int n = 0;
	int linesize = size.cx * 3;

	for (int x = size.cx - 1; x >= 0; x--)
	{
		for (int y = 0; y < size.cy; y++)
		{
			memcpy(pDst + n, pSrc + linesize * y + 3 * x, 3);
			n += 3;
		}
	}
}

static bool _ResizeWithMendBlack(uint8* pDst, uint8* pSrc, uint32 uDstLen, uint32 uSrcLen, const SIZE & dstSize, const SIZE & srcSize, uint32 bpp)
{
	if (!pDst || !pSrc)
		return false;

	if (uDstLen == 0 || uDstLen != dstSize.cx * dstSize.cy * bpp)
		return false;

	if (uSrcLen == 0 || uSrcLen != srcSize.cx * srcSize.cy * bpp)
		return false;

	if (dstSize.cx < srcSize.cx)
		return false;

	if (dstSize.cy < srcSize.cy)
		return false;

	uint32 dstLineblockSize = dstSize.cx * bpp;
	uint32 srcLineblockSize = srcSize.cx * bpp;

	int mendCxLeftEnd = dstSize.cx > srcSize.cx ? (dstSize.cx - srcSize.cx) / 2 : 0;
	int mendCyToEnd = dstSize.cy > srcSize.cy ? (dstSize.cy - srcSize.cy) / 2 : 0;

	for (int y = 0; y < dstSize.cy; y++)
	{
		if (y >= mendCyToEnd && y < mendCyToEnd + srcSize.cy)
		{
			if (mendCxLeftEnd > 0)
				memcpy(pDst + (bpp * mendCxLeftEnd), pSrc, srcLineblockSize);
			else
				memcpy(pDst, pSrc, srcLineblockSize);
			pSrc += srcLineblockSize;
		}
		pDst += dstLineblockSize;
	}
	return true;
}

QWidgetVideoRender::QWidgetVideoRender(QWidget* parent/* = 0*/, Qt::WindowFlags f/* = 0*/)
	:QWidget(parent, f)
	,m_colorFormat(COLOR_FORMAT_RGB24)
	,m_frameDataBufLen(0)
	,m_pFrameDataBuf(NULL)
	,m_pBkgDataBuf(NULL)
	,m_nRenderDataBufLen(0)
	,m_pRenderDataBuf(NULL)
	,m_pParentWidget(parent)
	,m_bPause(false)
	,m_lastClock(0)
	,m_nFpsCounter(FPS_Count)
	,m_nFps(0)
	,m_pRootView(nullptr)
	,m_winWidth(0)
	,m_winHeight(0)
{
	hMemDC = NULL;
	hDC = NULL;
	hMemBitmap = NULL;
	//setFixedSize(200, 150);
	//setStyleSheet(".QWidget{background-color:gray;}");
	setAttribute(Qt::WA_PaintOnScreen);
	render_menu_ = new QMenu(this);
	pActEnterMain = new QAction(QStringLiteral("设为主画面"), render_menu_);
	//pActStopSpeaking = new QAction(QStringLiteral("终止发言"), render_menu_);
	pActStopCamera = new QAction(QStringLiteral("关闭摄像头"), render_menu_);
	pActStopMic= new QAction(QStringLiteral("关闭麦克风"), render_menu_);

	pActExitMain = new QAction(QStringLiteral("退出主画面"), render_menu_);
	pActOpenCamera = new QAction(QStringLiteral("打开摄像头"), render_menu_);
	pActOpenMic = new QAction(QStringLiteral("打开麦克风"), render_menu_);
	pActEndInteract = new QAction(QStringLiteral("结束互动"), render_menu_);

	render_menu_->addAction(pActEnterMain);
	render_menu_->addAction(pActExitMain);
	render_menu_->addAction(pActStopCamera);
	render_menu_->addAction(pActOpenCamera);
	render_menu_->addAction(pActStopMic);
	render_menu_->addAction(pActOpenMic);
	render_menu_->addAction(pActEndInteract);

	render_menu_->setStyleSheet(".QMenu{background-color:#444444;\
		color: rgb(255,255,255);\
		font: 9pt \"微软雅黑\";}");
	connect(pActEnterMain, &QAction::triggered, this, [this] { emit doubleClicked(m_renderIndex); m_bFullScreen = true; });
	//connect(pActStopSpeaking, &QAction::triggered, this, [this] { emit actSpeaking(m_renderIndex, false); });
	connect(pActStopCamera, &QAction::triggered, this, [this] { emit actCamera(m_renderIndex, false); m_bCamera = false; });
	connect(pActStopMic, &QAction::triggered, this, [this] { emit actMic(m_renderIndex, false); m_bMic = false; });
	connect(pActExitMain, &QAction::triggered, this, [this] { emit escPressed(m_renderIndex); m_bFullScreen = false; });
	connect(pActOpenCamera, &QAction::triggered, this, [this] { emit actCamera(m_renderIndex, true); m_bCamera = true; });
	connect(pActOpenMic, &QAction::triggered, this, [this] { emit actMic(m_renderIndex, true); m_bMic = true; });
	connect(pActEndInteract, &QAction::triggered, this, [this] { emit actEnd(m_renderIndex);  });
	//setUpdatesEnabled(false);
}

QWidgetVideoRender::~QWidgetVideoRender()
{
	if (hMemDC)
	{
		DeleteObject(hMemDC);
		hMemDC = NULL;
	}
	if (hDC)
	{
		ReleaseDC(hWnd, hDC);
		hDC = NULL;
	}
	if (hMemBitmap)
	{
		DeleteObject(hMemBitmap);
		hMemBitmap = NULL;
	}
	SafeDelete(m_pFrameDataBuf);
	SafeDelete(m_pBkgDataBuf);
	SafeDelete(m_pRenderDataBuf);
	if (m_pRootView)
	{
		m_pRootView->uninit();
		m_pRootView->destroy();
	}

}

void QWidgetVideoRender::DoRender( const LiveVideoFrame *pFrameData )
{
	if (m_bPause)
	{
		return;
	}

	if (pFrameData == NULL || pFrameData->dataSize == 0)
	{		
		return;
	}
	assert(pFrameData->desc.colorFormat == COLOR_FORMAT_RGB24);//SDK1.3版本，demo渲染模块只支持渲染COLOR_FORMAT_RGB24格式的图像。
	

	if (m_frameDataBufLen!=pFrameData->dataSize)
	{
		if (m_pFrameDataBuf)
		{
			SafeDeleteArr(m_pFrameDataBuf)
		}
		if (m_pBkgDataBuf)
		{
			SafeDeleteArr(m_pBkgDataBuf);
		}

		m_colorFormat = pFrameData->desc.colorFormat;
		m_frameDataBufLen = pFrameData->dataSize;
		m_pFrameDataBuf = new uint8[m_frameDataBufLen];
		m_pBkgDataBuf = new uint8[m_frameDataBufLen];
		memset(m_pFrameDataBuf, 0, m_frameDataBufLen);
		memset(m_pBkgDataBuf, VIEW_BKG_COLOR, m_frameDataBufLen);
	}

	static void (* fun_table[4])(uint8*, uint8*, UINT, const SIZE &) = {&_CopyBits2Tex_None_0, &_CopyBits2Tex_None_90, &_CopyBits2Tex_None_180, &_CopyBits2Tex_None_270};

	if(pFrameData->desc.rotate == 0)//0
	{
		m_frameWidth = pFrameData->desc.width;
		m_frameHeight = pFrameData->desc.height;
	}
	else if(pFrameData->desc.rotate == 1)//90
	{
		m_frameWidth = pFrameData->desc.height;
		m_frameHeight = pFrameData->desc.width;
	}
	else if(pFrameData->desc.rotate == 2)//180
	{
		m_frameWidth = pFrameData->desc.width;
		m_frameHeight = pFrameData->desc.height;
	}
	else if(pFrameData->desc.rotate == 3)//270
	{
		m_frameWidth = pFrameData->desc.height;
		m_frameHeight = pFrameData->desc.width;
	}

	const SIZE size = {pFrameData->desc.width, pFrameData->desc.height};
	memset(m_pFrameDataBuf, 0, m_frameDataBufLen);
	fun_table[pFrameData->desc.rotate](m_pFrameDataBuf, pFrameData->data, m_frameDataBufLen, size);
	m_identifier = pFrameData->identifier.c_str();
	paintPic(m_pFrameDataBuf);
}

void QWidgetVideoRender::Clear()
{
	m_frameWidth = width();
	m_frameHeight = height();
	paintPic(m_pBkgDataBuf);
	m_lastClock = 0;
	m_identifier = "";
}

void QWidgetVideoRender::pauseRender()
{
	m_bPause = true;
	m_pParentWidget->setEnabled(false);
}

void QWidgetVideoRender::recoverRender()
{
	m_bPause = false;
	m_pParentWidget->setEnabled(true);
}

void QWidgetVideoRender::enterFullScreen()
{
	if (!isFullScreen())
	{
		m_pParentWidget = parentWidget();
		m_Rect = geometry();
		if (mainWindow)
		{
			this->setParent(mainWindow);
			this->setWindowFlags(Qt::Window);
		}
		else
		{
			this->setParent(NULL);
		}
		this->showFullScreen();
	}
}

void QWidgetVideoRender::exitFullScreen()
{
	if ( isFullScreen() )
	{
		this->setParent(m_pParentWidget);
		this->setGeometry(m_Rect);
		this->setWindowFlags(Qt::SubWindow);
		this->showNormal();
	}
}

void QWidgetVideoRender::setRenderIndex(int index)
{
	m_renderIndex = index;	
}

void QWidgetVideoRender::setMenuInfo(MenuInfo & menuInfo)
{
	m_bMainDis = menuInfo.mainDis;
	m_bCamera = menuInfo.camera;
	m_bMic = menuInfo.mic;
	m_bFullScreen = menuInfo.fullScreen;
	m_bTeacher = menuInfo.teacher;
}

void QWidgetVideoRender::updateMenuInfo()
{
	QList<QAction *> actionList = render_menu_->actions();
	for (int i=0;i<actionList.size();i++)
	{
		actionList[i]->setVisible(true);
	}

	if (!m_bMainDis)
	{
		if (m_bFullScreen) 
			render_menu_->actions()[0]->setVisible(false);  //设为主画面
		else
			render_menu_->actions()[1]->setVisible(false);  //退出主画面
	}
	else
	{
		render_menu_->actions()[0]->setVisible(false);
		render_menu_->actions()[1]->setVisible(false);
	}

	//if (m_bTeacher)
	//{
	//	if (!m_bCamera) //摄像头没打开
	//		render_menu_->actions()[2]->setVisible(false);   //关闭摄像头
	//	else
	//		render_menu_->actions()[3]->setVisible(false);   //打开摄像头

	//	if (!m_bMic)
	//		render_menu_->actions()[4]->setVisible(false);
	//	else
	//		render_menu_->actions()[5]->setVisible(false);
	//}
	//else
	//{
	render_menu_->actions()[2]->setVisible(false);
	render_menu_->actions()[3]->setVisible(false);
	render_menu_->actions()[4]->setVisible(false);
	render_menu_->actions()[5]->setVisible(false);
	if (m_bTeacher && (m_bCamera || m_bMic) )
	{
		render_menu_->actions()[6]->setVisible(true);
	}
	else
		render_menu_->actions()[6]->setVisible(false);
	//}
}

void QWidgetVideoRender::setFullScreen(bool fullScreen)
{
	m_bFullScreen = fullScreen;
}

void QWidgetVideoRender::beginRender()
{
	setUpdatesEnabled(false);
	
}

void QWidgetVideoRender::stopRender()
{
	setUpdatesEnabled(true);
}

void QWidgetVideoRender::mouseDoubleClickEvent( QMouseEvent * event )
{
	if (m_bPause)
	{
		return;
	}

	if (isFullScreen())
	{
		emit exitFullScreen(this);
	}
	else
	{
		emit doubleClicked(m_renderIndex);
		//emit applyFullScreen(this);
	}
}

void QWidgetVideoRender::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton )
	{		
		if (m_renderIndex == 0)
		{
			return;
		}
		updateMenuInfo();
		render_menu_->exec( QCursor::pos() );
	}
}

void QWidgetVideoRender::keyPressEvent(QKeyEvent * event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		if (isFullScreen())
		{
			exitFullScreen();
		}
		else
		{
			emit escPressed(m_renderIndex);
		}
		break;
	default:
		QWidget::keyPressEvent(event);
	}
}

void QWidgetVideoRender::paintEvent(QPaintEvent * event)
{

}

void QWidgetVideoRender::showEvent(QShowEvent *event)
{
	//m_pRootView->setBackgroundColor(m_bgColor);//灰色
}

void QWidgetVideoRender::paintPic( uint8* pData )
{
	int winWidth = width();
	int winHeight = height();

	int srcWidth = m_frameWidth;
	int srcHeight = m_frameHeight;

	int dstWidth = 0;
	int dstHeight = 0;

	float fWinRate = winWidth/(float)winHeight;
	float fSrcRate = srcWidth/(float)srcHeight;

	if (fWinRate>fSrcRate)//高不变，宽填充黑边
	{
		dstWidth = (int)(srcHeight * winWidth / (float)winHeight);
		dstHeight = srcHeight;
	}
	else//宽不变,高填充黑边
	{
		dstWidth = srcWidth;
		dstHeight = (int)(srcWidth * winHeight / (float)winWidth);
	}
	dstWidth = (dstWidth + 3) / 4 * 4; //保证宽度是4的倍数。

	if(m_nRenderDataBufLen != dstWidth * dstHeight * 3)
	{
		if(m_pRenderDataBuf != NULL)
		{
			SafeDeleteArr(m_pRenderDataBuf);
		}

		m_nRenderDataBufLen = dstWidth * dstHeight * 3; //RGB24
		m_pRenderDataBuf = new uint8[m_nRenderDataBufLen];
	}
	memset(m_pRenderDataBuf, 0 , m_nRenderDataBufLen);//清除上一次渲染

	SIZE srcSize = {srcWidth, srcHeight};
	SIZE dstSize = {dstWidth, dstHeight};

	_ResizeWithMendBlack(m_pRenderDataBuf, pData, m_nRenderDataBufLen, m_frameDataBufLen, dstSize, srcSize, 3);

	BITMAPINFO Bitmap;
	memset(&Bitmap,0,sizeof(BITMAPINFO));
	Bitmap.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);		
	Bitmap.bmiHeader.biWidth = dstWidth;
	Bitmap.bmiHeader.biHeight = -dstHeight;

	Bitmap.bmiHeader.biBitCount = 3 * 8;//COLOR_FORMAT_RGB24	
	Bitmap.bmiHeader.biPlanes = 1;	
	Bitmap.bmiHeader.biCompression = BI_RGB;//COLOR_FORMAT_RGB24	
	Bitmap.bmiHeader.biSizeImage = 0;	
	Bitmap.bmiHeader.biClrUsed = 0;
	Bitmap.bmiHeader.biXPelsPerMeter = 0;
	Bitmap.bmiHeader.biYPelsPerMeter = 0;	
	Bitmap.bmiHeader.biClrImportant = 0;

	hWnd = (HWND)this->winId();
	if (!hDC)
	{
		hDC = GetDC(hWnd);
	}
	
	if (!hMemDC)
	{
		hMemDC = CreateCompatibleDC(hDC);
	}
	if (!hMemBitmap)
	{
		hMemBitmap = CreateCompatibleBitmap(hDC, winWidth, winHeight);
	}
	else if (m_winWidth != winWidth || m_winHeight != winHeight)
	{
		m_winWidth = winWidth;
		m_winHeight = winHeight;
		DeleteObject(hMemBitmap);
		hMemBitmap = CreateCompatibleBitmap(hDC, winWidth, winHeight);
	}
	
	SelectObject(hMemDC, hMemBitmap);

	SetStretchBltMode(hMemDC, HALFTONE);
	SetBrushOrgEx(hMemDC, 0, 0, NULL);
	StretchDIBits(hMemDC, 0, 0, winWidth, winHeight, 0, 0, dstWidth, dstHeight,		
		m_pRenderDataBuf, &Bitmap, DIB_RGB_COLORS, SRCCOPY);

	BitBlt(hDC, 0, 0, winWidth, winHeight, hMemDC, 0, 0, SRCCOPY);

	if(m_nFpsCounter > 0)
	{
		--m_nFpsCounter;
	}
	else
	{
		clock_t curClock = clock();
		time_t nSpace = curClock - m_lastClock;
		if (nSpace!=0)
		{
			m_nFps = 1000*FPS_Count/nSpace;
		}
		m_lastClock = curClock;
		m_nFpsCounter = FPS_Count;
	}

}

std::string QWidgetVideoRender::getId()
{
	return m_identifier;
}

void QWidgetVideoRender::on_swap_view()
{
	std::string main_id = QWidgetVideoRender::main_view_->getId();
	QWidgetVideoRender::main_view_->setId(m_identifier);
	Clear();
}



