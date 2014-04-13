#include <liblistserv.h>
#include <stdio.h>
#include <glib.h>
#include <string.h>

char command[1000];

int main ()
{
  struct listserv *l = listserv_init (NULL, NULL, NULL);
  GHashTable *gh = g_hash_table_new_full (g_str_hash, g_str_equal,
					 g_free, g_free);
  char** owned_lists = listserv_getowned_lists(l);
  int i = 0;
  while (owned_lists[i]) {
    static const char* keywords[] = {"Owner", NULL};
    char** owners = listserv_getemails_fromkeywords (l, owned_lists[i],
						     keywords);
    int j = 0;
    while (owners[j]) {
      char *upper = g_ascii_strup (owners[j], -1);
      char *elem = g_hash_table_lookup (gh, upper);
      g_hash_table_insert(gh, upper,
			  elem ? g_strconcat (owned_lists[i], " ", elem, NULL) 
			  : g_strdup(owned_lists[i]));
      j++;
    }
    i++;
  }
  struct listserv_subscriber **subscribers =
    listserv_getsubscribers(l, "LISTOWNERS-L");
  i = 0;
  while (subscribers[i]) {
    char *upper = g_ascii_strup (subscribers[i]->email, -1);
    char *elem = g_hash_table_lookup (gh, upper);
    if (elem) {
      g_hash_table_remove (gh, upper);
    } else {
      sprintf(command, "QUIET DEL LISTOWNERS-L %s", upper);
      listserv_command(l, command);
    }
    i++;
    g_free(upper);
  }
  GHashTableIter iter;
  gpointer email;
  gpointer text;
  g_hash_table_iter_init (&iter, gh);
  while (g_hash_table_iter_next (&iter, &email, &text)) {
    sprintf(command, "QUIET ADD LISTOWNERS-L %s", (char*)email);
    if (strstr(listserv_command (l, command), "signup file")) {
      char *text2 = NULL;
      if (strchr ((char*)text, ' ') == NULL)
	text2 = g_strconcat ("GL ", (char*)text, NULL);
      sprintf (command, "QUIET ADD LISTOWNERS-L %s %s",
	      (char*)email, text2 ? text2 : (char*)text);
      if (text2) g_free(text2);
      listserv_command (l, command);
    }
  }
  g_hash_table_destroy(gh);
  listserv_destroy (l);
  return 0;
}
