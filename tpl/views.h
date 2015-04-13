@Define{simpleListViewDeclaration@
/*forwards*/
class New@Type@View;
class Edit@Type@View;

class @Type@sView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	@Type@sView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~@Type@sView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void retranslate();
	void applyLayout();
	void relayout();
	void updateToolButtonStates();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);

private slots:
	void new@Type@();
	void edit@Type@();
	void del@Type@();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	@Type@Model & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	Edit@Type@View * new@Type@View;
	Edit@Type@View * edit@Type@View;
};
@}Define@
