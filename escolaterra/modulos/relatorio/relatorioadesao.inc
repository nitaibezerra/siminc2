<?php


function monta_sql(){
	global $db;
	extract($_REQUEST);

	$where = array();
		
	
	// estado
	if( $estuf[0] && $estuf_campo_flag != '' ){
		array_push($where, "foo.estuf " . (!$estuf_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "')");
	}
	
	// municipio
	if( $muncod[0] && $muncod_campo_flag != '' ){
		array_push($where, "foo.muncod " . (!$muncod_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $muncod ) . "')");
	}
	
	if($escolasprioritarias) {
		$inner_municipiosprioritarios = "INNER JOIN escolaterra.municipiosprioritarios mp ON mp.muncod = foo.muncod";
	}
	
	if($escolasquilombolas=='S') {
		array_push($where, " foo.quilombola='Sim' ");
	}
	
	
	if($esfera && $esfera!='T') {
		array_push($where, " foo.esfera='".$esfera."' ");
	}
	
	
	$sql = "SELECT foo.* FROM (
			SELECT
				CASE WHEN i.estuf  IS NOT NULL THEN m2.muncod 
				     WHEN i.muncod IS NOT NULL THEN m.muncod 
				     ELSE 'Muncod não identificado' END as muncod,
				CASE WHEN i.estuf  IS NOT NULL THEN 'Estadual' 
				     WHEN i.muncod IS NOT NULL THEN 'Municipal' 
				     ELSE 'Esfera não identificada' END as esfera,
				CASE WHEN i.estuf  IS NOT NULL THEN es.estuf 
				     WHEN i.muncod IS NOT NULL THEN m.estuf 
				     ELSE 'UF não identificada' END as estuf,
				CASE WHEN i.estuf  IS NOT NULL THEN m2.mundescricao 
				     WHEN i.muncod IS NOT NULL THEN m.mundescricao 
				     ELSE 'Município não identificada' END as mundescricao,
				le.letcodigoinep,     
				le.letnome,
				le.letprofessores as professoresidentificados,
				ad.apeprofessoressolicitados as professoressolicitados,
				CASE WHEN le.letquilombolas=true THEN 'Sim' ELSE 'Não' END quilombola
			FROM par.pfadesaoprograma p 
			INNER JOIN workflow.documento d on d.docid = p.docid 
			INNER JOIN workflow.estadodocumento e on e.esdid = d.esdid
			INNER JOIN par.instrumentounidade i on i.inuid = p.inuid 
			LEFT JOIN escolaterra.adesaoprogramaescola ad on ad.adpid = p.adpid 
			LEFT JOIN escolaterra.listaescolasterra le on le.letid = ad.letid 
			LEFT JOIN territorios.municipio m on m.muncod = i.muncod 
			LEFT JOIN territorios.municipio m2 on m2.muncod = le.muncod
			LEFT JOIN territorios.estado es on es.estuf = i.estuf
			WHERE pfaid=15 AND adpresposta='S' AND d.esdid=841 
			ORDER BY 1,2,3
			) foo 
			{$inner_municipiosprioritarios}
			".(($where)?"WHERE ".implode(" AND ", $where):"");

	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												"esfera",
												"estuf",
												"mundescricao",
												"letnome",
												"professoresidentificados",
												"professoressolicitados"
											  )	  
					);
					
	array_push($agp['agrupador'], array(
									"campo" => "esfera",
									"label" => "Esfera")										
							   		);

	array_push($agp['agrupador'], array(
									"campo" => "estuf",
									"label" => "UF")										
							   		);	
							   		

	array_push($agp['agrupador'], array(
									"campo" => "mundescricao",
									"label" => "Município")										
							   		);
	
	return $agp;
}


function monta_coluna(){
	$coluna = array();
	
	array_push($coluna,array( "campo" => "professoresidentificados",
					   		  "label" => "Professores Identificados (CENSO 2012)",
							  "type"  => "numeric" ) );					
	
	
	array_push($coluna,array( "campo" => "professoressolicitados",
					   		  "label" => "Professores Solicitados",
							  "type"  => "numeric" ) );					
	

	return $coluna;			  	
}

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(600);
/* FIM configurações */


// exibe consulta
if($_REQUEST['tiporelatorio']){
	switch($_REQUEST['tiporelatorio']) {
		case 'html':
			include "relatorioadesao_resultado.inc";
			exit;
		case 'xls':
			
			$sql = monta_sql();
			
			ob_clean();
			header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
			header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
			header ( "Pragma: no-cache" );
			header ( "Content-type: application/xls; name=rel_adesaoescolaterra_".date("Ymdhis").".xls");
			header ( "Content-Disposition: attachment; filename=rel_adesaoescolaterra_".date("Ymdhis").".xls");
			header ( "Content-Description: MID Gera excel" );
			$db->monta_lista_tabulado($sql,array("Código IBGE","Esfera","UF","Município","Cod. INEP","Escola","Professores Identificados (CENSO 2012)","Professores Solicitados","Quilombolas?"),1000000,5,'N','100%','');
			exit;
			
	}
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo( "Relatório Adesão", 'Selecione os filtros' );

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">

	
function exibeRelatorioGeral(tipo){
	
	var formulario = document.formulario;
	
    selectAllOptions(document.getElementById('estuf'));
    selectAllOptions(document.getElementById('muncod'));
	
	formulario.action = 'escolaterra.php?modulo=relatorio/relatorioadesao&acao=A&tiporelatorio='+tipo;
	window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';

	
	formulario.submit();
	
}

	
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
				
/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
	}
}
	
/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
	
	
//-->
</script>


<form action="" method="post" name="formulario" id="filtro"> 

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita">Esfera:</td>
		<td>
			 <input type="radio" value="T" name="esfera" checked /> Todas <input type="radio" value="Municipal" name="esfera"/> Municipal <input type="radio" value="Estadual" name="esfera"/> Estadual
		</td>
	</tr>
<?php
$stSql = "SELECT
				estuf AS codigo,
				estuf || ' / ' || estdescricao AS descricao
			FROM 
				territorios.estado
			ORDER BY
				2 ";
mostrarComboPopup( 'UFs:', 'estuf',  $stSql, '', 'Selecione a(s) UF(s)' );

$stSql = "SELECT
				muncod AS codigo,
				estuf || ' / ' || mundescricao AS descricao
			FROM 
				territorios.municipio
			ORDER BY
				2 ";
mostrarComboPopup( 'Municípios:', 'muncod',  $stSql, '', 'Selecione o(s) Município(s)' );				

?>
	<tr>
		<td class="SubTituloDireita">Listar Municípios Prioritários:</td>
		<td>
			 <input type="checkbox" value="E" name="escolasprioritarias" /> Sim
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Listar Escolas Quilombolas:</td>
		<td>
			 <input type="checkbox" value="S" name="escolasquilombolas" /> Sim
		</td>
	</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <input type="button" value="Visualização por Município (HTML)" onclick="exibeRelatorioGeral('html');"/> 
				 <input type="button" value="Visualização por Escola(XLS)" onclick="exibeRelatorioGeral('xls');"/>
			</td>
		</tr>
	</table>

</form>