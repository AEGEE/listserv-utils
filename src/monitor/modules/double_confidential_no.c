//make sure that Subscribe is either closed, or contains Confirm
#include <glib.h>
#include "util.h"
#include <string.h>

static GHashTable* double_confidential_no_hashtable;
static GHashTable* double_confidential_no_prefix_hashtable;

void
double_confidential_no_LTX_load ()
{
  double_confidential_no_hashtable =
    g_hash_table_new_full ( g_str_hash, g_str_equal, g_free, g_free);
  double_confidential_no_prefix_hashtable =
    g_hash_table_new_full ( g_str_hash, g_str_equal, g_free, g_free);
}

void double_confidential_no_LTX_unload()
{
  g_hash_table_destroy (double_confidential_no_hashtable);
  g_hash_table_destroy (double_confidential_no_prefix_hashtable);
}

char*
double_confidential_no_LTX_comment (char *listname) {
  char *elem = g_hash_table_lookup (double_confidential_no_hashtable, g_hash_table_lookup (double_confidential_no_prefix_hashtable, listname));
  if (elem && strchr (elem, ' '))
    return g_strdup (elem);
  return NULL;
}

char
double_confidential_no_LTX_changed_list (struct listserv *l,
					 char * listname) {
  if (keyword_doesnot_contain (l, listname, "Confidential", "No"))
    return 0;
  int num_subscribers = 0;
  struct listserv_subscriber **subscribers =
    listserv_getsubscribers (l, listname);
  while (subscribers[num_subscribers] && num_subscribers < 1000)
    num_subscribers++;
  if (num_subscribers == 1000) return 0;
  char *prefix = g_strdup (listname);
  char *minus = strchr (prefix, '-');
  minus[0] = '\0';
  minus++;
  if (g_ascii_strcasecmp (prefix, "HEIDELBERG") == 0) {
    prefix[1] = 'D';
    prefix[2] = '\0';
  } else if (g_ascii_strcasecmp (prefix, "AEGEE") == 0
	     || g_ascii_strcasecmp (prefix, "MEMBERSHIP") == 0) {
    minus = g_strdup(minus);
    g_free (prefix);
    prefix = minus;
    minus = strchr (minus, '-');
    if (minus) {
      minus[0] = '\0';
      minus++;
    }
  }

  char *elem = g_hash_table_lookup (double_confidential_no_hashtable,
				    prefix);
  if (elem == NULL) {
    g_hash_table_insert (double_confidential_no_hashtable, prefix,
			 g_strdup (listname));
    return 0;
  } else {
    int ret;
    char *first_space = strchr (elem, ' ');
    if (first_space) {
      ret = 0;
    } else {
      ret = 1;
      g_hash_table_insert (double_confidential_no_prefix_hashtable,
			   g_strdup (listname), g_strdup (prefix));
    }
    g_hash_table_insert (double_confidential_no_hashtable, prefix,
			 g_strconcat(elem, " ", listname, NULL));
    return ret;
  }
}

char*
double_confidential_no_LTX_listowners_mailing (char *listname)
{
  char *elem =
    g_hash_table_lookup (double_confidential_no_hashtable,
		 g_hash_table_lookup (double_confidential_no_prefix_hashtable,
	         listname));
  int plural = (strchr(elem, ' ') != NULL);
  return g_strconcat ("\
== One Visible List per Antenna / Project / WG ===============================\r\n\
Since the list of visible lists at https://lists.aegee.org is overcrowded,\r\n\
only one list per body can be listed there.  The latter does not apply for\r\n\
lists with more than 1000 subscribers.\r\n\
Your body has the following lists visible:\r\n    ", elem, "\r\n\
Please decide which ", plural ? "of there lists" : "list", " shall be hidden. Then go to\r\n\
  https://lists.aegee.org/cgi-bin/wa?WIZARD=", listname, "&P=WIZARD_TASK&a=2\r\n \
and login.  At the end of this email you can find information how to login.\r\n\
\r\n\
Find the keyword *Confidential* and set its value to \"Yes\".\r\n\
\r\n\
Then click on Safe at the end of the page.", NULL);
}
