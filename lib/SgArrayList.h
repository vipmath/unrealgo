//----------------------------------------------------------------------------
/** @file SgArrayList.h
    Static list. */
//----------------------------------------------------------------------------

#ifndef SG_ARRAYLIST_H
#define SG_ARRAYLIST_H

#include <algorithm>

//----------------------------------------------------------------------------

/** Static list not using dynamic memory allocation.
    Elements need to have a default constructor.
    They should be value-types, not entity-types, because operations like
    Clear() do not call the destructor of the old elements immediately. */
template<typename T, int SIZE>
class GoArrayList {
 public:
  /** Const iterator */
  class Iterator {
   public:
    Iterator(const GoArrayList &list);
    const T &operator*() const;
    void operator++();
    operator bool() const;

   private:
    const T *m_end;
    const T *m_current;
  };
  /** Non-const iterator */
  class NonConstIterator {
   public:
    NonConstIterator(GoArrayList &list);
    T &operator*() const;
    void operator++();
    operator bool() const;

   private:
    const T *m_end;
    T *m_current;
  };
  GoArrayList();
  /** Construct list with one element. */
  explicit GoArrayList(const T &value);
  GoArrayList(const GoArrayList<T, SIZE> &list);
  GoArrayList &operator=(const GoArrayList &list);
  bool operator==(const GoArrayList &list) const;
  bool operator!=(const GoArrayList &list) const;
  T &operator[](int index);
  const T &operator[](int index) const;
  void Clear();
  bool Contains(const T &value) const;
  /** Remove first occurrence of a value.
      Like RemoveFirst, but more efficient and does not preserve
      order of remaining elements. The first occurrence of the value is
      replaced by the last element.
      @return false, if element was not found */
  bool Exclude(const T &value);
  /** PushBack value at the end of the list if it's not already in the
      list. Returns true iff value was added */
  bool Include(const T &value);
  /** Build intersection with other list.
      List may not contain duplicate entries. */
  GoArrayList Intersect(const GoArrayList<T, SIZE> &list) const;
  bool IsEmpty() const;
  T &Last();
  const T &Last() const;
  int Length() const;
  /** Remove the last element of the list.
      Does not return the last element for efficiency. To get the last
      element, use Last() before calling PopBack(). */
  void PopBack();
  void PushBack(const T &value);
  /** Push back all elements of another list.
      Works with lists of different maximum sizes.
      Requires: Total resulting number of elements will fit into the target
      list. */
  template<int SIZE2>
  void PushBackList(const GoArrayList<T, SIZE2> &list);
  /** Remove first occurence of a value.
      Preserves order of remaining elements.
      @see Exclude */
  void RemoveFirst(const T &value);
  /** Resize list.
      If new length is greater than current length, then the elements
      at a place greater than the old length are not initialized, they are
      just the old elements at this place.
      This is necessary if elements are re-used for efficiency and will be
      initialized later. */
  void Resize(int length);
  bool SameElements(const GoArrayList &list) const;
  void SetTo(const T &value);
  void Sort();

 private:
  friend class Iterator;
  friend class NonConstIterator;
  int m_len;
  T m_array[SIZE];
};

//----------------------------------------------------------------------------

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::Iterator::Iterator(const GoArrayList &list)
    : m_end(list.m_array + list.Length()),
      m_current(list.m_array) {}

template<typename T, int SIZE>
inline const T &GoArrayList<T, SIZE>::Iterator::operator*() const {
  DBG_ASSERT(*this);
  return *m_current;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::Iterator::operator++() {
  ++m_current;
}

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::Iterator::operator bool() const {
  return m_current < m_end;
}

template<typename T, int SIZE>
inline
GoArrayList<T, SIZE>::NonConstIterator::NonConstIterator(GoArrayList &list)
    : m_end(list.m_array + list.Length()),
      m_current(list.m_array) {}

template<typename T, int SIZE>
inline T &GoArrayList<T, SIZE>::NonConstIterator::operator*() const {
  DBG_ASSERT(*this);
  return *m_current;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::NonConstIterator::operator++() {
  ++m_current;
}

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::NonConstIterator::operator bool() const {
  return m_current < m_end;
}

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::GoArrayList()
    : m_len(0) {}

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::GoArrayList(const T &value) {
  SetTo(value);
  m_len = 1;
  m_array[0] = value;
}

template<typename T, int SIZE>
inline GoArrayList<T, SIZE>::GoArrayList(const GoArrayList<T, SIZE> &list) {
  *this = list;
}

template<typename T, int SIZE>
GoArrayList<T, SIZE> &GoArrayList<T, SIZE>::operator=(const GoArrayList &list) {
  m_len = list.m_len;
  T *p = m_array;
  const T *pp = list.m_array;
  for (int i = m_len; i--; ++p, ++pp)
    *p = *pp;
  return *this;
}

template<typename T, int SIZE>
bool GoArrayList<T, SIZE>::operator==(const GoArrayList &list) const {
  if (m_len != list.m_len)
    return false;
  const T *p = m_array;
  const T *pp = list.m_array;
  for (int i = m_len; i--; ++p, ++pp)
    if (*p != *pp)
      return false;
  return true;
}

template<typename T, int SIZE>
inline bool GoArrayList<T, SIZE>::operator!=(const GoArrayList &list) const {
  return !this->operator==(list);
}

template<typename T, int SIZE>
inline T &GoArrayList<T, SIZE>::operator[](int index) {
  DBG_ASSERT(index >= 0);
  DBG_ASSERT(index < m_len);
  return m_array[index];
}

template<typename T, int SIZE>
inline const T &GoArrayList<T, SIZE>::operator[](int index) const {
  DBG_ASSERT(index >= 0);
  DBG_ASSERT(index < m_len);
  return m_array[index];
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::Clear() {
  m_len = 0;
}

template<typename T, int SIZE>
bool GoArrayList<T, SIZE>::Contains(const T &value) const {
  int i;
  const T *t = m_array;
  for (i = m_len; i--; ++t)
    if (*t == value)
      return true;
  return false;
}

template<typename T, int SIZE>
bool GoArrayList<T, SIZE>::Exclude(const T &value) {
  // Go backwards through list, because with game playing programs
  // it is more likely that a recently added element is removed first
  T *t = m_array + m_len - 1;
  for (int i = m_len; i--; --t)
    if (*t == value) {
      --m_len;
      if (m_len > 0)
        *t = *(m_array + m_len);
      return true;
    }
  return false;
}

template<typename T, int SIZE>
bool GoArrayList<T, SIZE>::Include(const T &value) {
  if (Contains(value))
    return false;
  PushBack(value);
  return true;
}

template<typename T, int SIZE>
GoArrayList<T, SIZE>
GoArrayList<T, SIZE>::Intersect(const GoArrayList<T, SIZE> &list) const {
  GoArrayList<T, SIZE> result;
  const T *t = m_array;
  for (int i = m_len; i--; ++t)
    if (list.Contains(*t)) {
      DBG_ASSERT(!result.Contains(*t));
      result.PushBack(*t);
    }
  return result;
}

template<typename T, int SIZE>
inline bool GoArrayList<T, SIZE>::IsEmpty() const {
  return m_len == 0;
}

template<typename T, int SIZE>
inline T &GoArrayList<T, SIZE>::Last() {
  DBG_ASSERT(m_len > 0);
  return m_array[m_len - 1];
}

template<typename T, int SIZE>
inline const T &GoArrayList<T, SIZE>::Last() const {
  DBG_ASSERT(m_len > 0);
  return m_array[m_len - 1];
}

template<typename T, int SIZE>
inline int GoArrayList<T, SIZE>::Length() const {
  return m_len;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::PopBack() {
  DBG_ASSERT(m_len > 0);
  --m_len;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::PushBack(const T &value) {
  DBG_ASSERT(m_len < SIZE);
  m_array[m_len++] = value;
}

template<typename T, int SIZE>
template<int SIZE2>
inline void GoArrayList<T, SIZE>::PushBackList(const GoArrayList<T, SIZE2> &list) {
  for (typename GoArrayList<T, SIZE2>::Iterator it(list); it; ++it)
    PushBack(*it);
}

template<typename T, int SIZE>
void GoArrayList<T, SIZE>::RemoveFirst(const T &value) {
  int i;
  T *t = m_array;
  for (i = m_len; i--; ++t)
    if (*t == value) {
      for (; i--; ++t)
        *t = *(t + 1);
      --m_len;
      break;
    }
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::Resize(int length) {
  DBG_ASSERT(length >= 0);
  DBG_ASSERT(length <= SIZE);
  m_len = length;
}

template<typename T, int SIZE>
bool GoArrayList<T, SIZE>::SameElements(const GoArrayList &list) const {
  if (m_len != list.m_len)
    return false;
  const T *p = m_array;
  for (int i = m_len; i--; ++p)
    if (!list.Contains(*p))
      return false;
  return true;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::SetTo(const T &value) {
  m_len = 1;
  m_array[0] = value;
}

template<typename T, int SIZE>
inline void GoArrayList<T, SIZE>::Sort() {
  std::sort(m_array, m_array + m_len);
}

//----------------------------------------------------------------------------

#endif // SG_ARRAYLIST_H