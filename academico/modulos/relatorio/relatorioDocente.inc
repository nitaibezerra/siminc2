<?php
if ( $_POST['pesquisa'] || $_GET['pesquisa'] ){
	ini_set("memory_limit","250M");
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
	$obRelatorio->setBrasao($true ? true : false);
	$obRelatorio->setOverFlowTableHeight(500);
	$obRelatorio->setTotNivel(true);
	$obRelatorio->setEspandir(true);
	$obRelatorio->setTolizadorLinha(true);
	
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
	
	// Instituições
	if($entid[0]){
		array_push($where, " e.entid in ('" . implode( "','", $entid ) . "') ");
	}
	// Campus
	if($campus[0]){
		array_push($where, " s.campus in ('" . implode( "','", $campus ) . "') ");
	}
	// Tipo
	if($tipo){
		array_push($where, " serplanejado = '$tipo'");
	}
	// Anoinicio
	if($anoinicio){
		array_push($where, " serano >= '$anoinicio'");
	}
	// Anofim
	if($anofim){
		array_push($where, " serano <= '$anofim'");
	}
	
	//ver($arAgrupador,d);
	$existeAgrupadorTipo = false;
	$stCamposQuery = "";
	foreach ($agrupador as $agrup){
		if($agrup == 'campus'){
			$stCamposQuery .= "entcampus.entnome as campus, ";
		}
		if($agrup == 'instituicao'){
			$stCamposQuery .= "entinst.entnome as instituicao, ";
		}
	}
	
	if(!$_POST['tipo']){
		$stCampoTipoQuery1 .= "case when serplanejado = 'P' then 'Planejado'
						       when serplanejado = 'R' then 'Realizado'
						  end as tipo,";
		$stOrderTipo = 'tipo, ';
	} else {
		$stCampoTipoQuery2 = ",case when serplanejado = 'P' then 'Planejado'
							       when serplanejado = 'R' then 'Realizado'
							  end as tipo";
	}
	
	$sql = "select 
				$stCamposQuery
				sersemestre || '°/' || serano as semestre,
				$stCampoTipoQuery1
				serqtdde,
				serqtde20,
				serqtde40
				$stCampoTipoQuery2
			from academico.servidores s
				inner join entidade.entidade entcampus on s.entidcampus = entcampus.entid
				inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A'
				inner join entidade.funentassoc a on a.fueid = fn.fueid
				inner join entidade.entidade entinst on entinst.entid = a.entid 
				inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 11 and entinst.entstatus = 'A'
				" . ( is_array($where) && count($where) ? ' where' . implode(' AND ', $where) : '' ) ."
			ORDER BY " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' ) .", sersemestre, ".$stOrderTipo." serano
			";
// 	ver($sql,d);
	return $sql;
}

function monta_sql(){
	
	$where = array();
	
	extract($_POST);
	
	// Instituições
	if($entid[0]){
		array_push($where, " entinst.entid in ('" . implode( "','", $entid ) . "') ");
	}
	// Campus
	if($campus[0]){
		array_push($where, " s.entidcampus in ('" . implode( "','", $campus ) . "') ");
	}
	// Tipo
	if($tipo){
		array_push($where, " serplanejado = '$tipo'");
	}
	// Anoinicio
	if($anoinicio){
		array_push($where, " serano >= '$anoinicio'");
	}
	// Anofim
	if($anofim){
		array_push($where, " serano <= '$anofim'");
	}
	
	if(!$_POST['tipo']){
		$stOrderTipo = 'tipo, ';
	}
	
	$sql = "select 
				entcampus.entnome as campus,
				entinst.entnome as instituicao, 
				sersemestre || '°/' || serano as semestre,
				serqtdde,
				serqtde20,
				serqtde40,
				case when serplanejado = 'P' then 'Planejado'
				     when serplanejado = 'R' then 'Realizado'
				end as tipo
			from academico.servidores s
				inner join entidade.entidade entcampus on s.entidcampus = entcampus.entid
				inner join entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 17 and entcampus.entstatus = 'A'
				inner join entidade.funentassoc a on a.fueid = fn.fueid
				inner join entidade.entidade entinst on entinst.entid = a.entid 
				inner join entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 11 and entinst.entstatus = 'A'
				" . ( is_array($where) && count($where) ? ' where' . implode(' AND ', $where) : '' ) ."
			ORDER BY " . ( is_array($agrupador) && count($agrupador) ? implode(', ', $agrupador) : '' ) .", sersemestre, ".$stOrderTipo." serano
			";
// 	ver($sql,d);
	return $sql;
}

function monta_agp(){

	$agrupador = (array) $_POST['agrupador'];
	
	$agp = array(
				 "agrupador" 	  => array(),
				 "agrupadoColuna" => array(
											"semestre",
											"serqtdde",
											"serqtde20",
											"serqtde40",
											"tipo",
										  ) 	
				);
	
	foreach( $agrupador as $val ):
		switch ($val) {
		    case 'instituicao':
				array_push($agp['agrupador'], array(
													"campo" => "instituicao",
											  		"label" => "Instituição"
												   )										
						  );				
		    continue;
		    break;
		    case 'campus':
				array_push($agp['agrupador'], array(
													"campo" => "campus",
											  		"label" => "Campus"
												   )										
						  );				
		    continue;
		    break;
		}       
	endforeach;
	
	array_push($agp['agrupador'], array(
										"campo" => "semestre",
								  		"label" => "Semestre"
									   )										
			  );
	if(!$_POST['tipo']){
		array_push($agp['agrupador'], array(
											"campo" => "tipo",
									  		"label" => "Tipo"
										   )										
				  );
		
	}
			  
	
	return $agp;
}

function monta_coluna(){

	
	if($_POST['tipo']){
		$coluna = array(
				array(
					  "campo" => "serqtdde",
			   		  "label" => "Novos Docentes DE",
			   		  "type"  => "string"
				),
				array(
					  "campo" => "serqtde20",
			   		  "label" => "Novos Docentes 20 H",
			   		  "type"  => "string"
				),
				array(
					  "campo" => "serqtde40",
			   		  "label" => "Novos Docentes 40 H",
			   		  "type"  => "string"
				),
				array(
					  "campo" => "tipo",
			   		  "label" => "Tipo",
			   		  "type"  => "string"
				)

			  );
	} else {
		$coluna = array(
					array(
						  "campo" => "serqtdde",
				   		  "label" => "Novos Docentes DE",
				   		  "type"  => "string"
					),
					array(
						  "campo" => "serqtde20",
				   		  "label" => "Novos Docentes 20 H",
				   		  "type"  => "string"
					),
					array(
						  "campo" => "serqtde40",
				   		  "label" => "Novos Docentes 40 H",
				   		  "type"  => "string"
					)	
				  );
		
	}
	
	return $coluna;				  
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";

$titulo_modulo = "Relatório de Docentes";
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
				$origem = array('entnome' => 
									array(
											'codigo'    => 'instituicao',
											'descricao' => 'Instituição'
											),
								 'campnome' => 
									array(
											'codigo'    => 'campus',
											'descricao' => 'Campus'
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
    <tr>
        <td class="SubTituloDireita" valign="top">Período</td>
        <td>
        	Ano Inicial: <?=campo_texto( 'anoinicio', '', 'S', '', 5, 4, '', '', 'left', '', 0, 'id="anoinicio"','' ); ?>
        	Ano Final:  <?=campo_texto( 'anofim', '', 'S', '', 5, 4, '', '', 'left', '', 0, 'id="anofim"','' ); ?>
		</td>
   	</tr>
   	<?php
		$arTipo = array(array("codigo" => "P", "descricao" => "Planejado"),
						array("codigo" => "R", "descricao" => "Realizado"));
	?>
	<tr>
    	<td align='right' class="SubTituloDireita">Tipo:</td>
    	<td><?=$db->monta_combo('tipo',$arTipo,'S','Selecione o Tipo',"",'','','','N','','',$tipo);?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Visualizar" onclick="geraPopRelatorio('html');" style="cursor: pointer;"/>
			<input type="button" value="Visualizar XLS" onclick="geraPopRelatorio('xls');" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>