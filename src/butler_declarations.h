/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DECLARATIONS_H
#define BUTLER_DECLARATIONS_H

/*@BeginDecl@
	Class DatabaseDescriptor
	Fields {
		csjp::String name;	key		// will be the connection name
		SqlDriver driver;			// for example SQLite or PSql
		csjp::String databaseName;	// file name in case of sqlite
		csjp::String username;
		csjp::String password;
		YNBool savePassword;
		csjp::String host;			// host name or ip
		UInt port;
		}
@EndDecl@*/

/*@BeginDecl@
	Class Tag
	Fields {
		Text name; key			; TEXT
		Text description;		; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Ware
	Fields {
		Text name; key				; TEXT
		Text unit;					; TEXT
		Text icon;					; TEXT
		WareTypeSet types; set,spec	;
		WareTagSet tags; set,spec	;
		DateTime lastModified;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;				; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class WareTag
	Fields {
		Text ware; key,link		; TEXT
		Text tag; key			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (ware, tag)
		FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class WareType
	Fields {
		Text ware; key,link		; TEXT
		Text type; key			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (ware, type)
		FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class Company
	Fields {
		Text name; key			; TEXT					// Tesco Global Áruházak
		Text country;			; TEXT					// Magyarország
		Text city;				; TEXT					// Budaörs
		Text postalCode;		; TEXT					// 2040
		Text address;			; TEXT					// Kinizsi út 1-3.
		Text taxId;				; TEXT					// 10307078-2-44
		Text icon;				; TEXT					// base64 repr of an image
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Brand
	Fields {
		Text name; key			; TEXT
		Text company;			; TEXT
		Text icon;				; TEXT					// base64 repr of an image
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		FOREIGN KEY (company) REFERENCES company(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class Inventory
	Fields {
		Text name; key			; TEXT
		Text comment;			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class Partner
	Fields {
		Text name; key			; TEXT	// Kertvárosi tesco
		Text country;			; TEXT	// Magyarország
		Text city;				; TEXT	// Pécs
		Text postalCode;		; TEXT	// 7631
		Text address;			; TEXT	// Kincses út 1.
		Text company;			; TEXT	// Tesco Global Áruházak Zrt.
		Text storeName;			; TEXT	// 41052 számú bolt
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		FOREIGN KEY (company) REFERENCES company(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class Account
	Fields {
		Text name; key			; TEXT
		Text currency;			; TEXT
		Text bankOffice;		; TEXT
		Text iban;				; TEXT
		Text swiftCode;			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		FOREIGN KEY (bank_office) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class Item
	Fields {
		DateTime uploadDate; key	; TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date)
		Text name;					; TEXT
		Text unit;					; TEXT
		Text type;					; TEXT
		Text brand;					; TEXT
		Double quantity;			; DECIMAL(15,3) NOT NULL // amoutn to buy or not
		Double price;				; DECIMAL(15,2) NOT NULL // price of gross piece/amount quantity
		Text currency;				; TEXT
		Text account;				; TEXT
		Text partner;				; TEXT
		Text inventory;				; TEXT
		Text comment;				; TEXT
		DateTime invChangeDate;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		DateTime lastModified;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;				; CHAR(1) NOT NULL DEFAULT 'N'
		Double unitPrice; derived	; DECIMAL(15,3) NOT NULL
		Double quantityWithUnit; derived	; TEXT
		}
	Constraints {
		PRIMARY KEY (upload_date)
		FOREIGN KEY (name) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (brand) REFERENCES brand(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (account) REFERENCES account(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (inventory) REFERENCES inventory(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class ItemQuery
	Fields {
		Text name; key								; TEXT
		DateTime startDate;							; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		DateTime endDate;							; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		enum ItemQueryStockOptions stockOption;spec	; INTEGER NOT NULL
		enum ItemQueryTagOptions tagOption; spec	; INTEGER NOT NULL
		ItemQueryWithTagSet withTags; set,spec		;
		ItemQueryWithoutTagSet withoutTags; set,spec;
		ItemQueryWareSet wares; set,spec			;
		ItemQueryPartnerSet partners; set,spec		;
		DateTime lastModified;						; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;								; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class ItemQueryWithTag
	Fields {
		Text query; key,link	; TEXT
		Text tag; key			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (query, tag)
		FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class ItemQueryWithoutTag
	Fields {
		Text query; key,link	; TEXT
		Text tag; key			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (query, tag)
		FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class ItemQueryWare
	Fields {
		Text query; key,link	; TEXT
		Text ware; key			; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (query, ware)
		FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class ItemQueryPartner
	Fields {
		Text query; key,link	; TEXT
		Text partner; key		; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (query, partner)
		FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class InvEntry
	Fields {
		DateTime uploadDate; key	; TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date)
		Text name;					; TEXT
		Text unit;					; TEXT
		Text type;					; TEXT
		Text brand;					; TEXT
		Double quantity;			; DECIMAL(15,3) NOT NULL // amoutn to buy or not
		Double price;				; DECIMAL(15,2) NOT NULL // price of gross piece/amount quantity
		Text currency;				; TEXT
		Text account;				; TEXT
		Text partner;				; TEXT
		Text inventory;				; TEXT
		Text comment;				; TEXT
		DateTime invChangeDate;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		DateTime lastModified;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;				; CHAR(1) NOT NULL DEFAULT 'N'
		Double unitPrice; derived	; DECIMAL(15,3) NOT NULL
		Double quantityWithUnit; derived	; TEXT
		}
	Constraints {
		PRIMARY KEY (upload_date)
		FOREIGN KEY (name) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (brand) REFERENCES brand(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (account) REFERENCES account(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (inventory) REFERENCES inventory(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class Payment
	Fields {
		DateTime uploadDate; key	; TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date)
		Text account;				; TEXT
		Text partner;				; TEXT
		Double amount;				; DECIMAL(15,3) NOT NULL
		DateTime subject;			; TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date)
		DateTime payDate;			; TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date)
		Text comment;				; TEXT
		DateTime lastModified;		; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;				; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (upload_date)
		FOREIGN KEY (account) REFERENCES account(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (subject) REFERENCES item(upload_date) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

/*@BeginDecl@
	Class PaymentQuery
	Fields {
		Text name; key							; TEXT
		DateTime startDate;						; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		DateTime endDate;						; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		PaymentQueryPartnerSet partners; set,spec		;
		DateTime lastModified;					; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;							; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

/*@BeginDecl@
	Class PaymentQueryPartner
	Fields {
		Text query; key,link	; TEXT
		Text partner; key		; TEXT
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		YNBool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (query, partner)
		FOREIGN KEY (query) REFERENCES payment_query(name) ON DELETE CASCADE ON UPDATE CASCADE
		FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE
		}
@EndDecl@*/

#endif
