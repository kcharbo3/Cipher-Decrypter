// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.

template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
    int numItems;
    double arraySize;
    double loadFactor;
    
    struct Node
    {
        KeyType key;
        ValueType value;
        
        Node* next = nullptr;
    };
    
    Node** table;
    
    void clearBucket(Node*& head);
    int calcBucket(KeyType key) const;
    void delArray(int size);
    void addNode(Node*& head, KeyType key, ValueType value);
    void doubleArray();

};


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::clearBucket(Node*& n)
{
    if(n == nullptr)
        return;
    else
    {
        clearBucket(n->next);
        delete n;
    }
}

template <typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::calcBucket(KeyType key) const
{
    unsigned int hash(const	KeyType& k);
    unsigned int hashedKey = hash(key);
    return hashedKey % int(arraySize);
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::delArray(int size)
{
    for(int i = 0; i < size; i++)
    {
        clearBucket(table[i]);
        table[i] = nullptr;
    }
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::addNode(Node*& head, KeyType key, ValueType value)
{
    if(head == nullptr)
    {
        head = new Node;
        head->key = key;
        head->value = value;
        head->next = nullptr;
    }
    else
    {
        Node* ptr = head;
        while(ptr->next != nullptr)
        {
            ptr = ptr->next;
        }
        Node* n = new Node;
        n->key = key;
        n->value = value;
        n->next = nullptr;
        ptr->next = n;
    }
}


template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::doubleArray()
{
    Node** newArray = new Node*[int(arraySize) * 2];
    for(int i = 0; i < int(arraySize) * 2; i++)
        newArray[i] = nullptr;
    int oldArraySize = arraySize;
    arraySize *= 2;
    for(int i = 0; i < oldArraySize; i++)
    {
        Node* temp = table[i];
        while(temp != nullptr)
        {
            int bucket = calcBucket(temp->key);
            addNode(newArray[bucket], temp->key, temp->value);
            temp = temp->next;
        }
    }
    delArray(oldArraySize);
    delete [] table;
    table = newArray;
}

template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
: numItems(0), loadFactor(maxLoadFactor), arraySize(100.0)
{
    if(loadFactor <= 0)
        loadFactor = 0.5;
    if(loadFactor > 2.0)
        loadFactor = 2.0;
    table = new Node*[100];
    for(int i = 0; i < 100; i++)
        table[i] = nullptr;
}


template <typename KeyType, typename ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    delArray(arraySize);
    delete [] table;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    delArray(arraySize);
    delete [] table;
    table = new Node*[100];
    for(int i = 0; i < 100; i++)
        table[i] = nullptr;
}

template <typename KeyType, typename ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if(find(key) == nullptr)
    {
        numItems++;
        if( (numItems / arraySize) > loadFactor)
        {
            doubleArray();
        }
        int bucket = calcBucket(key);
        addNode(table[bucket], key, value);
    }
    else
    {
        ValueType* v = find(key);
        *v = value;
    }
}

template <typename KeyType, typename ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return numItems;
}

template <typename KeyType, typename ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return loadFactor;
}

template <typename KeyType, typename ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    ValueType* ans = nullptr;
    int bucket = calcBucket(key);
    Node* temp = table[bucket];
    while(temp != nullptr)
    {
        if(temp->key == key)
        {
            ans = &(temp->value);
            return ans;
        }
        temp = temp->next;
    }
    return ans;
}






