#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ModelModel;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	bool readAcmiFile(const QString &fileName);

	Ui::MainWindow		*ui;
	ModelModel*		m_ModelModel;

private slots:
	void on_horizontalSlider_valueChanged(int value);
	void on_actionOpen_triggered();
	void on_actionAbout_triggered();
	void on_comboBox_currentIndexChanged(int);
};

#endif // MAINWINDOW_H
