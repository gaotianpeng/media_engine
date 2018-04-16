#include "QWindowWhiteBoard.h"
#include <QWindow>
#include <QFileDialog>
#include <QTimer>
#include <QDateTime>
#include <ctime>
#include "QDialogProgress.h"
#include "BoardService.h"

QWindowWhiteBoard::QWindowWhiteBoard(bool enableDraw, QWidget *parent)
	: QMainWindow(parent)
	, _enableDraw(enableDraw)
{
	emit QDialogProgress::instance().showProgress(QStringLiteral("正在同步数据"));

	setWindowFlags(windowFlags() & ~Qt::Window);
	ui.setupUi(this);
	//this->setParent(parent);
	//ui.menuBar->hide();
	//ui.mainToolBar->hide();
	//ui.statusBar->hide();

	for(auto subWin : ui.centralWidget->subWindowList())
	{
		if(subWin->windowTitle() == "ToolsWindow")
		{
			_toolsWindow = subWin;
			_toolsWindow->setWindowFlags(Qt::FramelessWindowHint| _toolsWindow->windowFlags());
			//_toolsWindow->setAttribute(Qt::WA_TranslucentBackground);
			_toolsWindow->setAttribute(Qt::WA_Mapped);
			_toolsWindow->hide();
		}
		else if(subWin->windowTitle() == "PagesWindow")
		{
			_pagesWindow = subWin;
			_pagesWindow->setWindowFlags(Qt::FramelessWindowHint| _pagesWindow->windowFlags());
			//_pagesWindow->setAttribute(Qt::WA_TranslucentBackground );
			_pagesWindow->setAttribute(Qt::WA_Mapped);
			_pagesWindow->hide();
		}
		else if(subWin->windowTitle() == "ColorWindow")
		{
			_colorWindow = subWin;
			_colorWindow->setWindowFlags(Qt::FramelessWindowHint);
			_colorWindow->setAttribute(Qt::WA_TranslucentBackground);
			_colorWindow->hide();
		}
		else if(subWin->windowTitle() == "SizeWindow")
		{
			_sizeWindow = subWin;
			_sizeWindow->setWindowFlags(Qt::FramelessWindowHint);
			_sizeWindow->setAttribute(Qt::WA_TranslucentBackground);
			_sizeWindow->hide();
		}
		else if(subWin->windowTitle() == "ShapeWindow")
		{
			_shapeWindow = subWin;
			_shapeWindow->setWindowFlags(Qt::FramelessWindowHint);
			_shapeWindow->setAttribute(Qt::WA_TranslucentBackground);
			_shapeWindow->hide();
		}
		else if(subWin->windowTitle() == "DialogWindow")
		{
			_dialogWindow = subWin;
			_dialogWindow->setWindowFlags(Qt::FramelessWindowHint);
			_dialogWindow->setAttribute(Qt::WA_TranslucentBackground);
			_dialogWindow->hide();
		}
	}

	_timer = new QTimer(this);

	connectSignals();

	BoardService::instance().setCallback(this);
	BoardService::instance().syncEventData();

	_boardWindow = ui.centralWidget->addSubWindow(QWidget::createWindowContainer(QWindow::fromWinId((WId)BoardService::instance().getRenderWindow())));
	_boardWindow->setWindowTitle(QStringLiteral("BoardWindow"));
	_boardWindow->setWindowFlags(Qt::FramelessWindowHint);
	_boardWindow->show();

	ui.centralWidget->setActiveSubWindow(_toolsWindow);
	ui.centralWidget->setActiveSubWindow(_pagesWindow);
	ui.centralWidget->setActiveSubWindow(_colorWindow);
	ui.centralWidget->setActiveSubWindow(_sizeWindow);
	ui.centralWidget->setActiveSubWindow(_shapeWindow);
	ui.centralWidget->setActiveSubWindow(_dialogWindow);

	updateTool();

	on_updatePageWindow();

	_timer->start(100);
}

QWindowWhiteBoard::~QWindowWhiteBoard()
{
	_timer->stop();
}

void QWindowWhiteBoard::setEnableDraw(bool enableDraw)
{
	_enableDraw = enableDraw;
	if(_enableDraw)
	{
		showToolsWindow();
		showPagesWindow();
	}
	else
	{
		hideToolsWindow();
		hidePagesWindow();
		BoardService::instance().useTool(BoardTool::None);
	}
}

void QWindowWhiteBoard::resizeEvent(QResizeEvent * event)
{
	_boardWindow->resize(ui.centralWidget->width(), ui.centralWidget->height() - 42);
	_boardWindow->move(0, 0);

	_toolsWindow->resize(36, 288);
	_toolsWindow->move(0, (ui.centralWidget->height() - _toolsWindow->height()) / 2);
	_toolsWindow->setWindowState(Qt::WindowActive);

	_pagesWindow->resize(ui.centralWidget->width(), 42);
	_pagesWindow->move(0, ui.centralWidget->height() - _pagesWindow->height());
}

void QWindowWhiteBoard::showEvent(QShowEvent* event)
{
	setEnableDraw(_enableDraw);
}

bool QWindowWhiteBoard::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == _toolsWindow)
	{
		if (event->type() == QEvent::Type::Enter)
		{
			showToolsWindow();
		}
	}
	else if(watched == _pagesWindow)
	{
		if (event->type() == QEvent::Type::Enter)
		{
			showPagesWindow();
		}
	}
	return QMainWindow::eventFilter(watched, event);
}

void QWindowWhiteBoard::changeEvent(QEvent* e)
{

}

void QWindowWhiteBoard::on_chkColorPick_toggled(bool checked)
{
	if(checked)
	{
		showColorWindow();
	}
	else
	{
		hideColorWindow();
	}
}

void QWindowWhiteBoard::on_btnClear_clicked()
{
	BoardService::instance().clear();
}

void QWindowWhiteBoard::on_btnUpload_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		QStringLiteral("选择背景文件"),
		"",
		QStringLiteral("背景文件 (*.ppt *.pptx *.pdf *.doc *.docx *.png *.jpg *.jpeg *.bmp )"),
		nullptr);
	if (fileName.isEmpty()) return;
	emit QDialogProgress::instance().showProgress(QStringLiteral("正在上传"), 0);	

	QString suffixName;
	int index = fileName.lastIndexOf(".");
	if (index > 0)
	{
		suffixName = fileName.mid(index);
	}

	QString changeName = QStringLiteral("/\\") +  QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch());
	changeName += suffixName;
		
	BoardService::instance().uploadFile(TXCHttpRequest::a2w(changeName.toUtf8().data()), fileName.toStdWString());
}

void QWindowWhiteBoard::on_tool_toggled(bool checked)
{
	if(checked)
	{
		updateTool(true);
	}
}

void QWindowWhiteBoard::on_color_toggled(bool checked)
{
	if(checked)
	{
		updateColor();
		hideColorWindow();
	}
}

void QWindowWhiteBoard::on_size_toggled(bool checked)
{
	if(checked)
	{
		updateSize();
		hideSizeWindow();
	}
}

void QWindowWhiteBoard::on_shape_toggled(bool checked)
{
	if(checked)
	{
		updateShape();
	}
}

void QWindowWhiteBoard::on_btnUndo_clicked()
{
	BoardService::instance().undo();
}

void QWindowWhiteBoard::on_btnRedo_clicked()
{
	BoardService::instance().redo();
}

void QWindowWhiteBoard::on_btnCopy_clicked()
{
	BoardService::instance().copy();
}

void QWindowWhiteBoard::on_btnRemove_clicked()
{
	BoardService::instance().remove();
}

void QWindowWhiteBoard::on_btnLastPage_clicked()
{
	BoardService::instance().gotoLastPage();
	on_updatePageWindow();
}

void QWindowWhiteBoard::on_btnPageNumber_clicked()
{
}

void QWindowWhiteBoard::on_btnNextPage_clicked()
{
	BoardService::instance().gotoNextPage();
	on_updatePageWindow();
}

void QWindowWhiteBoard::on_btnAddPage_clicked()
{
	BoardService::instance().insertPage();
	on_updatePageWindow();
}

void QWindowWhiteBoard::on_btnDeletePage_clicked()
{
	showDialogWindow();
}

void QWindowWhiteBoard::on_sliderShapeThin_valueChanged(int value)
{
	ui.labelThinValue->setText(QString::number(value));
	updateShape();
}

void QWindowWhiteBoard::on_updateWindowStatus()
{
	const clock_t curClock = clock();

	if(_toolsWindow->underMouse())
	{
		_toolsLostClock = curClock;
	}
	if(_pagesWindow->underMouse())
	{
		_pagesLostClock = curClock;
	}
	if(_colorWindow->underMouse())
	{
		_colorLostClock = curClock;
	}
	if(_sizeWindow->underMouse())
	{
		_sizeLostClock = curClock;
	}
	if(_shapeWindow->underMouse())
	{
		_shapeLostClock = curClock;
	}

	if(_toolsLostClock != 0 && curClock - _toolsLostClock > 1000)
	{
		//hideToolsWindow();
		_toolsWindow->setWindowState(Qt::WindowActive);
		_toolsLostClock = 0;
	}
	if(_pagesLostClock != 0 && curClock - _pagesLostClock > 1000)
	{
		//hidePagesWindow();
		_pagesLostClock = 0;
	}
	if(_colorLostClock != 0 && curClock - _colorLostClock > 1000)
	{
		hideColorWindow();
		_colorLostClock = 0;
	}
	if(_sizeLostClock != 0 && curClock - _sizeLostClock > 1000)
	{
		hideSizeWindow();
		_sizeLostClock = 0;
	}
	if(_shapeLostClock != 0 && curClock - _shapeLostClock > 1000)
	{
		hideShapeWindow();
		_shapeLostClock = 0;
	}
}

void QWindowWhiteBoard::on_updatePageWindow()
{
	ui.btnPageNumber->setText(
		QString::number(BoardService::instance().getPageIndex() + 1) +
		" / " +
		QString::number(BoardService::instance().getPageCount()));
	ui.btnLastPage->setEnabled(BoardService::instance().getPageIndex() > 0);
	ui.btnNextPage->setEnabled(BoardService::instance().getPageIndex() < BoardService::instance().getPageCount() - 1);
	ui.btnDeletePage->setEnabled(BoardService::instance().getPageCount() > 1);
	//QT最小化后再恢复界面刷新会失效
	if (_enableDraw)
	{
		_pagesWindow->hide();
		_pagesWindow->show();
	}
}

void QWindowWhiteBoard::on_btnDialogClose_clicked()
{
	hideDialogWindow();
}

void QWindowWhiteBoard::on_btnDialogOK_clicked()
{
	BoardService::instance().deletePage();
	on_updatePageWindow();
	hideDialogWindow();
}

void QWindowWhiteBoard::on_btnDialogCancel_clicked()
{
	hideDialogWindow();
}

void QWindowWhiteBoard::connectSignals()
{
	qRegisterMetaType<txfunction>("txfunction");
	connect(this, SIGNAL(dispatch(txfunction)), this, SLOT(handle(txfunction)), Qt::QueuedConnection);
	connect(this, &QWindowWhiteBoard::updatePageWindow, this, &QWindowWhiteBoard::on_updatePageWindow);
	connect(this, &QWindowWhiteBoard::setUndoEnabled, ui.btnUndo, &QWidget::setEnabled);
	connect(this, &QWindowWhiteBoard::setRedoEnabled, ui.btnRedo, &QWidget::setEnabled);
	connect(this, &QWindowWhiteBoard::setCopyEnabled, ui.btnCopy, &QWidget::setEnabled);
	connect(this, &QWindowWhiteBoard::setRemoveEnabled, ui.btnRemove, &QWidget::setEnabled);

	connect(ui.rdoPen, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoPen, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoLine, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoLine, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoEraser, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoEraser, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoSelect, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoSelect, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoShape, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_tool_toggled);
	connect(ui.rdoShape, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_tool_toggled);

	connect(ui.rdoColorBlue, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorBlue, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorGreen, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorGreen, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorOrange, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorOrange, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorRed, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorRed, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorBlack, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorBlack, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorGrey, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_color_toggled);
	connect(ui.rdoColorGrey, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_color_toggled);

	connect(ui.rdoSizeLevel1, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_size_toggled);
	connect(ui.rdoSizeLevel1, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_size_toggled);
	connect(ui.rdoSizeLevel2, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_size_toggled);
	connect(ui.rdoSizeLevel2, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_size_toggled);
	connect(ui.rdoSizeLevel3, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_size_toggled);
	connect(ui.rdoSizeLevel3, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_size_toggled);

	connect(ui.rdoShapeEmptyRect, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeEmptyRect, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeFullRect, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeFullRect, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeEmptyEllipse, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeEmptyEllipse, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeFullEllipse, &QRadioButton::clicked, this, &QWindowWhiteBoard::on_shape_toggled);
	connect(ui.rdoShapeFullEllipse, &QRadioButton::toggled, this, &QWindowWhiteBoard::on_shape_toggled);

	connect(_timer, &QTimer::timeout, this, &QWindowWhiteBoard::on_updateWindowStatus);
}

void QWindowWhiteBoard::handle(txfunction func)
{
	func();
}

void QWindowWhiteBoard::updateTool(bool showToolWindow)
{
	if(ui.rdoPen->isChecked())
	{
		updateColor();
		updateSize();
		BoardService::instance().useTool(BoardTool::Pen);
		if(showToolWindow)
		{
			showSizeWindow();
		}
	}
	else if(ui.rdoLine->isChecked())
	{
		updateColor();
		updateSize();
		BoardService::instance().useTool(BoardTool::Line);
		if(showToolWindow)
		{
			showSizeWindow();
		}
	}
	else if (ui.rdoEraser->isChecked())
	{
		updateColor();
		updateSize();
		BoardService::instance().useTool(BoardTool::Eraser);
		if (showToolWindow)
		{
			showSizeWindow();
		}
	}
	else if(ui.rdoSelect->isChecked())
	{
		BoardService::instance().useTool(BoardTool::Select);
	}
	else if(ui.rdoShape->isChecked())
	{
		updateColor();
		updateShape();
		if(showToolWindow)
		{
			showShapeWindow();
		}
	}
}

void QWindowWhiteBoard::updateColor()
{
	QString stylesheet = QStringLiteral("QCheckBox{\nmargin: 6px 4px 6px 4px;\n}\n"
"QCheckBox:unchecked{\nborder-image: url(:/tools/whiteboard/tools/pick-normal.png);\n}"
"QCheckBox:checked{\nborder-radius: 2px;\nbackground - color:#33000000;\nborder-image: url(:/tools/whiteboard/tools/pick-press.png);\n}"
"QCheckBox::indicator{\nmargin:5px;\nwidth:18px\n;height: 14px;\nimage: none;\n");
	if(ui.rdoColorBlue->isChecked())
	{
		BoardService::instance().setColor(0x0075ffff);
		stylesheet += QStringLiteral("background-color: #0075ff;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
	else if(ui.rdoColorGreen->isChecked())
	{
		BoardService::instance().setColor(0x00cc00ff);
		stylesheet += QStringLiteral("background-color: #00cc00;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
	else if(ui.rdoColorOrange->isChecked())
	{
		BoardService::instance().setColor(0xff9903ff);
		stylesheet += QStringLiteral("background-color: #ff9903;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
	else if(ui.rdoColorRed->isChecked())
	{
		BoardService::instance().setColor(0xff0100ff);
		stylesheet += QStringLiteral("background-color: #ff0100;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
	else if(ui.rdoColorBlack->isChecked())
	{
		BoardService::instance().setColor(0x000000ff);
		stylesheet += QStringLiteral("background-color: #000000;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
	else if(ui.rdoColorGrey->isChecked())
	{
		BoardService::instance().setColor(0xccccccff);
		stylesheet += QStringLiteral("background-color: #cccccc;\n}");
		ui.chkColorPick->setStyleSheet(stylesheet);
	}
}

void QWindowWhiteBoard::updateSize()
{
	if(ui.rdoSizeLevel1->isChecked())
	{
		BoardService::instance().setWidth(50);
	}
	else if(ui.rdoSizeLevel2->isChecked())
	{
		BoardService::instance().setWidth(100);
	}
	else if(ui.rdoSizeLevel3->isChecked())
	{
		BoardService::instance().setWidth(150);
	}
}

void QWindowWhiteBoard::updateShape()
{
	if (ui.rdoShapeEmptyRect->isChecked())
	{
		BoardService::instance().useTool(BoardTool::Rectangle);
		BoardService::instance().setFill(false);
	}
	else if (ui.rdoShapeFullRect->isChecked())
	{
		BoardService::instance().useTool(BoardTool::Rectangle);
		BoardService::instance().setFill(true);
	}
	else if (ui.rdoShapeEmptyEllipse->isChecked())
	{
		BoardService::instance().useTool(BoardTool::Ellipse);
		BoardService::instance().setFill(false);
	}
	else if (ui.rdoShapeFullEllipse->isChecked())
	{
		BoardService::instance().useTool(BoardTool::Ellipse);
		BoardService::instance().setFill(true);
	}

	BoardService::instance().setWidth(ui.sliderShapeThin->value() * 50);
}

void QWindowWhiteBoard::showToolsWindow()
{
	_toolsWindow->resize(36, 288);
	_toolsWindow->move(0, (ui.centralWidget->height() - _toolsWindow->height()) / 2);
	_toolsWindow->setWindowState(Qt::WindowActive);
	_toolsWindow->show(); 
	
	_toolsLostClock = clock();
}

void QWindowWhiteBoard::hideToolsWindow()
{
	_toolsWindow->hide();
}

void QWindowWhiteBoard::showPagesWindow()
{
	_pagesWindow->resize(ui.centralWidget->width(), 42);
	_pagesWindow->move(0, ui.centralWidget->height() - _pagesWindow->height());
	_pagesWindow->show();

	_pagesLostClock = clock();
}

void QWindowWhiteBoard::hidePagesWindow()
{
	_pagesWindow->hide();
}

void QWindowWhiteBoard::showColorWindow()
{
	_colorWindow->move(36, _toolsWindow->y() + 0);
	_colorWindow->show();
	_colorLostClock = clock();
}

void QWindowWhiteBoard::hideColorWindow() const
{
	_colorWindow->hide();
	ui.chkColorPick->setChecked(false);
}

void QWindowWhiteBoard::showSizeWindow()
{
	if (ui.rdoPen->isChecked()){
		_sizeWindow->move(36, _toolsWindow->y() + 36);
	}
	else if (ui.rdoLine->isChecked()){
		_sizeWindow->move(36, _toolsWindow->y() + 36 * 2);
	}
	else if (ui.rdoEraser->isChecked()) {
		_sizeWindow->move(36, _toolsWindow->y() + 36 * 4);
	}
	_sizeWindow->show();
	_sizeLostClock = clock();
}

void QWindowWhiteBoard::hideSizeWindow() const
{
	_sizeWindow->hide();
}

void QWindowWhiteBoard::showShapeWindow()
{
	_shapeWindow->move(36, _toolsWindow->y() + 36 * 3);
	_shapeWindow->show();
	_shapeLostClock = clock();
}

void QWindowWhiteBoard::hideShapeWindow() const
{
	_shapeWindow->hide();
}

void QWindowWhiteBoard::showDialogWindow() const
{
	_dialogWindow->move(0, 0);
	_dialogWindow->resize(ui.centralWidget->size());
	_dialogWindow->show();
}

void QWindowWhiteBoard::hideDialogWindow() const
{
	_dialogWindow->hide();
}

void QWindowWhiteBoard::onUploadProgress(int percent)
{
	emit QDialogProgress::instance().showProgress(QStringLiteral("正在上传"), percent);
}

void QWindowWhiteBoard::onUploadResult(bool success)
{
	if(success)
	{
		emit QDialogProgress::instance().hideAfter(0);
		emit updatePageWindow();
	}
	else
	{
		//emit QDialogProgress::instance().showProgress(QStringLiteral("上传失败"), 100);
		emit QDialogProgress::instance().hideAfter(1000);
	}
}

void QWindowWhiteBoard::onStatusChanged(bool canUndo, bool canRedo, bool canCopy, bool canRemove)
{
	emit setUndoEnabled(canUndo);
	emit setRedoEnabled(canRedo);
	emit setCopyEnabled(canCopy);
	emit setRemoveEnabled(canRemove);
}

void QWindowWhiteBoard::onSyncEventResult(bool success)
{
	emit QDialogProgress::instance().hideAfter(0);
}
