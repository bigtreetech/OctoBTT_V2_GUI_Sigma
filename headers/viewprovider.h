#ifndef VIEWPROVIDER_H
#define VIEWPROVIDER_H

#include <QQuickImageProvider>
/**************************************************
 * 图像装载库
**************************************************/
class ViewProvider : public QQuickImageProvider
{
public:
    ViewProvider();
//    void updateView(int index, const QImage &view);
    void updateView(QImage view);
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;

private:
    /**
     * @note 为什么使用 hash 而不是 vector
     *       因为这样可以扩展到更广泛的使用场景
     */
//    QHash<int, QImage> m_views;
    QImage m_views;
};

#endif // VIEWPROVIDER_H
