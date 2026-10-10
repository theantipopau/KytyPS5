#ifndef KYTY_INCLUDE_BOOT_SPLASH_H
#define KYTY_INCLUDE_BOOT_SPLASH_H

#include <QObject>
#include <QSplashScreen>
#include <QTimer>

class BootSplash: public QSplashScreen {
	Q_OBJECT

public:
	explicit BootSplash();
	void PresentFrame();

signals:
	void Finished();

private:
	QTimer* m_timer    = nullptr;
	qreal   m_progress = 0.0;
};

#endif
