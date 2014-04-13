#include <glib.h>
#include <liblistserv.h>
#include <string.h>
#include "util.h"

extern GHashTable* unused_for_one_year_hashtable;
extern GHashTable* unused_for_two_years_hashtable;
extern GHashTable* unused_for_three_years_hashtable;
extern GHashTable* unused_for_four_years_hashtable;
extern GHashTable* unused_for_five_years_hashtable;
extern GHashTable* unused_for_six_years_hashtable;
extern GHashTable* unused_for_at_least_seven_years_hashtable;
extern GHashTable* never_actively_used_hashtable;

char
possibly_spammed_LTX_changed_list (struct listserv *l,
				   const char const * listname)
{
  if (g_hash_table_lookup_extended (unused_for_one_year_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_two_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_three_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_four_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_five_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_six_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (unused_for_at_least_seven_years_hashtable,
				    listname, NULL, NULL) == FALSE
      && g_hash_table_lookup_extended (never_actively_used_hashtable,
				       listname, NULL, NULL) == FALSE
      && keyword_does_contain (l, listname, "Send", "Public")
      && keyword_doesnot_contain (l, listname, "Confidential", "No")) {
    if ( keyword_does_contain (l, listname, "Send", "Non-Member")
	 && keyword_does_contain (l, listname, "Send", "Confirm")) {
      char **moderators = listserv_getlist_keyword (l, listname, "Moderator");
      if (moderators[0] != NULL)
	return 0;
    }
    return 1;
  }
  return 0;
}
