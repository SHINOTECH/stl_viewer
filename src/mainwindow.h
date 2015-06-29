#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camera.h"
#include "model.h"

class Canvas;
class QSlider;
class QPushButton;
class QTimer;

/*
 * The application window.
 */

class MainWindow: public QMainWindow {
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

  // Sets a new model.
  // Deletes the previous model, and takes ownership of the new one.
  void setModel(Model*);

  // Creates a new model from an STL file. Deletes the previous model.
  // silent: supresses the error messages if the file cannot be accessed
  void loadStl(QString const& fileName, bool silent=false);

  // Loads a bundled example model.
  void loadExampleIfExists();

private slots:
  void onCanvasRotated();
  void onSliderMoved();
  void onFlybyClicked();
  void onLightsClicked();

  void flyWithMe();
  void itsChristmas();

private:
  // The model is held here. Never NULL;
  model_ptr model;

  // camera and canvas
  Camera    camera;
  Canvas   *canvas;

  // UI elements
  QSlider  *xSlider, *ySlider;
  QPushButton *flybyButton, *lightsButton;
  QTimer   *flybyTimer, *lightsTimer;

private slots:
  // menu action
  void onFileOpen();
};

#endif
