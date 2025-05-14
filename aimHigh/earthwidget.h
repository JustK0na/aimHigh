// earthwidget.h

/**
 * @file earthwidget.h
 * @brief OpenGL based Qt class, displays 3D sphere that is model of Earth.
 */

#ifndef EARTHWIDGET_H
#define EARTHWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
/**
 * @class earthWidget
 * @brief Creates and draws sphere and its lighting based on OpenGl.
 */
class earthWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    /**
     * @brief  EarthWidget Constructor.
     */
    explicit earthWidget(QWidget *parent = nullptr);
    void setISSPosition(float latitude, float longitude, float altitude);

public slots:
    /**
     * @brief Slot that updates ISS data whenever new arrives.
     * @param longitude Current longitude of the ISS.
     * @param latitude  Current latitude of the ISS.
     * @param altitude  Altitude in kilometers.
     * @param velocity  Velocity in km/h.
     * @param timestamp UTC timestamp since 1970 in seconds.
     */
    void updateISSData(double longitude, double latitude, double altitude, double velocity, double timestamp);
protected:
    /**
     * @brief Initializes OpenGL settings and enables simple lighting.
     */
    void initializeGL() override;


    /**
     * @brief Resizes the OpenGL viewport its called every time Widget needs to be resized.
     *
     * @param w The new width of the widget.
     * @param h The new height of the widget.
     */
    void resizeGL(int w, int h) override;
    /**
     * @brief Renders the 3D scene, drawing the sphere by calling drawSphere().
     *
     */
    void paintGL() override;


private:


    float issLat = 0.0; ///<Object ISS Latitude
    float issLon = 0.0; ///<Object ISS Longitude
    float issAlt = 0.0; ///<Object ISS Altitude

    /**
     * @brief Draws a sphere using OpenGL and applies earth's mateial properieties for lighting.
     *
     * @param lats Number of latitude divisions for the sphere.
     * @param longs Number of longitude divisions for the sphere.
     * @param r radius of the sphere
     * @param color color of the sphere, in QColor type
     */
    void drawSphere(int lats, int longs, float r, const QColor &color);
};

#endif // EARTHWIDGET_H
