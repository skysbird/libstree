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
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <lst_debug.h>
#include <lst_structs.h>
#include <lst_stree.h>
#include <lst_string.h>
#include <lst_algorithms.h>


static void
test_usage(char *progname)
{
  printf("USAGE: %s <string(s) ...>\n"
	 "This test program first inserts the strings provided\n"
	 "into a suffix tree and prints it out.\n", progname);
  exit(0);
}


LST_Node_List head;
//LST_StringHash a;

void get_all_leafs(LST_Node *node){

    if(lst_node_is_leaf(node)){
        LST_Node_Item *item = malloc(sizeof(LST_Node_Item));
        item->node = node;
        TAILQ_INSERT_TAIL(&head,item,entries);
    } 
    LST_Edge *edge;
       for (edge =node->kids.lh_first; edge; edge = edge->siblings.le_next) {
            get_all_leafs(edge->dst_node);
      }
 
}

int
main(int argc, char **argv)
{
  TAILQ_INIT(&head);
  LST_STree     *tree;
  LST_String    *string = NULL;
  LST_StringSet *set;
  int i;
  
  //if (argc < 2)
  //  test_usage(argv[0]);

  /* Create a string set to conveniently hold all our strings: */
  set = lst_stringset_new();

  /* Add all strings passed on the command line to the set.
   * Note that we pass the string length excluding the null
   * terminator, libstree handles that generically.
   */
//  for (i = 1; i < argc; i++)
//    {
//      string = lst_string_new("aa", 1, strlen("aa"));
//      lst_stringset_add(set, string);
//    }

  string = lst_string_new((void *)"aa", sizeof(char), strlen("aa"));
  lst_stringset_add(set, string);

  string = lst_string_new((void *)"ab", sizeof(char), strlen("ab"));
  lst_stringset_add(set, string);

  string = lst_string_new((void *)"skysbird", sizeof(char), strlen("skysbird"));
  lst_stringset_add(set, string);

  string = lst_string_new((void *)"mapleskip", sizeof(char), strlen("mapleskip"));
  lst_stringset_add(set, string);

  /* Create a suffix tree for all strings in the set: */
  tree = lst_stree_new(set);
  fprintf(stderr, "Tree after all insertions:\n");
  lst_debug_print_tree(tree);

  //get the root's child edges
  LST_Node *node = tree->root_node;
  //LST_Edge *edge = NULL;
  LST_PathEnd end;

  LST_Edge *edge = node_find_edge_with_startitem(node, string,0);

  LST_String *string1 = lst_string_new((void *)"ky", sizeof(char), strlen("ky"));

  stree_follow_string_slow(tree,tree->root_node,string1,&end);

  
  get_all_leafs(end.node);
  //int i = 0;
  //for (node = end.node->leafs.lh_first; node; node = node->leafs.le_next){
  //  printf("%d\n",i);
  //  ++i;
  //}  

  //for (; edge; edge = edge->siblings.le_next) {
  //      fprintf(stderr,"%s\n",lst_string_print(edge->range.string));

  //}
  LST_Node_Item *item;
  TAILQ_FOREACH(item, &head, entries) {
    //LST_String *str = lst_node_get_string(item->node,0);
    LST_String *str = item->node->up_edge->range.string;
    fprintf(stderr,"%s\n",lst_string_print(str));

  }
  

  /* Free suffix tree: */
  lst_stree_free(tree);

  return 0;
}
