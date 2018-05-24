
/**	@file       mian.cpp
 *	@note       Hikvision Digital Technology Co., Ltd. All Right Reserved.
 *	@brief		 
 *
 *	@author     lipengfei
 *	@date       2018/05/10
 *	@note       ��ʷ��¼��
 *	@note       V1.0.0  
 *	@warning	
 */


#include <stdlib.h>
#include <stdio.h>
#include "OSSocket.h"
#include "JsonParse.h"
#include "CmdParse.h"
#include"astar.h"
#include"blockallocator.h"
#include"haikang.h"

#define MAX_SOCKET_BUFFER       (1024 * 1024 * 4)       /// ���ͽ����������4M



/** @fn     int RecvJuderData(OS_SOCKET hSocket, char *pBuffer)
 *  @brief	��������
 *	@param  -I   - OS_SOCKET hSocket
 *	@param  -I   - char * pBuffer
 *	@return int
 */
int RecvJuderData(OS_SOCKET hSocket, char *pBuffer)
{
    int         nRecvLen = 0;
    int         nLen = 0;

    while (1)
    {
        // ����ͷ������
        nLen = OSRecv(hSocket, pBuffer + nRecvLen, MAX_SOCKET_BUFFER);
        if (nLen <= 0)
        {
            printf("recv error\n");
            return nLen;
        }

        nRecvLen += nLen;

        if (nRecvLen >= SOCKET_HEAD_LEN)
        {
            break;
        }
    }

    int         nJsonLen = 0;
    char        szLen[10] = { 0 };

    memcpy(szLen, pBuffer, SOCKET_HEAD_LEN);

    nJsonLen = atoi(szLen);

    while (nRecvLen < (SOCKET_HEAD_LEN + nJsonLen))
    {
        // ˵�����ݻ�û������
        nLen = OSRecv(hSocket, pBuffer + nRecvLen, MAX_SOCKET_BUFFER);
        if (nLen <= 0)
        {
            printf("recv error\n");
            return nLen;
        }

        nRecvLen += nLen;
    }

    return 0;
}

/** @fn     int SendJuderData(OS_SOCKET hSocket, char *pBuffer, int nLen)
 *  @brief	��������
 *	@param  -I   - OS_SOCKET hSocket
 *	@param  -I   - char * pBuffer
 *	@param  -I   - int nLen
 *	@return int
 */
int SendJuderData(OS_SOCKET hSocket, char *pBuffer, int nLen)
{
    int     nSendLen = 0;
    int     nLenTmp = 0;

    while (nSendLen < nLen)
    {
        nLenTmp = OSSend(hSocket, pBuffer + nSendLen, nLen - nSendLen);
        if (nLenTmp < 0)
        {
            return -1;
        }

        nSendLen += nLenTmp;
    }

    return 0;
}


int main(int argc, char *argv[])
{
#ifdef OS_WINDOWS
    // windows�£���Ҫ���г�ʼ������
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("WSAStartup failed\n");
        return false;
    }

#endif

    char        szIp[64] = { 0 };
    int         nPort = 0;
    char        szToken[128] = { 0 };
    int         nRet = 0;
    OS_SOCKET   hSocket;
    char        *pRecvBuffer = NULL;
    char        *pSendBuffer = NULL;
    int         nLen = 0;
   
        // ���ص���ȥ�����
    /*if (argc != 4)
    {
        printf("error arg num\n");
        return -1;
    }*/

    // ��������
    /*strcpy(szIp, argv[1]);
    nPort = atoi(argv[2]);
    strcpy(szToken, argv[3]);59.110.142.4,32651,45c9f300-399d-41e7-ab4c-ec35bf0691c6*/

     strcpy(szIp, "47.95.243.246");
	 nPort = 32166;
     strcpy(szToken, "7ee66ae0-657e-4a01-afb5-5913b613e143");

    printf("server ip %s, prot %d, token %s\n", szIp, nPort, szToken);

    // ��ʼ���ӷ�����
    nRet = OSCreateSocket(szIp, (unsigned int)nPort, &hSocket);
    if (nRet != 0)
    {
        printf("connect server error\n");
        return nRet;
    }

    // ������ܷ����ڴ�
    pRecvBuffer = (char*)malloc(MAX_SOCKET_BUFFER);
    if (pRecvBuffer == NULL)
    {
        return -1;
    }

    pSendBuffer = (char*)malloc(MAX_SOCKET_BUFFER);
    if (pSendBuffer == NULL)
    {
        free(pRecvBuffer);
        return -1;
    }

    memset(pRecvBuffer, 0, MAX_SOCKET_BUFFER);

    // ��������  ���ӳɹ���Judger�᷵��һ����Ϣ��
    nRet = RecvJuderData(hSocket, pRecvBuffer);
    if (nRet != 0)
    {
        return nRet;
    }

    // json ����
    // ��ȡͷ��
    CONNECT_NOTICE  stNotice;

    nRet = ParserConnect(pRecvBuffer + SOCKET_HEAD_LEN, &stNotice);
    if (nRet != 0)
    {
        return nRet;
    }

    // ���ɱ�����ݵ�json
    TOKEN_INFO  stToken;

    strcpy(stToken.szToken, szToken);  // ����ǵ��Խ׶Σ�����������Ե�token�����ҵĶ�ս�л�ȡ��
                                       // ʵ�ʱ�������Ҫ������Եģ�����demoд�ģ��з��������ô��롣
    strcpy(stToken.szAction, "sendtoken");

    memset(pSendBuffer, 0, MAX_SOCKET_BUFFER);
    nRet = CreateTokenInfo(&stToken, pSendBuffer, &nLen);
    if (nRet != 0)
    {
        return nRet;
    }
    // ѡ������з������������(Player -> Judger)
    nRet = SendJuderData(hSocket, pSendBuffer, nLen);
    if (nRet != 0)
    {
        return nRet;
    }

    //�����֤���(Judger -> Player)��
    memset(pRecvBuffer, 0, MAX_SOCKET_BUFFER);
    nRet = RecvJuderData(hSocket, pRecvBuffer);
    if (nRet != 0)
    {
        return nRet;
    }

    // ������֤�����json
    TOKEN_RESULT      stResult;
    nRet = ParserTokenResult(pRecvBuffer + SOCKET_HEAD_LEN, &stResult);
    if (nRet != 0)
    {
        return 0;
    }

    // �Ƿ���֤�ɹ�
    if (stResult.nResult != 0)
    {
        printf("token check error\n");
        return -1;
    }
    // ѡ������з����������Լ���׼������(Player -> Judger)
    READY_PARAM     stReady;

    strcpy(stReady.szToken, szToken);
    strcpy(stReady.szAction, "ready");

    memset(pSendBuffer, 0, MAX_SOCKET_BUFFER);
    nRet = CreateReadyParam(&stReady, pSendBuffer, &nLen);
    if (nRet != 0)
    {
        return nRet;
    }
    nRet = SendJuderData(hSocket, pSendBuffer, nLen);
    if (nRet != 0)
    {
        return nRet;
    }

    //��ս��ʼ֪ͨ(Judger -> Player)�� 
    memset(pRecvBuffer, 0, MAX_SOCKET_BUFFER);
    nRet = RecvJuderData(hSocket, pRecvBuffer);
    if (nRet != 0)
    {
        return nRet;
    }
    // ��������
    //Mapinfo �ṹ����ܴܺ󣬲�̫�ʺϷ���ջ�У����Զ���Ϊȫ�ֻ����ڴ����
    MAP_INFO            *pstMapInfo;
    MATCH_STATUS        *pstMatchStatus;
    FLAY_PLANE          *pstFlayPlane;

    pstMapInfo = (MAP_INFO *)malloc(sizeof(MAP_INFO));
    if (pstMapInfo == NULL)
    {
        return -1;
    }

    pstMatchStatus = (MATCH_STATUS *)malloc(sizeof(MATCH_STATUS));
    if (pstMapInfo == NULL)
    {
        return -1;
    }

    pstFlayPlane = (FLAY_PLANE *)malloc(sizeof(FLAY_PLANE));
    if (pstFlayPlane == NULL)
    {
        return -1;
    }

    memset(pstMapInfo, 0, sizeof(MAP_INFO));
    memset(pstMatchStatus, 0, sizeof(MATCH_STATUS));
    memset(pstFlayPlane, 0, sizeof(FLAY_PLANE));

    nRet = ParserMapInfo(pRecvBuffer + SOCKET_HEAD_LEN, pstMapInfo);
    if (nRet != 0)
    {
        return nRet;
    }

    // ��һ�ΰ����˻��ĳ�ʼ��ֵ��flayplane
    pstFlayPlane->nPurchaseNum = 0;
    pstFlayPlane->nUavNum = pstMapInfo->nUavNum;
    for (int i = 0; i < pstMapInfo->nUavNum; i++)
    {
        pstFlayPlane->astUav[i] = pstMapInfo->astUav[i];
    }

    // ���ݷ�����ָ���ͣ�Ľ��ܷ�������

	//�Լ����������
	vector<uav2good>Uav2Good;
		map<int, Point3d>initialpos;
		int goodno = 0;
    while (1)
    {
        // ���е�ǰʱ�̵����ݼ���, �����мƻ��ṹ�壬ע�⣺0ʱ�̲��ܽ����ƶ�������һ�ν����ѭ��ʱ
        if (pstMatchStatus->nTime != 0)
        {
			AlgorithmCalculationFun(pstMapInfo, pstMatchStatus, pstFlayPlane, Uav2Good, initialpos,goodno);
        }

        
        //���ͷ��мƻ��ṹ��
        memset(pSendBuffer, 0, MAX_SOCKET_BUFFER);
        nRet = CreateFlayPlane(pstFlayPlane, szToken, pSendBuffer, &nLen);
        if (nRet != 0)
        {
            return nRet;
        }
        nRet = SendJuderData(hSocket, pSendBuffer, nLen);
        if (nRet != 0)
        {
            return nRet;
        }

        printf("%s\n", pSendBuffer);

        // ���ܵ�ǰ����״̬
        memset(pRecvBuffer, 0, MAX_SOCKET_BUFFER);
        nRet = RecvJuderData(hSocket, pRecvBuffer);
        if (nRet != 0)
        {
            return nRet;
        }

        // ����
        nRet = ParserMatchStatus(pRecvBuffer + SOCKET_HEAD_LEN, pstMatchStatus);
        if (nRet != 0)
        {
            return nRet;
        }

        if (pstMatchStatus->nMacthStatus == 1)
        {
            // ��������
            printf("game over, we value %d, enemy value %d\n", pstMatchStatus->nWeValue, pstMatchStatus->nEnemyValue);
            return 0;
        }
    }

    // �ر�socket
    OSCloseSocket(hSocket);
    // ��Դ����
    free(pRecvBuffer);
    free(pSendBuffer);
    free(pstMapInfo);
    free(pstMatchStatus);
    free(pstFlayPlane);

    return 0;
}