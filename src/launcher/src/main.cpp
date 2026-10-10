#include "mainDialog.h"
#include "bootSplash.h"
#include "launcherTheme.h"

#include <QApplication>
#include <QTimer>

int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	LauncherTheme::Initialize(a);

	auto* splash = new BootSplash;
	splash->show();
	splash->PresentFrame();

	MainDialog w;

	w.emit Start();

	// Show the game list only after the boot animation finishes.
	QTimer::singleShot(2600, &a, [&w, splash] {
		if (splash != nullptr) {
			splash->close();
			splash->deleteLater();
		}
		w.show();
		w.raise();
		w.activateWindow();
	});

	return QApplication::exec();
}
