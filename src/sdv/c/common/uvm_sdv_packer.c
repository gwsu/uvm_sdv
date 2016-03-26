
#include "uvm_sdv_packer.h"

#define UVM_SDV_PACK_ENSURE_SPACE(msg, sz) \
	if (((msg->msg_data_max)-(msg->msg_len)) < sz) { \
		uvm_sdv_ensure_space(msg, sz); \
	}

void uvm_sdv_ensure_space(
		uvm_sdv_transport_msg_t		*msg,
		uint32_t					sz)
{
	if ((msg->msg_data_max-msg->msg_len) < sz) {
		// Resize
		uint32_t *tmp = msg->msg_data;
		msg->msg_data = (uint32_t *)malloc(
				sizeof(uint32_t)*(msg->msg_data_max+sz+16));
		msg->msg_data_max += (sz+16);
		if (tmp) {
			memcpy(msg->msg_data, tmp, sizeof(uint32_t)*msg->msg_data_max);
			free(tmp);
		}
	}
}

void uvm_sdv_packer_init(
		uvm_sdv_packer_t			*packer,
		uvm_sdv_transport_msg_t		*msg)
{
	packer->msg = msg;
	packer->idx = 0;
}

#ifdef UNDEFINED
void uvm_sdv_pack_int(
		uvm_sdv_packer_t			*packer,
		uint64_t					val,
		uint32_t					bits)
{
	uvm_sdv_transport_msg_t *msg = packer->msg;
	if (bits <= 32) {
		// pack 32-bits
		UVM_SDV_PACK_ENSURE_SPACE(msg, 1);
		msg->msg_data[msg->msg_len++] = val;
	} else {
		// pack 64-bits
		UVM_SDV_PACK_ENSURE_SPACE(msg, 2);
		msg->msg_data[msg->msg_len++] = val;
		val >>= 32;
		msg->msg_data[msg->msg_len++] = val;
	}
}
#endif

void uvm_sdv_pack_object(
		uvm_sdv_packer_t			*packer,
		uvm_sdv_pack_func			pack_func,
		void						*obj)
{
	if (obj) {
		// non-NULL handle
		uvm_sdv_pack_int(packer, 1, 32);
		pack_func(packer, obj);
	} else {
		// NULL handle
		uvm_sdv_pack_int(packer, 0, 32);
	}
}

void uvm_sdv_pack_string(
		uvm_sdv_packer_t			*packer,
		const char					*str)
{
	uvm_sdv_transport_msg_t *msg = packer->msg;
	uint32_t tmp = 0;
	uint32_t i = 0;
	const char *c = str;
	uint32_t len_idx = msg->msg_len;
	uint32_t len = 0;

	// Number of words required to hold the string
	UVM_SDV_PACK_ENSURE_SPACE(msg, 1);
	msg->msg_data[msg->msg_len++] = 0;

	do {
		tmp |= (*c << 8*i);
		i++;

		if (!(i % 4)) {
			// completed a word
			UVM_SDV_PACK_ENSURE_SPACE(msg, 1);
			msg->msg_data[msg->msg_len++] = tmp;
			i = 0;
			tmp = 0;
			len++;
		}
	} while (*(c++));

	// Finish packing the last word
	if (i > 0) {
		UVM_SDV_PACK_ENSURE_SPACE(msg, 1);
		msg->msg_data[msg->msg_len++] = tmp;
		len++;
	}
	msg->msg_data[len_idx] = len;
}


uint64_t uvm_sdv_unpack_int(
		uvm_sdv_packer_t			*packer,
		uint32_t					bits)
{
	uint64_t ret, tmp;
	uvm_sdv_transport_msg_t *msg = packer->msg;

	if (bits <= 32) {
		ret = msg->msg_data[packer->idx++];
	} else {
		ret = msg->msg_data[packer->idx++];
		tmp = msg->msg_data[packer->idx++];
		tmp <<= 32;
		ret |= tmp;
	}

	return ret;
}

int uvm_sdv_unpack_isnull(
		uvm_sdv_packer_t				*packer)
{
	return (packer->msg->msg_data[packer->idx] != 0);
}

uint32_t uvm_sdv_unpack_string(
		uvm_sdv_packer_t				*packer,
		char							*str,
		uint32_t						len)
{
	uvm_sdv_transport_msg_t *msg = packer->msg;
	uint32_t ret = 0;
	uint32_t i;

	if (str) {
		// Consume the length
		ret = msg->msg_data[packer->idx++];

		ret *= 4;

		for (i=0; i<ret && i<len; i+=4) {
			uint32_t v = msg->msg_data[packer->idx++];

			*(str++) = v;
			*(str++) = (v >> 8);
			*(str++) = (v >> 16);
			*(str++) = (v >> 24);
		}
	} else {
		// Just return the length;
		ret = msg->msg_data[packer->idx];
	}

	return (4*ret);
}


