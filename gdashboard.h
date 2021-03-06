/**
 * Copyright (C) 2009-2014 by Gerardo Orellana <goaccess@prosoftcorp.com>
 * GoAccess - An Ncurses apache weblog analyzer & interactive viewer
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * A copy of the GNU General Public License is attached to this
 * source distribution for its full text.
 *
 * Visit http://goaccess.prosoftcorp.com for new releases.
 */

#if HAVE_CONFIG_H
#include <config.h>
#endif

#ifndef GDASHBOARD_H_INCLUDED
#define GDASHBOARD_H_INCLUDED

#include "ui.h"
#include "parser.h"

/* *INDENT-OFF* */
#define DASH_HEAD_POS   0  /* header line pos           */
#define DASH_DESC_POS   1  /* description line pos      */
#define DASH_EMPTY_POS  2  /* empty line pos            */
#define DASH_DATA_POS   3  /* empty line pos            */

#define DASH_COLLAPSED  11 /* total lines per module    */
#define DASH_EXPANDED   32 /* total lines when expanded */
#define DASH_NON_DATA   4  /* items without stats       */

#define DASH_INIT_X     2  /* x-axis offset             */
#define DASH_BW_LEN     11 /* max bandwidth length      */
#define DASH_SRV_TM_LEN 9  /* max served time length    */
#define DASH_SPACE      1  /* space between data        */

#define VISIT_HEAD "Unique visitors per day - Including spiders"
#define VISIT_DESC "Hits having the same IP, date and agent are a unique visit."
#define VISIT_ID   "visitors"
#define REQUE_HEAD "Requested files (Pages-URL)"
#define REQUE_DESC "Top Requested Files sorted by hits - [time served] [protocol] [method]"
#define REQUE_ID   "requests"
#define STATI_HEAD "Requested static files (e.g., png, js, css, etc.)"
#define STATI_DESC "Top Static Files sorted by hits - [time served] [protocol] [method]"
#define STATI_ID   "static_requests"
#define FOUND_HEAD "HTTP 404 Not Found URLs"
#define FOUND_DESC "Top 404 Not Found URLs sorted by hits - [time served] [protocol] [method]"
#define FOUND_ID   "not_found"
#define HOSTS_HEAD "Hosts"
#define HOSTS_DESC "Top Hosts sorted by hits - [bandwidth] [time served]"
#define HOSTS_ID   "hosts"
#define OPERA_HEAD "Operating Systems"
#define OPERA_DESC "Top Operating Systems sorted by visitors"
#define OPERA_ID   "os"
#define BROWS_HEAD "Browsers"
#define BROWS_DESC "Top Browsers sorted by visitors"
#define BROWS_ID   "browsers"
#define REFER_HEAD "Referrers URLs"
#define REFER_DESC "Top Requested Referrers sorted by hits"
#define REFER_ID   "referrers"
#define SITES_HEAD "Referring Sites"
#define SITES_DESC "Top Referring Sites sorted  by hits"
#define SITES_ID   "referring_sites"
#define KEYPH_HEAD "Keyphrases from Google's search engine"
#define KEYPH_DESC "Top Keyphrases sorted by hits"
#define KEYPH_ID   "keyphrases"
#define GEOLO_HEAD "Geo Location"
#define GEOLO_DESC "Continent > Country sorted by hits"
#define GEOLO_ID   "geolocation"
#define CODES_HEAD "HTTP Status Codes"
#define CODES_DESC "Top HTTP Status Codes sorted by hits"
#define CODES_ID   "status_codes"
#define GENER_ID   "general"

typedef struct GDashStyle_
{
   const int color_hits;
   const int color_data;
   const int color_bw;
   const int color_percent;
   const int color_bars;
   const int color_usecs;
   const int color_method;
   const int color_protocol;
} GDashStyle;

typedef struct GDashData_
{
   char *bandwidth;
   char *data;
   char *method;
   char *protocol;
   char *serve_time;
   float percent;
   int hits;
   short is_subitem;
   unsigned long long bw;
   unsigned long long usecs;
} GDashData;

typedef struct GDashModule_
{
   GDashData *data;
   GModule module;
   const char *desc;
   const char *head;
   int alloc_data;  /* alloc data items */
   int dash_size;   /* dashboard size   */
   int data_len;
   int hits_len;
   int holder_size; /* hash table size  */
   int ht_size;     /* hash table size  */
   int idx_data;    /* idx data         */
   int max_hits;
   int perc_len;
   unsigned short pos_y;
} GDashModule;

typedef struct GDash_
{
   int total_alloc;
   GDashModule module[TOTAL_MODULES];
} GDash;

typedef struct GSubItem_
{
   GModule module;
   const char *data;
   int hits;
   unsigned long long bw;
   struct GSubItem_ *prev;
   struct GSubItem_ *next;
} GSubItem;

typedef struct GSubList_
{
   int size;
   struct GSubItem_ *head;
   struct GSubItem_ *tail;
} GSubList;

typedef struct GHolderItem_
{
   char *data;
   char *method;
   char *protocol;
   GSubList *sub_list;
   int hits;
   unsigned long long bw;
   unsigned long long usecs;
} GHolderItem;

typedef struct GHolder_
{
   GHolderItem *items; /* data                             */
   GModule module;     /* current module                   */
   int idx;            /* first level index                */
   int holder_size;    /* total num of items (first level) */
   int sub_items_size; /* total number of sub items        */
} GHolder;

typedef struct GRawDataItem_
{
   void *key;
   void *value;
} GRawDataItem;

typedef struct GRawData_
{
   GRawDataItem *items; /* data                     */
   GModule module;      /* current module           */
   int idx;             /* first level index        */
   int size;            /* total num of items on ht */
} GRawData;

#ifdef HAVE_LIBGEOIP
char *get_geoip_data (const char *data);
#endif

float get_percentage (unsigned long long total, unsigned long long hit);
GDashData *new_gdata (unsigned int size);
GDash *new_gdash (void);
GHashTable *get_ht_by_module (GModule module);
GHolder *new_gholder (unsigned int size);
GRawData *parse_raw_data (GHashTable * ht, int ht_size, GModule module);
int get_item_idx_in_holder (GHolder * holder, const char *k);
int perform_next_find (GHolder * h, GScrolling * scrolling);
int render_find_dialog (WINDOW * main_win, GScrolling * scrolling);
int set_module_from_mouse_event (GScrolling *scrolling, GDash *dash, int y);
unsigned int get_ht_size_by_module (GModule module);
void *add_hostname_node (void *ptr_holder);
void add_sub_item_back (GSubList * sub_list, GModule module, const char *data, int hits, unsigned long long bw);
void display_content (WINDOW * win, GLog * logger, GDash * dash, GScrolling * scrolling);
void free_dashboard (GDash * dash);
void free_holder_by_module (GHolder ** holder, GModule module);
void free_holder (GHolder ** holder);
void load_data_to_dash (GHolder * h, GDash * dash, GModule module, GScrolling * scrolling);
void load_data_to_holder (GRawData * raw_data, GHolder * h, GModule module, GSort sort);
void load_host_to_holder (GHolder * h, char *ip);
void reset_find (void);
void reset_scroll_offsets (GScrolling * scrolling);
/* *INDENT-ON* */

#endif
