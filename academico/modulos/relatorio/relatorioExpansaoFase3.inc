<?php
if ( $_POST['pesquisa'] || $_GET['pesquisa'] ){
	ini_set("memory_limit", "1024M");
	set_time_limit(0);
	
	include APPRAIZ. 'includes/classes/relatorio.class.inc';
	
	if($_POST['tipoRel'] == 'html'){
		$sql = monta_sql();		
	}
	$dados  = $db->carregar($sql);
	$agrup  = monta_agp();
	$coluna = monta_coluna();
	
	$obRelatorio = new montaRelatorio();
	
	if($_POST['tipoRel'] == 'xls'){
		$arCabecalho = array();
		foreach ($_POST['agrupador'] as $agrup){
			if($agrup == 'campus'){
				array_push($arCabecalho, 'Campus');
			}
			if($agrup == 'instituicao'){
				array_push($arCabecalho, 'Instituição');
			}
		}
		array_push($arCabecalho, 'Semestre');
		if(!$_POST['tipo']){
			array_push($arCabecalho, 'Tipo');
		}
		foreach($coluna as $cabecalho){
			array_push($arCabecalho, $cabecalho['label']);
		}
		
		$sql = monta_sql_xls($agrup['agrupador']);
		
		$arDados = $db->carregar($sql);
		$arDados = ($arDados) ? $arDados : array();
		
		header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
		header ( "Pragma: no-cache" );
		header ( "Content-type: application/xls; name=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
		header ( "Content-Description: MID Gera excel" );
		$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	    die;
	}
	
	$obRelatorio->setAgrupador($agrup, $dados);
	$obRelatorio->setColuna($coluna);
	//$obRelatorio->setBrasao($true ? true : false);
	//$obRelatorio->setOverFlowTableHeight(500);
	$obRelatorio->setTotNivel(true);
	//$obRelatorio->setEspandir(false);
	
	?>
	<html>
		<head>
			<script type="text/javascript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
			<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
			<script type="text/javascript" src="../includes/remedial.js"></script>
			<script type="text/javascript" src="../includes/superTitle.js"></script>
		</head>
	<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">
	
	<?php 
	echo $obRelatorio->getRelatorio();
	?>
	</body>
	</html>
	<?php 
	die;	
}

function monta_sql_xls($arAgrupador){
	
	$where = array();
	
	extract($_POST);
	
	foreach($agrupador as $agr){
		if($agr == "faixadepopulacao"){
			$arrCampus[] = "fxptpm.tpmdsc as faixadepopulacao";
			$arrInner[] = "left join
								territorios.muntipomunicipio fxpmtm ON fxpmtm.muncod = mun.muncod
							left join
								territorios.tipomunicipio fxptpm ON fxptpm.tpmid = fxpmtm.tpmid
							left join
								territorios.grupotipomunicipio fxpgtm ON fxpgtm.gtmid = fxptpm.gtmid and fxpgtm.gtmid = 6";
		}
		if($agr == "territoriodacidadania"){
			$arrCampus[] = "tdctpm.tpmdsc as territoriodacidadania";
			$arrInner[] = "left join
								territorios.muntipomunicipio tdcmtm ON tdcmtm.muncod = mun.muncod
							left join
								territorios.tipomunicipio tdctpm ON tdctpm.tpmid = tdcmtm.tpmid
							left join
								territorios.grupotipomunicipio tdcgtm ON tdcgtm.gtmid = tdctpm.gtmid and tdcgtm.gtmid = 5 and tdcmtm.tpmid != 139";
		}
		if($agr == "instituto"){
			$instituto = true;
			$arrCampus[] = "ins.entnome as instituto";
			$arrInner[] = "/* Início Instituto */
							left join
								entidade.funentassoc ass ON ass.fueid = fun.fueid
							left join
								entidade.entidade ins ON ins.entid = ass.entid
							left join
								entidade.funcaoentidade fun2 ON fun2.entid = ins.entid and fun2.funid = 11
							/* Fim Instituto */";
		}
	}
	
	// Instituições
	if($entid[0] && $instituto){
		array_push($where, " ins.entid in ('" . implode( "','", $entid ) . "') ");
	}
	// Campus
	if($campus[0]){
		array_push($where, " cam.entid in ('" . implode( "','", $campus ) . "') ");
	}
		
	$sql = "select 
				reg.regdescricao as regiao,
				mes.mesdsc as mesoregiao,
				mun.mundescricao as municipio,
				mun.estuf as estado,
				cam.entnome as campus,
				".($arrCampus ? implode(",",$arrCampus)."," : "")."
				count(distinct mun.muncod) as qtdemunicipio,
				mun.munpopulacao as qtdepopulacao,
				(CASE
					WHEN aca.exiid = '2' THEN 1
					ELSE 0
				END) as qtdeunidpreexistente,
				(CASE
					WHEN aca.exiid != '2' THEN 1
					ELSE 0
				END) as qtdeexpcampusava,
				0 as estplaexp600,
				0 as estplaexp100
			from
				territorios.regiao reg 
			left join
				territorios.estado est ON est.regcod = reg.regcod
			left join 
				territorios.mesoregiao mes ON mes.estuf = est.estuf
			left join
				territorios.municipio mun ON mun.mescod = mes.mescod
			/* Início Campus */
			left
				entidade.endereco ede ON ede.muncod = mun.muncod and ede.endstatus = 'A'
			left join
				entidade.entidade cam ON ede.entid = cam.entid
			left join
				entidade.funcaoentidade fun ON fun.entid = cam.entid and fun.funid = 17 and cam.entstatus = 'A'
			".($arrInner ? implode(" ",$arrInner) : "")."
			left join
				academico.campus aca ON aca.entid = cam.entid
				academico.existencia ex ON ex.exiid = cam.exiid
			/* Fim Campus */
			" . ( is_array($where) && count($where) ? ' where' . implode(' AND ', $where) : '' ) ."
			GROUP BY
				mun.munpopulacao,
				aca.exiid,
				reg.regdescricao,
				mes.mesdsc,
				mun.mundescricao,
				mun.estuf,
				cam.entnome
				".($agrupador ? ",".implode(",",$agrupador) : "")."
			ORDER BY 
				" . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' );
//	ver($sql,d);
	return $sql;
}

function monta_sql(){
	
	$where = array();
	
	extract($_POST);
	
	foreach($agrupador as $agr){
		if($agr == "faixadepopulacao"){
			$arrCampus[] = "fxptpm.tpmdsc as faixadepopulacao";
			$arrInner[] = "inner join
								territorios.muntipomunicipio fxpmtm ON fxpmtm.muncod = mun.muncod
							inner join
								territorios.tipomunicipio fxptpm ON fxptpm.tpmid = fxpmtm.tpmid
							inner join
								territorios.grupotipomunicipio fxpgtm ON fxpgtm.gtmid = fxptpm.gtmid and fxpgtm.gtmid = 6";
		}
		if($agr == "territoriodacidadania"){
			$arrCampus[] = "tdctpm.tpmdsc as territoriodacidadania";
			$arrInner[] = "inner join
								territorios.muntipomunicipio tdcmtm ON tdcmtm.muncod = mun.muncod
							inner join
								territorios.tipomunicipio tdctpm ON tdctpm.tpmid = tdcmtm.tpmid
							inner join
								territorios.grupotipomunicipio tdcgtm ON tdcgtm.gtmid = tdctpm.gtmid and tdcgtm.gtmid = 5 and tdcmtm.tpmid != 139";
		}
		if($agr == "instituto"){
			$instituto = true;
			$arrCampus[] = "ins.entnome as instituto";
			$arrInner[] = "/* Início Instituto */
							inner join
								entidade.funentassoc ass ON ass.fueid = fun.fueid
							inner join
								entidade.entidade ins ON ins.entid = ass.entid and ins.entstatus = 'A'
							inner join
								entidade.funcaoentidade fun2 ON fun2.entid = ins.entid and fun2.funid = 11
							/* Fim Instituto */";
		}
	}
	
	// Instituições
	if($entid[0] && $instituto){
		array_push($where, " ins.entid in ('" . implode( "','", $entid ) . "') ");
	}
	// Campus
	if($campus[0]){
		array_push($where, " cam.entid in ('" . implode( "','", $campus ) . "') ");
	}
		
	$sql = "select 
				reg.regdescricao as regiao,
				mes.mesdsc as mesoregiao,
				mun.mundescricao as municipio,
				mun.estuf as estado,
				count(distinct mun.muncod) as qtdemunicipio,
				mun.munpopulacao as qtdepopulacao,
				(CASE
					WHEN aca.exiid = '2' THEN 1
					ELSE 0
				END) as qtdeunidpreexistente,
				(CASE
					WHEN aca.exiid != '2' THEN 1
					ELSE 0
				END) as qtdeexpcampusava,
				".($arrCampus ? implode(",",$arrCampus)."," : "")."
				cam.entnome as campus
			from
				entidade.endereco ede
			inner join
				entidade.entidade cam ON ede.entid = cam.entid
			inner join
				entidade.funcaoentidade fun ON fun.entid = cam.entid and fun.funid = 17 and cam.entstatus = 'A'
			inner join
				academico.campus aca ON aca.entid = cam.entid
			right join
				territorios.municipio mun ON mun.muncod = ede.muncod
			inner join
				territorios.mesoregiao mes ON mes.estuf = mun.estuf
			inner join
				territorios.estado est ON mun.estuf = est.estuf
			inner join
				territorios.regiao reg ON reg.regcod = est.regcod
			".($arrInner ? implode(" ",$arrInner) : "")."
			/* Fim Campus */
			" . ( is_array($where) && count($where) ? ' where' . implode(' AND ', $where) : '' ) ."
			GROUP BY
					mes.mesdsc,
					mun.mundescricao,
					mun.estuf,
					mun.munpopulacao,
					cam.entnome,
					aca.exiid
				".($agrupador ? ",".implode(",",$agrupador) : "")."
			ORDER BY 
				" . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' );
	//ver($sql,d);
	return $sql;
}

function monta_agp(){

	$agrupador = (array) $_POST['agrupador'];
	
	$agp = array(
				 "agrupador" 	  => array(),
				 "agrupadoColuna" => array(	"qtdemunicipio",
				 							"qtdepopulacao",
											"qtdeunidpreexistente",
											"qtdeexpcampusava",
											"estplaexp600",
											"estplaexp100"
										   )
				);
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'regiao':
				array_push($agp['agrupador'], array(
													"campo" => "regiao",
											 		"label" => "Região")										
									   				);					
		        break;	
		    case 'mesoregiao':
				array_push($agp['agrupador'], array(
												"campo" => "mesoregiao",
												"label" => "Mesoregião")										
										   		);	
				break;	    	
		    case 'municipio':
				array_push($agp['agrupador'], array(
												"campo" => "municipio",
												"label" => "Município")										
										   		);	
				break;					
		    case 'instituto':
				array_push($agp['agrupador'], array(
												"campo" => "instituto",
												"label" => "Instituto")										
										   		);	
				break;
			case 'campus':
				array_push($agp['agrupador'], array(
												"campo" => "campus",
												"label" => "Campus")										
										   		);	
				break;
			case 'territoriodacidadania':
				array_push($agp['agrupador'], array(
												"campo" => "territoriodacidadania",
												"label" => "Território da Cidadania")										
										   		);	
				break;
			case 'faixadepopulacao':
				array_push($agp['agrupador'], array(
												"campo" => "faixadepopulacao",
												"label" => "Faixa de População")										
										   		);	
				break;
			case 'estado':
				array_push($agp['agrupador'], array(
												"campo" => "estado",
												"label" => "Estado")										
										   		);	
				break;						
		}
	endforeach;
	return $agp;
}

function monta_coluna(){

		$coluna = array(
						array(
							  "campo" => "qtdemunicipio",
					   		  "label" => "Qtde Municípios",
					   		  "type"  => "numeric"
						),
						array(
							  "campo" => "qtdepopulacao",
					   		  "label" => "População",
					   		  "type"  => "numeric"
						),
						array(
							  "campo" => "qtdeunidpreexistente",
					   		  "label" => "Unidades Pré-Existentes",
					   		  "type"  => "numeric"
						),
						array(
							  "campo" => "qtdeexpcampusava",
					   		  "label" => "Qtde Expansão I,II e Campus Avançados",
					   		  "type"  => "numeric"
						),
						array(
							  "campo" => "estplaexp600",
					   		  "label" => "Estimativa Plano de Expansão III com 600",
					   		  "type"  => "numeric"
						),
						array(
							  "campo" => "estplaexp100",
					   		  "label" => "Estimativa Plano de Expansão III com 100",
					   		  "type"  => "numeric"
						)
					);
	return $coluna;				  
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Expansão Fase III";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function abrepopupCampus(){
	var itemselecionado = '';
	for(i=0;i<document.formulario.entid.length;i++) {
		if(document.formulario.entid[i]) {
			itemselecionado = itemselecionado+','+document.formulario.entid[i].value;
		}
	}
	itemselecionado = "{}"+itemselecionado+"{}";
	janela = window.open('academico.php?modulo=relatorio/combo_campus&acao=A&institutos='+itemselecionado,'Unidades','width=400,height=400,toolbar=no,status=no,scrollbars=1,top=200,left=480');
	janela.focus();
}

function geraPopRelatorio(tipoRel){
	if ( !$('#agrupador option').length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado.' );
		return false;
	}
	
	selectAllOptions( document.getElementById('entid') );
	selectAllOptions( document.getElementById('campus') );
	selectAllOptions( document.getElementById('agrupador') );

	if(tipoRel != 'xls'){
		var janela = window.open( '', 'resultadoRelatorioDocente', 'fullscreen=1,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	}
	
	$('#tipoRel').val(tipoRel);
	$('#formulario').submit();
}
	
</script>
<form name="formulario" id="formulario" action="" method="post" target="resultadoRelatorioDocente">
<input type="hidden" name="tipoRel" id="tipoRel" value=""/>
<input type="hidden" name="pesquisa" value="1"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita">Agrupadores</td>
		<td>
			<?php
				// Início dos agrupadores
				$agrupador = new Agrupador('formulario','');
				
				// Dados padrão de origem
				$origem = array('regiao' => 
									array(
											'codigo'    => 'regiao',
											'descricao' => 'Região'
											),
								 'mesoregiao' => 
									array(
											'codigo'    => 'mesoregiao',
											'descricao' => 'Mesoregião'
											),
								'municipio' => 
									array(
											'codigo'    => 'municipio',
											'descricao' => 'Município'
											),
								'Instituto' => 
									array(
											'codigo'    => 'instituto',
											'descricao' => 'Instituto'
											),
								'Campus' => 
									array(
											'codigo'    => 'campus',
											'descricao' => 'Campus'
											),
								'territoriodacidadania' => 
									array(
											'codigo'    => 'territoriodacidadania',
											'descricao' => 'Território da Cidadania'
											),
								'faixadepopulacao' => 
									array(
											'codigo'    => 'faixadepopulacao',
											'descricao' => 'Faixa de População'
											),
								'estado' => 
									array(
											'codigo'    => 'estado',
											'descricao' => 'Estado'
											)
				 );
				
				// exibe agrupador
				$agrupador->setOrigem( 'naoAgrupador', null, $origem );
				$agrupador->setDestino( 'agrupador', null);
				$agrupador->exibir();
				
			?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Instituição</td>
		<td>
		<?
		$sqlComboInstituicao = "SELECT 
									ent.entid as codigo,
									ent.entnome as descricao 
								FROM
									entidade.entidade ent
								INNER JOIN entidade.funcaoentidade fue ON fue.entid = ent.entid
								WHERE 
									funid = 11 --Instituição
									AND ent.entstatus = 'A'";
		$entid = array(0 => array("codigo" => "", "descricao" => "Todas"));
		combo_popup("entid", $sqlComboInstituicao, "Entidade", "400x400", 0, '', "", "S", false, false, 5, 400);
		?>
		</td>
	</tr>
	<tr>
        <td class="SubTituloDireita" valign="top">Campus</td>
        <td>
	        <select multiple="multiple" size="5" name="campus[]" id="campus" ondblclick="abrepopupCampus();" class="CampoEstilo" style="width:400px;">
	        	<option value="">Todas</option>
	        </select>
        </td>
   	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Visualizar" onclick="geraPopRelatorio('html');" style="cursor: pointer;"/>
			<input type="button" value="Visualizar XLS" onclick="geraPopRelatorio('xls');" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>