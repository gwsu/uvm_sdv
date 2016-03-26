/*
 * sw_test.h
 *
 *  Created on: Mar 22, 2016
 *      Author: ballance
 */

#ifndef SW_TEST_H_
#define SW_TEST_H_

#if defined(_WIN32) || defined(__CYGWIN__)
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif


void report_error_c(const char *msg);


#endif /* SW_TEST_H_ */
