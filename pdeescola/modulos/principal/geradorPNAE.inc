<?php
ini_set("memory_limit","2250M");
set_time_limit(0);

if($_REQUEST['gerarXML']){
if($_REQUEST['ano']){
	
if($_REQUEST['tipoAno'] == '1'){
	$tipoAno = "AND mem.entcodent not in (SELECT  mem.entcodent FROM pdeescola.memaiseducacao mem WHERE mem.memanoreferencia = ".($_REQUEST['ano']-1)." AND mem.memstatus = 'A' )"; 
}
else if($_REQUEST['tipoAno'] == '2'){
	$tipoAno = "AND mem.entcodent in (SELECT  mem.entcodent FROM pdeescola.memaiseducacao mem WHERE mem.memanoreferencia = ".($_REQUEST['ano']-1)." AND mem.memstatus = 'A' )"; 
}

$sql = "SELECT  
			mem.entcodent as co_escola,
			'1' as co_modalidade,
			'20' as co_nivel_ensino, 
			sum(map.mapquantidade) as qt_qluno
		FROM 
			pdeescola.memaiseducacao mem
		INNER JOIN pdeescola.mealunoparticipante map ON map.memid = mem.memid AND map.mapano = ".$_REQUEST['ano']."
		WHERE 
			mem.memanoreferencia = ".$_REQUEST['ano']." 
			AND mem.memstatus = 'A' 
			".$tipoAno."
		GROUP BY mem.entcodent
		ORDER BY mem.entcodent";

$dados = $db->carregar($sql);
if($dados){
$nu_arquivo 		= 1;
$dt_criacao_arquivo = date("d/m/Y");
$qt_registro 		= count($dados);
$an_exercicio 		= $_REQUEST['ano'];

$XML = '<?xml version="1.0"?>
<cabecalho>
    <nu_arquivo>'.$nu_arquivo.'</nu_arquivo>
    <dt_criacao_arquivo>'.$dt_criacao_arquivo.'</dt_criacao_arquivo>
    <qt_registro>'.$qt_registro.'</qt_registro>
    <an_exercicio>'.$an_exercicio.'</an_exercicio>   
<registros>';
	  	foreach($dados as $chave=>$dado){
	  		$XML .= '<registro>
		    	<co_escola>'.$dado['co_escola'].'</co_escola>
		    	<co_modalidade>'.$dado['co_modalidade'].'</co_modalidade>
			    <co_nivel_ensino>'.$dado['co_nivel_ensino'].'</co_nivel_ensino>
		    	<qt_qluno>'.$dado['qt_qluno'].'</qt_qluno>    
			</registro>';
	  	}
$XML .= '</registros>    
</cabecalho>';

$arqXml = <<<XML
$XML
XML;

	if($_REQUEST['gerarXML'] == 1){
		ob_get_clean();
		header( "Content-Type: text/xml" );
		header( 'Content-Disposition: attachment; filename=pnae.xml');
		echo $arqXml;
		exit();
		
	}
}else{
	alert('Não existe escolas.');
}
}else{
	alert('Selecione um Ano.');
}
}
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
?>
<html>
	<head>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
	<body>
		<form method="post" name="formulario" id="formulario">
			<input type="hidden" name="gerarXML" id="gerarXML" value="0"/>
			<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
				<tr>
					<td colspan="2" class="SubtituloCentro">Gerador PNAE</td>
				</tr>
				<tr>
					<td colspan="2" class="SubtituloCentro" style="font-size:10px; ">Selecione um ano para gerar o arquivo.</td>
				</tr>
				<tr>
					<td width="25%" class="SubtituloDireita">
						Ano:
					</td>
					<td>
						<select  name='ano' id="ano">
							<option value="" >Selecione</option>
							<option value="2011">2011</option>
							<option value="2012">2012</option>
							<option value="2013">2013</option>
							<option value="2014">2014</option>
						</select>
						<input type="radio" name="tipoAno" value="1" checked="checked"> Ano Atual
						&nbsp;&nbsp;&nbsp;&nbsp;
						<input type="radio" name="tipoAno" value="2"> Ano Anterior
					</td>
				</tr>
				<tr>
					<td class="SubtituloDireita">
					</td>
					<td class="SubtituloEsquerda"> 
						<input type="button" name="Gerar" value="Gerar" onclick="gerarArquivo();" /> 
					</td>
				</tr>
			</table>
		</form>
	</body>
</html>
<script type="text/javascript">
	function gerarArquivo(){
		document.getElementById('gerarXML').value = 1;
		document.getElementById('formulario').submit();
	}
</script>
