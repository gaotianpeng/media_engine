#include "AccountAuth.h"
#include "txCloudLog.h"
#include "QDialogMessage.h"
#include "QWindowCourse.h"
#include "Context.h"
#include "QDialogProgress.h"

extern QWidget * mainWindow;
AccountAuth::AccountAuth(QObject *parent)
	: QObject(parent)
	, m_service(std::make_shared<AccountService>(1400042982, 17802))
	, m_settings("Tencent", "TXCloudEdu")
	, m_frontPage()
    , m_enterRoom(m_service)
{
    // 切换主线程
    qRegisterMetaType<txfunction>("txfunction");
    connect(this, SIGNAL(dispatch(txfunction)), this, SLOT(handle(txfunction)), Qt::QueuedConnection);
    m_service->setCallback(this);
}

AccountAuth::~AccountAuth()
{
	//m_service->exitService();
	m_service->setCallback(nullptr);
}

int AccountAuth::exec(int reEnter)
{
  //  if (m_settings.contains("login/id") && m_settings.contains("login/pwd"))
  //  {
  //      QString id = m_settings.value("login/id").toString();
  //      QString pwd = m_settings.value("login/pwd").toString();

  //      Context::instance().setUserId(id.toStdString());
  //      Context::instance().setUserPwd(pwd.toStdString());

		//emit QDialogProgress::instance().showProgress(QStringLiteral("正在登录账号"));
  //      m_service->loginAccount(id.toStdString(), pwd.toStdString());
  //  }
  //  else
	if (!reEnter)
    {
        QString id;
        QString pwd;
		wchar_t localid[256] = { 0 };
		wchar_t localpass[256] = { 0 };
		GetPrivateProfileString(L"TXCloudEdu", L"account", L"", localid, 256, L"txcloudprofile.ini");
		GetPrivateProfileString(L"TXCloudEdu", L"password", L"", localpass, 256, L"txcloudprofile.ini");
		//id.fromStdWString(localid);
		//pwd.fromStdWString(localpass);
		if (id.isEmpty())
		{
			::srand(::time(NULL));
			id.sprintf("win%d_%d", ::time(NULL), ::rand() % 10000);
			pwd.sprintf("%d", ::rand() % 10000);

			Context::instance().setUserId(id.toStdString());
			Context::instance().setUserPwd(pwd.toStdString());
			WritePrivateProfileString(L"TXCloudEdu", L"account", id.toStdWString().c_str(), L"txcloudprofile.ini");
			WritePrivateProfileString(L"TXCloudEdu", L"password", pwd.toStdWString().c_str(), L"txcloudprofile.ini");

			emit QDialogProgress::instance().showProgress(QStringLiteral("正在注册账号"));
			m_service->registerAccount(id.toStdString(), pwd.toStdString());
		}
		else
		{
			Context::instance().setUserId(id.toStdString());
			Context::instance().setUserPwd(pwd.toStdString());
			m_settings.setValue("login/id", Context::instance().userId().c_str());
			m_settings.setValue("login/pwd", Context::instance().userPwd().c_str());

			emit QDialogProgress::instance().showProgress(QStringLiteral("正在登录账号"));
			m_service->loginAccount(Context::instance().userId(), Context::instance().userPwd());
		}
    }

	do 
	{
		do
		{
			if (Context::instance().isLogin())
			{
				m_service->queryRoomList();
			}			
			m_frontPage.exec();
		} while (RetryRoomType == m_frontPage.enterType());

		if (NoType != m_frontPage.enterType())
		{
			m_enterRoom.init(m_frontPage.enterType(), m_frontPage.roomId(), m_frontPage.roomName());
			m_enterRoom.exec();
		}
		else
		{
			quit();
		}
	} while (!isEnterRoom());

    return 0;
}

void AccountAuth::quit()
{
	m_service->exitService();
    exit(0);
}

void AccountAuth::exitRoom()
{
	if (authInfo().m_userType == STUDENT_USER_TYPE)
	{
		m_service->quitRoom();
	}
	else
	{
		m_service->destoryRoom();
	}
}

bool AccountAuth::isEnterRoom() const
{
    return Context::instance().isEnterRoom();
}

AuthInfo AccountAuth::authInfo() const
{
    AuthInfo info;
    info.m_userType = (CreateRoomType == m_frontPage.enterType() ? TEACHER_USER_TYPE : STUDENT_USER_TYPE);
    info.m_sdkAppId = Context::instance().sdkAppID();
    info.m_accountType = Context::instance().accountType();
    info.m_nickName = Context::instance().nickName();
    info.m_userId = Context::instance().userId();
    info.m_token = Context::instance().token();
    info.m_userSig = Context::instance().userSig();
    info.m_roomID = Context::instance().roomID();
    info.m_roomSig = Context::instance().roomSig();
    info.m_roomName = Context::instance().roomName();
    info.m_roomOwner = Context::instance().roomOwner();
    info.m_chatGroudID = Context::instance().chatGroudID();
    info.m_whiteBoardGroudID = Context::instance().whiteBoardGroudID();
    info.m_joinConfURL = Context::instance().joinConfURL();

    return info;
}

void AccountAuth::onQueryRoomList(int code, const char *desc, const std::vector<RoomItem>& roomList)
{
    emit dispatch([this, roomList] {
		emit QDialogProgress::instance().hideAfter(0);
        m_frontPage.setRoomList(roomList);
    });
}

void AccountAuth::onRegister(int code, const char *desc)
{
    if (0 != code)
    {
        // todo
		txCloudEduLog_e("txcloud", "onRegister Fail, code = %d", code);
		emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
			QDialogMessage::exec(QString::fromWCharArray(L"Register Fail!"), QDialogMessage::OK);
		});
    }
    else
    {
        m_settings.setValue("login/id", Context::instance().userId().c_str());
        m_settings.setValue("login/pwd", Context::instance().userPwd().c_str());

		emit QDialogProgress::instance().showProgress(QStringLiteral("正在登录账号"));
        m_service->loginAccount(Context::instance().userId(), Context::instance().userPwd());
    }
}

void AccountAuth::onLogin(int code, const char *desc)
{
    if (0 != code)
    {
        // todo
		txCloudEduLog_e("txcloud", "onLogin Fail, code = %d", code);
		emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
			QDialogMessage::exec(QString::fromWCharArray(L"Login Fail!"), QDialogMessage::OK);
		});
    }
    else
    {
        emit dispatch([this] {
            m_service->loginiLive();
        });

		emit QDialogProgress::instance().showProgress(QStringLiteral("正在获取课堂列表"));
        m_service->queryRoomList();
    }
}

void AccountAuth::onLogout(int code, const char *desc)
{
	QApplication::exit(0);
}

void AccountAuth::onCreateRoom(int code, const char *desc)
{
    if (0 != code)
    {
        // todo
		txCloudEduLog_e("txcloud", "onCreateRoom Fail, code = %d", code);
		emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
			QDialogMessage::exec(QString::fromWCharArray(L"CreateRoom Fail!"), QDialogMessage::OK);
		});
    }
    else
    {
		emit QDialogProgress::instance().showProgress(QStringLiteral("正在进入课堂"));
        m_service->joinRoom(Context::instance().roomID(), "", Context::instance().nickName());
		//m_enterRoom.done(0);
    }
}

void AccountAuth::onJoinRoom(int code, const char *desc)
{
    if (0 != code)
    {
        // todo
		txCloudEduLog_e("txcloud", "onJoinRoom Fail, code = %d", code);
		emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
			QDialogMessage::exec(QString::fromWCharArray(L"JoinRoom Fail!"), QDialogMessage::OK);
		});
    }
    else
    {
        emit dispatch([this] {
            m_service->joiniLiveRoom();
        });
    }
}

void AccountAuth::onReportJoinRoom(int code, const char *desc)
{
    if (0 != code)
    {
        // todo
		txCloudEduLog_e("txcloud", "onReportJoinRoom Fail, code = %d", code);
		emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
			QDialogMessage::exec(QString::fromWCharArray(L"ReportJoinRoom Fail!"), QDialogMessage::OK);
		});
    }
    else
    {
        emit dispatch([this] {
			emit QDialogProgress::instance().hideAfter(0);
	        m_enterRoom.done(0);
        });
    }
}

void AccountAuth::onQuitRoom(int code, const char *desc)
{
	//emit dispatch([this] {
	//	m_service->exitService();
	//});	
//	quit();
}

void AccountAuth::onQuitAVRoom()
{
	QApplication::exit();
}

void AccountAuth::onError()
{
	emit dispatch([this] {
		emit QDialogProgress::instance().hideAfter(0);
		m_enterRoom.done(0);
	});
}

void AccountAuth::handle(txfunction func)
{
    func();
}
