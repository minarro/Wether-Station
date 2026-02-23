#ifndef XTAL_H_
#define XTAL_H_

#define _XTAL_FREQ 8000000UL

#if _XTAL_FREQ == 8000000
	#define IRCF_SEL 7
#elif _XTAL_FREQ == 4000000
	#define IRCF_SEL 6
#elif _XTAL_FREQ == 2000000
	#define IRCF_SEL 5
#elif _XTAL_FREQ == 1000000
	#define IRCF_SEL 4
#elif _XTAL_FREQ == 500000
	#define IRCF_SEL 3
#elif _XTAL_FREQ == 250000
	#define IRCF_SEL 2
#elif _XTAL_FREQ == 125000
	#define IRCF_SEL 1
#elif _XTAL_FREQ == 31250
	#define IRCF_SEL 0
#else
	#error _XTAL_FREQ set to unsupported value
#endif

#endif