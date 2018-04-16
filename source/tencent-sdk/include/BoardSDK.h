// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� EDUSDK_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// EDUSDK_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#pragma once

#ifdef EDUSDK_EXPORTS
#define EDUSDK_API __declspec(dllexport)
#else
#define EDUSDK_API __declspec(dllimport)
#endif

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <cstdint>

class BoardSDKImpl;

/**
 * \brief �װ�ص��ӿڣ��û�������ʵ������ӿڲ�����BoardSDK��setCallback��������ͨ�����ø��װ�
 */
struct BoardCallback
{
	/**
	 * \brief �����¼����ݵ����ʹ�����ڰװ���ִ�в���ʱ��SDK��������Ԫ���ݰ�����JSON��ʽ������������ú���
	 * {
	 *		"boardId":"#DEFAULT", 	// �װ�id��Ĭ��Ϊ#DEFAULT
	 *		"operator":"user1",
	 *		"actions":
	 *			[
	 *				...	//һϵ�аװ����
	 *			],
	 *	}
	 * \param data		���������ݵ��ַ���������
	 * \param length	�ַ�������
	 */
	virtual void onActionsData(const char *data, uint32_t length) = 0;

	/**
	 * \brief �װ��¼����ݵ�����װ�ҳ���ͼ����ʽ�����仯ʱ�������ú��������ݸ�ʽ����Э��
	 * \param data		���������ݵ��ַ���������
	 * \param length	�ַ�������
	 */
	virtual void onBoardEventData(const char *data, uint32_t length) = 0;

	/**
	 * \brief ״̬�¼������װ�״̬�����仯ʱ����
	 * \param canUndo	�Ƿ�ɳ���
	 * \param canRedo	�Ƿ������
	 * \param canCopy	�Ƿ�ɸ���
	 * \param canRemove	�Ƿ��ɾ��
	 */
	virtual void onStatusChanged(bool canUndo, bool canRedo, bool canCopy, bool canRemove) = 0;

	/**
	 * \brief ��ȡʱ����¼���SDK��Ҫ��ȡͳһ��ʱ���ʱ�������¼�
	 * \return ʱ�������λ����
	 */
	virtual uint32_t onGetTime() = 0;
};

/**
 * \brief �װ幤�߶���
 */
enum class BoardTool
{
	None,			//��ʹ���κι��ߣ���ֹ���ƣ�
	Pen,			//Ǧ�ʹ���
	Eraser,			//��Ƥ������
	Laser,			//�����
	Line,			//ֱ�߹���
	Ellipse,		//��Բ����
	Rectangle,		//Բ�Ǿ��ι���
	Select,			//ѡ�򹤾�
};

// �����Ǵ� BoardSDK.dll ������
class EDUSDK_API BoardSDK {
public:
	/**
	 * \brief �װ�SDK���캯��
	 * \param userID				��ǰ�û�ID
	 * \param parentHWnd			�����ھ����Ϊ����װ�û�и�����
	 */
	BoardSDK(const char *userID, HWND parentHWnd = nullptr);
	~BoardSDK();

	/**
	 * \brief ��ȡ�װ���Ⱦ���ھ��
	 * \return ��Ⱦ���ھ��
	 */
	HWND getRenderWindow() const;

	/**
	 * \brief ���ûص��ӿ�
	 * \param callback				�ص��ӿ�
	 */
	void setCallback(BoardCallback *callback) const;

	/**
	 * \brief �װ�ҳ�����
	 * \param toPageID				Ҫ��ת��ҳ��ID
	 * \param deletePagesID			Ҫɾ����ҳ��ID����
	 * \param deletePagesCount		Ҫɾ����ҳ��ID����
	 */
	void pageOperate(const char *toPageID, const char **deletePagesID = nullptr, uint32_t deletePagesCount = 0) const;

	/**
	 * \brief ��ȡ�װ�ҳ��
	 * \param pagesID				�ַ������黺�������ڽ���ҳ��ID��Ϊ�ձ�ʾֻ��ѯ�װ�ҳ�������صİװ�ID˳��Ϊ������У�
	 * \param capacity				�ַ������鳤��
	 * \return �װ�ҳ��
	 */
	uint32_t getPages(char **pagesID = nullptr, uint32_t capacity = 0) const;

	/**
	 * \brief ָ��Ҫʹ�õİװ幤��
	 * \param tool					�װ幤��
	 */
	void useTool(BoardTool tool) const;

	/**
	 * \brief ָ��Ҫʹ�õĿ�ȣ�����Ǧ�ʡ�ֱ�ߡ���Բ�����ι��ߣ��ò���ָʾ���߿�������Ƥ��������ʹ��ߣ��ò���ָʾ��ֱ����
	 * \param width					���
	 */
	void setWidth(uint32_t width) const;

	/**
	 * \brief ָ��Ҫʹ�õ���ɫ
	 * \param rgba					��ɫֵ�����ֽ���Ӹߵ��ͷֱ�ΪRed��Green��Blue��Alpha����
	 */
	void setColor(uint32_t rgba) const;

	/**
	 * \brief ָ���Ƿ��������
	 * \param fill					�Ƿ��������
	 */
	void setFill(bool fill) const;

	/**
	 * \brief ָ��Ҫʹ�õİ뾶����ǰ������ָ��Բ�Ǿ��ε�Բ�ǰ뾶��
	 * \param radius				�뾶
	 */
	void setRadius(uint32_t radius) const;

	/**
	 * \brief ָ��Ҫʹ�õı���ͼƬ
	 * \param url					����ͼƬURL�������ļ���file://��ͷ��ʾ��Ϊ�ձ�ʾ��ձ�����
	 * \param pageID				�ƶ�Ҫ���ñ���ͼƬ��ҳ��ID��Ϊ�ձ�ʾ��ǰҳ�棩
	 */
	void useBackground(const wchar_t *url, const char *pageID = nullptr) const;

	/**
	 * \brief ����ѡ��ͼ�ν��п�������
	 */
	void copy() const;

	/**
	 * \brief ����ѡ��ͼ�ν���ɾ������
	 */
	void remove() const;

	/**
	 * \brief ����
	 */
	void undo() const;

	/**
	 * \brief ����
	 */
	void redo() const;

	/**
	 * \brief ��հװ�
	 */
	void clear() const;

	/**
	 * \brief ��װ���Ӳ���Ԫ���ݣ�ͨ��BoardCallback��sendData������ȡ�����ݣ�������JSON��ʽ�����
	 * {
	 *		"boardId":"#DEFAULT", 	// �װ�id��Ĭ��Ϊ#DEFAULT
	 *		"operator":"user1",
	 *		"actions":
	 *			[
	 *				...	//һϵ�аװ����
	 *			],
	 *	}
	 * \param data					���������ݵ��ַ���������
	 * \param length				�ַ�������
	 */
	void appendActionsData(const char *data, uint32_t length) const;

	/**
	 * \brief ��װ�����¼����ݣ����ݸ�ʽ����Э��
	 * \param data					���������ݵ��ַ���������
	 * \param length				�ַ�������
	 */
	void appendBoardEventData(const char *data, uint32_t length) const;

private:
	BoardSDKImpl *_impl;
};
