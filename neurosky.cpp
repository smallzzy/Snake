#include "neurosky.h"

#include <iostream>
#include <QDebug>
#include <QMutex>
#include <Windows.h>

#include "thinkgear.h"
#include "NSK_Algo.h"

// import functions from algo.dll, basic definitions from NSK_Algo.h
// call functionName as (functionNameAddr)

#define NSK_ALGO_CDECL(ret, func, args)    \
    typedef ret(__cdecl *func##_Dll) args; \
    func##_Dll func##Addr = NULL;          \
    char func##Str[] = #func;

NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_Init, (eNSK_ALGO_TYPE type, const NS_STR dataPath))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_Uninit, (NS_VOID))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_RegisterCallback, (NskAlgo_Callback cbFunc, NS_VOID *userData))
NSK_ALGO_CDECL(NS_STR, NSK_ALGO_SdkVersion, (NS_VOID))
NSK_ALGO_CDECL(NS_STR, NSK_ALGO_AlgoVersion, (eNSK_ALGO_TYPE type))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_Start, (NS_BOOL bBaseline))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_Pause, (NS_VOID))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_Stop, (NS_VOID))
NSK_ALGO_CDECL(eNSK_ALGO_RET, NSK_ALGO_DataStream, (eNSK_ALGO_DATA_TYPE type, NS_INT16 *data, NS_INT dataLenght))

#ifdef _WIN64
#define ALGO_SDK_DLL "AlgoSdkDll64.dll"
#else
#define ALGO_SDK_DLL L"AlgoSdkDll.dll"
#endif

static void *getFuncAddr(HINSTANCE hinstLib, char *funcName, bool *bError)
{
    void *funcPtr = (void *)GetProcAddress(hinstLib, funcName);
    *bError = true;
    if (NULL == funcPtr)
    {
	wchar_t szBuff[100] = {0};
	swprintf(szBuff, 100, L"Failed ot get %s function address", (wchar_t *)funcName);
	//MessageBox(hwnd, szBuff, L"Error", MB_OK);
	*bError = false;
    }
    return funcPtr;
}

static bool getFuncAddrs(HINSTANCE hinstLib)
{
    bool bError;

    NSK_ALGO_InitAddr = (NSK_ALGO_Init_Dll)getFuncAddr(hinstLib, NSK_ALGO_InitStr, &bError);
    NSK_ALGO_UninitAddr = (NSK_ALGO_Uninit_Dll)getFuncAddr(hinstLib, NSK_ALGO_UninitStr, &bError);
    NSK_ALGO_RegisterCallbackAddr = (NSK_ALGO_RegisterCallback_Dll)getFuncAddr(hinstLib, NSK_ALGO_RegisterCallbackStr, &bError);
    NSK_ALGO_SdkVersionAddr = (NSK_ALGO_SdkVersion_Dll)getFuncAddr(hinstLib, NSK_ALGO_SdkVersionStr, &bError);
    NSK_ALGO_AlgoVersionAddr = (NSK_ALGO_AlgoVersion_Dll)getFuncAddr(hinstLib, NSK_ALGO_AlgoVersionStr, &bError);
    NSK_ALGO_StartAddr = (NSK_ALGO_Start_Dll)getFuncAddr(hinstLib, NSK_ALGO_StartStr, &bError);
    NSK_ALGO_PauseAddr = (NSK_ALGO_Pause_Dll)getFuncAddr(hinstLib, NSK_ALGO_PauseStr, &bError);
    NSK_ALGO_StopAddr = (NSK_ALGO_Stop_Dll)getFuncAddr(hinstLib, NSK_ALGO_StopStr, &bError);
    NSK_ALGO_DataStreamAddr = (NSK_ALGO_DataStream_Dll)getFuncAddr(hinstLib, NSK_ALGO_DataStreamStr, &bError);

    return bError;
}

static void AlgoSdkCallback(sNSK_ALGO_CB_PARAM param)
{
    static wchar_t buffer[512];
    static wchar_t sbuffer[512];
    static wchar_t qbuffer[512];

    Neurosky *handle = (Neurosky *)param.userData;
    bool bRunning = false;
    bool bInited = false;
    buffer[0] = sbuffer[0] = qbuffer[0] = 0;
    switch (param.cbType)
    {
    case NSK_ALGO_CB_TYPE_STATE:
    {
	//handle->sendStatus(QString("guigui"));

	// update gui here
    }
    break;
    case NSK_ALGO_CB_TYPE_SIGNAL_LEVEL:
    {
	eNSK_ALGO_SIGNAL_QUALITY sq = (eNSK_ALGO_SIGNAL_QUALITY)param.param.sq;
	switch (sq)
	{
	case NSK_ALGO_SQ_GOOD:
	    handle->currentOutput.signalQuality = QString("Good");
	    break;
	case NSK_ALGO_SQ_MEDIUM:
	    handle->currentOutput.signalQuality = QString("Medium");
	    break;
	case NSK_ALGO_SQ_NOT_DETECTED:
	    handle->currentOutput.signalQuality = QString("Not detected");
	    break;
	case NSK_ALGO_SQ_POOR:
	    handle->currentOutput.signalQuality = QString("Poor");
	    break;
	}
	handle->currentOutput.updated = true;
    }
    break;
    case NSK_ALGO_CB_TYPE_ALGO:
    {
	switch (param.param.index.type)
	{
	case NSK_ALGO_TYPE_ATT:
	{
	    int att = param.param.index.value.group.att_index;
		handle->currentOutput.attValue = att;
	    //swprintf(buffer, 512, L"ATT = %3d", att);
	    //		OutputToEditBox(buffer);
	    //		OutputLog(buffer);
	    break;
	}
	case NSK_ALGO_TYPE_MED:
	{
	    int med = param.param.index.value.group.med_index;
	    //swprintf(buffer, 512, L"MED = %3d", med);
	    //		OutputToEditBox(buffer);
	    //		OutputLog(buffer);
	    break;
	}
	case NSK_ALGO_TYPE_BLINK:
	{
	    int strength = param.param.index.value.group.eye_blink_strength;
	    swprintf(buffer, 512, L"Eye blink strength = %4d", strength);
	    //		OutputToEditBox(buffer);
	    //		OutputLog(buffer);
	    break;
	}
	case NSK_ALGO_TYPE_BP:
	{
	    swprintf(buffer, 512, L"EEG Bandpower: Delta: %1.4f Theta: %1.4f Alpha: %1.4f Beta: %1.4f Gamma: %1.4f",
		     param.param.index.value.group.bp_index.delta_power,
		     param.param.index.value.group.bp_index.theta_power,
		     param.param.index.value.group.bp_index.alpha_power,
		     param.param.index.value.group.bp_index.beta_power,
		     param.param.index.value.group.bp_index.gamma_power);
	    //		OutputToEditBox(buffer);
	    //OutputLog(buffer);
	    break;
	}
	}
	handle->currentOutput.updated = true;
    }
    break;
    }
}

// --- CONSTRUCTOR ---
Neurosky::Neurosky(std::string portName)
{
    connectionId = TG_GetNewConnectionId();

    comPortName = portName;

    std::cout << comPortName << std::endl;

    HINSTANCE hinstLib = LoadLibrary(ALGO_SDK_DLL);
    getFuncAddrs(hinstLib);
    // you could copy data from constructor arguments to internal variables here.
}

// --- DECONSTRUCTOR ---
Neurosky::~Neurosky()
{
    eNSK_ALGO_RET ret = (NSK_ALGO_StopAddr)();

	ret = (NSK_ALGO_UninitAddr)();
	if (ret == NSK_ALGO_RET_SUCCESS)
	{
		qDebug("algo uninit success");
	}

    TG_Disconnect(connectionId);     // disconnect
    TG_FreeConnection(connectionId); // clean up

    qDebug("destroyed!");
    emit destroyed();

    // free resources
}

// --- PROCESS ---
// Start processing data.
void Neurosky::process()
{
    if (bConnected == false)
    {
	// connect thinkgear
	int errCode = TG_Connect(connectionId,
				 comPortName.c_str(),
				 TG_BAUD_57600,
				 TG_STREAM_PACKETS);

	if (errCode < 0)
	{
	    qDebug() << "ERROR: TG_Connect() returned " << errCode;
		emit signalError();
	    return;
	    //emit signalStatus("Connected");
	}
	//emit signalStatus();

	// set thinkgear electrical filter
	errCode = MWM15_setFilterType(connectionId, MWM15_FILTER_TYPE_60HZ);
	if (errCode < 0)
	{
	    qDebug() << "ERROR: MWM15_setFilterType() returned " << errCode;
	}

	bConnected = true;
    }

    if (bInited == false)
    {
	int lSelectedAlgos = 0;
	char readBuf[1024] = {0};
	lSelectedAlgos |= NSK_ALGO_TYPE_ATT;
	eNSK_ALGO_RET ret = (NSK_ALGO_RegisterCallbackAddr)(AlgoSdkCallback, this);
	ret = (NSK_ALGO_InitAddr)((eNSK_ALGO_TYPE)(lSelectedAlgos), readBuf);

	if (NSK_ALGO_RET_SUCCESS == ret)
	{
	    //MessageBox(hWnd, L"Algo SDK inited successfully", L"Information", MB_OK);
	    qDebug() << "Algo SDK inited successfully";
	}

	bInited = true;
    }

    if (bRunning == false)
    {
	eNSK_ALGO_RET ret = (NSK_ALGO_StartAddr)(NS_FALSE);

	bRunning = true;
    }

    forever
    {
	mutex.lock();
	if (bInterrupt)
	{
	    qDebug() << "interrupt of thread?";
	    break;
	    //condition.wait(&mutex);
	}

	if (currentOutput.updated)
	{
	    currentOutput.updated = false;
	    emit signalUpdate();
	}
	mutex.unlock();
	read();
    }

    emit signalFinished();

    qDebug() << "end of thread?";
}

Neurosky::Output Neurosky::output() const
{
    return currentOutput;
}

void Neurosky::read()
{
    int packetsRead = TG_ReadPackets(connectionId, 1);

    if (packetsRead == 1)
    {
	//qDebug() << "read 1 packet";

	/* If the Packet containted a new raw wave value... */
	if (TG_GetValueStatus(connectionId, TG_DATA_RAW) != 0)
	{
	    /* Get and print out the new raw value */
	    rawData[rawCount++] = (short)TG_GetValue(connectionId, TG_DATA_RAW);
	    lastRawCount = rawCount;
	    if (rawCount == 512)
	    {
		(NSK_ALGO_DataStreamAddr)(NSK_ALGO_DATA_TYPE_EEG, rawData, rawCount);
		rawCount = 0;
	    }
	}
	if (TG_GetValueStatus(connectionId, TG_DATA_POOR_SIGNAL) != 0)
	{
	    short pq = (short)TG_GetValue(connectionId, TG_DATA_POOR_SIGNAL);
	    SYSTEMTIME st;
	    GetSystemTime(&st);
	    swprintf(buffer, 100, L"%6d, PQ[%3d], [%d]", st.wSecond * 1000 + st.wMilliseconds, pq, lastRawCount);
	    rawCount = 0;
	    // OutputLog(buffer);
	    (NSK_ALGO_DataStreamAddr)(NSK_ALGO_DATA_TYPE_PQ, &pq, 1);
	}
	if (TG_GetValueStatus(connectionId, TG_DATA_ATTENTION) != 0)
	{
	    short att = (short)TG_GetValue(connectionId, TG_DATA_ATTENTION);
	    (NSK_ALGO_DataStreamAddr)(NSK_ALGO_DATA_TYPE_ATT, &att, 1);
	}
	if (TG_GetValueStatus(connectionId, TG_DATA_MEDITATION) != 0)
	{
	    short med = (short)TG_GetValue(connectionId, TG_DATA_MEDITATION);
	    (NSK_ALGO_DataStreamAddr)(NSK_ALGO_DATA_TYPE_MED, &med, 1);
	}
    }
}

void Neurosky::quit2()
{
    QMutexLocker locker(&mutex);
    qDebug() << "signal received ";
    bInterrupt = true;
}