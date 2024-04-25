#include "gw-process-filter.h"
#include <gio/gio.h>

struct _GwProcessFilter
{
    GObject parent_instance;

    GSubprocess *subprocess;
    GDataOutputStream *stdin;
    GDataInputStream *stdout;
};

G_DEFINE_TYPE(GwProcessFilter, gw_process_filter, G_TYPE_OBJECT)

static void gw_process_filter_close(GwProcessFilter *self)
{
    g_clear_object(&self->stdin);
    g_clear_object(&self->stdout);
    g_clear_object(&self->subprocess);
}

static void gw_process_filter_dispose(GObject *object)
{
    GwProcessFilter *self = GW_PROCESS_FILTER(object);

    gw_process_filter_close(self);

    G_OBJECT_CLASS(gw_process_filter_parent_class)->dispose(object);
}

static void gw_process_filter_class_init(GwProcessFilterClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = gw_process_filter_dispose;
}

static void gw_process_filter_init(GwProcessFilter *self)
{
    (void)self;
}

GwProcessFilter *gw_process_filter_new(const gchar *executable, const gchar *arg, GError **error)
{
    g_return_val_if_fail(executable != NULL, NULL);
    g_return_val_if_fail(error == NULL || *error == NULL, NULL);

    GSubprocessFlags flags = G_SUBPROCESS_FLAGS_STDIN_PIPE | G_SUBPROCESS_FLAGS_STDOUT_PIPE |
                             G_SUBPROCESS_FLAGS_STDERR_SILENCE;
    GSubprocess *subprocess;
    if (arg != NULL) {
        subprocess = g_subprocess_new(flags, error, executable, arg, NULL);
    } else {
        subprocess = g_subprocess_new(flags, error, executable, NULL);
    }
    if (subprocess == NULL) {
        return NULL;
    }

    GwProcessFilter *self = g_object_new(GW_TYPE_PROCESS_FILTER, NULL);
    self->subprocess = subprocess;

    self->stdin = g_data_output_stream_new(g_subprocess_get_stdin_pipe(self->subprocess));
    self->stdout = g_data_input_stream_new(g_subprocess_get_stdout_pipe(self->subprocess));

    return self;
}

gchar *gw_process_filter_do_filter(GwProcessFilter *self, const gchar *input, GError **error)
{
    if (!g_data_output_stream_put_string(self->stdin, input, NULL, error)) {
        return NULL;
    }
    if (!g_data_output_stream_put_string(self->stdin, "\n", NULL, error)) {
        return NULL;
    }
    if (!g_output_stream_flush(G_OUTPUT_STREAM(self->stdin), NULL, error)) {
        return NULL;
    }

    return g_data_input_stream_read_line(self->stdout, NULL, NULL, error);
}

gchar *gw_process_filter_filter(GwProcessFilter *self, const gchar *input, GError **error)
{
    g_return_val_if_fail(GW_IS_PROCESS_FILTER(self), NULL);
    g_return_val_if_fail(input != NULL, NULL);
    g_return_val_if_fail(error == NULL || *error == NULL, NULL);

    gchar *output = NULL;
    if (self->subprocess != NULL) {
        output = gw_process_filter_do_filter(self, input, error);

        if (output == NULL) {
            gw_process_filter_close(self);
        }
    }

    // Return the unmodified input in case an error occurred.
    if (output == NULL) {
        output = g_strdup(input);
    }

    return output;
}