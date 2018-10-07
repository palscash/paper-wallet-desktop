// Copyright (c) 2014-2017 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "base58.h"

#include <assert.h>
#include <string.h>

/** All alphanumeric characters except for "0", "I", "O", and "l" */
static const char* pszBase58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

namespace palscash {

	namespace base58 {
		bool decode_base(const char* psz, std::vector<unsigned char>& vch) {
			// Skip leading spaces.
			while (*psz && isspace(*psz))
				psz++;
			// Skip and count leading '1's.
			int zeroes = 0;
			int length = 0;
			while (*psz == '1') {
				zeroes++;
				psz++;
			}
			// Allocate enough space in big-endian base256 representation.
			int size = strlen(psz) * 733 / 1000 + 1; // log(58) / log(256), rounded up.
			std::vector<unsigned char> b256(size);
			// Process the characters.
			while (*psz && !isspace(*psz)) {
				// Decode base58 character
				const char* ch = strchr(pszBase58, *psz);
				if (ch == nullptr)
					return false;
				// Apply "b256 = b256 * 58 + ch".
				int carry = ch - pszBase58;
				int i = 0;
				for (std::vector<unsigned char>::reverse_iterator it = b256.rbegin(); (carry != 0 || i < length) && (it != b256.rend()); ++it, ++i) {
					carry += 58 * (*it);
					*it = carry % 256;
					carry /= 256;
				}
				assert(carry == 0);
				length = i;
				psz++;
			}
			// Skip trailing spaces.
			while (isspace(*psz))
				psz++;
			if (*psz != 0)
				return false;
			// Skip leading zeroes in b256.
			std::vector<unsigned char>::iterator it = b256.begin() + (size - length);
			while (it != b256.end() && *it == 0)
				it++;
			// Copy result into output vector.
			vch.reserve(zeroes + (b256.end() - it));
			vch.assign(zeroes, 0x00);
			while (it != b256.end())
				vch.push_back(*(it++));
			return true;
		}

		std::string encode_base(const unsigned char* pbegin, const unsigned char* pend) {
			// Skip & count leading zeroes.
			int zeroes = 0;
			int length = 0;
			while (pbegin != pend && *pbegin == 0) {
				pbegin++;
				zeroes++;
			}
			// Allocate enough space in big-endian base58 representation.
			int size = (pend - pbegin) * 138 / 100 + 1; // log(256) / log(58), rounded up.
			std::vector<unsigned char> b58(size);
			// Process the bytes.
			while (pbegin != pend) {
				int carry = *pbegin;
				int i = 0;
				// Apply "b58 = b58 * 256 + ch".
				for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); (carry != 0 || i < length) && (it != b58.rend()); it++, i++) {
					carry += 256 * (*it);
					*it = carry % 58;
					carry /= 58;
				}

				assert(carry == 0);
				length = i;
				pbegin++;
			}
			// Skip leading zeroes in base58 result.
			std::vector<unsigned char>::iterator it = b58.begin() + (size - length);
			while (it != b58.end() && *it == 0)
				it++;
			// Translate the result into a string.
			std::string str;
			str.reserve(zeroes + (b58.end() - it));
			str.assign(zeroes, '1');
			while (it != b58.end())
				str += pszBase58[*(it++)];
			return str;
		}

		std::string encode_base(const std::vector<unsigned char>& vch) {
			return encode_base(vch.data(), vch.data() + vch.size());
		}

		std::vector<unsigned char> decode_base(const std::string& str) {

			std::vector<unsigned char> vchRet;

			decode_base(str.c_str(), vchRet);

			return vchRet;
		}

	}
}
