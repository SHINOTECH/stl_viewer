#include "mainwindow.h"
#include "canvas.h"

#include <QMenuBar>
#include <QMenu>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QTimer>

#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(): model(new Model), camera() {
  // main area
  setCentralWidget(new QWidget);

  auto layout = new QHBoxLayout;
  centralWidget()->setLayout(layout);

  // menu
  QMenuBar  *menuBar  = new QMenuBar;
  QMenu     *fileMenu = menuBar->addMenu("&File");
  QAction   *openFile = new QAction("&Open STL File...",fileMenu);
  openFile->setShortcut(Qt::CTRL|Qt::Key_O);

  fileMenu->addAction(openFile);
  connect(openFile, &QAction::triggered, this, &MainWindow::onFileOpen);

  setMenuBar(menuBar);

  // canvas
  layout->addWidget((canvas = new Canvas(camera)),1);

  // UI
  auto uiLayout = new QVBoxLayout;
  layout->addLayout(uiLayout);

  auto sliderLayout = new QHBoxLayout;
  uiLayout->addLayout(sliderLayout);

  auto buttonLayout = new QVBoxLayout;
  uiLayout->addLayout(buttonLayout);

  // two sliders to rotate
  auto makeSlider = [&]() {
    auto slider = new QSlider(Qt::Vertical);
    slider->setRange(0, 360); // full circle of degrees
    slider->setSingleStep(1);
    slider->setPageStep(15);
    slider->setTickInterval(15);
    slider->setTickPosition(QSlider::TicksRight);
    sliderLayout->addWidget(slider);
    return slider;
  };

  xSlider = makeSlider();
  ySlider = makeSlider();

  // a two-state button to animate
  flybyButton = new QPushButton("flyby");
  flybyButton->setCheckable(true);
  buttonLayout->addWidget(flybyButton);

  flybyTimer = new QTimer(this);

  // a two-state button for lights
  lightsButton = new QPushButton("lights");
  lightsButton->setCheckable(true);
  buttonLayout->addWidget(lightsButton);

  lightsTimer = new QTimer(this);

  // connect all
  connect(xSlider, &QSlider::valueChanged,  this, &MainWindow::onSliderMoved);
  connect(ySlider, &QSlider::valueChanged,  this, &MainWindow::onSliderMoved);
  connect(canvas,  &Canvas::rotated,        this, &MainWindow::onCanvasRotated);

  connect(flybyButton,  &QPushButton::clicked,  this, &MainWindow::onFlybyClicked);
  connect(lightsButton, &QPushButton::clicked,  this, &MainWindow::onLightsClicked);

  connect(flybyTimer,   &QTimer::timeout,   this, &MainWindow::flyWithMe);
  connect(lightsTimer,  &QTimer::timeout,   this, &MainWindow::itsChristmas);
}

MainWindow::~MainWindow() {
}

void MainWindow::setModel(Model* m){
  model.reset(m);
  canvas->setModel(model.get());
}

void MainWindow::loadStl(QString const& fileName, bool silent) {
  if (!fileName.isEmpty()) {
    try {
      QFile file(fileName);
      if (!file.open(QFile::ReadOnly)) {
        if (silent) return;
        else Error::raise("cannot open file");
      }

      QDataStream is(&file);
      setModel(Model::fromStl(is));

    } catch (Error const& e) {
      QMessageBox::warning(this,"Error",e.message);
    }
  }
}

void MainWindow::loadExampleIfExists() {
  // if the skull is bundled, load it
  loadStl(qApp->applicationDirPath() + "/skull.stl", true);
  // and face it
  model->world.rotate(-90, V3D(1,0,0));
}

void MainWindow::onCanvasRotated() {
  QPoint rot = camera.rotatedTo();
  xSlider->setValue(rot.x());
  ySlider->setValue(rot.y());
}

void MainWindow::onSliderMoved() {
  canvas->rotateTo(QPoint(xSlider->value(),ySlider->value()));
}

void MainWindow::onFlybyClicked() {
  if (flybyButton->isChecked())
    flybyTimer->start(20);
  else
    flybyTimer->stop();
}

void MainWindow::onLightsClicked() {
  if (lightsButton->isChecked())
    lightsTimer->start(100);
  else
    lightsTimer->stop();
}

void MainWindow::flyWithMe() {
  canvas->rotateBy(QPoint(1,-1));
  onCanvasRotated();
}

void MainWindow::itsChristmas() {
  camera.changeLights();
  canvas->update();
}

void MainWindow::onFileOpen() {
  loadStl(QFileDialog::getOpenFileName(this,
    "Open STL file",QString(),"STL files (*.stl)",NULL,QFileDialog::ReadOnly));
}

// eof
