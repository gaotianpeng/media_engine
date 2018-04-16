#ifndef VideoRender_h_
#define VideoRender_h_

#include "iLive.h"
#include <QWidget>
#include <QMenu>
#include <ctime>

using namespace ilive;

struct MenuInfo
{
	bool mainDis;
	bool camera;
	bool mic;
	bool fullScreen;
	bool teacher;
};

class QWidgetVideoRender : public QWidget
{
	Q_OBJECT
public:
	QWidgetVideoRender(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
	~QWidgetVideoRender();
	void	DoRender(const LiveVideoFrame *pFrameData);
	void	Clear();

	void	pauseRender();
	void	recoverRender();

	void	enterFullScreen();//ȫ��
	void	exitFullScreen();//�˳�ȫ��

	void    setRenderIndex(int index);
	void    setMenuInfo(MenuInfo & menuInfo);
	void    updateMenuInfo();
	void    setFullScreen(bool fullScreen);
	void    beginRender();
	void    stopRender();

	std::string getId();
	void setId(const std::string &id)
	{
		m_identifier = id;
	}
	static QWidgetVideoRender *main_view_;

signals:
	void	applyFullScreen(QWidgetVideoRender* pRender);
	void	exitFullScreen(QWidgetVideoRender* pRender);
	void    doubleClicked(int index);
	void    escPressed(int index);

	void    actSpeaking(int index, bool open);
	void    actCamera(int index, bool open);
	void    actMic(int index, bool open);
	void    actEnd(int index);
protected:
	void	mouseDoubleClickEvent( QMouseEvent * event ) override;
	void    mousePressEvent(QMouseEvent * event) override;
	void	keyPressEvent(QKeyEvent *event) override;
	void	paintEvent(QPaintEvent * event) override;
	void	showEvent(QShowEvent *event) override;
private:
	void	paintPic(uint8* pData);
	
private:

	iLiveRootView*	m_pRootView;
	std::string	m_identifier;
	E_ColorFormat m_colorFormat;
	uint32		m_frameDataBufLen;	//֡���ݳ���
	uint8*		m_pFrameDataBuf;	//֡���ݻ���
	uint8*		m_pBkgDataBuf;		//��������

	uint32		m_frameWidth;		//֡���
	uint32		m_frameHeight;		//֡�߶�

	uint32		m_nRenderDataBufLen;//��Ⱦ�����С
	uint8*		m_pRenderDataBuf;	//��Ⱦ����

	QWidget*	m_pParentWidget;
	QRect		m_Rect;
	bool		m_bPause;//�Ƿ���ͣ��Ⱦ	

	clock_t		m_lastClock;
	int			m_nFpsCounter;
	int			m_nFps;
	QMenu *  render_menu_;
	QAction* pActEnterMain;
	QAction* pActStopSpeaking;
	QAction* pActStopCamera;
	QAction* pActStopMic;

	QAction* pActExitMain;
	QAction* pActOpenCamera;
	QAction* pActOpenMic;
	QAction* pActEndInteract;

	int			m_renderIndex;

	bool 		m_bMainDis;
	bool		m_bCamera;
	bool		m_bMic;
	bool		m_bFullScreen;
	bool        m_bTeacher;

	int			m_winWidth;
	int			m_winHeight;

	HWND hWnd;
	HDC hDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
private slots:
	void on_swap_view();
};

#endif //VideoRender_h_
