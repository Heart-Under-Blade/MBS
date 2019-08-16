#include "ScatteringScene.h"

#include "ParticleProxy.h"

#include <QMouseEvent>
#include <QDebug>
#include <QGraphicsTextItem>
#include <QMatrix4x4>

struct VertexData
{
	QVector3D position;
	QVector2D texCoord;
};

ScatteringScene::ScatteringScene()
	: indexBuf(QOpenGLBuffer::IndexBuffer)
{
	isDrawLocalAxes = false;
	textSize = 5;
	mainPen.setColor(Qt::black);

	dashPen.setColor(Qt::gray);
	dashPen.setStyle(Qt::DashLine);
	dashPen.setDashPattern(QVector<qreal>{static_cast<double>(textSize),
										  static_cast<double>(textSize*2)});
	zoomFactor = 0.1;
//	scene = new QGraphicsScene(this);
//	setScene(scene);
//	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
//	initializeGL();
//	paintGL();
	initializeOpenGLFunctions();

	arrayBuf.create();
	indexBuf.create();

	initGeometry();
}

ScatteringScene::~ScatteringScene()
{
	arrayBuf.destroy();
	indexBuf.destroy();
}

void ScatteringScene::DrawTrack(const SceneData &particle)
{
//	auto &track = particle.track;
//	auto &facet = particle.trackLastFacet;

//	if (track.isEmpty())
//	{
//		return;
//	}

//	QPen pen(Qt::red);

//	QPen pen2 = dashPen;
//	pen2.setColor(Qt::red);

//	// polygons
////	for (const NumberedFacet &pol : track)
////	{
////		scene->addPolygon(pol.pol, pen, QBrush(Qt::green));
////	}

//	foreach (const QPolygonF p, facet)
//	{
//		scene->addPolygon(p, pen, QBrush(Qt::green));
//	}

////	scene->addPolygon(track.at(track.size()-2).pol, pen, QBrush(Qt::green));

//	// lines
//	QPointF c1 = track.first().pol.at(0);
//	QPointF c2 = CenterOfPolygon(track.at(1).pol);
//	scene->addLine(c1.x(), c1.y(), c2.x(), c2.y(), pen);
////	scene->addEllipse(c1.x()-2, c1.y()-2, 4, 4, pen, QBrush(Qt::green));

//	for (int i = 2; i < track.size(); ++i)
//	{
//		c1 = c2;
//		c2 = CenterOfPolygon(track.at(i).pol);
//		scene->addLine(c1.x(), c1.y(), c2.x(), c2.y(), pen2);
//		scene->addEllipse(c1.x()-2, c1.y()-2, 4, 4, pen, QBrush(Qt::red));
//	}

//	c1 = CenterOfPolygon(track.at(track.size()-2).pol);
//	c2 = track.last().pol.at(0);
//	scene->addLine(c1.x(), c1.y(), c2.x(), c2.y(), pen);
////	scene->addEllipse(c2.x()-2, c2.y()-2, 4, 4, pen, QBrush(Qt::green));

//	// numbers
//	QFont font;
//	font.setBold(true);

//	QPointF c3 = CenterOfPolygon(track.at(track.size()-1).pol);
//	DrawColoredText("in", track.at(0).pol.at(0), mainPen.color(), font);
//	scene->addEllipse(c3.x()-2, c3.y()-2, 4, 4, pen, QBrush(Qt::yellow));

//	for (int i = 1; i < track.size()-1; ++i)
//	{
//		DrawFacetNumber(track.at(i), mainPen.color());
//	}

////	DrawColoredText("out", track.last().pol.at(0), mainPen.color(), font);
}

void ScatteringScene::DrawScene(const SceneData &sceneData,
							 bool drawNumbers, bool drawAxes)
{
//	scene->clear();
////	QBrush brush(QColor(QRgba64::fromRgba(0, 0, 255, 255/(1/sqrt(refrIndex*4)))));

//	DrawFacets(sceneData.invisibleFacets, drawNumbers, dashPen);

//	DrawFacets(sceneData.visibleFacets, drawNumbers,
//			   QPen(Qt::blue), QBrush(Qt::cyan));

//	DrawTrack(sceneData);

//	if (drawAxes)
//	{
//		DrawAxes(sceneData.globalAxes, QPen(Qt::red), "w");
//	}

//	if (isDrawLocalAxes)
//	{
//		QPen pen = dashPen;
//		pen.setColor(Qt::blue);
//		DrawAxes(sceneData.localAxes, pen);
//	}
}

QPointF ScatteringScene::CenterOfPolygon(const QPolygonF &pol)
{
	QPointF center;

	foreach (QPointF p, pol)
	{
		center.setX(center.x() + p.x());
		center.setY(center.y() + p.y());
	}

	center /= pol.size();
	return center;
}

void ScatteringScene::DrawColoredText(const QString &text, const QPointF &pos,
								   const QColor &color, QFont font)
{
//	font.setPointSize(textSize);
//	QGraphicsTextItem *facetNumber = scene->addText(text, font);

//	facetNumber->setDefaultTextColor(color);
//	QPointF textCenter = facetNumber->boundingRect().center();
//	double x = pos.x() - textCenter.x();
//	double y = pos.y() - textCenter.y();
//	facetNumber->setPos(x, y);
}

void ScatteringScene::DrawFacetNumber(const NumberedFacet &facet, const QColor &color)
{
	QPointF center = CenterOfPolygon(facet.pol);
	QString numText = QString::number(facet.num);
	DrawColoredText(numText, center, color);
}

void ScatteringScene::DrawAxis(const QPointF &axis, const QString &letter,
							const QPen &pen)
{
//	double x = axis.x();
//	double y = axis.y();
//	scene->addLine(0, 0, x, y, pen);

//	QPointF d = axis/20;
//	QPointF vecToArrow = axis - d;

//	QGraphicsItem *text = scene->addText(letter);
//	text->moveBy(x, y);

//	double len = sqrt(x*x + y*y);
//	double arrowSize = len/15;

//	QPolygonF arrow;
//	arrow << QPointF(x, y) << QPointF(x-arrowSize, y-arrowSize);
}

void ScatteringScene::DrawAxes(const QVector<QPointF> &axes, const QPen &pen,
							const QString &suffix)
{
	double size = textSize*2;

	DrawAxis(axes[0], "X" + suffix, pen);
	DrawAxis(axes[1], "Y" + suffix, pen);
	DrawAxis(axes[2], "Z" + suffix, pen);
//	QPolygonF arrowX;
//	arrowX << QPointF(size, 0) << QPointF(size-arrowSize, arrowSize/4)
//		   << QPointF(size-arrowSize, -arrowSize/4) << QPointF(size, 0);
//	scene->addPolygon(arrowX, redPen, redBrush);

//	QPolygonF arrowY;
//	arrowY << QPointF(0, size) << QPointF(arrowSize/4, size-arrowSize)
//		   << QPointF(-arrowSize/4, size-arrowSize) << QPointF(0, size);
//	scene->addPolygon(arrowY, redPen, redBrush);
//	scene->addEllipse(-size, -size, size*2, size*2, pen);
}

void ScatteringScene::setFacets(const QVector<NumberedFacet> &facets, bool drawNumbers,
								const QPen &pen, const QBrush &brush)
{
	glBegin(GL_POLYGON);

	glEnd();

//	for (int i = 0; i < facets.size(); ++i)
//	{
//		scene->addPolygon(facets[i].pol, pen, brush);

//		if (drawNumbers)
//		{
//			DrawFacetNumber(facets[i], pen.color());
//		}
	//	}
}

void ScatteringScene::drawFacets()
{
	glBegin(GL_POLYGON);

	glColor3f( 1.0, 0.0, 0.0 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glColor3f( 0.0, 1.0, 0.0 );
	glVertex3f(  0.5,  0.5, -0.5 );
	glColor3f( 0.0, 0.0, 1.0 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glColor3f( 1.0, 0.0, 1.0 );
	glVertex3f( -0.5, -0.5, -0.5 );

	glEnd();
}

//void ScatteringScene::wheelEvent()
//{
//	QPoint wheel = event->angleDelta();

//	double zoomIn = 1 + zoomFactor;
//	double zoomOut = 1 - zoomFactor;

//	if (!wheel.isNull())
//	{
//		if (wheel.ry() > 0)
//		{
//			scale(zoomIn, zoomIn);
//		}
//		else
//		{
//			scale(zoomOut, zoomOut);
//		}
//	}

//}

void ScatteringScene::initGeometry()
{
	// For cube we would need only 8 vertices but we have to
	// duplicate vertex for each face because texture coordinate
	// is different.
	VertexData vertices[] = {
		// Vertex data for face 0
		{QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.0f, 0.0f)},  // v0
		{QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.0f)}, // v1
		{QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.0f, 0.5f)},  // v2
		{QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v3

		// Vertex data for face 1
		{QVector3D( 1.0f, -1.0f,  1.0f), QVector2D( 0.0f, 0.5f)}, // v4
		{QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.5f)}, // v5
		{QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.0f, 1.0f)},  // v6
		{QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v7

		// Vertex data for face 2
		{QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v8
		{QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(1.0f, 0.5f)},  // v9
		{QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}, // v10
		{QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(1.0f, 1.0f)},  // v11

		// Vertex data for face 3
		{QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v12
		{QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(1.0f, 0.0f)},  // v13
		{QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.66f, 0.5f)}, // v14
		{QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(1.0f, 0.5f)},  // v15

		// Vertex data for face 4
		{QVector3D(-1.0f, -1.0f, -1.0f), QVector2D(0.33f, 0.0f)}, // v16
		{QVector3D( 1.0f, -1.0f, -1.0f), QVector2D(0.66f, 0.0f)}, // v17
		{QVector3D(-1.0f, -1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v18
		{QVector3D( 1.0f, -1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v19

		// Vertex data for face 5
		{QVector3D(-1.0f,  1.0f,  1.0f), QVector2D(0.33f, 0.5f)}, // v20
		{QVector3D( 1.0f,  1.0f,  1.0f), QVector2D(0.66f, 0.5f)}, // v21
		{QVector3D(-1.0f,  1.0f, -1.0f), QVector2D(0.33f, 1.0f)}, // v22
		{QVector3D( 1.0f,  1.0f, -1.0f), QVector2D(0.66f, 1.0f)}  // v23
	};

	// Indices for drawing cube faces using triangle strips.
	// Triangle strips can be connected by duplicating indices
	// between the strips. If connecting strips have opposite
	// vertex order then last index of the first strip and first
	// index of the second strip needs to be duplicated. If
	// connecting strips have same vertex order then only last
	// index of the first strip needs to be duplicated.
	GLushort indices[] = {
		 0,  1,  2,  3,  3,     // Face 0 - triangle strip ( v0,  v1,  v2,  v3)
		 4,  4,  5,  6,  7,  7, // Face 1 - triangle strip ( v4,  v5,  v6,  v7)
		 8,  8,  9, 10, 11, 11, // Face 2 - triangle strip ( v8,  v9, v10, v11)
		12, 12, 13, 14, 15, 15, // Face 3 - triangle strip (v12, v13, v14, v15)
		16, 16, 17, 18, 19, 19, // Face 4 - triangle strip (v16, v17, v18, v19)
		20, 20, 21, 22, 23      // Face 5 - triangle strip (v20, v21, v22, v23)
	};

	// Transfer vertex data to VBO 0
	arrayBuf.bind();
	arrayBuf.allocate(vertices, 24 * sizeof(VertexData));

	// Transfer index data to VBO 1
	indexBuf.bind();
	indexBuf.allocate(indices, 34 * sizeof(GLushort));
}

void ScatteringScene::drawGeometry(QOpenGLShaderProgram *program)
{
	arrayBuf.bind();
	indexBuf.bind();

	// Offset for position
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLE_STRIP, 34, GL_UNSIGNED_SHORT, 0);
}

void ScatteringScene::setDrawLocalAxes(bool value)
{
	isDrawLocalAxes = value;
}

