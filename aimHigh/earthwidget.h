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
    /**
     * @brief Draws a sphere using OpenGL and applies earth's mateial properieties for lighting.
     *
     * @param lats Number of latitude divisions for the sphere.
     * @param longs Number of longitude divisions for the sphere.
     */
    void drawSphere(int lats, int longs);
};

#endif // EARTHWIDGET_H
