

#include "pch.h"


//#include <Malloc.h>
#include "MemoryMgr.h"

#ifdef _MEM_SYSTEM_ON_
	CMemoryMgr g_MemMgr;
	CMemoryMgr* g_pMemMgr = &g_MemMgr;
#else
	CMemoryMgr* g_pMemMgr = 0;
#endif

CMemoryMgr::CMemoryMgr() :
m_lCurMemAllocated(0),
m_lTotalMemAllocated(0),
m_iNumMemAllocations(0),
m_iNumMemDeAllocations(0)
{
	g_pMemMgr = this;
}

CMemoryMgr::~CMemoryMgr()
{
	g_pMemMgr = NULL;
}

