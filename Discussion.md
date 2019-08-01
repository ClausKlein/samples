This section contains follow-up material on rules and sets of rules.
In particular, here we present further rationale, longer examples, and discussions of alternatives.

### <a name="Sd-order"></a>Discussion: Define and initialize member variables in the order of member declaration

Member variables are always initialized in the order they are declared in the class definition, so write them in that order in the constructor initialization list. Writing them in a different order just makes the code confusing because it won't run in the order you see, and that can make it hard to see order-dependent bugs.

    class Employee {
        string email, first, last;
    public:
        Employee(const char* firstName, const char* lastName);
        // ...
    };

    Employee::Employee(const char* firstName, const char* lastName)
      : first(firstName),
        last(lastName),
        // BAD: first and last not yet constructed
        email(first + "." + last + "@acme.com")
    {}

In this example, `email` will be constructed before `first` and `last` because it is declared first. That means its constructor will attempt to use `first` and `last` too soon -- not just before they are set to the desired values, but before they are constructed at all.

If the class definition and the constructor body are in separate files, the long-distance influence that the order of member variable declarations has over the constructor's correctness will be even harder to spot.

**References**:

[\[Cline99\]](#Cline99) §22.03-11, [\[Dewhurst03\]](#Dewhurst03) §52-53, [\[Koenig97\]](#Koenig97) §4, [\[Lakos96\]](#Lakos96) §10.3.5, [\[Meyers97\]](#Meyers97) §13, [\[Murray93\]](#Murray93) §2.1.3, [\[Sutter00\]](#Sutter00) §47

### <a name="Sd-init"></a>Discussion: Use of `=`, `{}`, and `()` as initializers

???

### <a name="Sd-factory"></a>Discussion: Use a factory function if you need "virtual behavior" during initialization

If your design wants virtual dispatch into a derived class from a base class constructor or destructor for functions like `f` and `g`, you need other techniques, such as a post-constructor -- a separate member function the caller must invoke to complete initialization, which can safely call `f` and `g` because in member functions virtual calls behave normally. Some techniques for this are shown in the References. Here's a non-exhaustive list of options:

* *Pass the buck:* Just document that user code must call the post-initialization function right after constructing an object.
* *Post-initialize lazily:* Do it during the first call of a member function. A Boolean flag in the base class tells whether or not post-construction has taken place yet.
* *Use virtual base class semantics:* Language rules dictate that the constructor most-derived class decides which base constructor will be invoked; you can use that to your advantage. (See [\[Taligent94\]](#Taligent94).)
* *Use a factory function:* This way, you can easily force a mandatory invocation of a post-constructor function.

Here is an example of the last option:

    class B {
    public:
        B() { /* ... */ f(); /* ... */ }   // BAD: see Item 49.1

        virtual void f() = 0;

        // ...
    };

```
{{factory_function.cpp}}
```

This design requires the following discipline:

* Derived classes such as `D` must not expose a public constructor. Otherwise, `D`'s users could create `D` objects that don't invoke `PostInitialize`.
* Allocation is limited to `operator new`. `B` can, however, override `new` (see Items 45 and 46).
* `D` must define a constructor with the same parameters that `B` selected. Defining several overloads of `Create` can assuage this problem, however; and the overloads can even be templated on the argument types.

If the requirements above are met, the design guarantees that `PostInitialize` has been called for any fully constructed `B`-derived object. `PostInitialize` doesn't need to be virtual; it can, however, invoke virtual functions freely.

In summary, no post-construction technique is perfect. The worst techniques dodge the whole issue by simply asking the caller to invoke the post-constructor manually. Even the best require a different syntax for constructing objects (easy to check at compile time) and/or cooperation from derived class authors (impossible to check at compile time).

**References**: [\[Alexandrescu01\]](#Alexandrescu01) §3, [\[Boost\]](#Boost), [\[Dewhurst03\]](#Dewhurst03) §75, [\[Meyers97\]](#Meyers97) §46, [\[Stroustrup00\]](#Stroustrup00) §15.4.3, [\[Taligent94\]](#Taligent94)

### <a name="Sd-dtor"></a>Discussion: Make base class destructors public and virtual, or protected and nonvirtual

Should destruction behave virtually? That is, should destruction through a pointer to a `base` class be allowed? If yes, then `base`'s destructor must be public in order to be callable, and virtual otherwise calling it results in undefined behavior. Otherwise, it should be protected so that only derived classes can invoke it in their own destructors, and nonvirtual since it doesn't need to behave virtually.

##### Example

The common case for a base class is that it's intended to have publicly derived classes, and so calling code is just about sure to use something like a `shared_ptr<base>`:

    class Base {
    public:
        ~Base();                   // BAD, not virtual
        virtual ~Base();           // GOOD
        // ...
    };

    class Derived : public Base { /* ... */ };

    {
        unique_ptr<Base> pb = make_unique<Derived>();
        // ...
    } // ~pb invokes correct destructor only when ~Base is virtual

In rarer cases, such as policy classes, the class is used as a base class for convenience, not for polymorphic behavior. It is recommended to make those destructors protected and nonvirtual:

    class My_policy {
    public:
        virtual ~My_policy();      // BAD, public and virtual
    protected:
        ~My_policy();              // GOOD
        // ...
    };

    template<class Policy>
    class customizable : Policy { /* ... */ }; // note: private inheritance

##### Note

This simple guideline illustrates a subtle issue and reflects modern uses of inheritance and object-oriented design principles.

For a base class `Base`, calling code might try to destroy derived objects through pointers to `Base`, such as when using a `unique_ptr<Base>`. If `Base`'s destructor is public and nonvirtual (the default), it can be accidentally called on a pointer that actually points to a derived object, in which case the behavior of the attempted deletion is undefined. This state of affairs has led older coding standards to impose a blanket requirement that all base class destructors must be virtual. This is overkill (even if it is the common case); instead, the rule should be to make base class destructors virtual if and only if they are public.

To write a base class is to define an abstraction (see Items 35 through 37). Recall that for each member function participating in that abstraction, you need to decide:

* Whether it should behave virtually or not.
* Whether it should be publicly available to all callers using a pointer to `Base` or else be a hidden internal implementation detail.

As described in Item 39, for a normal member function, the choice is between allowing it to be called via a pointer to `Base` nonvirtually (but possibly with virtual behavior if it invokes virtual functions, such as in the NVI or Template Method patterns), virtually, or not at all. The NVI pattern is a technique to avoid public virtual functions.

Destruction can be viewed as just another operation, albeit with special semantics that make nonvirtual calls dangerous or wrong. For a base class destructor, therefore, the choice is between allowing it to be called via a pointer to `Base` virtually or not at all; "nonvirtually" is not an option. Hence, a base class destructor is virtual if it can be called (i.e., is public), and nonvirtual otherwise.

Note that the NVI pattern cannot be applied to the destructor because constructors and destructors cannot make deep virtual calls. (See Items 39 and 55.)

Corollary: When writing a base class, always write a destructor explicitly, because the implicitly generated one is public and nonvirtual. You can always `=default` the implementation if the default body is fine and you're just writing the function to give it the proper visibility and virtuality.

##### Exception

Some component architectures (e.g., COM and CORBA) don't use a standard deletion mechanism, and foster different protocols for object disposal. Follow the local patterns and idioms, and adapt this guideline as appropriate.

Consider also this rare case:

* `B` is both a base class and a concrete class that can be instantiated by itself, and so the destructor must be public for `B` objects to be created and destroyed.
* Yet `B` also has no virtual functions and is not meant to be used polymorphically, and so although the destructor is public it does not need to be virtual.

Then, even though the destructor has to be public, there can be great pressure to not make it virtual because as the first virtual function it would incur all the run-time type overhead when the added functionality should never be needed.

In this rare case, you could make the destructor public and nonvirtual but clearly document that further-derived objects must not be used polymorphically as `B`'s. This is what was done with `std::unary_function`.

In general, however, avoid concrete base classes (see Item 35). For example, `unary_function` is a bundle-of-typedefs that was never intended to be instantiated standalone. It really makes no sense to give it a public destructor; a better design would be to follow this Item's advice and give it a protected nonvirtual destructor.

**References**: [\[C++CS\]](#CplusplusCS) Item 50, [\[Cargill92\]](#Cargill92) pp. 77-79, 207, [\[Cline99\]](#Cline99) §21.06, 21.12-13, [\[Henricson97\]](#Henricson97) pp. 110-114, [\[Koenig97\]](#Koenig97) Chapters 4, 11, [\[Meyers97\]](#Meyers97) §14, [\[Stroustrup00\]](#Stroustrup00) §12.4.2, [\[Sutter02\]](#Sutter02) §27, [\[Sutter04\]](#Sutter04) §18

### <a name="Sd-noexcept"></a>Discussion: Usage of noexcept

???

### <a name="Sd-never-fail"></a>Discussion: Destructors, deallocation, and swap must never fail

Never allow an error to be reported from a destructor, a resource deallocation function (e.g., `operator delete`), or a `swap` function using `throw`. It is nearly impossible to write useful code if these operations can fail, and even if something does go wrong it nearly never makes any sense to retry. Specifically, types whose destructors may throw an exception are flatly forbidden from use with the C++ Standard Library. Most destructors are now implicitly `noexcept` by default.

##### Example

    class Nefarious {
    public:
        Nefarious()  { /* code that could throw */ }   // ok
        ~Nefarious() { /* code that could throw */ }   // BAD, should not throw
        // ...
    };

1. `Nefarious` objects are hard to use safely even as local variables:


        void test(string& s)
        {
            Nefarious n;          // trouble brewing
            string copy = s;      // copy the string
        } // destroy copy and then n

    Here, copying `s` could throw, and if that throws and if `n`'s destructor then also throws, the program will exit via `std::terminate` because two exceptions can't be propagated simultaneously.

2. Classes with `Nefarious` members or bases are also hard to use safely, because their destructors must invoke `Nefarious`' destructor, and are similarly poisoned by its poor behavior:


        class Innocent_bystander {
            Nefarious member;     // oops, poisons the enclosing class's destructor
            // ...
        };

        void test(string& s)
        {
            Innocent_bystander i; // more trouble brewing
            string copy2 = s;      // copy the string
        } // destroy copy and then i

    Here, if constructing `copy2` throws, we have the same problem because `i`'s destructor now also can throw, and if so we'll invoke `std::terminate`.

3. You can't reliably create global or static `Nefarious` objects either:


        static Nefarious n;       // oops, any destructor exception can't be caught

4. You can't reliably create arrays of `Nefarious`:


        void test()
        {
            std::array<Nefarious, 10> arr; // this line can std::terminate(!)
        }

    The behavior of arrays is undefined in the presence of destructors that throw because there is no reasonable rollback behavior that could ever be devised. Just think: What code can the compiler generate for constructing an `arr` where, if the fourth object's constructor throws, the code has to give up and in its cleanup mode tries to call the destructors of the already-constructed objects ... and one or more of those destructors throws? There is no satisfactory answer.

5. You can't use `Nefarious` objects in standard containers:


        std::vector<Nefarious> vec(10);   // this line can std::terminate()

    The standard library forbids all destructors used with it from throwing. You can't store `Nefarious` objects in standard containers or use them with any other part of the standard library.

##### Note

These are key functions that must not fail because they are necessary for the two key operations in transactional programming: to back out work if problems are encountered during processing, and to commit work if no problems occur. If there's no way to safely back out using no-fail operations, then no-fail rollback is impossible to implement. If there's no way to safely commit state changes using a no-fail operation (notably, but not limited to, `swap`), then no-fail commit is impossible to implement.

Consider the following advice and requirements found in the C++ Standard:

> If a destructor called during stack unwinding exits with an exception, terminate is called (15.5.1). So destructors should generally catch exceptions and not let them propagate out of the destructor. --[\[C++03\]](#Cplusplus03) §15.2(3)
>
> No destructor operation defined in the C++ Standard Library (including the destructor of any type that is used to instantiate a standard-library template) will throw an exception. --[\[C++03\]](#Cplusplus03) §17.4.4.8(3)

Deallocation functions, including specifically overloaded `operator delete` and `operator delete[]`, fall into the same category, because they too are used during cleanup in general, and during exception handling in particular, to back out of partial work that needs to be undone.
Besides destructors and deallocation functions, common error-safety techniques rely also on `swap` operations never failing -- in this case, not because they are used to implement a guaranteed rollback, but because they are used to implement a guaranteed commit. For example, here is an idiomatic implementation of `operator=` for a type `T` that performs copy construction followed by a call to a no-fail `swap`:

    T& T::operator=(const T& other) {
        auto temp = other;
        swap(temp);
        return *this;
    }

(See also Item 56. ???)

Fortunately, when releasing a resource, the scope for failure is definitely smaller. If using exceptions as the error reporting mechanism, make sure such functions handle all exceptions and other errors that their internal processing might generate. (For exceptions, simply wrap everything sensitive that your destructor does in a `try/catch(...)` block.) This is particularly important because a destructor might be called in a crisis situation, such as failure to allocate a system resource (e.g., memory, files, locks, ports, windows, or other system objects).

When using exceptions as your error handling mechanism, always document this behavior by declaring these functions `noexcept`. (See Item 75.)

**References**: [\[C++CS\]](#CplusplusCS) Item 51; [\[C++03\]](#Cplusplus03) §15.2(3), §17.4.4.8(3), [\[Meyers96\]](#Meyers96) §11, [\[Stroustrup00\]](#Stroustrup00) §14.4.7, §E.2-4, [\[Sutter00\]](#Sutter00) §8, §16, [\[Sutter02\]](#Sutter02) §18-19

## <a name="Sd-consistent"></a>Define Copy, move, and destroy consistently

##### Reason

 ???

##### Note

If you define a copy constructor, you must also define a copy assignment operator.

##### Note

If you define a move constructor, you must also define a move assignment operator.

##### Example

    class X {
        // ...
    public:
        X(const X&) { /* stuff */ }

        // BAD: failed to also define a copy assignment operator

        X(x&&) noexcept { /* stuff */ }

        // BAD: failed to also define a move assignment operator
    };

    X x1;
    X x2 = x1; // ok
    x2 = x1;   // pitfall: either fails to compile, or does something suspicious

If you define a destructor, you should not use the compiler-generated copy or move operation; you probably need to define or suppress copy and/or move.

    class X {
        HANDLE hnd;
        // ...
    public:
        ~X() { /* custom stuff, such as closing hnd */ }
        // suspicious: no mention of copying or moving -- what happens to hnd?
    };

    X x1;
    X x2 = x1; // pitfall: either fails to compile, or does something suspicious
    x2 = x1;   // pitfall: either fails to compile, or does something suspicious

If you define copying, and any base or member has a type that defines a move operation, you should also define a move operation.

    class X {
        string s; // defines more efficient move operations
        // ... other data members ...
    public:
        X(const X&) { /* stuff */ }
        X& operator=(const X&) { /* stuff */ }

        // BAD: failed to also define a move construction and move assignment
        // (why wasn't the custom "stuff" repeated here?)
    };

    X test()
    {
        X local;
        // ...
        return local;  // pitfall: will be inefficient and/or do the wrong thing
    }

If you define any of the copy constructor, copy assignment operator, or destructor, you probably should define the others.

##### Note

If you need to define any of these five functions, it means you need it to do more than its default behavior -- and the five are asymmetrically interrelated. Here's how:

* If you write/disable either of the copy constructor or the copy assignment operator, you probably need to do the same for the other: If one does "special" work, probably so should the other because the two functions should have similar effects. (See Item 53, which expands on this point in isolation.)
* If you explicitly write the copying functions, you probably need to write the destructor: If the "special" work in the copy constructor is to allocate or duplicate some resource (e.g., memory, file, socket), you need to deallocate it in the destructor.
* If you explicitly write the destructor, you probably need to explicitly write or disable copying: If you have to write a non-trivial destructor, it's often because you need to manually release a resource that the object held. If so, it is likely that those resources require careful duplication, and then you need to pay attention to the way objects are copied and assigned, or disable copying completely.

In many cases, holding properly encapsulated resources using RAII "owning" objects can eliminate the need to write these operations yourself. (See Item 13.)

Prefer compiler-generated (including `=default`) special members; only these can be classified as "trivial", and at least one major standard library vendor heavily optimizes for classes having trivial special members. This is likely to become common practice.

**Exceptions**: When any of the special functions are declared only to make them nonpublic or virtual, but without special semantics, it doesn't imply that the others are needed.
In rare cases, classes that have members of strange types (such as reference members) are an exception because they have peculiar copy semantics.
In a class holding a reference, you likely need to write the copy constructor and the assignment operator, but the default destructor already does the right thing. (Note that using a reference member is almost always wrong.)

**References**: [\[C++CS\]](#CplusplusCS) Item 52; [\[Cline99\]](#Cline99) §30.01-14, [\[Koenig97\]](#Koenig97) §4, [\[Stroustrup00\]](#Stroustrup00) §5.5, §10.4, [\[SuttHysl04b\]](#SuttHysl04b)

Resource management rule summary:

* [Provide strong resource safety; that is, never leak anything that you think of as a resource](#Cr-safety)
* [Never throw while holding a resource not owned by a handle](#Cr-never)
* [A "raw" pointer or reference is never a resource handle](#Cr-raw)
* [Never let a pointer outlive the object it points to](#Cr-outlive)
* [Use templates to express containers (and other resource handles)](#Cr-templates)
* [Return containers by value (relying on move or copy elision for efficiency)](#Cr-value-return)
* [If a class is a resource handle, it needs a constructor, a destructor, and copy and/or move operations](#Cr-handle)
* [If a class is a container, give it an initializer-list constructor](#Cr-list)

### <a name="Cr-safety"></a>Discussion: Provide strong resource safety; that is, never leak anything that you think of as a resource

##### Reason

Prevent leaks. Leaks can lead to performance degradation, mysterious error, system crashes, and security violations.

**Alternative formulation**: Have every resource represented as an object of some class managing its lifetime.

##### Example

    template<class T>
    class Vector {
    // ...
    private:
        T* elem;   // sz elements on the free store, owned by the class object
        int sz;
    };

This class is a resource handle. It manages the lifetime of the `T`s. To do so, `Vector` must define or delete [the set of special operations](???) (constructors, a destructor, etc.).

##### Example

    ??? "odd" non-memory resource ???

##### Enforcement

The basic technique for preventing leaks is to have every resource owned by a resource handle with a suitable destructor. A checker can find "naked `new`s". Given a list of C-style allocation functions (e.g., `fopen()`), a checker can also find uses that are not managed by a resource handle. In general, "naked pointers" can be viewed with suspicion, flagged, and/or analyzed. A complete list of resources cannot be generated without human input (the definition of "a resource" is necessarily too general), but a tool can be "parameterized" with a resource list.

### <a name="Cr-never"></a>Discussion: Never throw while holding a resource not owned by a handle

##### Reason

That would be a leak.

##### Example

    void f(int i)
    {
        FILE* f = fopen("a file", "r");
        ifstream is { "another file" };
        // ...
        if (i == 0) return;
        // ...
        fclose(f);
    }

If `i == 0` the file handle for `a file` is leaked. On the other hand, the `ifstream` for `another file` will correctly close its file (upon destruction). If you must use an explicit pointer, rather than a resource handle with specific semantics, use a `unique_ptr` or a `shared_ptr` with a custom deleter:

    void f(int i)
    {
        unique_ptr<FILE, int(*)(FILE*)> f(fopen("a file", "r"), fclose);
        // ...
        if (i == 0) return;
        // ...
    }

Better:

    void f(int i)
    {
        ifstream input {"a file"};
        // ...
        if (i == 0) return;
        // ...
    }

##### Enforcement

A checker must consider all "naked pointers" suspicious.
A checker probably must rely on a human-provided list of resources.
For starters, we know about the standard-library containers, `string`, and smart pointers.
The use of `span` and `string_span` should help a lot (they are not resource handles).

### <a name="Cr-raw"></a>Discussion: A "raw" pointer or reference is never a resource handle

##### Reason

To be able to distinguish owners from views.

##### Note

This is independent of how you "spell" pointer: `T*`, `T&`, `Ptr<T>` and `Range<T>` are not owners.

### <a name="Cr-outlive"></a>Discussion: Never let a pointer outlive the object it points to

##### Reason

To avoid extremely hard-to-find errors. Dereferencing such a pointer is undefined behavior and could lead to violations of the type system.

##### Example

    string* bad()   // really bad
    {
        vector<string> v = { "This", "will", "cause", "trouble", "!" };
        // leaking a pointer into a destroyed member of a destroyed object (v)
        return &v[0];
    }

    void use()
    {
        string* p = bad();
        vector<int> xx = {7, 8, 9};
        // undefined behavior: x may not be the string "This"
        string x = *p;
        // undefined behavior: we don't know what (if anything) is allocated a location p
        *p = "Evil!";
    }

The `string`s of `v` are destroyed upon exit from `bad()` and so is `v` itself. The returned pointer points to unallocated memory on the free store. This memory (pointed into by `p`) may have been reallocated by the time `*p` is executed. There may be no `string` to read and a write through `p` could easily corrupt objects of unrelated types.

##### Enforcement

Most compilers already warn about simple cases and have the information to do more. Consider any pointer returned from a function suspect. Use containers, resource handles, and views (e.g., `span` known not to be resource handles) to lower the number of cases to be examined. For starters, consider every class with a destructor as resource handle.

### <a name="Cr-templates"></a>Discussion: Use templates to express containers (and other resource handles)

##### Reason

To provide statically type-safe manipulation of elements.

##### Example

    template<typename T> class Vector {
        // ...
        T* elem;   // point to sz elements of type T
        int sz;
    };

### <a name="Cr-value-return"></a>Discussion: Return containers by value (relying on move or copy elision for efficiency)

##### Reason

To simplify code and eliminate a need for explicit memory management. To bring an object into a surrounding scope, thereby extending its lifetime.

**See also**: [F.20, the general item about "out" output values](#Rf-out)

##### Example

    vector<int> get_large_vector()
    {
        return ...;
    }

    auto v = get_large_vector(); //  return by value is ok, most modern compilers will do copy elision

##### Exception

See the Exceptions in [F.20](#Rf-out).

##### Enforcement

Check for pointers and references returned from functions and see if they are assigned to resource handles (e.g., to a `unique_ptr`).

### <a name="Cr-handle"></a>Discussion: If a class is a resource handle, it needs a constructor, a destructor, and copy and/or move operations

##### Reason

To provide complete control of the lifetime of the resource. To provide a coherent set of operations on the resource.

##### Example

    ??? Messing with pointers

##### Note

If all members are resource handles, rely on the default special operations where possible.

    template<typename T> struct Named {
        string name;
        T value;
    };

Now `Named` has a default constructor, a destructor, and efficient copy and move operations, provided `T` has.

##### Enforcement

In general, a tool cannot know if a class is a resource handle. However, if a class has some of [the default operations](#SS-ctor), it should have all, and if a class has a member that is a resource handle, it should be considered as resource handle.

### <a name="Cr-list"></a>Discussion: If a class is a container, give it an initializer-list constructor

##### Reason

It is common to need an initial set of elements.

##### Example

    template<typename T> class Vector {
    public:
        Vector(std::initializer_list<T>);
        // ...
    };

    Vector<string> vs { "Nygaard", "Ritchie" };

##### Enforcement

When is a class a container? ???

