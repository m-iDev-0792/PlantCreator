#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

class ModelView :public QOpenGLWidget, protected QOpenGLFunctions
{
  Q_OBJECT
public:
  explicit ModelView(QWidget *parent = nullptr);
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
signals:

public slots:
};

#endif // MODELVIEW_H
