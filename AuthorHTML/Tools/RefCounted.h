// RefCounted
//
#pragma once

template<typename T>
class Ref;

// Leid een klasse af van RefCounted om hem reference counted te maken.
class RefCounted
{
protected:
  template<typename T>
  friend class Ref;

  RefCounted()
    : m_referenties(0)
  {
  }
  void VoegReferentieToe()
  {
    m_referenties++;
  }
  void LaatReferentieLos();

  virtual ~RefCounted() {}
private:
  int m_referenties;
};


// Gebruik vervolgens Ref om naar de objecten van die klasse te verwijzen.
template<typename T>
class Ref
{
  T* m_obj;
public:
  explicit Ref(T* obj = 0)
    : m_obj(obj)
  {
    if (m_obj)
    {
      m_obj->VoegReferentieToe();
    }
  }
  
  Ref(const Ref& other)
    : m_obj(other.m_obj)
  {
    if (m_obj)
    {
      m_obj->VoegReferentieToe();
    }    
  }
  
  ~Ref()
  {
    if (m_obj)
    {
      m_obj->LaatReferentieLos();
    }
  }
  
  T* operator->() const
  {
    return m_obj;
  }
  
  T* GeefPointer() const
  {
    return m_obj;
  }
  
  template<typename T2>
    Ref<T2> Als()
  {
    return Ref<T2>(dynamic_cast<T2*>(m_obj));
  }

  Ref& operator=(const Ref& other)
  {    
    T* oldObj = m_obj;
    m_obj = other.m_obj;
    // Volgorde: EERST de nieuwe refereren, DAN de oude loslaten.
    // Dan werkt het namelijk ook als het dezelfde is. :)
    if (m_obj)
    {
      m_obj->VoegReferentieToe();
    }
    if (oldObj)
    {
      oldObj->LaatReferentieLos();
    }
    return *this;
  }

  operator bool() const
  {
    return (bool) (m_obj != 0);
  }

  int operator !() const
  {
    return (int) (m_obj == 0);
  }
};

inline void
RefCounted::LaatReferentieLos()
{
  if (m_referenties < 1)
  {
    TRACE("No reference to let go off\n");
  }
  m_referenties--;
  if (!m_referenties)
  {
    delete this;
  }
}
