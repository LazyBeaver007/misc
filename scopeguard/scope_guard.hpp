



#include <utility>
#include <system_error>
#include <type_traits>
template<typename F>
class ScopeGuard
{
    public:
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
    return ScopeGuard<std::decay_t<F>>(std::forward(f));
}
