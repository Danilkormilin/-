
#include <ctime>
#include <iostream>
#include <algorithm>

// Объявление переменных для работы с CALLBACK функциями
bool opt = false, tep = false;
unsigned int tepclock = 0;
unsigned int optframe = 0;
unsigned int tepframe = 0;
CHeatmap CH1;

// CALLBACK Функция для получения максимальной 
// и минимальной температуры
// вызывается 1 раз в секунду из API
void CALLBACK cbRadiometryAttachCB(LLONG lAttachHandle, 
	NET_RADIOMETRY_DATA* pBuf, int nBufLen, 
	LDWORD dwUser)
{
	static CString strTemperature = "";
	float* pTempForPixels = new 
		float[pBuf->stMetaData.nWidth * 
		pBuf->stMetaData.nHeight * sizeof(float)];
	if (VSIF_RadiometryDataParse(pBuf, 
		NULL, pTempForPixels))
	{
		int x1, y1, x2, y2;
		float loTemp = 1000.0, hiTemp = -1000.0;
		for (int i = 0; 
			i < pBuf->stMetaData.nHeight; i++)
		{
			for (int j = 0; 
				j < pBuf->stMetaData.nWidth; j++)
			{
				float temp = pTempForPixels[i * 
					pBuf->stMetaData.nWidth + j];

				if (temp < loTemp)
				{
					loTemp = temp;
					x1 = j;
					y1 = i;
				}

				if (temp > hiTemp)
				{
					hiTemp = temp;
					x2 = j;
					y2 = i;
				}
			}
		}
		// запись информации в файл
		FILE* fp;
		fp = fopen("saves\\temps.bin", "ab");
		fwrite(&(pBuf->stMetaData.stTime.dwSecond), 
			sizeof(DWORD), 1, fp);
		fwrite(&loTemp, sizeof(float), 1, fp);
		fwrite(&hiTemp, sizeof(float), 1, fp);
		fclose(fp);
	}

	delete[] pTempForPixels;

}

// регистрация функции для вызова
void CHeatmap::OnStartfetch()
{
	NET_IN_RADIOMETRY_FETCH stInFetch = 
		{ sizeof(stInFetch), m_nHeatChannel };
	NET_OUT_RADIOMETRY_FETCH stOutFetch = 
		{ sizeof(stOutFetch) };
	VSIF_RadiometryFetch(m_lLoginID, 
		&stInFetch, &stOutFetch, 1000);
}

// функция вызываемая при нажатии на кнопку начала 
// записи
void CHeatmap::OnBnClickedStartfetch2()
{
	if (!m_isFetch)
	{
		if (VSIF_RadiometryStartFetch(m_lLoginID, 
			m_nHeatChannel))
		{
			m_isFetch = true;
		}
	}
	else
	{
		VSIF_RadiometryStopFetch(m_lLoginID, 
			m_nHeatChannel);
		m_isFetch = false;
	}
}

// регистраций функции для получения максимальной
// и минимальной температуры
void CHeatmap::OnAttach()
{
	NET_IN_RADIOMETRY_ATTACH stIn = { sizeof(stIn) };
	stIn.nChannel = m_nHeatChannel;
	stIn.dwUser = (LDWORD)this;
	stIn.cbNotify = cbRadiometryAttachCB;
	NET_OUT_RADIOMETRY_ATTACH stOut = { sizeof(stOut) };
	m_lAttachhandle = VSIF_RadiometryAttach(m_lLoginID, 
		&stIn, &stOut, 1000);
}

// Остановка вызова функции для получения максимальной и 
// минимальной температуры
void CHeatmap::OnStop()
{
	VSIF_RadiometryDetach(m_lAttachhandle);
}

// Функция авторизации 
void CRealPlayAndPTZControlDlg::OnBTLogin()
{
	//получение интерфейса ввода
	BOOL bValid = UpdateData(TRUE);	
	if (bValid)
	{
		//переменная с возможным кодом ошибки.
		int err = 0;	
		char* pchDVRIP;
		CString strDvrIP = GetDvrIP();
		pchDVRIP = (LPSTR)(LPCSTR)"192.168.25.199";
		WORD wDVRPort = (WORD)m_DvrPort;
		char* pchUserName = (LPSTR)(LPCSTR)"admin";
		char* pchPassword = (LPSTR)(LPCSTR)"susu";
		NET_DEVICEINFO_Ex deviceInfo = { 0 };
		//вызов аторизации из API 
		LLONG lRet = VSIF_LoginEx2(pchDVRIP, 
			wDVRPort, pchUserName,
			pchPassword, EM_LOGIN_SPEC_CAP_TCP, NULL,
			&deviceInfo, &err);
		// если регистрация успешна получение информации о
		// состоянии устройства
		if (0 != lRet)
		{
			m_LoginID = lRet;
			GetDlgItem(IDC_BT_Login)->EnableWindow(FALSE);
			GetDlgItem(IDC_BT_Leave)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_Play)->EnableWindow(TRUE);
			int nRetLen = 0;
			NET_DEV_CHN_COUNT_INFO stuChn = 
				{ sizeof(NET_DEV_CHN_COUNT_INFO) };
			stuChn.stuVideoIn.dwSize = 
				sizeof(stuChn.stuVideoIn);
			stuChn.stuVideoOut.dwSize = 
				sizeof(stuChn.stuVideoOut);
			BOOL bRet = VSIF_QueryDevState(lRet, 
				VS_DEVSTATE_DEV_CHN_COUNT, (char*)&stuChn, 
				stuChn.dwSize, &nRetLen);
			if (!bRet)
			{
				DWORD dwError = 
					VSIF_GetLastError() & 0x7fffffff;
			}
			m_nChannelCount = __max(deviceInfo.nChanNum, 
				stuChn.stuVideoIn.nMaxTotal);
			int nIndex = 0;
			m_comboChannel.ResetContent();
			for (int i = 0; i < m_nChannelCount; i++)
			{
				CString str;
				str.Format("%d", i + 1);
				nIndex = m_comboChannel.AddString(str);
				m_comboChannel.SetItemData(nIndex, i);
			}
			if (0 < m_comboChannel.GetCount())
			{
				nIndex = m_comboChannel.
					AddString(
						ConvertString("Multi_Preview"));
				m_comboChannel.SetItemData(nIndex, -1);
				m_comboChannel.SetCurSel(0);
			}
			CH1.m_lLoginID = m_LoginID;
			CH1.OnAttach();
			CH1.OnBnClickedStartfetch2();
		}
		else
		{
			//Сообщение об ошибке
			ShowLoginErrorReason(err);
		}
	}
	SetWindowText(
		ConvertString("RealPlayAndPTZControl"));

}

// CALLBACK функция вызываемая при получении данных
void CALLBACK DecodeCallback(LONG nPort, 
	FRAME_DECODE_INFO* pFrameDecodeInfo, 
	FRAME_INFO_EX* pFrameInfo, void* pUser)
{
	if (pUser == 0)
	{
		return;
	}
	CRealPlayAndPTZControlDlg* dlg = 
		(CRealPlayAndPTZControlDlg*)pUser;
	dlg->ReceiveDecodeData(nPort, 
		pFrameDecodeInfo, pFrameInfo);
}

//обработка полученных данных
void CRealPlayAndPTZControlDlg::ReceiveDecodeData(
	LONG nPort, 
	FRAME_DECODE_INFO* pFrameDecodeInfo, 
	FRAME_INFO_EX* pFrameInfo)
{
	// проверка на ошибки
	if (0 == nPort || 
		pFrameDecodeInfo == NULL || pFrameInfo == NULL)
	{
		return;
	}

	// обработка оптических кадров
	if (pFrameInfo->nFrameType == 0) 
	{
		FILE* fp;
		if (pFrameInfo->nHeight == 1080)
		{
			fp = fopen("saves\\1920x1080.yuv", "ab");
			opt = true;
			if (optframe != tepframe)
			{
				fclose(fp);
				return;
			}
			if (tep && opt)
			{
				optframe += 1;
			}

		}
		// обработка тепловых кадров
		else
		{
			fp = fopen("saves\\1280x1024.yuv", "ab");
			tep = true;
			if (optframe != tepframe + 1)
			{
				fclose(fp);
				return;
			}
			if (tep && opt)
			{
				tepframe += 1;
			}

		}
		// запись полученого кадра и контроль за динной кадра
		if (tep && opt)
		{
			tepclock = clock();
			fwrite(pFrameDecodeInfo->pVideoData[0], 
				1, pFrameDecodeInfo->nStride[0] * 
				pFrameDecodeInfo->nHeight[0], fp);
			fwrite(pFrameDecodeInfo->pVideoData[1], 
				1, pFrameDecodeInfo->nStride[1] * 
				pFrameDecodeInfo->nHeight[1], fp);
			fwrite(pFrameDecodeInfo->pVideoData[2], 
				1, pFrameDecodeInfo->nStride[2] * 
				pFrameDecodeInfo->nHeight[2], fp);
			Sleep(max(0, 50 - (clock() - tepclock)));
		}
		fclose(fp);
	}
}