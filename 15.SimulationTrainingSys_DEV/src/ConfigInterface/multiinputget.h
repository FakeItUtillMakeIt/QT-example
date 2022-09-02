#ifndef MULTIINPUTGET_H
#define MULTIINPUTGET_H

#include <QDialog>

namespace Ui {
class MultiInputGet;
}

class MultiInputGet : public QDialog
{
    Q_OBJECT

public:
    explicit MultiInputGet(QWidget *parent = 0);
    ~MultiInputGet();
	bool get_result() { return  m_accepted; }
	bool get_x_range(double& xmin, double& xmax);
	void update_range(double xmin, double xmax,double ymin,double ymax);

	bool get_y_range(double& ymin, double& ymax);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::MultiInputGet *ui;
	bool m_accepted = false;
};

#endif // MULTIINPUTGET_H
