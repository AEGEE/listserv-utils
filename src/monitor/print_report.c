#include <glib.h>
#include <liblistserv.h>
#include "listserv-monitor.h"

struct report {
  int affected_lists;
  struct listserv *l;
  GString *detailed_report;
  int *errors;
  int categories; // 0 - no categories, 1!, 2@, 3:, 4- , 5?
  int format; // 0 - txt, 1 - html
  int comment;
};

static void
per_list (gpointer p1, gpointer p2, gpointer p3)
{
  char *listname   = (char*)p1;
  GByteArray *gb   = (GByteArray*)p2;
  struct report *r = (struct report*)p3;
  GString *list = g_string_new ("");
  if (r->format == 0) {
    char temp[20];
    sprintf (temp, "%%-%is ", longest_listname);
    g_string_append_printf (list, temp, listname);
  }
  else
    g_string_append_printf (list, "%s</td><td>", listname);
  unsigned int k;
  int list_affected = 0;
  int notify_listmaster_only = 0;
  for (k = 0; k < modules->len; k++) {
    struct module *mod = g_ptr_array_index (modules, k);
    //0 - OK, 1 - notify listmasters, 2 - notify listmasters and listowners
    //3 - no changed_list, 4 - module disabled
    // Categories
    // 0: "!" notify listowners and listmasters = 1
    // 1: "@" notify listowners = 2
    // 2: ":" module has no changed_list = 3
    // 3: "-" module disabled = 5, 6
    // 4: "?" module unnecessary disabled = 4, 7
    char *comment;
    if (gb->data[k] >= 4) {
      char *listhdr = listserv_get (r->l, listname, "HDR");
      strcpy (disabled_module+7, mod->name);
      comment = g_strstr_len (listhdr, -1, disabled_module)
	+ strlen (mod->name) + 7;
      while (comment[0] == ' ') comment++;
      if (comment[0] != '\n') {
	comment--;
	int len = 0;
	while (comment[len] != '\n') len++;
	len+= 1;
	comment = g_strndup (comment, len);
	comment[0] = '(';
	comment[len-1] = ')';
      } else comment = NULL;
    } else if (mod->comment && gb->data[k]) {
      char *temp = mod->comment (listname);
      if (temp) {
	comment = g_strconcat ("(", temp, ")", NULL);
	g_free (temp);
      } else comment = NULL;
    } else comment = NULL;
    switch (gb->data[k]) {
      case 0: break;
      case 1:
	r->errors[k * 5 + 0]++;
	r->categories |= 1;
        list_affected = 1;
	g_string_append_printf (list, "!%s%s ", mod->name,
				comment ? comment : "");
	notify_listmaster_only = 1;
	break; // "!"
      case 2:
	r->errors[k * 5 + 1]++;
	r->categories |= 2;
        list_affected = 1;
	g_string_append_printf (list, "@%s%s ", mod->name,
				comment ? comment : "");
	break; // "@"
      case 3:
	r->errors[k * 5 + 2]++;
	r->categories |= 4;
        list_affected = 1;
	g_string_append_printf (list, ":%s%s ", mod->name,
				comment ? comment : "");
	break; // ":"
      case 5:
      case 6:
	r->errors[k * 5 + 3]++;
	r->categories |= 8;
        list_affected = 1;
	g_string_append_printf (list, "-%s%s ", mod->name,
				comment ? comment : "");
	break; // "-"
      case 4:
      case 7:
	r->errors[k * 5 + 4]++;
	r->categories |= 16;
        list_affected = 1;
	g_string_append_printf (list, "?%s%s ", mod->name,
				comment ? comment : "");
	break; // "?"
    }
    if (comment) {
      g_free (comment);
      r->comment++;
    }
  }
  if (   (r->format == 0 && (list->len > longest_listname + 2))
      || (r->format == 1 && (list->len > strlen(listname) + 9))) {
    g_string_erase (list, list->len - 1, 1);
    if (r->format == 0)
      g_string_append_printf (r->detailed_report, "%s\r\n", list->str);
    else
      g_string_append_printf (r->detailed_report,
			      "<tr style='background: %s'><td>%s</td></tr>\r\n",
			      (notify_listmaster_only == 0) ?
			        ((r->affected_lists % 2 == 0) ? "#DDDDDD" : "#EEEEEE"):
                                ((r->affected_lists % 2 == 0) ? "#FFBB99" : "#FFCC99"),
			      list->str);
    r->affected_lists += list_affected;
  }
  g_string_free (list, TRUE);
}

char*
print_report (GHashTable *gh, struct listserv *l, int format)
{
  struct report r;
  r.errors = g_malloc0(sizeof(int) * 5 * modules->len);
  r.affected_lists = 0;
  r.format = format;
  r.comment = 0;
  r.l = l;
  r.categories = 0;
  r.detailed_report = g_string_new_len("", 0);
  GString *report = g_string_new("");
  if (r.format == 1)
    g_string_append_printf(report, "<html><body>");
  char **owned_lists = listserv_getowned_lists(l);
  int i = 0;
  while (owned_lists[i]) {
    GByteArray *gb = g_hash_table_lookup(gh, owned_lists[i]);
    if (gb != NULL)
      per_list(owned_lists[i], gb, &r);
    i++;
  }
  if (r.affected_lists == 0) {
    g_free(r.errors);
    g_string_free (report, TRUE);
    g_string_free (r.detailed_report, TRUE);
    return strdup ("");
  }
  if (r.format == 0)
    g_string_append_printf (report, "%37s", "module name");
  else
    g_string_append_printf (report, "<table><tr><th>%s</th>", "module name");

#define insert(x,y) if (r.categories & x) {		\
    if (r.format == 0) g_string_append_printf (report, "   %c", y);	\
  else g_string_append_printf (report, "<th>%c</th>", y);\
 }
  insert(1, '!');
  insert(2, '@');
  insert(4, ':');
  insert(8, '-');
  insert(16, '?');
#undef insert

#define NEW_LINE  ((r.format == 0)?"\r\n":"\r\n<br/>")

  if (r.format == 1)
    g_string_append_printf(report, "</tr>\r\n");
  unsigned int k;
  int j = 0;
  for (k = 0; k < modules->len; k++) {
    if (   r.errors[k * 5 + 0]
	|| r.errors[k * 5 + 1]
	|| r.errors[k * 5 + 2]
	|| r.errors[k * 5 + 3]
	|| r.errors[k * 5 + 4]) {
      struct module *mod = g_ptr_array_index (modules, k);
      j++;
      if (r.format == 0)
	g_string_append_printf (report, "\r\n%37s", mod->name);
      else 
	g_string_append_printf (report,
				"<tr style='background: %s'><td>%s</td>",
				(j % 2 == 0) ? "#DDDDDD":"#EEEEEE",
				mod->name);
      int f;
      int j;
      for (f = 0, j = 1; f < 5; f++, j*=2)
	if (r.categories & j) {
	  if (r.errors[k * 5 + f]) {
	    if (r.format == 0)
	      g_string_append_printf (report, "%4d", r.errors[k * 5 + f]);
	    else
	      g_string_append_printf(report,"<td align='right'>%d</td>",r.errors[k * 5 + f]);
	  } else if (r.format == 0)
	           g_string_append_printf (report, "    ");
                 else
	           g_string_append_printf (report, "<td></td>");
      }
      if (r.format == 1)
	g_string_append_printf (report, "</tr>\r\n");
    }
  }
  if (r.format == 1)
    g_string_append_printf (report, "</table>");  

  g_string_append_printf (report, "%s%37s %d out of %d (%2.1f%%)%s", NEW_LINE,
			  "Affected lists", r.affected_lists, i, r.affected_lists * 100.0 / i, NEW_LINE);
  g_string_append_printf (report, "%s\r\n--------     Legend     --------%s",
			  NEW_LINE, NEW_LINE);
  if (r.categories & 1)
    g_string_append_printf (report, "%s! - problems with the list, reported only to the listmasters", NEW_LINE);
  if (r.categories & 2)
    g_string_append_printf (report, "%s@ - problems with the list, reported biweekly to the listowners", NEW_LINE);
  if (r.categories & 4)
    g_string_append_printf (report, "%s: - module has no changed_list function (might be useful in the future)", NEW_LINE);
  if (r.categories & 8)
    g_string_append_printf (report, "%s- - module is disabled for that list, criterion not fulfiled", NEW_LINE);
  if (r.categories & 16)
    g_string_append_printf (report, "%s? - module is unnecessary disabled for that list, criterion fulfiled", NEW_LINE);

  g_string_append_printf (report, "%s%s\
Format of the detailed report: \
Only lists, where some criterion is not met, or is disabled, are mentioned.  Each line starts with a listname, followed by the unmatched criteria (=module name validating the criterion). Each module name is prefixed by a symbol from the above legend.%s%s", NEW_LINE, NEW_LINE, NEW_LINE, NEW_LINE);
  if (r.format == 0)
    g_string_append_printf (report,
			    "-------- Detailed Report %s--------\r\n%s",
			    r.comment ? " (with comments)" : "",
			    r.detailed_report->str);
  else
    g_string_append_printf (report, "<br/><table><caption><b>Detailed Report</b></caption><tr><th>Listname</th><th>Failed modules%s</th></tr>\r\n%s</table>",
			    r.comment ? " (with comments)" : "",
			    r.detailed_report->str);
  g_string_free (r.detailed_report, TRUE);
  g_free (r.errors);
  if (r.format == 1)
    g_string_append_printf (report, "</body></html>\r\n");
  return g_string_free (report, FALSE);
}

void mail_report (GHashTable *gh, struct listserv *l)
{
  char *rcpts[] = {"mail@aegee.org", NULL};
  GString *text = g_string_new("--0016e659f7a2276c170480ea7029\r\n\
Content-Type: text/plain; charset=UTF-8\r\n\r\n");
  char *temp = print_report (gh, l, 0);
  g_string_append_printf (text, temp);
  g_free (temp);
  g_string_append_printf (text, "--0016e659f7a2276c170480ea7029\r\n\
Content-Type: text/html; charset=UTF-8\r\n\r\n");
  temp = print_report (gh, l, 1);
  g_string_append_printf (text, temp);
  g_free (temp);
  g_string_append_printf (text, "--0016e659f7a2276c170480ea7029--\r\n");
  sendmail(rcpts, "Black list of lists", "multipart/alternative; boundary=0016e659f7a2276c170480ea7029", text->str);
  g_string_free(text, TRUE);
}
