// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef ABSTRACTIONS_SHA512
#define ABSTRACTIONS_SHA512

#include <data/crypto/sha512.hpp>
#include <abstractions/abstractions.hpp>

namespace abstractions {
    
    namespace sha512 {
        using digest = data::sha512::digest;
        
        inline digest hash(const bytes& b) {
            return data::sha512::hash(b);
        }
        
        template <N n>
        inline digest hash(const std::array<byte, n>& b) {
            return data::sha512::hash(b);
        }
    }

}

#endif
