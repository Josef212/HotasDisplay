#pragma once

#include "IDisplayDataProvider.h"

#include <vector>
#include <minwindef.h>

extern "C" {
#include "DirectOutput.h"
}

class DirectOutputWrapper
{
public:
	DirectOutputWrapper();
	DirectOutputWrapper(IDisplayDataProvider* dataProvider);
	~DirectOutputWrapper();

	void SetDataProvider(IDisplayDataProvider* dataProvider) { m_dataProvider = dataProvider; }
	const IDisplayDataProvider* GetDataProvider() { return m_dataProvider; }

	HRESULT Init(const wchar_t* pluginName);
	HRESULT Deinit();

	void RegisterDevice();
	bool GetDeviceType();

	HRESULT SetDeviceProfile(const wchar_t* filepath);
	HRESULT SetPage(int pageNumber, const DWORD flag, const TCHAR* debugName = NULL);
	HRESULT SetString(int pageNumber, int stringLineId, const wchar_t* output);
	HRESULT SetString(int pageNumber, int stringLineId, const std::string& output);

	HRESULT RegisterSoftBtnCbk();
	HRESULT RegisterPageCbk();
	HRESULT UnregisterSoftBtnCbk();
	HRESULT UnregisterPageCbk();

	int GetCurrentPage();
	void HandlePageChange();
	void UpdateCurrentPage();
	void UpdatePage(int pageNumber);

private:
	void UpdatePageOnScroll(int onDownMinusUp);
	void CheckLastError();

	static void __stdcall OnEnumerateDevice(void* hDevice, void* pCtx);
	static void __stdcall OnDeviceChanged(void* hDevice, bool bAdded, void* pCtx);
	static void __stdcall OnPageChanged(void* hDevice, DWORD dwPage, bool bSetActive, void* pCtx);
	static void __stdcall OnSoftButtonChanged(void* hDevice, DWORD dwButtons, void* pCtx);

private:
	typedef std::vector<void*> DeviceList;

	IDisplayDataProvider* m_dataProvider;
	DeviceList m_deviceList;
	HMODULE m_dll;
	HRESULT m_hr;
	int m_currentPage, m_scroll;
};
