#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

class QGraphicsScene;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();


protected:
  void timerEvent(QTimerEvent *event);

private:

  void recalculateFields();
  quint8 numNeighbours(quint8 k, quint8 l);

  void drawField();

  const quint8 xDim;

  const quint8 yDim;
    const quint8 fieldSize;

    QList<QList<bool> > m_lstLife;


    Ui::MainWindow *ui;

    QGraphicsScene *m_pGraphicsScene;
};

#endif // MAINWINDOW_H
