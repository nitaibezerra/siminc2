CREATE TABLE fabrica.itemauditoria (
	itemid SERIAL NOT NULL,
	itemnome CHARACTER(30) NOT NULL,
	itemdsc CHARACTER(250) NOT NULL,
	itemsituacao BOOLEAN NOT NULL DEFAULT TRUE,
	CONSTRAINT pk_itemid PRIMARY KEY(itemid)
);

CREATE TABLE fabrica.itemauditoriadetalhesauditoria (
	iadaid SERIAL NOT NULL,
	dtaid INTEGER,
	itemid INTEGER,
	CONSTRAINT pk_iadaid PRIMARY KEY(iadaid),
	CONSTRAINT fk_detalhesauditoria_itemauditoradetalhesauditoria FOREIGN KEY (dtaid) REFERENCES fabrica.detalhesauditoria (dtaid) ON UPDATE RESTRICT ON DELETE RESTRICT,
	CONSTRAINT fk_itemauditoria_itemauditoradetalhesauditoria FOREIGN KEY (itemid) REFERENCES fabrica.itemauditoria (itemid) ON UPDATE RESTRICT ON DELETE RESTRICT
);
