/*
 *      build.c - this file is part of Geany, a fast and lightweight IDE
 *
 *      Copyright 2006 Enrico Troeger <enrico.troeger@uvena.de>
 *
 *      This program is free software; you can redistribute it and/or modify
 *      it under the terms of the GNU General Public License as published by
 *      the Free Software Foundation; either version 2 of the License, or
 *      (at your option) any later version.
 *
 *      This program is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *      GNU General Public License for more details.
 *
 *      You should have received a copy of the GNU General Public License
 *      along with this program; if not, write to the Free Software
 *      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $Id$
 */


#include "build.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#ifdef G_OS_UNIX
# include <sys/types.h>
# include <sys/wait.h>
#endif

#include "support.h"
#include "utils.h"
#include "dialogs.h"
#include "msgwindow.h"
#include "document.h"


static GIOChannel *build_set_up_io_channel (gint fd, GIOCondition cond, GIOFunc func, gpointer data);
static gboolean build_iofunc(GIOChannel *ioc, GIOCondition cond, gpointer data);
static gboolean build_create_shellscript(const gint idx, const gchar *fname, const gchar *cmd);
static GPid build_spawn_cmd(gint idx, gchar **cmd);



GPid build_compile_tex_file(gint idx, gint mode)
{
	gchar **argv;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;
	
	argv = g_new0(gchar*, 2);
	argv[0] = (mode == 0) ? g_strdup(doc_list[idx].file_type->programs->compiler) : 
							g_strdup(doc_list[idx].file_type->programs->linker);
	argv[1] = NULL;

	return build_spawn_cmd(idx, argv);
}


GPid build_view_tex_file(gint idx, gint mode)
{
	gchar **argv;
	gchar  *executable = NULL;
	gchar  *view_file = NULL;
	gchar  *locale_filename = NULL;
	gchar  *cmd_string = NULL;
	gchar  *locale_cmd_string = NULL;
	GError *error = NULL;
	GPid 	child_pid;
	struct stat st;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;

	executable = utils_remove_ext_from_filename(doc_list[idx].file_name);
	view_file = g_strconcat(executable, (mode == 0) ? ".dvi" : ".pdf", NULL);

	// try convert in locale for stat()
	locale_filename = g_locale_from_utf8(view_file, -1, NULL, NULL, NULL);
	if (locale_filename == NULL) locale_filename = g_strdup(view_file);

	// check wether view_file exists
	if (stat(locale_filename, &st) != 0)
	{
		msgwin_status_add(_("Failed to view %s (make sure it is already compiled)"), view_file);
		g_free(executable);
		g_free(view_file);
		g_free(locale_filename);
		return (GPid) 1;
	}

	// replace %f and %e in the run_cmd string
	cmd_string = g_strdup((mode == 0) ?	g_strdup(doc_list[idx].file_type->programs->run_cmd) : 
										g_strdup(doc_list[idx].file_type->programs->run_cmd2));
	cmd_string = utils_str_replace(cmd_string, "%f", view_file);
	cmd_string = utils_str_replace(cmd_string, "%e", executable);
	
	// try convert in locale
	locale_cmd_string = g_locale_from_utf8(cmd_string, -1, NULL, NULL, NULL);
	if (locale_cmd_string == NULL) locale_cmd_string = g_strdup(view_file);

	argv = g_new0(gchar *, 4);
	argv[0] = g_strdup("/bin/sh");
	argv[1] = g_strdup("-c");
	argv[2] = locale_cmd_string;
	argv[3] = NULL;

	if (! g_spawn_async_with_pipes(NULL, argv, NULL, G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
						NULL, NULL, &child_pid, NULL, NULL, NULL, &error))
	{
		geany_debug("g_spawn_async_with_pipes() failed: %s", error->message);
		msgwin_status_add(_("Process failed (%s)"), error->message);

		g_free(view_file);
		g_free(executable);
		g_free(locale_filename);
		g_free(cmd_string);
		g_strfreev(argv);
		g_error_free(error);
		error = NULL;
		return (GPid) 0;
	}

	g_free(view_file);
	g_free(executable);
	g_free(locale_filename);
	g_free(cmd_string);
	g_strfreev(argv);

	return child_pid;
}


GPid build_make_file(gint idx, gboolean cust_target)
{
	gchar **argv;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;
	
	argv = g_new0(gchar*, 3);
	if (cust_target && app->build_make_custopt)
	{	//cust-target
		argv[0] = g_strdup(app->build_make_cmd);
		argv[1] = g_strdup(app->build_make_custopt);
		argv[2] = NULL;
	}
	else
	{
		argv[0] = g_strdup(app->build_make_cmd);
		argv[1] = g_strdup("all");
		argv[2] = NULL;
	}

	return build_spawn_cmd(idx, argv);
}


GPid build_compile_file(gint idx)
{
	gchar **argv;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;

	argv = g_new0(gchar *, 2);
	argv[0] = g_strdup(doc_list[idx].file_type->programs->compiler);
	argv[1] = NULL;

	return build_spawn_cmd(idx, argv);
}


GPid build_link_file(gint idx)
{
	gchar **argv;
	gchar *executable = NULL;
	gchar *object_file, *locale_filename;
	struct stat st, st2;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;

	locale_filename = g_locale_from_utf8(doc_list[idx].file_name, -1, NULL, NULL, NULL);
	if (locale_filename == NULL) locale_filename = g_strdup(doc_list[idx].file_name);

	executable = utils_remove_ext_from_filename(locale_filename);
	object_file = g_strdup_printf("%s.o", executable);

	// check wether object file (file.o) exists
	if (stat(object_file, &st) == 0)
	{	// check wether src is newer than object file
		if (stat(locale_filename, &st2) == 0)
		{
			if (st2.st_mtime > st.st_mtime)
			{
				// set object_file to NULL, so the source file will be used for linking,
				// more precisely then we compile and link instead of just linking
				g_free(object_file);
				object_file = NULL;
			}
		}
		else
		{
			dialogs_show_error("Something very strange is occured, could not stat %s (%s)",
					doc_list[idx].file_name, strerror(errno));
		}
	}

	argv = g_new0(gchar *, 4);
	argv[0] = g_strdup(doc_list[idx].file_type->programs->linker);
	argv[1] = g_strdup("-o");
	argv[2] = g_path_get_basename(executable);
	argv[3] = NULL;

	g_free(executable);
	g_free(object_file);
	g_free(locale_filename);

	return build_spawn_cmd(idx, argv);
}


static GPid build_spawn_cmd(gint idx, gchar **cmd)
{
	GError  *error = NULL;
	gchar **argv;
	gchar	*working_dir;
	gchar	*utf8_working_dir;
	gchar	*cmd_string;
	gchar	*utf8_cmd_string;
	gchar	*locale_filename;
	gchar	*executable;
	gchar	*tmp;
	GPid     child_pid;
	gint     stdout_fd;
	gint     stderr_fd;

	app->cur_idx = idx;
	cmd_string = g_strjoinv(" ", cmd);
	g_strfreev(cmd);

	locale_filename = g_locale_from_utf8(doc_list[idx].file_name, -1, NULL, NULL, NULL);
	if (locale_filename == NULL) locale_filename = g_strdup(doc_list[idx].file_name);

	executable = utils_remove_ext_from_filename(locale_filename);
	
	// replace %f and %e in the command string
	tmp = g_path_get_basename(locale_filename);
	cmd_string = utils_str_replace(cmd_string, "%f", tmp);
	g_free(tmp);
	tmp = g_path_get_basename(executable);
	cmd_string = utils_str_replace(cmd_string, "%e", tmp);
	g_free(tmp);
	g_free(executable);
	
	utf8_cmd_string = g_locale_to_utf8(cmd_string, -1, NULL, NULL, NULL);
	if (utf8_cmd_string == NULL) utf8_cmd_string = g_strdup(cmd_string);

	argv = g_new0(gchar *, 4);
	argv[0] = g_strdup("/bin/sh");
	argv[1] = g_strdup("-c");
	argv[2] = cmd_string;
	argv[3] = NULL;

	working_dir = g_path_get_dirname(locale_filename);
	utf8_working_dir = g_path_get_dirname(doc_list[idx].file_name);
	gtk_list_store_clear(msgwindow.store_compiler);
	msgwin_compiler_add(COLOR_BLUE, FALSE, _("%s (in directory: %s)"), utf8_cmd_string, utf8_working_dir);
	gtk_notebook_set_current_page(GTK_NOTEBOOK(msgwindow.notebook), MSG_COMPILER);

	if (! g_spawn_async_with_pipes(working_dir, argv, NULL, G_SPAWN_SEARCH_PATH | G_SPAWN_DO_NOT_REAP_CHILD,
						NULL, NULL, &child_pid, NULL, &stdout_fd, &stderr_fd, &error))
	{
		geany_debug("g_spawn_async_with_pipes() failed: %s", error->message);
		msgwin_status_add(_("Process failed (%s)"), error->message);
		g_strfreev(argv);
		g_error_free(error);
		g_free(working_dir);
		g_free(utf8_working_dir);
		g_free(utf8_cmd_string);
		g_free(locale_filename);
		error = NULL;
		return (GPid) 0;
	}

	// use GIOChannels to monitor stdout and stderr
	build_set_up_io_channel(stdout_fd, G_IO_IN|G_IO_PRI|G_IO_ERR|G_IO_HUP|G_IO_NVAL, build_iofunc, GINT_TO_POINTER(0));
	build_set_up_io_channel(stderr_fd, G_IO_IN|G_IO_PRI|G_IO_ERR|G_IO_HUP|G_IO_NVAL, build_iofunc, GINT_TO_POINTER(1));

	g_strfreev(argv);
	g_free(utf8_working_dir);
	g_free(utf8_cmd_string);
	g_free(working_dir);
	g_free(locale_filename);

	return child_pid;
}


GPid build_run_cmd(gint idx)
{
	GPid	 child_pid;
	GPid	 result_id;	// either child_pid or error id.
	GError	*error = NULL;
	gchar  **argv = NULL;
	gchar  **term_argv = NULL;
	gchar	*working_dir = NULL;
	gchar	*long_executable = NULL;
	gchar	*check_executable = NULL;
	gchar	*utf8_check_executable = NULL;
	gchar	*locale_filename = NULL;
	gchar	*locale_term_cmd = NULL;
	gchar	*cmd = NULL;
	gchar	*tmp = NULL;
	gchar	*executable = NULL;
	gchar	*script_name;
	guint    term_argv_len, i;
	struct stat st;

	if (idx < 0 || doc_list[idx].file_name == NULL) return (GPid) 1;
	
	script_name = g_strdup("./geany_run_script.sh");

	locale_filename = g_locale_from_utf8(doc_list[idx].file_name, -1, NULL, NULL, NULL);
	if (locale_filename == NULL) locale_filename = g_strdup(doc_list[idx].file_name);

	locale_term_cmd = g_locale_from_utf8(app->build_term_cmd, -1, NULL, NULL, NULL);
	if (locale_term_cmd == NULL) locale_term_cmd = g_strdup(app->build_term_cmd);
	// split the term_cmd, so arguments will work too
	term_argv = g_strsplit(locale_term_cmd, " ", -1);
	term_argv_len = g_strv_length(term_argv);
	
	long_executable = utils_remove_ext_from_filename(locale_filename);

	// only check for existing executable, if executable is required by %e
	if (strstr(doc_list[idx].file_type->programs->run_cmd, "%e") != NULL)
	{
		// add .class extension for JAVA source files (only for stat)
		if (doc_list[idx].file_type->id == GEANY_FILETYPES_JAVA)
			check_executable = g_strconcat(long_executable, ".class", NULL);
		else
			check_executable = g_strdup(long_executable);
			
		// check whether executable exists
		if (stat(check_executable, &st) != 0)
		{
			utf8_check_executable = utils_remove_ext_from_filename(doc_list[idx].file_name);
			msgwin_status_add(_("Failed to execute %s (make sure it is already built)"),
														utf8_check_executable);
			result_id = (GPid) 1;
			goto free_strings;
		}
	}
	

	// check if terminal path is set (to prevent misleading error messages)
	if (stat(term_argv[0], &st) != 0)
	{
		msgwin_status_add(
			_("Could not find terminal '%s' "
				"(check path for Terminal tool setting in Preferences)"), app->build_term_cmd);
		result_id = (GPid) 1;
		goto free_strings;
	}

	executable = g_path_get_basename(long_executable);

	working_dir = g_path_get_dirname(locale_filename);
	if (chdir(working_dir) != 0)
	{
		gchar *utf8_working_dir = NULL;
		utf8_working_dir = g_locale_to_utf8(working_dir, -1, NULL, NULL, NULL);
		if (utf8_working_dir == NULL) utf8_working_dir = g_strdup(working_dir);
		
		msgwin_status_add(_("Failed to change the working directory to %s"), working_dir);
		result_id = (GPid) 1;	// return 1, to prevent error handling of the caller
		g_free(utf8_working_dir);
		goto free_strings;
	}
	
	// replace %f and %e in the run_cmd string
	cmd = g_strdup(doc_list[idx].file_type->programs->run_cmd);
	tmp = g_path_get_basename(locale_filename);
	cmd = utils_str_replace(cmd, "%f", tmp);
	g_free(tmp);
	cmd = utils_str_replace(cmd, "%e", executable);

	// write a little shellscript to call the executable (similar to anjuta_launcher but "internal")
	// (script_name should be ok in UTF8 without converting in locale because it contains no umlauts)
	if (! build_create_shellscript(idx, script_name, cmd))
	{
		utf8_check_executable = utils_remove_ext_from_filename(doc_list[idx].file_name);
		msgwin_status_add(_("Failed to execute %s (start-script could not be created)"),
													utf8_check_executable);
		result_id = (GPid) 1;
		goto free_strings;
	}

	argv = g_new0(gchar *, term_argv_len + 3);
	for (i = 0; i < term_argv_len; i++)
	{
		argv[i] = g_strdup(term_argv[i]);
	}
	argv[term_argv_len   ] = g_strdup("-e");
	argv[term_argv_len + 1] = g_strdup(script_name);
	argv[term_argv_len + 2] = NULL;

	if (! g_spawn_async_with_pipes(working_dir, argv, NULL, G_SPAWN_SEARCH_PATH,
						NULL, NULL, &child_pid, NULL, NULL, NULL, &error))
	{
		geany_debug("g_spawn_async_with_pipes() failed: %s", error->message);
		msgwin_status_add(_("Process failed (%s)"), error->message);
		unlink(script_name);
		g_error_free(error);
		error = NULL;
		result_id = (GPid) 0;
		goto free_strings;
	}

	result_id = child_pid; // g_spawn was successful, result is child process id

	free_strings:
	/* free all non-NULL strings */
	g_strfreev(argv);
	g_strfreev(term_argv);
	g_free(working_dir);
	g_free(cmd);
	g_free(utf8_check_executable);
	g_free(locale_filename);
	g_free(locale_term_cmd);
	g_free(check_executable);
	g_free(long_executable);
	g_free(executable);
	g_free(script_name);

	return result_id;
}


static gboolean build_iofunc(GIOChannel *ioc, GIOCondition cond, gpointer data)
{
	if (cond & (G_IO_IN | G_IO_PRI))
	{
		//GIOStatus s;
		gchar *msg;
		guint x = 1;
		gint line, idx;

		while (g_io_channel_read_line(ioc, &msg, NULL, NULL, NULL) && msg)
		{
			//if (s != G_IO_STATUS_NORMAL && s != G_IO_STATUS_EOF) break;
			if (GPOINTER_TO_INT(data))
			{
				msgwin_compiler_add(COLOR_RED, FALSE, g_strstrip(msg));
				if (app->pref_editor_use_indicators)
				{
					utils_parse_compiler_error_line(g_strstrip(msg), &idx, &line);
					if (line != -1) document_set_indicator(idx, line - 1);				
				}
			}
			else
				msgwin_compiler_add(COLOR_BLACK, FALSE, g_strstrip(msg));

			x++;
			g_free(msg);
		}
	}
	if (cond & (G_IO_ERR | G_IO_HUP | G_IO_NVAL))
		return FALSE;

	return TRUE;
}


static GIOChannel *build_set_up_io_channel(gint fd, GIOCondition cond, GIOFunc func, gpointer data)
{
	GIOChannel *ioc;
	GError *error = NULL;
	const gchar *encoding;

	ioc = g_io_channel_unix_new(fd);

	g_io_channel_set_flags(ioc, G_IO_FLAG_NONBLOCK, NULL);
	if (! g_get_charset(&encoding))
	{	// hope this works reliably
		g_io_channel_set_encoding(ioc, encoding, &error);
		if (error)
		{
			geany_debug("compile: %s", error->message);
			g_error_free(error);
			return ioc;
		}
	}
	// "auto-close" ;-)
	g_io_channel_set_close_on_unref(ioc, TRUE);

	g_io_add_watch(ioc, cond, func, data);
	g_io_channel_unref(ioc);

	return ioc;
}


void build_exit_cb(GPid child_pid, gint status, gpointer user_data)
{
#ifdef G_OS_UNIX
	gboolean failure = FALSE;

	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) != EXIT_SUCCESS)
			failure = TRUE;
	}
	else if (WIFSIGNALED(status))
	{
		// the terminating signal: WTERMSIG (status));
		failure = TRUE;
	}
	else
	{	// any other failure occured
		failure = TRUE;
	}


	if (failure)
	{
		msgwin_compiler_add(COLOR_BLUE, TRUE, _("compilation finished unsuccessful"));
	}
	else
	{
		msgwin_compiler_add(COLOR_BLUE, TRUE, _("compilation finished successful"));
	}

#endif
	utils_beep();
	gtk_widget_set_sensitive(app->compile_button, TRUE);
	g_spawn_close_pid(child_pid);
}


static gboolean build_create_shellscript(const gint idx, const gchar *fname, const gchar *cmd)
{
	FILE *fp;
	gint i;
	gchar *str, *exec, **tmp_args = NULL, *tmp;

	fp = fopen(fname, "w");
	if (! fp) return FALSE;

	// enclose all args in ""
	tmp_args = g_strsplit(cmd, " ", -1);
	for (i = 0; ; i++)
	{
		if (tmp_args[i] == NULL) break;
		tmp = g_strdup(tmp_args[i]);
		g_free(tmp_args[i]);
		tmp_args[i] = g_strconcat("\"", tmp, "\"", NULL);
		g_free(tmp);
	}
	exec = g_strjoinv(" ", tmp_args);

	str = g_strdup_printf(
		"#!/bin/sh\n\n%s\n\necho \"\n\n------------------\n(program exited with code: $?)\" \
		\n\necho \"Press return to continue\"\nread\nunlink $0\n", exec);
	fputs(str, fp);
	g_free(str);
	g_free(exec);
	g_strfreev(tmp_args);

	if (chmod(fname, 0700) != 0)
	{
		unlink(fname);
		return FALSE;
	}
	fclose(fp);

	return TRUE;
}


