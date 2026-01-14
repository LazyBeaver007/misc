



#include <utility>
#include <system_error>
#include <type_traits>
template<typename F> 
//template that will execute a callable object F when it goes out of scope
class ScopeGuard
{
    public:
    //constructor
    //explicit prevents implicit conversions
    //rvalue is a temporary value that does not have persistent memory adress
    //lvalues refer to mem location, rvalue ref to data value (eg: int&& rref = 20)
    //rvalues enables move constructor, allowing temp objects to transfer resource instead of copying them
        explicit ScopeGuard(F&& f) noexcept(std::is_nothrow_copy_constructible_v<F>)
            : func_(std::forward<F>(f)), active_(true){}
        ScopeGuard(const ScopeGuard&)= delete;
        ScopeGuard& operator=(const ScopeGuard&)=delete;

        ScopeGuard(ScopeGuard&& other) noexcept(std::is_nothrow_copy_constructible_v<F>)
            :func_(std::move(other.func_)),active_(other.active_)
        {
            other.active_=false;
        }

        ScopeGuard& operator=(ScopeGuard& other) noexcept(std::is_nothrow_copy_constructible_v<F>)
        {
            if(this!=&other)
            {
                if(active_)
                {
                    try{func_();} catch(...){}

                }

                func_ = std::move(other.func_);
                active_ = other.active_;
                other.active_ = false;
            }

            return *this;
        }

        ~ScopeGuard() noexcept
        {
            if(active_)
            {
                try{func_();}catch(...){}
            }
        }
        void dismiss() noexcept{active_=false;}

    private:
        F func_;
        bool active_{false};

};


//deduce template param
template<typename F>
auto make_scope_guard(F&& f) noexcept
{
    return ScopeGuard<std::decay_t<F>>(std::forward<F>(f));
}


//func_(std::forward<F>(f))  
// // If f was rvalue, forward as rvalue; if lvalue, forward as lvalue
//
//noexcpet(..) is a function specifier that decalres if a func can throw excpetions
//if condition inside is true it wont throw excpetion


//noexcept(std::is_nothrow_copy_constructible_v<F>)
//“Declare this function as noexcept if and only if type F’s copy constructor is noexcept
