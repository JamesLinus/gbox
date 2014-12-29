/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */ 
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the main item
#define GB_DEMO_MAIN_ITEM(name)         { #name, gb_demo_##name##_main }

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the demo type
typedef struct __gb_demo_t
{
    // the demo name
    tb_char_t const*    name;

    // the demo main
    tb_int_t            (*main)(tb_int_t argc, tb_char_t** argv);

}gb_demo_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the demos
static gb_demo_t g_demo[] = 
{
    // core
    GB_DEMO_MAIN_ITEM(core_path)
,   GB_DEMO_MAIN_ITEM(core_bitmap)
,   GB_DEMO_MAIN_ITEM(core_vector)

    // mesh
,   GB_DEMO_MAIN_ITEM(base_mesh)

    // ohter
,   GB_DEMO_MAIN_ITEM(other_test)
};

/* //////////////////////////////////////////////////////////////////////////////////////
 * main
 */
tb_int_t main(tb_int_t argc, tb_char_t** argv)
{
    // init tbox
#if 0
    if (!tb_init(tb_null, (tb_byte_t*)malloc(300 * 1024 * 1024), 300 * 1024 * 1024)) return 0;
#else
    if (!tb_init(tb_null, tb_null, 0)) return 0;
#endif

    // init gbox
    if (!gb_init()) return 0;

    // init 
    tb_int_t            ok = 0;
    tb_char_t const*    name = tb_null;

    // find the main func from the .demo file
    if (!name)
    {
        // init file
        tb_file_ref_t file = tb_file_init(".demo", TB_FILE_MODE_RO);
        if (file)
        {
            // read line
            tb_char_t line[8192] = {0};
            if (tb_file_read(file, (tb_byte_t*)line, sizeof(line) - 1))
            {
                tb_size_t i = 0;
                tb_size_t n = tb_arrayn(g_demo);
                for (i = 0; i < n; i++)
                {
                    // find it?
                    if (g_demo[i].name && !tb_strnicmp(g_demo[i].name, line, tb_strlen(g_demo[i].name)))
                    {
                        // save name
                        name = g_demo[i].name;

                        // done main
                        ok = g_demo[i].main(argc, argv);
                        break;
                    }
                }
            }

            // exit file
            tb_file_exit(file);
        }
    }

    // find the main func from the first argument
    if (!name && argc > 1 && argv[1])
    {
        tb_size_t i = 0;
        tb_size_t n = tb_arrayn(g_demo);
        for (i = 0; i < n; i++)
        {
            // find it?
            if (g_demo[i].name && !tb_stricmp(g_demo[i].name, argv[1]))
            {
                // save name
                name = g_demo[i].name;

                // done main
                ok = g_demo[i].main(argc - 1, argv + 1);
                break;
            }
        }
    }

    // no this demo? help it
    if (!name)
    {
        tb_trace_i("======================================================================");
        tb_trace_i("help: echo \"name\" > ./.demo");
        tb_trace_i("help:     ./demo.b args ...");
        tb_trace_i("help: or");
        tb_trace_i("help: ./demo.b name args ...");
        tb_trace_i("help: ");
        tb_trace_i("help: example: echo \"stream\" > ./.demo");
        tb_trace_i("help: example:     ./demo.b --help");
        tb_trace_i("help: example:     ./demo.b http://www.xxxxx.com /tmp/a");
        tb_trace_i("help: example: or");
        tb_trace_i("help: example: ./demo.b stream http://www.xxxxx.com /tmp/a");
        tb_trace_i("help: ");

        // walk name
        tb_size_t i = 0;
        tb_size_t n = tb_arrayn(g_demo);
        for (i = 0; i < n; i++) tb_trace_i("help: name: %s", g_demo[i].name);
    }

    // exit gbox
    gb_exit();

    // exit tbox
    tb_exit();

    // ok?
    return ok;
}
