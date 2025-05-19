#include <cstdio>
#include <WinSock2.h>

#include "function.h"


#pragma comment(lib,"ws2_32.lib")


int ParserRTPMpegTS(int iPort)
{
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	if (WSAStartup(sockVersion, &wsaData))
		return -1;

	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == serSocket)
	{
		printf("socker error!!!\n");

		return -1;
	}

	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(iPort);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(serSocket, (sockaddr*)&serAddr,sizeof(serAddr)))
	{
		printf("bind error!!!\n");
		closesocket(serSocket);

		return -1;
	}

	printf("Listening on port %d\n", iPort);

	sockaddr_in remoteAddr;
	int iSockSize = sizeof(sockaddr_in);
	int iCount = 0;
	char recvData[10000] = { 0 };
	FILE* pFile = fopen("../Resource/output_dump.ts", "wb+");

	while (1)
	{
		// 读取一个rtp包
		int iPktSize = recvfrom(serSocket, recvData, 10000, 0, (sockaddr*)&remoteAddr, &iSockSize);
		if (iPktSize > 0)
		{
			// 解析rtp header
			SRTPFixedHeader rtpHeader;
			int iRTPHeader = sizeof(SRTPFixedHeader);
			memcpy(&rtpHeader, recvData, iRTPHeader);

			char payloadStr[10] = { 0 };
			char payload = rtpHeader.payLoad;
			switch (payload) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18: sprintf(payloadStr, "Audio"); break;
			case 31: sprintf(payloadStr, "H.261"); break;
			case 32: sprintf(payloadStr, "MPV"); break;
			case 33: sprintf(payloadStr, "MP2T"); break;
			case 34: sprintf(payloadStr, "H.263"); break;
			case 96: sprintf(payloadStr, "H.264"); break;
			default:sprintf(payloadStr, "other"); break;
			}

			unsigned int timeStamp = ntohl(rtpHeader.timeStamp);
			unsigned int seqNum = ntohs(rtpHeader.seqNum);

			fprintf(stdout, "[RTP Pkt] %5d| %5s| %10u| %5d| %5d|\n", iCount, payloadStr, timeStamp, seqNum, iPktSize);

			// 解析rtp data
			char* pRtpData = recvData + iRTPHeader;
			int iRtpDataSize = iPktSize - iRTPHeader;
			fwrite(pRtpData, 1, iRtpDataSize, pFile);

			// 解析MPEGTS
			if (33 == payload)
			{
				//SMPEGTSFixedHeader mpegtsHeader;
				for (int i = 0; i < iRtpDataSize; i = i + 188)
				{
					if(0x47 != pRtpData[i])
						break;

					fprintf(stdout, "	[MPEGTS Pkt\n");
				}
			}
		}
		else
		{
			fprintf(stdout, "[UDP Pkt] %5d| %5d|\n", iCount, iPktSize);
			fwrite(recvData, iPktSize, 1, pFile);
		}

		++iCount;
	}

	closesocket(serSocket);
	WSACleanup();
	if (nullptr != pFile)
		fclose(pFile);

	return 0;
}
