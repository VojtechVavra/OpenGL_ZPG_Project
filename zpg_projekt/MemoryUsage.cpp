#include "MemoryUsage.hpp"

// Memory usage libraries
#include <windows.h>
#include <psapi.h>
#include <iostream>

/* 
   * Viditelnost anonymn�ho namespace se vztahuje na cel� soubor,
   * co� umo��uje p��stup k jeho entit�m odkudkoli v tomto souboru.
*/
namespace MemoryUsage
{
	namespace {
		short megabyte = 1024;
		/*
		*  Pokud je va�e aplikace multithreadov�, nebo pokud p�edpokl�d�te, 
		*  �e funkce printMemoryUsage() bude vol�na sou�asn� z v�ce vl�ken, 
		*  je lep�� pou��t variantu, kde je "pmc" jako lok�ln� prom�nnou ve funkci. 
		*  T�m se vyhnete probl�m�m s p�episov�n�m mezi v�cevl�knov�mu p��stupu.
		*
		*  Pokud v�ak v�te, �e va�e aplikace je single-threaded, 
		*  nebo �e funkce nebude vol�na sou�asn� z v�ce vl�ken, 
		*  varianta um�stnit prom�nnout mimo funkci do anonymn�ho namespacu 
		*  m��e b�t efektivn�j�� z hlediska v�konu a pam�ov� re�ie. 
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
