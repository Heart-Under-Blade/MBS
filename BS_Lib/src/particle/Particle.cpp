#include "Particle.h"
#include <fstream>
#include <iostream>
#include <cstring>
#include "global.h"

Particle::Particle()
{
	SetFacetIndices();
}

Particle::Particle(int nFacets, const complex &refrIndex, bool isNonConvex)
	: m_refractiveIndex(refrIndex),
	  m_isNonConvex(isNonConvex)
{
	nElems = nFacets;
	SetFacetIndices();
}

Particle::~Particle()
{
}

void Particle::SetFacetIndices()
{
	for (int i = 0; i < MAX_FACET_NUM; ++i)
	{
		elems[i].actual.index = i;
	}
}

Facet *Particle::GetActualFacet(int i)
{
	return &elems[i].actual;
}

void Particle::SetFromFile(const std::string &filename, double sizeIndex)
{

	std::ifstream pfile(filename, std::ios::in);

	if (!pfile.is_open())
	{
		std::cerr << "File \"" << filename << "\" is not found" << std::endl;
		throw std::exception();
	}

	const int bufSize = 1024;
	char *buff = (char*)malloc(sizeof(char) * bufSize);

	nElems = 0;
	Facet *facet = &(elems[nElems++].origin);

	char *ptr, *trash;

	pfile.getline(buff, bufSize);
	m_isNonConvex = strtol(buff, &trash, 10);

	pfile.getline(buff, bufSize);
	isAggregated = strtol(buff, &trash, 10);

	// read symmetry params
	{
		pfile.getline(buff, bufSize);

		ptr = strtok(buff, " ");
		m_symmetry.beta = Orientation::DegToRad(strtod(ptr, &trash));

		ptr = strtok(NULL, " ");
		m_symmetry.gamma = Orientation::DegToRad(strtod(ptr, &trash));
	}

	pfile.getline(buff, bufSize); // skip empty line

	while (!pfile.eof()) // read vertices of facets
	{
		pfile.getline(buff, bufSize);
		ptr = strtok(buff, " ");

		if (strlen(buff) == 0)
		{
			facet = &(elems[nElems++].origin);
			continue;
		}

		int c_i = 0;

		while (ptr != NULL)
		{
			facet->arr[facet->nVertices].coordinates[c_i++] = strtod(ptr, &trash);
			ptr = strtok(NULL, " ");
		}

		++(facet->nVertices);
	}

	pfile.close();

	// correction of number of facet
	while (elems[nElems-1].origin.nVertices == 0)
	{
		--nElems;
	}

	SetDefaultNormals();
	Reset();
	SetDefaultCenters();

	if (m_isNonConvex || isAggregated)
	{
		for (int i = 0; i < nElems; ++i)
		{
			elems[i].origin.isOverlayedIn = true;
			elems[i].origin.isOverlayedOut = true;
			elems[i].actual.isOverlayedIn = true;
			elems[i].actual.isOverlayedOut = true;
		}
	}
}

void Particle::Rotate(const Orientation &angle, bool isGlobal)
{
	Rotator *rotator;

	if (isGlobal)
	{
		rotator = &m_globalRotator;
	}
	else
	{
		rotator = &m_localRotator;
	}

	rotAngle = angle;
	rotator->SetRotationMatrix(rotAngle);

	for (int i = 0; i < nElems; ++i)
	{
		auto &facet = elems[i];

		// rotate facet
		for (int j = 0; j < facet.origin.nVertices; ++j)
		{
			rotator->RotatePoint(facet.origin.arr[j], facet.actual.arr[j]);
		}

		// rotate center
		rotator->RotatePoint(facet.origin.center, facet.actual.center);

		// rotate normal
		rotator->RotatePoint(facet.origin.in_normal, facet.actual.in_normal);

		// set D params
		double d = Point3f::DotProduct(facet.actual.arr[0], facet.actual.in_normal);
		facet.actual.in_normal.d_param = -d;

		// set external normals
		facet.actual.ex_normal = -facet.actual.in_normal;
		facet.actual.ex_normal.d_param = d;
	}
}

void Particle::Fix()
{
	for (int i = 0; i < nElems; ++i)
	{
		for (int j = 0; j < elems[i].actual.nVertices; ++j)
		{
			elems[i].origin.arr[j] = elems[i].actual.arr[j];
		}
	}
}

void Particle::Merge(const std::vector<Particle> &parts)
{
	int i = 0;
	nElems = 0;

	for (const Particle &part : parts)
	{
		nElems += part.nElems;

		for (int j = 0; j < part.nElems; ++j)
		{
			elems[i++].origin = part.elems[j].actual;
		}
	}

	isAggregated = true;
}

double Particle::ComputeRotationRadius() const
{
	Point3f p0(0, 0, 0);

	double radius = 0;

	for (int i = 0; i < nElems; ++i)
	{
		const Facet &facet = elems[i].actual;

		for (int j = 0; j < facet.nVertices; ++j)
		{
			Point3f v_len = facet.arr[j] - p0;
			double len = Point3f::Length(v_len);

			if (len > radius)
			{
				radius = len;
			}
		}
	}

	return radius;
}

const complex &Particle::GetRefractiveIndex() const
{
	return m_refractiveIndex;
}

const Angle3d &Particle::GetSymmetry() const
{
	return m_symmetry;
}

void Particle::Move(float dx, float dy, float dz)
{
	for (int i = 0; i < nElems; ++i)
	{
		for (int j = 0; j < elems[i].origin.nVertices; ++j)
		{
			elems[i].actual.arr[j] = elems[i].origin.arr[j] + Point3f(dx, dy, dz);
		}
	}
}

bool Particle::IsNonConvex() const
{
	return m_isNonConvex;
}

void Particle::Output()
{
	std::ofstream M("particle.dat", std::ios::out);

	for (int i = 0; i < nElems; ++i)
	{
		Facet &facet = elems[i].actual;

		for (int j = 0; j < facet.nVertices; ++j)
		{
			Point3f p = facet.arr[j];
			M << p.coordinates[0] << ' '
							<< p.coordinates[1] << ' '
							<< p.coordinates[2] << ' '
							<< i ;
			M << std::endl;
		}

		M << std::endl << std::endl;;
	}

	M.close();
}

void Particle::SetRefractiveIndex(const complex &value)
{
	m_refractiveIndex = value;
}

void Particle::SetDefaultNormals()
{
	for (int i = 0; i < nElems; ++i)
	{
		elems[i].origin.SetNormal();
	}
}

void Particle::Reset()
{
	for (int i = 0; i < nElems; ++i)
	{
		elems[i].actual = elems[i].origin;
	}
}

void Particle::SetDefaultCenters()
{
	for (int i = 0; i < nElems; ++i)
	{
		elems[i].origin.SetCenter();
	}
}

void Particle::SetSymmetry(double beta, double gamma, double alpha)
{
	m_symmetry.beta = beta;
	m_symmetry.gamma = gamma;
	m_symmetry.alpha = alpha;
}
