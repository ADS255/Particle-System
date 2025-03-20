#include "FileHandler.h"


void FileHandler::SaveFileJSON(nlohmann::json json)
{
	IFileSaveDialog* pFileSave;
	std::string filePath;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL, IID_IFileSaveDialog, (void**)&pFileSave);
		if (SUCCEEDED(hr))
		{
			pFileSave->SetDefaultExtension(L"json");
			pFileSave->SetFileName(L"Untitled.json");

			COMDLG_FILTERSPEC fileTypes[] = {
				{L"JSON Files (*.json)", L"*.json"}
			};
			pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

			// Show the Save As dialog
			hr = pFileSave->Show(NULL);
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileSave->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr))
					{
						char narrowPath[MAX_PATH];
						wcstombs(narrowPath, pszFilePath, MAX_PATH);
						filePath = narrowPath;

						std::ofstream outFile(filePath, std::ios::out | std::ios::trunc);
						if (outFile.is_open())
						{
							outFile << json.dump(4);;
							outFile.close();
						}
						else
						{
							std::cerr << "Failed to save file: " << filePath << std::endl;
						}

						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileSave->Release();
		}
		CoUninitialize();
	}
}

nlohmann::json FileHandler::LoadFileJSON()
{
	IFileOpenDialog* pFileOpen;
	std::string filePath;
	nlohmann::json jsonData;

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (SUCCEEDED(hr))
	{
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, (void**)&pFileOpen);
		if (SUCCEEDED(hr))
		{
			COMDLG_FILTERSPEC fileTypes[] = {
				{ L"JSON Files (*.json)", L"*.json" }
			};
			pFileOpen->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);

			hr = pFileOpen->Show(NULL);
			if (SUCCEEDED(hr))
			{
				IShellItem* pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
					if (SUCCEEDED(hr))
					{
						char narrowPath[MAX_PATH];
						wcstombs(narrowPath, pszFilePath, MAX_PATH);
						filePath = narrowPath;
						CoTaskMemFree(pszFilePath);

						std::ifstream inFile(filePath);
						if (inFile.is_open())
						{
							try
							{
								inFile >> jsonData;
							}
							catch (const std::exception& e)
							{
								std::cerr << "Error parsing JSON: " << e.what() << std::endl;
							}
							inFile.close();
						}
						else
						{
							std::cerr << "Failed to open file: " << filePath << std::endl;
						}
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	return jsonData;
}
