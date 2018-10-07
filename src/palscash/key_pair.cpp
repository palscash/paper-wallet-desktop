// Copyright (c) 2016-2018 The ZDP developers
// Copyright (c) 2018 PalsCash team
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

#include <openssl/rand.h>
#include <openssl/err.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/bn.h>
#include <openssl/ec.h>

#include "crypto_utils.h"
#include "key_pair.h"
#include "base58.h"

namespace palscash {

key_pair::key_pair() {

	auto buffer = palscash::crypto::random(SHA256_DIGEST_LENGTH);

	auto key = palscash::crypto::ec_new_keypair(buffer);

	// Private key in Base58
	{

		auto priv_bn = EC_KEY_get0_private_key(key);

		priv_byte_array.resize(BN_num_bytes(priv_bn));

		std::fill(priv_byte_array.begin(), priv_byte_array.end(), 0);

		BN_bn2bin(priv_bn, priv_byte_array.data());

		auto priv_base58 = palscash::base58::encode_base(priv_byte_array);

		this->private_key = priv_base58;

	}

	// Public key in Base58
	{

		auto pub_bn = BN_new();

		EC_POINT_point2bn(EC_KEY_get0_group(key), EC_KEY_get0_public_key(key), point_conversion_form_t::POINT_CONVERSION_COMPRESSED, pub_bn, nullptr);

		pub_byte_array.resize(BN_num_bytes(pub_bn));

		std::fill(pub_byte_array.begin(), pub_byte_array.end(), 0);

		BN_bn2bin(pub_bn, pub_byte_array.data());

		auto pub_base58 = palscash::base58::encode_base(pub_byte_array);

		this->public_key = std::string(pub_base58);

	}

	EC_KEY_free(key);
}

key_pair::key_pair(std::string priv_key) {

	this->private_key = priv_key;
	this->priv_byte_array = palscash::base58::decode_base(priv_key);

	this->public_key = palscash::crypto::get_public_key(priv_key);
	this->pub_byte_array = palscash::base58::decode_base(public_key);

}

key_pair::~key_pair() {
}

std::string const & key_pair::get_private_key() const {
	return private_key;
}

std::string const & key_pair::get_public_key() const {
	return public_key;
}

std::vector<unsigned char> const & key_pair::get_priv_byte_array() const {
	return priv_byte_array;
}

std::vector<unsigned char> const & key_pair::get_pub_byte_array() const {
	return pub_byte_array;
}

std::string key_pair::to_account_uuid() {

	auto pub_key_hash = palscash::crypto::hash_public_key(pub_byte_array);

	// Calculate CRC8 checksum of a public key
	auto crc = palscash::crypto::checksum(pub_key_hash);

	std::stringstream sstream;
	sstream << std::setfill('0') << std::setw(2) << std::hex << crc;
	auto crcHex = sstream.str();

	std::replace(crcHex.begin(), crcHex.end(), '0', 'x'); // replace all 'x' to 'y'

	std::stringstream output;
	output << "pca"; // prefix
	output << "x27"; // hardcoded ID of secp256k1 curve
	output << palscash::base58::encode_base(pub_key_hash);
	output << crcHex;

	return output.str();

}

} /* namespace palscash */

