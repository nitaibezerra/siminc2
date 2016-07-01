<?
set_time_limit(0);

if ( $_REQUEST['filtrosession'] ){
	$filtroSession = $_REQUEST['filtrosession'];
}

?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
$dados = $db->carregar($sql);
$agrup = monta_agp();
$col   = monta_coluna();

$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao($true ? true : false);
$r->setTotNivel(true);
$r->setEspandir($_POST['expandir']);
//$r->setBrasao(true);
if( $_POST['req'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Viagens_Exterior_-_".date('d-m-Y_H_i');
	echo $r->getRelatorioXls();
}else{
	echo $r->getRelatorio();
}

function monta_sql(){
	
	extract($_POST);
	
	$select = array();
	$from	= array();
	
	if( $ano[0] && ( $ano_campo_flag || $ano_campo_flag == '1' )){
		$where[] = " to_char(tbl1.avedata,'YYYY') " . (($ano_campo_excludente == null || $ano_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $ano ) . "') ";
	}
		
	if( $unidade[0] && ( $unidade_campo_flag || $unidade_campo_flag == '1' )){
		$where[] = " ent.entid " . (($unidade_campo_excludente == null || $unidade_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unidade ) . "') ";
	}
	
	if( $programa[0] && ( $programa_campo_flag || $programa_campo_flag == '1' )){
		$where[] = " pg.pveid " . (($programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $programa ) . "') ";
	}
	
	if( $situacao[0] && ( $situacao_campo_flag || $situacao_campo_flag == '1' )){
		$where[] = " ed.esdid " . (($situacao_campo_excludente == null || $situacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $situacao ) . "') ";
	}
		
	/*
	$agrupador = is_array($agrupador) ? implode(',', $agrupador) : $agrupador; 
	
	$agrupador = $_REQUEST['agrupador'];
	$agrupador[array_search('detalhessolicitacao',$agrupador)] = 'objeto';
	*/
	//SQL NOVO
	$sql = "select
				entnome as unidade,
				tbl1.aveid as nusolicitacao,
				esddsc as situacao,
				ed.esdid,
				ent.entid,
				avenomereitor as reitor,
				avenumauto as nuautorizacao,
				pvedsc as programa,
				--( select sum(qtde) from academico.autvexqtdeprg atp where atp.aveid = tbl1.aveid) as qtde_solicitada,
				qtde as qtde_solicitada,
				to_char(avedata,'DD/MM/YYYY') as datasolicitacao,
				'1' as qtde_reg,
				'Detalhes da Solicitação Nº '||tbl1.aveid as detalhessolicitacao
			from 
				academico.autviagemexterior tbl1
			inner join 
				academico.autvexqtdeprg au on au.aveid = tbl1.aveid
			inner join 
				academico.progviagemexterior pg on pg.pveid = au.pveid
			inner join 
				entidade.entidade ent ON tbl1.entid = ent.entid
			inner join 
				workflow.documento d ON d.docid = tbl1.docid
			inner join 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			where 
				avestatus = 'A'
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
			ORDER BY 
				" . (is_array($agrupador) ? implode(' , ', $agrupador)."," : '') . "
				tbl1.aveid ";
		/*
	$sql = "SELECT 
				sldid as id,
				CASE WHEN ent.entsig <> '' THEN
					ent.entsig || ' - '
				     ELSE
					''
				END
				|| COALESCE(ent.entnome , 'Não informado') as instituicao ,
				ent.entid as idinstituicao,
				tpa.tpadsc as tiposolicitacao,
				tpa.tpaid as idtiposolicitacao, 
				sld.sldid || ' - ' || sld.sldobjeto as objeto, 
				sld.sldjustificativa as justificativa, 
				sld.sldvlrestimado as valor,
				CASE fue.funid 
					WHEN 11 THEN 'Educação Profissional'
					WHEN 12 THEN 'Educação Superior'
					WHEN 16 THEN 'Hospitais'
					ELSE 'Não informado'
				END  as tipoensino,
				'1' as qtde,
				gnd.gndcod || ' - ' || gnd.gnddsc as gnd,
				slf.slfdsc as momento
			FROM 
				academico.solicitacaodecreto sld
			INNER JOIN entidade.entidade 	     ent ON sld.entid = ent.entid
			INNER JOIN entidade.funcaoentidade   fue ON fue.entid = ent.entid and fuestatus='A'
			INNER JOIN academico.tiposolicitacao tpa ON tpa.tpaid = sld.tpaid
			LEFT JOIN public.gnd 				 gnd ON gnd.gndcod = sld.gndcod
			LEFT JOIN academico.sldfase			 slf ON slf.slfid = sld.slfid
			WHERE 
				entstatus = 'A' 
				AND sld.sldstatus = 'A' 
				AND fue.funid in (11,12,16)
				" . (is_array($where) ? " AND ".implode(' AND ', $where) : '') . "
			ORDER BY 
				" . (is_array($agrupador) ? implode(' , ', $agrupador)."," : '') . "
				ent.entsig, 
				ent.entnome, 
				ent.entnome ";
		*/
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"unidade",	
									   		"situacao", 
 									   		"ano",
 									   		"nusolicitacao",
 									   		"programa",
											"detalhessolicitacao",
											"qtde_solicitada",
											"qtde_reg",
											"datasolicitacao",
											"nuautorizacao"
										  )	  
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'unidade':
		    	array_push($agp['agrupador'], array(
													"campo" => "unidade",
											  		"label" => "Unidade")										
									   				);
   								
		    	continue;
		        break;
			case 'situacao':
				array_push($agp['agrupador'], array(
													"campo" => "situacao",
											  		"label" => "Situação")										
									   				);				
		    	continue;
		        break;		        
		    case 'programa':
		    	array_push($agp['agrupador'], array(
													"campo" => "programa",
											  		"label" => "Programa")										
									   				);
   								
		    	continue;
		        break;
		    case 'detalhessolicitacao':
		    	array_push($agp['agrupador'], array(
													"campo" => "detalhessolicitacao",
											  		"label" => "Detalhes Solicitação")										
									   				);
   								
		    	continue;
		        break;
		}
	endforeach;
	return $agp;
}

function monta_coluna(){
	
	
	$agrupador = $_REQUEST['agrupador'];
	
	if(in_array('detalhessolicitacao',$agrupador)){
	
	$coluna    = array(
						array(
							  "campo" => "nuautorizacao",
					   		  "label" => "Nº. Autorização",
							  "type"  => "string"
						),
						array(
							  "campo" => "datasolicitacao",
					   		  "label" => "Data",
							  "type"  => "string"
						),
						array(
							  "campo" => "qtde_solicitada",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
						)
					);
					
	}else{
					
	$coluna    = array(
						array(
							  "campo" => "qtde_solicitada",
					   		  "label" => "Quantidade",
							  "type"  => "numeric"
						)						
					);
					
	}				
					
	return $coluna;			  	
}
?>
</body>