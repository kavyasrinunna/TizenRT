/****************************************************************************
 *
 * Copyright 2016 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

/// @file wqueue.c
/// @brief Test Case Example for Work Queue API
/**************************************************************************
*
*   Copyright (C) 2007-2009, 2011 Gregory Nutt. All rights reserved.
*   Author: Gregory Nutt <gnutt@nuttx.org>
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
* 3. Neither the name NuttX nor the names of its contributors may be
*    used to endorse or promote products derived from this software
*    without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
* ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************/

/**************************************************************************
* Included Files
**************************************************************************/
#include <tinyara/config.h>
#include <tinyara/wqueue.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include "tc_internal.h"

/**************************************************************************
* Private Definitions
**************************************************************************/


#if defined(CONFIG_SCHED_WORKQUEUE) || defined(CONFIG_LIB_USRWORK)



/**************************************************************************
* Private Types
**************************************************************************/

/**************************************************************************
* Private Function Prototypes
**************************************************************************/

/**************************************************************************
* Global Variables
**************************************************************************/

/**************************************************************************
* Private Variables
**************************************************************************/

static systime_t start_time;

/**************************************************************************
* Private Functions
**************************************************************************/
static void wq_test1(void *arg)
{
	printf("worker thread 1 \n");
}

static void wq_test2(void *arg)
{
	printf("worker thread 2 \n");
}

static void wq_test3(void *arg)
{
	printf("worker thread 3 \n");
}
/**************************************************************************
* Public Functions
**************************************************************************/
#if defined(CONFIG_SCHED_HPWORK) || defined(CONFIG_SCHED_LPWORK)
static void tc_wqueue_cancel(void)
{
	int result;
	struct work_s *test_work1;
	struct work_s *test_work2;
	struct work_s *test_work3;

	test_work1 = (struct work_s *)malloc(sizeof(struct work_s));
	test_work2 = (struct work_s *)malloc(sizeof(struct work_s));
	test_work3 = (struct work_s *)malloc(sizeof(struct work_s));

	start_time = clock_systimer();

	result = work_queue(HPWORK, test_work1, wq_test1, (void *)0, 10);
	TC_ASSERT_EQ_CLEANUP("work_queue",result, OK, goto to_free);

	result = work_queue(LPWORK, test_work2, wq_test2, (void *)0, 10);
	TC_ASSERT_EQ_CLEANUP("work_queue",result, OK, goto to_free);
	result = work_cancel(LPWORK, test_work2);
	TC_ASSERT_EQ_CLEANUP("work_cancel", result, OK, goto to_free);

	result = work_queue(HPWORK, test_work3, wq_test3, (void *)0, 10);
	TC_ASSERT_EQ_CLEANUP("work_queue",result, OK, goto to_free);
	result = work_cancel(HPWORK, test_work3);
	TC_ASSERT_EQ_CLEANUP("work_cancel", result, OK, goto to_free);

	result = work_queue(LPWORK, test_work3, wq_test3, (void *)0, 10);
	TC_ASSERT_EQ_CLEANUP("work_queue",result, OK, goto to_free);

	sleep(1);


to_free:
		free(test_work1);
		free(test_work2);
		free(test_work3);
	TC_SUCCESS_RESULT();
}
#endif
/****************************************************************************
 * Name: mqueue
 ****************************************************************************/
int wqueue_main(void)
{
#if defined(CONFIG_SCHED_HPWORK) || defined(CONFIG_SCHED_LPWORK)
	tc_wqueue_cancel();
#endif
	return 0;
}
#endif
