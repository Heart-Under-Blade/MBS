#pragma once

#include <QMainWindow>
#include <QDialog>
#include <QChartView>

#include "MyGLWidget.h"
#include "SettingsDialog.h"
#include "ParticleProxy.h"
#include "BeamDirectionChart.h"

class BeamItemModel;
class ScatteringScene;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_comboBox_types_currentIndexChanged(int);
	void on_pushButton_clicked();
	void on_treeView_tracks_clicked(const QModelIndex &index);
	void on_lineEdit_search_textChanged(const QString &arg1);
	void on_toolButton_resetRot_clicked();
	void on_toolButton_resetView_clicked();
	void on_toolButton_expandTree_clicked();

	void UpdateScene();
	void UpdateScene(int);
	void UpdateScene(double);

	void ParticleChanged(int);
	void ParticleChanged(double);

	void OpenSettingsDialog(bool);
	void AcceptSettings();

	void on_toolButton_unit_clicked();

private:
	Ui::MainWindow *ui;
	ParticleProxy *p_proxy;
	BeamItemModel *model;
	QChartView *chartView;
	SettingsDialog *settingsDialog;

	MyGLWidget *particleView;

	char format;
	QMap<QString, QVariant> state;
	double coordinateOffset = 90;
	int beamNumber;
	bool drawTrack;
	bool isTreeExpanded;
	bool mustConvertAngle = true;

	Settings settings;
	BeamDirectionChart *dirChart;
	QWidget *widget;

private:
	void FillParticleTypes();
	void SetAdditionalParam();
	void SetDirectionChart();
	void SetBeamTree();
	Angle3d GetRotateAngle();
	Angle3d GetViewAngle();
	void SetParticle();
	void LoadSession();
	void SaveSession();
	void ConnectWidgets();
	void SetParticleView();
	void FillResultBeamData(const BeamInfo &info);
	void SetViewAngle(const GlobalAngle &value);
	void DeleteModel();
	void GetDirections(QMap<int, QPointF> &directions);
	void SetInputPrecisions();
};
