<title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php

function obras_monta_agp_painel_gerencial(){
	
	$colunas = $_REQUEST["colunas"];
	$agrupador = $colunas;//array('nomedaobra');
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( 'cpf', 'banco', 'conta', 'agencia', 'datainclusao', 'codigoscdb', 'objetivo',
										   'empenho', 'itinerario', 'ajudatotal', 'embarquetotal', 'deducao', 'vlrtotal', 'situacao' )
				);
	
	array_push($agp['agrupador'], array(
										"campo" => "nome",
								  		"label" => "Nome")										
						   				);
	
	return $agp;
	
}

/**
 * Funçao que monta o sql para trazer o relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_painel_gerencial(){
	
	global $db;
	
	$where = array();
	
	array_push($where, 'sol.solstatus = \'A\' ');
	
	// Filtros
	if($_REQUEST['ungcod'][0]!=''){
		array_push($where, 'ung.ungcod IN (\''.implode('\',\'',$_REQUEST['ungcod']).'\')');
	}
	if($_REQUEST['empcod'][0]!=''){
		array_push($where, 'emu.empcod IN (\''.implode('\',\'',$_REQUEST['empcod']).'\')');
	}
	if($_REQUEST['esdid'][0]!=''){
		array_push($where, 'esd.esdid IN (\''.implode('\',\'',$_REQUEST['esdid']).'\')');
	}
	if($_REQUEST['usucpf'][0]!=''){
		array_push($where, 'sol.usucpf IN (\''.implode('\',\'',$_REQUEST['usucpf']).'\')');
	}
	if($_REQUEST['data1']!=''){
		array_push($where, 'sol.soldatainclusao >= \''.ajusta_data($_REQUEST['data1']).'\'');
	}
	if($_REQUEST['data2']!=''){
		array_push($where, 'sol.soldatainclusao <= \''.ajusta_data($_REQUEST['data2']).' 23:59:59\'');
	}
	if(!$db->testa_superuser()){
		$perfil = pegaPerfilArray($_SESSION['usucpf'],$_SESSION['sisid']);
	
		$sql = "SELECT DISTINCT 
				p.ungcod,
				eug.empcod
			FROM 
				evento.usuarioresponsabilidade ur 
			INNER JOIN public.unidadegestora 		   p ON ur.ungcod  = p.ungcod
			INNER JOIN evento.empenho_unidadegestora eug ON eug.ungcod = ur.ungcod
			WHERE
				ur.rpustatus = 'A' and
				ur.usucpf = '".$_SESSION['usucpf']."' and
				ur.pflcod IN (".implode(',',$perfil).") and
				ur.prsano = '".$_SESSION['exercicio']."'";

		$notUngcod = $db->carregarColuna($sql);
		array_push($notUngcod, '00000000');
		$notEmpcod = $db->carregarColuna($sql,'empcod');
		array_push($notEmpcod, '00000000');
	}
	
	// monta o sql 
	$sql = "SELECT DISTINCT
				--Colunas
				--1 as qtd,
				solnome as nome,
				replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as cpf,
				'\''||solbanco||'' as banco,
				'\''||solagencia||'' as agencia,
				'\''||solconta||'' as conta,
				to_char(sol.soldatainclusao,'DD/MM/YYYY') as datainclusao,
				CASE WHEN trim(solcodigoscdb) = ''
					THEN 'Objetivo'
					ELSE solcodigoscdb
				END codigoscdb,
				CASE WHEN trim(solobjetivo) = ''
					THEN 'Objetivo'
					ELSE trim(LEADING '	' FROM solobjetivo)
				END objetivo,
				emu.empcod||' - '||coalesce(emu.empdsc,'Empenho') as empenho,
				evento.retoritinerario(sol.solid) as itinerario,
				round(vlr,2) as ajudatotal,
				round(embarque,2) as embarquetotal,
				round((soldeducaoalim+(soldeducaotransp*soltotdias)),2) as deducao,
				round((vlr+embarque-(soldeducaoalim+(soldeducaotransp*soltotdias))),2) as vlrtotal,
				esd.esddsc as situacao
			FROM
				evento.solicitacaodiaria sol
			INNER JOIN public.unidadegestora 	 ung ON ung.ungcod = sol.ungcod  
			INNER JOIN evento.empenho_unidadegestora emu ON emu.ungcod = sol.ungcod AND emu.empcod = sol.empcod
			INNER JOIN workflow.documento 	   	 doc ON doc.docid  = sol.docid
			INNER JOIN workflow.estadodocumento  esd ON esd.esdid  = doc.esdid
			INNER JOIN (
					SELECT 
						solid,
						sum(itidias*(itivalordiaria*(CASE WHEN itimeiadiaria THEN 0.5 ELSE 1 END))+(CASE WHEN itiultimo THEN itivalordiaria/2 ELSE 0 END)) as vlr,
						sum(itivaloradicionalembarque) as embarque
						
					FROM
						evento.itinerariosolicitacaodiaria
					WHERE
						itistatus = 'A'
					GROUP BY solid ) iti ON iti.solid = sol.solid
			WHERE
				".(count($notUngcod)>0&&(!$db->testa_superuser()) ? "sol.ungcod IN ('".implode('\',\'', $notUngcod)."') AND" : "")."
				".(count($notEmpcod)>0&&(!$db->testa_superuser()) ? "sol.empcod IN ('".implode('\',\'', $notEmpcod)."') AND" : "")."
				1=1
				".( (count($where)>0) ? 'AND '.implode(' AND ',$where) : '')."
			GROUP BY
				soldeducaoalim,
				soldeducaotransp,
				iti.vlr,
				iti.embarque,
				emu.empcod,
				emu.empdsc,
				ung.ungdsc,
				esd.esddsc,
				sol.solid,
				sol.usucpf,
				sol.solnome,
				sol.soldatainclusao,
				sol.soltotdias,
				solbanco,
				solconta,
				solagencia,
				solcodigoscdb,
				solobjetivo
			ORDER BY
				empenho,
				esd.esddsc,
				nome" ;
//	ver($sql,d);
	return $sql;
	
}

function obras_monta_coluna_painel_gerencial(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "cpf",
					   		   "label" 	  => "CPF",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "banco",
					   		   "label" 	  => "Banco",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "agencia",
					   		   "label" 	  => "Agência",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "conta",
					   		   "label" 	  => "Conta Corrente",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "datainclusao",
					   		   "label" 	  => "Data da Inclusão",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "codigoscdb",
					   		   "label" 	  => "Código SCDP",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "objetivo",
					   		   "label" 	  => "Objetivo da Viagem",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "empenho",
					   		   "label" 	  => "Nota de Empenho",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "itinerario",
					   		   "label" 	  => "Itinerário",
					   		   "blockAgp" => "",
					   		   "type"	  => "string") );
	array_push( $coluna, array("campo" 	  => "ajudatotal",
					   		   "label" 	  => "Valor Ajuda de Custo",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "embarquetotal",
					   		   "label" 	  => "Adicional Embarque",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "deducao",
					   		   "label" 	  => "Deduções",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "vlrtotal",
					   		   "label" 	  => "Valor a Pagar",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "situacao",
					   		   "label" 	  => "Situação",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	
	return $coluna;
	
}


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */


// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

monta_titulo('Relatório de Ajuda de Custo', '' );

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$agrupador = obras_monta_agp_painel_gerencial();
$sql       = obras_monta_sql_painel_gerencial(); 
$coluna    = obras_monta_coluna_painel_gerencial();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados); 
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir(false);
$rel->setTotNivel(true);

if( $_REQUEST['tiporel'] == 2 ){
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$arCabecalho = Array('Nome','CPF','Banco','Agência','Conta Corrente','Data da Inclusão','Código SCDP','Objetivo da Viagem','Nota de Empenho','Itinerário','Valor Ajuda de Custo','Adicional Embarque','Deduções','Valor a Pagar','Situação');
	$db->monta_lista_tabulado($sql,$arCabecalho,100000,5,'N','100%',$par2);
	exit();
}else{
	echo $rel->getRelatorio(); 
}
?>