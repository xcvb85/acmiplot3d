QT       += core gui opengl xml
TARGET = ACMIPlot3D
TEMPLATE = app
DEFINES += GIT_HASH="\\\"$$system(git -C \""$$_PRO_FILE_PWD_"\" rev-parse --short HEAD)\\\""

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

unix {
	!macx {
		LIBS	+=	-lGLU
	}
}

win32 {
	LIBS += -lopengl32 -lglu32
}

RESOURCES += \
	Ressources/acmiplot3d.qrc

INCLUDEPATH +=	\
	DataSource \
	Objects

SOURCES += \
	glwidget.cpp \
	main.cpp \
	mainwindow.cpp \
	modelloader.cpp \
	trajectory.cpp \
    model.cpp

HEADERS  += \
	glwidget.h \
	mainwindow.h \
	modelloader.h \
	trajectory.h \
    model.h

FORMS += mainwindow.ui
