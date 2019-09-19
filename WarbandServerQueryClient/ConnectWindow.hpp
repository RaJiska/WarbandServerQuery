#ifndef CONNECTWINDOW_HPP
#define CONNECTWINDOW_HPP

#include <QDialog>
#include <QtConcurrent/QtConcurrent>

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = 0);
    ~ConnectWindow() = default;

public slots:
    void connectToServer(void);

private:
    Ui::ConnectWindow *ui;
    QWidget *wParent;
    QFuture<void> future;
};

#endif // CONNECTWINDOW_HPP
