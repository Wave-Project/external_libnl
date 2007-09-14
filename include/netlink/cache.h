/*
 * netlink/cache.h		Caching Module
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License as published by the Free Software Foundation version 2.1
 *	of the License.
 *
 * Copyright (c) 2003-2006 Thomas Graf <tgraf@suug.ch>
 */

#ifndef NETLINK_CACHE_H_
#define NETLINK_CACHE_H_

#include <netlink/netlink.h>
#include <netlink/msg.h>
#include <netlink/utils.h>
#include <netlink/object.h>

#ifdef __cplusplus
extern "C" {
#endif

struct nl_cache;
struct nl_cache_ops;

typedef void (*change_func_t)(struct nl_cache *, struct nl_object *, int);

/* Access Functions */
extern int			nl_cache_nitems(struct nl_cache *);
extern int			nl_cache_nitems_filter(struct nl_cache *,
						       struct nl_object *);
extern struct nl_cache_ops *	nl_cache_get_ops(struct nl_cache *);
extern struct nl_object *	nl_cache_get_first(struct nl_cache *);
extern struct nl_object *	nl_cache_get_last(struct nl_cache *);
extern struct nl_object *	nl_cache_get_next(struct nl_object *);
extern struct nl_object *	nl_cache_get_prev(struct nl_object *);

/* Cache creation/deletion */
#define nl_cache_alloc_from_ops(ptr)	nl_cache_alloc(ptr)
extern struct nl_cache *	nl_cache_alloc(struct nl_cache_ops *);
extern struct nl_cache *	nl_cache_alloc_name(const char *);
extern struct nl_cache *	nl_cache_subset(struct nl_cache *,
						struct nl_object *);
extern void			nl_cache_clear(struct nl_cache *);
extern void			nl_cache_free(struct nl_cache *);

/* Cache modification */
extern int			nl_cache_add(struct nl_cache *,
					     struct nl_object *);
extern int			nl_cache_parse_and_add(struct nl_cache *,
						       struct nl_msg *);
#define nl_cache_delete(a, b)	nl_cache_remove(b)
extern void			nl_cache_remove(struct nl_object *);
#define nl_cache_update(a, b)	nl_cache_refill(a, b)
extern int			nl_cache_refill(struct nl_handle *,
						struct nl_cache *);
extern int			nl_cache_pickup(struct nl_handle *,
						struct nl_cache *);
extern int			nl_cache_resync(struct nl_handle *,
						struct nl_cache *,
						change_func_t);
extern int			nl_cache_include(struct nl_cache *,
						 struct nl_object *,
						 change_func_t);

/* General */
extern int			nl_cache_is_empty(struct nl_cache *);
extern void			nl_cache_mark_all(struct nl_cache *);

/* Dumping */
extern void			nl_cache_dump(struct nl_cache *,
					      struct nl_dump_params *);
extern void			nl_cache_dump_filter(struct nl_cache *,
						     struct nl_dump_params *,
						     struct nl_object *);

/* Iterators */
extern void			nl_cache_foreach(struct nl_cache *,
						 void (*cb)(struct nl_object *,
							    void *),
						 void *arg);
extern void			nl_cache_foreach_filter(struct nl_cache *,
							struct nl_object *,
							void (*cb)(struct
								   nl_object *,
								   void *),
							void *arg);

/* --- cache management --- */

/* Cache type management */
extern struct nl_cache_ops *	nl_cache_ops_lookup(const char *);
extern struct nl_cache_ops *	nl_cache_ops_lookup_for_obj(struct nl_object_ops *);
extern void			nl_cache_mngt_foreach(void (*cb)(struct nl_cache_ops *, void *), void *);
extern int			nl_cache_mngt_register(struct nl_cache_ops *);
extern int			nl_cache_mngt_unregister(struct nl_cache_ops *);

/* Global cache provisioning/requiring */
extern void			nl_cache_mngt_provide(struct nl_cache *);
extern void			nl_cache_mngt_unprovide(struct nl_cache *);
extern struct nl_cache *	nl_cache_mngt_require(const char *);

struct nl_cache_mngr;

#define NL_AUTO_PROVIDE		1

extern struct nl_cache_mngr *	nl_cache_mngr_alloc(struct nl_handle *,
						    int, int);
extern struct nl_cache *	nl_cache_mngr_add(struct nl_cache_mngr *,
						  const char *,
						  change_func_t);
extern int			nl_cache_mngr_get_fd(struct nl_cache_mngr *);
extern int			nl_cache_mngr_poll(struct nl_cache_mngr *,
						   int);
extern int			nl_cache_mngr_data_ready(struct nl_cache_mngr *);
extern void			nl_cache_mngr_free(struct nl_cache_mngr *);

#ifdef __cplusplus
}
#endif

#endif
