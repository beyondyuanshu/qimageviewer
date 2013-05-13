#include "settings.h"
#include "ui_settings.h"

Settings::Settings(QWidget *parent) : ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->languageComboBox->setItemIcon(0,(QIcon(QPixmap(":/res/flag-sys.png"))));
    ui->languageComboBox->setItemIcon(1,(QIcon(QPixmap(":/res/flag-usa.png"))));
    ui->languageComboBox->setItemIcon(2,(QIcon(QPixmap(":/res/flag-rus.png"))));
    ui->languageComboBox->setItemIcon(3,(QIcon(QPixmap(":/res/flag-ukr.png"))));
    ui->languageComboBox->setItemIcon(4,(QIcon(QPixmap(":/res/flag-pol.png"))));
}

void Settings::setDefaultSettings(QString language,
                                  QString defaultfolder,
                                  bool mouseZoom, bool mouseFullscreen,
                                  bool slideshowSmoothTransition, double slideshowInterval,
                                  int panelalignment,hotkeysStruct hotkeys)
{
    old_lang = language;
    old_defaultfolder = defaultfolder;
    old_mouseZoom = mouseZoom;
    old_mouseFullscreen = mouseFullscreen;
    old_slideshowInterval = slideshowInterval;
    old_slideshowSmoothTransition = slideshowSmoothTransition;
    old_panelalignment = panelalignment;

    old_hotkeys = hotkeys;

    if (language == "eng") ui->languageComboBox->setCurrentIndex(1);
    else if (language == "rus") ui->languageComboBox->setCurrentIndex(2);
    else if (language == "ukr") ui->languageComboBox->setCurrentIndex(3);
    else if (language == "pol") ui->languageComboBox->setCurrentIndex(4);
    else ui->languageComboBox->setCurrentIndex(0);

    ui->defaultfolderLineEdit->setText(defaultfolder);
    ui->mouseFullscreenCheckBox->setChecked(mouseZoom);
    ui->mouseZoomCheckBox->setChecked(mouseFullscreen);
    ui->slideshowIntervalSpinBox->setValue(slideshowInterval);
    ui->slideshowTransitionCheckBox->setChecked(slideshowSmoothTransition);

    ui->toolpanelComboBox->setCurrentIndex(panelalignment);

    setHotkeys();
}

QTreeWidgetItem * Settings::addCategory(QString name)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->hotkeyWidget);
    item->setText(0,name);
    ui->hotkeyWidget->addTopLevelItem(item);
    return item;
}

QTreeWidgetItem * Settings::addHotkey(QTreeWidgetItem * category, QString name, QString hotkey, QString icon, QString defaultHotkey)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(category);
    item->setText(0,name);
    item->setText(1,hotkey);
    item->setIcon(0,QIcon(QPixmap(icon)));
    item->setWhatsThis(2,defaultHotkey);
    category->addChild(item);
    return item;
}

void Settings::setHotkeys()
{
    ui->hotkeyWidget->setColumnWidth(0,300);
    QStringList ColumnNames;
    ColumnNames << tr("Command") << tr("Hotkey");
    ui->hotkeyWidget->setHeaderLabels(ColumnNames);

    QHotkeyWidget = new qhotkeywidget;
    connect(QHotkeyWidget,SIGNAL(butOKClicked()),this,SLOT(changingEndAccept()));
    connect(QHotkeyWidget,SIGNAL(butCancelClicked()),this,SLOT(changingEndDecline()));
    QHotkeyWidget->isChanging = false;

    categoryFile = addCategory("File");
    categoryFile->setExpanded(true);
    hotkeyItemFileOpen = addHotkey(categoryFile,tr("Open image"),old_hotkeys.fileOpen,":/res/file-open.png","Ctrl+O");
    hotkeyItemFileSave = addHotkey(categoryFile,tr("Save"),old_hotkeys.fileSave,":/res/file-save.png","Ctrl+S");
    hotkeyItemFileSaveAs = addHotkey(categoryFile,tr("Save as..."),old_hotkeys.fileSaveAs,"","Ctrl+Shift+S");
    hotkeyItemFileSettings = addHotkey(categoryFile,tr("Settings"),old_hotkeys.fileSettings,":/res/settings.png","");
    hotkeyItemFileQuit = addHotkey(categoryFile,tr("Quit"),old_hotkeys.fileQuit,"","Ctrl+Q");

    categoryEdit = addCategory("Edit");
    categoryEdit->setExpanded(true);
    hotkeyItemEditUndo = addHotkey(categoryEdit,tr("Undo"),old_hotkeys.editUndo,"","Ctrl+Z");
    hotkeyItemEditRedo = addHotkey(categoryEdit,tr("Redo"),old_hotkeys.editRedo,"","Ctrl+Shift+Z");
    hotkeyItemEditRotateRight = addHotkey(categoryEdit,tr("Rotate to the right"),old_hotkeys.editRotateRight,":/res/rotate-right.png","Ctrl+Shift+L");
    hotkeyItemEditRotateLeft = addHotkey(categoryEdit,tr("Rotate to the left"),old_hotkeys.editRotateLeft,":/res/rotate-left.png","Ctrl+Shift+R");
    hotkeyItemEditFlipHorizontal = addHotkey(categoryEdit,tr("Flip horizontal"),old_hotkeys.editFlipHorizontal,":/res/flip-horizontal.png","");
    hotkeyItemEditFlipVertical = addHotkey(categoryEdit,tr("Flip vertical"),old_hotkeys.editFlipVertical,":/res/flip-vertical.png","");
    hotkeyItemEditCrop = addHotkey(categoryEdit,tr("Crop"),old_hotkeys.editCrop,":/res/crop.png","Ctrl+Shift+C");
    hotkeyItemEditResize = addHotkey(categoryEdit,tr("Resize"),old_hotkeys.editResize,":/res/resize.png","Ctrl+R");
    hotkeyItemEditResizeItems = addHotkey(categoryEdit,tr("Resize items..."),old_hotkeys.editResizeItems,":/res/resize-items.png","Ctrl+Shift+R");

    categoryWatch = addCategory("Watch");
    categoryWatch->setExpanded(true);
    hotkeyItemWatchPrevious = addHotkey(categoryWatch,tr("Previous image"),old_hotkeys.watchPrevious,":/res/prev.png","Ctrl+Left");
    hotkeyItemWatchNext = addHotkey(categoryWatch,tr("Next image"),old_hotkeys.watchNext,":/res/next.png","Ctrl+Right");
    hotkeyItemWatchFullscreen = addHotkey(categoryWatch,tr("Fullscreen"),old_hotkeys.watchFullscreen,":/res/fullscreen.png","F11");
    hotkeyItemWatchSlideshow = addHotkey(categoryWatch,tr("Slideshow"),old_hotkeys.watchSlideshow,":/res/slideshow.png","F5");
    hotkeyItemWatchWallpaper = addHotkey(categoryWatch,tr("Set as wallpaper"),old_hotkeys.watchWallpaper,":/res/wallpaper.png","");
    hotkeyItemZoomIn = addHotkey(categoryWatch,tr("Zoom in"),old_hotkeys.zoomIn,":/res/zoom-in.png","+");
    hotkeyItemZoomOut = addHotkey(categoryWatch,tr("Zoom out"),old_hotkeys.zoomOut,":/res/zoom-out.png","-");
    hotkeyItemZoomWindow = addHotkey(categoryWatch,tr("Window size"),old_hotkeys.zoomWindow,":/res/zoom-window.png","");
    hotkeyItemZoomOriginal = addHotkey(categoryWatch,tr("Original size"),old_hotkeys.zoomOriginal,":/res/zoom-original.png","");

    categoryHelp = addCategory("Help");
    categoryHelp->setExpanded(true);
    hotkeyItemHelpAbout = addHotkey(categoryHelp,tr("About"),old_hotkeys.helpAbout,":/res/help.png","F1");
}

void Settings::saveHotkeys()
{
    old_hotkeys.fileOpen = hotkeyItemFileOpen->text(1);
    old_hotkeys.fileSave = hotkeyItemFileSave->text(1);
    old_hotkeys.fileSaveAs = hotkeyItemFileSaveAs->text(1);
    old_hotkeys.fileSettings = hotkeyItemFileSettings->text(1);
    old_hotkeys.fileQuit = hotkeyItemFileQuit->text(1);
    old_hotkeys.editUndo = hotkeyItemEditUndo->text(1);
    old_hotkeys.editRedo = hotkeyItemEditRedo->text(1);
    old_hotkeys.editRotateLeft = hotkeyItemEditRotateLeft->text(1);
    old_hotkeys.editRotateRight = hotkeyItemEditRotateRight->text(1);
    old_hotkeys.editFlipHorizontal = hotkeyItemEditFlipHorizontal->text(1);
    old_hotkeys.editFlipVertical = hotkeyItemEditFlipVertical->text(1);
    old_hotkeys.editCrop = hotkeyItemEditCrop->text(1);
    old_hotkeys.editResize = hotkeyItemEditResize->text(1);
    old_hotkeys.editResizeItems = hotkeyItemEditResizeItems->text(1);
    old_hotkeys.watchPrevious = hotkeyItemWatchPrevious->text(1);
    old_hotkeys.watchNext = hotkeyItemWatchNext->text(1);
    old_hotkeys.watchFullscreen = hotkeyItemWatchFullscreen->text(1);
    old_hotkeys.watchSlideshow = hotkeyItemWatchSlideshow->text(1);
    old_hotkeys.watchWallpaper = hotkeyItemWatchWallpaper->text(1);
    old_hotkeys.zoomIn = hotkeyItemZoomIn->text(1);
    old_hotkeys.zoomOut = hotkeyItemZoomOut->text(1);
    old_hotkeys.zoomWindow = hotkeyItemZoomWindow->text(1);
    old_hotkeys.zoomOriginal = hotkeyItemZoomOriginal->text(1);
    old_hotkeys.helpAbout = hotkeyItemHelpAbout->text(1);
}

Settings::~Settings()
{
}

void Settings::on_acceptButton_clicked()
{
    QString lang;
    switch (ui->languageComboBox->currentIndex())
    {
    case 0:lang = "sys";break;
    case 1:lang = "eng";break;
    case 2:lang = "rus";break;
    case 3:lang = "ukr";break;
    case 4:lang = "pol";break;
    }

    if (old_lang != lang)
        QMessageBox::question(this, tr("Warning!"),
                              tr("Language will be changing after restart program"),
                              QMessageBox::Ok | QMessageBox::Default);

    old_lang = lang;
    old_defaultfolder = ui->defaultfolderLineEdit->text();
    old_mouseZoom = ui->mouseZoomCheckBox->isChecked();
    old_mouseFullscreen = ui->mouseFullscreenCheckBox->isChecked();
    old_slideshowInterval = ui->slideshowIntervalSpinBox->value();
    old_slideshowSmoothTransition = ui->slideshowTransitionCheckBox->isChecked();
    old_panelalignment = ui->toolpanelComboBox->currentIndex();

    saveHotkeys();

    close();
}

void Settings::closeEvent(QCloseEvent *event)
{
    QHotkeyWidget->close();
    emit acceptsettings(old_lang,
                        old_defaultfolder,
                        old_mouseZoom,
                        old_mouseFullscreen,
                        old_slideshowSmoothTransition,
                        old_slideshowInterval,
                        old_panelalignment,
                        old_hotkeys);
    event->accept();
}

void Settings::on_resetButton_clicked()
{
#ifdef Q_OS_LINUX
    ui->defaultfolderLineEdit->setText("/home/"+QString::fromLocal8Bit( getenv("USER") ));
#endif
#ifdef Q_OS_WIN32
    ui->defaultfolderLineEdit->setText("C:\\Users\\"+QString::fromLocal8Bit( getenv("USER") ));
#endif
    ui->mouseFullscreenCheckBox->setChecked(true);
    ui->mouseZoomCheckBox->setChecked(true);
    ui->slideshowIntervalSpinBox->setValue(1);
    ui->slideshowTransitionCheckBox->setChecked(true);
}

void Settings::on_defaultfolderBrowseButton_clicked()
{
    QFileDialog dialog;
    dialog.setOption(QFileDialog::ShowDirsOnly);
    dialog.setFileMode(QFileDialog::Directory);
    QString path = dialog.getExistingDirectory(this,tr("Open directory"),
                                               ui->defaultfolderLineEdit->text(),
                                               QFileDialog::ShowDirsOnly
                                               | QFileDialog::DontResolveSymlinks);
    ui->defaultfolderLineEdit->setText(path);
}

void Settings::on_slideshowIntervalButton_clicked()
{
    ui->slideshowIntervalSpinBox->setValue(1);
}

void Settings::on_cancelButton_clicked()
{
    close();
}

void Settings::on_hotkeyWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (QHotkeyWidget->isChanging == false && item != categoryFile &&
            item != categoryEdit && item != categoryWatch && item != categoryHelp)
    {
        QHotkeyWidget->loadItem(item);
        QHotkeyWidget->show();
        QHotkeyWidget->isChanging = true;
    }
}

void Settings::changingEndAccept()
{
    QHotkeyWidget->changingItem->setText(1,QHotkeyWidget->hotkeyEdit->text());
    QHotkeyWidget->isChanging = false;
    QHotkeyWidget->close();
}
void Settings::changingEndDecline()
{
    QHotkeyWidget->isChanging = false;
    QHotkeyWidget->close();
}

void Settings::on_resetHotkeysButton_clicked()
{
    hotkeyItemFileOpen->setText(1,"Ctrl+O");
    hotkeyItemFileSave->setText(1,"Ctrl+S");
    hotkeyItemFileSaveAs->setText(1,"Ctrl+Shift+S");
    hotkeyItemFileSettings->setText(1,"");
    hotkeyItemFileQuit->setText(1,"Ctrl+Q");

    hotkeyItemEditUndo->setText(1,"Ctrl+Z");
    hotkeyItemEditRedo->setText(1,"Ctrl+Shift+Z");
    hotkeyItemEditRotateRight->setText(1,"Ctrl+Shift+L");
    hotkeyItemEditRotateLeft->setText(1,"Ctrl+Shift+R");
    hotkeyItemEditFlipHorizontal->setText(1,"");
    hotkeyItemEditFlipVertical->setText(1,"");
    hotkeyItemEditCrop->setText(1,"Ctrl+Shift+C");
    hotkeyItemEditResize->setText(1,"Ctrl+R");
    hotkeyItemEditResizeItems->setText(1,"Ctrl+Shift+R");

    hotkeyItemWatchPrevious->setText(1,"Ctrl+Left");
    hotkeyItemWatchNext->setText(1,"Ctrl+Right");
    hotkeyItemWatchFullscreen->setText(1,"F11");
    hotkeyItemWatchSlideshow->setText(1,"F5");
    hotkeyItemWatchWallpaper->setText(1,"");
    hotkeyItemZoomIn->setText(1,"+");
    hotkeyItemZoomOut->setText(1,"-");
    hotkeyItemZoomWindow->setText(1,"");
    hotkeyItemZoomOriginal->setText(1,"");

    hotkeyItemHelpAbout->setText(1,"F1");
}
