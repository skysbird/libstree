/*

Copyright (C) 2003-2006 Christian Kreibich <christian@whoop.org>.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies of the Software and its documentation and acknowledgment shall be
given in the documentation and software packages that this Software was
used.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/
#ifndef __lst_structs_h
#define __lst_structs_h

#include <sys/queue.h>
#include "lst_string.h"

#define LST_STRING_HASH_SIZE         199

typedef struct lst_stree             LST_STree;
typedef struct lst_node              LST_Node;
typedef struct lst_edge              LST_Edge;
typedef struct lst_string_hash_item  LST_StringHashItem;
typedef struct lst_string_hash       LST_StringHash;
typedef struct lst_node_list       LST_Node_List;


struct lst_edge
{
  LIST_ENTRY(lst_edge)        siblings;

  LST_Node                   *src_node;
  LST_Node                   *dst_node;

  LST_StringIndex             range;
};


struct lst_node
{
  /* Each node maintains a list for its children. */
  LIST_HEAD(elist, lst_edge)  kids;
  u_int                       num_kids;

  /* For DFS/BFS iteration, we maintain a list as well. */
  TAILQ_ENTRY(lst_node)       iteration;

  /* Separately, the leafs of the tree are linked up. */
  LIST_ENTRY(lst_node)        leafs;

  LST_Edge                   *up_edge;

  LST_Node                   *suffix_link_node;  

  int                         index;
  u_int                       id;

  u_int32_t                   visitors;
  u_int                       bus_visited;
};


struct lst_phase_num
{
  LIST_ENTRY(lst_phase_num)   items;
  
  u_int                       phase;
}; 

struct lst_string_hash_item
{
  LIST_ENTRY(lst_string_hash_item) items;

  LST_String   *string;
  int           index;
};

LIST_HEAD(lst_string_hash, lst_string_hash_item);

struct lst_stree
{
  /* Number of strings currently in tree.
   */
  u_int                             num_strings;

  /* Current phase of Ukkonen's algorithm.
   * In order to implement the "Once a leaf, always
   * a leaf" Trick as explained by Gusfield, we make
   * this a pointer to an integer.
   */
  u_int                            *phase;

  /* To avoid the O(m) cost of setting that value in
   * stone once a string insertion is over, we make phase
   * point into the following list. A new element is created
   * for every string insertion.
   */
  LIST_HEAD(phase_s, lst_phase_num) phases;

  /* Current extension of Ukkonen's algorithm. */
  u_int                             ext;

  /* Well ... guess :) */
  LST_Node                         *root_node;

  /* We keep the leafs of our tree chained up: */
  LIST_HEAD(leaf_s, lst_node)       leafs;

  /* A simple hashtable for the strings in the tree, mapping
   * them to indices starting from 1:
   */
  LST_StringHash                   *string_hash;

  /* A counter for string index numbers */
  int                               string_index;

  /* Whether or not we allow duplicates in our tree */
  int                               allow_duplicates;

  /* After each string insertion, the visitor bitstrings in
   * the nodes are outdated. We note this in the following
   * flag. It's cleared whenever lst_alg_set_visitors() is
   * called, which happens if necessary at the beginning of
   * lst_stree_remove_string().
   */
  int                               needs_visitor_update;

  /* If needs_visitors_update is 0, this visitors value can
   * still be used.
   */
  u_int                             visitors;
};


/* A path in an implicit suffix tree can end at either a node, or
 * at some point in the label of an edge. We remember in each
 * extension where we ended using an LST_PathEnd structure.
 */
typedef struct lst_path_end
{
  LST_Node    *node;

  LST_Edge    *edge;
  u_int        offset;

} LST_PathEnd;


typedef struct lst_node_item {
    LST_Node *node;
    TAILQ_ENTRY(lst_node_item) entries;
} LST_Node_Item;

TAILQ_HEAD(lst_node_list, lst_node_item);

#endif
