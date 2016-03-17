#include "uvm_sdv_memblk_transport.h"
#include "uvm_sdv_config.h"

typedef struct fifo_s {
	uint32_t			*write_ptr;
	uint32_t			*read_ptr;
	uint32_t			sz;
	uint32_t			*data_ptr;
} fifo_t;

typedef struct memblk_transport_s {
	fifo_t						sw2hvl_msg_queue;
	fifo_t						hvl2sw_msg_queue;

	uvm_sdv_transport_msg_t		*msg_alloc;
} memblk_transport_t;

static uvm_sdv_transport_msg_t *memblk_transport_recv_msg(
		uvm_sdv_transport_t				*tp,
		uint32_t						block)
{
	memblk_transport_t *tp_prv = (memblk_transport_t *)tp->prv_data;
	uvm_sdv_transport_msg_t *msg = tp->alloc_msg(tp, 0);
	volatile fifo_t *hvl2sw_msg_queue = &tp_prv->hvl2sw_msg_queue;
	uint32_t rp, wp, sz, tmp;
	uint32_t msg_idx = 0;

	// TODO: mutex

	// Wait until we can read at least two words
	rp = *hvl2sw_msg_queue->read_ptr;
	while (1) {
		wp = *(hvl2sw_msg_queue->write_ptr);
		sz = (wp >= rp)?(wp-rp):(hvl2sw_msg_queue->sz-(rp-wp));

		if (sz >= 2 || !block) {
			// Have at least two words
			break;
		}
	}

	// Non-blocking, no data
	if (sz < 2) {
		return 0;
	}

	tmp = hvl2sw_msg_queue->data_ptr[rp];
	rp = ((rp + 1) % hvl2sw_msg_queue->sz);
	msg->endpoint_id = (tmp >> 16);
	msg->msg_type = (tmp & 0xFFFF);

	tmp = hvl2sw_msg_queue->data_ptr[rp];
	rp = ((rp + 1) % hvl2sw_msg_queue->sz);
	msg->msg_len = tmp;

	if (msg->msg_data_max < msg->msg_len) {
		// Re-alloc the buffer
		if (msg->msg_data) {
			free(msg->msg_data);
		}
		msg->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg->msg_len);
	}

	// Write-back the read pointer so the software
	// can see it
	*(hvl2sw_msg_queue->read_ptr) = rp;

	while (msg_idx < msg->msg_len) {
		sz = (wp >= rp)?(wp-rp):(hvl2sw_msg_queue->sz-(rp-wp));

		if (sz > 0) {
			msg->msg_data[msg_idx++] = hvl2sw_msg_queue->data_ptr[rp];
			rp = ((rp + 1) % hvl2sw_msg_queue->sz);
			sz--;
			// If we're about to stall, then write-back the
			// read pointer
			if (sz == 0 && msg_idx < msg->msg_len) {
				*(hvl2sw_msg_queue->read_ptr) = rp;
			}
		} else {
			wp = *(hvl2sw_msg_queue->write_ptr);
		}
	}

	// Commit the final read pointer
	*(hvl2sw_msg_queue->read_ptr) = rp;

	// TODO: mutex

	return msg;
}

static int memblk_transport_send_msg(
		uvm_sdv_transport_t				*tp,
		uvm_sdv_transport_msg_t			*msg)
{
	memblk_transport_t *tp_prv = (memblk_transport_t *)tp->prv_data;
	volatile fifo_t *sw2hvl_msg_queue = &tp_prv->sw2hvl_msg_queue;
	uint32_t rp, wp, sz, tmp;
	uint32_t msg_idx = 0;

	// TODO: mutex

	// Wait until we can write at least two words
	wp = *sw2hvl_msg_queue->write_ptr;
	while (1) {
		rp = *(sw2hvl_msg_queue->read_ptr);
		sz = (wp >= rp)?(sw2hvl_msg_queue->sz-(wp-rp)):(rp-wp);

		if (sz >= 2) {
			// Have at least two words
			break;
		}
	}

	tmp = (msg->endpoint_id << 16 | msg->msg_type);
	sw2hvl_msg_queue->data_ptr[wp] = tmp;
	wp = ((wp + 1) % sw2hvl_msg_queue->sz);

	sw2hvl_msg_queue->data_ptr[wp] = msg->msg_len;
	wp = ((wp + 1) % sw2hvl_msg_queue->sz);

	// Write-back the write pointer so the software
	// can see it
	*(sw2hvl_msg_queue->write_ptr) = wp;

	while (msg_idx < msg->msg_len) {
		sz = (wp >= rp)?(sw2hvl_msg_queue->sz-(wp-rp)):(rp-wp);

		if (sz > 0) {
			sw2hvl_msg_queue->data_ptr[wp] =  msg->msg_data[msg_idx++];
			wp = ((wp + 1) % sw2hvl_msg_queue->sz);
			sz--;
			// If we're about to stall, then write-back the
			// write pointer
			if (sz == 0 && msg_idx < msg->msg_len) {
				*(sw2hvl_msg_queue->write_ptr) = wp;
			}
		} else {
			rp = *(sw2hvl_msg_queue->read_ptr);
		}
	}

	// Commit the final write pointer
	*(sw2hvl_msg_queue->write_ptr) = wp;

	// TODO: mutex

	return 0;
}

static uvm_sdv_transport_msg_t *memblk_transport_alloc_msg(
		uvm_sdv_transport_t				*tp,
		uint32_t						msg_len)
{
	memblk_transport_t *tp_prv = (memblk_transport_t *)tp->prv_data;
	uvm_sdv_transport_msg_t *ret;

	// TODO: mutex
	if (tp_prv->msg_alloc) {
		ret = tp_prv->msg_alloc;
		tp_prv->msg_alloc = ret->next;

		if (ret->msg_data_max < msg_len) {
			free(ret->msg_data);
			ret->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg_len);
			ret->msg_data_max = msg_len;
		}
	} else {
		// Allocate a new message
		ret = (uvm_sdv_transport_msg_t *)malloc(sizeof(uvm_sdv_transport_msg_t));
		if (msg_len > 0) {
			ret->msg_data = (uint32_t *)malloc(sizeof(uint32_t)*msg_len);
			ret->msg_data_max = msg_len;
		} else {
			ret->msg_data = 0;
			ret->msg_data_max = 0;
		}
	}

	// Initialize fields
	ret->msg_len = 0;
	ret->next = 0;

	// TODO: mutex

	return ret;
}

static void memblk_transport_free_msg(
		uvm_sdv_transport_t				*tp,
		uvm_sdv_transport_msg_t			*msg)
{
	memblk_transport_t *tp_prv = (memblk_transport_t *)tp->prv_data;
	// TODO: mutex
	msg->next = tp_prv->msg_alloc;
	tp_prv->msg_alloc = msg;
	// TODO: mutex
}

uvm_sdv_transport_t *uvm_sdv_memblk_transport_create(
		uint32_t			*memblk,
		uint32_t			memblk_sz)
{
	uvm_sdv_transport_t *tp = (uvm_sdv_transport_t *)malloc(sizeof(uvm_sdv_transport_t));
	memblk_transport_t *tp_prv = (memblk_transport_t *)malloc(sizeof(memblk_transport_t));

	tp->prv_data = tp_prv;
	tp_prv->msg_alloc = 0;

	tp_prv->sw2hvl_msg_queue.write_ptr  = &memblk[1];
	tp_prv->sw2hvl_msg_queue.read_ptr   = &memblk[2];
	tp_prv->hvl2sw_msg_queue.write_ptr  = &memblk[3];
	tp_prv->hvl2sw_msg_queue.read_ptr   = &memblk[4];

	tp_prv->sw2hvl_msg_queue.sz = (memblk_sz-5)/2;
	tp_prv->hvl2sw_msg_queue.sz = (memblk_sz-5)/2;

	tp_prv->sw2hvl_msg_queue.data_ptr = &memblk[5];
	tp_prv->hvl2sw_msg_queue.data_ptr = &memblk[5+tp_prv->sw2hvl_msg_queue.sz];

	tp->recv_msg = &memblk_transport_recv_msg;
	tp->send_msg = &memblk_transport_send_msg;
	tp->alloc_msg = &memblk_transport_alloc_msg;
	tp->free_msg = &memblk_transport_free_msg;

	return tp;
}
