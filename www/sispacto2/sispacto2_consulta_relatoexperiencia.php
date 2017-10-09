<?
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/workflow.php";

error_reporting(1);

include "_constantes.php";
include "_funcoes.php";
include "_funcoes_professoralfabetizador.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']) {
	$_SESSION['usucpforigem'] = '';
	$_SESSION['usucpf'] = '';
}

$_SESSION['sisid'] = SIS_SISPACTO;

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


if($_REQUEST['requisicao']=='verRelatoExperiencia') {
	
	?>
	<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script language="JavaScript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	</head>
	<body topmargin="0" leftmargin="0">
	<?
	$iusnome = $db->pegaUm("SELECT iusnome FROM sispacto2.identificacaousuario WHERE iusd='".$_REQUEST['iusd']."'");
	echo '<p style="font-size:large" align=center>'.$iusnome.'</p>';
	$es = estruturaRelatoExperiencia(array());
	
	$titulo 		 = 'Relato de experiência';
	$perguntainicial = 'Você tem certeza de que a experiência contribui para a aquisição da proficiência na escrita dos estudantes?';
		
	$relatoexperiencia = $db->pegaLinha("SELECT * FROM sispacto2.relatoexperiencia WHERE iusd='".$_REQUEST['iusd']."'");
	if($relatoexperiencia) extract($relatoexperiencia);
	
	$consulta_q = true;
		
	include_once APPRAIZ_SISPACTO."/professoralfabetizador/montarQuestionario.inc";
	
	?>
	</body>
	</html>
	<?
	exit;

}


?>
<html>
<head>
	<title><?php echo NOME_SISTEMA; ?></title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
	<script>
	function acessarRelato(iusd) {
			window.open(window.location+'?requisicao=verRelatoExperiencia&iusd='+iusd,'imagem','width=800,height=600,resizable=yes,scrollbars=yes');
	}
	
	function consultarRelatoExperiencia() {

		var f_areatematica = jQuery("[name^='reeareatematica[]']:checked:enabled").length;
		var f_turma 	   = jQuery("[name^='reeturma[]']:checked:enabled").length;
		var f_tempoduracao = jQuery("[name^='reetempoduracao[]']:checked:enabled").length;
		var f_organizacao  = jQuery("[name^='reeorganizacao[]']:checked:enabled").length;
		var f_objetivo     = jQuery("[name^='reeobjetivo[]']:checked:enabled").length;
		var f_tecnicas     = jQuery("[name^='reetecnicas[]']:checked:enabled").length;

		if(f_areatematica==0) {
			alert('Selecione o filtro: Área temática');
			return false;
		}

		if(f_turma==0) {
			alert('Selecione o filtro: Turma');
			return false;
		}

		if(f_tempoduracao==0) {
			alert('Selecione o filtro: Tempo de duração da experiência');
			return false;
		}

		if(f_organizacao==0) {
			alert('Selecione o filtro: Organização');
			return false;
		}

		if(f_objetivo==0) {
			alert('Selecione o filtro: Objetivo');
			return false;
		}

		if(f_tecnicas==0) {
			alert('Selecione o filtro: Técnicas utilizadas');
			return false;
		}
		
		jQuery('#consultar').attr('disabled','disabled');
		document.getElementById('formulario').submit();
	
	}

	</script>
</head>
<body topmargin="0" leftmargin="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td width="25%"><img src="/includes/layout/azul/img/logo.png" border="0" /></td>
	<td valign="middle" style="font-size:15px;"><b>Relato de experiências dos professores alfabetizadores - SISPACTO 2014</b></td>
</tr>
</table>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="requisicao" value="consultarRelatoExperiencia">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
<tr>
	<td class="SubTituloDireita" width="25%" valign="top">Área temática:</td>
	<td valign="top"><input type="checkbox" name="reeareatematica[]" value="P" <?=(($_POST['reeareatematica'])?((in_array('P',$_POST['reeareatematica']))?'checked':''):'') ?>> Língua Portuguesa<br>
		<input type="checkbox" name="reeareatematica[]" value="M" <?=(($_POST['reeareatematica'])?((in_array('M',$_POST['reeareatematica']))?'checked':''):'') ?>> Matemática</td>
	<td class="SubTituloDireita" width="25%" valign="top">Turma:</td>
	<td valign="top"><input type="checkbox" name="reeturma[]" value="1" <?=(($_POST['reeturma'])?((in_array('1',$_POST['reeturma']))?'checked':''):'') ?>> 1º ano<br>
		<input type="checkbox" name="reeturma[]" value="2" <?=(($_POST['reeturma'])?((in_array('2',$_POST['reeturma']))?'checked':''):'') ?>> 2º ano/ 1ª série<br>
		<input type="checkbox" name="reeturma[]" value="3" <?=(($_POST['reeturma'])?((in_array('3',$_POST['reeturma']))?'checked':''):'') ?>> 2º ano/ 1ª série<br>
		<input type="checkbox" name="reeturma[]" value="4" <?=(($_POST['reeturma'])?((in_array('4',$_POST['reeturma']))?'checked':''):'') ?>> 3º ano/ 2ª série<br>
		<input type="checkbox" name="reeturma[]" value="5" <?=(($_POST['reeturma'])?((in_array('5',$_POST['reeturma']))?'checked':''):'') ?>> 3ª série<br>
		<input type="checkbox" name="reeturma[]" value="6" <?=(($_POST['reeturma'])?((in_array('6',$_POST['reeturma']))?'checked':''):'') ?>> Multisseriada<br>
		</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Tempo de duração da experiência:</td>
	<td valign="top"><input type="checkbox" name="reetempoduracao[]" value="1" <?=(($_POST['reetempoduracao'])?((in_array('1',$_POST['reetempoduracao']))?'checked':''):'') ?>> Menos de 20 minutos<br>
		<input type="checkbox" name="reetempoduracao[]" value="2" <?=(($_POST['reetempoduracao'])?((in_array('2',$_POST['reetempoduracao']))?'checked':''):'') ?>> Entre 20 e 40 minutos<br>
		<input type="checkbox" name="reetempoduracao[]" value="3" <?=(($_POST['reetempoduracao'])?((in_array('3',$_POST['reetempoduracao']))?'checked':''):'') ?>> Mais de 40 minutos
		</td>
	<td class="SubTituloDireita" valign="top">Organização:</td>
	<td valign="top"><input type="checkbox" name="reeorganizacao[]" value="1" <?=(($_POST['reeorganizacao'])?((in_array('1',$_POST['reeorganizacao']))?'checked':''):'') ?>> Individual<br>
		<input type="checkbox" name="reeorganizacao[]" value="2" <?=(($_POST['reeorganizacao'])?((in_array('2',$_POST['reeorganizacao']))?'checked':''):'') ?>> 2 pessoas<br>
		<input type="checkbox" name="reeorganizacao[]" value="3" <?=(($_POST['reeorganizacao'])?((in_array('3',$_POST['reeorganizacao']))?'checked':''):'') ?>> 3 pessoas<br>
		<input type="checkbox" name="reeorganizacao[]" value="4" <?=(($_POST['reeorganizacao'])?((in_array('4',$_POST['reeorganizacao']))?'checked':''):'') ?>> Mais de 3 pessoas<br>
		</td>
</tr>
<tr>
	<td class="SubTituloDireita" valign="top">Objetivo principal da experiência:</td>
	<td valign="top"><input type="checkbox" name="reeobjetivo[]" value="1" <?=(($_POST['reeobjetivo'])?((in_array('1',$_POST['reeobjetivo']))?'checked':''):'') ?>> Apropriar-se do Sistema de Escrita Alfabética (SEA)<br>
					 <input type="checkbox" name="reeobjetivo[]" value="2" <?=(($_POST['reeobjetivo'])?((in_array('2',$_POST['reeobjetivo']))?'checked':''):'') ?>> Reconhecer a função social de um texto<br>
					 <input type="checkbox" name="reeobjetivo[]" value="3" <?=(($_POST['reeobjetivo'])?((in_array('3',$_POST['reeobjetivo']))?'checked':''):'') ?>> Identificar e utilizar diferentes suportes textuais<br>
					 <input type="checkbox" name="reeobjetivo[]" value="4" <?=(($_POST['reeobjetivo'])?((in_array('4',$_POST['reeobjetivo']))?'checked':''):'') ?>> Produzir textos utilizando diversos gêneros<br>
					 <input type="checkbox" name="reeobjetivo[]" value="5" <?=(($_POST['reeobjetivo'])?((in_array('5',$_POST['reeobjetivo']))?'checked':''):'') ?>> Conhecer e fazer uso da norma padrão na escrita de textos<br>
					 <input type="checkbox" name="reeobjetivo[]" value="6" <?=(($_POST['reeobjetivo'])?((in_array('6',$_POST['reeobjetivo']))?'checked':''):'') ?>> Outro objetivo
		</td>
	<td class="SubTituloDireita" valign="top">Técnicas utilizadas:</td>
	<td valign="top"><input type="checkbox" name="reetecnicas[]" value="1" <?=(($_POST['reetecnicas'])?((in_array('1',$_POST['reetecnicas']))?'checked':''):'') ?>> Brincadeira<br>
					 <input type="checkbox" name="reetecnicas[]" value="2" <?=(($_POST['reetecnicas'])?((in_array('2',$_POST['reetecnicas']))?'checked':''):'') ?>> Jogo<br>
					 <input type="checkbox" name="reetecnicas[]" value="3" <?=(($_POST['reetecnicas'])?((in_array('3',$_POST['reetecnicas']))?'checked':''):'') ?>> Dramatização<br>
					 <input type="checkbox" name="reetecnicas[]" value="4" <?=(($_POST['reetecnicas'])?((in_array('4',$_POST['reetecnicas']))?'checked':''):'') ?>> Exposição dialogada<br>
					 <input type="checkbox" name="reetecnicas[]" value="5" <?=(($_POST['reetecnicas'])?((in_array('5',$_POST['reetecnicas']))?'checked':''):'') ?>> Exercício escrito<br>
					 <input type="checkbox" name="reetecnicas[]" value="6" <?=(($_POST['reetecnicas'])?((in_array('6',$_POST['reetecnicas']))?'checked':''):'') ?>> Leitura em voz alta<br>
					 <input type="checkbox" name="reetecnicas[]" value="7" <?=(($_POST['reetecnicas'])?((in_array('7',$_POST['reetecnicas']))?'checked':''):'') ?>> Recorte e colagem<br>
					 <input type="checkbox" name="reetecnicas[]" value="8" <?=(($_POST['reetecnicas'])?((in_array('8',$_POST['reetecnicas']))?'checked':''):'') ?>> Outra técnica<br>
					 
		</td>
	
</tr>
<tr>
	<td class="SubTituloCentro" colspan="4"><input type="button" name="consultar" id="consultar" value="Consultar" onclick="consultarRelatoExperiencia();"></td>
</tr>
</table>
</form>

<?php

if($_POST['requisicao']=='consultarRelatoExperiencia') :


if($_POST['reeareatematica']) {
	unset($or);
	foreach($_POST['reeareatematica'] as $reeareatematica) {
		$or[] = "reeareatematica='".$reeareatematica."'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}

if($_POST['reeturma']) {
	unset($or);
	foreach($_POST['reeturma'] as $reeturma) {
		$or[] = "reeturma ilike '%".$reeturma."%'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}

if($_POST['reetempoduracao']) {
	unset($or);
	foreach($_POST['reetempoduracao'] as $reetempoduracao) {
		$or[] = "reetempoduracao ilike '%".$reetempoduracao."%'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}

if($_POST['reeorganizacao']) {
	unset($or);
	foreach($_POST['reeorganizacao'] as $reeorganizacao) {
		$or[] = "reeorganizacao ilike '%".$reeorganizacao."%'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}

if($_POST['reeobjetivo']) {
	unset($or);
	foreach($_POST['reeobjetivo'] as $reeobjetivo) {
		$or[] = "reeobjetivo ilike '%".$reeobjetivo."%'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}

if($_POST['reetecnicas']) {
	unset($or);
	foreach($_POST['reetecnicas'] as $reetecnicas) {
		$or[] = "reetecnicas ilike '%".$reetecnicas."%'";
	}
	$wh[] = "(".implode(" OR ", $or).")";
}


$sql = "SELECT '<img src=../imagens/consultar.gif style=cursor:pointer; onclick=acessarRelato('||i.iusd||');>' as acao, i.iusnome, m.estuf, m.mundescricao, CASE WHEN reeareatematica='P' THEN 'Português' WHEN reeareatematica='M' THEN 'Matemática' END as tema, r.reetitulo FROM sispacto2.relatoexperiencia r 
		INNER JOIN sispacto2.identificacaousuario i ON i.iusd = r.iusd 
		INNER JOIN territorios.municipio m ON m.muncod = i.muncodatuacao 
		WHERE (correcao1+correcao2+correcao3+correcao4+correcao5) >= 5 ".(($wh)?"AND ".implode(" AND ",$wh):"")." ORDER BY i.iusnome";

$cabecalho = array("&nbsp;","Professor Alfabetizador","UF","Município","Área temática","Titulo da experiência");
$db->monta_lista_simples($sql,$cabecalho,100000,5,'N','95%','center',true, false, false, true);
	
endif;
?>
</body>
</html>