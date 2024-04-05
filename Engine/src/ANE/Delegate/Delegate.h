// MIT License
//
// Copyright (c) 2022 David Lafreniere
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// https://github.com/endurodave/AsyncMulticastDelegateModern

#pragma once

class DelegateBase {
public:
    DelegateBase() = default;
    virtual ~DelegateBase() noexcept = default;

    /// Derived class must implement operator== to compare objects.
    virtual bool operator==(const DelegateBase& rhs) const = 0;
    virtual bool operator!=(const DelegateBase& rhs) const { return !(*this == rhs); }

    /// Use Clone to provide a deep copy using a base pointer. Covariant
    /// overloading is used so that a Clone() method return type is a
    /// more specific type in the derived class implementations.
    /// @return A dynamic copy of this instance created with operator new.
    /// @post The caller is responsible for deleting the clone instance.
    virtual DelegateBase* Clone() const = 0;
};

template <class R>
struct Delegate; // Not defined

template <class RetType, class... Args>
class Delegate<RetType(Args...)> : public DelegateBase {
public:
    virtual RetType operator()(Args... args) = 0;
    virtual Delegate* Clone() const = 0;
};

template <class R>
struct DelegateFree; // Not defined

template <class RetType, class... Args>
class DelegateFree<RetType(Args...)> : public Delegate<RetType(Args...)> {
public:
    typedef RetType(*FreeFunc)(Args...);
    using ClassType = DelegateFree<RetType(Args...)>;

    DelegateFree(FreeFunc func) { Bind(func); }
    DelegateFree() = delete;

    /// Bind a free function to the delegate.
    void Bind(FreeFunc func) { m_func = func; }

    virtual DelegateFree* Clone() const override { return new DelegateFree(*this); }

    /// Invoke the bound delegate function.
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, args...);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func;
    }

    bool Empty() const { return !m_func; }
    void Clear() { m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    FreeFunc m_func = nullptr;		// Pointer to a free function
};

template <class C, class R>
struct DelegateMember; // Not defined

template <class TClass, class RetType, class... Args>
class DelegateMember<TClass, RetType(Args...)> : public Delegate<RetType(Args...)>
{
public:
    typedef TClass* ObjectPtr;
    typedef RetType(TClass::*MemberFunc)(Args...);
    typedef RetType(TClass::*ConstMemberFunc)(Args...) const;
    using ClassType = DelegateMember<TClass, RetType(Args...)>;

    DelegateMember(ObjectPtr object, MemberFunc func) { Bind(object, func); }
    DelegateMember(ObjectPtr object, ConstMemberFunc func) { Bind(object, func); }
    DelegateMember() = delete;

    /// Bind a member function to a delegate.
    void Bind(ObjectPtr object, MemberFunc func) {
        m_object = object;
        m_func = func;
    }

    /// Bind a const member function to a delegate.
    void Bind(ObjectPtr object, ConstMemberFunc func) {
        m_object = object;
        m_func = reinterpret_cast<MemberFunc>(func);
    }

    virtual DelegateMember* Clone() const override { return new DelegateMember(*this); }

    // Invoke the bound delegate function
    virtual RetType operator()(Args... args) override {
        return std::invoke(m_func, m_object, args...);
    }

    virtual bool operator==(const DelegateBase& rhs) const override {
        auto derivedRhs = dynamic_cast<const ClassType*>(&rhs);
        return derivedRhs &&
            m_func == derivedRhs->m_func &&
            m_object == derivedRhs->m_object;
    }

    bool Empty() const { return !(m_object && m_func); }
    void Clear() { m_object = nullptr; m_func = nullptr; }

    explicit operator bool() const { return !Empty(); }

private:
    ObjectPtr m_object = nullptr;	// Pointer to a class object
    MemberFunc m_func = nullptr;   	// Pointer to an instance member function
};

template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args)) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

template <class TClass, class RetType, class... Args>
DelegateMember<TClass, RetType(Args...)> MakeDelegate(TClass* object, RetType(TClass::*func)(Args... args) const) {
    return DelegateMember<TClass, RetType(Args...)>(object, func);
}

template <class RetType, class... Args>
DelegateFree<RetType(Args...)> MakeDelegate(RetType(*func)(Args... args)) {
    return DelegateFree<RetType(Args...)>(func);
}

//Singlecast delegate

template <class R>
struct SinglecastDelegate; // Not defined

/// @brief A non-thread safe delegate container storing one delegate. Void and
/// non-void return values supported.
template<class RetType, class... Args>
class SinglecastDelegate<RetType(Args...)>
{
public:
    SinglecastDelegate() = default;
    ~SinglecastDelegate() { Clear(); }

    RetType operator()(Args... args) {
        return (*m_delegate)(args...);	// Invoke delegate callback
    }

    void operator=(const Delegate<RetType(Args...)>& delegate) {
        Clear();
        m_delegate = delegate.Clone();	// Create a duplicate delegate
    }

    void operator=(const Delegate<RetType(Args...)>* delegate) {
        Clear();
        if (delegate)
            m_delegate = delegate->Clone();  // Create a duplicate delegate
    }

    /// Any registered delegates?
    bool Empty() const { return !m_delegate; }

    /// Remove registered delegate
    void Clear() {
        if (m_delegate) {
            delete m_delegate;
            m_delegate = nullptr;
        }
    }

    explicit operator bool() const { return !Empty(); }

private:
    // Prevent copying objects
    SinglecastDelegate(const SinglecastDelegate&) = delete;
    SinglecastDelegate& operator=(const SinglecastDelegate&) = delete;

    /// Registered delegate.
    Delegate<RetType(Args...)>* m_delegate = nullptr;
};

// Multicast Delegates

template <class R>
struct MulticastDelegate; // Not defined

/// @brief Not thread-safe multicast delegate container class. The class has a linked
/// list of Delegate<> instances. When invoked, each Delegate instance within the invocation
/// list is called. MulticastDelegate<> does not support return values. A void return
/// must always be used.
template<class RetType, class... Args>
class MulticastDelegate<RetType(Args...)>
{
public:
    MulticastDelegate() = default;
    ~MulticastDelegate() { Clear(); }

    RetType operator()(Args... args) {
        for (Delegate<RetType(Args...)>* delegate : m_delegates)
            (*delegate)(args...);	// Invoke delegate callback
    }

    void operator+=(const Delegate<RetType(Args...)>& delegate) {
        m_delegates.push_back(delegate.Clone());
    }
    void operator-=(const Delegate<RetType(Args...)>& delegate) {
        for (auto it = m_delegates.begin(); it != m_delegates.end(); ++it)
        {
            if (*((DelegateBase*)&delegate) == *((DelegateBase*)(*it)))
            {
                delete (*it);
                m_delegates.erase(it);
                break;
            }
        }
    }

    /// Any registered delegates?
    bool Empty() const { return m_delegates.empty(); }

    /// Removal all registered delegates.
    void Clear() {
        auto it = m_delegates.begin();
        while (it != m_delegates.end())
        {
            delete (*it);
            it = m_delegates.erase(it);
        }
    }

    explicit operator bool() const { return !Empty(); }

private:
    // Prevent copying objects
    MulticastDelegate(const MulticastDelegate&) = delete;
    MulticastDelegate& operator=(const MulticastDelegate&) = delete;

    /// List of registered delegates
    std::list<Delegate<RetType(Args...)>*> m_delegates;
};
