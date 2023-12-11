/*
 * Copyright (c) Tony Bybell 1999-2011.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */

#include "globals.h"

/*
 * tree.h 12/05/98ajb
 */
#ifndef WAVE_TREE_H
#define WAVE_TREE_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <gtkwave.h>

#define FST_TREE_SEARCH_NEXT_LIMIT (40)

#define WAVE_T_WHICH_UNDEFINED_COMPNAME (-1)
#define WAVE_T_WHICH_COMPNAME_START (-2)

#ifdef WAVE_USE_STRUCT_PACKING
#pragma pack(push)
#pragma pack(1)
#endif

struct stem_struct_t
{
    uint32_t stem_idx; /* in stem_path_string_table */
    uint32_t stem_line_number;
};

#ifdef WAVE_USE_STRUCT_PACKING
#pragma pack(pop)
#endif

/* names at the end of the main hierarchy 010104ajb    */

struct treechain
{
    GwTreeNode *tree; /* top of list of selected item in hierarchy */
    GwTreeNode *label; /* actual selected item in hierarchy */
    struct treechain *next;
};

struct autocoalesce_free_list
{
    struct autocoalesce_free_list *
        next; /* list of coalesced names generated by treesearch gadget..only user of this struct */
    char *name; /* free up next time filtering is performed */
};

void init_tree(void);
void build_tree_from_name(const char *s, int which);
void treedebug(GwTreeNode *t, char *s);

char *leastsig_hiername(char *nam);
void allocate_and_decorate_module_tree_node(unsigned char ttype,
                                            const char *scopename,
                                            const char *compname,
                                            uint32_t scopename_len,
                                            uint32_t compname_len,
                                            uint32_t t_stem,
                                            uint32_t t_istem);
int decorated_module_cleanup(void);

void order_facs_from_treesort(GwTreeNode *t, void *v);

void treenamefix(GwTreeNode *t);

#ifdef WAVE_USE_STRUCT_PACKING
#define WAVE_TALLOC_POOL_SIZE (64 * 1024)
#define WAVE_TALLOC_ALTREQ_SIZE (4 * 1024)
GwTreeNode *talloc_2(size_t siz);
#else
#define talloc_2(x) calloc_2(1, (x))
#endif

/* #define WAVE_DISABLE_FAST_TREE */

enum treeview_columns
{
    XXX_NAME_COLUMN,
    XXX_TREE_COLUMN,
    XXX_PXB_COLUMN,
    XXX_NUM_COLUMNS
};

void XXX_maketree(GtkTreeIter *subtree, GwTreeNode *t);
void XXX_maketree2(GtkTreeIter *subtree, GwTreeNode *t, int depth);

void sst_exclusion_loader(void);

GwTreeNode *fetchhigh(GwTreeNode *t);
GwTreeNode *fetchlow(GwTreeNode *t);
void recurse_fetch_high_low(GwTreeNode *t);

#endif
