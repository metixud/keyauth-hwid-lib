#include "utils.hpp"

#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

std::string utils::get_hwid() {
	HRESULT hres;
	std::string uuid = "none";

	// Initialize COM.
	hres = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hres))
		return uuid;

	// Set general COM security levels
	hres = CoInitializeSecurity(
		NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE, NULL);
	if (FAILED(hres)) {
		CoUninitialize();
		return uuid;
	}

	IWbemLocator *pLoc = NULL;
	hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *)&pLoc);
	if (FAILED(hres)) {
		CoUninitialize();
		return uuid;
	}

	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL, NULL, 0, NULL, 0, 0, &pSvc);
	if (FAILED(hres)) {
		pLoc->Release();
		CoUninitialize();
		return uuid;
	}

	hres = CoSetProxyBlanket(
		pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
		RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL, EOAC_NONE);
	if (FAILED(hres)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return uuid;
	}

	IEnumWbemClassObject* pEnumerator = NULL;
	hres = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT UUID FROM Win32_ComputerSystemProduct"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL, &pEnumerator);
	if (FAILED(hres)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return uuid;
	}

	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
	if (pEnumerator) {
		HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
		if (uReturn != 0 && pclsObj) {
			VARIANT vtProp;
			hr = pclsObj->Get(L"UUID", 0, &vtProp, 0, 0);
			if (SUCCEEDED(hr) && vtProp.vt == VT_BSTR) {
				uuid = _bstr_t(vtProp.bstrVal);
			}
			VariantClear(&vtProp);
			pclsObj->Release();
		}
		pEnumerator->Release();
	}
	pSvc->Release();
	pLoc->Release();
	CoUninitialize();
	return uuid;
}

std::time_t utils::string_to_timet(std::string timestamp) {
	auto cv = strtol(timestamp.c_str(), NULL, 10);

	return (time_t)cv;
}

std::tm utils::timet_to_tm(time_t timestamp) {
	std::tm context;

	localtime_s(&context, &timestamp);

	return context;
}
