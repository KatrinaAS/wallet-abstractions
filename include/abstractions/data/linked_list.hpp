#ifndef ABSTRACTIONS_DATA_LINKED_LIST_HPP
#define ABSTRACTIONS_DATA_LINKED_LIST_HPP

#include <abstractions/data/list.hpp>

namespace abstractions
{
    
    namespace data
    {
        
        // Now we say that a list containing elements of
        // type X is also a pointer to something of type Y. 
        template <typename X>
        struct linked_list {
            using next = pointer<list::node<X, linked_list>>;
            
            next Next;
            
            bool empty() const {
                return Next == nullptr;
            }
            
            const X& first() const {
                return Next->First;
            }
            
            const linked_list rest() const {
                if (empty()) return nullptr;
                
                return Next->Rest;
            }
            
            const list::node<X, linked_list> operator*() {
                return *Next;
            }
            
            next operator->() const {
                return Next;
            }
            
            linked_list<X> append(X x) const {
                return linked_list<X>(std::make_shared<list::node<X, linked_list>>(list::node<X, linked_list>{x, *this}));
            }
                
            linked_list<X> operator+(X x) const {
                return append(x);
            }
                
            bool contains(X x) const {
                if (empty()) return false;
                
                Next->contains(x);
            }
            
            bool operator==(const next n) {
                return Next == n;
            }
            
            bool operator==(const linked_list<X>& l) {
                if (this == &l) return true;
                return Next == l.Next;
            }
            
            linked_list(pointer<list::node<X, linked_list>> n) : Next() {}
            linked_list() : Next{nullptr} {}
            
            list::iterator<linked_list<X>, X> begin() {
                return {*this};
            }
            
            list::iterator<linked_list<X>, X> end() {
                return {};
            }
            
        };
        
    }

    template <typename X>
    inline bool empty(const data::linked_list<X> l) {
        return l.empty();
    }

    template <typename X>
    inline const X& first(const data::linked_list<X> l) {
        return l.first();
    }

    template <typename X>
    inline const data::linked_list<X> rest(const data::linked_list<X> l) {
        return l.rest();
    }

    template <typename X>
    inline const data::linked_list<X> append(const data::linked_list<X> l, const X elem) {
        return l.append(elem);
    }

}

#endif

