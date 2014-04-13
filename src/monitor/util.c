#include "util.h"
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <glib/gstdio.h>


int keyword_try_update (struct listserv *l, const char const *listname,
		    const char* keyword, const char const *value)
{
  char* listhdr = listserv_get (l, listname, "HDR");
  time_t _time = time (NULL);
  struct tm lt;
  localtime_r (&_time, &lt);
  char* comment = g_alloca(12);
  g_sprintf(comment, "LMO%04d%02d%02d", 
	   lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
  char* replacement = g_alloca(value ? strlen(value) : 0 + 40);
  g_sprintf(replacement, "* %-17s = %s (%s)", keyword, value, comment);
  char *pattern = g_alloca(strlen(keyword) + 27);
  g_sprintf(pattern, "[ \t]%s[ \t]*=[ \t]*[^ \t\t]", keyword);
  //    if ( /* strcasestr (listhdr, keyword) == 0 */
  if ( value && g_regex_match_simple (pattern, listhdr,
				      G_REGEX_CASELESS | G_REGEX_DOTALL,
				      G_REGEX_MATCH_NOTEMPTY) == FALSE) {
    GString *str = g_string_new (listhdr);
    g_string_append_c (str, '\n');
    g_string_append_printf (str, replacement, keyword, value);
    char *ret = listserv_putheader (l, listname, str->str);
    g_string_free (str, TRUE);
    return ret[0] != 'T';
  } else {
    char *regex = g_alloca (80 + strlen(keyword));
    g_sprintf (regex,
      "^(.*)(\\*[ \t]*%s[ \t]*=[ \t]*[^ \n\t]*(?:,[ \t]*[^ \n\t]+)*)(.*)$",
	       keyword);
    GRegex *reg = g_regex_new (regex, G_REGEX_CASELESS | G_REGEX_DOTALL,
			       G_REGEX_MATCH_NOTEMPTY, NULL);
    gchar **found = g_regex_split (reg, listhdr, 0);
    if (found[0] == NULL
	|| found[1] == NULL
	|| found[2] == NULL
	|| found[3] == NULL
	|| found[4] == NULL)
      return 1;
    GString *newhdr = g_string_new(found[1]);
    if (value)
      g_string_append_printf (newhdr, "%s", replacement);
    else 
      g_string_append_printf (newhdr, "* %s (removed %s)", found[2], comment);
    g_string_append_printf(newhdr, "%s", found[3]);
    g_strfreev(found);
    char *ret = listserv_putheader (l, listname, newhdr->str);
    g_string_free (newhdr, TRUE);
    g_regex_unref (reg);
    return ret[0] != 'T';
  }
}

int
keyword_does_contain (struct listserv *l, const char const * listname,
		      const char* const keyword, const char const * value)
{
  char **kw = listserv_getlist_keyword(l, listname, keyword);
  int i = 0;
  while (kw[i])
    if (g_ascii_strcasecmp(value, kw[i]) == 0) return 1;
    else i++;
  return 0;
}

int
keyword_doesnot_contain (struct listserv *l, const char const * listname, 
			 const char const *keyword,
			 const char const *value)
{
  char **kw = listserv_getlist_keyword(l, listname, keyword);
  int i = 0;
  while (kw[i])
    if (g_ascii_strcasecmp(value, kw[i]) == 0) return 0;
    else i++;
  return 1;
}
