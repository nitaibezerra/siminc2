<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		<style type="">
			@media print {.notprint { display: none } .div_rolagem{display: none} }	
			@media screen {.notscreen { display: none; }
			
			.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
			
		</style>		
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
<?php
include APPRAIZ. 'includes/classes/relatorio.class.inc';

$sql   = monta_sql();
//ver($sql,d);
$dados = $db->carregar($sql);
//ver($dados,d);

$agrup = monta_agp();

$col   = monta_coluna();
$r = new montaRelatorio();
//ver($agrup);
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setBrasao(true);
//$r->setTotNivel(true);
$r->setEspandir(false);
//$r->setBrasao(true);
echo $r->getRelatorio();

?>
<table  align="center" cellspacing="1" cellpadding="4">
	<tr>
		<td style="height: 20px;"></td>
	</tr>
	<tr>
		<td style="text-align: center;" class="div_rolagem">
			<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
		</td>
	</tr>
</table>
</body>
<?php

function monta_sql(){
	$filtro = array();
	
	extract($_POST);
	
	//ver($_POST);
	
	$autid 		 = !is_array($autid) ? explode(',', $autid) : $autid;
	$responsavel = !is_array($responsavel) ? explode(',', $responsavel) : $responsavel;
	$parid 		 = !is_array($parid) ? explode(',', $parid) : $parid;
	$tpaid		 = !is_array($tpaid) ? explode(',', $tpaid) : $tpaid;
	$estuf		 = !is_array($estuf) ? explode(',', $estuf) : $estuf;
	$entid 		 = !is_array($entid) ? explode(',', $entid) : $entid;
	$unicod 	 = !is_array($unicod) ? explode(',', $unicod) : $unicod;
	$gndcod 	 = !is_array($gndcod) ? explode(',', $gndcod) : $gndcod;
	$acacod 	 = !is_array($acacod) ? explode(',', $acacod) : $acacod;
	$espid 	 	 = !is_array($espid) ? explode(',', $espid) : $espid;
	
	if( $autid[0] && ( $autid_campo_flag || $autid_campo_flag == '1' )){
		$where[] = " aut.autid " . (( $autid_campo_excludente == null || $autid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $autid ) . "') ";		
	}
	if( $responsavel[0] && ( $responsavel_campo_flag || $responsavel_campo_flag == '1' )){
		$where[] = " res.resid " . (( $responsavel_campo_excludente == null || $responsavel_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $responsavel ) . "') ";		
	}
	if( $parid[0] && ( $parid_campo_flag || $parid_campo_flag == '1' )){
		$where[] = " par.parid " . (( $parid_campo_excludente == null || $parid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $parid ) . "') ";		
	}
	if( $tpaid[0] && ( $tpaid_campo_flag || $tpaid_campo_flag == '1' )){
		$where[] = " tpa.tpaid" . (( $tpaid_campo_excludente == null || $tpaid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpaid ) . "') ";		
	}
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " aut.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	if( $entid[0] && ( $entid_campo_flag || $entid_campo_flag == '1' )){
		$where[] = " ent.entid " . (( $entid_campo_excludente == null || $entid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $entid ) . "') ";		
	}
	if( $unicod[0] && ( $unicod_campo_flag || $unicod_campo_flag == '1' )){
		$where[] = " vfun.unicod " . (( $unicod_campo_excludente == null || $unicod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unicod ) . "') ";		
	}
	if( $gndcod[0] && ( $gndcod_campo_flag || $gndcod_campo_flag == '1' )){
		$where[] = " emd.gndcod " . (( $gndcod_campo_excludente == null || $gndcod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $gndcod ) . "') ";		
	}
	if( $acacod[0] && ( $acacod_campo_flag || $acacod_campo_flag == '1' )){
		$where[] = " vfun.acacod " . (( $acacod_campo_excludente == null || $acacod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acacod ) . "') ";		
	}
	if( $espid[0] && ( $espid_campo_flag || $espid_campo_flag == '1' )){
		$where[] = " esp.espid " . (( $espid_campo_excludente == null || $espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $espid ) . "') ";		
	}
	
	if($emecod){
		$where[] = "eme.emecod = '".$emecod."'";
	}

	
	$agrupador = $_REQUEST['agrupador'];

	$order = array();
	foreach ($agrupador as $val){
		switch ($val) {
			case 'emecod':
				$order[] = 'eme.emecod';
			break;
			case 'entid':
				$order[] = 'ent.entid';
			break;
			case 'acacod':
				$order[] = 'vfun.acacod';
			break;
			case 'autid':
				$order[] = 'aut.autid';
			break;
			case 'estuf':
				$order[] = 'aut.estuf';
			break;
			case 'parid':
				$order[] = 'par.parid';
			break;
			case 'gndcod':
				$order[] = 'emd.gndcod';
			break;
			case 'resid':
				$order[] = 'res.resid';
			break;
			case 'tpaid':
				$order[] = 'tpa.tpaid';
			break;
			case 'unicod':
				$order[] = 'vfun.unicod';
			break;
		}
	}

	$sql = "SELECT
				case when eme.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome,
			    par.parsigla,
			    aut.estuf,
			    (CASE WHEN res.resid is not null THEN res.resdsc
			        ELSE 'Não Informado' 
			    END) as responsavel,
			    eme.emecod,
			    vfun.fupfuncionalprogramatica,
			    vfun.fupdsc,
			    emd.gndcod,
			    emd.foncod,
			    emd.mapcod,
			    (ent.entnumcpfcnpj ||' - '|| ent.entnome) as entidade,
			    ede.edeobjeto,
			    vfun.unicod,
			    (vfun.acacod ||' - '|| vfun.acadsc) as acaoorcamentaria,
			    tpa.tpanome,
				count(eme.emecod) as TotalEmenda,
			    emd.emdvalor as valoremenda,
			    COALESCE(ede.edevalor,0) as valorentidade,
			    COALESCE(count(ede.edevalor),0) as TotalEntidade
			FROM
				emenda.emenda eme
			    inner join emenda.emendadetalhe emd on emd.emeid = eme.emeid
			    inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
			    inner join emenda.autor aut on aut.autid = eme.autid
			    inner join emenda.partido par on par.parid = aut.parid
			    inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
			    inner join emenda.v_funcionalprogramatica vfun on vfun.acaid = eme.acaid
			    inner join emenda.responsavel res on res.resid = eme.resid
			    inner join entidade.entidade ent on ent.entid = ede.entid
			inner join emenda.iniciativa ini on ini.resid = res.resid
			inner join emenda.iniciativaespecificacao ine on ine.iniid = ini.iniid
			inner join emenda.especificacao esp on esp.espid = ine.espid
			inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
			--inner join emenda.especificacao esp on res.resid = esp.resid
			    ".(($where)?" WHERE ".implode(" AND ", $where):"")."
			GROUP BY
				emd.emdvalor,
			    ede.edevalor,
			    aut.autnome,
			    eme.emerelator,
			    par.parsigla,
			    aut.estuf,
			    responsavel,
			    eme.emecod,
			    vfun.fupfuncionalprogramatica,
			    vfun.fupdsc,
			    emd.gndcod,
			    emd.foncod,
			    emd.mapcod,
			    entidade,
			    ede.edeobjeto,
			    vfun.unicod,
			    acaoorcamentaria,
			    tpa.tpanome,
			    ent.entid,
			    vfun.acacod,
			    par.parid,
			    res.resid,
			    tpa.tpaid,
			    vfun.unicod,
			    aut.autid
			ORDER BY ".
				implode(',', $order);
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	
	if (!$agrupador){
		$agrupador = array(
							'autid',
							'acacod',
							'emecod',
							'entid',
							'estuf',
							'gndcod',
							'parid',
							'resid',
							'tpaid',
							'unicod'
						 );
	}elseif ( !is_array($agrupador) ){
		$agrupador = explode(",", $agrupador);
	}
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array(
											"totalemenda",
											"valoremenda",	
											"totalentidade",	
											"valorentidade"	
										  )	  
				);

	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'resid':
				array_push($agp['agrupador'], array(
													"campo" => "responsavel",
											  		"label" => "Responsavel")										
									   				);				
		    	continue;
		        break;
		    case 'emecod':
				array_push($agp['agrupador'], array(
													"campo" => "emecod",
											  		"label" => "Emenda")										
									   				);					
		    	continue;
		        break;		    	
		    case 'entid':
				array_push($agp['agrupador'], array(
													"campo" => "entidade",
											 		"label" => "Entidade")										
									   				);					
		    	continue;			
		        break;	
		    case 'unicod':
				array_push($agp['agrupador'], array(
												"campo" => "unicod",
												"label" => "Unidade")										
										   		);	
				continue;
				break;	    	
		    case 'acacod':
				array_push($agp['agrupador'], array(
												"campo" => "acaoorcamentaria",
												"label" => "Ação")										
										   		);	
				continue;
				break;	    	
		    case 'gndcod':
				array_push($agp['agrupador'], array(
												"campo" => "gndcod",
												"label" => "GND")										
										   		);
				continue;
				break;					
		    case 'autid':
				array_push($agp['agrupador'], array(
												"campo" => "autnome",
												"label" => "Autor")										
										   		);	
				continue;
				break;					
		    case 'tpaid':
				array_push($agp['agrupador'], array(
												"campo" => "tpanome",
												"label" => "Tipo de Autor")										
										   		);	
				continue;
				break;					
		    case 'estuf':
				array_push($agp['agrupador'], array(
												"campo" => "estuf",
												"label" => "Estado")										
										   		);	
				continue;
				break;					
		    case 'parid':
				array_push($agp['agrupador'], array(
												"campo" => "parsigla",
												"label" => "Partido")										
										   		);	
				continue;
				break;					
		}
	endforeach;
	return $agp;
}

function monta_coluna(){

	$coluna = array(
					array(
						"campo" => "totalemenda",
					   	"label" => "Quantidade de Emenda",
						"type"  => "numeric",
						"html" 	=> "{valorCampo}",
								   "php"   => array(
							  					"expressao" => "'{totalemenda}' != '0,00'",
												"var"		=> "valorCampo",
											 	"true"		=> "{totalemenda}",
											 	"false"		=> "-"
											   ) 
						),
					array(
						"campo" => "valoremenda",
					   	"label" => "Valor da Emenda (R$)",
						//"type"  => "numeric",
						),
					array(
						"campo" => "totalentidade",
					   	"label" => "Quantidade de Entidade",
						"type"  => "numeric"
						),
					array(
						"campo" => "valorentidade",
					   	"label" => "Valor da Entidade (R$)",
						//"type"  => "numeric"
						)
					);
	return $coluna;
}

?>