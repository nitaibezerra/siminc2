<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

//$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();
//2269
$sql = "select anx.anxid, anx.arqid, (select ptrid from emenda.planotrabalho where ptridpai = anx.ptrid and ptrstatus = 'A' limit 1) as ptridfilho, 
			anx.ptrid as ptridpai, anx.anxdsc, anx.anaid, anx.mailid, anx.anxtipo, anx.anxtermoref,
            anx.refid, anx.anxprotocolo, anx.anxdata, anx.gdoid			   
		from emenda.anexo anx
	 	where ptrid in (2269) and anx.anxtipo = 'P' and anx.gdoid is null";
$arrDados = $db->carregar( $sql );
$arrDados = $arrDados ? $arrDados : array();
ver($arrDados,d);
foreach ($arrDados as $v) {
	$ptridpai = pegaPaiPTA();
	
	$sql = "UPDATE emenda.anexo SET arqid = :arqid WHERE anxid = :anxid";
	
	/*$sql = "INSERT INTO public.arquivo(arqnome, arqdescricao, arqextensao, arqtipo, arqtamanho, arqdata, arqhora, arqstatus, usucpf, sisid)
			(SELECT arqnome, arqdescricao, arqextensao, arqtipo, arqtamanho, arqdata, arqhora, arqstatus, usucpf, sisid 
				FROM public.arquivo WHERE arqid = {$v['arqid']}) returning arqid";	
	$arqid = $db->pegaUm($sql);*/
	
	/*echo '<pre>';
	echo $sql.'<br><br>';
	$sql = "INSERT INTO emenda.anexo(arqid, ptrid, anxdsc, anaid, mailid, anxtipo, anxtermoref, refid, anxprotocolo, anxdata, gdoid)
			(select {$v['arqid']}, {$v['ptridfilho']}, anxdsc, anaid, mailid, anxtipo, anxtermoref, refid, anxprotocolo, anxdata, gdoid 
				from emenda.anexo where anxid = {$v['anxid']})";
	$db->executar($sql);
	echo '<pre>';
	echo $sql.'<br><br>';*/
}

$db->commit();








die;

$sql = "select DISTINCT
	ptr.ptrid,
	ptr.ptrcod,
	enb.estuf,
    mun.mundescricao,
    enb.enbcnpj,
    enb.enbnome
from 
	emenda.planotrabalho ptr
    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
    left join territorios.municipio mun on mun.muncod = enb.muncod
where
	ptr.ptrexercicio = '2012'
    and ptr.ptrstatus = 'A'
    and enb.enbstatus = 'A'
order by 
	ptr.ptrcod";

$arrDados = $db->carregar($sql);
$arrRegistro = array();
foreach ($arrDados as $key => $v) {
	$sql = "select DISTINCT ini.ininome from emenda.ptiniciativa pti
				inner join emenda.iniciativa ini on ini.iniid = pti.iniid
			where
				ini.inistatus = 'A'
			    and pti.ptrid = {$v['ptrid']}";
	$arrIniciativa = $db->carregarColuna( $sql );
	$arrIniciativa = $arrIniciativa ? $arrIniciativa : array();
	
	$sql = "select DISTINCT case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as itemcomposicao
			from emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid
			    inner join emenda.ptitemkit ptk on ptk.pteid = ptie.pteid
			    left join emenda.itempar_especificacao ite on ite.iteid = ptk.iteid
			    left join emenda.itempar gip on gip.ipaid = ite.ipaid
				left join par.propostaitemcomposicao pic on pic.picid = gip.picid
			where
				ptie.ptestatus = 'A'
			    and pti.ptrid = 3240
			union 
			select DISTINCT case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as itemcomposicao
			from emenda.ptiniciativa pti
				inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid
			    inner join emenda.ptitemkit ptk on ptk.pteid = ptie.pteid
			    left join emenda.itempar_especificacao ite on ite.iteid = ptk.iteid
			    left join emenda.itemobras io on io.itoid = ite.itoid
			    left join obras.pretipoobra pto on pto.ptoid = io.ptoid
			where
			    pti.ptrid = {$v['ptrid']}
			    and ptie.ptestatus = 'A'";
			    
	$arrItens = $db->carregarColuna( $sql );
	$arrItens = $arrItens ? $arrItens : array();
	
	$arrRegistro[$key]= array('ptrcod' => $v['ptrcod'],
							  'estuf' => $v['estuf'],
							  'mundescricao' => $v['mundescricao'],
							  'enbcnpj' => $v['enbcnpj'],
							  'enbnome' => $v['enbnome'],
							  'iniciativa' => implode(', ', $arrIniciativa),							
							  'item' => implode(', ', $arrItens)							
							); 
}
ob_clean();
header('content-type: text/html; charset=ISO-8859-1');		
$cabecalho = array('PTA', 'Estado', 'Municipio', 'CNPJ', 'Entidade', 'Iniciativa', 'Item Composição');		
$db->sql_to_excel($arrRegistro, 'relEmendasPTA', $cabecalho, $formato);
exit;


die();

$sql = "SELECT sisid, sisdsc, sisurl, sisabrev, sisdiretorio, sisfinalidade,
  sisrelacionado,
  sispublico,
  sisstatus,
  sisexercicio,
  sismostra,
  sisemail,
  paginainicial,
  sisarquivo,
  sistel,
  sisfax,
  sisdtalteradados
FROM 
  seguranca.sistema WHERE sisid = 999";

$arrSistema = $db->carregar( $sql );

foreach ($arrSistema as $v) {
	$sql = "INSERT INTO seguranca.sistema(
  sisid,
  sisdsc,
  sisurl,
  sisabrev,
  sisdiretorio,
  sisfinalidade,
  sisrelacionado,
  sispublico,
  sisstatus,
  sisexercicio,
  sismostra,
  sisemail,
  paginainicial,
  sisarquivo,
  sistel,
  sisfax,
  sisdtalteradados
) 
(
  '{$v['sisid']}'
  '{$v['sisdsc']}'
  '{$v['sisurl']}'
  '{$v['sisabrev']}'
  '{$v['sisdiretorio']}'
  '{$v['sisfinalidade']}'
  '{$v['sisrelacionado']}'
  '{$v['sispublico']}'
  '{$v['sisstatus']}'
  '{$v['sisexercicio']}'
  '{$v['sismostra']}'
  '{$v['sisemail']}'
  '{$v['paginainicial']}'
  '{$v['sisarquivo']}'
  '{$v['sistel']}'
  '{$v['sisfax']}'
  '{$v['sisdtalteradados']}'
);";

$sqle = "SELECT 
  pflcod,
  pfldsc,
  pfldatainicio,
  pfldatafim,
  pflstatus,
  pflresponsabilidade,
  pflsncumulativo,
  pflfinalidade,
  pflnivel,
  pfldescricao,
  sisid,
  pflsuperuser,
  pflsuporte,
  pflinddelegar,
  pflpadrao,
  modid
FROM 
  seguranca.perfil where sisid = 999";
$arrPerfil = $db->carregar( $sqle );
foreach ($arrPerfil as $p) {
	$sql.= "INSERT INTO 
  seguranca.perfil
(
  pflcod,
  pfldsc,
  pfldatainicio,
  pfldatafim,
  pflstatus,
  pflresponsabilidade,
  pflsncumulativo,
  pflfinalidade,
  pflnivel,
  pfldescricao,
  sisid,
  pflsuperuser,
  pflsuporte,
  pflinddelegar,
  pflpadrao,
  modid
) 
VALUES (
  '{$p['pflcod']}',
  '{$p['pfldsc']}',
  '{$p['pfldatainicio']}',
  '{$p['pfldatafim']}',
  '{$p['pflstatus']}',
  '{$p['pflresponsabilidade']}',
  '{$p['pflsncumulativo']}',
  '{$p['pflfinalidade']}',
  '{$p['pflnivel']}',
  '{$p['pfldescricao']}',
  '{$p['sisid']}',
  '{$p['pflsuperuser']}',
  '{$p['pflsuporte']}',
  '{$p['pflinddelegar']}',
  '{$p['pflpadrao']}',
  '{$p['modid']}'
);";
}

$sqle = "SELECT 
  mnucod,
  mnucodpai,
  mnudsc,
  mnustatus,
  mnulink,
  mnutipo,
  mnustile,
  mnuhtml,
  mnusnsubmenu,
  mnutransacao,
  mnushow,
  abacod,
  mnuhelp,
  sisid,
  mnuid,
  mnuidpai,
  mnuordem
FROM 
  seguranca.menu where sisid = 999 order by mnucod";

$arrMenu = $db->carregar( $sqle );
foreach ($arrMenu as $m) {
	$sql.= "INSERT INTO 
  seguranca.menu
(
  mnucod,
  mnucodpai,
  mnudsc,
  mnustatus,
  mnulink,
  mnutipo,
  mnustile,
  mnuhtml,
  mnusnsubmenu,
  mnutransacao,
  mnushow,
  abacod,
  mnuhelp,
  sisid,
  mnuid,
  mnuidpai,
  mnuordem
) 
VALUES (
  '{$m['mnucod']}',
  ".($m['mnucodpai'] ? "'".$m['mnucodpai']."'" : 'null').",
  '{$m['mnudsc']}',
  '{$m['mnustatus']}',
  '{$m['mnulink']}',
  '{$m['mnutipo']}',
  '{$m['mnustile']}',
  '{$m['mnuhtml']}',
  '{$m['mnusnsubmenu']}',
  '{$m['mnutransacao']}',
  '{$m['mnushow']}',
  ".($m['abacod'] ? "'".$m['abacod']."'" : 'null').",
  '{$m['mnuhelp']}',
  '{$m['sisid']}',
  '{$m['mnuid']}',
  ".($m['mnuidpai'] ? "'".$m['mnuidpai']."'" : 'null').",
  '{$m['mnuordem']}'
);";
}

$sqle = "SELECT 
  abacod,
  abadsc,
  sisid
FROM 
  seguranca.aba where sisid = 999";
$arrAba = $db->carregar( $sqle );
foreach ($arrAba as $a) {
	$sql.= "INSERT INTO 
  seguranca.aba
(
  abacod,
  abadsc,
  sisid
) 
VALUES (
  '{$a['abacod']}',
  '{$a['abadsc']}',
  '{$a['sisid']}'
);";
}

}
ver(simec_htmlentities($sql) );


die;

$sql = "select 
			ptr.ptrid, ptr.ptrcod, ptr.ptrexercicio, ptm.refid
		from 
			emenda.planotrabalho ptr
			inner join emenda.ptminreformulacao ptm on ptm.ptrid = ptr.ptrid
		    inner join workflow.documento doc on doc.docid = ptr.docid
		WHERE
			doc.esdid = 207
		    --and ptm.refsituacaoreformulacao = 'C'
		    and ptr.ptrcod = 1306";
$arrDados = $db->carregar($sql);

foreach ($arrDados as $v) {
	$sql = "UPDATE emenda.ptminreformulacao SET refsituacaoreformulacao = 'F' WHERE refid = {$v['refid']}";
	$db->executar($sql);
	insereTiposReformulacao( $v['ptrid'], $v['refid'], true );
}


/*$sql = "select DISTINCT p.ptrcod, p.ptrid, p.ptridpai, p.ptrexercicio from 
	emenda.planotrabalho p
	inner join workflow.documento d on d.docid = p.docid    
where 
	 d.tpdid = 8
     and d.esdid = 53
     and p.ptrstatus = 'A'
     --and p.ptrexercicio = '2012'";

$arrDados = $db->carregar($sql);

foreach ($arrDados as $v) {
	$total = $db->pegaUm("select count(anaid) from emenda.analise a where ptrid = {$v['ptrid']} and anastatus = 'A' and anatipo = 'D'");
	if( $total == 0 ){
		$sql = "INSERT INTO emenda.analise( ptrid, anatipo, analote, anadatainclusao )
				VALUES ({$v['ptrcod']}, 'D', 1, 'now');";
		echo $sql.'<br>';
	}
}*/
die;

include_once APPRAIZ."emenda/classes/WSIntegracaoSiconv.class.inc";

/*$usuario = '18428908168';
$senha = md5('Hernandes.reis3');*/

$usuario = 'rmedeiro';
$senha = 'R5659532';

//$urlWsdl = 'https://wshom.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
//$urlWsdl = 'https://www.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
$urlWsdl = 'http://172.20.65.93:8080/IntraSiconvWS/services/SimecWsFacade?wsdl';
//$urlWsdl = 'http://www.fnde.gov.br/IntraSiconvWS/services/SimecWsFacade?wsdl';
/*
 * ptrcod = 2959 e ptrid = 3747
 * ptrcod = 2960 e ptrid = 3748
 */

$arrParam = array('ptrid' 	=> 3234,
				  'usuario' => $usuario,
				  'senha' 	=> $senha,
				  'url' 	=> $urlWsdl
				);

$obWS = new WSIntegracaoSiconv($arrParam);
$obWS->enviaPropostaWS();
//$obWS->solicitarNotaEmpenhoWS();
$obWS->consultarEmpenhoWS();
//$obWS->consultaConvenioWS();

/*$wsdl = 'https://wshom.convenios.gov.br/siconv-services-interfaceSiconv/InterfaceSiconvHandlerSrv?wsdl';
$options = Array(
				'exceptions'	=> true,
		        'trace'			=> true,
				'encoding'		=> 'ISO-8859-1' );
$client = new SoapClient($wsdl, $options);
ver($client->__getFunctions(),d);*/

//echo insereFilhosPTA(1679);
//echo deletaFilhosPTA( 3477 );
die;

/*$sql = "SELECT DISTINCT
		    eme.emecod,
		    eme.emeid,
		    res.resassunto,
		    res.resdsc
		FROM
		    emenda.emenda eme
		    inner join emenda.responsavel res on res.resid = eme.resid
		WHERE
			eme.emeano = 2012
		    --and eme.emecod = '10660005'
		order by eme.emecod";

$arrEmenda = $db->carregar( $sql );

$arrRegistro = array();
foreach ($arrEmenda as $key => $v) {
	$sql = "SELECT
				ini.ininome
			FROM
				emenda.emendadetalhe emd
			    inner join emenda.iniciativaemendadetalhe ied on ied.emdid = emd.emdid
			    inner join emenda.iniciativa ini on ini.iniid = ied.iniid
			WHERE
				emd.emeid = {$v['emeid']}
			    and ied.iedstatus = 'A'
			    and ini.inistatus = 'A'
			ORDER BY
				ini.ininome";
	$arInidsc = $db->carregarColuna( $sql );
	$arInidsc = $arInidsc ? $arInidsc : array();
	
	$sql = "SELECT
				enb.enbcnpj||' - '||enb.enbnome as entidades
			FROM
				emenda.emendadetalhe emd
			    inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
			    inner join emenda.entidadebeneficiada enb on enb.enbid = ede.enbid
			WHERE
				emd.emeid = {$v['emeid']}
			    and ede.edestatus = 'A'
			    and enb.enbstatus = 'A'
			ORDER BY
				enb.enbnome";
				
	$arEntidade = $db->carregarColuna( $sql );
	$arEntidade = $arEntidade ? $arEntidade : array();
	//ver(implode(', ', $arEntidade),d);
	if( $arInidsc[0] && $arEntidade[0] ){
		array_push( $arrRegistro, array("emecod" => $v['emecod'], 
										"beneficiario" => implode('<br>', $arEntidade),
										"iniciativa" => implode('<br>', $arInidsc)
 									) );
	}
}
$cabecalho = array("Emenda", "Beneficiário", "Iniciativa");
$db->monta_lista($arrRegistro, $cabecalho, 100000, 4, 'N','Center','','form');
die();
//ver($arrRegistro,d);
ob_clean();
header('content-type: text/html; charset=ISO-8859-1');		
$formato = array('n', 's', 's');
$db->sql_to_excel($arrRegistro, 'relEmendasPTA', $cabecalho, $formato);
exit;*/

/*$sql = "SELECT 
			p.ptrcod,
		    p.ptrid
		FROM 
			emenda.planotrabalho p
		    inner join emenda.analise a on a.ptrid = p.ptrid
		    inner join workflow.documento doc ON p.docid = doc.docid
		WHERE
			p.ptrstatus = 'A' 
		    and a.anastatus = 'A'
		    AND p.ptrexercicio = 2011
		    AND doc.esdid = 56
		    and a.anatipo = 'T'
    		and a.anadataconclusao is null
    		and p.resid = 1";

$arrDados = $db->carregar( $sql );
$arrDados = $arrDados ? $arrDados : array();

$Count = 0;
foreach ($arrDados as $v) {
	$docid =  $db->pegaUm("SELECT docid FROM emenda.planotrabalho WHERE ptrid = ".$v['ptrid']);
	$arDados = array( "url" => "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A", "ptrid" => $v['ptrid'] );
	

	$analote = $db->pegaUm("select max(analote) from emenda.analise where ptrid = {$v['ptrid']} and anastatus = 'A' and anatipo = 'M'");
	$analote = !empty($analote) ? $analote + 1 : 1;
	
	$sql = "insert into emenda.analise(ptrid, uniid, anatipo, analote, anadatainclusao, anastatus) 
			 values ({$v['ptrid']}, 8, 'M', {$analote}, now(), 'A');";
	$db->executar($sql);
	wf_alterarEstado( $docid, '1035', '', $arDados );
	$Count++;
}

echo '<pre>';
echo 'Total de Registro atualizado: '.$Count;*/

echo deletaFilhosPTA( 2811 );
echo deletaFilhosPTA( 2812 );
//echo deletaFilhosPTA( 2945 );
echo deletaFilhosPTA( 2946 );
die;
/*
loop cnpj_duplicados begin

	enbids = buscar_entidades_pelo_cnpj
	
	loop enbids begin
	
		procura_entidade_habilitada;
		
		
	end
	
	atualizar_tabelas_para

end

1º Etapa -> Carrego todas as entidades cadastrada na tabela emenda.entidadebeneficiada
2º Etapa -> executo um sql para trabalhar com uma entidade por vez, verificando se aquela 
				entidade esta duplicada, caso contrario não a trato.
2º Etapa -> verificar quais entidades possuam PTA.

*/

$enbano = $_REQUEST['enbano'];
$enbanoExerc = $_REQUEST['anoexercicio'];

if( empty($enbano) || empty($enbanoExerc) ) die('Falta informar o ano da entidade e o ano do exercicio');

$sql = "SELECT eb.enbid, eb.enbcnpj, eb.enbnome, eb.enbstatus, eb.enbsituacaohabilita, eb.muncod, eb.enbano
				FROM emenda.entidadebeneficiada eb
				WHERE eb.enbano = '$enbano'
					--and eb.enbcnpj = ''
				order by eb.enbcnpj";

$arEntidade = $db->carregar( $sql );

$arEnbcnpj = array();
$arEnbid = array();
$arEnbidDelete = array();
$boExisteVinculo = false;
$boPTA = false;

foreach ($arEntidade as $v) {
	
	if( !in_array( $v['enbcnpj'], $arEnbcnpj ) ){
		
		$sql = "SELECT enbid FROM emenda.entidadebeneficiada eb WHERE enbcnpj = '".$v['enbcnpj']."' and enbano = $enbanoExerc";
		$enbid = $db->pegaUm( $sql );
		
		#atualiza entidade cadastrada nas tabelas abaixo com enbid de ano diferente
		if( !empty($enbid) ){
			$sql = "UPDATE emenda.planotrabalho set enbid = {$v['enbid']} WHERE enbid = $enbid and ptrexercicio = '$enbano'";
			$db->executar( $sql );
			
			$sql = "UPDATE emenda.entbeneficiadacontrapartida set enbid = {$v['enbid']} WHERE enbid = $enbid and ebcexercicio = '$enbano'";
			$db->executar( $sql );
			
			$sql = "UPDATE emenda.emendadetalheentidade SET enbid = {$v['enbid']} WHERE edeid in (select ede.edeid from emenda.emendadetalheentidade ede
					    inner join emenda.emendadetalhe ed on ed.emdid = ede.emdid
					    inner join emenda.emenda e on e.emeid = ed.emeid
					where ede.enbid = $enbid and e.emeano = '$enbano')";

			$db->executar( $sql );
		}
		
		$sql = "SELECT eb.enbid, eb.enbcnpj, eb.enbnome, eb.enbstatus, eb.enbsituacaohabilita, eb.muncod,
					(select distinct enbid from emenda.usuarioresponsabilidade  where enbid = eb.enbid) as usuario,
					(select distinct enbid from emenda.emendadetalheentidade where enbid = eb.enbid) as detalhe,
					(select distinct enbid from emenda.planotrabalho where enbid = eb.enbid and ptrexercicio = eb.enbano) as pta,
					(select distinct enbid from emenda.entbeneficiadacontrapartida where enbid = eb.enbid and ebcexercicio = '$enbano') as partida,
					(select distinct e.enbid from emenda.entbeneficiadacontrapartida e where enbid = eb.enbid and ebcexercicio = '$enbano') as partidavalor,
					eb.enbano 
				FROM emenda.entidadebeneficiada eb WHERE eb.enbcnpj = '".$v['enbcnpj']."' and eb.enbano = $enbano
				order by eb.enbcnpj, (select distinct enbid from emenda.planotrabalho where enbid = eb.enbid and ptrexercicio = eb.enbano) is null, 
				eb.enbstatus, eb.enbsituacaohabilita asc";
			
		$arEntDuplicada = $db->carregar( $sql );
		$arEntDuplicada = ( $arEntDuplicada ? $arEntDuplicada : array() );
		
		#trato somente entidades duplicadas
		$enbidPTA = '';
		$boPTA = false;
		$boNoPTA = false;
		$enbidAnterior = '';		
		foreach ($arEntDuplicada as $key => $valor) {
			
			if( !empty($valor['pta']) ){
				$enbidPTA = $valor['pta'];
				#atualiza dados da entidade quando estiver ptas cadastrados para mesma entidade com enbid diferente
				if( $boPTA ){
					array_push( $arEnbidDelete, array("enbid" => $enbidAnterior, "enbidPTA" => $enbidPTA, "enbcnpj" => $valor['enbcnpj'], "enbnome" => $valor['enbnome']) );
					//if( !empty($valor['usuario']) ){
						/*$sql = "DELETE FROM emenda.usuarioresponsabilidade WHERE enbid = $enbidPTA";
						
						$sql = "INSERT INTO emenda.usuarioresponsabilidade( pflcod, usucpf, enbid, autid, resid, rpustatus, rpudata_inc, uniid)
								(SELECT pflcod, usucpf, enbid, autid, resid, rpustatus, rpudata_inc, uniid FROM emenda.usuarioresponsabilidade
									WHERE enbid = $enbidPTA)";*/
						$sql = "UPDATE emenda.usuarioresponsabilidade SET enbid = $enbidAnterior where enbid = $enbidPTA";
						$db->executar( $sql );
					//}
					
					$sql = "UPDATE emenda.emendadetalheentidade SET enbid = $enbidAnterior WHERE enbid = $enbidPTA";
					$db->executar( $sql );
					
					$sql = "UPDATE emenda.entbeneficiadacontrapartida SET enbid = $enbidAnterior WHERE enbid = $enbidPTA";
					$db->executar( $sql );
					
					$sql = "UPDATE emenda.planotrabalho SET enbid = $enbidAnterior WHERE enbid = $enbidPTA";
					$db->executar( $sql );
					
					$sql = "DELETE FROM emenda.entidadebeneficiada WHERE enbid = $enbidPTA";
					$db->executar( $sql );
				} else {
					$enbidAnterior = $valor['enbid'];
				}
				$boPTA = true;
			} else {
				#atualiza dados da entidade quandoe não estiver pta cadastrado para elas.
				if( $boPTA ){
					$boPTA = false;
					array_push( $arEnbidDelete, array("enbid" => $valor['enbid'], "enbidPTA" => $enbidPTA, "enbcnpj" => $valor['enbcnpj'], "enbnome" => $valor['enbnome']) );
					$sql = "UPDATE emenda.usuarioresponsabilidade SET enbid = $enbidPTA where enbid = {$valor['enbid']}";
					$db->executar( $sql );
					
					$sql = "UPDATE emenda.emendadetalheentidade SET enbid = $enbidPTA WHERE enbid = {$valor['enbid']}";
					$db->executar( $sql );
					
					$sql = "UPDATE emenda.entbeneficiadacontrapartida SET enbid = $enbidPTA WHERE enbid = {$valor['enbid']}";
					$db->executar( $sql );
					
					$sql = "UPDATE emenda.planotrabalho SET enbid = $enbidPTA WHERE enbid = {$valor['enbid']}";
					$db->executar( $sql );
					
					$sql = "DELETE FROM emenda.entidadebeneficiada WHERE enbid = {$valor['enbid']}";
					$db->executar( $sql );
				} else { #para os casos de entidades que não estão vinculadas ao pta
					if( !$boNoPTA ){
						$enbidNoPTA = $valor['enbid'];
						$boNoPTA = true;
					} else {
						array_push( $arEnbidDelete, array("enbid" => $valor['enbid'], "enbidPTA" => $enbidNoPTA, "enbcnpj" => $valor['enbcnpj'], "enbnome" => $valor['enbnome']) );
						
						$sql = "UPDATE emenda.usuarioresponsabilidade SET enbid = $enbidNoPTA where enbid = {$valor['enbid']}";
						$db->executar( $sql );
	
						$sql = "UPDATE emenda.emendadetalheentidade SET enbid = $enbidNoPTA WHERE enbid = {$valor['enbid']}";
						$db->executar( $sql );
						
						$sql = "UPDATE emenda.entbeneficiadacontrapartida SET enbid = $enbidNoPTA WHERE enbid = {$valor['enbid']}";
						$db->executar( $sql );
						
						$sql = "UPDATE emenda.planotrabalho SET enbid = $enbidNoPTA WHERE enbid = {$valor['enbid']}";
						$db->executar( $sql );
						
						$sql = "DELETE FROM emenda.entidadebeneficiada WHERE enbid = {$valor['enbid']}";
						$db->executar( $sql );
					}
				}
			}
		}
		
		array_push( $arEnbcnpj, $v['enbcnpj'] );
	}
}
if($db->commit()){
	$sql = "select * from emenda.entidadebeneficiada e 
			where
				enbcnpj not in ( select enbcnpj from emenda.entidadebeneficiada where enbcnpj = e.enbcnpj and enbano = $enbano)
			    and enbcnpj is not null";
	
	$arDados = $db->carregar( $sql );
	$arDados = ( $arDados ? $arDados : array() );
	
	$arCNPJDublic = array();
	#cadastra a entidade que existe em 2009 mais não existe em 2010
	foreach ($arDados as $v) {
		if( !in_array( $v['enbcnpj'], $arCNPJDublic ) ){
			$sql = "INSERT INTO emenda.entidadebeneficiada(entid, enbstatus, enbano, enbsituacaohabilita,
					  enbdataalteracao, enbcnpj, muncod, estuf, enblog, enbnum, enbbai, enbcep,
					  enbnumdddcomercial, enbnumcomercial, enbnumfax, enbemail, enborgcod, enbnome) 
					(SELECT entid, 'A', '2010', enbsituacaohabilita,
					  enbdataalteracao, enbcnpj, muncod, estuf, enblog, enbnum, enbbai, enbcep,
					  enbnumdddcomercial, enbnumcomercial, enbnumfax, enbemail, enborgcod, enbnome 
					FROM emenda.entidadebeneficiada WHERE enbcnpj = '{$v['enbcnpj']}' limit 1)";
			
			$db->executar( $sql );
			array_push( $arCNPJDublic, $v['enbcnpj'] );
		}
	}
	$db->commit();	
	echo 'Registros atualizados com sucesso!';
} else echo 'Falha na atualização!';
echo '<pre>';
print_r(sizeof($arEnbidDelete));
echo "<br>";
print_r($arEnbidDelete);
//ver( $arEnbidDelete, $arEnbid, $arEnbcnpj );


/*include_once (APPRAIZ . 'includes/classes/Fnde_Webservice_Client.class.inc');

header("http-equiv='Content-Type' content='text/html; charset=iso-8859-1'");

$usuario = 'SIMECPRISCILA';
$senha = 'simecpriscila';
$co_municipio_siafi='9677';
$nu_cgc_favorecido ='';
$co_esfera_adm = '2';
$vl_empenho ='260000.00';
$nu_processo ='23400014974200918';
$sg_uf='GO';
$co_especie_empenho='01';
$co_plano_interno_solic='FFF50B9102N';
$co_esfera_orcamentaria_solic='1';
$co_ptres_solic='026710';
$co_fonte_recurso_solic='0100000000';
$co_natureza_despesa_solic='444042';
$an_convenio='2009';
$nu_convenio='656600';

$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
<header>
<app>string</app>
<version>string</version>
<created>2003-10-18T13:41:04.74</created>
</header>
	<body>
	<params>
	    <co_municipio_siafi>$co_municipio_siafi</co_municipio_siafi>
	    <nu_cgc_favorecido>$nu_cgc_favorecido</nu_cgc_favorecido>
	    <co_esfera_adm>$co_esfera_adm</co_esfera_adm>
	    <vl_empenho>$vl_empenho</vl_empenho>
	    <nu_processo>$nu_processo</nu_processo>
	    <sg_uf>$sg_uf</sg_uf>
	    <co_especie_empenho>$co_especie_empenho</co_especie_empenho>
	    <co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
	    <co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
	    <co_ptres_solic>$co_ptres_solic</co_ptres_solic>
	    <co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
	    <co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
	    <an_convenio>$an_convenio</an_convenio>
	    <nu_convenio>$nu_convenio</nu_convenio>
	</params>
	</body>
</request>
XML;

$url = "http://172.20.200.116/webservices/sigefemendas/integracao/web/dev.php/empenho/solicitar";
//$url = "http://www.fnde.gov.br/webservices/sigefemendas/index.php/empenho/solicitar";

try {
$xml = Fnde_Webservice_Client::CreateRequest()
	->setURL('http://www.fnde.gov.br/webservices/sigefemendas/index.php/empenho/solicitar')
    ->setParams( array('xml' => $arqXml, 'usuario' => $usuario, 'senha' => $senha) )
    ->execute(true);

	$xml = simplexml_load_string( stripslashes($xml));
    if ( (int) $xml->status->result ){
        $rows = $xml->body->row->children();
        foreach($rows as $key => $value){
            $value = utf8_decode($value);
            echo "<b>{$key}</b>: {$value} <br/>";
        }
    } else {
        throw new Exception('['. $xml->status->error->message->code . '] ' .utf8_decode($xml->status->error->message->text));
    }
    echo "</fieldset>";
} catch (Exception $e){
    echo "Erro: [{$e->getCode()}] -  {$e->getMessage()}";
}*/

