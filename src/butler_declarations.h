/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DECLARATIONS_H
#define BUTLER_DECLARATIONS_H

/*@BeginDecl@
	Class DatabaseDescriptor
	Fields {
		QString name;	key		// will be the connection name
		QString driver;			// for example QSQLITE
		QString databaseName;	// file name in case of sqlite
		QString username;
		QString password;
		bool savePassword;
		QString host;			// host name or ip
		unsigned port;
		}
@EndDecl@*/

/*@BeginDecl@
	Class Tag
	Fields {
		Text name; key			; TEXT
		Text description;		; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Ware
	Fields {
		Text name; key			; TEXT
		Text unit;				; TEXT NOT NULL DEFAULT ''
		Text icon;				; TEXT
		//WareTypeSet types; set	; WHERE WareType.ware = 'name'
		//WareTagSet tags; set	; WHERE WareTag.ware = 'name'
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class WareTag
	Fields {
		Text ware; key			; TEXT NOT NULL
		Text tag; key			; TEXT NOT NULL
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (ware, tag)
		}
@EndDecl@*/

/*@BeginDecl@
	Class WareType
	Fields {
		Text ware; key			; TEXT NOT NULL
		Text name; key			; TEXT NOT NULL
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (ware, name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Company
	Fields {
		Text name; key			; TEXT									// Tesco Global Áruházak
		Text country;			; TEXT NOT NULL DEFAULT ''				// Magyarország
		Text city;														// Budaörs
		Text postalCode;												// 2040
		Text address;													// Kinizsi út 1-3.
		Text taxId;														// 10307078-2-44
		Text icon;														// base64 repr of an image
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Brand
	Fields {
		Text name; key			; TEXT
		Text company;			; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Inventory
	Fields {
		Text name; key			; TEXT
		Text comment;			; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Item
	Fields {
		DateTime uploadDate; key	// non editable
		Text name;
		Text unit;
		Text type;
		Text brand;
		double quantity;			// amoutn to buy or not
		double price;				// price of gross piece/amount quantity
		Text currency;
		Text account;
		Text partner;
		Text inventory;
		Text comment;
		DateTime invChangeDate;
		DateTime lastModified;		// non editable
		}
	Constraints {
		PRIMARY KEY (uploadDate)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Partner
	Fields {
		Text name; key				// Kertvárosi tesco
		Text country;				// Magyarország
		Text city;			// Pécs
		Text postalCode;			// 7631
		Text address;				// Kincses út 1.
		Text company;				// Tesco Global Áruházak Zrt.
		Text storeName;				// 41052 számú bolt
		DateTime lastModified;		// non-editable
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Account
	Fields {
		Text name; key
		Text currency;
		Text bankOffice;
		Text iban;
		Text swiftCode;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Payment
	Fields {
		DateTime uploadDate; key
		Text account;
		Text partner;
		double amount;
		DateTime subject;
		DateTime payDate;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (uploadDate)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Query
	Fields {
		Text name; key
		DateTime startDate;
		DateTime endDate;
		enum StockOptions stockOption;
		enum TagOptions tagOption;
		TagNameSet withTags;
		TagNameSet withoutTags;
		WareNameSet wares;
		PartnerNameSet partners;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

#endif
