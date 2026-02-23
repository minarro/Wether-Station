#include "bme280.h"
#include "bme280-interface.h"
#include <stdbool.h>
#include <stdint.h>

#define ID 0xD0
#define RESET 0xE0
#define CTRL_HUM 0xF2
#define STATUS 0xF3
#define CTRL_MEAS 0xF4
#define CONFIG 0xF5
#define PRESS_MSB 0xF7
#define PRESS_LSB 0xF8
#define PRESS_XLSB 0xF9
#define TEMP_MSB 0xFA
#define TEMP_LSB 0xFB
#define TEMP_XLSB 0xFC
#define HUM_MSB 0xFD
#define HUM_LSB 0xFE

static int32_t t_fine = 0; // Fine resolution temperature for compensation of humitdity and pressure
static BmeCal cal = {0};

static float compensateT(int32_t t_raw)
{
	int32_t x  = (t_raw >> 3) - (cal.t1 <<1);
	x *= cal.t2;
	x >>= 11;
	int32_t y = (t_raw >> 4) - cal.t1;
	int32_t z = (y * y) >> 12;
	z *= cal.t3;
	z >>= 14;
	t_fine = x + z;
	return ((t_fine * 5 + 128) >> 8) / 100.0f;
}

static float compensateH(int32_t h_raw)
{
	int32_t x = t_fine - 76800;
	int32_t y = (h_raw << 14) - (cal.h4 << 20) - (cal.h5 * x);
	y += 16384;
	y >>= 15;
	int32_t z = (x * cal.h6) >> 10;
	z *= (((x * cal.h3) >> 11) + 32768);
	z = (z >> 10) + 2097152;
	z = (z * cal.h2) + 8192;
	z >>=  14;
	x = y * z;

	y = x >> 15;
	z = (y * y) >> 7;
	z *= cal.h1;
	z >>= 4;
	x -= z;

	if(x < 0) x = 0;
	else if(x > 419430400) x = 419430400;
	return  (x >> 12) / 1024.0f;
}

static float compensateP(int32_t p_raw)
{
	int32_t x = (t_fine >> 1) - 64000;
	int32_t y = x >> 2;
	int32_t z = (y * y) >> 11;
	z *= cal.p6;
	z += (x * cal.p5) << 1;
	z >>= 2;
	z += cal.p4 << 16;

	x *= cal.p2;
	x >>= 1;
	x += (cal.p3 * ((y * y) >> 13)) >> 3;
	x >>= 18;
	x += 32768 ;
	x *= cal.p1;
	x >>= 15;
	if(!x) return 0; // avoid exception caused by division by zero

	uint32_t p = (uint32_t)(1048576 - p_raw - (z >> 12));
	p *= 3125;
	if(p < 0x80000000) p = (p << 1) / (uint32_t)x;
	else p = (p / (uint32_t)x) * 2;

	uint32_t q = p >> 3;
	x = cal.p9 * (int32_t)((q * q) >> 13);
	x >>= 12;
	y = (int32_t)(p >> 2) * cal.p8;
	y >>= 13;
	z = (int32_t)p + ((x + y + cal.p7) >> 4);
	return (uint32_t)z / 1000.0f;
}

uint8_t bme_getId(void)
{
	return bme_read(ID);
}

void bme_reset(void)
{
	bme_write(RESET, 0xB6);
}

bool bme_isMeasuring(void)
{
	return (bool)(bme_read(STATUS) & 0b1000);
}

bool bme_isUpdating(void)
{
	return (bool)(bme_read(STATUS) & 0b0001);
}

void bme_setHumSampling(enum BmeOversampling setting)
{
	bme_write(CTRL_HUM, setting);
	// Need to write CTRL_MEAS for changes to take effect
	uint8_t data = bme_read(CTRL_MEAS);
	bme_write(CTRL_MEAS, data);
}

void bme_setTemSampling(enum BmeOversampling setting)
{
	uint8_t data = bme_read(CTRL_MEAS);
	data = (data & 0x1F) | (uint8_t)(setting << 5);
	bme_write(CTRL_MEAS, data);
}

void bme_setPresSampling(enum BmeOversampling setting)
{
	uint8_t data = bme_read(CTRL_MEAS);
	data = (data & 0xE3) | (uint8_t)(setting << 2);
	bme_write(CTRL_MEAS, data);
}

void bme_setMode(enum BmeMode setting)
{
	uint8_t data = bme_read(CTRL_MEAS);
	data = (data & 0xFC) | (uint8_t)setting;
	bme_write(CTRL_MEAS, data);
}

void bme_setCtrl(enum BmeOversampling s, enum BmeMode m)
{
	bme_write(CTRL_HUM, s);
	bme_write(CTRL_MEAS, (uint8_t)(s << 5 | s << 2 | m));
}

void bme_setStandbyTime(enum BmeStandby setting)
{
	uint8_t data = bme_read(CONFIG);
	data = (data & 0x1F) | (uint8_t)(setting << 5);
	bme_write(CONFIG, data);
}

void bme_setFilterCoef(enum BmeFilter setting)
{
	uint8_t data = bme_read(CONFIG);
	data = (data & 0xe3) | (uint8_t)(setting << 2);
	bme_write(CONFIG, data);
}

void bme_enable3WireSpi(void)
{
	bme_write(CONFIG, bme_read(CONFIG) | 0x01);
}

void bme_disable3WireSpi(void)
{
	bme_write(CONFIG, bme_read(CONFIG) | 0xFE);
}

BmeCal const *bme_init(void)
{
	while(bme_isUpdating()) continue;
	cal.t1 = (uint16_t)(bme_read(0x89) << 8 | bme_read(0x88));
	cal.t2 = (int16_t)(bme_read(0x8B) << 8 | bme_read(0x8A));
	cal.t3 = (int16_t)(bme_read(0x8D) << 8 | bme_read(0x8C));
	cal.p1 = (uint16_t)(bme_read(0x8F) << 8 | bme_read(0x8E));
	cal.p2 = (int16_t)(bme_read(0x91) << 8 | bme_read(0x90));
	cal.p3 = (int16_t)(bme_read(0x93) << 8 | bme_read(0x92));
	cal.p4 = (int16_t)(bme_read(0x95) << 8 | bme_read(0x94));
	cal.p5 = (int16_t)(bme_read(0x97) << 8 | bme_read(0x96));
	cal.p6 = (int16_t)(bme_read(0x99) << 8 | bme_read(0x98));
	cal.p7 = (int16_t)(bme_read(0x9B) << 8 | bme_read(0x9A));
	cal.p8 = (int16_t)(bme_read(0x9D) << 8 | bme_read(0x9C));
	cal.p9 = (int16_t)(bme_read(0x9F) << 8 | bme_read(0x9E));
	cal.h1 = (uint8_t)bme_read(0xA1);
	cal.h2 = (int16_t)(bme_read(0xE2) << 8 | bme_read(0xE1));
	cal.h3 = (uint8_t)bme_read(0xE3);
	cal.h4 = (int16_t)(bme_read(0xE4) << 4 | (bme_read(0xE5) & 0x0F));
	cal.h5 = (int16_t)(bme_read(0xE6) << 4 | bme_read(0xE5) >> 4);
	cal.h6 = (int8_t)bme_read(0xE7);
	return &cal;
}

void bme_getRawData(int32_t *t_raw, int32_t *h_raw, int32_t *p_raw)
{
	uint8_t data[8] = {0};
	bme_burstRead(PRESS_MSB, 8, data);
	*p_raw = (int32_t)data[0] << 12 | data[1] << 4 | data[2] >> 4;
	*t_raw = (int32_t)data[3] << 12 | data[4] << 4 | data[5] >> 4;
	*h_raw = data[6] << 8 | data[7];
}

void bme_getData(float *t, float *h, float *p)
{
	int32_t t_raw, h_raw, p_raw;
	bme_getRawData(&t_raw, &h_raw, &p_raw);
	*t = compensateT(t_raw);
	*h = compensateH(h_raw);
	*p = compensateP(p_raw);
}

void bme_getSample(float *t, float *h, float *p)
{
	bme_setCtrl(BME_SAMPLEx1, BME_FORCED);
	while(!bme_isMeasuring()) continue;
	while(bme_isMeasuring()) continue;
	bme_getData(t, h, p);
}
