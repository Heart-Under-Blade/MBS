#pragma once

#include <QPen>
#include <QFont>
#include <QOpenGLShaderProgram>

#include "ParticleModel.h"

class NumberedFacet;

struct SceneData
{
	double refrIndex;
	QVector<QPointF> localAxes;
	QVector<QPointF> globalAxes;
	QVector<NumberedFacet> track;
	QVector<QPolygonF> trackLastFacet;
	QVector<NumberedFacet> visibleFacets;
	QVector<NumberedFacet> invisibleFacets;
};

class ScatteringScene : protected QOpenGLFunctions
{
public:
	ScatteringScene();
	~ScatteringScene();

	void drawGeometry(QOpenGLShaderProgram *program);

	void DrawScene(const SceneData &sceneData,
				   bool drawNumbers, bool drawAxes);

	void setDrawLocalAxes(bool value);

	// QWidget interface
protected:
//	void wheelEvent(QWheelEvent *event) override;

private:
	double zoomFactor;

	QPen mainPen;
	QPen dashPen;

	int textSize;
	bool isDrawLocalAxes;

	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;

//	QOpenGLContext *m_context;

private:
	void initGeometry();

	void setFacets(const QVector<NumberedFacet> &facets, bool drawNumbers,
					const QPen &pen, const QBrush &brush = QBrush());
	void drawFacets();

	void DrawFacetNumber(const NumberedFacet &facet, const QColor &color);
	void DrawAxis(const QPointF &axis, const QString &letter,
				  const QPen &pen = QPen());
	void DrawAxes(const QVector<QPointF> &axes, const QPen &pen,
				  const QString &suffix = QString());
	void DrawTrack(const SceneData &particle);
	QPointF CenterOfPolygon(const QPolygonF &pol);
	void DrawColoredText(const QString &text, const QPointF &pos,
						 const QColor &color, QFont font = QFont());

};
