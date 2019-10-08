// Copyright (c) 2019 Katrina Swales
// Copyright (c) 2019 Daniel Krawisz
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"

#include <data/types.hpp>
#include <abstractions/crypto/address.hpp>
#include <abstractions/pattern.hpp>
#include <abstractions/wallet/machine.hpp>
#include <abstractions/work/work.hpp>
#include <abstractions/script/pow.hpp>
#include "gtest/gtest.h"
#include "testData.h"

class AddressTest : public testing::TestWithParam<test_data> {};

TEST_P(AddressTest, TestAddresses) {
    using namespace abstractions;
    
    bitcoin::secret key{"0x000000000000000100000000000000000000000100010000000000000000000f"};
    bitcoin::pubkey pub = key.to_public();
    bitcoin::signature not_real{};
    
    std::string message_1{"abcd"};
    std::string message_2{"1234"};
    
    sha256::digest reference_1 = crypto::hash256(message_1);
    sha256::digest reference_2 = crypto::hash256(message_2);
    
    // difficulty is only 1 / 32 so it is possible that the test will fail because
    // one nonce is good for pow with both messages. If so, increase difficulty.
    const work::target difficulty{31, 0xffffff};
    
    work::order to_do_1 = {work::reference_and_pubkey(reference_1, pub), difficulty};
    work::order to_do_2 = {work::reference_and_pubkey(reference_2, pub), difficulty};
    
    int64 nonce_1 = work::work(to_do_1);
    int64 nonce_2 = work::work(to_do_2);
    
    script::pow_lock pow_lock_1{script::lock_with_pow(reference_1, difficulty)->compile()};
    script::pow_lock pow_lock_2{script::lock_with_pow(reference_2, difficulty)->compile()};
    
    script::pow_key pow_key_1{script::unlock_with_pow(not_real, pub, nonce_1)->compile()};
    script::pow_key pow_key_2{script::unlock_with_pow(not_real, pub, nonce_2)->compile()};
    
    work::candidate candidate_1 = to_do_1.candidate(nonce_1);
    work::candidate candidate_2 = to_do_2.candidate(nonce_2);
    
    EXPECT_TRUE(work::hash(candidate_1) < difficulty.expand());
    EXPECT_TRUE(work::hash(candidate_2) < difficulty.expand());
    
    EXPECT_FALSE(work::hash(to_do_1.candidate(nonce_2)) < difficulty.expand());
    EXPECT_FALSE(work::hash(to_do_2.candidate(nonce_1)) < difficulty.expand());
    
    EXPECT_TRUE(candidate_1 == script::unlock(pow_lock_1, pow_key_1));
    EXPECT_TRUE(candidate_2 == script::unlock(pow_lock_2, pow_key_2));
    
    EXPECT_FALSE(candidate_1 == script::unlock(pow_lock_2, pow_key_2));
    EXPECT_FALSE(candidate_2 == script::unlock(pow_lock_1, pow_key_1));
    
    EXPECT_FALSE(candidate_1 == script::unlock(pow_lock_1, pow_key_2));
    EXPECT_FALSE(candidate_2 == script::unlock(pow_lock_2, pow_key_1));
    
    EXPECT_FALSE(candidate_1 == script::unlock(pow_lock_2, pow_key_1));
    EXPECT_FALSE(candidate_2 == script::unlock(pow_lock_1, pow_key_2));
    
    std::cout << "nonce 1 is " << nonce_1 << std::endl;
    std::cout << "nonce 2 is " << nonce_2 << std::endl;
    
}

#pragma clang diagnostic pop
