#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QGraphicsScene>
#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
  m_pGraphicsScene(new QGraphicsScene(this)),
  xDim(50),
  yDim(30),
  fieldSize(15)
{
    ui->setupUi(this);
    ui->m_pGraphicsView->setScene(m_pGraphicsScene);

    showMaximized();

    qsrand(QTime::currentTime().msec());

    for (int j=0; j<xDim; ++j) {
      QList<bool> col;
      for (int i=0; i<yDim; ++i) {
        col << false;
      }
      m_lstLife << col;
    }

    const int numLife = 250;
    Q_ASSERT(numLife < xDim * yDim);

    int xRand, yRand;
    int count = 0;
    while (true) {
      xRand = qrand() % xDim;
      yRand = qrand() % yDim;

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


  for (int j=0; j<xDim; ++j) {
    QList<bool> col;
    for (int i=0; i<yDim; ++i) {
      col << false;
    }
    newFields << col;
  }


  int nNeighbours;
  for (int j=0; j<xDim; ++j) {
    for (int i=0; i<yDim; ++i) {
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
    if (0 > j + k || xDim <= j + k) {
      continue;
    }
    for (int i=-1; i<=1; ++i) {
      if (i == 0 && j == 0) { // the field itself
        continue;
      }

      if (0 > i + l || yDim <= i + l) {
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
  for (int i=0; i<=xDim; ++i) {
    m_pGraphicsScene->addLine(10 + i * fieldSize ,10, 10 + i * fieldSize, yDim * fieldSize +10);
  }

  // Horizontal lines
  for (int j=0; j<=yDim; ++j) {
    m_pGraphicsScene->addLine(10, 10 + j * fieldSize ,xDim * fieldSize + 10 ,10 + j * fieldSize);
  }


  for (int j=0; j<xDim; ++j) {
    for (int i=0; i<yDim; ++i) {
      if (m_lstLife[j][i]) {
        m_pGraphicsScene->addRect(10 + fieldSize * j, 10 + fieldSize * i, fieldSize, fieldSize, QPen(), QBrush(Qt::black));
      }
    }
  }




  m_pGraphicsScene->update();


}
