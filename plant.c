// Отключение предупреждения о якобы устаревшей localtime() в Visual Studio.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "plant.h"

int i,j_1,j2;
double jtim,r,r1,r2,r3,r4,delt,tau,dx;
long int h;
struct tm mytime;

static void
gettime(struct tm* result) {
	time_t now = time(NULL);
	*result = *localtime(&now);
}

void
plant_init(Plant plant) {
    for (i = 0; i < sizeof(Plant) / sizeof(plant[0]); i++) {
        plant[i] = 0;
    }
}

/* Имитатор об"екта исследования : KANAL - номер канала опроса;
	 XIZM - результат измерения по каналу; plant - вспомогательный
	 массив типа REAL  с 52 элементами.
*/

double
initdat(void) {
	int i;
	double rr,rr4,rt;

	gettime(&mytime);
	rr=mytime.tm_sec;
	rr4=mytime.tm_min;
	rr=rr+60*rr4;
	rr4=mytime.tm_hour;
	rr=rr+3600*rr4;
	do {
		rr-=1000;
	} while(rr>32767);
	i=(int)rr;
	rr4=i;
	rt=(rr-rr4)*1.0E+5+7127.1;
	return rt;
}


/* Датчик равномерно распределенных [0,1] псевдослучайных чисел  */
static void
randd(double * dxx,double * rr) {
	double dy = 117239.7;
	double rrr,rrr1;
	rrr=4; (*dxx)*=5;
	do {
		rrr1=rrr*dy;
		if ((*dxx)>rrr1) (*dxx)-=rrr1;
		rrr=rrr*0.5;
	} while(rrr>0.9);
	(*rr)=(*dxx)/dy;
}


/* Генерация псевдослучайных нормальных чисел */
static double
gauss ( double r,double s) {
	int i;
	double r2,r1;
	r1=0;
	for(i=1;i<12;i++) {
		randd(&dx,&r2); r1=r1+r2;
	};
	r1=(r1-6)*s+r;
	return r1;
} /* GAUSS */


/* Генерация независимых переменных */
static void
izmer7(int j, Plant plant) {
	double r,rr;
	gettime(&mytime);
	rr=mytime.tm_sec;
	r= mytime.tm_min;
	rr+=60*r;
	r=mytime.tm_hour;
	rr+=3600*r;
	plant[11]=rr;
	randd(&dx,&r);
	if (j==1) plant[1]=-20+50*r;
	else
	if (j==2) plant[2]=740+30*r;
	else
	if (j==3) plant[3]=50+50*r;
	else
	if (j==4) {
		plant[4]=0;
		if (r>0.5) plant[4]=1;
	}
	else
	if (j==5) {
		plant[5]=2;
		if (r<=0.33333) plant[5]=1;
		else
		if (r>=0.666667) plant[5]=3;
	}
	else
	if (j==6) {
		plant[6]=0;
		if (r>0.5) plant[6]=1;
	};
} /* IZMER7 */


double
plant_measure(int kanal, Plant plant) {
	/* Начало основной части процедуры OPROS */
	double xizm;
	dx=plant[12];
	if ((dx<0.1)||(dx>1.0E+6)) dx=initdat();
	if (kanal>=1) {
		gettime(&mytime);
		r4  =mytime.tm_sec;
		delt=mytime.tm_min;
		r4+=60*delt;
		delt=mytime.tm_hour;
		r4+=3600*delt;
		delt=r4-plant[11]-180;
		if (kanal<=10) {
			/* Независимые переменные */
			plant[11]=0;
			if (kanal <= 6) izmer7(kanal, plant);
			r=plant[kanal];
		}
		else
		if (kanal<=70) {
			r4=0; r=1; r1=gauss(r4,r);
			if (kanal<=25) {
				/* Лин. регресс. модель - пассивный эксперимент */
				j_1=kanal-11;
				j2=kanal-18;
				r=30+j_1*5-(5-j2*0.14)*plant[1]+(0.6-j_1*0.014)*plant[2]+
				(350-j_1*10)*plant[4]-(3-j2*0.07)*plant[1]*plant[4]+
				(0.05+j_1*0.001)*plant[1]*plant[3]+0.01*plant[3]*plant[3];
				r1=5*r1;
			}
			else
			if (kanal <= 40) {
				/* Лин. регресс. модель - активно-пассивный эксперимент */
				j_1=kanal-26; j2=kanal-33;
				r=100+j_1*20+(7+j_1*0.5)*plant[1]+(0.6+j2*0.01)*plant[2]-
				(15+j_1*2)*plant[5]+(4+j2*0.1)*plant[7]+
				(40+j_1*2)*plant[8]+(0.01+j2*0.0005)*plant[1]*plant[2]-
				(0.5+j2*0.01)*plant[7]*plant[8]+(0.7-j_1*0.02)*plant[5]*
				plant[7]-(0.15-j_1*0.01)*plant[1]*plant[1]+(30-j_1*1.4)*
				plant[8]*plant[8] ;
				r1=5*r1;
			}
			else
			if (kanal <= 55) {
				/* Нелин. модель - пассивный эксперимент */
				r=30+(kanal-41)*3+(10+(kanal-48)*0.5)*exp((0.055-
				(kanal-41)*0.001)*plant[1])-(2+(kanal-50)*0.1)*
				exp(0.07*plant[1]-(0.0001+(kanal-46)*0.00002)*plant[2]+
				(1+(kanal-48)*0.06)*plant[4]);
				r1=1.5*r1;
			}
			else
			if (kanal <= 70) {
				/* Нелинейная модель - активно-пассивный эксперимент */
				j_1=kanal-56; j2=kanal-63;
				r=60+(kanal-65)*3+(-5.5+j_1*0.1)*exp((0.05+j_1*0.001)*
				plant[1]+(-0.1+j2*0.006)*plant[9])+(9-j_1*0.2)*
				exp((-0.55+j_1*0.01)*plant[10]+(0.7+j2*0.03)*plant[5]);
				r1=1.5*r1;
			};
			r=r+r1;
		}
		else
		if (kanal <= 85) {
			/* Динамическая модель - инерционное звено */
			gettime(&mytime);
			jtim=mytime.tm_sec;
			r4=  mytime.tm_min;
			jtim+=60*r4;
			r4=  mytime.tm_hour;
			jtim+=3600*r4;
			tau=15-(kanal-71)*0.4f;
			r=0; r3=0;
			for (i=20;i>=1;i--) {
				r2=jtim-plant[i+12]; r2=r2/tau;
				if (r2<=50) r=r+(plant[i+32]-r3)*(1-exp(-r2));
				r3=plant[i+32];
			};
			r=(20+(kanal-78)*2)*r;
			/* for i:=1 to 20 do writeln(plant[i+12]:10:3,plant[i+32]:10:3) */
		}
		else {
			j_1=kanal-100; r1=j_1*0.1f; r4=1;
			r=gauss(r1,r4);
		};
	};
	xizm=r;
	plant[12]=dx;
	return xizm;
}

void
plant_control(int kanal, double upr, Plant plant) {
	/* Имитация подачи управл. воздействия на об'ект */
	typedef double mm[4];
	int  i,i1,j1;
	double jtim,r;
	mm umin = {0}, umax = {0};

	umin[0]=-70;  umax[0]=-umin[0];
	umin[1]=-3;   umax[1]=-umin[1];
	umin[2]=-5;   umax[2]=-umin[2];
	umin[3]=-2.5; umax[3]=-umin[3];
	if ((kanal < 7) || (kanal > 10)) {
		/*, kanal:7,' упр.возд.=', upr:10:3)*/
		fprintf(stderr, "uprav: error: channel index %d out of valid 7..10\n", kanal);
	}
	else {
		j1=kanal-6;
		if (upr>umax[j1]) upr=umax[j1];
		else
		if (upr<umin[j1]) upr=umin[j1];
		plant[kanal]=upr;
		if (kanal==10)
		if (fabs(plant[33]-upr)>=0.01) {
			gettime(&mytime);
			jtim=mytime.tm_sec;
			r=mytime.tm_min;
			jtim+=60*r;
			r=h;
			jtim+=3600*r;
			for(i=2;i<20;i++) {
				i1=22-i; plant[i1+32]=plant[i1+31];
				plant[i1+12]=plant[i1+11];
			};
			plant[33]=upr; plant[13]=jtim;
		};
	};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
