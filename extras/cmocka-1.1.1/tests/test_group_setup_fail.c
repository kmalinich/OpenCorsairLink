/* Use the unit test allocators */
#define UNIT_TESTING 1

#include <cmocka.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

static int
group_setup_failing(void** state) {
	(void)state; /* unused */
	return 1; /* To indicate the failure */
}

static void
test_true(void** state) {
	(void)state; /* unused */
	assert_true(1);
}

static void
test_false(void** state) {
	(void)state; /* unused */
	assert_false(0);
}

int
main(void) {
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(test_true),
		cmocka_unit_test(test_false),
	};

	return cmocka_run_group_tests(tests, group_setup_failing, NULL);
}
