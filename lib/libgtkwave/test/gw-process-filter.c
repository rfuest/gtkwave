#include <gtkwave.h>

static void test_basic(void) {
    GError *error = NULL;
    GwProcessFilter *filter = gw_process_filter_new("files/filter.py", NULL, &error);
    g_assert_no_error(error);
    g_assert_nonnull(filter);

    gchar *str;

    str = gw_process_filter_filter(filter, "abc", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "filter: 'abc'");
    g_free(str);

    str = gw_process_filter_filter(filter, "0123456789", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "filter: '0123456789'");
    g_free(str);

    str = gw_process_filter_filter(filter, "", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "filter: ''");
    g_free(str);

    g_object_unref(filter);
}

static void test_with_argument(void) {
    GError *error = NULL;
    GwProcessFilter *filter = gw_process_filter_new("files/filter.py", "arg", &error);
    g_assert_no_error(error);
    g_assert_nonnull(filter);

    gchar *str;

    str = gw_process_filter_filter(filter, "abc", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "arg: 'abc'");
    g_free(str);

    str = gw_process_filter_filter(filter, "0123456789", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "arg: '0123456789'");
    g_free(str);

    str = gw_process_filter_filter(filter, "", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "arg: ''");
    g_free(str);

    g_object_unref(filter);
}

static void test_crash(void) {
    GError *error = NULL;
    GwProcessFilter *filter = gw_process_filter_new("files/filter.py", NULL, &error);
    g_assert_no_error(error);
    g_assert_nonnull(filter);

    gchar *str;

    str = gw_process_filter_filter(filter, "1", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "filter: '1'");
    g_free(str);

    str = gw_process_filter_filter(filter, "2", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "filter: '2'");
    g_free(str);

    str = gw_process_filter_filter(filter, "crash", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "crash");
    g_free(str);

    str = gw_process_filter_filter(filter, "3", &error);
    g_assert_no_error(error);
    g_assert_cmpstr(str, ==, "3");
    g_free(str);

    g_object_unref(filter);
}


static void test_error_invalid_executable(void) {
    GError *error = NULL;
    GwProcessFilter *filter = gw_process_filter_new("files/does_not_exist.py", NULL, &error);
    g_assert_error(error, g_spawn_error_quark(), G_SPAWN_ERROR_NOENT);
    g_assert_null(filter);
}

int main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func("/process_filter/basic", test_basic);
    g_test_add_func("/process_filter/with_argument", test_with_argument);
    g_test_add_func("/process_filter/crash", test_crash);
    g_test_add_func("/process_filter/error_invalid_executable", test_error_invalid_executable);

    return g_test_run();
}