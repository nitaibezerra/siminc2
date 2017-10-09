<title><?php echo NOME_SISTEMA; ?></title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?php

function obras_monta_agp_painel_gerencial(){
	
	$colunas = $_REQUEST["colunas"];
	$agrupador = $colunas;//array('nomedaobra');
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtd", "ajuda", "embarque", "deducao", "vlr" )
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "tipounidade":
				array_push($agp['agrupador'], array(
													"campo" => "tipounidade",
											  		"label" => "Tipo da Unidade")										
									   				);
			break;
			case "unidade":
				array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade")										
									   				);
			break;
			case "empenho":
				array_push($agp['agrupador'], array(
													"campo" => "empenho",
											  		"label" => "Nota de Empenho")										
									   				);
			break;
			case "situacao":
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação")										
									   				);
			break;
			case "beneficiario":
				array_push($agp['agrupador'], array(
													"campo" => "beneficiario",
											  		"label" => "Beneficiário")										
									   				);
			break;
			case "mes":
				array_push($agp['agrupador'], array(
													"campo" => "mes",
											  		"label" => "Mês")										
									   				);
			break;
			case "dia":
				array_push($agp['agrupador'], array(
													"campo" => "dia",
											  		"label" => "Data da Solicitação")										
									   				);
			break;
		}	
	}
	
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
	if($_REQUEST['tpuni']=='G'){
		array_push($where, 'sol.ungcod is not null');
	}
	if($_REQUEST['tpuni']=='O'){
		array_push($where, 'sol.unicod is not null');
	}
	if($_REQUEST['ungcod'][0]!=''){
		array_push($where, 'ung.ungcod IN (\''.implode('\',\'',$_REQUEST['ungcod']).'\')');
	}
	if($_REQUEST['unicod'][0]!=''){
		array_push($where, 'uni.unicod IN (\''.implode('\',\'',$_REQUEST['unicod']).'\')');
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
				1 as qtd,
				round(vlr,2) as ajuda,
				round(embarque,2) as embarque,
				round((soldeducaoalim+(soldeducaotransp*soltotdias)),2) as deducao,
				round((vlr+embarque-(soldeducaoalim+(soldeducaotransp*soltotdias))),2) as vlr,
				--Agrupadores/Filtros
				CASE 
					WHEN sol.ungcod is not null THEN 'Unidade Gestora'
					WHEN sol.unicod is not null THEN 'Unidade Orçamentária'
				END as tipounidade,
				coalesce(ung.ungdsc,uni.unidsc) as unidade,
				emu.empcod||' - '||coalesce(emu.empdsc,'Empenho') as empenho,
				esd.esddsc as situacao,
				sol.solnome||' - '||replace(to_char(sol.usucpf::bigint, '000:000:000-00'), ':', '.') as beneficiario,
				retornames(to_char(sol.soldatainclusao,'MM')) as mes,
				to_char(sol.soldatainclusao,'DD/MM/YYYY') as dia
				--Filtro Somente  de <= soldatainclusao <= até
			FROM
				evento.solicitacaodiaria sol
			LEFT  JOIN public.unidadegestora 	 ung ON ung.ungcod = sol.ungcod  
			LEFT  JOIN public.unidade 	 	 uni ON uni.unicod = sol.unicod 
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
				sol.usucpf,
				sol.solnome,
				sol.soldatainclusao,
				sol.soltotdias,
				sol.ungcod,
				sol.unicod,
				ung.ungdsc,
				uni.unidsc
			ORDER BY
				unidade,
				empenho,
				esd.esddsc,
				beneficiario" ;
//				ver($sql);
	return $sql;
	
}

function obras_monta_coluna_painel_gerencial(){
	
	$coluna = array();
	
	array_push( $coluna, array("campo" 	  => "qtd",
					   		   "label" 	  => "Quantidade de Solicitações",
					   		   "blockAgp" => "",
					   		   "type"	  => "numeric") );
	array_push( $coluna, array("campo" 	  => "ajuda",
					   		   "label" 	  => "Ajuda de Custo",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "embarque",
					   		   "label" 	  => "Adicional por Embarque",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "deducao",
					   		   "label" 	  => "Deduções",
					   		   "blockAgp" => "",
					   		   "type"	  => "float") );
	array_push( $coluna, array("campo" 	  => "vlr",
					   		   "label" 	  => "Valor Total",
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
	$nomeDoArquivoXls="Relatório_Ajuda_de_custo_".date('d-m-Y_H_i');
	echo $rel->getRelatorioXls();
}else{
	echo $rel->getRelatorio(); 
}
?>
