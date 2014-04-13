#include <glib.h>
#include <liblistserv.h>
#include <string.h>
#include "util.h"
#include <stdlib.h>

extern GHashTable *never_actively_used_hashtable;
GHashTable* unused_for_one_year_hashtable;
GHashTable* unused_for_two_years_hashtable;
GHashTable* unused_for_three_years_hashtable;
GHashTable* unused_for_four_years_hashtable;
GHashTable* unused_for_five_years_hashtable;
GHashTable* unused_for_six_years_hashtable;
GHashTable* unused_for_at_least_seven_years_hashtable;

void unused_for_one_year_LTX_load ()
{
  unused_for_one_year_hashtable = g_hash_table_new(g_str_hash,
						   g_str_equal);
  unused_for_two_years_hashtable = g_hash_table_new(g_str_hash,
						    g_str_equal);
  unused_for_three_years_hashtable = g_hash_table_new(g_str_hash,
						      g_str_equal);
  unused_for_four_years_hashtable = g_hash_table_new(g_str_hash,
						     g_str_equal);
  unused_for_five_years_hashtable = g_hash_table_new(g_str_hash,
						     g_str_equal);
  unused_for_six_years_hashtable = g_hash_table_new(g_str_hash,
						    g_str_equal);
  unused_for_at_least_seven_years_hashtable = g_hash_table_new(g_str_hash,
							       g_str_equal);
}

void unused_for_one_year_LTX_unload ()
{
  g_hash_table_destroy (unused_for_one_year_hashtable);
  g_hash_table_destroy (unused_for_two_years_hashtable);
  g_hash_table_destroy (unused_for_three_years_hashtable);
  g_hash_table_destroy (unused_for_four_years_hashtable);
  g_hash_table_destroy (unused_for_five_years_hashtable);
  g_hash_table_destroy (unused_for_six_years_hashtable);
  g_hash_table_destroy (unused_for_at_least_seven_years_hashtable);
}

char
unused_for_one_year_LTX_changed_list (struct listserv *l, const char const * listname) {
  if (keyword_does_contain (l, listname, "Notebook", "No")
      || g_hash_table_lookup_extended (never_actively_used_hashtable,
				       listname, NULL, NULL) == TRUE)
    return 0;

  char **files = listserv_list_filelist(l, listname);
  int j = 0;
  char *last_log = NULL;
  while (files[j]) {
    if (files[j][0] == 'L' && files[j][1] == 'O' && files[j][2] == 'G')
      last_log = files[j];
    j++;
  }

  if (!last_log) return 0;
  long int last_post = strtol (last_log + 3, NULL, 10);
  int l_year, l_month;
  if (strlen (last_log) < 6) {
    l_year = last_post;
    l_month = 0;
  } else {
    l_year = last_post / 100;
    l_month = last_post % 100;
  }
  l_year += (l_year > 85) ? 1900 : 2000;
  //now l_year is the year of the last posting and
  //l_month is the month of the last posting, or 0

  time_t _time = time (NULL);
  struct tm lt;
  localtime_r (&_time, &lt);
  int this_month = lt.tm_mon;
  int this_year = lt.tm_year + 1900;

  int now = this_year * 12 + this_month;
  int then = l_year * 12 + (l_month ? l_month : 12);

  //  g_printf ("%s last year %i last month %i diff_m %i diff_y %i\n",
  //	    listname, l_year, l_month, (now - then) / 12);
  switch ((now - then)  / 12) {
    case 0: return 0;
    case 1: g_hash_table_insert (unused_for_one_year_hashtable,
				 (gpointer) listname, NULL);
      return 1;
    case 2: g_hash_table_insert (unused_for_two_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
    case 3: g_hash_table_insert (unused_for_three_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
    case 4: g_hash_table_insert (unused_for_four_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
    case 5: g_hash_table_insert (unused_for_five_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
    case 6: g_hash_table_insert (unused_for_six_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
    case 7:
    default:g_hash_table_insert (unused_for_at_least_seven_years_hashtable,
				 (gpointer) listname, NULL);
      return 0;
  }
}

char*
unused_for_one_year_LTX_listowners_mailing (const char const *listname __attribute__ ((unused)))
{
  return strdup("\
== Unused list for at least one year =========================================\r\n\
Your list was not used in the last year.  Would you mind if it is deleted?\r\n\
Shall you need to ask a friend of yours before deciding, please do so on\r\n\
your own and let us know the final result.");
}
