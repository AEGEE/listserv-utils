#include "listserv-monitor.h"
#include <glib/gstdio.h>


static void	
load_modules() {
  extern const lt_dlsymlist lt_preloaded_symbols[];
  lt_dlpreload_default (lt_preloaded_symbols);
  lt_dlinit();
  modules = g_ptr_array_new ();
  int i = 0;
  longest_modulename = 0;
  while (lt_preloaded_symbols[i].name != NULL) {
    if (lt_preloaded_symbols[i++].address != NULL) continue;
    struct module *mod = g_malloc (sizeof (struct module));
    mod->name = g_strdup (lt_preloaded_symbols[i-1].name);
    if (strlen(mod->name) > longest_modulename)
      longest_modulename = strlen (mod->name);
    char *temp = strchr(mod->name, '.');
    if (temp) *temp = '\0';
    mod->mod                 = lt_dlopen (lt_preloaded_symbols[i-1].name);
    mod->load                = lt_dlsym (mod->mod, "load");
    mod->unload              = lt_dlsym (mod->mod, "unload");
    mod->listowners_mailing  = lt_dlsym (mod->mod, "listowners_mailing");
    mod->changed_list        = lt_dlsym (mod->mod, "changed_list");
    mod->comment             = lt_dlsym (mod->mod, "comment");
    mod->changed_mailtpl     = lt_dlsym (mod->mod, "changed_mailtpl");
    if (mod->changed_list == NULL && mod->changed_mailtpl == NULL) {
      g_free (mod->name);
      lt_dlclose (mod->mod);
      g_free (mod);
      continue;
    }
    if (mod->load) mod->load ();
    g_ptr_array_add (modules, mod);
  }
  disabled_module = g_malloc(longest_modulename + 8);
  g_sprintf(disabled_module, "* * no_");
}

GByteArray*
changed_list(struct listserv *l, char *listname) {
  unsigned i;
  GByteArray *gb = g_byte_array_sized_new(modules->len);
  disabled_module[7] = '\0';
  char *list_header = g_strstr_len (listserv_get (l, listname, "HDR"), -1,
				    disabled_module);
  for (i = 0; i < modules->len; i++) {
    guint8 mod_status = 0;
    if (list_header) {
      struct module *mod = g_ptr_array_index (modules, i);
      strcpy (disabled_module+7, mod->name);
      if (g_strstr_len (list_header, -1, disabled_module))//module disabled 
	mod_status = 4;
    }
    g_byte_array_append(gb, &mod_status, 1);
  }

  for (i = 0; i < modules->len; i++) {
    struct module *mod = g_ptr_array_index (modules, i);
    if (mod->changed_list){ 
      if (mod->changed_list (l, listname)) {
	gb->data[i] |= 1;
	if (gb->data[i] && mod->listowners_mailing)
	  gb->data[i] += 1;
      }
    } else
      gb->data[i] |= 3;
  }
  return gb;
}

GByteArray*
changed_mailtpl (struct listserv *l __attribute__ ((unused)),
		 char *listname __attribute__ ((unused)))
{
  return NULL;
}

static void
unload_modules () {
  unsigned int i;
  for (i = 0; i < modules->len; i++) {
    struct module *mod = g_ptr_array_index (modules, i);
    if (mod->unload) mod->unload ();
    g_free(mod->name);
    lt_dlclose (mod->mod);
    g_free (mod);
  }
  g_ptr_array_free (modules, 1);
  lt_dlexit();
  g_free(disabled_module);
}

void
del_list (gpointer data)
{
  g_byte_array_free (data, TRUE);
}


void
update_hash_table (GHashTable *gh, struct listserv *l, char* listname)
{
  GByteArray *gb = g_hash_table_lookup (gh, listname);
  if (gb) g_byte_array_free (gb, TRUE);
  gb = changed_list(l, listname);
  g_hash_table_insert(gh, listname, gb);
  if (strlen(listname) > longest_listname)
    longest_listname = strlen(listname);
}

int
main () {
  /*  printf("%s\n", insert_new_lines("f0fffd fdfdsa fdsafsad fdsfdsafdafdfasd fdsdsfdfsadfsdsffd f1dsfdsdfsadsfadsfasffffd fds fdsfadsfads fdsafasdf  fgdsfdsaf fdsafa fasdfs  f2dsadfasdfads fsadfasdddddddddddddddddddddd fddddddddddddddddddddddd d3ddddddddddddddd\n \
 f41dsafdsafsa fadsfadsfsda\n\
f42sadfdsaf dfdafadsfads fasdfad\n\
f5dsafdsa ffffffffffffffffffffffffffff ddddddddddddddddddd s6sssssssssssssssssssss ddddddddddddddddddddd fffffffffffffffffff d7ddddddddddddddddd", 1));
return 0; */
  load_modules ();
  struct listserv *l = listserv_init (NULL, NULL, NULL);
  char **owned_lists = listserv_getowned_lists (l);
  longest_listname = 0;
  int i = 0;
  GHashTable *gh = g_hash_table_new_full ( g_str_hash, g_str_equal,
			       NULL, NULL /* del_list */);
  while (owned_lists[i])
      update_hash_table (gh, l, owned_lists[i++]);
  //  update_hash_table (gh, l, "ITC-BOARD-L"); char *report = print_report (gh, l, 0);  g_printf (report);  g_free (report);    exit;
  mail_report(gh, l);
  //  mailing_alllists (gh);
  g_hash_table_destroy (gh);
  listserv_destroy (l);
  unload_modules ();
  return 0;
}
