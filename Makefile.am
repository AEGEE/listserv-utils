LIBTOOL_DEPS = @LIBTOOL_DEPS@
AM_LDFLAGS=
ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = m4/gnulib-cache.m4
AM_CPPFLAGS = -I src/ -I src/monitor/
AM_CFLAGS = -Wall -Wextra ${glib_CFLAGS} ${liblistserv_CFLAGS}
SUBDIRS	= lib

pkglib_LTLIBRARIES = 
noinst_LTLIBRARIES=
EXTRA_LTLIBRARIES =
lib_LTLIBRARIES = 
MOSTLYCLEANFILES = 
CLEANFILES =
BUILT_SOURCES = 
include_HEADERS = 
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

#info_TEXINFOS = doc/prdr-milter.texi
#TEXINFO_TEX = doc/texinfo.tex
#EXTRA_DIST = doc/prdr-milter.texi.orig

#install-exec-hook: killall -SIGHUP aegee-prdr


bin_PROGRAMS = src/list-upload src/generate-listowners-l src/listserv-nagios src/listserv-monitor
#src/post-listserv-milter 

#src_post_listserv_milter_post_listserv_milter_LDADD = -lpthread -lmilter
#src_post_listserv_milter_post_listserv_milter_CFLAGS = ${glib_CFLAGS} ${liblistserv_CFLAGS}
# src_post_listserv_milter_post_listserv_milter_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}

src_listserv_nagios_SOURCES = src/listserv-nagios.c
src_listserv_nagios_LDFLAGS = ${glib_LIBS} ${liblistserv_LIBS}

src_list_upload_SOURCES = src/list-upload.c
src_list_upload_LDFLAGS =  ${glib_LIBS} ${liblistserv_LIBS}

src_generate_listowners_l_SOURCES = src/generate-listowners-l.c
src_generate_listowners_l_LDFLAGS = ${glib_LIBS} ${liblistserv_LIBS}

src_listserv_monitor_SOURCES = src/monitor/listserv-monitor.c src/monitor/util.h src/monitor/util.c src/monitor/mailing.c src/monitor/print_report.c
src_listserv_monitor_LDFLAGS = ${glib_LIBS} ${liblistserv_LIBS} $(LIBLTDL)
src_listserv_monitor_LDADD =

pkglib_LTLIBRARIES += src/monitor/modules/change_log.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/change_log.la
src_monitor_modules_change_log_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_change_log_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/content_filter.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/content_filter.la
src_monitor_modules_content_filter_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_content_filter_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/double_confidential_no.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/double_confidential_no.la
src_monitor_modules_double_confidential_no_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_double_confidential_no_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/exit.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/exit.la
src_monitor_modules_exit_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_exit_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/invalid_emails.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/invalid_emails.la
src_monitor_modules_invalid_emails_la_LIBADD = -lresolv ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_invalid_emails_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/list_address.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/list_address.la
src_monitor_modules_list_address_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_list_address_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/list_id.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/list_id.la
src_monitor_modules_list_id_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_list_id_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/no_listowners.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/no_listowners.la
src_monitor_modules_no_listowners_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_no_listowners_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/notebook.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/notebook.la
src_monitor_modules_notebook_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_notebook_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/notebook_header.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/notebook_header.la
src_monitor_modules_notebook_header_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_notebook_header_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/one_listowner.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/one_listowner.la
src_monitor_modules_one_listowner_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_one_listowner_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/review.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/review.la
src_monitor_modules_review_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_review_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/send_public_ack_yes.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/send_public_ack_yes.la
src_monitor_modules_send_public_ack_yes_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_send_public_ack_yes_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/send_public_confidential_no.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/send_public_confidential_no.la
src_monitor_modules_send_public_confidential_no_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_send_public_confidential_no_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/send_public_confirm_nomoderator.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/send_public_confirm_nomoderator.la
src_monitor_modules_send_public_confirm_nomoderator_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_send_public_confirm_nomoderator_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/sub_lists.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/sub_lists.la
src_monitor_modules_sub_lists_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_sub_lists_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/subscription.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/subscription.la
src_monitor_modules_subscription_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_subscription_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/subscription_open_review_private.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/subscription_open_review_private.la
src_monitor_modules_subscription_open_review_private_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_subscription_open_review_private_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/never_actively_used.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/never_actively_used.la
src_monitor_modules_never_actively_used_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_never_actively_used_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_one_year.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_one_year.la
src_monitor_modules_unused_for_one_year_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_one_year_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_two_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_two_years.la
src_monitor_modules_unused_for_two_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_two_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_three_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_three_years.la
src_monitor_modules_unused_for_three_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_three_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_four_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_four_years.la
src_monitor_modules_unused_for_four_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_four_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_five_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_five_years.la
src_monitor_modules_unused_for_five_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_five_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_six_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_six_years.la
src_monitor_modules_unused_for_six_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_six_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_for_at_least_seven_years.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_for_at_least_seven_years.la
src_monitor_modules_unused_for_at_least_seven_years_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_for_at_least_seven_years_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/unused_confidential_no.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/unused_confidential_no.la
src_monitor_modules_unused_confidential_no_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_unused_confidential_no_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/confidential_no_subscription_by_owner.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/confidential_no_subscription_by_owner.la
src_monitor_modules_confidential_no_subscription_by_owner_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_confidential_no_subscription_by_owner_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/confidential_no_subscription_closed.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/confidential_no_subscription_closed.la
src_monitor_modules_confidential_no_subscription_closed_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_confidential_no_subscription_closed_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/confidential_no_send_public.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/confidential_no_send_public.la
src_monitor_modules_confidential_no_send_public_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_confidential_no_send_public_la_LDFLAGS = -module -avoid-version -no-undefined -static


pkglib_LTLIBRARIES += src/monitor/modules/possibly_spammed.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/possibly_spammed.la
src_monitor_modules_possibly_spammed_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_possibly_spammed_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/validate.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/validate.la
src_monitor_modules_validate_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_validate_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/web_archives.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/web_archives.la
src_monitor_modules_web_archives_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_web_archives_la_LDFLAGS = -module -avoid-version -no-undefined -static

pkglib_LTLIBRARIES += src/monitor/modules/x_tags.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/x_tags.la
src_monitor_modules_x_tags_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_x_tags_la_LDFLAGS = -module -avoid-version -no-undefined -static




pkglib_LTLIBRARIES += src/monitor/modules/confidential_no_without_listowner.la
src_listserv_monitor_LDADD += -dlpreopen src/monitor/modules/confidential_no_without_listowner.la
src_monitor_modules_confidential_no_without_listowner_la_LIBADD = ${glib_LIBS} ${liblistserv_LIBS}
src_monitor_modules_confidential_no_without_listowner_la_LDFLAGS = -module -avoid-version -no-undefined -static


install-exec-hook:
	rm $(DESTDIR)$(pkglibdir)/*
	rmdir "$(DESTDIR)$(pkglibdir)"


include libltdl/ltdl.mk
#
#install-exec-hook:
#	rm -r ${pkglibdir}
