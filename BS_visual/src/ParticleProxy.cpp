#include "ParticleProxy.h"

#include "Particle.h"
#include "RegularColumn.h"
#include "HexagonalAggregate.h"
#include "CertainAggregate.h"
#include "HollowColumn.h"
#include "Bullet.h"
#include "BulletRosette.h"
#include "DistortedColumn.h"

#include "global.h"
#include "ScatteringConvex.h"
#include "ScatteringNonConvex.h"
#include "Mueller.hpp"
#include "Handler.h"

#include <iostream>
using namespace std;

#ifdef _DEBUG
using namespace std;
ofstream debfile("deb.dat", ios::out);
#endif

ParticleProxy::ParticleProxy()
{
	m_particle = nullptr;
	m_scattering = nullptr;

	particleTypes.insert(PType::RegularColumn, "Column");
	particleTypes.insert(PType::HollowColumn, "Hollow column");
	particleTypes.insert(PType::Bullet, "Bullet");
	particleTypes.insert(PType::BulletRosette, "Bullet-rosette");
	particleTypes.insert(PType::Aggregate, "Aggregate");
}

ParticleProxy::~ParticleProxy()
{
	if (m_particle != nullptr)
	{
		delete m_particle;
	}

	if (m_scattering != nullptr)
	{
		delete m_scattering;
	}
}

void ParticleProxy::SetParticleFromFile(const QString &filename, int sizeIndex)
{
	if (m_particle != nullptr)
	{
		delete m_particle;
	}

	m_particle = new Particle();
	m_particle->SetFromFile(filename.toStdString(), sizeIndex);
	m_particle->Rotate(Angle3d(0, Angle3d::DegToRad(90), 0));
}

QString ParticleProxy::RecoverTrack(IdType id, int level)
{
	QString track;
	int coef = m_particle->nElems + 1;
	std::vector<int> tmp_track;

	IdType tmpId = id/coef;
	for (int i = 0; i <= level; ++i)
	{
		int tmp = (tmpId%coef)/*.toInt()*/;
		tmpId -= tmp;
		tmpId /= coef;
		tmp -= 1;
		tmp_track.push_back(tmp);
	}

	if (!tmp_track.empty())
	{
		track = QString::number(tmp_track.at(tmp_track.size()-1));

		for (int i = tmp_track.size()-2; i >= 0; --i)
		{
			track += "-" + QString::number(tmp_track.at(i));
		}
	}

	return track.trimmed();
}

void ParticleProxy::SetParticle(const QString &type, double refrIndex,
								double height, double diameter,
								double additional)
{
	if (m_particle != nullptr)
	{
		delete m_particle;
	}

	Size size(diameter, height);

	if (type == particleTypes.value(PType::RegularColumn))
	{
		ptype = PType::RegularColumn;
		m_particle = new RegularColumn(refrIndex, size);
	}
	else if (type == particleTypes.value(PType::HollowColumn))
	{
		ptype = PType::HollowColumn;
		m_particle = new HollowColumn(refrIndex, size, additional);
	}
	else if (type == particleTypes.value(PType::Bullet))
	{
		ptype = PType::Bullet;
		additional = (diameter*sqrt(3)*tan(Angle3d::DegToRad(62)))/4;
		m_particle = new Bullet(refrIndex, size, additional);
	}
	else if (type == particleTypes.value(PType::BulletRosette))
	{
		ptype = PType::BulletRosette;
		additional = (diameter*sqrt(3)*tan(Angle3d::DegToRad(62)))/4;
		m_particle = new BulletRosette(refrIndex, size, additional);
	}
	else if (type == particleTypes.value(PType::Aggregate))
	{
		ptype = PType::Aggregate;
		m_particle = new CertainAggregate(refrIndex, additional);
	}

	m_particle->Rotate(Angle3d(0, Angle3d::DegToRad(90), 0));
}

void RotateMuller(const Point3f &dir, matrix &M)
{
	const float &z = dir.coordinates[2];

	if (z >= 1-DBL_EPSILON && z <= DBL_EPSILON-1)
	{
		const float &y = dir.coordinates[1];

		if (y*y > DBL_EPSILON)
		{	// rotate the Mueller matrix of the beam to appropriate coordinate system

			const float &x = dir.coordinates[0];
			const float &y = dir.coordinates[1];

			double tmp = y*y;

			tmp = acos(x/sqrt(x*x+tmp));

			if (y < 0)
			{
				tmp = M_2PI-tmp;
			}

			tmp *= -2.0;
			RightRotateMueller(M, cos(tmp), sin(tmp));
		}
	}
}

void ParticleProxy::SetScattering(const Point3f &incidentDir, int reflNum,
							   const Point3f &polarizationBasis)
{
	bool isOpticalPath = true;

	if (m_scattering != nullptr)
	{
		delete m_scattering;
	}

	if (m_particle->IsNonConvex())
	{
		m_scattering = new ScatteringNonConvex(m_particle, Light{incidentDir, polarizationBasis},
											 reflNum);
	}
	else
	{
		m_scattering = new ScatteringConvex(m_particle, Light{incidentDir, polarizationBasis},
										  reflNum);
	}
}


void ParticleProxy::RotateTrack(const Angle3d &angle, Rotator &rotator,
								Beam &beam)
{
	rotator.SetRotationMatrix(angle);

	for (Polygon1 &pol : beam.pols)
	{
		Point3f resP;
		Polygon1 resPol;

		for (int i = 0; i < pol.nVertices; ++i)
		{
			rotator.RotatePoint(pol.arr[i], resP);
			resPol.AddVertex(resP);
		}

		pol = resPol;
	}
}

void ParticleProxy::SetBeamInfo(int beamNumber, Beam &beam, BeamInfo &info)
{
	info.beam = beam;
	info.area = Handler::BeamCrossSection(beam)/**sinBeta*/;

	double phi, theta;
	beam.GetSpherical(phi, theta);
	info.thetaDeg = 180 - Angle3d::RadToDeg(theta);
	info.phiDeg = Angle3d::RadToDeg(phi);

	info.track = RecoverTrack(beam.id, beam.actNo)+":";
	info.number = beamNumber;

	matrix M = Mueller(beam.Jones);
	RotateMuller(beam.direction, M);
	info.M   << M[0][0] << M[0][1] << M[0][2] << M[0][3]
			 << M[1][0] << M[1][1] << M[1][2] << M[1][3]
			 << M[2][0] << M[2][1] << M[2][2] << M[2][3]
			 << M[3][0] << M[3][1] << M[3][2] << M[3][3];
}

void ParticleProxy::Trace(const Angle3d &angle, const Angle3d &viewAngle, int reflNum)
{
	beamData.clear();

	Point3f incidentDir(0, 0, -1);
	Point3f polarizationBasis(0, 1, 0);

	SetScattering(incidentDir, reflNum, polarizationBasis);

	vector<Beam> outBeams;
	m_particle->Rotate(angle);
	m_scattering->ScatterLight(outBeams);

	int count = 0;

	for (Beam &beam : outBeams)
	{
		beam.RotateSpherical(-incidentDir, polarizationBasis);

		// add last polygon
		Polygon1 pol;
		Point3f p = beam.direction * m_particle->ComputeRotationRadius()*2;
		pol.AddVertex(p);
		beam.pols.push_back(pol);

//		RotatePolygon(viewAngle, m_globalRotator, beam);

		BeamInfo info;
		SetBeamInfo(++count, beam, info);

		QString dir = QString("%1, %2").arg(info.phiDeg).arg(info.thetaDeg);
#ifdef _DEBUG // DEB
		if (info.beam.pols.at(0).nVertices == 0)
			int fff = 0;
#endif
		if (beamData.contains(dir))
		{
			auto it = beamData.find(dir);
			(*it).insertMulti(info.track, info);
		}
		else
		{
			BeamData data;
			data.insert(info.track, info);
			beamData.insert(dir, data);
		}
	}
}

void ParticleProxy::ConvertCoordinates(QPolygonF &pol)
{
	for (int i = 0; i < pol.size(); ++i)
	{
		pol[i].setY(-pol[i].y());
	}
}

void ParticleProxy::RotateParticle(const Angle3d &rotAngle, const Angle3d &viewAngle)
{
	m_particle->Rotate(rotAngle);
	m_particle->Rotate(viewAngle, true);
}

void ParticleProxy::RotatePoints(const Angle3d &angle,
								 const std::vector<Point3f> &points,
								 std::vector<Point3f> &rotatedPoints)
{
	m_localRotator.SetRotationMatrix(angle);

	for (const Point3f &p : points)
	{
		Point3f resPoint;
		m_localRotator.RotatePoint(p, resPoint);
		rotatedPoints.push_back(resPoint);
	}
}

QVector<QPointF> ParticleProxy::RotateLocalAxes(const Angle3d &angle, const Angle3d &viewAngle)
{
	vector<Point3f> rotatedAxes, resAxes;
	vector<Point3f> axes {localAxes.x, localAxes.y, localAxes.z};

	RotatePoints(angle, axes, rotatedAxes);
	RotatePoints(viewAngle, rotatedAxes, resAxes);

	double size = m_particle->ComputeRotationRadius() * 2;
	resAxes[0] = resAxes[0] * size;
	resAxes[1] = resAxes[1] * size;
	resAxes[2] = resAxes[2] * size;

	QPolygonF res;
	res.append(QPointF{resAxes.at(0).coordinates[0], resAxes.at(0).coordinates[1]});
	res.append(QPointF{resAxes.at(1).coordinates[0], resAxes.at(1).coordinates[1]});
	res.append(QPointF{resAxes.at(2).coordinates[0], resAxes.at(2).coordinates[1]});

	ConvertCoordinates(res);
	return res;
}

QVector<QPointF> ParticleProxy::RotateGlobalAxes(const Angle3d &viewAngle)
{
	vector<Point3f> resAxes;
	RotatePoints(viewAngle, vector<Point3f>{globalAxes.x, globalAxes.y, globalAxes.z}, resAxes);

	double size = m_particle->ComputeRotationRadius() * 2;
	resAxes[0] = resAxes[0] * size;
	resAxes[1] = resAxes[1] * size;
	resAxes[2] = resAxes[2] * size;

	QPolygonF res;
	res.append(QPointF{resAxes.at(0).coordinates[0], resAxes.at(0).coordinates[1]});
	res.append(QPointF{resAxes.at(1).coordinates[0], resAxes.at(1).coordinates[1]});
	res.append(QPointF{resAxes.at(2).coordinates[0], resAxes.at(2).coordinates[1]});

	ConvertCoordinates(res);
	return res;
}

QString ParticleProxy::GetParticleType() const
{
	return particleTypes.value(ptype);
}

QStringList ParticleProxy::GetParticleTypes() const
{
	return particleTypes.values();
}

void ParticleProxy::GetTrack(int beamNumber, const Angle3d &viewAngle,
							 const Angle3d &rotAngle,
							 QVector<NumberedFacet> &track,
							 QVector<QPolygonF> &trackLastFacet)
{
	BeamInfo info;
	GetBeamByNumber(beamNumber, info);

	QVector<Beam> beams;
	GetBeamsByKey(info.track, beams);

	for (int i = 0; i < beams.size(); ++i)
	{
		Beam &b = beams[i];

//		RotatePolygon(rotAngle, m_localRotator, b);
		RotateTrack(viewAngle, m_globalRotator, b);

		QPolygonF pol;
		Polygon1 &ppl1 = b.pols.at(b.pols.size()-1);

		for (int j = 0; j < ppl1.nVertices; ++j)
		{
			Point3f &p = ppl1.arr[j];
			pol.append(QPointF(p.coordinates[0], p.coordinates[1]));
		}

		ConvertCoordinates(pol);
		trackLastFacet.append(pol);
	}

//	RotatePolygon(rotAngle, m_localRotator, info.beam);
	RotateTrack(viewAngle, m_globalRotator, info.beam);

	std::vector<int> tr;
	Tracks::RecoverTrack(m_particle->nElems, info.beam, tr);

	for (int i = 0; i < info.beam.pols.size(); ++i)
	{
		QPolygonF pol;
		Polygon1 &pol1 = info.beam.pols.at(i);

		for (int j = 0; j < pol1.nVertices; ++j)
		{
			Point3f &p = pol1.arr[j];
			pol.append(QPointF(p.coordinates[0], p.coordinates[1]));
		}

		ConvertCoordinates(pol);

		if (i == 0 || i == info.beam.pols.size()-1)
		{
			track.append(NumberedFacet{0, pol});
		}
		else
		{
			track.append(NumberedFacet{tr[i-1], pol});
		}
	}
}

QString ParticleProxy::GetBeamDataString()
{
	QString res;
	int f = 3, b = 16;

	foreach (QString key, beamData.keys())
	{
		BeamData data = beamData.value(key);
		res += key+"\t"+QString::number(data.size())+"\n";

		foreach (QString dkey, data.uniqueKeys())
		{
			auto infos = data.values(dkey);

			foreach (BeamInfo info, infos)
			{
				QString d = QString("0x%1").arg(info.number, f, b, QChar('0'))+"\n";
				res += " "+dkey+"\t"+d;
			}
		}
	}

	return res;
}

QString ParticleProxy::GetBeamDataString(const QString &searchLine)
{
	QString res;
	int f = 3, b = 16;

	foreach (QString key, beamData.keys())
	{
		BeamData data = beamData.value(key);
		QStringList children;

		foreach (QString dkey, data.uniqueKeys())
		{
			if (dkey.startsWith(searchLine))
			{
				auto infos = data.values(dkey);

				foreach (BeamInfo info, infos)
				{
					children += " "+dkey+"\t"+QString("0x%1").arg(info.number, f, b, QChar('0'))+"\n";
				}
			}
		}

		if (!children.isEmpty())
		{
			res += key+"\t"+QString::number(children.size())+"\n";
			res += QString(children.join(""));
		}
	}

	return res;
}

BeamInfo &ParticleProxy::GetBeamByKeys(const QString &trackKey, const QString &beamKey)
{
	return beamData[trackKey][beamKey];
}

void ParticleProxy::GetBeamByNumber(int number, BeamInfo &binfo)
{
	foreach (QString key, beamData.keys())
	{
		BeamData data = beamData.value(key);

		foreach (QString dkey, data.uniqueKeys())
		{
			auto infos = data.values(dkey);

			foreach (BeamInfo info, infos)
			{
				if (info.number == number)
				{
					binfo = info;
				}
			}
		}
	}
}

int ParticleProxy::GetTotalBeamCount()
{
	int total = 0;

	foreach (QString key, beamData.keys())
	{
		BeamData data = beamData.value(key);
		total += data.size();
	}

	return total;
}

void ParticleProxy::GetBeamsByKey(const QString &trackKey, QVector<Beam> &beams)
{
	foreach (QString key, beamData.keys())
	{
		BeamData data = beamData.value(key);

		foreach (QString dkey, data.uniqueKeys())
		{
			if (dkey.startsWith(trackKey))
			{
				auto infos = data.values(dkey);

				foreach (BeamInfo info, infos)
				{
					beams.append(info.beam);
				}
			}
		}
	}
}

void ParticleProxy::GetVisibleFacets(QVector<NumberedFacet> &visfacets,
									 QVector<NumberedFacet> &invisFacets)
{
	Point3f incidentDir(0, 0, -1);
	Point3f polarizationBasis(0, 1, 0);

	Point3f point = incidentDir * m_particle->ComputeRotationRadius() * 2;
	incidentDir.d_param = Point3f::DotProduct(point, incidentDir);

	SetScattering(incidentDir, 0, polarizationBasis);

	Array<Facet*> facets;
	m_scattering->SelectOriginVisibleFacets(facets);

	for (int i = facets.nElems-1; i >= 0; --i)
//	for (int i = 0; i < facetIDs.size; ++i)
	{
		Facet *facet = facets.elems[i];
		QPolygonF pol;

		for (int j = 0; j < facet->nVertices; ++j)
		{
			Point3f &p = facet->arr[j];
			pol.append(QPointF(p.coordinates[0], p.coordinates[1]));
		}

		ConvertCoordinates(pol);
		visfacets.append(NumberedFacet{facet->index, pol});
	}


	// get invisible facets
//	for (int i = 0; i < m_particle->nElems; ++i)
//	{
//		bool hasFacet = false;

//		for (int j = 0; j < facets.nElems; ++j)
//		{
//			hasFacet = facets.elems[j]->index == i;
//		}

//		if (!hasFacet)
//		{
//			Facet *facet = m_particle->GetActualFacet(i);
//			QPolygonF pol;

//			for (int j = 0; j < facet->nVertices; ++j)
//			{
//				Point3f &p = facet->arr[j];
//				pol.append(QPointF(p.coordinates[0], p.coordinates[1]));
//			}

//			ConvertCoordinates(pol);
//			invisFacets.append(NumberedFacet{i, pol});
//		}
//	}
}

// BUG: не всегда правильно объединяет, пофиксить
QPolygonF ParticleProxy::Union(QVector<QPolygonF> polygons, double epsilon)
{
	QVector<QPair<int, int>> connectPoints;

	QPolygonF resPol = polygons[0];

	for (int i = 1; i < polygons.size(); ++i)
	{
		QPolygonF &pol = polygons[i];
		bool isFound = false;

		for (int j = 0; j < resPol.size(); ++j)
		{
			for (int k = 0; (k < polygons[i].size()) && !isFound; ++k)
			{
				QPointF dif = resPol[j] - pol[k];
				double length = sqrt(QPointF::dotProduct(dif, dif));

				if (length < epsilon)
				{
					connectPoints.append(QPair<int, int>(j, k));

					if (connectPoints.size() == 2)
					{
						isFound = true;
					}
				}
			}

			if (isFound) // union
			{
				bool isNear = (connectPoints[1].first - connectPoints[0].first == 1);
				int first = isNear ? 0 : 1;
				int last = isNear ? 1 : 0;

				auto pFirst = connectPoints[first];
				auto pLast = connectPoints[last];

				int resFirst = connectPoints[first].first;
				resPol.remove(resFirst);

				// determine order
				int dir = (pLast.second - pFirst.second == 1) ? -1 : 1;

				for (int l = pFirst.second; l != pLast.second; l += 1*dir)
				{
					if (l == pol.size())
					{
						l = -1;
						continue;
					}
					else if (l == -1)
					{
						l = pol.size();
						continue;
					}

					resPol.insert(resFirst++, pol[l]);
				}

				polygons.remove(i); // current polygon is already united with result polygon
				i = 0;
				connectPoints.clear();
				break;
			}
		}
	}

	// remove duplicates
	std::vector<int> remList;

	int i = 1;
	int j = resPol.size()-1;

	while (j != -1)
	{
		if (i == resPol.size())
		{
			i = 0;
			j = resPol.size()-2;
		}

		QPointF dif = resPol[i] - resPol[j];
		double length = sqrt(QPointF::dotProduct(dif, dif));

		if (length < epsilon)
		{
			remList.push_back(i);
		}

		j = i-1;
		++i;
	}

	std::sort(remList.begin(), remList.end());

	for (int k = remList.size()-1; k >= 0; --k)
	{
		resPol.remove(k);
	}

	return resPol;
}

const BeamMap &ParticleProxy::GetBeamMap() const
{
	return beamData;
}

Particle *ParticleProxy::GetParticle() const
{
	return m_particle;
}
