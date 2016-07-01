<?php
$sqls = array(// Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Atenção Básica
			  0 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15006') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Média Complexidade (SIA+AIH)
			  1 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15007') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (3/6) - SIA+AIH Pago
			  2 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('464') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Alta Complexidade (SIA+AIH)
			  3 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15008','15016') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (4/6) - Alta Complexidade (SIA+AIH)
			  4 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('469') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - FAEC
			  5 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15017') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (4/6) - Alta Complexidade (SIA+AIH)
			  6 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('474') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - IAPI - Incentivo de Assistência a População Indígena
			  7 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15011') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - FIDEPS
			  8 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15010') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Recurso Interministerial MS
			  9 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15012') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Convênios do Fundo Nacional de Saúde - FNS (Recursos Liberados)	, Custeio
			  10 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('477') AND linid IN ('15139') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Convênios do Fundo Nacional de Saúde - FNS (Recursos Liberados)	, Capital
			  11 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('478') AND linid IN ('15139') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Emendas Parlamentares (Recursos Liberados)	, Custeio
			  12 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('477') AND linid IN ('15140') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Emendas Parlamentares (Recursos Liberados)	, Capital
			  13 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('478') AND linid IN ('15140') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Programa Interministerial (Ação 6379), Custeio
			  14 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15141') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Programa Interministerial (Ação 6379), Capital
			  15 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15141') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Custeio
			  16 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15144') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Capital
			  17 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15144') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Custeio
			  18 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('479') AND linid IN ('15143') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (1/2) - Emenda Parlamentar - Recursos liberados, Capital
			  19 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('480') AND linid IN ('15143') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (2/2)  - Residentes
			  20 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15145') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (1/6) - RJU/CLT-MEC
			  21 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('493') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (1/4) - Receita de Ensino
			  22 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15146') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Educação (2/2)  - Receita Total de Pesquisa Executada via HUF
			  23 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15147') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Aluguéis
			  24 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15155') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Estacionamentos
			  25 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15156') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (3/4) - Doações
			  26 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15157') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (2/4) - Capital
			  27 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15153') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (2/4) - Custeio
			  28 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15154') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Convênios Privados
			  29 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15158') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Pacientes Particulares
			  30 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15159') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Outras Fontes (4/4) - Contratos para Execução de SADT não Incluídos na Contratualização
			  31 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15161') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Despesas com Materiais (R$)
			  32 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('487','529','565') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Contratos de Serviços - Universidade (R$)
			  33 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('488','550','566') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Contratos de Serviços - Hospital (R$)
			  34 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('489','530','567') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (1/7)  - Despesas de Custeio (R$)
			  35 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('490','553','568') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (3/6) - RPA
			  36 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('505') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (1/6) e (3/6) - Tercerizados
			  37 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('507','495') AND ctiexercicio='{ano}' {perid}",
			  //  Força de Trabalho (3/6) - CLT
			  38 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('503') AND ctiexercicio='{ano}' {perid}",
			  // Força de Trabalho (1/6) - RJU
			  39 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('493') AND ctiexercicio='{ano}' {perid}",
			  // Força de Trabalho (2/6) - Tercerizados: Outras Fontes
			  40 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('501') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - IAC (Incentivo à Contratualização)
			  41 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15009') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (1/6) - Outros Recursos
			  42 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15013') AND ctiexercicio='{ano}' {perid}",
			  
			  
			  //  Dados Financeiros de Entrada - Ministério da Educação (1/2)  - Funcionamento dos Hospitais de Ensino (Ação 4086): Capital
			  43 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15142') AND colid IN ('480') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Entrada - Ministério da Educação (1/2)  - Funcionamento dos Hospitais de Ensino (Ação 4086): Custeio
			  44 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15142') AND colid IN ('479') AND ctiexercicio='{ano}' {perid}",
			  
			  // Dados Financeiros de Saída (5/7) - Equipamento e Material Permanentes
			  45 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15203') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Saída (5/7) - Obras (exceto reformas)
			  46 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15204') AND ctiexercicio='{ano}' {perid}",
			  //  Dívidas (1/2)
			  47 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('2037','2038','2039','2030','2040','2031','2041','2032','2042','2033','2043','2034','2035','2036') AND ctiexercicio='{ano}' {perid}",
			  //  Dados Financeiros de Saída (7/7) 
  			  48 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('15215') AND ctiexercicio='{ano}' {perid}",
  			   // Dívidas (2/2)
  			  49 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND linid IN ('2017','2027','2018','2028','2019','2029','2020','2021','2022','2023','2024','2025','2016','2026') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Programa REHUF	, Capital
			  50 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('478') AND linid IN ('53783') AND ctiexercicio='{ano}' {perid}",
			  // Dados Financeiros de Entrada - Ministério da Saúde (6/6) - Programa REHUF	, Custeio
			  51 => "SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem WHERE esuid='{esuid}' AND colid IN ('477') AND linid IN ('53783') AND ctiexercicio='{ano}' {perid}"
			  
			  );

			  
			  $hospitais = (array) $db->carregar("SELECT	ent.entid,
											ent.entnome
										  FROM entidade.entidade ent 
										  LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
										  LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
										  WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) 
										    ".(($_POST['entid'][0])?" AND ent.entid IN('".implode("','",$_POST['entid'])."')":"")." 
										  ORDER BY 
										    ent.entsig, ent.entnome");

			  
if(!$_REQUEST['anorel'])$_REQUEST['anorel']=$_REQUEST['anos'][0];
if(!$_REQUEST['anorel'])$_REQUEST['anorel']='2010';

$_PERIODOS_ANO['2009'] = array(1 => array('1','12','13','16','17','19','20','21','22','24','28','29','30','31','32','33','34','35','36','37','38','39','40','41','42','43','44','45','46','47','49','50','51','52','53','54','55','56','57','59','60','61','62','63','64','65','111','113','117'),
				   			   2 => array('66','67','68','69','70','71','72','73','74','75','76','77','78','79','80','81','82','83','84','85','86','87','88','89','90','91','92','93','94','95','96','97','98','99','100','101','102','103','104','105','106','107','108','109','110','112','114'),
				   			   3 => array('124','125','126','127','128','129','130','131','132','133','134','135','136','137','138','139','140','141','142','143','144','145','146','147','148','149','150','151','152','153','154','155','156','157','158','159','160','161','162','163','164','165','166','167','168','169','170','171'));				   

$_PERIODOS_ANO['2010'] = array(1 => array('177','178','179','180','181','182','183','184','185','186','187','188','189','190','191','192','193','194','195','196','198','199','200','201','202','203','204','205','206','207','208','209','210','211','212','213','214','215','216','217','218','219','220','221','222','223','224'),
				   			   2 => array('225','226','227','228','229','230','231','232','233','234','235','236','237','238','239','240','241','242','243','244','245','246','247','248','249','250','251','252','253','254','255','256','257','258','259','260','261','262','263','264','265','266','267','268','269','270','271'),
				   			   3 => array('277','278','279','280','281','282','283','284','285','286','287','288','289','290','291','292','293','294','295','296','297','298','299','300','301','302','303','304','305','306','307','308','309','310','311','312','313','314','315','316','317','318','319','320','321','322','323'));

$_PERIODOS_ANO['2011'] = array(1 => array('329','330','331','332','333','334','335','336','337','338','339','340','341','342','343','344','345','346','347','348','349','350','351','352','353','354','355','356','357','358','359','360','361','362','363','364','365','366','367','368','369','370','371','372','373','374','375'),
				   			   2 => array('376','377','378','379','380','381','382','383','384','385','386','387','388','389','390','391','392','393','394','395','396','397','398','399','400','401','402','403','404','405','406','407','408','409','410','411','412','413','414','415','416','417','418','419','420','421','422'),
				   			   3 => array('428','429','430','431','432','433','434','435','436','437','438','439','440','441','442','443','444','445','446','447','448','449','450','451','452','453','454','455','456','457','458','459','460','461','462','463','464','465','466','467','468','469','470','471','472','473','474'));
				   			   
				   
$_PERIODOS = $_PERIODOS_ANO[$_REQUEST['anorel']];

if($_REQUEST['perrel']) {
	$perids = " AND perid IN('".implode("','", $_PERIODOS[$_REQUEST['perrel']])."')";
}

foreach($hospitais as $hosp) {
	$_SESSION['rehuf_var']['esuid'] = $db->pegaUm("SELECT esuid FROM rehuf.estruturaunidade WHERE entid='".$hosp['entid']."'");

	/* Atenção Básica */
	$AtencaoBasica[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[0]));
	/* Média Complexidade (SIA+AIH) Contratualizado */
	$MediaComplexidadeContru[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[1]));
	/* Média Complexidade (SIA+AIH) Pago */
	$MediaComplexidadePg[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[2]));
	/* Alta Complexidade (SIA+AIH) Contratualizado */
	$AltaComplexidadeContru[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[3]));
	/* Alta Complexidade (SIA+AIH) Pago */
	$AltaComplexidadePg[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[4]));
	/* FAEC Contratualizado */
	$FAECContru[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[5]));
	/* FAEC (SIA+AIH) Pago */
	$FAECPg[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[6]));
	
	/* IAPI */
	$IAPI[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[7]));
	/* IAC */
	$IAC[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[41]));
	/* Outros Recursos */
	$OutrosRecursos[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[42]));
	/* FIDEPS */
	$FIDEPS[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[8]));
	/* Interministerial */
	$Interministerial[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[9]));
	
	
	/* FNS Custeio */
	$FNSCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[10]));
	/* FNS Capital */
	$FNSCapital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[11]));
	/* Emendas Parlamentares Custeio */
	$EmendasParCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[12]));
	/* Emendas Parlamentares Capital */
	$EmendasParCapital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[13]));
	/* Programa REHUF Capital */
	$ProgramaRehufCapital[$_REQUEST['anorel']] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[50]));
	/* Programa REHUF Custeio */
	$ProgramaRehufCusteio[$_REQUEST['anorel']] = $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[51]));
	
	
	
	
	/* Programa Interministerial (Ação 6379) Capital */
	$Acao6379Capital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[15]));
	/* Programa Interministerial (Ação 6379) Custeio */
	$Acao6379Custeio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[14]));
	
	/* Funcionamento dos Hospitais de Ensino (Ação 4086): Capital */
	$Acao4086Capital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[43]));
	/* Programa Interministerial (Ação 6379) Custeio */
	$Acao4086Custeio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[44]));
				  
	/* Emenda Parlamentar - Recursos liberados Custeio */
	$EmendaParLCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[16]));
	/* Emenda Parlamentar - Recursos liberados Capital */
	$EmendaParLCapital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[17]));
	/* Emenda Parlamentar - Recursos liberados Custeio */
	$DemaisAcoesCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[18]));
	/* Emenda Parlamentar - Recursos liberados Capital */
	$DemaisAcoesCapital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[19]));
	/* Residentes */
	$Residentes[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[20]));
	/* RJU */
	$PagamentoRJU[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[21]));
	
	/* Receita de Ensino */
	$ReceitaEnsino[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[22]));
	/* Receita Total de Pesquisa Executada via HUF */
	$PesquisaHUF[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[23]));
	
	
	/* Alugueis */
	$Alugueis[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[24]));
	/* Doações */
	$Doacoes[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[26]));
	/* Estacionamento */
	$Estacionamento[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[25]));
	
	/* Capital */
	$ApoioCapital[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[27]));
	/* Custeio */
	$ApoioCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[28]));
	
	/* Convênios Privados */
	$ConveniosPrivados[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[29]));
	/* Pacientes Particulares */
	$PacientesParticulares[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[30]));
	/* Contratos para Execução de SADT não Incluídos na Contratualização */
	$ContratosSADT[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[31]));
	
	$DespesasMateriais[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[32]));
	$ContratosServicosUni[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[33]));
	$ContratosServicosHos[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[34]));
	$DespesasCusteio[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[35]));
		
	$RPA[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[36]));
	$Tercerizados[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[37]));
	
	$CLT[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[38]));
	$RJU[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[39]));
	$TercerizadosOutrasF[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[40]));
	
	$EquipMatPerma[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[45]));
	$Obras[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[46]));
	
	// no caso da divida (excessão)
	$_ANO_ACUM['2009'] = array('fund' => 148, 'hosp' => 147);
	$_ANO_ACUM['2010'] = array('fund' => 300, 'hosp' => 299);
	$_ANO_ACUM['2011'] = array('fund' => 451, 'hosp' => 450);
	
	$TotalDividas[$_REQUEST['anorel']] += ($db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],(($perids)?$perids:"AND perid IN('".$_ANO_ACUM[$_REQUEST['anorel']]['fund']."')")),$sqls[47]))+$db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],(($perids)?$perids:"AND perid IN('".$_ANO_ACUM[$_REQUEST['anorel']]['hosp']."')")),$sqls[49])));
	
	$DespesaAmortizacaoDividas[$_REQUEST['anorel']] += $db->pegaUm(str_replace(array("{esuid}","{ano}","{perid}"),array($_SESSION['rehuf_var']['esuid'],$_REQUEST['anorel'],$perids),$sqls[48]));
	
}

$TotalIncentivosSaude[$_REQUEST['anorel']] += $IAPI[$_REQUEST['anorel']]+$IAC[$_REQUEST['anorel']]+$OutrosRecursos[$_REQUEST['anorel']]+$FIDEPS[$_REQUEST['anorel']]+$Interministerial[$_REQUEST['anorel']];

$TotalPesquisa[$_REQUEST['anorel']] += $ReceitaEnsino[$_REQUEST['anorel']]+$PesquisaHUF[$_REQUEST['anorel']];

$TotalRecursosPECusteio[$_REQUEST['anorel']] += $FNSCusteio[$_REQUEST['anorel']]+$EmendasParCusteio[$_REQUEST['anorel']]+$ProgramaRehufCusteio[$_REQUEST['anorel']];
$TotalRecursosPECapital[$_REQUEST['anorel']] += $FNSCapital[$_REQUEST['anorel']]+$EmendasParCapital[$_REQUEST['anorel']]+$ProgramaRehufCapital[$_REQUEST['anorel']]; 


$TotalProjetosEspCapital[$_REQUEST['anorel']] += $Acao6379Capital[$_REQUEST['anorel']]+$Acao4086Capital[$_REQUEST['anorel']]+$EmendaParLCapital[$_REQUEST['anorel']]+$DemaisAcoesCapital[$_REQUEST['anorel']];
$TotalProjetosEspCusteio[$_REQUEST['anorel']] += $Acao6379Custeio[$_REQUEST['anorel']]+$Acao4086Custeio[$_REQUEST['anorel']]+$EmendaParLCusteio[$_REQUEST['anorel']]+$DemaisAcoesCusteio[$_REQUEST['anorel']]+$Residentes[$_REQUEST['anorel']]+$PagamentoRJU[$_REQUEST['anorel']];


$TotalNaoOperacional[$_REQUEST['anorel']] += $Alugueis[$_REQUEST['anorel']]+$Doacoes[$_REQUEST['anorel']]+$Estacionamento[$_REQUEST['anorel']];

$TotalApoio[$_REQUEST['anorel']] += $ApoioCusteio[$_REQUEST['anorel']]+$ApoioCapital[$_REQUEST['anorel']];

$TotalAssistenciais[$_REQUEST['anorel']] += $ConveniosPrivados[$_REQUEST['anorel']]+$PacientesParticulares[$_REQUEST['anorel']]+$ContratosSADT[$_REQUEST['anorel']];

$TotalReceitaSUSContru[$_REQUEST['anorel']] += $AtencaoBasica[$_REQUEST['anorel']]+$MediaComplexidadeContru[$_REQUEST['anorel']]+$AltaComplexidadeContru[$_REQUEST['anorel']]+$FAECContru[$_REQUEST['anorel']];
$TotalReceitaSUSPg[$_REQUEST['anorel']] += $AtencaoBasica[$_REQUEST['anorel']]+$MediaComplexidadePg[$_REQUEST['anorel']]+$AltaComplexidadePg[$_REQUEST['anorel']]+$FAECPg[$_REQUEST['anorel']];

$TotalDespesaCapital[$_REQUEST['anorel']] += $EquipMatPerma[$_REQUEST['anorel']]+$Obras[$_REQUEST['anorel']];
$TotalDespesaCorrente[$_REQUEST['anorel']] += $DespesasMateriais[$_REQUEST['anorel']]+$ContratosServicosUni[$_REQUEST['anorel']]+$ContratosServicosHos[$_REQUEST['anorel']]+$DespesasCusteio[$_REQUEST['anorel']]+$Residentes[$_REQUEST['anorel']]+$RJU[$_REQUEST['anorel']]+$CLT[$_REQUEST['anorel']]+$RPA[$_REQUEST['anorel']]+$Tercerizados[$_REQUEST['anorel']]+$TercerizadosOutrasF[$_REQUEST['anorel']];

$TotalReceitaCorrente[$_REQUEST['anorel']] += $TotalReceitaSUSPg[$_REQUEST['anorel']]+$TotalIncentivosSaude[$_REQUEST['anorel']]+$TotalRecursosPECusteio[$_REQUEST['anorel']]+$TotalProjetosEspCusteio[$_REQUEST['anorel']]+$TotalPesquisa[$_REQUEST['anorel']]+$TotalNaoOperacional[$_REQUEST['anorel']]+$ApoioCusteio[$_REQUEST['anorel']]+$TotalAssistenciais[$_REQUEST['anorel']];
$TotalReceitaCapital[$_REQUEST['anorel']] += $TotalRecursosPECapital[$_REQUEST['anorel']]+$TotalProjetosEspCapital[$_REQUEST['anorel']]+$ApoioCapital[$_REQUEST['anorel']];

?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<tr>
	<td colspan="3" class="SubTituloEsquerda" style="background-color:white;">Dados Financeiros (Entrada)</td>
</tr>
<tr>
	<td colspan="3" class="SubTituloCentro" style="background-color:gray;">Ministério da Saúde</td>
</tr>
<tr>
	<td class="SubTituloCentro">Receita Efetiva SUS (R$) - Assistencial</td>
	<td class="SubTituloCentro">Contratualizado (R$)</td>
	<td class="SubTituloCentro">Aprovado/Pago (R$)</td>
</tr>
<tr>
	<td class="SubTituloDireita">Atenção Básica</td>
	<td align="right"><? echo number_format($AtencaoBasica[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
	<td align="right"><? echo number_format($AtencaoBasica[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Média Complexidade - SIA/SIH</td>
	<td align="right"><? echo number_format($MediaComplexidadeContru[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
	<td align="right"><? echo number_format($MediaComplexidadePg[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Alta Complexidade - SIA/SIH</td>
	<td align="right"><? echo number_format($AltaComplexidadeContru[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
	<td align="right"><? echo number_format($AltaComplexidadePg[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">FAEC (Fundo de Ações Estratégicas e Compensação)</td>
	<td align="right"><? echo number_format($FAECContru[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
	<td align="right"><? echo number_format($FAECPg[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td align="right"><strong><? echo number_format($TotalReceitaSUSContru[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
	<td align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalReceitaSUSPg[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>

<tr>
	<td class="SubTituloCentro">Incentivos Ministério da Saúde</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">IAPI - Incentivo de Assistência a População Indígena</td>
	<td colspan="2" align="right"><? echo number_format($IAPI[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">FIDEPS</td>
	<td colspan="2" align="right"><? echo number_format($FIDEPS[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">IAC (Incentivo à Contratualização)</td>
	<td colspan="2" align="right"><? echo number_format($IAC[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Recurso Interministerial (MS)</td>
	<td colspan="2" align="right"><? echo number_format($Interministerial[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Outros Recursos</td>
	<td colspan="2" align="right"><? echo number_format($OutrosRecursos[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalIncentivosSaude[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Recursos de Projetos Específicos</td>
</tr>
<tr>
	<td class="SubTituloCentro">Capital</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Projetos do FNS</td>
	<td colspan="2" align="right"><? echo number_format($FNSCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Emenda Parlamentar (MS)</td>
	<td colspan="2" align="right"><? echo number_format($EmendasParCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Programa REHUF</td>
	<td colspan="2" align="right"><? echo number_format($ProgramaRehufCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(3)</font> <strong><? echo number_format($TotalRecursosPECapital[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Custeio</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Projetos do FNS</td>
	<td colspan="2" align="right"><? echo number_format($FNSCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Emenda Parlamentar (MS)</td>
	<td colspan="2" align="right"><? echo number_format($EmendasParCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Programa REHUF</td>
	<td colspan="2" align="right"><? echo number_format($ProgramaRehufCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalRecursosPECusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td colspan="3" class="SubTituloCentro" style="background-color:gray;">Ministério da Educação</td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Projetos Específicos</td>
</tr>
<tr>
	<td class="SubTituloCentro">Capital</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Programa Interministerial - MEC - Ação 6379</td>
	<td colspan="2" align="right"><? echo number_format($Acao6379Capital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Funcionamento dos Hospitais de Ensino - Ação 4086</td>
	<td colspan="2" align="right"><? echo number_format($Acao4086Capital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Emenda Parlamentar - Recursos liberados</td>
	<td colspan="2" align="right"><? echo number_format($EmendaParLCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Demais Ações</td>
	<td colspan="2" align="right"><? echo number_format($DemaisAcoesCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(3)</font> <strong><? echo number_format($TotalProjetosEspCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Custeio</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Programa Interministerial - MEC - Ação 6379</td>
	<td colspan="2" align="right"><? echo number_format($Acao6379Custeio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Funcionamento dos Hospitais de Ensino - Ação 4086</td>
	<td colspan="2" align="right"><? echo number_format($Acao4086Custeio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Emenda Parlamentar - Recursos liberados</td>
	<td colspan="2" align="right"><? echo number_format($EmendaParLCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Demais Ações</td>
	<td colspan="2" align="right"><? echo number_format($DemaisAcoesCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Bolsas de Residências</td>
	<td colspan="2" align="right"><? echo number_format($Residentes[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Pagamento de RJU Ativo</td>
	<td colspan="2" align="right"><? echo number_format($PagamentoRJU[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalProjetosEspCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3">Dados Financeiros de Entrada - Outras Fontes R$</td>
</tr>
<tr>
	<td class="SubTituloCentro">Receita Gerada pela Atividade de Pesquisa</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Receitas de Ensino</td>
	<td colspan="2" align="right"><? echo number_format($ReceitaEnsino[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Receita Total de Pesquisa Executada via HUF</td>
	<td colspan="2" align="right"><? echo number_format($PesquisaHUF[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalPesquisa[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Receitas Não-Operacionais R$</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Aluguéis</td>
	<td colspan="2" align="right"><? echo number_format($Alugueis[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Estacionamento</td>
	<td colspan="2" align="right"><? echo number_format($Doacoes[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Doações</td>
	<td colspan="2" align="right"><? echo number_format($Estacionamento[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalNaoOperacional[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Receitas Capitadas pelas Fundações de Apoio (R$)</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Capital</td>
	<td colspan="2" align="right"><font size=1>(3)</font> <? echo number_format($ApoioCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Custeio</td>
	<td colspan="2" align="right"><font size=1>(1)</font> <? echo number_format($ApoioCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><strong><? echo number_format($TotalApoio[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Receitas Assistenciais (R$)</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Convênios Privados</td>
	<td colspan="2" align="right"><? echo number_format($ConveniosPrivados[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Pacientes Particulares</td>
	<td colspan="2" align="right"><? echo number_format($PacientesParticulares[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Contratos para execução de SADT não incluídos na contratualização</td>
	<td colspan="2" align="right"><? echo number_format($ContratosSADT[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(1)</font> <strong><? echo number_format($TotalAssistenciais[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloEsquerda" colspan="3" style="background-color:white;">Dados Financeiros (Saída)</td>
</tr>
<tr>
	<td class="SubTituloCentro">Despesa Corrente</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Despesas com Materiais</td>
	<td colspan="2" align="right"><? echo number_format($DespesasMateriais[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Contratos de Serviços Universidade</td>
	<td colspan="2" align="right"><? echo number_format($ContratosServicosUni[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Contratos de Serviços Hospital</td>
	<td colspan="2" align="right"><? echo number_format($ContratosServicosHos[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Despesas de custeio</td>
	<td colspan="2" align="right"><? echo number_format($DespesasCusteio[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Bolsa de Residentes</td>
	<td colspan="2" align="right"><? echo number_format($Residentes[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Pagamento RJU</td>
	<td colspan="2" align="right"><? echo number_format($RJU[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">CLT</td>
	<td colspan="2" align="right"><? echo number_format($CLT[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">RPA</td>
	<td colspan="2" align="right"><? echo number_format($RPA[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Tercerizados</td>
	<td colspan="2" align="right"><? echo number_format($Tercerizados[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Tercerizados: Fontes SUS</td>
	<td colspan="2" align="right"><? echo number_format($TercerizadosOutrasF[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(2)</font> <strong><? echo number_format($TotalDespesaCorrente[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloCentro">Despesa de Capital</td>
	<td class="SubTituloCentro" colspan="2">R$</td>
</tr>
<tr>
	<td class="SubTituloDireita">Equipamentos e Materiais Permanentes</td>
	<td colspan="2" align="right"><? echo number_format($EquipMatPerma[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita">Obras (exceto reforma)</td>
	<td colspan="2" align="right"><? echo number_format($Obras[$_REQUEST['anorel']], 2, ',', '.'); ?></td>
</tr>
<tr>
	<td class="SubTituloDireita"><strong>TOTAL GERAL</strong></td>
	<td colspan="2" align="right"><font size=1>(4)</font> <strong><? echo number_format($TotalDespesaCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td colspan="3" class="SubTituloCentro" style="background-color:gray;">Totais</td>
</tr>
<tr>
	<td class="SubTituloDireita">Total de Receita Corrente</td>
	<td colspan="2" align="right"><font size=1>(S1)</font> <strong><? echo number_format($TotalReceitaCorrente[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloDireita">Total de Despesa Corrente</td>
	<td colspan="2" align="right"><font size=1>(S2)</font> <strong><? echo number_format($TotalDespesaCorrente[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloDireita">Total de Receita Capital</td>
	<td colspan="2" align="right"><font size=1>(S3)</font> <strong><? echo number_format($TotalReceitaCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloDireita">Total de Despesa Capital</td>
	<td colspan="2" align="right"><font size=1>(S4)</font> <strong><? echo number_format($TotalDespesaCapital[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloDireita">Total de Dívidas</td>
	<td colspan="2" align="right"><strong><? echo number_format($TotalDividas[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
<tr>
	<td class="SubTituloDireita">Despesa de amortização das dívidas</td>
	<td colspan="2" align="right"><strong><? echo number_format($DespesaAmortizacaoDividas[$_REQUEST['anorel']], 2, ',', '.'); ?></strong></td>
</tr>
</table>