#include <glib.h>
#include <liblistserv.h>
#include <string.h>

extern GHashTable* unused_for_at_least_seven_years_hashtable;

char
unused_for_at_least_seven_years_LTX_changed_list (struct listserv *l __attribute__ ((unused)),
				   const char const * listname)
{
  return (g_hash_table_lookup_extended 
	  (unused_for_at_least_seven_years_hashtable,
	   listname, NULL, NULL) == FALSE) ? 0 : 1;
}

char*
unused_for_at_least_seven_years_LTX_listowners_mailing (const char const *listname __attribute__ ((unused)))
{
  return strdup("\
== Unused list for at least seven years ======================================\r\n\
Your list was not used in the last seven years.  Would you mind if it is \r\n\
deleted?  Shall you need to ask a friend of yours before deciding, please do \r\n\
so on your own and let us know the final result.");
}
