#include "QWindowShareRect.h"
#include <QMouseEvent>
#include <wtypes.h>

extern QWidget * mainTabWindow;
QWindowShareRect::QWindowShareRect(QWidget *parent)
	: QMainWindow(parent)
	, m_bRectChange(false)
{
	setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_TranslucentBackground);

	m_pLabel = new QLabel();
	m_pLabel->setFixedWidth(70);
	m_pLabel->setFixedHeight(22);
	m_pLabel->setAlignment(Qt::AlignCenter);
	m_pLabel->setWindowFlags(Qt::FramelessWindowHint|windowFlags());
	m_pLabel->setStyleSheet("QLabel{\n	background-color: #3f000000;\n	color:#ffe7e7e7;\n    opacity: 0.0;\n}");
	m_pLabel->hide();

	m_pTool = nullptr;
	edgeMargin = 4;        //���ü���ԵΪ3
	resizeDir = nodir;   //��ʼ����ⷽ��Ϊ��
	setMouseTracking(true);
	ui.setupUi(this);
	connect(this, SIGNAL(notifyChangeShareRect()), mainTabWindow, SLOT(on_changeShareRect()));
}

QWindowShareRect::~QWindowShareRect()
{
	if (m_pLabel)
	{
		delete m_pLabel;
	}
}

void QWindowShareRect::init(int x, int y, int w, int h, QWindowShareToolBar *pTool)
{
	m_pTool = pTool;
	setGeometry(QRect(x, y, w, h));
	QString str;
	str.sprintf("%d*%d", w, h);
	m_pLabel->setText(str);
	m_pLabel->setGeometry(x, y - 25, m_pLabel->size().width(), m_pLabel->size().height());
	m_pLabel->show();

	m_pTool->setGeometry(x+w- m_pTool->size().width(), y+h+4, m_pTool->size().width(), m_pTool->size().height());
	m_pTool->show();
}

void QWindowShareRect::getShareRect(int& left, int& top, int& right, int& bottom)
{
	left = geometry().left();
	top = geometry().top();
	right = geometry().right();
	bottom = geometry().bottom();
}

void QWindowShareRect::mouseReleaseEvent(QMouseEvent *event)
{
	event->ignore();
	if (m_bRectChange)
	{
		m_bRectChange = false;
		emit notifyChangeShareRect();
	}
	if (resizeDir != nodir) {         //��ԭ�����ʽ
		testEdge();
	}
}


void QWindowShareRect::mousePressEvent(QMouseEvent *event)
{
	event->ignore();
	if (event->button() == Qt::LeftButton)
	{
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		testEdge();
	}
	else if (event->button() == Qt::RightButton)
	{
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		resizeDir = nodir;
	}
}

void QWindowShareRect::mouseMoveEvent(QMouseEvent *event)
{
	event->ignore();
	if (event->buttons() & Qt::LeftButton) {                 //�������ǰ��µ�
		if (resizeDir == nodir) {                             //�����겻�Ƿ��ڱ�Ե��ô˵���������϶�����
			move(event->globalPos() - dragPosition);
			QPoint pos = event->globalPos() - dragPosition;
			m_pLabel->setGeometry(pos.x(), pos.y() - 25, m_pLabel->size().width(), m_pLabel->size().height());
			m_pTool->setGeometry(pos.x()+this->size().width()-m_pTool->size().width(), pos.y()+this->size().height()+4, m_pTool->size().width(), m_pTool->size().height());
			m_bRectChange = true;
		}
		else {
			int ptop, pbottom, pleft, pright;                   //�����������ҵ�ֵ
			ptop = frameGeometry().top();
			pbottom = frameGeometry().bottom();
			pleft = frameGeometry().left();
			pright = frameGeometry().right();
			if (resizeDir & top) {                               //�����ĳߴ緽���а������������ҷ���
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

			if (resizeDir & left) {                        //������ҷ���
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
			setGeometry(QRect(QPoint(pleft, ptop), QPoint(pright, pbottom)));
			QString str;
			str.sprintf("%d*%d", (pright - pleft), (pbottom - ptop));
			m_pLabel->setText(str);
			m_pLabel->setGeometry(pleft, ptop - 25, m_pLabel->size().width(), m_pLabel->size().height());
			m_pTool->setGeometry(pright - m_pTool->size().width(), pbottom + 4, m_pTool->size().width(), m_pTool->size().height());
			m_bRectChange = true;
		}
	}
	else testEdge();   //�����϶����ڡ����ı䴰�ڴ�С�ߴ��ʱ��  �������Ե
}

void QWindowShareRect::testEdge()
{
	int diffLeft = abs(cursor().pos().x() - frameGeometry().left());      //���������봰�����������ж��پ���
	int diffRight = abs(cursor().pos().x() - frameGeometry().right());
	int diffTop = abs(cursor().pos().y() - frameGeometry().top());
	int diffBottom = abs(cursor().pos().y() - frameGeometry().bottom());

	QCursor tempCursor;                                    //��õ�ǰ�����ʽ��ע��:ֻ�ܻ�õ�ǰ�����ʽȻ�����������������ʽ
	tempCursor = cursor();                                 //��Ϊ��õĲ������ָ�룬���Բ���������:cursor().setXXXXX

	if (diffTop < edgeMargin) {                              //���� ��Ե���� ����ı�ߴ�ķ���
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

	setCursor(tempCursor);
}
