//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CNamenMap.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Pre-STL name map
//
#pragma once
#include "StdAfx.h"

#pragma warning (disable: 4312)
#pragma warning (disable: 4311)
#pragma warning (disable: 4267)

class CNamenMap
{
public:
	// Association
  struct CAssoc
  {
  public:
	  CAssoc* pNext;
	  CString key;

	  CAssoc* pListNext; // Linked list
	  CAssoc* pListPrev; // Linked list

	  void* value;
  };

  struct iterator_type {};
  typedef iterator_type* iterator;

public:

// Construction
	CNamenMap(bool uniekeNamen = true);

// Attributes
	// number of elements
	int GetCount() const;
	bool IsEmpty() const;

	// Lookup
	bool Lookup(const char* key, void*& rValue) const;
	bool LookupKey(const char* key, CString& rKey) const;
	bool Contains(const char* key) const;

// Operations
	// Lookup and add if not there
	void*& operator[](const CString& key);

	// add a new (key, value) pair
	void SetAt(const CString& key, void* newValue);
	int  SetPos(const CString& key, int pos);

	// removing existing (key, ?) pair
	bool RemoveKey(const CString& key);
	void RemoveAll();

	// iterating all (key, value) pairs
	iterator GetStartPosition() const;
	void GetNextAssoc(iterator& rNextPosition, CString& rKey, void*& rValue) const;

	// advanced features for derived classes
	unsigned int GetHashTableSize() const;
	void InitHashTable(unsigned int hashSize, bool bAllocNow = true);

// Overridables: special non-virtual (see map implementation for details)
	// Routine used to user-provided hash keys
	static unsigned int HashKey(const CString& key);
	static unsigned int HashKey(unsigned long  key);

// Implementation
protected:
	CAssoc* NewAssoc();
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(const char* key, unsigned int& nHash) const;

protected:
	unsigned int m_nHashTableSize;
	int m_nCount;

	CAssoc** m_pHashTable;

  bool m_caseSensitive;
  bool m_uniekeNaam;

	CAssoc* m_pStartLijst;
	CAssoc* m_pEindeLijst;

public:
	~CNamenMap();
};

#ifndef _AFXCOLL_INLINE
#define _AFXCOLL_INLINE inline
#endif

_AFXCOLL_INLINE
int CNamenMap::GetCount() const
	{ return m_nCount; }

_AFXCOLL_INLINE
bool CNamenMap::IsEmpty() const
	{ return m_nCount == 0; }

_AFXCOLL_INLINE
void CNamenMap::SetAt(const CString& key, void* newValue)
	{ (*this)[key] = newValue; }

_AFXCOLL_INLINE
CNamenMap::iterator 
CNamenMap::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : ((iterator) -1L); }

_AFXCOLL_INLINE
unsigned int CNamenMap::GetHashTableSize() const
	{ return m_nHashTableSize; }

_AFXCOLL_INLINE
unsigned int CNamenMap::HashKey(const CString& pkey)
{
  const char* key = (const char*)pkey;
	unsigned int nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + tolower(*key++);
	return nHash;
}

_AFXCOLL_INLINE
unsigned int CNamenMap::HashKey(unsigned long key)
{
	// default identity hash - works for most primitive values
	return (unsigned int)(key >> 4);
}


class CMapIStringLong : private CNamenMap
{
public:
  CMapIStringLong(bool uniekeNamen = true) :CNamenMap(uniekeNamen) {};
  ~CMapIStringLong() {};

  bool Lookup(const char* key, long& rValue) const       {return CNamenMap::Lookup(key,(void*&)rValue);}
  bool LookupKey(const char* key, CString& rKey) const   {return CNamenMap::LookupKey(key,rKey);}
  void SetAt(const CString& key, long newValue)     {CNamenMap::SetAt(key,(void*)newValue);}
 	int  SetPos(const CString& key, int pos)          {return CNamenMap::SetPos(key,pos);}
  bool RemoveKey(const CString& key)                {return CNamenMap::RemoveKey(key);}
  void RemoveAll()                                  {CNamenMap::RemoveAll();}

  bool IsEmpty() const                              {return CNamenMap::IsEmpty();}
  int GetCount() const                              {return CNamenMap::GetCount();}
  int Aantal() const                                {return CNamenMap::GetCount();}
  iterator GetStartPosition() const                 {return CNamenMap::GetStartPosition();}
	void GetNextAssoc(iterator& rNextPosition, CString& rKey, long& rValue) const
            {CNamenMap::GetNextAssoc(rNextPosition,rKey,(void*&)rValue);}
};
//************************************************************************************************
// CLASS CMapIString
//************************************************************************************************
template <class T>
class CMapIString : private CNamenMap
{
public:

  using CNamenMap::iterator;

  CMapIString(bool eigenaar = false,bool uniekeNamen = true);
  ~CMapIString();

  bool Lookup(const char* key, T*& rValue) const;
  bool LookupKey(const char* key, CString& rKey) const;
  void SetAt(const CString& key, T* newValue);
 	int  SetPos(const CString& key, int pos);
  bool RemoveKey(const CString& key);
	void RemoveAll();

  bool IsEmpty() const;
	int GetCount() const;
	int Aantal() const;
  iterator GetStartPosition() const;
	void GetNextAssoc(iterator& rNextPosition, CString& rKey, T*& rValue) const;

private:
  bool m_eigenaar;

};

template <class T>
CMapIString<T>::CMapIString(bool eigenaar,bool uniekeNamen)
               :CNamenMap(uniekeNamen)
{
  m_eigenaar = eigenaar;
}

template <class T>
CMapIString<T>::~CMapIString()
{
  RemoveAll();
}

template <class T>
bool CMapIString<T>::Lookup(const char* key, T*& rValue) const
{
  return CNamenMap::Lookup(key,(void*&)rValue);
}

template <class T>
bool CMapIString<T>::LookupKey(const char* key, CString& rKey) const
{
  return CNamenMap::LookupKey(key,rKey);
}

template <class T>
void CMapIString<T>::SetAt(const CString& key, T* newValue)
{
  if(m_eigenaar)
  {
    T* val;
    if (CNamenMap::Lookup(key,(void*&)val))
      delete val;
  }
  CNamenMap::SetAt(key,(void*)newValue);
}

template <class T>
int CMapIString<T>::SetPos(const CString& key, int pos)
{
  return CNamenMap::SetPos(key,pos);
}

template <class T>
bool CMapIString<T>::RemoveKey(const CString& key)
{
  if(m_eigenaar)
  {
    T* val;
    if (CNamenMap::Lookup(key,(void*&)val))
      delete val;
  }
  return CNamenMap::RemoveKey(key);
}

template <class T>
void CMapIString<T>::RemoveAll()
{
  if(m_eigenaar)
  {
    iterator pos = GetStartPosition();
    while(pos)
    {
      CString key;
      T* val;
    	CNamenMap::GetNextAssoc(pos,key,(void*&)val);
      delete val;
    }
  }
  CNamenMap::RemoveAll();
}

template <class T>
bool CMapIString<T>::IsEmpty() const
{
  return CNamenMap::IsEmpty();
}

template <class T>
int CMapIString<T>::GetCount() const
{
  return CNamenMap::GetCount();
}

template <class T>
int CMapIString<T>::Aantal() const
{
  return CNamenMap::GetCount();
}

template <class T>
typename CMapIString<T>::iterator 
CMapIString<T>::GetStartPosition() const
{
  return CNamenMap::GetStartPosition();
}

template <class T>
void CMapIString<T>::GetNextAssoc(iterator& rNextPosition, CString& rKey, T*& rValue) const
{
  CNamenMap::GetNextAssoc(rNextPosition,rKey,(void*&)rValue);
}


//************************************************************************************************
// CLASS CMapIIterator
//************************************************************************************************
template<class T>
class CMapIIterator
{
public:
  CMapIIterator(const CMapIString<T>& list);

	bool operator++();

  T* Huidige();
  T* HuidigElement();
  CString& HuidigeKey();
  bool Volgende();
  bool EindeLijst();

private:
  const CMapIString<T>& m_list;
  typedef typename CMapIString<T>::iterator iterator;
  
  T*         m_huidige;
  CString    m_key;
  iterator   m_pos;
  bool       m_einde;
};


template<class T>
CMapIIterator<T>::CMapIIterator(const CMapIString<T>& p_list)
:m_list(p_list),
 m_pos(p_list.GetStartPosition()),
 m_huidige(NULL),
 m_einde(false)
{
  Volgende();
}

template<class T>
bool CMapIIterator<T>::operator++()
{
  return Volgende();
}

template<class T>
T* CMapIIterator<T>::Huidige()
{
  return m_huidige;
}

template<class T>
T* CMapIIterator<T>::HuidigElement()
{
  return m_huidige;
}

template<class T>
CString& CMapIIterator<T>::HuidigeKey()
{
  return m_key;
}


template<class T>
bool CMapIIterator<T>::Volgende()
{
  if(m_pos)
    m_list.GetNextAssoc(m_pos,m_key,m_huidige);
  else
    m_einde = true;

  return m_einde;
}

template<class T>
bool CMapIIterator<T>::EindeLijst()
{
  return m_einde;
}
