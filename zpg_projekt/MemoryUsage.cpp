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
}
