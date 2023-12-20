#include <initializer_list>
#include <stdint.h>
#include <memory>
#include <stdexcept>

template <typename T>
class Vector
{
private:
    T *arr;
    std::size_t size;
    std::size_t capacity;
    void Copy();
public:
    Vector() : capacity(0), size(0){};
    Vector(const Vector &Object);
    Vector(std::size_t size);
    Vector(std::size_t size, T variable);
    Vector(std::initializer_list<T> list);
    T &operator[](std::size_t i){ return arr[i];}
    T &operator=(const Vector &Object);
    T & at(std::size_t i);
    void Clear(); 
    void Erase (std::size_t i); // реализовать 
    void Insert (std::size_t i, T &value ); // реализовать
    void Swap(); // реализовать 
    const T &operator[](std::size_t i) const { return arr[i];}
    std::size_t Size() const { return size; }
    std::size_t Capacity() const { return capacity; }
    bool Empty() { return size == 0; }
    void ShrinkToFit() { capacity = size; }
    void Reserve(std::size_t capacity);
    void Resize(std::size_t size, const T& value = T());
    void PopBack();
    void PushBack(T &value);
    ~Vector();
};

template <typename T>
Vector<T>::Vector(std::size_t size) : size(size), capacity(2 * size)
{
    arr = new T[capacity];
}

template <typename T>
void Vector<T>::Insert(std::size_t i, ){
    if (i){

    }

}

template <typename T>
Vector<T>::Vector(const Vector & object){
    this->arr = reinterpret_cast<T *>(new int8_t[object.capacity * sizeof(T)]);
    try
    {
        std::uninitialized_copy(object.arr, object.arr + size, this->arr);
    }
    catch (...)
    {
        delete[] reinterpret_cast<int8_t *>(arr);
        throw;
    }
    this->capacity = object.capacity;
    this->size = object.size;
}

template <typename T>
T & Vector<T>::operator=(const Vector &Object){
    if (this == &Object)
        return *this;
    for (std::size_t i = 0; i < size; i++)
    {
        (arr + i)->~T(); // plecement new 
    }
    delete[] reinterpret_cast<int8_t *>(arr); // чтобы не вызывывать деструкторы
    this->arr = reinterpret_cast<T *>(new int8_t[Object.capacity * sizeof(T)]);
    try
    {
        std::uninitialized_copy(Object.arr, Object.arr + size, this->arr);
    }
    catch (...)
    {
        delete[] reinterpret_cast<int8_t *>(arr);
        throw;
    }
    this->capacity = Object.capacity;
    this->size = Object.size;
}

template <typename T>
Vector<T>::Vector(std::size_t size, T variable) : size(size), capacity(2 * size)
{
    arr = new T[capacity];
    for (std::size_t i = 0; i < size; ++i)
    {
        this->arr[i] = variable[i];
    }
}

template <typename T>
T& Vector<T>::at(std::size_t i){
    if (i > size)
        throw std::out_of_range("...");
    return arr[i];
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> list) : size(list.size()), capacity(2 * list.size())
{
    arr = new T[capacity];
    std::size_t i = 0;
    for (const T &element : list)
    {
        arr[i++] = element;
    }
}


template <typename T>
Vector<T>::~Vector(){
    delete[] arr;
    arr = nullptr;
}

template <typename T>
void Vector<T>::Resize(std::size_t size, const T& value = T())
{
    if (size = this->size) // we don't do anything
        return; 
    if (size > capacity){
        Reserve(size);
    }
    for (std::size_t i = this->size; i < size; ++i){
        arr[i]= value;
    }
    this->size = size;
}

template <typename T>
void Vector<T>:: Reserve(std::size_t capacity){
    if (capacity <= size)
        return;
    T* newarr = reinterpret_cast<T *>(new int8_t [capacity * sizeof(T)]); // типа выделяет память через int и переврдить её в тип T и он будет не сконструированн реально
    try{
    std::uninitialized_copy(arr, arr + size, newarr);
    }
    catch(...){
        delete[] reinterpret_cast<int8_t *>(newarr);
        throw;
    }
    delete[] reinterpret_cast<int8_t *>(arr); // чтобы не вызывывать деструкторы
    arr = newarr;
    this->capacity =capacity; 
}

template <typename T>
void Vector<T>::PushBack(T &value){
    if (size = 0) Resize (1);
    if (capacity == size) Reserve(2 * size);
    new (arr + size) T (value);
    size++;
}

template <typename T>
void Vector<T>::PopBack()
{
    if (size = 0)
       throw std::out_of_range("...");
    (arr - 1)->~T();
    --size;
}

template <typename T>
void Vector<T>::Clear(){
    if (size == 0)
        return;
    for (std::size_t i = 0; i < size; i++){
        (arr + i)->~T();
    }
    delete[] reinterpret_cast<int8_t *>(arr);
    this->size = 0;
    this->capacity = 1;
    arr = reinterpret_cast<T *>(new int8_t [capacity * sizeof(T)]);
}