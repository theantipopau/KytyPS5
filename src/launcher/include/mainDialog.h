#ifndef MAIN_DIALOG_H
#define MAIN_DIALOG_H

#include <QDialog>
#include <QString>

class QWidget;
class QProcess;
class MainDialogPrivate;
class QSettings;
class QResizeEvent;

class Configuration;
class MainDialog: public QDialog {
	Q_OBJECT

signals:
	void Start();
	void Resize();

public:
	explicit MainDialog(QWidget* parent = nullptr);
	~MainDialog() override = default;

	void RunInterpreter(QProcess* process, const Configuration& info);

	static void WriteSettings(QSettings& s);
	static void ReadSettings(QSettings& s);

	[[nodiscard]] static bool SkipBootSplash() { return m_skip_boot_splash; }
	static void              SetSkipBootSplash(bool skip) { m_skip_boot_splash = skip; }

	void resizeEvent(QResizeEvent* event) override;

private:
	static bool        m_skip_boot_splash;
	MainDialogPrivate* m_p = nullptr;
};

#endif // MAIN_DIALOG_H
