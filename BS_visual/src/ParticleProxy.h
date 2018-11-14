#pragma once

#include <QMap>
#include <QVector>
#include <QPolygonF>
#include <QString>
#include <QStringList>

#include "Beam.h"
#include "Rotator.h"

class Particle;
class Scattering;

struct BeamInfo
{
	BeamInfo() {}

	Beam beam;
	double thetaDeg;
	double phiDeg;
	double area;
	QString track;
	QVector<double> M;
	int number;
};

struct GlobalAngle
{
	GlobalAngle() : phi(0.f), theta(0.f), psy(0.f) {}

	GlobalAngle(double a, double b, double g)
		: phi(a), theta(b), psy(g) {}

	double phi;
	double theta;
	double psy;
};

struct NumberedFacet
{
	int num;
	QPolygonF pol;
};

struct Axes
{
	Axes()
	{
		x = Point3f(1, 0, 0);
		y = Point3f(0, 1, 0);
		z = Point3f(0, 0, 1);
	}

	Axes(const Point3f &px, const Point3f &py, const Point3f &pz)
	{
		x = px;
		y = py;
		z = pz;
	}

	Point3f x;
	Point3f y;
	Point3f z;
};

typedef QMap<QString, BeamInfo> BeamData;
typedef QMap<QString, BeamData> BeamMap;

class ParticleProxy
{
public:
	ParticleProxy();
	~ParticleProxy();

	void SetParticleFromFile(const QString &filename, int sizeIndex);
	void SetParticle(const QString &type, double refrIndex, double height,
					 double diameter, double additional = 1.0);
	void Trace(const Angle3d &angle, const Angle3d &viewAngle, int reflNum);
	void Clear();

	void RotateParticle(const Angle3d &rotAngle, const Angle3d &viewAngle);
	QVector<QPointF> RotateLocalAxes(const Angle3d &angle, const Angle3d &viewAngle);
	QVector<QPointF> RotateGlobalAxes(const Angle3d &viewAngle);

	QString GetParticleType() const;
	QStringList GetParticleTypes() const;

	void GetTrack(int beamNumber, const Angle3d &viewAngle,
				  const Angle3d &rotAngle,
				  QVector<NumberedFacet> &track,
				  QVector<QPolygonF> &trackLastFacet);
	QString GetBeamDataString();
	QString GetBeamDataString(const QString &searchLine);
	BeamInfo &GetBeamByKeys(const QString &trackKey, const QString &beamKey);
	void GetBeamByNumber(int number, BeamInfo &binfo);
	int GetTotalBeamCount();
	void GetBeamsByKey(const QString &trackKey, QVector<Beam> &beams);

	const BeamMap &GetBeamMap() const;

	Particle *GetParticle() const;
	void GetVisibleFacets(QVector<NumberedFacet> &visfacets, QVector<NumberedFacet> &invisFacets);

	void SetScattering(const Point3f &incidentDir, int reflNum,
					const Point3f &polarizationBasis);

	QString RecoverTrack(long long id, int level);
	void ConvertCoordinates(QPolygonF &pol);

	void RotatePoints(const Angle3d &angle, const std::vector<Point3f> &points,
					  std::vector<Point3f> &rotatedPoints);

private:

	enum class PType : int
	{
		RegularColumn,
		HollowColumn,
		Bullet,
		BulletRosette,
		Aggregate
	};

	QMap<PType, QString> particleTypes;

	BeamMap beamData;

	Particle *m_particle;
	Scattering *m_scattering;
	PType ptype;
	Axes localAxes;
	Axes globalAxes;

	LocalRotator m_localRotator;
	GlobalRotator m_globalRotator;

private:
	QPolygonF Union(QVector<QPolygonF> polygons, double epsilon);
	void RotateTrack(const Angle3d &angle, Rotator &rotator, Beam &beam);
	void SetBeamInfo(int beamNumber, Beam &beam, BeamInfo &info);
};
