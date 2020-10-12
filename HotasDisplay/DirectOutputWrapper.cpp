#include "stdafx.h"
#include "DirectOutputWrapper.h"

#define GET_FN(TYPE, DLL, NAME) (TYPE)GetProcAddress(DLL, NAME)
#define INSTANCE(ctx) static_cast<DirectOutputWrapper*>(ctx)
//#define CHECK_HR(hr) if(FAILED(hr)) { std::cout << "FAILED: " << std::system_category().message(m_hr) << std::endl; } else { std::cout << "DONE." << std::endl; }
#define CHECK_HR(hr) CheckLastError()

DirectOutputWrapper::DirectOutputWrapper() : m_deviceList(), m_dll(NULL), m_hr(S_OK), m_currentPage(0), m_scroll(0)
{
	
}

DirectOutputWrapper::~DirectOutputWrapper()
{
	
}

HRESULT DirectOutputWrapper::Init(const wchar_t* pluginName)
{
	std::cout << "Loading DirectOutput library... ";
	m_dll = LoadLibrary(TEXT("Dependencies/DirectOutput.dll"));
	std::cout << (m_dll != NULL ? "DONE" : "FAILED") << std::endl;

	if (m_dll == NULL)
	{
		return -1;
	}

	std::cout << "Init DirectOutput library... ";

	auto initFn = GET_FN(Pfn_DirectOutput_Initialize, m_dll, "DirectOutput_Initialize");
	m_hr = initFn(pluginName);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::Deinit()
{
	std::cout << "Deinit DirectOutput library... ";

	auto deinitFn = GET_FN(Pfn_DirectOutput_Deinitialize, m_dll, "DirectOutput_Initialize");
	m_hr = deinitFn();
	CHECK_HR(m_hr);

	std::cout << "Freeing DirectOutput library... ";
	std::cout << (FreeLibrary(m_dll) ? "DONE" : "FAILED") << std::endl;

	return m_hr;
}

void DirectOutputWrapper::RegisterDevice()
{
	std::cout << "Registering device callback... ";
	auto deviceCbkFn = GET_FN(Pfn_DirectOutput_RegisterDeviceCallback, m_dll, "DirectOutput_RegisterDeviceCallback");
	m_hr = deviceCbkFn(OnDeviceChanged, this);
	CHECK_HR(m_hr);

	std::cout << "Enumerating device callback... ";
	auto enumerateFn = GET_FN(Pfn_DirectOutput_Enumerate, m_dll, "DirectOutput_Enumerate");
	m_hr = enumerateFn(OnEnumerateDevice, this);
	CHECK_HR(m_hr);
}

bool DirectOutputWrapper::GetDeviceType()
{
	std::cout << "Getting devices [" << m_deviceList.size() << "]... ";

	auto getDeviceTypeFn = GET_FN(Pfn_DirectOutput_GetDeviceType, m_dll, "DirectOutput_GetDeviceType");
	GUID typeGUID = { 0 };

	if (m_deviceList.size() == 0)
	{
		std::cout << std::endl;
		std::cout << "The controller is not connected or is not a X52 Pro." << std::endl;
		std::cout << "Close the app and plug the controller." << std::endl;
		return false;
	}

	for (auto device : m_deviceList)
	{
		m_hr = getDeviceTypeFn(device, &typeGUID);
		if (FAILED(m_hr))
		{
			std::cout << "FAILED." << std::endl;
			return false;
		}

		if (typeGUID == DeviceType_X52Pro)
		{
			std::cout << "DONE. Device found > X52Pro" << std::endl;
			return true;
		}
	}

	return false;
}

HRESULT DirectOutputWrapper::SetDeviceProfile(const wchar_t* filepath)
{
	std::cout << "Setting device profile... ";
	auto setProfileFn = GET_FN(Pfn_DirectOutput_SetProfile, m_dll, "DirectOutput_SetProfile");
	m_hr = setProfileFn(m_deviceList[0], wcslen(filepath), filepath);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::SetPage(int pageNumber, const DWORD flag, const TCHAR* debugName)
{
	std::cout << "Adding page...";
	auto setPageFn = GET_FN(Pfn_DirectOutput_AddPage, m_dll, "DirectOutput_AddPage");
	m_hr = setPageFn(m_deviceList[0], pageNumber, flag);
	CHECK_HR(m_hr);

	if (flag & FLAG_SET_AS_ACTIVE)
	{
		m_currentPage = pageNumber;
	}

	return m_hr;
}

HRESULT DirectOutputWrapper::SetString(int pageNumber, int stringLineId, const wchar_t* output)
{
	std::cout << "Setting string [" << pageNumber << " - " << stringLineId << "] ... ";
	auto setStringFn = GET_FN(Pfn_DirectOutput_SetString, m_dll, "DirectOutput_SetString");
	m_hr = setStringFn(m_deviceList[0], pageNumber, stringLineId, wcslen(output), output);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::RegisterSoftBtnCbk()
{
	std::cout << "Registering soft buttons callback... ";
	auto regSoftBtnFn = GET_FN(Pfn_DirectOutput_RegisterSoftButtonCallback, m_dll, "DirectOutput_RegisterSoftButtonCallback");
	m_hr = regSoftBtnFn(m_deviceList[0], OnSoftButtonChanged, this);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::RegisterPageCbk()
{
	std::cout << "Registering page callback... ";
	auto regPageFn = GET_FN(Pfn_DirectOutput_RegisterPageCallback, m_dll, "DirectOutput_RegisterPageCallback");
	regPageFn(m_deviceList[0], OnPageChanged, this);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::UnregisterSoftBtnCbk()
{
	std::cout << "Unregistering soft buttons callback... ";
	auto unregSoftBtnFn = GET_FN(Pfn_DirectOutput_RegisterSoftButtonCallback, m_dll, "DirectOutput_RegisterSoftButtonCallback");
	m_hr = unregSoftBtnFn(m_deviceList[0], NULL, NULL);
	CHECK_HR(m_hr);

	return m_hr;
}

HRESULT DirectOutputWrapper::UnregisterPageCbk()
{
	std::cout << "Unregistering page callback... ";
	auto unregPageFn = GET_FN(Pfn_DirectOutput_RegisterPageCallback, m_dll, "DirectOutput_RegisterPageCallback");
	unregPageFn(m_deviceList[0], NULL, NULL);
	CHECK_HR(m_hr);

	return m_hr;
}

int DirectOutputWrapper::GetCurrentPage()
{
	return m_currentPage;
}

void DirectOutputWrapper::HandlePageChange()
{
	UpdateCurrentPage();
}

void DirectOutputWrapper::UpdateCurrentPage()
{
	UpdatePage(m_currentPage);
}

void DirectOutputWrapper::UpdatePage(int pageNumber)
{
	std::cout << "Updating page " << pageNumber << std::endl;

	wchar_t buffer[16];
	swprintf_s(buffer, 16, L"Page: %d", pageNumber);
	SetString(pageNumber, 0, buffer);
	swprintf_s(buffer, 16, L"Scroll: %d", m_scroll);
	SetString(pageNumber, 1, buffer);
}

void DirectOutputWrapper::UpdatePageOnScroll(int onDownMinusUp)
{
	assert(onDownMinusUp == 1 || onDownMinusUp == -1);
	m_scroll += onDownMinusUp;
	UpdateCurrentPage();
}

void DirectOutputWrapper::CheckLastError()
{
	if (m_hr == S_OK)
	{
		std::cout << "DONE." << std::endl;
		return;
	}

	std::string error;

	switch (m_hr)
	{
	case E_PAGENOTACTIVE: error = "Page not active."; break;
	case E_BUFFERTOOSMALL: error = "Buffer too small."; break;
	default: error = std::system_category().message(m_hr); break;
	}

	std::cout << "FAILED: " << error << std::endl;
}

void __stdcall DirectOutputWrapper::OnEnumerateDevice(void* hDevice, void* pCtx)
{
	INSTANCE(pCtx)->m_deviceList.push_back(hDevice);
}

void __stdcall DirectOutputWrapper::OnDeviceChanged(void* hDevice, bool bAdded, void* pCtx)
{
	auto instance = INSTANCE(pCtx);
	if (bAdded)
	{
		TCHAR tsz[1024];
		size_t size = sizeof(tsz) / sizeof(tsz[0]);
		_sntprintf_s(tsz, size, size, _T("DeviceAdded(%p)\n"), hDevice);
		OutputDebugString(tsz);

		instance->m_deviceList.push_back(hDevice);
	}
	else
	{
		TCHAR tsz[1024];
		size_t size = sizeof(tsz) / sizeof(tsz[0]);
		_sntprintf_s(tsz, size, size, _T("DeviceRemoved(%p)\n"), hDevice);
		OutputDebugString(tsz);

		auto it = std::find(instance->m_deviceList.begin(), instance->m_deviceList.end(), hDevice);
		if (it != instance->m_deviceList.end())
		{
			instance->m_deviceList.erase(it);
		}
	}
}

void __stdcall DirectOutputWrapper::OnPageChanged(void* hDevice, DWORD dwPage, bool bSetActive, void* pCtx)
{
	std::cout << "\t# PageChanged: " << dwPage << " - " << (bSetActive ? "true" : "false") << std::endl;

	if (bSetActive)
	{
		auto instance = INSTANCE(pCtx);
		instance->m_currentPage = dwPage;
		instance->HandlePageChange();
	}
}

void __stdcall DirectOutputWrapper::OnSoftButtonChanged(void* hDevice, DWORD dwButtons, void* pCtx)
{
	std::cout << "\t# SoftButtonPressed: " << dwButtons << std::endl;

	if (dwButtons & SoftButton_Up)
		INSTANCE(pCtx)->UpdatePageOnScroll(-1);
	else if (dwButtons & SoftButton_Down)
		INSTANCE(pCtx)->UpdatePageOnScroll(1);
	else if (dwButtons & SoftButton_Select)
	{
		auto instance = INSTANCE(pCtx);
		instance->m_scroll = 0;
		instance->UpdateCurrentPage();
	}
}
