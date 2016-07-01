<?php

/************ CARREGA OBJETOS E VARIAVEIS ****************/
//$db 	= new cls_banco();
$inuid 	= $_SESSION['inuid'];

require_once APPRAIZ . 'cte/classes/ParecerPar.class.inc';
$obParecer = new ParecerPar();
$parid = $_REQUEST['parid'];

if ($_REQUEST['acao']     == 'A'){
    $tppid = 1;
    $tituloAtual = "Financeiro";
}elseif ($_REQUEST['acao'] == 'B'){
    $tppid = 2;
    $tituloAtual = "de Engenharia";
}elseif ($_REQUEST['acao'] == 'C'){
    $tppid = 3;
}

if( $tppid && $inuid  ){ // se o monitoramento do item já existe carrega dados.
	$dadosParecer = $obParecer->recuperaPorID($parid);
	if($dadosParecer){
		$dadosParecer 					= current($dadosParecer);
		$partexto 						= $dadosParecer['partexto'];
		$pardata						= $dadosParecer['pardata'];
	}
}



/************ MONTA TITULO (CABEÇALHO) ****************/
cte_montaTitulo( $titulo_modulo, 'Históricos de parecer '.$tituloAtual ); 
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
</head>
<body>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td width="25%" align='right' class="SubTituloDireita">Tipo de parecer:</td>
        <td><?php echo $titulo_modulo;?></td>
    </tr>
    <tr>
       	<td align='right' class="SubTituloDireita">Data:</td>
        <td><?php echo campo_texto('pardata', "N", "N", "", 30, 50, "", "", '', '', '', 'id="pardata"', '', $pardata );?></td>
    </tr>
    <tr>
       	<td align='right' class="SubTituloDireita">Parecer:</td>
        <td><?php echo campo_textarea('partexto', 'N', 'N', 'Parecer', '100', '10', null ,'', 0, '', false, 'Parecer');?></td>
    </tr>
</table>
</body>
</html>