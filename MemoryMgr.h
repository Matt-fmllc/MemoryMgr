

#ifndef _MEMORY_MANAGER_
	#define _MEMORY_MANAGER_


#include <Malloc.h>
#include "Common.h"
//#include "..\\Core\\Source\\CAssert.h"
//#include "..\\Core\\Source\\CBaseTypes.h"

#define _MEM_SYSTEM_ON_		// define to use internal memory system


class CMemObject;
class CMemoryMgr;

// global reference
extern CMemoryMgr* g_pMemMgr;

typedef enum{
	E_MT_NONE = 0,
	E_MT_DEBUG,
	
	E_MT_UNDEFINED,
	E_MT_OBJECT,
	E_MT_OBJECT_DATA,
	E_MT_STRINGS,
	E_MT_FILE,
	E_MT_STATIC_FILE,
	E_MT_WORLD_GEOMETRY,
	E_MT_TEXTURE,
	E_MT_STATIC_TEXTURE,
	E_MT_MODEL,
	E_MT_STATIC_MODEL,
	E_MT_SOUND,
	E_MT_STATIC_SOUND,
	E_MT_ANIMATION,
	E_MT_STATIC_ANIMATION,

	E_MT_MAX
} E_MEMTYPE;

const Int MAX_HEAPS = 20;

//////////////////////////////////////////////////////////////////////////
//	CChunk
//////////////////////////////////////////////////////////////////////////
class CChunk
{
private:
	Bool		m_bStatus;
	Int			m_iSize;
protected:
public:
	CChunk() {}
	~CChunk() {}
};


//////////////////////////////////////////////////////////////////////////
//	CHeap
//////////////////////////////////////////////////////////////////////////
class CHeap
{
private:
	CMemoryMgr*	m_pMemMgr;
	CChunk*			m_pChunkList;
	void*				m_pHeapBase;
	void*				m_pCurHeapLoc;
	Int					m_iHeapSize;
protected:
public:
	CHeap() {}
	~CHeap() {}
};

//////////////////////////////////////////////////////////////////////////
//	CMemoryMgr
//////////////////////////////////////////////////////////////////////////
class CMemoryMgr
{
public:
	friend CMemObject;
	friend CHeap;
	
private:
	CMemObject*		m_pObjectList;
	
	CHeap					m_aHeaps[MAX_HEAPS];

	Long					m_lTotalMemAllocated;
	Long					m_lCurMemAllocated;
	Int						m_iNumMemAllocations;
	Int						m_iNumMemDeAllocations;

protected:
public:
	CMemoryMgr();
	~CMemoryMgr();

	void Init_MemoryMgr() {}
	void ShutDown_MemoryMgr() {}

	inline void* Malloc(Int _iSize, E_MEMTYPE _eType);
	inline void  DeAlloc(void* _p);
	inline void	 Delete(void* _p);
};

inline void* CMemoryMgr::Malloc(Int _iSize, E_MEMTYPE _eType)
{ 
	m_iNumMemAllocations++;
	m_lTotalMemAllocated += _iSize;
	m_lCurMemAllocated += _iSize;
	return malloc(_iSize); 	
}
inline void CMemoryMgr::DeAlloc(void* _p)
{
	m_iNumMemDeAllocations++;
	free(_p);
}
inline void CMemoryMgr::Delete(void* _p)
{
	DeAlloc(_p);
//	m_iNumMemDeAllocations--;
//	::delete(_p);
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
class CMemObject
{
private:
	CMemoryMgr*	m_pMemMgr;
	void*				m_pObject;
	int					m_iSize;

protected:
public:
	CMemObject(CMemoryMgr* _pMemMgr) { m_pMemMgr = _pMemMgr; }
	~CMemObject() {}

	void SetSize(int _iSize) { ASSERT(m_iSize>0); m_iSize = _iSize; }
};


//////////////////////////////////////////////////////////////////////////
//	Operator Overrides
//////////////////////////////////////////////////////////////////////////
#ifdef _MEM_SYSTEM_ON_
	
#ifdef malloc
#undef malloc
#define malloc()	ASSERT("malloc not allowed, use Malloc instead")
#endif

	//#pragma push_macro("new")
//	#undef  new
	inline void* operator new( size_t cb )
	{
//		void *res = malloc( cb );
		void *res = g_pMemMgr->Malloc(cb, E_MT_OBJECT);

		// 	RTCCALLBACK(_RTC_Allocate_hook, (res, cb, 0));

		return res;
	}

	inline void* operator new( size_t cb, E_MEMTYPE _memType )
	{
		return NULL;
	}
	//#pragma pop_macro("new")

//	#undef delete
	inline void operator delete( void* _p )
	{
		g_pMemMgr->Delete(_p);
	}

	inline void operator delete( void* _p, size_t _size )
	{
		ASSERT(0 && "Not Handled");
	}

	inline void* Malloc(size_t _size, E_MEMTYPE _E_MEMTYPE)
	{
		void* res = g_pMemMgr->Malloc(_size, _E_MEMTYPE);

		return res;
	}

	inline void Free(void* _p)
	{
		g_pMemMgr->DeAlloc(_p);
	}

#else

	inline void* Malloc(size_t _size, E_MEMTYPE _E_MEMTYPE)
	{
		return malloc(_size);
	}

	inline void Free(void* _p)
	{
		free(_p);
	}

#endif

#endif