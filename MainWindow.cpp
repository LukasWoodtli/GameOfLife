#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  mc_iXDim(70),
  mc_iYDim(40),
  mc_iFieldSize(15),
  ui(new Ui::MainWindow),
  m_pGraphicsScene(new QGraphicsScene(this))
{
    ui->setupUi(this);
    ui->m_pGraphicsView->setScene(m_pGraphicsScene);

    showMaximized();

    qsrand(QTime::currentTime().msec());

    initField();

    setRandomField();

    drawField();

    startTimer(100);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent ( QTimerEvent * event ) {
  recalculateFields();
  drawField();
}

void MainWindow::recalculateFields() {
  QList<QList<bool> > newFields;


  for (int j=0; j<mc_iXDim; ++j) {
    QList<bool> col;
    for (int i=0; i<mc_iYDim; ++i) {
      col << false;
    }
    newFields << col;
  }


  int nNeighbours;
  for (int j=0; j<mc_iXDim; ++j) {
    for (int i=0; i<mc_iYDim; ++i) {
      nNeighbours = numNeighbours(j, i);

      if (!m_lstLife[j][i] && 3 == nNeighbours) {
        newFields[j][i] = true;
      }

      else if (m_lstLife[j][i] && 2 > nNeighbours) {
        newFields[j][i] = false;
      }

      else if (m_lstLife[j][i] && (2 == nNeighbours || 3 == nNeighbours)) {
        newFields[j][i] = true;
      }

      else if (m_lstLife[j][i] && 3 < nNeighbours) {
        newFields[j][i] = false;
      }
  }
}


  m_lstLife = newFields;

}

quint8 MainWindow::numNeighbours(quint8 k, quint8 l) {
  quint8 nNeighbours = 0;

  for (int j=-1; j<=1; ++j) {
    if (0 > j + k || mc_iXDim <= j + k) {
      continue;
    }
    for (int i=-1; i<=1; ++i) {
      if (i == 0 && j == 0) { // the field itself
        continue;
      }

      if (0 > i + l || mc_iYDim <= i + l) {
        continue;
      }

      if (m_lstLife[j + k][i + l]) {
        nNeighbours++;
      }
    }
  }

  Q_ASSERT(8 >= nNeighbours);

  return nNeighbours;
}

void MainWindow::drawField() {

  m_pGraphicsScene->clear();

  // Vertical lines
  for (int i=0; i<=mc_iXDim; ++i) {
    m_pGraphicsScene->addLine(10 + i * mc_iFieldSize ,10, 10 + i * mc_iFieldSize, mc_iYDim * mc_iFieldSize +10);
  }

  // Horizontal lines
  for (int j=0; j<=mc_iYDim; ++j) {
    m_pGraphicsScene->addLine(10, 10 + j * mc_iFieldSize ,mc_iXDim * mc_iFieldSize + 10 ,10 + j * mc_iFieldSize);
  }


  for (int j=0; j<mc_iXDim; ++j) {
    for (int i=0; i<mc_iYDim; ++i) {
      if (m_lstLife[j][i]) {
        m_pGraphicsScene->addRect(10 + mc_iFieldSize * j, 10 + mc_iFieldSize * i, mc_iFieldSize, mc_iFieldSize, QPen(), QBrush(Qt::black));
      }
    }
  }

  m_pGraphicsScene->update();
}


void MainWindow::initField() {
  for (int j=0; j<mc_iXDim; ++j) {
    QList<bool> col;
    for (int i=0; i<mc_iYDim; ++i) {
      col << false;
    }
    m_lstLife << col;
  }
}

void MainWindow::setRandomField() {
    static const int numLife = 650;
    Q_ASSERT(numLife < mc_iXDim * mc_iYDim);

    int xRand, yRand;
    int count = 0;
    while (true) {
      xRand = qrand() % mc_iXDim;
      yRand = qrand() % mc_iYDim;

      if (m_lstLife[xRand][yRand] != true) {
        m_lstLife[xRand][yRand] = true;
        count++;
      }
      else
      {
        continue;
      }
      if (count >= numLife)
      {
        break;
      }
    }
}
