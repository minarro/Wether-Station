#ifndef BME280_H_
#define BME280_H_

#include <stdbool.h>
#include <stdint.h>

enum BmeOversampling
{
	BME_NO_SAMPLE,
	BME_SAMPLEx1,
	BME_SAMPLEx2,
	BME_SAMPLEx4,
	BME_SAMPLEx8,
	BME_SAMPLEx16
};

enum BmeMode
{
	BME_SLEEP,
	BME_FORCED,
	BME_NORMAL=3
};

enum BmeStandby
{
	BME_500ns,
	BME_62ms,
	BME_125ms,
	BME_250ms,
	BME_500ms,
	BME_1s,
	BME_10ms,
	BME_20ms
};

enum BmeFilter
{
	BME_FILTER_OFF,
	BME_FILTER_COEF_2,
	BME_FILTER_COEF_4,
	BME_FILTER_COEF_8,
	BME_FILTER_COEF_16
};

typedef struct BmeCal BmeCal;
struct BmeCal
{
	int32_t t1;
	int32_t t2;
	int32_t t3;
	int32_t h1;
	int32_t h2;
	int32_t h3;
	int32_t h4;
	int32_t h5;
	int32_t h6;
	int32_t p1;
	int32_t p2;
	int32_t p3;
	int32_t p4;
	int32_t p5;
	int32_t p6;
	int32_t p7;
	int32_t p8;
	int32_t p9;
};

uint8_t bme_getId(void); // Should always return 96
void bme_reset(void); // Equiv to POR
bool bme_isMeasuring(void); // Check if true before waiting for false!
bool bme_isUpdating(void);
void bme_setHumSampling(enum BmeOversampling);
void bme_setTemSampling(enum BmeOversampling);
void bme_setPresSampling(enum BmeOversampling);
void bme_setMode(enum BmeMode);
void bme_setCtrl(enum BmeOversampling, enum BmeMode); // Set sampling and mode in one call for convinience
void bme_setStandbyTime(enum BmeStandby);
void bme_setFilterCoef(enum BmeFilter);
void bme_enable3WireSpi(void);
void bme_disable3WireSpi(void);

BmeCal const *bme_init(void); // Call this once after power-on
void bme_getRawData(int32_t *t_raw, int32_t *h_raw, int32_t *p_raw); // Uncompensated
void bme_getData(float *t, float *h, float *p); // Compensated
void bme_getSample(float *t, float *h, float *p); // Blocking. Use normal mode for asynchronous readout

#endif