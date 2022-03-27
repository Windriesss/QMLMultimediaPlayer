#ifndef VIDEOASSET_H
#define VIDEOASSET_H

#include<QObject>
#include<QAbstractListModel>
#include<QDataStream>
#include<QQmlEngine>

class VideoAsset : public QObject
{
	Q_OBJECT
		Q_PROPERTY(QString name READ name CONSTANT)
		Q_PROPERTY(QString url READ url CONSTANT)
		Q_PROPERTY(QString coverImage READ coverImage CONSTANT)

public:
	VideoAsset();
	VideoAsset(const QString& name, const QString& url, const QString& coverImage, QObject* parent = nullptr);

	const QString& name() const;
	const QString& url() const;
	const QString& coverImage() const;

	friend QDataStream& operator>>(QDataStream& in, VideoAsset& t) {
		in >> t.m_name >> t.m_url >> t.m_coverImage;
		return in;
	}
	friend QDataStream& operator<<(QDataStream& out, const VideoAsset& t) {
		out << t.m_name << t.m_url << t.m_coverImage;
		return out;
	}

private:
	QString m_name;
	QString m_url;
	QString m_coverImage;
};




class VideoAssetModel :public QAbstractListModel {
	Q_OBJECT
public:
	Q_INVOKABLE void addVideo(const QString& url);
	Q_INVOKABLE void removeVideo(int index);
	Q_INVOKABLE void loadAssets();

private:
	QList<QSharedPointer<VideoAsset>> m_datas;
	void storeAssets();

public:
	int rowCount(const QModelIndex& parent) const override;
	QVariant data(const QModelIndex& index, int role) const override;//重写了data返回函数
	QHash<int, QByteArray> roleNames() const override;

signals:
	void testSignal();

};

void registerToQML(QQmlEngine& engine);


#endif // VIDEOASSET_H
