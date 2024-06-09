#include "downloadManager.h"

DownloadManager::DownloadManager()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void DownloadManager::doDownload(const QUrl &url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = manager.get(request);

#if QT_CONFIG(ssl)
    connect(reply, SIGNAL(sslErrors(QList<QSslError>)),
            SLOT(sslErrors(QList<QSslError>)));
#endif

    currentDownloads.append(reply);
}

QString DownloadManager::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    QString appdata_path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    QDir dir(appdata_path);

    if (!dir.exists())
        dir.mkpath(appdata_path);

    if (basename.isEmpty())
        basename = "download";

    QString file_path = dir.absoluteFilePath(basename);

    return file_path;
}

bool DownloadManager::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    QFileInfo fi(file);

    if (!file.open(QIODevice::WriteOnly)) {
        logger.log(tr("Could not open %1 for writing: %2")
                .arg(filename)
                .arg(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

bool DownloadManager::isHttpRedirect(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    return statusCode == 301 || statusCode == 302 || statusCode == 303
           || statusCode == 305 || statusCode == 307 || statusCode == 308;
}

void DownloadManager::sslErrors(const QList<QSslError> &sslErrors)
{
#if QT_CONFIG(ssl)
    for (const QSslError &error : sslErrors)
        logger.log(tr("SSL error: %1").arg(error.errorString()));
#else
    Q_UNUSED(sslErrors);
#endif
}

void DownloadManager::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error()) {
        logger.log(tr("Download of %1 failed: %2")
                .arg(url.toEncoded().constData())
                .arg(reply->errorString()));
    } else {
        if (isHttpRedirect(reply)) {
            logger.log(tr("Request was redirected."));
        } else {
            QString filename = saveFileName(url);
            if (saveToDisk(filename, reply)) {
                logger.log(tr("Download of %1 succeeded (saved to %2)")
                       .arg(url.toEncoded().constData())
                       .arg(filename));
            }
        }
    }
    emit downloadFinished(url.toString());

    currentDownloads.removeAll(reply);
    reply->deleteLater();
}
