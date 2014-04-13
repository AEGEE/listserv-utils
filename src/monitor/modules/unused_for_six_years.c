#include <glib.h>
#include <liblistserv.h>
#include <string.h>

extern GHashTable* unused_for_six_years_hashtable;

char
unused_for_six_years_LTX_changed_list (struct listserv *l __attribute__ ((unused)),
				   const char const * listname)
{
  return (g_hash_table_lookup_extended (unused_for_six_years_hashtable,
				listname, NULL, NULL) == FALSE) ? 0 : 1;
}

char*
unused_for_six_years_LTX_listowners_mailing (const char const *listname  __attribute__ ((unused)))
{
  return strdup("\
== Unused list for at least six years ========================================\r\n\
Your list was not used in the last six years.  Would you mind if it is deleted?\r\n\
Shall you need to ask a friend of yours before deciding, please do so on\r\n\
your own and let us know the final result.");
}
