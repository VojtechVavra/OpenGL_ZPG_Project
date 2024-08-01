#include "MemoryUsage.hpp"

// Memory usage libraries
#include <windows.h>
#include <psapi.h>
#include <iostream>

/* 
   * Viditelnost anonymního namespace se vztahuje na celı soubor,
   * co umoòuje pøístup k jeho entitám odkudkoli v tomto souboru.
*/
namespace MemoryUsage
{
	namespace {
		short megabyte = 1024;
		/*
		*  Pokud je vaše aplikace multithreadová, nebo pokud pøedpokládáte, 
		*  e funkce printMemoryUsage() bude volána souèasnì z více vláken, 
		*  je lepší pouít variantu, kde je "pmc" jako lokální promìnnou ve funkci. 
		*  Tím se vyhnete problémùm s pøepisováním mezi vícevláknovému pøístupu.
		*
		*  Pokud však víte, e vaše aplikace je single-threaded, 
		*  nebo e funkce nebude volána souèasnì z více vláken, 
		*  varianta umístnit promìnnout mimo funkci do anonymního namespacu 
		*  mùe bıt efektivnìjší z hlediska vıkonu a pamìové reie. 
		*/
	}
	
	void printMemoryUsage(const std::string& name)
	{
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE process = GetCurrentProcess();

		if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc))) {
			int memoryUsedInKB = pmc.WorkingSetSize / megabyte;
			std::cout << "- " << name << " Current memory usage: " << memoryUsedInKB << " KB (" << memoryUsedInKB / megabyte << " MB)" << std::endl;
		}
		else {
			std::cerr << "Failed to get memory usage." << std::endl;
		}

		CloseHandle(process);
	}

	int getMemoryUsage()
	{
		PROCESS_MEMORY_COUNTERS pmc;
		HANDLE process = GetCurrentProcess();

		int memoryUsedInMB = -1;
		int memoryUsedInKB = -1;
		if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc))) {
			memoryUsedInKB = pmc.WorkingSetSize / megabyte;
			memoryUsedInMB = memoryUsedInKB / megabyte;
		}

		CloseHandle(process);

		//return memoryUsedInKB;
		return memoryUsedInMB;
	}


	/*void PrintVRAMInfo() {
		IDXGIFactory* pFactory = nullptr;
		IDXGIAdapter* pAdapter = nullptr;

		if (SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory))) {
			if (SUCCEEDED(pFactory->EnumAdapters(0, &pAdapter))) {
				DXGI_ADAPTER_DESC desc;
				if (SUCCEEDED(pAdapter->GetDesc(&desc))) {
					std::wcout << L"VRAM Size: " << desc.DedicatedVideoMemory / (1024 * 1024) << L" MB" << std::endl;
				}
				pAdapter->Release();
			}
			pFactory->Release();
		}
		else {
			std::cerr << "Failed to create DXGI factory" << std::endl;
		}
	}*/
}
