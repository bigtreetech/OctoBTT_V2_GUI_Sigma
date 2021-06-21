#include "../headers/viewprovider.h"
/**************************************************
 * 图像装载库
**************************************************/

ViewProvider::ViewProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{

}

//void ViewProvider::updateView(int index, const QImage &view)
//{
//    m_views[index] = view;
//}

void ViewProvider::updateView(QImage view)
{
//    m_views = QImage();
    m_views = view;
}

QImage ViewProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id);
    Q_UNUSED(size);
    Q_UNUSED(requestedSize);
    //这里可以id.left(1);
//    int index = id.left(id.indexOf("###")).toInt();
//    QImage view = m_views[index];

//    if (!view.isNull()) {
//        view.scaled(requestedSize);

//        if (size) *size = requestedSize;
//    }

//    return view;

//    if (!m_views.isNull()) {
//        m_views.scaled(requestedSize);

//        if (size) *size = requestedSize;
//    }
    return m_views;
}
