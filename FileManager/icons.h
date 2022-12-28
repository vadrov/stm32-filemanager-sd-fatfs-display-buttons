/*
 * Author: VadRov
 * Copyright (C) 2021, VadRov, all right reserved.
 * Значки файлов и каталогов для файлового менеджера
 *
 *  Допускается свободное распространение без целей коммерческого использования.
 *  При коммерческом использовании необходимо согласование с автором.
 *  Распространятся по типу "как есть", то есть использование осуществляете на свой страх и риск.
 *  Автор не предоставляет никаких гарантий.
 *
 * https://www.youtube.com/@VadRov
 * https://dzen.ru/vadrov
 * https://vk.com/vadrov
 * https://t.me/vadrov_channel
 *
 */

#ifndef SRC_ICONS_H_
#define SRC_ICONS_H_

#include "main.h"

typedef struct {
	const uint16_t *data;
	uint8_t width;
	uint8_t height;
} Icon_Image;

extern const Icon_Image Icon_Prev;
extern const Icon_Image Icon_Catalog;
extern const Icon_Image Icon_Video;
extern const Icon_Image Icon_Picture;
extern const Icon_Image Icon_Text;
extern const Icon_Image Icon_Other;
extern const Icon_Image Icon_Audio;

#endif /* SRC_ICONS_H_ */
