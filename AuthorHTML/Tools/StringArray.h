// StringArray
//
#pragma once
#include "StdAfx.h"
#include <vector>

class StringArray  : public std::vector<CString>
{
public:

  typedef StringArray              this_type;
  typedef std::vector<CString>      base_type;


// Attributes
	int GetSize() const
  {
    return base_type::size();
  }
	int GetUpperBound() const
  {
    return base_type::size() - 1;

  }
	void SetSize(int nNewSize)//, int nGrowBy = -1)
  {
    if(nNewSize >= (int)base_type::size())
    {
      base_type::resize(nNewSize + 1);
    }
  }

// Operations
	// Clean up
//	void FreeExtra();
	void RemoveAll()
  {
    base_type::clear();
  }

	// Accessing elements
	CString GetAt(int nIndex) const
  {
    return base_type::at(nIndex);
  }
	void SetAt(int nIndex, const char* newElement)
  {
    base_type::at(nIndex) = CString(newElement);
  }

	void SetAt(int nIndex, const CString& newElement)
  {
    base_type::at(nIndex) = newElement;
  }

	CString& ElementAt(int nIndex)
  {
    return base_type::at(nIndex);
  }


	// Potentially growing the array
	void SetAtGrow(int nIndex, const char* newElement)
  {
    if(nIndex >= (int)base_type::size())
      base_type::resize(nIndex + 1);
    base_type::at(nIndex) = CString(newElement);

  }

	void SetAtGrow(int nIndex, const CString& newElement)
  {
    if(nIndex >= (int)base_type::size())
      base_type::resize(nIndex + 1);
    base_type::at(nIndex) = newElement;

  }

	int Add(const char* newElement)
  {
    base_type::push_back(CString(newElement));
  }

	void Add(const CString& newElement)
  {
    base_type::push_back(newElement);
  }

// Wordt NIET gebruikt:
// int Append(const StringArray& src);
//	void Copy(const StringArray& src);

	// overloaded operator helpers
	CString operator[](int nIndex) const
  {
    return base_type::operator[](nIndex);
  }
	CString& operator[](int nIndex)
  {
    return base_type::operator[](nIndex);
  }
// Wordt NIET gebruikt:
//	// Operations that move elements around
//	void InsertAt(int nIndex, LPCTSTR newElement, int nCount = 1);
//
//	void InsertAt(int nIndex, const CString& newElement, int nCount = 1);
//
//	void RemoveAt(int nIndex, int nCount = 1);
//	void InsertAt(int nStartIndex, StringArray* pNewArray);

};

