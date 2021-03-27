#include<stdio.h>
#include<stdlib.h>

#ifndef METRICS_H
#define METRICS_H

#define MAX_RESAMPLES 1000
#define E_MIN 1e-11
#define NAME_MAX_LEN 96


typedef struct Part Part;
typedef struct Metric Metric;

typedef int (*PerturbFun)(const Metric* metric, Part* part);

typedef struct Metric{
	int dim; // Dimensiones de cada submetrica
	double* bw; // Anchos de banda
	PerturbFun perturb; // Funcion de perturbacion
	int n_gp; // Cantidad de parametros geometricos
	double* geom_par; // Parametros geometricos de cada submetrica
} Metric;

Metric* Metric_create(int dim, const double* bw, PerturbFun perturb, int n_gp, const double* geom_par);
Metric* Metric_copy(const Metric* from);
void Metric_destroy(Metric* metric);

typedef struct Geometry{
	int ord; // Cantidad de submetricas
	Metric** ms; // Submetricas
	char* bwfilename; // Nombre de archivo con anchos de banda
	FILE* bwfile; // Archivo con anchos de banda

	double* trasl; // Traslacion de la metrica
	double* rot; // Rotacion de la metrica
} Geometry;

Geometry* Geom_create(int ord, Metric** metrics, const char* bwfilename, int variable_bw,
	const double* trasl, const double* rot);
Geometry* Geom_copy(const Geometry* from);
int Geom_perturb(const Geometry* geom, Part* part);
int Geom_next(Geometry* geom);
void Geom_destroy(Geometry* geom);

int E_perturb(const Metric* metric, Part* part);
int Let_perturb(const Metric* metric, Part* part);

int Vol_perturb(const Metric* metric, Part* part);
int SurfXY_perturb(const Metric* metric, Part* part);
int Guide_perturb(const Metric* metric, Part* part);

int Isotrop_perturb(const Metric* metric, Part* part);
int Polar_perturb(const Metric* metric, Part* part);


#endif
