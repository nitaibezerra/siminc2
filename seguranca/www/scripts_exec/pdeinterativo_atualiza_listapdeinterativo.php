<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */

// carrega as funções gerais
include_once "/var/www/simec/global/config.inc";
//include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '00000000191';


$servidor_bd = '';
$porta_bd = '5432';
$nome_bd = '';
$usuario_db = '';
$senha_bd = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$Tinicio = getmicrotime();

$tabela = $db->pegaUm("select relname from pg_stat_user_tables where relname='listapdeinterativonova' order by relname");

if(!$tabela) {

	$sql = "CREATE TABLE pdeinterativo.listapdeinterativonova
			(
			  lstid serial NOT NULL,
			  pdeid integer,
			  esdid integer,
			  usucpf character(11),
			  docid integer,
			  foto text,
			  pdicodinep character varying(10),
			  pdenome character varying(180),
			  pdiesfera character varying(120),
			  mundescricao character varying(255),
			  estuf character(2),
			  usucpfdiretor bpchar,
			  usunome character varying(250),
			  usuemail character varying(150),
			  realizado character varying(250),
			  datatramitacao timestamp without time zone,
			  percent numeric,
			  realizadof character varying(250),
			  datatramitacaof timestamp without time zone,
			  pagamento character varying(250),
			  tempdeescola character varying(250),
			  pditempdeescola boolean,
			  aedid integer,
			  aedidf integer,
			  htddata timestamp without time zone,
			  htddataf timestamp without time zone,
			  muncod character varying(7),
			  pdigeridapde boolean,
			  pdienergiaeletricacenso boolean,
			  pdienergiaeletrica boolean, 
			  pdipossuicoordenadasgeograficas boolean,
			  CONSTRAINT pk_listapdeinterativonova PRIMARY KEY (lstid)
			)
			WITH (
			  OIDS=FALSE
			);
			ALTER TABLE pdeinterativo.listapdeinterativonova OWNER TO simec;";
	
	pg_query($sql);
	
	$sql = "CREATE INDEX ix_pdinterativo_listapdeinterativonova_aedid
  		ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (aedid);
		
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_aedidf
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (aedidf);
		
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_datatramitacao
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (datatramitacao);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_esdid
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (esdid);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_estuf
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (estuf);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_lstid
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (lstid);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_muncod
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (muncod);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_pdenome
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (pdenome);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_pdiesfera
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (pdiesfera);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_pdigeridapde
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (pdigeridapde);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_usucpf
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (usucpf);
		
		CREATE INDEX ix_pdinterativo_listapdeinterativonova_usucpfdiretor
		  ON pdeinterativo.listapdeinterativonova
		  USING btree
		  (usucpfdiretor);";

	pg_query($sql);

}




$sql = "insert into pdeinterativo.listapdeinterativonova (   pdeid, 
		  esdid ,
		  usucpf ,
		  docid ,
		  foto ,
		  pdicodinep ,
		  pdenome,
		  pdiesfera,
		  mundescricao ,
		  estuf ,
		  usucpfdiretor ,
		  usunome ,
		  usuemail ,
		  realizado ,
		  datatramitacao ,
		  percent ,
		  realizadof,
		  datatramitacaof,
		  pagamento,
		  tempdeescola ,
		  pditempdeescola ,
		  aedid ,
		  aedidf ,
		  htddata,
		  htddataf,
		  muncod,
		  pdigeridapde,
		  pdienergiaeletricacenso,
		  pdienergiaeletrica, 
		  pdipossuicoordenadasgeograficas )
		select 
			pde.pdeid, doc.esdid , usu.usucpf, pde.docid,
						CASE WHEN (SELECT count(1) FROM pdeinterativo.galeriafoto gal WHERE gal.pdeid = pde.pdeid and gal.gfostatus = 'A') > 0  
							THEN '<img src=\"../imagens/cam_foto.gif\" onclick=\"galeriaPDEInterativo(' || pde.pdeid || ')\" class=\"link\" >'
							ELSE ''
						END as foto,
						pdicodinep,
						pdenome,
						pdiesfera,
						mun.mundescricao,
						mun.estuf,
						COALESCE(usu.usucpf,'Escola sem diretor. Leia as orientações.') as usucpfdiretor,
						COALESCE(usu.usunome,'Escola sem diretor.Leia as orientações.') as usunome,
						COALESCE(usu.usuemail,'Escola sem diretor. Leia as orientações.') as usuemail,
						CASE WHEN doc.aeddscrealizada IS NOT NULL THEN doc.aeddscrealizada 
							 WHEN pde.docid IS NULL THEN 'Não iniciado' 
							 ELSE 'Em elaboração' END as realizado,
						CASE WHEN doc.htddata IS NOT NULL THEN doc.htddata::timestamp ELSE null END as datatramitacao,
						((CASE WHEN total > 0 THEN round(((total)::numeric(10,2) /
							(select count(distinct abaid) from pdeinterativo.aba where (abatipo != 'O' or abatipo is null) and abaidpai is not null and abaid not in (2,3,4,5,6,7,8,54))::numeric(10,2))*100,0) ELSE 0 END) ) as percent,
						CASE WHEN docf.aeddscrealizada IS NOT NULL THEN docf.aeddscrealizada 
							 ELSE 'Em elaboração' END as realizadof,
						CASE WHEN docf.htddata IS NOT NULL THEN docf.htddata::timestamp ELSE null END as datatramitacaof
						,(CASE 
								WHEN spasituacao is true THEN 'Pago' 
								WHEN spasituacao is false THEN 'Pendente - '||COALESCE(mopdesc,'')
								WHEN spasituacao is null THEN 'N/A'
							END) as pagamento,
						CASE WHEN pditempdeescola=TRUE THEN '<img src=../imagens/check.jpg border=0>' 
							 ELSE '-' END as tempdeescola,
		pditempdeescola,
		doc.aedid,
		docf.aedid,
		doc.htddata,
		docf.htddata htddataf,
		mun.muncod,
		pde.pdigeridapde,
		pde.pdienergiaeletricacenso,
		pde.pdienergiaeletrica,
		CASE WHEN medlatitude IS NOT NULL AND medlongitude IS NOT NULL THEN true ELSE false END as pdipossuicoordenadasgeograficas
						from 
						pdeinterativo.pdinterativo pde 
						left join ( select pes.usucpf, ptp.pdeid from pdeinterativo.pessoa pes 
		                            inner join pdeinterativo.pessoatipoperfil ptp on ptp.pesid=pes.pesid 
		                            where pes.pflcod = 544 AND pes.pesstatus = 'A'and ptp.tpeid=2) pes ON pes.pdeid = pde.pdeid
						left join seguranca.usuario usu ON usu.usucpf = pes.usucpf 
						left join territorios.municipio mun ON pde.muncod = mun.muncod
						left join  ( select doc.docid, atd.aeddscrealizada, hst.htddata, doc.esdid, atd.aedid from
		                             workflow.documento doc 
		                             inner join workflow.historicodocumento hst ON hst.hstid = doc.hstid 
		                             inner join workflow.acaoestadodoc atd ON atd.aedid = hst.aedid 
		                             where doc.tpdid in(43,57,63) ) doc ON doc.docid = pde.docid 
						 
						left join  ( select docf.docid, atdf.aeddscrealizada, hstf.htddata, atdf.aedid from
									 workflow.documento docf 
									 inner join workflow.historicodocumento hstf ON hstf.hstid = docf.hstid 
									 inner join workflow.acaoestadodoc atdf ON atdf.aedid = hstf.aedid 
									 where docf.tpdid = 55 ) docf ON docf.docid = pde.formacaodocid 
						
						left join pdeinterativo.situacaopagamento sit ON sit.pdeid = pde.pdeid and spastatus = 'A' 
						left join pdeinterativo.motivopagamento mtp ON mtp.mopid = sit.mopid 
						left join (select pdeid, count(distinct abaid) as total from pdeinterativo.abaresposta group by pdeid ) abaresp ON abaresp.pdeid = pde.pdeid				
					 WHERE pde.pdistatus='A' ;";

pg_query($sql);

$sql = "vacuum pdeinterativo.listapdeinterativonova;";
pg_query($sql);

$sql = "ALTER TABLE pdeinterativo.listapdeinterativo RENAME TO listapdeinterativoold;
		ALTER TABLE pdeinterativo.listapdeinterativonova RENAME TO listapdeinterativo;";
pg_query($sql);

$sql = "drop table pdeinterativo.listapdeinterativoold;";
pg_query($sql);


$sql = "ALTER INDEX pdeinterativo.pk_listapdeinterativonova RENAME TO pk_listapdeinterativo;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_aedid RENAME TO ix_pdinterativo_listapdeinterativo_aedid;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_aedidf RENAME TO ix_pdinterativo_listapdeinterativo_aedidf;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_datatramitacao RENAME TO ix_pdinterativo_listapdeinterativo_datatramitacao;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_esdid RENAME TO ix_pdinterativo_listapdeinterativo_esdid;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_estuf RENAME TO ix_pdinterativo_listapdeinterativo_estuf;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_lstid RENAME TO ix_pdinterativo_listapdeinterativo_lstid;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_muncod RENAME TO ix_pdinterativo_listapdeinterativo_muncod;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_pdenome RENAME TO ix_pdinterativo_listapdeinterativo_pdenome;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_pdiesfera RENAME TO ix_pdinterativo_listapdeinterativo_pdiesfera;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_pdigeridapde RENAME TO ix_pdinterativo_listapdeinterativo_pdigeridapde;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_usucpf RENAME TO ix_pdinterativo_listapdeinterativo_usucpf;
		ALTER INDEX pdeinterativo.ix_pdinterativo_listapdeinterativonova_usucpfdiretor RENAME TO ix_pdinterativo_listapdeinterativo_usucpfdiretor;";

pg_query($sql);

$Tfim = getmicrotime() - $Tinicio;


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= SIGLA_SISTEMA. " - PDEInterativo";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject 		= "Atualizando Lita PDEInterativo";
$mensagem->Body 		= "Todos as operações (Lista PDEInterativo) foram executadas com sucesso : ".$Tfim." segundos";
$mensagem->IsHTML( true );
$mensagem->Send();

?>
