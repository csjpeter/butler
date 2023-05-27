/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <butler_pannview.h>

PannView::PannView(QWidget * parent) :
	QWidget(parent),
	scrollArea(0),
	scroller(&scrollArea)
{
	titleLabel.setProperty("windowTitle", true);
	setAttribute(Qt::WA_QuitOnClose, false);
	setFocusPolicy(Qt::NoFocus);
	main.setFocusPolicy(Qt::NoFocus);

	scrollArea.horizontalScrollBar()->setObjectName("pannviewscrollbar");
	scrollArea.verticalScrollBar()->setObjectName("pannviewscrollbar");
	scrollArea.setFrameStyle(QFrame::NoFrame);
	scrollArea.setWidget(&main);
	scrollArea.setWidgetResizable(true);
	scrollArea.setFocusPolicy(Qt::NoFocus);
	//scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	/* left side scrollbar HACK part 1 */
	scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(scrollArea.verticalScrollBar(), SIGNAL(rangeChanged(int, int)),
			this, SLOT(verticalScrollBarRangeChanged()));

	QHBoxLayout * hLayout = new QHBoxLayout;
	hLayout->setContentsMargins(0,0,0,0);
	/* left side scrollbar HACK part 1 */
	hLayout->addWidget(scrollArea.verticalScrollBar());
	hLayout->addWidget(&scrollArea);

	QVBoxLayout * vLayout = new QVBoxLayout;
	//if(qApp->desktop()->width() == width())
		vLayout->addWidget(&titleLabel);
	vLayout->addWidget(&toolBar);
	vLayout->addLayout(hLayout);
	vLayout->addWidget(&footerBar);
	vLayout->setContentsMargins(0,0,0,0);
	QWidget::setLayout(vLayout);

	installEventFilter(this);
	setSizeIncrement(0, 0);
	setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}
PannView::~PannView() {}

void PannView::setupView()
{
	connect(&toolBar.backButton, SIGNAL(clicked()), this, SLOT(reject()));
	toolBar.relayout();
	footerBar.relayout();
}

QLayout * PannView::layout() const
{
	return main.layout();
}

void PannView::setLayout(QLayout * layout)
{
	delete main.layout();
	main.setLayout(layout);
}

QSize PannView::sizeHint()
{
	QSize s(scrollArea.sizeHint());
	/*LOG("Size hint for viewport %, % for window with title: %.",
			s.rwidth(), s.rheight(), titleLabel.text());*/
	return QSize(s.rwidth() + 50, s.rheight() + 50);
}

int PannView::width()
{
	return scrollArea.viewport()->width();
}

void PannView::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	verticalScrollBarRangeChanged();
}

void PannView::closeEvent(QCloseEvent *event)
{
	QWidget::closeEvent(event);
}

bool PannView::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() != QEvent::KeyPress)
		return QObject::eventFilter(obj, event);

	QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
	DBG("Received key press ", keyEvent);

	switch (keyEvent->key()){
		/*case Qt::Key_Enter:
		case Qt::Key_Return:
			break;*/
		case Qt::Key_Escape:
			{
				QWidget * focused = focusWidget();
				if(focused)
					focusWidget()->clearFocus();
				else
					reject();
			}
			return true;
		default:
			break;
	}
	return QObject::eventFilter(obj, event);
}

void PannView::loadState(const QString & prefix)
{
	QSettings settings;
	QPoint pos = settings.value(prefix + "/position", QPoint(-1, -1)).toPoint();
	QSize size = settings.value(prefix + "/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	else
		adjustSize();
	if(pos.x() != -1)
		move(pos);
}

void PannView::saveState(const QString & prefix)
{
	QSettings settings;
	settings.setValue(prefix + "/position", pos());
	settings.setValue(prefix + "/size", size());
}

void PannView::setWindowTitle(const QString & str)
{
	//LOG("Window width: % and %", qApp->desktop()->width(), QWidget::width());
	//if(qApp->desktop()->width() <= QWidget::width()){
#ifdef Q_OS_ANDROID
		titleLabel.setText(str);
		titleLabel.show();
#else
//	} else {
		QWidget::setWindowTitle(str);
		titleLabel.hide();
#endif
//	}
}

void PannView::verticalScrollBarRangeChanged()
{
	scrollArea.verticalScrollBar()->setVisible(
			scrollArea.verticalScrollBar()->minimum() <
			scrollArea.verticalScrollBar()->maximum());
}

void PannView::activate()
{
	QWidget::show(); raise(); activateWindow();
	//setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
}
void PannView::accept() { this->accepted(); this->finished(1); close(); }
void PannView::reject() { this->rejected(); this->finished(0); close(); }
void PannView::done(int result) { this->finished(result); close(); }


