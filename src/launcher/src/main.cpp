#include "mainDialog.h"
#include "bootSplash.h"
#include "launcherTheme.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <QTimer>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	LauncherTheme::Initialize(a);

	// Read the skip-splash preference directly from Kyty.ini ([Launcher]/skip_boot_splash).
	if (QFile::exists(QStringLiteral("Kyty.ini"))) {
		QSettings settings(QStringLiteral("Kyty.ini"), QSettings::IniFormat);
		MainDialog::SetSkipBootSplash(settings.value("Launcher/skip_boot_splash", false).toBool());
	}

	auto* splash = new BootSplash;

	if (splash != nullptr && !MainDialog::SkipBootSplash()) {
		splash->show();
		splash->PresentFrame();
	}

	MainDialog w;

	w.emit Start();

	// Show the game list only after the boot animation finishes (or immediately when skipped).
	QTimer::singleShot(splash->isVisible() ? 2600 : 0, &a, [&w, splash] {
		if (splash != nullptr && splash->isVisible()) {
			splash->close();
			splash->deleteLater();
		}
		w.show();
		w.raise();
		w.activateWindow();
	});
	if (splash == nullptr || !splash->isVisible()) {
		splash->deleteLater();
		splash = nullptr;
		QTimer::singleShot(0, &a, [&w] {
			w.show();
			w.raise();
			w.activateWindow();
		});
	}

	return QApplication::exec();
}
