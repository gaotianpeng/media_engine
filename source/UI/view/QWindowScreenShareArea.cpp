#include "QWindowScreenShareArea.h"
#include <QMouseEvent>
#include <QPaintEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>


extern QWidget * mainTabWindow;
QWindowScreenShareArea::QWindowScreenShareArea(QWidget *parent)
	: QMainWindow(parent)
{
//	resize(QSize(m_nDesktopWidth, m_nDesktopHeight));
	//setWindowOpacity(0);
	ui.setupUi(this);
	//setAttribute(Qt::WA_Mapped);
	m_pLabel = new QLabel(this);
	m_pLabel->setFixedWidth(70);
	m_pLabel->setFixedHeight(22);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->setStyleSheet("QLabel{\n	background-color: #7f000000;;\n	color:#ffe7e7e7;\n    opacity: 0.0;\n}");
	m_pLabel->hide();
	edgeMargin = 2;

	connect(this, SIGNAL(notifyCancelShare()), mainTabWindow, SLOT(on_cancelShare()));
	connect(this, SIGNAL(notifyChoseShare(int,int,int,int)), mainTabWindow, SLOT(on_choseShare(int, int, int, int)));
}

QWindowScreenShareArea::~QWindowScreenShareArea()
{
	if (m_pLabel)
	{
		delete m_pLabel;
	}
}

void QWindowScreenShareArea::init()
{
	m_bAreaChose = false;
	m_bMouseDown = false;
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow);
	setAttribute(Qt::WA_TranslucentBackground);
	//setWindowModality(Qt::WindowModal);
	//setWindowFlags(Qt::Window | Qt::CustomizeWindowHint);
	//setStyleSheet("{\nbackground: #23000000;\n}");
	m_nDesktopWidth = QApplication::desktop()->width();
	m_nDesktopHeight = QApplication::desktop()->height();
	setGeometry(QRect(QPoint(0, 0), QPoint(m_nDesktopWidth, m_nDesktopHeight)));
	setWindowState(Qt::WindowActive);
}

void QWindowScreenShareArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (!m_bMouseDown && !m_bAreaChose)
		{
			m_PressedPos = event->pos();
			m_PressedGlobalPos = event->globalPos();
			m_pLabel->show();
		}
		m_bMouseDown = true;		
	}
	else if (event->button() == Qt::RightButton)
	{
		m_bMouseDown = false;
		emit notifyCancelShare();
	}
}

void QWindowScreenShareArea::mouseMoveEvent(QMouseEvent *event)
{
	if (m_bMouseDown && !m_bAreaChose)
	{
		m_CurrentPos = event->pos();
		m_CurrentGlobalPos = event->globalPos();
		QString str;
		str.sprintf("%d*%d", abs(m_CurrentPos.x() - m_PressedPos.x()), abs(m_CurrentPos.y() - m_PressedPos.y()));
		m_pLabel->setText(str);
		m_pLabel->move(min(m_PressedGlobalPos.x(), m_CurrentGlobalPos.x()), min(m_PressedGlobalPos.y(), m_CurrentGlobalPos.y())-25);
		update();
	}	
}

void QWindowScreenShareArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMouseDown = false;
		if (!m_bAreaChose)
		{
			m_CurrentPos = event->pos();
			m_CurrentGlobalPos = event->globalPos();
			if (m_PressedGlobalPos.x() != m_CurrentGlobalPos.x() && m_PressedGlobalPos.y() != m_CurrentGlobalPos.y())
			{			
				setGeometry(QRect(QPoint(min(m_PressedGlobalPos.x(), m_CurrentGlobalPos.x()), min(m_PressedGlobalPos.y(), m_CurrentGlobalPos.y())),
					QPoint(max(m_PressedGlobalPos.x(), m_CurrentGlobalPos.x()), max(m_PressedGlobalPos.y(), m_CurrentGlobalPos.y()))));
				//hide();
				//setStyleSheet("#QWindowScreenShareArea{border: 2px solid rgb(0, 110, 255);\nbackground: #00000000;\n}");
				//setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
				//show();
				//setWindowFlags(Qt::FramelessWindowHint);
				//setAttribute(Qt::WA_TranslucentBackground);
				//setWindowFlags(Qt::WindowStaysOnTopHint );
				m_bAreaChose = true;
				notifyChoseShare(min(m_PressedGlobalPos.x(), m_CurrentGlobalPos.x()), min(m_PressedGlobalPos.y(), m_CurrentGlobalPos.y()), abs(m_CurrentGlobalPos.x() - m_PressedGlobalPos.x()), abs(m_CurrentGlobalPos.y() - m_PressedGlobalPos.y()));
				hide();
			}
		}
	}
}

void QWindowScreenShareArea::paintEvent(QPaintEvent*)
{
	if (m_bMouseDown && !m_bAreaChose)
	{
		QPainter painter(this);
		QPen pen;
		pen.setColor(QColor(0, 110, 255));
		pen.setWidth(2);
		painter.setPen(pen);
		//QPoint tempPos = cursor().pos();
		painter.drawRect(min(m_PressedPos.x(), m_CurrentPos.x())-1, min(m_PressedPos.y(), m_CurrentPos.y())-1, abs(m_CurrentPos.x() - m_PressedPos.x())+4,abs(m_CurrentPos.y() - m_PressedPos.y())+2 );
		QPainter pt(this);
		pt.setCompositionMode(QPainter::CompositionMode_Clear);
		pt.fillRect(min(m_PressedPos.x(), m_CurrentPos.x()), min(m_PressedPos.y(), m_CurrentPos.y()), abs(m_CurrentPos.x() - m_PressedPos.x()), abs(m_CurrentPos.y() - m_PressedPos.y()), QBrush(Qt::NoBrush));
	}
}

/*
void QWindowScreenShareArea::mouseReleaseEvent(QMouseEvent *event)
{
	event->ignore();
	if (resizeDir != nodir) {         //还原鼠标样式
		testEdge();
	}
}

void QWindowScreenShareArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bMouseDown = true;
		this->setCursor(Qt::PointingHandCursor);
		m_PressedPos = event->globalPos() - this->pos();
		m_dragPosition = event->globalPos() - frameGeometry().topLeft();
	}
}

void QWindowScreenShareArea::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		if (resizeDir == nodir)
		{//移动窗口
			QPoint newPos = event->globalPos();
			move(newPos - m_PressedPos);
		}
		else
		{
			//ptop,pbottom,pleft,pright;                   //窗口上下左右的值
			int ptop = frameGeometry().top();
			int pbottom = frameGeometry().bottom();
			int pleft = frameGeometry().left();
			int pright = frameGeometry().right();
			if (resizeDir & top) {                               //检测更改尺寸方向中包含的上下左右分量
				if (height() == minimumHeight()) {
					ptop = min(event->globalY(), ptop);
				}
				else if (height() == maximumHeight()) {
					ptop = max(event->globalY(), ptop);
				}
				else {
					ptop = event->globalY();
				}
			}
			else if (resizeDir & bottom) {
				if (height() == minimumHeight()) {
					pbottom = max(event->globalY(), ptop);
				}
				else if (height() == maximumHeight()) {
					pbottom = min(event->globalY(), ptop);
				}
				else {
					pbottom = event->globalY();
				}
			}

			if (resizeDir & left) {                        //检测左右分量
				if (width() == minimumWidth()) {
					pleft = min(event->globalX(), pleft);
				}
				else if (width() == maximumWidth()) {
					pleft = max(event->globalX(), pleft);
				}
				else {
					pleft = event->globalX();
				}
			}
			else if (resizeDir & right) {
				if (width() == minimumWidth()) {
					pright = max(event->globalX(), pright);
				}
				else if (width() == maximumWidth()) {
					pright = min(event->globalX(), pright);
				}
				else {
					pright = event->globalX();
				}
			}
			setGeometry(QRect(QPoint(pleft,ptop),QPoint(pright, pbottom)));
		}
	}
	else
	{
		testEdge();
	}//当不拖动窗口、不改变窗口大小尺寸的时候  检测鼠标边缘
}

void QWindowScreenShareArea::testEdge()
{
	int diffLeft = abs(cursor().pos().x() - frameGeometry().left());      //计算鼠标距离窗口上下左右有多少距离
	int diffRight = abs(cursor().pos().x() - frameGeometry().right());
	int diffTop = abs(cursor().pos().y() - frameGeometry().top());
	int diffBottom = abs(cursor().pos().y() - frameGeometry().bottom());

	QCursor tempCursor;                                    //获得当前鼠标样式，注意:只能获得当前鼠标样式然后再重新设置鼠标样式
	tempCursor = cursor();                                 //因为获得的不是鼠标指针，所以不能这样用:cursor().setXXXXX

	if (diffTop < edgeMargin) {                              //根据 边缘距离 分类改变尺寸的方向
		if (diffLeft < edgeMargin) {
			resizeDir = topLeft;
			tempCursor.setShape(Qt::SizeFDiagCursor);
		}
		else if (diffRight < edgeMargin) {
			resizeDir = topRight;
			tempCursor.setShape(Qt::SizeBDiagCursor);
		}
		else {
			resizeDir = top;
			tempCursor.setShape(Qt::SizeVerCursor);
		}
	}
	else if (diffBottom < edgeMargin) {
		if (diffLeft < edgeMargin) {
			resizeDir = bottomLeft;
			tempCursor.setShape(Qt::SizeBDiagCursor);
		}
		else if (diffRight < edgeMargin) {
			resizeDir = bottomRight;
			tempCursor.setShape(Qt::SizeFDiagCursor);
		}
		else {
			resizeDir = bottom;
			tempCursor.setShape(Qt::SizeVerCursor);
		}
	}
	else if (diffLeft < edgeMargin) {
		resizeDir = left;
		tempCursor.setShape(Qt::SizeHorCursor);
	}
	else if (diffRight < edgeMargin) {
		resizeDir = right;
		tempCursor.setShape(Qt::SizeHorCursor);
	}
	else {
		resizeDir = nodir;
		tempCursor.setShape(Qt::ArrowCursor);
	}

	setCursor(tempCursor);                    //重新设置鼠标,主要是改样式
}
*/