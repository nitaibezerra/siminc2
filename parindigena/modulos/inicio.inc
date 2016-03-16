<?

if( $_REQUEST["tipo"] == "carrega_obras" ){
	
	$dados = explode("_", $_GET['val']);
			
	$sql = "SELECT
				'<center><img src=\"../imagens/consultar.gif\" onclick=\"verObra( ' || obrid || ' );\" style=\"cursor:pointer;\"/></center>' as acao,
				obrdesc,
				upper(entnome) as entnome,
				CASE WHEN obrdtinicio is not null THEN to_char(obrdtinicio, 'DD/MM/YYYY') ELSE 'Não Informado' END as inicio,
				CASE WHEN obrdttermino is not null THEN to_char(obrdttermino, 'DD/MM/YYYY') ELSE 'Não Informado' END as termino,
				stodesc,
				(SELECT replace(coalesce(round(SUM(icopercexecutado), 2), '0') || ' %', '.', ',') as total FROM obras.itenscomposicaoobra WHERE obrid = oi.obrid) as percentual,
				CASE WHEN oi.obrdtvistoria is not null THEN to_char(oi.obrdtvistoria, 'DD/MM/YYYY') ELSE to_char(oi.obsdtinclusao, 'DD/MM/YYYY') END as ultimadata
			FROM
				obras.obrainfraestrutura oi
			INNER JOIN
				entidade.endereco ed ON ed.endid = oi.endid
			INNER JOIN
				obras.situacaoobra so ON so.stoid = oi.stoid
			INNER JOIN
				entidade.entidade ee ON ee.entid = oi.entidunidade
			WHERE
				estuf = '{$dados[0]}' AND
				cloid = 4 AND
				obsstatus = 'A'";
	
	$cabecalho = array( "Ação", "Nome da Obra", "Unidade Implantadora", "Início de Execução", "Término de Execução", "Situação da Obra", "% Executado da Obra", "Última Atualização" );
	$db->monta_lista_simples( $sql, $cabecalho, 100, 30, 'N', '95%');
	
	die;
			
}

if ($_REQUEST['excluirFi'])
{    
    //exclusao lógica, nao sendo feita uma verificaçao de integridade.
    $sql = "UPDATE 
                 parindigena.itemmonitoramento 
            SET
                 itmstatus = 'I'
            WHERE
                 friid = '{$_REQUEST['excluirFi']}'";
    
    $db->executar($sql);
    $db->commit();
    header('Location: parindigena.php?modulo=inicio&acao=C');
}
if ($_REQUEST['excluirFc'])
{    
    //exclusao lógica, nao sendo feita uma verificaçao de integridade.
    $sql = "UPDATE 
                 parindigena.itemmonitoramento 
            SET
                 itmstatus = 'I'
            WHERE
                 frcid = '{$_REQUEST['excluirFc']}'";
    
    $db->executar($sql);       
    $db->commit();
    header('Location: parindigena.php?modulo=inicio&acao=C');    
}
if ($_REQUEST['excluirMd'])
{    
    //exclusao lógica, nao sendo feita uma verificaçao de integridade.
    $sql = "UPDATE 
                 parindigena.itemmonitoramento 
            SET
                 itmstatus = 'I'
            WHERE
                 madid = '{$_REQUEST['excluirMd']}'";
    
    $db->executar($sql);  
    $db->commit();
    header('Location: parindigena.php?modulo=inicio&acao=C');
}

if ($_REQUEST['excluirOb'])
{    

	$obrid = $db->pegaLinha("SELECT 
								obrid 
							FROM
								parindigena.itemmonitoramento 
							WHERE itmid = ".$_REQUEST['excluirOb']);

    //exclusao lógica, nao sendo feita uma verificaçao de integridade.
    $sql = "UPDATE 
                 parindigena.itemmonitoramento 
            SET
                 itmstatus = 'I'
            WHERE
                 itmid = '{$_REQUEST['excluirOb']}'";
    
    $db->executar($sql);
    
    
    $sql = sprintf("DELETE 
    					FROM parindigena.territorioobrainfraestrutura 
    				WHERE 
    					obrid = %d",
    				$obrid);
    $db->executar($sql);
    
    $sql = sprintf("DELETE 
    					FROM parindigena.povoobrainfraestrutura
    				WHERE 
    					obrid = %d",
    				$obrid);
    $db->executar($sql);
    
    $sql = sprintf("DELETE 
    					FROM parindigena.linguaobrainfraestrutura
    				WHERE 
    					obrid = %d",
    				$obrid);
    $db->executar($sql);    
    
    $db->commit();
    header('Location: parindigena.php?modulo=inicio&acao=C');    
}


include APPRAIZ . 'includes/cabecalho.inc';
print '<br />';

/*
echo("<pre>");
print_r( $_SESSION );
echo("</pre>");
die();
*/ 

$titulo_modulo = "Monitoramento do PAR Indígena";
monta_titulo( $titulo_modulo, 'Lista de Estados' );

?>
<script src="../includes/prototype.js" type="text/javascript"></script>
<script src="./js/parindigena.js" type="text/javascript"></script>
<script src="./js/ajax.js" type="text/javascript"></script>

<!-- (INÍCIO) BIBLIOTECAS - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<script	language="javascript" type="text/javascript" src="../includes/blendtrans.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/_start.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/slider/slider.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/tags/superTitle.js"></script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/JsLibrary/slider/slider.css" type="text/css" rel="stylesheet"></link>
<!-- (FIM) BIBLIOTECAS -->

<!-- (INÍCIO) BLOCOS INTERNOS DE HTML - PARA USO DOS COMPONENTES (CALENDÁRIO E SLIDER) -->
<div id="sliderDiv"	style="z-index: 1000; width: 200px; *height: 97px; left: 2px; top: 2px; visibility: visible; display: none;">
<div class="monthYearPicker" style="left: 167px; width: 35px;" id="minuteDropDown"></div>
<div class="topBar" id="topBar" style="top: 150px;"><img
	onclick="removeSlider()"
	onmouseover="this.src = '../includes/JsLibrary/slider/images/close_over.gif'"
	onmouseout="this.src = '../includes/JsLibrary/slider/images/close.gif'"
	src="../includes/JsLibrary/slider/images/close.gif"
	style="position: absolute; right: 2px;" /></div>
<div>
<table cellspacing="1" width="100%">
	<tr>
		<td style="background-color: #E2EBED">Situação:</td>
		<td style="text-align:left"><select onchange="alteraStatus(this)"
			id="situacaoSlider"
			style="border:1px; border-style:solid; border-color:black; width:100%; font-size:10px;">
			<option value='1'>Não Iniciado</option>
			<option value='2'>Em Andamento</option>
			<option value='3'>Suspenso</option>
			<option value='4'>Cancelado</option>
			<option value='5'>Concluído</option>
		</select></td>
	</tr>
	<tr>
		<td style="background-color: #E2EBED">Andamento:</td>
		<td><span id="slider_target"
			style="width: 40px; position: relative; *left: -41px; top: 5px; height: 3px;"></span>
		&nbsp; <input type="text" name="valor" id="valorSlider" value="200"
			style="font-size: 10px; width: 30px" readonly />%</td>
		<script>
			form_widget_amount_slider('slider_target',document.getElementById( "valorSlider" ),70,0,100,"arredonda(document.getElementById( 'valorSlider' ))");
		</script>
	</tr>
	<tr>
		<td colspan="2" style="background-color: #E2EBED"><input type="button"
			onclick="slicerSubmit()" value="Ok"
			style="height:18px; font-size: 10px; " /></td>
	</tr>
</table>
</div>
</div>

<input type="hidden" id="inputGeral" value="" readonly="readonly" onchange="desmontaCalendario(this)" />
<!-- (FIM) BLOCOS INTERNOS DE HTML -->
<?php
montarArvore();

if ($_REQUEST['estuf']){
	echo "<script type=\"text/javascript\">
			<!--
			alteraIcone('{$_REQUEST['estuf']}', 1);
			//-->
		  </script>";
	
}
?>