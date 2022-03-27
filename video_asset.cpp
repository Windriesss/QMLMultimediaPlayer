#include "video_asset.h"

#include <QFile>
#include <QFileInfo>
#include <QQmlEngine>
#include <QQuickImageProvider>

#include "video_info_fetcher.h"

QString g_image_provider_name = "cover_image";
QString g_image_name_prefix   = "image://" + g_image_provider_name + "/";

class CoverImageProvider : public QQuickImageProvider
{
public:
    CoverImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}

    QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) override
    {
        return QPixmap(id);
    }
};

VideoAsset::VideoAsset() {}

VideoAsset::VideoAsset(const QString& name, const QString& url, const QString& coverImage, QObject* parent) : m_name(name), m_url(url), m_coverImage(coverImage)
{}

const QString& VideoAsset::name() const
{
    return m_name;
}

const QString& VideoAsset::url() const
{
    return m_url;
}

const QString& VideoAsset::coverImage() const
{
    return m_coverImage;
}

void registerToQML(QQmlEngine& engine)
{
    qmlRegisterType<VideoAssetModel>("com.ghj.asset", 1, 0, "VideoAssetModel");
    engine.addImageProvider(g_image_provider_name.toLatin1(), new CoverImageProvider);
}

void VideoAssetModel::addVideo(const QString& url)
{
    QStringList parts    = url.split("/");
    QString     filename = *(parts.end() - 1);
    //取封面
    fetchFirstFrameFromVideoUrl(url, [=](const QImage& image) {
        QString   imageName = filename.split(".").at(0) + ".png";
        QFileInfo info(imageName);
        int       i = 1;
        while (info.exists()) {
            imageName = filename.split(".").at(0) + QString::number(i++) + ".png";
            info      = QFileInfo(imageName);
        }
        bool result = image.save(imageName, "PNG");
        auto rowcnt = m_datas.count();
        beginInsertRows(QModelIndex(), rowcnt, rowcnt);
        auto asset = QSharedPointer<VideoAsset>(new VideoAsset(filename, url, g_image_name_prefix + imageName));
        m_datas.append(asset);
        endInsertRows();
        storeAssets();
    });
}

void VideoAssetModel::removeVideo(int index)
{
    if (index < m_datas.count()) {
        beginRemoveRows(QModelIndex(), index, index);
        auto asset = m_datas[index];
        m_datas.remove(index);
        endRemoveRows();
        QString imageName = asset->coverImage();
        imageName         = imageName.remove(g_image_name_prefix);
        QFile file(imageName);
        if (file.exists()) {
            file.remove();
        }
        storeAssets();
    }
}

void VideoAssetModel::loadAssets()
{
    QFile file("playlist");
    if (file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly)) {
        QDataStream in(&file);
        while (!in.atEnd()) {
            beginInsertRows(QModelIndex(), m_datas.size(), m_datas.size());
            auto asset = QSharedPointer<VideoAsset>(new VideoAsset);
            in >> *asset;
            m_datas.push_back(asset);
            endInsertRows();
        }

    } else {
        qDebug() << "load Assets error!!!";
    }
}

void VideoAssetModel::storeAssets()
{
    QFile file("playlist");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        for (const auto& t : m_datas) {
            out << *t;
        }
        file.close();
    }
}

int VideoAssetModel::rowCount(const QModelIndex& parent) const
{
    return m_datas.count();
}

QVariant VideoAssetModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)    //没有别的用处，只是用于避免编译器警告
    return QVariant::fromValue(m_datas[index.row()].data());
}

QHash<int, QByteArray> VideoAssetModel::roleNames() const
{
    QHash<int, QByteArray> hash;
    hash[0] = "asset";
    return hash;
}
