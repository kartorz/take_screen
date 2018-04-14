/*
 * This file is for multi-language.
 *
 *  Athor: LiQiong Lee
 *  
 *  Changes:
 *    2013/07/08: First create.
 */

#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_
//#define EN
#define ZH

#ifdef EN
#include "language_en.h"
#elif defined(ZH)
#include "language_zh.h"
#endif

#endif