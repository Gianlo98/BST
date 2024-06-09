#ifndef RECORDLIST_H
#define RECORDLIST_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include "downloadManager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class RecordList; }
class QFile;
QT_END_NAMESPACE

QT_CHARTS_BEGIN_NAMESPACE
class QChart;
QT_CHARTS_END_NAMESPACE

typedef QPair<int, QString> Data;
typedef QList<Data> DataList;

QT_CHARTS_USE_NAMESPACE

class RecordList : public QMainWindow, public LoggerOutput
{
    Q_OBJECT

public:
    RecordList(QWidget *parent = nullptr);
    ~RecordList();
    void logAppend(QString l);
    void addRecordCompany(const QString& name, const QString& url);

private slots:
    void on_openURLBtn_clicked();
    void downloadFileFinished(QString file); //Custom
    void on_recordSelector_currentIndexChanged(const QString &arg1);

private:
    QChart *createBarChart() const;
    QChart* createEmptyBarChart() const;

private:
    Ui::RecordList* ui;
    DownloadManager dmanager;
    QtCharts::QChartView *countView; //Grafico n. artisti
    QtCharts::QChartView *compView; //Grafico comparazione
    QMap<QString, QString> recordListUrl; // Case discografiche url
    QMap<QString, QString> recordMap; //Ottenere URL da nome
};
#endif // RECORDLIST_H
