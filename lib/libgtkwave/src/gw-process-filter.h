#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define GW_TYPE_PROCESS_FILTER (gw_process_filter_get_type())
G_DECLARE_FINAL_TYPE(GwProcessFilter, gw_process_filter, GW, PROCESS_FILTER, GObject)

GwProcessFilter *gw_process_filter_new(const gchar *executable, const gchar *arg, GError **error);

gchar *gw_process_filter_filter(GwProcessFilter *self, const gchar *input, GError **error);

G_END_DECLS