
int add_aliases(struct listserv *l, char* listname, FILE* aliases) {
  //  FILE *aliases = fopen("/home/listserv/aliases.listserv", "a");
  char *listname_small = strdup(listname), *x = listname_small;
  while (x) {
    x[0] = tolower(x[0]);
    x++;
  };
  x = listname_small;
  fprintf(aliases, "\r\n\
%s: \"|lsv_amin /home/listserv/spool %s\"\r\n\
owner-%s:               \"|lsv_amin /home/listserv/spool owner-%s\"\r\n\
%s-request:             \"|lsv_amin /home/listserv/spool %s-request\"\r\n\
%s-search-request:      \"|lsv_amin /home/listserv/spool %s-search-request\"\r\n\
%s-server:              \"|lsv_amin /home/listserv/spool %s-server\"\r\n\
%s-unsubscribe-request: \"|lsv_amin /home/listserv/spool %s-unsubscribe-request\"\r\n\
%s-signoff-request:     \"|lsv_amin /home/listserv/spool %s-signoff-request\"\r\n\
%s-check-subscription:  \"|del\"\r\n", x, x, 
	  /*owner */ x, x,
	  /* -request */ x, x,
	  /* -search-request*/ x, x,
	  /* -server */ x, x,
	  /* -unsubscribe-request */ x, x,
	  /* -signoff-request */ x, x,
	  /* -check-subscription */ x);
  //subscribe request
  char **kw = listserv_getlist_keyword(l, listname, "Subscription");
  if (strcasecmp("Closed", kw[0]))
    fprintf(aliases, "%s-subscribe-request: \"|lsv_amin /home/listserv/spool %s-subscribe-request\"\r\n", x, x);
  fprintf(aliases, "\r\n");
  free(listname);
}

//called on start up (and when a list is deleted)
int generate_aliases_listserv(struct listserv *l) {
  FILE* aliases = fopen("/home/listserv/aliases.listserv", "w");
  fprintf(aliases, "# aliases for listserv\r\n\
#\r\n\
# These aliases are incorporated into the alias database\r\n\
# every morning by the cron script in /root.\r\n\
# If the listserv administrator creates a list, the\r\n\
# corresponding aliases have to be included in this file.\r\n\
#\r\n\
# Stefan Recksiegel\r\n\
\r\n\
# removed 20010419 - DZ\r\n\
# listserv_abuse: calin.haiduc@aegee.org, skluesen@IX.URZ.UNI-HEIDELBERG.DE, arthur, klitzing@Pool.Informatik.RWTH-Aachen.DE, daniel, hauk@f\mi.uni-passau.de\r\n\
\r\n\
listserv: \"|lsv_amin /home/listserv/spool -t listserv\", didopalauzov+AEGEE.IT.Listserv\r\n\
#listserv-request: \"|lsv_amin /home/listserv/spool -t listserv-request\"
listserv-request: mail\r\n\
owner-listserv: \"|lsv_amin /home/listserv/spool -t owner-listserv\\r\n\"
\r\n");
  char **kw = listserv_owned_lists(l);
  int i;
  for (i = 0; kw[i]; i++) 
    add_aliases(l, kw[i], aliases);
  fclose(aliases);
}
