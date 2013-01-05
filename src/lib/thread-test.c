/*
 * thread-test -- thread and related synchronization tools unit tests.
 *
 * Copyright (c) 2012 Raphael Manfredi <Raphael_Manfredi@pobox.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "common.h"

#include "atomic.h"
#include "compat_poll.h"
#include "compat_sleep_ms.h"
#include "cond.h"
#include "cq.h"
#include "log.h"
#include "misc.h"
#include "mutex.h"
#include "parse.h"
#include "path.h"
#include "semaphore.h"
#include "str.h"
#include "thread.h"
#include "tm.h"
#include "waiter.h"
#include "xmalloc.h"

const char *progname;

static bool sleep_before_exit;
static bool async_exit;
static unsigned cond_timeout;

static void G_GNUC_NORETURN
usage(void)
{
	fprintf(stderr,
		"Usage: %s [-hejsACIMNS] [-n count] [-t ms] [-T secs]"
		"  -h : prints this help message\n"
		"  -e : use emulated semaphores\n"
		"  -j : join created threads\n"
		"  -n : amount of times to repeat tests\n"
		"  -s : let each created thread sleep for 1 second before ending\n"
		"  -t : timeout value (ms) for condition waits\n"
		"  -A : use asynchronous exit callbacks\n"
		"  -C : test thread creation\n"
		"  -I : test inter-thread waiter signaling\n"
		"  -M : monitors tennis match via waiters\n"
		"  -N : add broadcast noise during tennis session\n"
		"  -S : test semaphore layer\n"
		"  -T : test condition layer via tennis session for specified secs\n"
		"Values given as decimal, hexadecimal (0x), octal (0) or binary (0b)\n"
		, progname);
	exit(EXIT_FAILURE);
}

static char *names[] = { "one", "two", "three", "four", "five" };

static void
exit_callback(void *result, void *arg)
{
	char *name = arg;
	ulong length = pointer_to_ulong(result);
	printf("thread \"%s\" finished, result length is %lu\n", name, length);
}

static void *
compute_length(void *arg)
{
	unsigned stid = thread_small_id();
	const char *name = arg;
	char *scratch = xstrdup(name);

	printf("%s given \"%s\"\n", thread_id_name(stid), scratch);
	xfree(scratch);
	if (sleep_before_exit)
		sleep(1);
	return ulong_to_pointer(strlen(name));
}

static void
test_create_one(bool repeat, bool join)
{
	unsigned i;
	int launched[G_N_ELEMENTS(names)];

	for (i = 0; i < G_N_ELEMENTS(names); i++) {
		int r;
		int flags = 0;

		if (!join)
			flags |= THREAD_F_DETACH;

		if (async_exit)
			flags |= THREAD_F_ASYNC_EXIT;

		r = thread_create_full(compute_length, names[i], flags,
				16384, exit_callback, names[i]);

		launched[i] = r;

		if (-1 == r) {
			if (errno != EAGAIN || !repeat)
				s_warning("cannot create thread #%u: %m", i);
		} else {
			int j;
			if (!repeat)
				printf("thread i=%u created as %s\n", i, thread_id_name(r));
			if (!join) {
				j = thread_join(r, NULL, FALSE);
				if (-1 != j) {
					s_warning("thread_join() worked for %s?\n",
						thread_id_name(r));
				} else if (errno != EINVAL) {
					s_warning("thread_join() failure on %s: %m",
						thread_id_name(r));
				} else {
					if (!repeat) {
						printf("%s cannot be joined, that's OK\n",
							thread_id_name(r));
					}
				}
			}
		}
	}

	if (!repeat && !join)
		compat_sleep_ms(200);		/* Let all the threads run */

	if (join) {
		printf("now joining the %u threads\n", (uint) G_N_ELEMENTS(launched));
		for (i = 0; i < G_N_ELEMENTS(launched); i++) {
			int r = launched[i];

			if (-1 == r) {
				if (!repeat)
					printf("skipping unlaunched thread i=%u\n", i);
			} else {
				void *result;
				int j = thread_join(r, &result, FALSE);		/* Block */
				if (-1 == j) {
					s_warning("thread_join() failed for %s: %m",
						thread_id_name(r));
				} else {
					ulong length = pointer_to_ulong(result);
					if (!repeat) {
						printf("%s finished, result length is %lu\n",
							thread_id_name(r), length);
					}
				}
			}
		}
	}

	if (async_exit)
		cq_dispatch();
}

static void
test_create(unsigned repeat, bool join)
{
	unsigned i;

	for (i = 0; i < repeat; i++) {
		test_create_one(repeat > 1, join);
	}
}

static void *
test_inter_main(void *arg)
{
	waiter_t *w = waiter_refcnt_inc(arg);

	sleep(1);
	printf("signaling main thread\n");
	fflush(stdout);
	waiter_signal(w);
	
	compat_sleep_ms(5);
	waiter_refcnt_dec(arg);
	return NULL;
}

static void
test_inter(void)
{
	int r;
	waiter_t *mw, *w;
	bool refed;

	mw = waiter_make(NULL);
	w = waiter_spawn(mw, int_to_pointer(31416));
	
	r = thread_create(test_inter_main, w, THREAD_F_DETACH, 0);
	if (-1 == r)
		s_error("could not launch thread: %m");

	printf("main thread waiting\n");
	fflush(stdout);
	if (!waiter_suspend(mw))
		s_error("could not suspend itself");
	printf("main thread awoken\n");

	refed = waiter_refcnt_dec(w);
	printf("child waiter %s referenced\n", refed ? "still" : "no longer");
	while (waiter_child_count(mw) != 1) {
		printf("waiting for all children in waiter to go\n");
		fflush(stdout);
		compat_sleep_ms(5);
	}
	refed = waiter_refcnt_dec(mw);
	printf("master waiter %s referenced\n", refed ? "still" : "no longer");
}

struct test_semaphore_arg {
	int n;			/* thread number */
	semaphore_t *s;	/* semaphore to use */
};

static void *
test_semaphore_main(void *arg)
{
	struct test_semaphore_arg *ta = arg;
	int i;
	int n = ta->n;
	semaphore_t *s = ta->s;

	xfree(ta);

	for (i = 0; i < 6; i++) {
		bool got = TRUE;
		printf("thread #%d alive, waiting for event\n", n);
		fflush(stdout);
		if (semaphore_acquire(s, 1, NULL)) {
			printf("#%d got semaphore!\n", n);
			fflush(stdout);
		} else {
			s_warning("thread #%d cannot get semaphore: %m", n);
			got = FALSE;
		}
		sleep(1);
		if (got) {
			printf("#%d releasing semaphore!\n", n);
			fflush(stdout);
			semaphore_release(s, 1);
		}
	}

	printf("thread #%d exiting!\n", n);
	return NULL;
}

static int
test_semaphore_thread_launch(int n, semaphore_t *s)
{
	struct test_semaphore_arg *arg;
	int r;

	XMALLOC(arg);
	arg->n = n;
	arg->s = s;
	r = thread_create(test_semaphore_main, arg, 0, 0);
	if (-1 == r)
		s_error("could not launch thread #%d: %m", n);

	return r;
}

static void
test_semaphore(bool emulated)
{
	semaphore_t *s;
	tm_t timeout;
	int i;
	int r[3];

	s = semaphore_create_full(0, emulated);
	if (semaphore_acquire_try(s, 1))
		s_error("could acquire empty semaphore!");
	semaphore_release(s, 2);

	tm_fill_ms(&timeout, 1000);

	if (!semaphore_acquire(s, 1, &timeout))
		s_error("could not acquire semaphore, first time!");
	if (!semaphore_acquire(s, 1, &timeout))
		s_error("could not acquire semaphore, second time!");

	tm_fill_ms(&timeout, 500);
	printf("will wait 1/2 second...\n");
	if (semaphore_acquire(s, 1, &timeout))
		s_error("could acquire empty semaphore!");
	g_assert_log(EAGAIN == errno,
		"improper errno, expected EAGAIN (%d) but got %m", EAGAIN);
	printf("good, failed to acquire empty semaphore.\n");

	r[0] = test_semaphore_thread_launch(1, s);
	r[1] = test_semaphore_thread_launch(2, s);
	r[2] = test_semaphore_thread_launch(3, s);

	semaphore_release(s, 1);

	for (i = 0; i < 6; i++) {
		printf("main alive\n");
		fflush(stdout);
		sleep(1);
	}

	printf("main waiting for subthreads\n");
	fflush(stdout);

	for (i = 0; UNSIGNED(i) < G_N_ELEMENTS(r); i++) {
		if (-1 == thread_join(r[i], NULL, FALSE))
			s_error("failed to join with %s: %m", thread_id_name(r[i]));
	}

	printf("main is done, final semaphore value is %d\n", semaphore_value(s));

	semaphore_destroy(&s);
}

/*
 * The tennis game is a C port of a C++ demo program posted to
 * comp.programming.threads circa 2001 to test condition waiting
 * implementations and outline bugs in them.
 *
 * When run normally without noise, no spurious wakeups should happen and
 * of course no deadlocks.
 *
 * When broadcast noise is added, there will be spurious wakeups but there
 * should be no deadlocks either.
 */

enum game_state {
	START_GAME,			/* Game starting */
	PLAYER_A,			/* Player A's turn */
	PLAYER_B,			/* Player B's turn */
	GAME_OVER,			/* Game over */
	ONE_PLAYER_GONE,
	BOTH_PLAYER_GONE,
};

static enum game_state game_state;
static mutex_t game_state_lock = MUTEX_INIT;
static cond_t game_state_change = COND_INIT;
static struct game_stats {
	int play;
	int spurious;
	int timeout;
} game_stats[2];
const char *name[] = { "A", "B" };
enum game_state other[] = { PLAYER_B, PLAYER_A };

static void
create_player(thread_main_t start, int n)
{
	g_assert(n >= 0 && n < (int) G_N_ELEMENTS(name));

	if (-1 == thread_create(start, int_to_pointer(n), THREAD_F_DETACH, 8192))
		s_error("cannot launch player %s: %m", name[n]);
}

static void *
player(void *num)
{
	int n;
	enum game_state other_player;
	const char *me;
	struct game_stats *stats;
	tm_t timeout;

	n = pointer_to_int(num);
	me = name[n];
	other_player = other[n];
	stats = &game_stats[n];

	if (cond_timeout != 0)
		tm_fill_ms(&timeout, cond_timeout);

	mutex_lock(&game_state_lock);

	while (game_state < GAME_OVER) {
		stats->play++;
		printf("%s plays\n", me);
		fflush(stdout);

		game_state = other_player;
		cond_signal(&game_state_change, &game_state_lock);

		/* Wait until it's my turn to play again */

		do {
			if (cond_timeout != 0) {
				bool success = cond_timed_wait(
					&game_state_change, &game_state_lock, &timeout);

				if (!success) {
					stats->timeout++;
					printf("** TIMEOUT ** %s wakeup\n", me);
					fflush(stdout);
					continue;
				}
			} else {
				cond_wait(&game_state_change, &game_state_lock);
			}

			if (other_player == game_state) {
				stats->spurious++;
				printf("** SPURIOUS ** %s wakeup\n", me);
				fflush(stdout);
			}
		} while (other_player == game_state);
	}

	game_state++;
	printf("%s leaving\n", me);
	fflush(stdout);

	cond_broadcast(&game_state_change, &game_state_lock);
	mutex_unlock(&game_state_lock);

	return NULL;
}

static void
player_stats(int n)
{
	struct game_stats *stats;

	g_assert(n >= 0 && n < (int) G_N_ELEMENTS(game_stats));

	stats = &game_stats[n];

	printf("%s played %d times (%d spurious event%s, %d timeout%s)\n",
		name[n], stats->play, stats->spurious, 1 == stats->spurious ? "" : "s",
		stats->timeout, 1 == stats->timeout ? "" : "s");
}

static void
test_condition(unsigned play_time, bool emulated, bool monitor, bool noise)
{
	int i;
	game_state = START_GAME;
	waiter_t *w;
	uint notifications = 0;

	g_assert(0 == cond_waiting_count(&game_state_change));

	if (emulated)
		cond_init_full(&game_state_change, &game_state_lock, emulated);

	g_assert(0 == cond_waiting_count(&game_state_change));
	g_assert(0 == cond_pending_count(&game_state_change));
	g_assert(0 == cond_signal_count(&game_state_change));

	for (i = 0; i < (int) G_N_ELEMENTS(name); i++) {
		create_player(player, i);
	}

	if (monitor) {
		struct pollfd wfd[1];
		tm_t end, play;

		tm_now_exact(&end);
		tm_fill_ms(&play, play_time * 1000);
		tm_add(&end, &play);

		w = waiter_make(NULL);
		cond_waiter_add(&game_state_change, w);
		g_assert(2 == waiter_refcnt(w));

		if (!waiter_suspend(w))
			s_warning("cannot suspend myself");
		if (waiter_notified(w)) {
			waiter_ack(w);
			notifications++;
		} else {
			s_warning("waiter should have been notified");
		}

		wfd[0].fd = waiter_fd(w);
		wfd[0].events = POLLIN;

		for (;;) {
			tm_t now;
			int ret;

			tm_now_exact(&now);
			if (tm_elapsed_f(&end, &now) <= 0.0)
				break;

			ret = compat_poll(wfd, G_N_ELEMENTS(wfd), 1000);
			if (ret < 0) {
				s_warning("poll() failed: %m");
			} else {
				printf("[match still on]\n");
				fflush(stdout);
				notifications++;
				waiter_ack(w);
			}
		}

		cond_waiter_remove(&game_state_change, w);
		g_assert(1 == waiter_refcnt(w));
		waiter_refcnt_dec(w);
	} else {
		sleep(play_time);	/* Let them play */
	}

	if (noise) {
		printf("** Noise ON **\n");
		for (i = 0; i < 100000; i++) {
			mutex_lock(&game_state_lock);
			cond_broadcast(&game_state_change, &game_state_lock);
			mutex_unlock(&game_state_lock);
		}
		printf("** Noise OFF **\n");
	}

	mutex_lock(&game_state_lock);
	game_state = GAME_OVER;

	printf("Stopping the game...\n");
	fflush(stdout);

	cond_broadcast(&game_state_change, &game_state_lock);

	do {
		cond_wait(&game_state_change, &game_state_lock);
	} while (game_state < BOTH_PLAYER_GONE);

	g_assert(0 == cond_waiting_count(&game_state_change));

	printf("Game over!\n");

	mutex_unlock(&game_state_lock);
	mutex_destroy(&game_state_lock);
	if (!cond_reset(&game_state_change))
		s_warning("cannot reset condition?");
	cond_destroy(&game_state_change);

	/* Must work also after destruction */

	g_assert(0 == cond_waiting_count(&game_state_change));
	g_assert(0 == cond_pending_count(&game_state_change));

	for (i = 0; i < (int) G_N_ELEMENTS(name); i++) {
		player_stats(i);
	}
	if (monitor) {
		printf("main got %u notification%s\n",
			notifications, 1 == notifications ? "" : "s");
	}
}

static unsigned
get_number(const char *arg, int opt)
{
	int error;
	uint32 val;
	
	val = parse_v32(arg, NULL, &error);
	if (0 == val && error != 0) {
		fprintf(stderr, "%s: invalid -%c argument \"%s\": %s\n",
			progname, opt, arg, g_strerror(error));
		exit(EXIT_FAILURE);
	}

	return val;
}

int
main(int argc, char **argv)
{
	extern int optind;
	extern char *optarg;
	int c;
	bool create = FALSE, join = FALSE, sem = FALSE, emulated = FALSE;
	bool play_tennis = FALSE, monitor = FALSE, noise = FALSE;
	bool inter = FALSE;
	unsigned repeat = 1, play_time = 0;

	mingw_early_init();
	progname = filepath_basename(argv[0]);
	thread_set_main(TRUE);		/* We're the main thread, we can block */

	misc_init();

	while ((c = getopt(argc, argv, "hejn:st:ACIMNST:")) != EOF) {
		switch (c) {
		case 'A':			/* use asynchronous exit callbacks */
			async_exit = TRUE;
			break;
		case 'C':			/* test thread creation */
			create = TRUE;
			break;
		case 'I':			/* test inter-thread signaling */
			inter = TRUE;
			break;
		case 'M':			/* monitor tennis match */
			monitor = TRUE;
			break;
		case 'N':			/* add cond_broadcast() noise */
			noise = TRUE;
			break;
		case 'S':			/* test semaphore layer */
			sem = TRUE;
			break;
		case 'T':			/* test condition layer */
			play_time = get_number(optarg, c);
			play_tennis = TRUE;
			break;
		case 'e':			/* use emulated semaphores */
			emulated = TRUE;
			break;
		case 'j':			/* join threads */
			join = TRUE;
			break;
		case 'n':			/* repeat tests */
			repeat = get_number(optarg, c);
			break;
		case 't':			/* condition wait timeout (0 = none) */
			cond_timeout = get_number(optarg, c);
			break;
		case 's':			/* threads sleep for 1 second before ending */
			sleep_before_exit = TRUE;
			break;
		case 'h':			/* show help */
		default:
			usage();
			break;
		}
	}

	if ((argc -= optind) != 0)
		usage();

	if (!atomic_ops_available())
		s_warning("Atomic memory operations not supported!");

	if (sem)
		test_semaphore(emulated);

	if (play_tennis)
		test_condition(play_time, emulated, monitor, noise);

	if (create)
		test_create(repeat, join);

	if (inter)
		test_inter();

	exit(EXIT_SUCCESS);	/* Required to cleanup semaphores if not destroyed */
}

