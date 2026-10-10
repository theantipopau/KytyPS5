#include "bootSplash.h"

#include <QApplication>
#include <QGuiApplication>
#include <QPalette>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QScreen>
#include <QTimer>

#include <algorithm>
#include <cmath>

#include <QSplashScreen>

namespace {

constexpr int   kSplashWidth  = 480;
constexpr int   kSplashHeight = 270;
constexpr int   kBarHeight    = 3;
constexpr int   kBarWidth     = 200;

QPixmap RenderSplashFrame(const QSize& size, qreal progress) {
	QPixmap frame(size);
	frame.fill(Qt::black);

	QPainter painter(&frame);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::TextAntialiasing);

	const int w = size.width();
	const int h = size.height();

	// "Kyty" wordmark, centered slightly above the middle like the PS5 boot text.
	QFont wordmark;
	wordmark.setFamily(QStringLiteral("Helvetica Neue"));
	wordmark.setBold(true);
	wordmark.setPixelSize(h / 9);
	painter.setFont(wordmark);
	painter.setPen(QColor(240, 240, 240));

	const QString text = QStringLiteral("Kyty");
	QRect text_rect = painter.fontMetrics().boundingRect(text);
	text_rect.moveCenter(QPoint(w / 2, h / 2 - kBarHeight * 4));
	painter.drawText(text_rect, Qt::AlignCenter, text);

	// Thin progress line underneath, PS5-style.
	const int bar_x = (w - kBarWidth) / 2;
	const int bar_y = h / 2 + h / 12;
	painter.fillRect(QRect(bar_x, bar_y, kBarWidth, kBarHeight), QColor(60, 60, 60));

	const int filled = static_cast<int>(kBarWidth * std::clamp(progress, 0.0, 1.0));
	if (filled > 0) {
		painter.fillRect(QRect(bar_x, bar_y, filled, kBarHeight), QColor(240, 240, 240));
	}

	return frame;
}

} // namespace

BootSplash::BootSplash()
    : QSplashScreen(QPixmap(), Qt::SplashScreen | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint),
      m_timer(nullptr),
      m_progress(0.0) {
	const auto screen_geometry = QGuiApplication::primaryScreen()->geometry();
	setGeometry(QRect(screen_geometry.center() - QPoint(kSplashWidth / 2, kSplashHeight / 2),
	                  QSize(kSplashWidth, kSplashHeight)));
	setAutoFillBackground(true);

	QPalette splash_palette = palette();
	splash_palette.setColor(QPalette::Window, Qt::black);
	setPalette(splash_palette);

	auto* timer = new QTimer(this);
	timer->setInterval(1000 / 30);
	connect(timer, &QTimer::timeout, this, [this, timer] {
		m_progress += 0.02;
		if (m_progress > 1.0) {
			m_progress = 1.0;
		}
		PresentFrame();
		if (m_progress >= 1.0 && m_timer != nullptr) {
			m_timer = nullptr;
			timer->stop();
			timer->deleteLater();
			QTimer::singleShot(700, this, [this] { emit Finished(); });
		}
	});
	m_timer = timer;
	timer->start();
}

void BootSplash::PresentFrame() {
	auto frame = RenderSplashFrame(QSize(kSplashWidth, kSplashHeight), m_progress);
	QSplashScreen::setPixmap(frame);
	repaint();
}
