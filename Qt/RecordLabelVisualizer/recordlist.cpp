#include "recordlist.h"
#include "ui_recordlist.h"
#include <QtCharts/QBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSet>
#include <QtCore/QRandomGenerator>
#include <QDesktopServices>
#include <QBarCategoryAxis>
#include "logger.h"
RecordList::RecordList(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::RecordList)
{
     ui->setupUi(this);
     Logger::getInstance().connect(this);
     QObject::connect(&dmanager,SIGNAL(downloadFinished(QString)), this, SLOT(downloadFileFinished(QString)));

     /*
      * 1.Vengano scaricati automaticamente i file relativi a due etichette dai seguenti link:
      * a. Universal (http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt);
      * b. EMI (http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt);
      **/
     addRecordCompany("Universal", "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_Universal_artists.txt");
     addRecordCompany("EMI", "http://www.ivl.disco.unimib.it/minisites/cpp/List_of_EMI_artists.txt");

     /*
      * 4. Vengano creati due grafici, uno per ciascuna etichetta, in cui riportare il numero di artisti per ciascuna lettera;
      */
     countView = new QtCharts::QChartView(createEmptyBarChart());
     ui->countGraphLayout->addWidget(countView);

     /*
      * 5. Venga visualizzato un grafico in cui si riporta il numero di artisti per ciascuna etichetta.
      */
     compView = new QtCharts::QChartView(createEmptyBarChart());
     ui->compGraphLayout->addWidget(compView);
}

RecordList::~RecordList()
{
    delete ui;
}

void RecordList::addRecordCompany(const QString& name, const QString& url){
    recordListUrl.insert(name, url);
    logAppend(tr("Added new Record Company \"named\" %1 [url: %2]").arg(name).arg(url));
    dmanager.doDownload(QUrl::fromUserInput(url));
}

void RecordList::downloadFileFinished(QString url){
   QString recordLabel = recordListUrl.key(url);
    ui->recordSelector->addItem(recordLabel);
    ui->recordSelector->setEnabled(true);

    //Conto le linee del file = numero artisti
    QString basename = QFileInfo(url).fileName();

    QString appdata_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(appdata_path);
    QString fileName = dir.absoluteFilePath(basename);

    int lines = 0;
    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly)){
       QTextStream in(&inputFile);
        while (!in.atEnd()){
           in.readLine();
           lines++;
        }
    }

    //Aggiungo il numero di artisti al grafo di comparazione
    QChart* chart = compView->chart();
    QBarSeries* series = qobject_cast<QBarSeries*>(chart->series().first());
    QBarSet* bs = series->barSets().first();

    bs->append(lines); //Aggiungo il numero di artisti al set

    logAppend(tr("File %1 has %2 entries").arg(recordListUrl.key(url)).arg(QString::number(lines)));

    QBarCategoryAxis* asix = qobject_cast<QBarCategoryAxis*>(series->attachedAxes().first());
    asix->append(recordLabel); //Aggiungo il nome della casa discografica al set

    QValueAxis* asiy = qobject_cast<QValueAxis*>(series->attachedAxes().last());
    if(asiy->max() < lines){ //Correggo il range del grafico
        asiy->setMax(lines);
    }
}


void RecordList::logAppend(QString log){
    ui->logArea->append(QString("%1 %2").arg(QTime::currentTime().toString("[hh:mm:ss] ")).arg(log));
}

QChart* RecordList::createEmptyBarChart() const{
     QChart* chart = new QChart(); //Grafico da costruire
     QBarSeries* series = new QBarSeries(chart); //Serie che contiene i set
     QBarSet* bs = new QBarSet("N. Artisti");
     series->append(bs);

     chart->addSeries(series);
     chart->setAnimationOptions(QChart::SeriesAnimations);

     QBarCategoryAxis *axisX = new QBarCategoryAxis();
     chart->addAxis(axisX, Qt::AlignBottom);
     series->attachAxis(axisX);

     QValueAxis *axisY = new QValueAxis();
     axisY->setLabelFormat("%i");
     chart->addAxis(axisY, Qt::AlignLeft);
     series->attachAxis(axisY);

     return chart;
}

inline QColor rndColors(){
   return QColor::fromRgb(QRandomGenerator::global()->generate());
}

void RecordList::on_recordSelector_currentIndexChanged(const QString &recordLabel)
{
    logAppend(tr("Selected record: %1").arg(recordLabel));
    QString basename = QFileInfo(recordListUrl.value(recordLabel)).fileName();
    QString appdata_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(appdata_path);
    QString fileName = dir.absoluteFilePath(basename);

    ui->list->clear();  //Cancello la lista
    recordMap.clear();  //Cancello la mappa

    QMap<QChar, int> countMap;  //Contatore iniziali lettere


    QFile inputFile(fileName);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       QString line;
       QStringList list;
       while (!in.atEnd())
       {
          line = in.readLine();
          list = line.split(" ", QString::KeepEmptyParts);

       /*
        * 2. Vengano creati e visualizzati automaticamente i due elenchi contenenti
        *    gli artisti di ciascuna etichetta;
        **/
          if(list.size() > 1){
               ui->list->addItem(list[1]);
               recordMap.insert(list[1], list[0]);

               //Conto iniziale artista
               QChar start = list[1].toUpper().at(0);

               if(start.toUpper().toLatin1() < 65 || start.toUpper().toLatin1() > 90){
                  start = '*';
               }

               int v = countMap.contains(start) ? countMap.value(start) : 0;
               countMap.insert(start, ++v);

          }
       }
       inputFile.close();
    }

    //Aggiungo il numero di artisti al grafo di comparazione
    QChart* chart = countView->chart();
    QBarSeries* series = qobject_cast<QBarSeries*>(chart->series().first());

    series->clear(); //Cancello il set precedente
    QBarSet* bs = new QBarSet("Numero artisti"); //Creo un nuovo set

    bs->setColor(rndColors());

    QBarCategoryAxis* asix = qobject_cast<QBarCategoryAxis*>(series->attachedAxes().first());
    asix->clear(); //Cancello le label precedenti nell'asse X

    int max = 0;
    QMap<QChar, int>::Iterator it = countMap.begin();
    int others_c = -1;
    while (it != countMap.end()) {
        if(it.key() == '*'){
            others_c = it.value();
        }else{
            asix->append(it.key());
            bs->append(it.value());  //Aggiungo il numero di artisti al set
        }

        if(it.value() > max)    //Tengo traccia del numero più alto
            max = it.value();
        ++it;
    }

    if(others_c != -1){
        asix->append("Others");
        bs->append(others_c);
    }


    series->append(bs);

    QValueAxis* asiy = qobject_cast<QValueAxis*>(series->attachedAxes().last());
        asiy->setMax(max); //Correggo il range del grafico
}

/*
 * 3. Per ciascun artista sia presente l’hyperlink alla propria pagina Wikipedia.
 *    Questo punto può essere implementato sfruttando la capacità di QLabel di gestire
 *    hypertext e riutilizzando il link presente all’interno del file scaricato contenuto l’elenco degli artisti;
 */
void RecordList::on_openURLBtn_clicked()
{
    QString wikiPath = recordMap.value(ui->list->currentItem()->text());
    QString url = tr("http://wikipedia.org%1").arg(wikiPath.midRef(1, wikiPath.length()-2));
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
    logAppend(tr("Opened url: %1").arg(url));
}
