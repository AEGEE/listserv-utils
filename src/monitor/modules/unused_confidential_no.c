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

static inline int core (struct listserv *l,
		        const char const *listname) {
  return keyword_doesnot_contain (l, listname, "Notebook", "No")
    && (g_hash_table_lookup_extended (unused_for_one_year_hashtable,
				      listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_two_years_hashtable,
					 listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_three_years_hashtable,
					 listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_four_years_hashtable,
					 listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_five_years_hashtable,
					 listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_six_years_hashtable,
					 listname, NULL, NULL)
	|| g_hash_table_lookup_extended (unused_for_at_least_seven_years_hashtable,
					 listname, NULL, NULL))
    && keyword_does_contain (l, listname, "Confidential", "No");
}

char
unused_confidential_no_LTX_changed_list (struct listserv *l,
					  const char const * listname)
{
  if (core (l, listname)) {
    keyword_try_update (l, listname, "Confidential", "Yes");
    return core (l, listname);
  }
  return 0;
}
