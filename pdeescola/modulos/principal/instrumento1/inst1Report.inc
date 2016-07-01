<?php
//pde_verificaSessao();
include_once APPRAIZ . 'includes/workflow.php';

/*----------------- Configuraï¿½ï¿½es de Pï¿½gina ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

//recuperando entid 
if(!$_REQUEST['entid']){
	$entid = $_SESSION['entid'];
}
if($_REQUEST['entid']){
	$entid = $_REQUEST['entid'];
	$_SESSION['entid'] = $entid; 
}

if(!isset($_SESSION['entid']) || $_SESSION['entid'] == null){
	
	alert("Este relatório deve ser acessado pelo módulo PDE Escola!");
	echo "<script>document.location.href = 'pdeescola.php?modulo=inicio&acao=C';</script>";	
	exit();
}

// Tratamento para exibir alert qdo perfil não está atribuído a nenhuma escola
if (!isset($_SESSION["entid"])){
	echo '<script>
		      alert(\'Usuário não possui uma escola atribuída.\');
		      history.go(-1);
	      </script>';
	exit();
}
// Tratamento Exercicio base de comparacao - exibir dados do ano de exercicio ate ano vigente.
if($_SESSION["exercicio"] != $_SESSION['exercicio_atual']) {
				echo "<script language=\javascript\" type=\"text/javascript\">
						alert('Nenhum dado para este ano Exercício Base de Comparação.');
						window.history.go(-1);
 					</script>";
				exit;	
}

unset($_SESSION['pdeid']);
criarDocumento( $entid );
$docid 			   = pegarDocid( $entid );
$_SESSION['pdeid'] = pegarPdeid($entid);

error_reporting(0);
ini_set('display_errors', FALSE);

$entid = $_SESSION['entid'] ? $_SESSION['entid'] : $_GET['entid'];
$docid  = pegarDocid( $_SESSION['entid'] );
$_SESSION['entid'] = $entid;


//recuperando entid do wokflow
criarDocumento( $entid );

$sql = "SELECT
			pdeid
		FROM 
			pdeescola.pdeescola
		WHERE
			entid = {$entid}
		AND 
			docid = {$docid}";
			
$pdeid = $db->pegaUm($sql);	

$_SESSION['pdeid'] = $pdeid;


//$_SESSION['entid'] = $_GET['entid'];

/***********************************************************
 * Componente que irï¿½ montar os butï¿½es do formulï¿½rio
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serï¿½o carregados pelo 
 * 										   __construct "Padrï¿½es ï¿½ classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   nï¿½o precisa dar include do arquivo na pï¿½gina.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos mï¿½todos 
 * 																		1ï¿½ parametro
 * 																		passa a funï¿½ï¿½o que serï¿½ aplicada nos botï¿½es 
 * 																		"anterior, prï¿½ximo, salvar anterior, salvar prï¿½ximo e salvar".
 * 																		2ï¿½ parametro
 * 																		serï¿½ um array de "ï¿½ndice" igual ao parametro da funcï¿½o 
 * 																		passada no 1ï¿½ parametro e o valor serï¿½ o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulï¿½rio "dentro da tabela ainda", este monta os butï¿½es.
 * 											  O parametro passado refere-se a funï¿½ï¿½o javascript que validarï¿½ o formulï¿½rio,
 * 											  devendo esta submeter o formulï¿½rio. 
 * 
 ***********************************************************/
/*
 * relatorio de todas a questões do instrumento 1.
 */
$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', '');
echo cabecalhoPDE(); 

ini_set( "memory_limit", "512M" );
set_time_limit(0);

/*----------------- Recupera dados - Dados Escola ---------------------*/
$dados = $db->pegaLinha("SELECT 	pdeuf,
								pdemunicipio, 
								pdenome,
								pdenomediretor,
								pdebairro,
								pdelogradouro,
								pdecomplemento,
								pdetelefone,
								pdeemail,
								tloid
						FROM pdeescola.pdeescola
						WHERE pdeid = ".$pdeid);

extract($dados);
if($dados['tloid'] == 1){
	$check1 = 'checked="checked"';
}else if($dados['tloid'] == 2){
	$check2 = 'checked="checked"';
}else if($dados['tloid'] == 3){
	$check3 = 'checked="checked"';
}
//busca nome escola por entid
$sql = "SELECT entnome as nome
		FROM entidade.entidade as e
		INNER JOIN entidade.funcaoentidade as fe ON fe.entid = e.entid
		WHERE funid = 3 and
		  	  tpcid IN (1,3) AND
	    	  e.entid IN ('{$entid}')";
$NomeEscola = $db->pegaum($sql);
$pdenome = $NomeEscola;

?>
<?=subCabecalho('1-6 - Dados da Escola.'); ?>	
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
  <tr>
    <td class="SubTituloDireita" >Nome da escola:</td>
    <td colspan="3"><?=campo_texto('pdenome', 'S', 'N' , '', 50, 100, '', '', 'left', '', 0, 'id="pdenome"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nome do diretor:</td>
    <td colspan="3"><?=campo_texto('pdenomediretor', 'S', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="pdenomediretor" onkeypress="return somenteLetras(event);"'); ?></td>
  </tr>
   <tr>
    <td class="SubTituloDireita" >Telefone:</td>
    <td colspan="3"><?=campo_texto('pdetelefone', 'N', 'S' , '', 50, 10, '##########', '', 'left', '', 0, 'id="pdetelefone"'); ?></td>
   </tr>
   <tr>
    <td class="SubTituloDireita" >E-mail:</td>
    <td  colspan="3" ><?=campo_texto('pdeemail', '', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="pdeemail"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Endereço da escola:</td>
    <td class="SubTituloDireita" colspan="3"></td>
  </tr>
   <tr>
    <td class="SubTituloDireita">Estado: </td>
    <td  colspan="3"><?=campo_texto('pdeuf', 'S', 'S' , '', 50, 2, '', '', 'left', '', 0, 'id="pdeuf" onkeypress="return somenteLetras(event);"'); ?></td>
   </tr>
   <tr>
    <td class="SubTituloDireita" >Município:</td>
    <td  colspan="3"><?=campo_texto('pdemunicipio', 'S', 'S' , '', 50, 80, '', '', 'left', '', 0, 'id="pdemunicipio"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Bairro:</td>
    <td colspan="3"><?=campo_texto('pdebairro', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdebairro"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Logradouro:</td>
    <td colspan="3"><?=campo_texto('pdelogradouro', 'S', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdelogradouro"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Complemento:</td>
    <td colspan="3"><?=campo_texto('pdecomplemento', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdecomplemento"'); ?></td>
  </tr>
  <tr>
     <td class="SubTituloDireita" >Localização:</td>
      <td colspan="3"><?=campo_radio('tloid','S','S' , '', 50, 255, '', '', 'left', '', 0, 'id="tloid"'); ?>
       <input id="area" type="radio" name="area" value="1" <?=$check1;?>/>Área urbana <br>
       <input id="area" type="radio" name="area" value="2" <?=$check2;?>/>Área rural <br>
       <input id="area" type="radio" name="area" value="3" <?=$check3;?>/>Área urbana periférica <br> 
  	 </td>
  </tr>			
</table>

<br>
<br>
<br>
<br>
<?php


/*----------------- Recupera dados ---------------------*/  
$dados = $db->carregar("SELECT t.tmeid as id, n.tmeid as selecionado,
								tmedescricao as descricao
						FROM pdeescola.tiponivelmodalidadeensino t
						left join pdeescola.nivelmodalidadeensino n on n.tmeid = t.tmeid and pdeid = ".$pdeid);
$total = count($dados); 
?>
<?=subCabecalho('7 - Nível e modalidade de ensino ministrados na escola.'); ?>	
<form method="post" id="formulario" name="formulario">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" name="total" value="<?=$total?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<? foreach($dados as $dados){ 
	$check = '';
	if($dados['selecionado']){
		$check = 'checked="checked"';
	}
?>
  <tr>
    <td width="28%" class="SubTituloDireita" ><?=$dados['descricao'];?></td>
    <td ><input type="checkbox" id="nivel<?=$dados['id'];?>" name="nivel<?=$dados['id'];?>" value="<?=$dados['id']; ?>" <?=$check;?> /></td>
  </tr>
<? } ?> 				
</table>
</form>

<br>
<br>

<br>
<br>

<?

/*----------------- Recupera dados ---------------------*/  	
 $sqlCombo = "select
                        td.tidid as codigo,
                        td.tiddescricao as descricao,
                        dc.tidid as selecionado,
                        dc.dcuqtd as quantidade,
                        dc.dcuqtdcondadequada as adequado,
                        dc.dcuqtdcondinadequada as inadequado,
                        dc.dcuid,
                        dc.dcudescinadequado  
                    from pdeescola.tipodependencia td
                    left join pdeescola.dependenciascondicaouso dc on dc.tidid = td.tidid and dc.pdeid = ".$pdeid."
                    order by
                        td.tiddescricao
             ";
 $dependencias = $db->carregar( $sqlCombo );
// dbg( $dependencias );
// die();
 $total = count($dependencias);	
?>
<?=subCabecalho('8 - Dependência escolares e condições de uso.'); ?>	
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" name="total" value="<?=$total?>">
<table class="tabela listagem" bgcolor="#f5f5f5" align="center">
  <tr>
    <td class="SubtituloEsquerda"  ></td>
    <td class="SubtituloEsquerda"  width="79">Depend&ecirc;ncias</td>
    <td class="SubtituloEsquerda"  width="96">Quantidade</td>
    <td class="SubtituloEsquerda"  colspan="2">Condi&ccedil;&otilde;es de utiliza&ccedil;&atilde;o </td>
    <td class="SubtituloEsquerda">O que está inadequado</td>
  </tr>
  <tr>
  <td class="SubtituloEsquerda"  ></td>
    <td class="SubtituloEsquerda"  colspan="2">&nbsp;</td>
    <td class="SubtituloEsquerda"  width="80">Adequado</td>
    <td class="SubtituloEsquerda"  width="80">Inadequado</td>
    <td class="SubtituloEsquerda"></td>
  </tr>
   <?
   		$podeeditar = 0;
      	foreach($dependencias as $dependencias){
			$ID	  				= $dependencias['codigo'];
			$Nome 				= $dependencias['descricao'];
			$marcado 			= $dependencias['selecionado'];
			$quantidade 		= $dependencias['quantidade'];
			$adequado 			= $dependencias['adequado'];
			$inadequado 		= $dependencias['inadequado'];
			$dcuid	    		= $dependencias['dcuid'];
			$dcudescinadequado 	= $dependencias['dcudescinadequado'];
			$check = '';
      		if($marcado) {
	      		$check = 'checked="checked"';	
	      		$visivel = " display:block; ";
	      		$qtdDisabled = "";
      			$condicoesDisabled = "";
      		}
      		else {
      			$visivel = " display:none;  ";
      			$qtdDisabled = "disabled=\"disabled\"";
      			$condicoesDisabled = "disabled=\"disabled\"";
      		}
   ?>
  <tr>
  	<td>
  	<input type="checkbox" id="dep<?=$ID;?>" name="dep<?=$ID;?>" title="<?= $Nome?>" value="<?=$ID;?>" onclick="criadescricao(<?=$ID;?>);" <?=$check;?> />
  	<input type="hidden" id="dcudescinadequado<?=$ID?>" name="dcudescinadequado<?=$ID?>" value="<?=$dcudescinadequado?>">
  	</td>
    <td><?=$Nome; ?></td>
    <td> <input style="width:50;" name="quantidade<?=$ID;?>" id="quantidade<?=$ID;?>" value="<?=$quantidade; ?>" type="text" <?=$qtdDisabled?> onkeypress="return somenteNumeros(event);"  onkeyup="valor_total(<?=$ID;?>);" readonly="true" class="disabled" /></td>
    <td> <input style="width:50" name="adequado<?=$ID;?>" id="adequado<?=$ID;?>" value="<?=$adequado; ?>" type="text" <?=$condicoesDisabled?> onkeypress="return somenteNumeros(event);" onkeyup="valor_total(<?=$ID;?>);" onblur="coloca_zero(<?=$ID;?>);" /> </td>
    <td> <input style="width:50" name="inadequado<?=$ID;?>" id="inadequado<?=$ID;?>" value="<?=$inadequado; ?>" type="text" <?=$condicoesDisabled?> onkeypress="return somenteNumeros(event);" onkeyup="valor_total(<?=$ID;?>);"onchange="coloca_zero(<?=$ID;?>);" /></td>
    <td>  
    <span id="descri<?=$ID;?>" style=" <?=$visivel; ?>  cursor:pointer;float:left" onclick="abrepopup( <?= $dcuid = ($dcuid == NULL) ? 0 : $dcuid;?>,<?= $ID = ($ID == NULL) ? 0 : $ID;?> );"></span>
    </td>
  </tr>
  <?
	}
  ?>
 			
</table>
</form>
<br>
<br>

<br>
<br>
<!-- inicio 9_1 -->

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formulario">

<?=subCabecalho('9.1 - Matrícula inicial (ano anterior) - Educação Infantil'); 
//matutino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			   END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid					
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(9) and titid = '1'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?> 	  	 	  	  	  	
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%"> Série / Ano / Ciclo</th>
		<th colspan="2">MATUTINO </th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	//$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Vespertino
$sql = "SELECT  DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			   END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid			
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(9) and titid = '2'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
		<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">VESPERTINO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Noturno
$sql = "SELECT DISTINCT 
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			   END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(9) and titid = '3'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">NOTURNO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
$sql = "SELECT  DISTINCT 
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			   END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid	
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(9) and titid = '4'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">INTEGRAL</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
</form>
<!-- fim 9.1 -->
<br>
<br>

<br>
<br>
<!-- inicio 10_1 -->
<?php
$pdeid = $_POST['pdeid'] ? $_POST['pdeid'] : $_SESSION['pdeid'];
?>


<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formulario">
<?=subCabecalho('10.1 - Matrícula inicial (ano anterior) - Ensino Fundamental'); 
//matutino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno,
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			   END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(2, 3) 
	--	AND 
	--		m.ppritem = 'p10_1' 
	    AND
	        pdeanoreferencia is not null
		AND 
			titid = '1'
		ORDER BY 
			scedescricao";

?> 	  	 	  	  	  	
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%"> Série / Ano / Ciclo</th>
		<th colspan="2">MATUTINO </th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	//$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Vespertino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno,
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid			
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(2, 3) 
	--	AND 
	--		m.ppritem = 'p10_1' 
		AND 
			titid = '2'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
		<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">VESPERTINO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Noturno
$sql = "SELECT DISTINCT 
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
		    END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(2, 3) 
	--	AND 
	--		m.ppritem = 'p10_1' 
		AND 
			titid = '3'
	    AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">NOTURNO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
$sql = "SELECT  DISTINCT  
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			   ELSE 0
			 END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(2, 3) 
	--	AND 
	--		m.ppritem = 'p10_1' 
	    AND 
	 		titid = '4'
	    AND
	        pdeanoreferencia is not null	 		
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="8%">Série / Ano / Ciclo</th>
		<th colspan="2">INTEGRAL</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
</form>


<!-- fim 10_1-->

<br>
<br>

<?=subCabecalho('10.2 - Aproveitamento dos alunos da 1ª - 4ª série/ 1º - 5º ano do Ensino Fundamental(ano anterior)'); ?>
<form method="post" id="p92_form_aproveitamento"
	name="p92_form_aproveitamento"><input type="hidden" name="submetido"
	value="1"> <input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<thead>
	<tr>
	  	<td colspan="11">
  		 	Ano de Referencia: 
			<?php
				$ano = $onmid ? $onmanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
					if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
					echo $ano;
			?>
			<input type="hidden" name="pjianoreferencia" value="<?=$ano?>">
		</td>
	</tr>
		<tr>
			<th>SÉRIE/ANO</th>
			<th>Matrícula Inicial</th>
			<th>Admitidos após mês de março</th>
			<th>Afastados por Abandono</th>
			<th>Afastados por Transferências</th>
			<th>Matrícula Final</th>
			<th>Aprovados</th>
			<th>Reprovados</th>
			<th>Taxa de Aprovação</th>
			<th>Taxa de Reprovação</th>
			<th>Taxa de Abandono</th>
		</tr>
	</thead>
	<tbody>
	<?php
	$serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar
								WHERE tmeid = 2 AND scetipo = 'S' ORDER BY sceid");

	$totalMatInicial = 0;
	$totalAdmitido = 0;
	$totalAbandono = 0;
	$totalTransferencia = 0;
	$totalMatFinalTela1 = 0;
	$totalAprovados = 0;
	$totalReprovados = 0;

	for($i=0; $i<count($serie); $i++) {
		//echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			
		$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	 WHERE pdeid = ".$pdeid." 
									   AND sceid = ".$serie[$i]["sceid"]."
									   AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
		
		
		for($j=0; $j<count($dados); $j++) {
			
		$div = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] );
		
			if( $div > 0 ) {
				//var taxaReprov = Number(reprovados / (matInic + qtdMarco - qtdTransf) * 100);
				//$taxa_aprovacao = str_replace('.',',',simec_number_format((($dados[$j]["apaqtdaprovados"] / $dados[$j]["apaqtdmatriculafinal"]) * 100)))."%";
				//if($dados[$j]["apaqtdaprovados"] != 0){
				$taxa_aprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdaprovados"] / ($div) * 100))))."%";
				$taxa_reprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdreprovados"] / ($div) * 100))))."%";
				//$taxa_reprovacao = str_replace('.',',',simec_number_format((($dados[$j]["apaqtdreprovados"] / $dados[$j]["apaqtdmatriculafinal"]) * 100)))."%";
				$taxa_abandono = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdafastadosabandono"] / ($div) * 100))))."%";
				//}
			}
			else {
				$taxa_aprovacao = "0%";
				$taxa_reprovacao = "0%";
				$taxa_abandono = "0%";
			}

			$totalMatInicial += $dados[$j]["apaqtdmatriculainicial"];
			$totalAdmitido += $dados[$j]["apaqtdadmitidosaposmarco"];
			$totalAbandono += $dados[$j]["apaqtdafastadosabandono"];
			$totalTransferencia += $dados[$j]["apaqtdafastadostransferencia"];
			$totalMatFinalTela1 += $dados[$j]["apaqtdmatriculafinal"];
			$totalAprovados += $dados[$j]["apaqtdaprovados"];
			$totalReprovados += $dados[$j]["apaqtdreprovados"];



			$sqlTotal = "SELECT distinct
								m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(2, 3) 
								AND m.sceid = ".$serie[$i]["sceid"]."
								order by m.sceid
								";
	
			$rsTotal = $db->carregar( $sqlTotal);

			for( $x = 0; $x <count( $rsTotal ); $x++)
			{
				$qtdTotalAluno[$serie[$i]["sceid"]] += $rsTotal[$x]['maiqtdaluno'];
			}

			if( !$qtdTotalAluno[$serie[$i]["sceid"]])
			$value = $dados[$j]["apaqtdmatriculainicial"];
			else
			$value = $qtdTotalAluno[$serie[$i]["sceid"]];


			echo "<tr height=\"30px\" align=\"center\">
					  	<td bgcolor=\"#f0f0f0\">
					  		<b>".$serie[$i]["scedescricao"]."</b>
					  		<input type=\"hidden\" name=\"serie_id[]\" value=\"".$serie[$i]["sceid"]."\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal;\"disabled;\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" value=\"$value\" onblur=\"alteraTaxa('apaqtdmatriculainicial',".$serie[$i]["sceid"].",'totalMatInicial');\" onkeypress=\"return somenteNumeros(event);\"readonly=\"true;\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" name=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdadmitidosaposmarco"]."\" onblur=\"alteraTaxa('apaqtdadmitidosaposmarco',".$serie[$i]["sceid"].",'totalAdmitido');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadosabandono"]."\" onblur=\"alteraTaxa('apaqtdafastadosabandono',".$serie[$i]["sceid"].",'totalAbandono');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadostransferencia"]."\" onblur=\"alteraTaxa('apaqtdafastadostransferencia',".$serie[$i]["sceid"].",'totalTransferencia');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  <td bgcolor=\"#f0f0f0\">
					  <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdmatriculafinal"]."\" readonly=\"true\" >
					  			
					  </td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdaprovados"]."\" onblur=\"alteraTaxa('apaqtdaprovados',".$serie[$i]["sceid"].",'totalAprovados');\"  onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdreprovados"]."\" onblur=\"alteraTaxa('apaqtdreprovados',".$serie[$i]["sceid"].",'totalReprovados');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_aprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_aprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_reprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_reprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_abandono[".$serie[$i]["sceid"]."]\">
					  		".$taxa_abandono."
					  	</td>
					  </tr>";
		}
	}
	$totalDiv = 0;
	$totalDiv = $totalMatInicial + $totalAdmitido + $totalTransferencia;
	if(($totalDiv != 0 ) && ( $totalDiv != '')) {
		$totalTaxaAprovacao = str_replace('.',',',simec_number_format((($totalAprovados / $totalDiv) * 100), 1));
		$totalTaxaReprovacao = str_replace('.',',',simec_number_format((($totalReprovados / $totalDiv) * 100), 1));
		$totaTaxaAbandono = str_replace('.',',',simec_number_format((($totalAbandono / $totalDiv) * 100), 1));
	}
	else {
		$totalTaxaAprovacao = 0;
		$totalTaxaReprovacao = 0;
		$totaTaxaAbandono = 0;
	}

	echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"totalMatInicial\" bgcolor=\"#DADADA\">".$totalMatInicial."</td>
					<td id=\"totalAdmitido\" bgcolor=\"#DADADA\">".$totalAdmitido."</td>
					<td id=\"totalAbandono\" bgcolor=\"#DADADA\">".$totalAbandono."</td>
					<td id=\"totalTransferencia\" bgcolor=\"#DADADA\">".$totalTransferencia."</td>
					<td id=\"totalMatFinal\" bgcolor=\"#DADADA\">".$totalMatFinalTela1."</td>
					<td id=\"totalAprovados\" bgcolor=\"#DADADA\">".$totalAprovados."</td>
					<td id=\"totalReprovados\" bgcolor=\"#DADADA\">".$totalReprovados."</td>
					<td id=\"totalTaxaAprovacao\" bgcolor=\"#DADADA\">".$totalTaxaAprovacao."%</td>
					<td id=\"totalTaxaReprovacao\" bgcolor=\"#DADADA\">".$totalTaxaReprovacao."%</td>
					<td id=\"totaTaxaAbandono\" bgcolor=\"#DADADA\">".$totaTaxaAbandono."%</td>
			  </tr>";
	?>
	</tbody>
</table>
</form>
<br>
<br>

<br>
<br>

<?=subCabecalho('10.3 - Aproveitamento dos alunos da 5ª - 8ª série/ 6º - 9º ano do Ensino Fundamental (ano anterior) '); ?>
<form method="post" id="p93_form_aproveitamento" name="p93_form_aproveitamento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
	  	<td colspan="11">
  		 	Ano de Referencia: 
			<?php
				$ano = $onmid ? $onmanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
					if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
					echo $ano;
			?>
			<input type="hidden" name="pjianoreferencia" value="<?=$ano?>">
		</td>
	</tr>

	<tr>
		<th>SÉRIE</th>
		<th>Matrícula Inicial</th>
		<th>Admitidos após mês de março</th>
		<th>Afastados por Abandono</th>
		<th>Afastados por Transferências</th>
		<th>Matrícula Final</th>
		<th>Aprovados</th>
		<th>Reprovados</th>
		<th>Taxa de Aprovação</th>
		<th>Taxa de Reprovação</th>
		<th>Taxa de Abandono</th>
	</tr>
</thead>
<tbody>
	<?php
		$serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
								WHERE tmeid = 3 AND scetipo = 'S' ORDER BY sceid");
		
		$totalMatInicial = 0;
		$totalAdmitido = 0;
		$totalAbandono = 0;
		$totalTransferencia = 0;
		$totalMatFinalTela1 = 0;
		$totalAprovados = 0;
		$totalReprovados = 0;
		
		for($i=0; $i<count($serie); $i++) {
			//echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			
			$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			
			for($j=0; $j<count($dados); $j++) {
				$div = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] );
				
				if($div > 0) {
			 
					$taxa_aprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdaprovados"] / $div * 100))))."%";
					$taxa_reprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdreprovados"] / $div * 100))))."%";
					$taxa_abandono = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdafastadosabandono"] / $div * 100))))."%";
				
				}
				else {
					$taxa_aprovacao = "0%";
					$taxa_reprovacao = "0%";
					$taxa_abandono = "0%";
				}
				
				$totalMatInicial += $dados[$j]["apaqtdmatriculainicial"];
				$totalAdmitido += $dados[$j]["apaqtdadmitidosaposmarco"];
				$totalAbandono += $dados[$j]["apaqtdafastadosabandono"];
				$totalTransferencia += $dados[$j]["apaqtdafastadostransferencia"];
				$totalMatFinalTela1 += $dados[$j]["apaqtdmatriculafinal"];
				$totalAprovados += $dados[$j]["apaqtdaprovados"];
				$totalReprovados += $dados[$j]["apaqtdreprovados"];
				
				$sqlTotal = "SELECT  m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(2, 3) 
								AND m.sceid = ".$serie[$i]["sceid"]."
								order by m.sceid
								";
			$rsTotal = $db->carregar( $sqlTotal);

			for( $x = 0; $x <count( $rsTotal ); $x++)
			{
				$qtdTotalAluno[$serie[$i]["sceid"]] += $rsTotal[$x]['maiqtdaluno'];
			}

			if( !$qtdTotalAluno[$serie[$i]["sceid"]])
			$value = $dados[$j]["apaqtdmatriculainicial"];
			else
			$value = $qtdTotalAluno[$serie[$i]["sceid"]];
			echo "<tr height=\"30px\" align=\"center\">
					  	<td bgcolor=\"#f0f0f0\">
					  		<b>".$serie[$i]["scedescricao"]."</b>
					  		<input type=\"hidden\" name=\"serie_id[]\" value=\"".$serie[$i]["sceid"]."\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal;\"disabled;\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" value=\"$value\" onblur=\"alteraTaxa('apaqtdmatriculainicial',".$serie[$i]["sceid"].",'totalMatInicial');\" onkeypress=\"return somenteNumeros(event);\"readonly=\"true;\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" name=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdadmitidosaposmarco"]."\" onblur=\"alteraTaxa('apaqtdadmitidosaposmarco',".$serie[$i]["sceid"].",'totalAdmitido');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadosabandono"]."\" onblur=\"alteraTaxa('apaqtdafastadosabandono',".$serie[$i]["sceid"].",'totalAbandono');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadostransferencia"]."\" onblur=\"alteraTaxa('apaqtdafastadostransferencia',".$serie[$i]["sceid"].",'totalTransferencia');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					   <td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdmatriculafinal"]."\" readonly=\"true\" />
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdaprovados"]."\" onblur=\"alteraTaxa('apaqtdaprovados',".$serie[$i]["sceid"].",'totalAprovados');\"  onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdreprovados"]."\" onblur=\"alteraTaxa('apaqtdreprovados',".$serie[$i]["sceid"].",'totalReprovados');\"  onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_aprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_aprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_reprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_reprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_abandono[".$serie[$i]["sceid"]."]\">
					  		".$taxa_abandono."
					  	</td>
					  </tr>";
		}
	}
		$total = ($totalMatInicial + $totalAdmitido - $totalTransferencia );
		if($total > 0) {
			$totalTaxaAprovacao = str_replace('.',',',simec_number_format(($totalAprovados / $total * 100)));
			$totalTaxaReprovacao = str_replace('.',',',simec_number_format(($totalReprovados / $total * 100)));
			$totaTaxaAbandono = str_replace('.',',',simec_number_format(($totalAbandono / $total * 100)));
		}
		else {
			$totalTaxaAprovacao = 0;
			$totalTaxaReprovacao = 0;
			$totaTaxaAbandono = 0;
		}
			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"totalMatInicial\" bgcolor=\"#DADADA\">".$totalMatInicial."</td>
					<td id=\"totalAdmitido\" bgcolor=\"#DADADA\">".$totalAdmitido."</td>
					<td id=\"totalAbandono\" bgcolor=\"#DADADA\">".$totalAbandono."</td>
					<td id=\"totalTransferencia\" bgcolor=\"#DADADA\">".$totalTransferencia."</td>
					<td id=\"totalMatFinal\" bgcolor=\"#DADADA\">".$totalMatFinalTela1."</td>
					<td id=\"totalAprovados\" bgcolor=\"#DADADA\">".$totalAprovados."</td>
					<td id=\"totalReprovados\" bgcolor=\"#DADADA\">".$totalReprovados."</td>
					<td id=\"totalTaxaAprovacao\" bgcolor=\"#DADADA\">".$totalTaxaAprovacao."%</td>
					<td id=\"totalTaxaReprovacao\" bgcolor=\"#DADADA\">".$totalTaxaReprovacao."%</td>
					<td id=\"totaTaxaAbandono\" bgcolor=\"#DADADA\">".$totaTaxaAbandono."%</td>
			  </tr>";
	?>
</tbody>

</table>
</form>
<br>
<br>

<br>
<br>
<?

$arCiclos = $db->carregar( 'select * from pdeescola.seriecicloescolar where scetipo = \'C\' order by sceid' );

#Verifica se já tem dados gravados
$sql = "SELECT
		 *
		FROM
		 pdeescola.aproveitamentoalunocicloetapa
		WHERE
		 pdeid = ".$pdeid;
$dadosBanco = $db->carregar($sql);
$dadosBanco = is_array( $dadosBanco ) ? $dadosBanco : array();
 
?>

<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<?=subCabecalho('10.4 Aproveitamento dos alunos(ano anterior)'); ?>
<table class="tabela listagem" id="itensDeMatricula" border="2" align="center" cellpadding="0" cellspacing="4" width="100%" style="text-align: center">
<thead>
	<tr>
	  	<td align="left" colspan="21">
  		 	Ano de Referencia: 
			<?php
				$ano = $onmid ? $onmanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
					if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
					echo $ano;
			?>
			<input type="hidden" name="pjianoreferencia" value="<?=$ano?>">
		</td>
	</tr>
  <tr>
    <th rowspan="2">Série / Etapa </th>
    <th rowspan="2">Matrícula Inicial</th>
    <th rowspan="2">Admitidos após mês de março</th>
    <th rowspan="2">Afastados por Abandono</th>
    <th rowspan="2">Afastados por Transferência</th>
    <th colspan="4">Avaliação</th>
    <th rowspan="2">Matrícula Atual</th>
    <th colspan="5">Taxa de Progressão</th>
  </tr>
  <tr>
    <th>PS</th>
    <th>PPDA</th>
    <th>RFC</th>
    <th>PMAE</th>
    <th>PS</th>
    <th>PPDA</th>
    <th>PMAE</th>
    <th>Retenção</th>
    <th>Taxa de abandono</th>
  </tr>
<?php
$letra = '1';
$arIdsCiclo = array();
# Inicializa variáveis de subtotal com valor 0
$subTotalMatriculaInicial = $subTotalMatAtual = $subTotalAdmitMarco = $subTotalAfastAbandono = $subTotalAfastTransf = $subTotalAvaPS = $subTotalAvaPPDA = $subTotalAvaRFC = $subTotalAvaPMAE = $subTotalTxProgPS = $subTotalTxProgPPDA = $subTotalTxProgPMAE = $subTotalTxProgRetencao = $subTotalTotalTxAbandono = 0;
foreach( $arCiclos as $arCiclo ) {
		#dbg( $arCiclo, true );
		$descricao = $arCiclo['scedescricao'];

		# Verifica se deve mostrar total parcial 
		if ( $letra != $descricao[0] ) {
?>
	<tr id="subTotal1">
		<th><b>TOTAL</b></th>
		<th><span id="subTotalMatriculaInicial_<?php echo $letra; ?>"><?php echo $subTotalMatriculaInicial; ?></span></th>
		<th><span id="subTotalAdmitMarco_<?php echo $letra; ?>"><?php echo $subTotalAdmitMarco; ?></span></th>
		<th><span id="subTotalAfastAbandono_<?php echo $letra; ?>"><?php echo $subTotalAfastAbandono; ?></span></th>
		<th><span id="subTotalAfastTransf_<?php echo $letra; ?>"><?php echo $subTotalAfastTransf; ?></span></th>
		<th><span id="subTotalAvaPS_<?php echo $letra; ?>"><?php echo $subTotalAvaPS; ?></span></th>
		<th><span id="subTotalAvaPPDA_<?php echo $letra; ?>"><?php echo $subTotalAvaPPDA; ?></span></th>
		<th><span id="subTotalAvaRFC_<?php echo $letra; ?>"><?php echo $subTotalAvaRFC; ?></span></th>
		<th><span id="subTotalAvaPMAE_<?php echo $letra; ?>"><?php echo $subTotalAvaPMAE; ?></span></th>
		<th><span id="subTotalMatAtual_<?php echo $letra; ?>"><?php echo $subTotalMatAtual; ?></span></th>
		<th><span id="subTotalTxProgPS_<?php echo $letra; ?>"><?php echo $subTotalTxProgPS; ?></span></th>
		<th><span id="subTotalTxProgPPDA_<?php echo $letra; ?>"><?php echo $subTotalTxProgPPDA; ?></span></th>
		<th><span id="subTotalTxProgPMAE_<?php echo $letra; ?>"><?php echo $subTotalTxProgPMAE; ?></span></th>
		<th><span id="subTotalTxProgRetencao_<?php echo $letra; ?>"><?php echo $subTotalTxProgRetencao; ?></span></th>
		<th><span id="subTotalTotalTxAbandono_<?php echo $letra; ?>"><?php echo round( $subTotalTotalTxAbandono/3, 2 ); ?> %</span></th>
	</tr>
<?php
			$letra = $descricao[0];
		
			# Inicializa variáveis de subtotal com valor 0
			$subTotalMatriculaInicial = $subTotalMatAtual = $subTotalAdmitMarco = $subTotalAfastAbandono = $subTotalAfastTransf = $subTotalAvaPS = $subTotalAvaPPDA = $subTotalAvaRFC = $subTotalAvaPMAE = $subTotalTxProgPS = $subTotalTxProgPPDA = $subTotalTxProgPMAE = $subTotalTxProgRetencao = $subTotalTotalTxAbandono = 0;
		 } // if imprime total
		# Soma subtotais
	    $subTotalMatriculaInicial += $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'];
	    $subTotalAdmitMarco       += $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'];
	    $subTotalAfastAbandono    += $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'];
	    $subTotalAfastTransf      += $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'];
	    $subTotalAvaPS            += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalps'];
	    $subTotalAvaPPDA          += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalppda'];
	    $subTotalAvaRFC           += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalrfc'];
	    $subTotalAvaPMAE          += $arDadosBanco[$arCiclo['sceid']]['apaqtdavalpmae'];
	    $subTotalMatAtual         += $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculaatual'];
	    $subTotalTxProgPS         += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpps'];
	    $subTotalTxProgPPDA       += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpppda'];
	    $subTotalTxProgPMAE       += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxppmae'];
	    $subTotalTxProgRetencao   += $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpretencao'];
	    $subTotalTotalTxAbandono  += $txAbandono;
	    
	    $valor_divisor = ( $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'] + 
					      $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'] -
					      $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'] );
		
		if($valor_divisor == 0) { 
			$resultado = 0;
		} else {
			$resultado = ($arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'] / $valor_divisor);
		}
			
	    $txAbandono = round( $resultado * 100, 2 );
	    		  
		$arIdsCiclo[$letra][] = $arCiclo['sceid'];
?>
  <tr>
    <th><?php echo $descricao; ?></th>
    <td align="center"><?php echo campo_texto('arMatriculaInicial['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'MatriculaInicial\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculainicial'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAdmitMarco['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AdmitMarco\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdadmitidoaposmarco'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAfastAbandono['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AfastAbandono\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadoabandono'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAfastTransf['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AfastTransf\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdafastadotransferencia'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPS['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPS\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalps'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPPDA['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPPDA\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalppda'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaRFC['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaRFC\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalrfc'] ); ?></td>
    <td align="center"><?php echo campo_texto('arAvaPMAE['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'AvaPMAE\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdavalpmae'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMatAtual['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'MatAtual\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdmatriculaatual'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPS['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPS\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpps'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPPDA['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPPDA\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpppda'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgPMAE['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgPMAE\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxppmae'] ); ?></td>
    <td align="center"><?php echo campo_texto('arTxProgRetencao['.$arCiclo['sceid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'TxProgRetencao\' ); calcularTaxaAbandono( '.$arCiclo['sceid'].' )"', '', $arDadosBanco[$arCiclo['sceid']]['apaqtdtxpretencao'] ); ?></td>
    <th><span id="txAbandono[<?php echo $arCiclo['sceid']; ?>]"><?php echo $txAbandono; ?> %</span></th>
  </tr>
<?php 

	} ?>
	<tr id="subTotal1">
		<th><b>TOTAL</b></th>
		<th><span id="subTotalMatriculaInicial_<?php echo $letra; ?>"><?php echo $subTotalMatriculaInicial; ?></span></th>
		<th><span id="subTotalAdmitMarco_<?php echo $letra; ?>"><?php echo $subTotalAdmitMarco; ?></span></th>
		<th><span id="subTotalAfastAbandono_<?php echo $letra; ?>"><?php echo $subTotalAfastAbandono; ?></span></th>
		<th><span id="subTotalAfastTransf_<?php echo $letra; ?>"><?php echo $subTotalAfastTransf; ?></span></th>
		<th><span id="subTotalAvaPS_<?php echo $letra; ?>"><?php echo $subTotalAvaPS; ?></span></th>
		<th><span id="subTotalAvaPPDA_<?php echo $letra; ?>"><?php echo $subTotalAvaPPDA; ?></span></th>
		<th><span id="subTotalAvaRFC_<?php echo $letra; ?>"><?php echo $subTotalAvaRFC; ?></span></th>
		<th><span id="subTotalAvaPMAE_<?php echo $letra; ?>"><?php echo $subTotalAvaPMAE; ?></span></th>
		<th><span id="subTotalMatAtual_<?php echo $letra; ?>"><?php echo $subTotalMatAtual; ?></span></th>
		<th><span id="subTotalTxProgPS_<?php echo $letra; ?>"><?php echo $subTotalTxProgPS; ?></span></th>
		<th><span id="subTotalTxProgPPDA_<?php echo $letra; ?>"><?php echo $subTotalTxProgPPDA; ?></span></th>
		<th><span id="subTotalTxProgPMAE_<?php echo $letra; ?>"><?php echo $subTotalTxProgPMAE; ?></span></th>
		<th><span id="subTotalTxProgRetencao_<?php echo $letra; ?>"><?php echo $subTotalTxProgRetencao; ?></span></th>
		<th><span id="subTotalTotalTxAbandono_<?php echo $letra; ?>"><?php echo round( $subTotalTotalTxAbandono/3, 2 ) ?> %</span></th>
	</tr>
</table>
</form>
<br>
<br>

<br>
<br>

<?=subCabecalho('10.5 - Distorção idade-série - 1ª - 4ª série/ 1º - 5º ano do Ensino Fundamental (ano anterior)'); ?>
<form method="post" id="p96_form_aproveitamento" name="p96_form_aproveitamento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela listagem" align="center">
 	<tr>
  	<td colspan="3">
  		
  		 <?php 
  		 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
  		 $anoex2  = $anoex +1;
    	 $anoex3  = $anoex2 +1;
//  	 $anoex4  = $anoex3 -1;
	  	 $arrUrl  = $_SERVER['argv'][0];
		 $arrUrl  = explode("&", $arrUrl);
		 $arrItem = explode("/", $arrUrl[0]);	 
		 $indice  = (count($arrItem) - 1);
		 $ppritem = $arrItem[$indice];
  		 
  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
  		 					  FROM pdeescola.pdepreenchimento 
  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
  		 					  AND ppritem = '$ppritem'";
  		 
		 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>   
    </td>
  </tr>
</table>
<?php
$sql_turno = "SELECT titid, titdescricao FROM pdeescola.tipoturno ORDER BY titid";
$turno = $db->carregar ( $sql_turno );

for( $y = 0; $y < count( $turno ); $y++ )
{ 
    
    $titid = $turno[$y]['titid'];
?>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <thead>
    <td colspan="11">
    	<div class="SubtituloEsquerda" style="padding: 5px; text-align: left; "><?php echo $turno[$y]['titdescricao']; ?></div>
    </td>
    </thead>
    <thead>
        <tr>
            <th width="5%">SÉRIE/ANO</th>
            <th width="10%">Matrícula Final (A)</th>
            <th width="10%">Até 7 anos</th>
            <th width="10%">Até 8 anos</th>
            <th width="10%">Até 9 anos</th>
            <th width="10%">Até 10 anos</th>
            <th width="10%">Até 11 anos</th>
            <th width="10%">Até 12 anos</th>
            <th width="10%">Mais de 12 anos</th>
            <th width="15%">Total de alunos com idade superior à série respectiva (B)</th>
            <th width="10%">Taxa de Distorção (B/A) x 100</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
                                    WHERE tmeid = 2 AND scetipo = 'S' AND scedescricao in ('0/1º','1ª/2º','2ª/3º','3ª/4º','4ª/5º')
                                    ORDER BY sceid");
            
                    $subTotalMatFinalTela5 = 0;
                    $subTotal7anos = 0;
                    $subTotal8anos = 0;
                    $subTotal9anos = 0;
                    $subTotal10anos = 0;
                    $subTotal11anos = 0;
                    $subTotal12anos = 0;
                    $subTotalMais12anos = 0;
                    $subTotalIdadeSuperior = 0;
                    
            
            
            for($i=0; $i<count($serie); $i++) {
                //echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."');  </script>";
                $x++;
                $dados = $db->carregar("SELECT
                                            matfinal,	
                                            sum(qtdalunos0) as qtd7anos,
                                            sum(qtdalunos1) as qtd8anos,
                                            sum(qtdalunos2) as qtd9anos,
                                            sum(qtdalunos3) as qtd10anos,
                                            sum(qtdalunos4) as qtd11anos,
                                            sum(qtdalunos5) as qtd12anos,
                                            sum(qtdalunos6) as qtdmais12anos
                                        FROM
                                        (
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            dis.disqtdalunos as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 1 AND
                                            titid = '{$turno[$y]['titid']}'
                                            
                                        
                                        UNION ALL
                                        
                                        
                                        SELECT 
                                        	
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            dis.disqtdalunos as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 2 AND
                                            titid = '{$turno[$y]['titid']}'
                                            
                                        
                                        UNION ALL
                                        
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            dis.disqtdalunos as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 3 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            dis.disqtdalunos as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 4 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            dis.disqtdalunos as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 5 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            dis.disqtdalunos as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 6 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos0,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            dis.disqtdalunos as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 7 AND
                                            titid = '{$turno[$y]['titid']}'
                                        ) as foo 
                                        GROUP BY matfinal");
                        
                for($j=0; $j<count($dados); $j++) {
                	if(($serie[$i]["sceid"]) == 1){
                    	$idadeSuperior = ($dados[$j]["qtd8anos"] + $dados[$j]["qtd9anos"] + $dados[$j]["qtd10anos"] + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                	}
                    if(($serie[$i]["sceid"]) == 2){
                    	$idadeSuperior = ($dados[$j]["qtd9anos"] + $dados[$j]["qtd10anos"] + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                	} 
                	if(($serie[$i]["sceid"]) == 3){
                    	$idadeSuperior = ($dados[$j]["qtd10anos"] + $dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                	}
                    if(($serie[$i]["sceid"]) == 4){
                    	$idadeSuperior = ($dados[$j]["qtd11anos"] + $dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                	} 
                	if(($serie[$i]["sceid"]) == 5){
                    	$idadeSuperior = ($dados[$j]["qtd12anos"] + $dados[$j]["qtdmais12anos"]);
                	}  
                   	$idadeSuperior = ($idadeSuperior == '') ? 0 : $idadeSuperior;
                	
                    if($dados[$j]["matfinal"] != 0)
                    {
                        $taxa = str_replace('.',',',simec_number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100),2));
                    	$taxaDistorcao = floor( $taxa ).'%';
                    }
                    else
                    {
                        $taxaDistorcao = 0;
                    }
                    
                    $subTotalMatFinalTela5 += $dados[$j]["matfinal"];
                    $subTotal7anos += $dados[$j]["qtd7anos"];
                    $subTotal8anos += $dados[$j]["qtd8anos"]; 
                    $subTotal9anos += $dados[$j]["qtd9anos"];
                    $subTotal10anos += $dados[$j]["qtd10anos"];
                    $subTotal11anos += $dados[$j]["qtd11anos"];
                    $subTotal12anos += $dados[$j]["qtd12anos"];
                    $subTotalMais12anos += $dados[$j]["qtdmais12anos"];
                    $subTotalIdadeSuperior += $idadeSuperior;
                    
                    
                    echo "<tr height=\"30px\" align=\"center\">
                            <td bgcolor=\"#f0f0f0\">
                                <b>".$serie[$i]["scedescricao"]."</b>
                                <input type=\"hidden\" name=\"serie_id[$y][".$serie[$i]["sceid"]."]\" value=\"".$serie[$i]["sceid"]."\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"matfinal[$y][".$serie[$i]["sceid"]."]\" name=\"matfinal[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["matfinal"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td> 
                                                                                                  
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd7anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd7anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd7anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>

                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd8anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd8anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd8anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                             
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd9anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd9anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd9anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                             
                            <td bgcolor=\"#f0f0f0\">                                                          
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd10anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd10anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd10anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                              
                            <td bgcolor=\"#f0f0f0\">                                                           
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd11anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd11anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd11anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>                                                                              
                            <td bgcolor=\"#f0f0f0\">                                                           
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd12anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtdmais12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtdmais12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtdmais12anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\" id=\"idade_superior[$y][".$serie[$i]["sceid"]."]\">
                                ".$idadeSuperior."                     
                            </td>                                      
                            <td bgcolor=\"#f0f0f0\" id=\"taxa_distorcao[$y][".$serie[$i]["sceid"]."]\">
                                ".$taxaDistorcao."
                            </td>
                          </tr>";
                }
            }
            
        if($subTotalMatFinalTela5 != 0)
		{
			$totalTaxa = str_replace('.',',',simec_number_format((($subTotalIdadeSuperior / $subTotalMatFinalTela5) * 100),2));
			$subTotalTaxaDistorcao = round( $totalTaxa );
		}
		else
		{
			$subTotalTaxaDistorcao = 0 + "%";
		}
                $totalMatFinalTotal += $subTotalMatFinalTela5;
		        $total7anosTotal += $subTotal7anos;
		        $total8anosTotal += $subTotal8anos;
		        $total9anosTotal += $subTotal9anos;
		        $total10anosTotal += $subTotal10anos;
		        $total11anosTotal += $subTotal11anos;
		        $total12anosTotal += $subTotal12anos;
		        $totalMais12anosTotal += $subTotalMais12anos;
		        $totalIdadeSuperiorTotal += $subTotalIdadeSuperior;

            
           echo "<tr height=\"30px\" align=\"center\">
                        <td bgcolor=\"#DADADA\"><b>Subtotal</b></td>
                        <td id=\"subTotalMatFinal[$y]\" bgcolor=\"#DADADA\">".$subTotalMatFinalTela5."</td>
                        <td id=\"subTotal7anos[$y]\" bgcolor=\"#DADADA\">".$subTotal7anos."</td>
                        <td id=\"subTotal8anos[$y]\" bgcolor=\"#DADADA\">".$subTotal8anos."</td>
                        <td id=\"subTotal9anos[$y]\" bgcolor=\"#DADADA\">".$subTotal9anos."</td>
                        <td id=\"subTotal10anos[$y]\" bgcolor=\"#DADADA\">".$subTotal10anos."</td>
                        <td id=\"subTotal11anos[$y]\" bgcolor=\"#DADADA\">".$subTotal11anos."</td>
                        <td id=\"subTotal12anos[$y]\" bgcolor=\"#DADADA\">".$subTotal12anos."</td>
                        <td id=\"subTotalMais12anos[$y]\" bgcolor=\"#DADADA\">".$subTotalMais12anos."</td>
                        <td id=\"subTotalIdadeSuperior[$y]\" bgcolor=\"#DADADA\">".$subTotalIdadeSuperior."</td>
                        <td id=\"subTotalTaxaDistorcao[$y]\" bgcolor=\"#DADADA\">".$subTotalTaxaDistorcao."%</td>
                  </tr>";
        ?>
    </tbody>
    
<?php

		if($totalMatFinalTotal != 0)
		{
			$totalTaxa = str_replace('.',',',simec_number_format((($totalIdadeSuperiorTotal / $totalMatFinalTotal) * 100),2));
			$totalTaxaDistorcaoTotal = floor( $totalTaxa );
		}
		else
		{
			$totalTaxaDistorcaoTotal = 0 + "%";
		}

}
//end of turn´s for

               
            echo "<tr height=\"30px\" align=\"center\">
                        <td bgcolor=\"gray\"><b>TOTAL</b></td>
                        <td id=\"totalMatFinal\" bgcolor=\"gray\">".$totalMatFinalTotal."</td>
                        <td id=\"total7anos\" bgcolor=\"gray\">".$total7anosTotal."</td>
                        <td id=\"total8anos\" bgcolor=\"gray\">".$total8anosTotal."</td>
                        <td id=\"total9anos\" bgcolor=\"gray\">".$total9anosTotal."</td>
                        <td id=\"total10anos\" bgcolor=\"gray\">".$total10anosTotal."</td>
                        <td id=\"total11anos\" bgcolor=\"gray\">".$total11anosTotal."</td>
                        <td id=\"total12anos\" bgcolor=\"gray\">".$total12anosTotal."</td>
                        <td id=\"totalMais12anos\" bgcolor=\"gray\">".$totalMais12anosTotal."</td>
                        <td id=\"totalIdadeSuperior\" bgcolor=\"gray\">".$totalIdadeSuperiorTotal ."</td>
                        <td id=\"totalTaxaDistorcao\" bgcolor=\"gray\">".$totalTaxaDistorcaoTotal."%</td>
                  </tr>";
                  ?>
 </table>
 </form>
 <br>
 <br>

<br>
<br>
 
<?=subCabecalho('10.6 - Distorção idade-série - 5ª - 8ª série/ 6º - 9º ano do Ensino Fundamental (ano anterior)'); ?>
<form method="post" id="p97_form_aproveitamento" name="p97_form_aproveitamento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela listagem" align="center">
 	<tr>
  	<td colspan="3">
  		
  		 <?php 
	  	 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
	  	 $anoex2  = $anoex +1;
//  	 $anoex3  = $anoex2 -1;
//  	 $anoex4  = $anoex3 -1;
	  	 $arrUrl  = $_SERVER['argv'][0];
		 $arrUrl  = explode("&", $arrUrl);
		 $arrItem = explode("/", $arrUrl[0]);	 
		 $indice  = (count($arrItem) - 1);
		 $ppritem = $arrItem[$indice];
  		 
  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
  		 					  FROM pdeescola.pdepreenchimento 
  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
  		 					  AND ppritem = '$ppritem'";
  		 
		 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
    		<!--  
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
         	-->
		 </select>  
    </td>
  </tr>
</table>

<?php
$sql_turno = "SELECT titid, titdescricao FROM pdeescola.tipoturno ORDER BY titid";
$turno = $db->carregar ( $sql_turno );

for( $y = 0; $y < count( $turno ); $y++ )
{ 
    
    $titid = $turno[$y]['titid'];
?>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <thead>
    <td colspan= "10">
    <div class="SubtituloEsquerda" style="padding: 5px; text-align: left; "><?php echo $turno[$y]['titdescricao']; ?></div>
    </td>
    </thead>
    <thead>
        <tr>
            <th width="5%">SÉRIE/ANO</th>
            <th width="10%">Matrícula Final (A)</th>
            <th width="10%">Até 12 anos</th>
            <th width="10%">Até 13 anos</th>
            <th width="10%">Até 14 anos</th>
            <th width="10%">Até 15 anos</th>
            <th width="10%">Até 16 anos</th>
            <th width="10%">Mais de 16 anos</th>
            <th width="15%">Total de alunos com idade superior à série respectiva (B)</th>
            <th width="10%">Taxa de Distorção (B/A) x 100</th>
        </tr>
    </thead>
    <tbody>
        <?php
            $serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
                                    WHERE tmeid = 3 AND scetipo = 'S'
                                    ORDER BY sceid");
            /*
            echo("<pre>");
        	print_r( $serie );
         	echo("</pre>");
            //die();
            */
                        
            $totalMatFinalTela6 = 0;
            $total12anos = 0;
            $total13anos = 0;
            $total14anos = 0;
            $total15anos = 0;
            $total16anos = 0;
            $totalMais16anos = 0;
            $totalIdadeSuperior = 0;
            
            for($i=0; $i<count($serie); $i++) {
                //echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
                
                $dados = $db->carregar("SELECT
                                            matfinal,
                                            sum(qtdalunos1) as qtd12anos,
                                            sum(qtdalunos2) as qtd13anos,
                                            sum(qtdalunos3) as qtd14anos,
                                            sum(qtdalunos4) as qtd15anos,
                                            sum(qtdalunos5) as qtd16anos,
                                            sum(qtdalunos6) as qtdmais16anos
                                        FROM
                                        (
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            dis.disqtdalunos as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 5 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos1,
                                            dis.disqtdalunos as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 7 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            dis.disqtdalunos as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 8 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            dis.disqtdalunos as qtdalunos4,
                                            0 as qtdalunos5,
                                            0 as qtdalunos6 
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 9 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            dis.disqtdalunos as qtdalunos5,
                                            0 as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 10 AND
                                            titid = '{$turno[$y]['titid']}'
                                        
                                        UNION ALL
                                        
                                        SELECT 
                                            dis.disqtdmatriculafinal as matfinal,
                                            0 as qtdalunos1,
                                            0 as qtdalunos2,
                                            0 as qtdalunos3,
                                            0 as qtdalunos4,
                                            0 as qtdalunos5,
                                            dis.disqtdalunos as qtdalunos6
                                        FROM 
                                            pdeescola.distorcaoidadeserie dis
                                        WHERE 
                                            dis.pdeid = ".$pdeid." AND 
                                            dis.sceid = ".$serie[$i]["sceid"]." AND 
                                            dis.disanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA." AND
                                            dis.faeid = 11 AND
                                            titid = '{$turno[$y]['titid']}'
                                        ) as foo 
                                        GROUP BY matfinal");
                                      
                for($j=0; $j<count($dados); $j++) {
                	if(($serie[$i]["sceid"]) == 6){
                    	$idadeSuperior = ($dados[$j]["qtd13anos"] + $dados[$j]["qtd14anos"] + $dados[$j]["qtd15anos"] + $dados[$j]["qtd16anos"] + $dados[$j]["qtdmais16anos"]);
                	}
                    if(($serie[$i]["sceid"]) == 7){
                	   	$idadeSuperior = ($dados[$j]["qtd14anos"] + $dados[$j]["qtd15anos"] + $dados[$j]["qtd16anos"] + $dados[$j]["qtdmais16anos"]);
                	}
                    if(($serie[$i]["sceid"]) == 8){
                	  	$idadeSuperior = ($dados[$j]["qtd15anos"] + $dados[$j]["qtd16anos"] + $dados[$j]["qtdmais16anos"]);
                	}               	
                    if(($serie[$i]["sceid"]) == 9){
                	   	$idadeSuperior = ($dados[$j]["qtd16anos"] + $dados[$j]["qtdmais16anos"]);
                	}             	
                    $idadeSuperior = ($idadeSuperior == '') ? 0 : $idadeSuperior;
                	
                    if($dados[$j]["matfinal"] != 0)
                    {
                        $taxa = str_replace('.',',',simec_number_format((($idadeSuperior / $dados[$j]["matfinal"]) * 100),2))."%";
                    	$taxaDistorcao = round( $taxa ).'%';
                    }
                    else
                    {
                    	$taxaDistorcao = 0 ;
                    }
					 
                    $totalMatFinalTela6 += $dados[$j]["matfinal"];
                    $total12anos += $dados[$j]["qtd12anos"];
                    $total13anos += $dados[$j]["qtd13anos"];
                    $total14anos += $dados[$j]["qtd14anos"];
                    $total15anos += $dados[$j]["qtd15anos"];
                    $total16anos += $dados[$j]["qtd16anos"];
                    $totalMais16anos += $dados[$j]["qtdmais16anos"];
                    $totalIdadeSuperior += $idadeSuperior;
 
                    echo "<tr height=\"30px\" align=\"center\">
                            <td bgcolor=\"#f0f0f0\">
                                <b>".$serie[$i]["scedescricao"]."</b>
                                <input type=\"hidden\" name=\"serie_id[$y][".$serie[$i]["sceid"]."]\" value=\"".$serie[$i]["sceid"]."\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"matfinal[$y][".$serie[$i]["sceid"]."]\" name=\"matfinal[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["matfinal"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd12anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd12anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd13anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd13anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd13anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd14anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd14anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd14anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd15anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd15anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd15anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtd16anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtd16anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtd16anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\">
                                <input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"qtdmais16anos[$y][".$serie[$i]["sceid"]."]\" name=\"qtdmais16anos[$y][".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["qtdmais16anos"]."\" onkeypress=\"return somenteNumeros(event);\">
                            </td>
                            <td bgcolor=\"#f0f0f0\" id=\"idade_superior[$y][".$serie[$i]["sceid"]."]\">
                                ".$idadeSuperior."
                            </td>
                            <td bgcolor=\"#f0f0f0\" id=\"taxa_distorcao[$y][".$serie[$i]["sceid"]."]\">
                                ".$taxaDistorcao."
                            </td>
                          </tr>";

                }
            }
		if($totalMatFinalTela6 != 0)
		{
			$totalTaxa = str_replace('.',',',simec_number_format((($totalIdadeSuperior / $totalMatFinalTela6) * 100),2));
			$totalTaxaDistorcao = round( $totalTaxa );
		}
		else
		{
			$totalTaxaDistorcao = '0';
		}
                $totalMatFinalTotalTela6 += $totalMatFinalTela6;
		        $total12anosTotalTela6 += $total12anos;
		        $total13anosTotalTela6 += $total13anos;
		        $total14anosTotalTela6 += $total14anos;
		        $total15anosTotalTela6 += $total15anos;
		        $total16anosTotalTela6 += $total16anos;
		        $totalMais16anosTotalTela6 += $totalMais16anos;
		        $totalIdadeSuperiorTotalTela6 += $totalIdadeSuperior;

		        

				
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"subTotalMatFinal[$y]\" bgcolor=\"#DADADA\">".$totalMatFinalTela6."</td>
					<td id=\"subTotal12anos[$y]\" bgcolor=\"#DADADA\">".$total12anos."</td>
					<td id=\"subTotal13anos[$y]\" bgcolor=\"#DADADA\">".$total13anos."</td>
					<td id=\"subTotal14anos[$y]\" bgcolor=\"#DADADA\">".$total14anos."</td>
					<td id=\"subTotal15anos[$y]\" bgcolor=\"#DADADA\">".$total15anos."</td>
					<td id=\"subTotal16anos[$y]\" bgcolor=\"#DADADA\">".$total16anos."</td>
					<td id=\"subTotalMais16anos[$y]\" bgcolor=\"#DADADA\">".$totalMais16anos."</td>
					<td id=\"subTotalIdadeSuperior[$y]\" bgcolor=\"#DADADA\">".$totalIdadeSuperior."</td>
					<td id=\"subTotalTaxaDistorcao[$y]\" bgcolor=\"#DADADA\">".$totalTaxaDistorcao."%</td>
			  </tr>";
					
		if($totalMatFinalTotalTela6 != 0)
		{
			$totalTaxa = str_replace('.',',',simec_number_format((($totalIdadeSuperiorTotalTela6 / $totalMatFinalTotalTela6) * 100),2));
			$totalTaxaDistorcaoTotalTela6 = round( $totalTaxa );
		}
		else
		{
			$totalTaxaDistorcaoTotalTela6 = '0';
		}
}//end of turn´s for


			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"gray\"><b>TOTAL</b></td>
					<td id=\"totalMatFinal\" bgcolor=\"gray\">".$totalMatFinalTotalTela6."</td>
					<td id=\"total12anos\" bgcolor=\"gray\">".$total12anosTotalTela6."</td>
					<td id=\"total13anos\" bgcolor=\"gray\">".$total13anosTotalTela6."</td>
					<td id=\"total14anos\" bgcolor=\"gray\">".$total14anosTotalTela6."</td>
					<td id=\"total15anos\" bgcolor=\"gray\">".$total15anosTotalTela6."</td>
					<td id=\"total16anos\" bgcolor=\"gray\">".$total16anosTotalTela6."</td>
					<td id=\"totalMais16anos\" bgcolor=\"gray\">".$totalMais16anosTotalTela6."</td>
					<td id=\"totalIdadeSuperior\" bgcolor=\"gray\">".$totalIdadeSuperiorTotalTela6."</td>
					<td id=\"totalTaxaDistorcao\" bgcolor=\"gray\">".$totalTaxaDistorcaoTotalTela6."%</td>
			  </tr>";
	?>
</tbody>
</table>
</form>

<br>
<br>

<br>
<br>
<!-- inicio 11_1 -->

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formulario">

<?=subCabecalho('11.1 - Matrícula inicial (ano anterior) - Ensino Médio'); 
//matutino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(6) and titid = '1'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?> 	  	 	  	  	  	
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="6%"> Série / Ano / Ciclo</th>
		<th colspan="2">MATUTINO </th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	//$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Vespertino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(6) and titid = '2'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
		<tbody>
		<th width="6%">Série / Ano / Ciclo</th>
		<th colspan="2">VESPERTINO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Noturno
$sql = "SELECT DISTINCT 
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(6) and titid = '3'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="6%">Série / Ano / Ciclo</th>
		<th colspan="2">NOTURNO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
$sql = "SELECT DISTINCT 
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid				
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(6) and titid = '4'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="6%">Série / Ano / Ciclo</th>
		<th colspan="2">INTEGRAL</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="3%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
</form>
<!-- fim 11_1 -->
<br>
<br>

<br>
<br>

<?=subCabecalho('11.2 - Aproveitamento dos alunos da 1ª à 3ª série do Ensino Médio (ano anterior)'); ?>
<form method="post" id="p102_form_aproveitamento" name="p102_form_aproveitamento">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela listagem" align="center">
 	<tr>
  	<td colspan="3">
  		
  		 <?php 
  		 $anoex   = $_SESSION['exercicio_atual'];
  		 $anoex2  = $anoex +1;
//  	 $anoex3  = $anoex2 -1;
//  	 $anoex4  = $anoex3 -1;
  		 $arrUrl  = $_SERVER['argv'][0];
	  	 $arrUrl  = explode("&", $arrUrl);
		 $arrItem = explode("/", $arrUrl[0]);	 
		 $indice  = (count($arrItem) - 1);
	  	 $ppritem = $arrItem[$indice];
  		 
  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
  		 					  FROM pdeescola.pdepreenchimento 
  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
  		 					  AND ppritem = '$ppritem'";
  		 
		 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>   
    </td>
  </tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
		<th>SÉRIE</th>
		<th>Matrícula Inicial</th>
		<th>Admitidos após mês de março</th>
		<th>Afastados por Abandono</th>
		<th>Afastados por Transferências</th>
		<th>Matrícula Final</th>
		<th>Aprovados</th>
		<th>Reprovados</th>
		<th>Taxa de Aprovação</th>
		<th>Taxa de Reprovação</th>
		<th>Taxa de Abandono</th>
	</tr>
</thead>
<tbody>
	<?php
		$serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
								WHERE tmeid = 6 AND scetipo = 'S' ORDER BY sceid");
		
		$totalMatInicial = 0;
		$totalAdmitido = 0;
		$totalAbandono = 0;
		$totalTransferencia = 0;
		$totalMatFinal = 0;
		$totalAprovados = 0;
		$totalReprovados = 0;
		
		for($i=0; $i<count($serie); $i++) {
			//cho "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			
			$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			
					for($j=0; $j<count($dados); $j++) {
				if($dados[$j]["apaqtdmatriculainicial"] != 0) {
					$divisor = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] );
					
			 		if( $divisor > 0  )
			 		{
				 		//$taxa_aprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdaprovados"] / ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] ) * 100))))."%";
						$taxa_aprovacao = ($dados[$j]["apaqtdaprovados"] / ($divisor) * 100);
						$taxa_aprovacao = simec_number_format($taxa_aprovacao,1, '.', '')."%";
						
						//$taxa_reprovacao = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdreprovados"] / ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] ) * 100))))."%";
						$taxa_reprovacao = ($dados[$j]["apaqtdreprovados"] / ($divisor) * 100);
						$taxa_reprovacao = simec_number_format($taxa_reprovacao,1, '.', '')."%";
						
						//$taxa_abandono = str_replace('.',',',simec_number_format(floor(($dados[$j]["apaqtdafastadosabandono"] / ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"] ) * 100))))."%";
						$taxa_abandono = ($dados[$j]["apaqtdafastadosabandono"] / ($divisor ) * 100);
						$taxa_abandono = simec_number_format($taxa_abandono,1, '.', '')."%";
			 		}
				}
				else {
					$taxa_aprovacao = "0%";
					$taxa_reprovacao = "0%";
					$taxa_abandono = "0%";
				}
				
				$totalMatInicial += $dados[$j]["apaqtdmatriculainicial"];
				$totalAdmitido += $dados[$j]["apaqtdadmitidosaposmarco"];
				$totalAbandono += $dados[$j]["apaqtdafastadosabandono"];
				$totalTransferencia += $dados[$j]["apaqtdafastadostransferencia"];
				$totalMatFinal += $dados[$j]["apaqtdmatriculafinal"];
				$totalAprovados += $dados[$j]["apaqtdaprovados"];
				$totalReprovados += $dados[$j]["apaqtdreprovados"];
				
						 $sqlTotal = "SELECT distinct m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(6)
								AND m.sceid = ".$serie[$i]["sceid"]."
								order by m.sceid
								";
			$rsTotal = $db->carregar( $sqlTotal);

			for( $x = 0; $x <count( $rsTotal ); $x++)
			{
				$qtdTotalAluno[$serie[$i]["sceid"]] += $rsTotal[$x]['maiqtdaluno'];
			}

			if( !$qtdTotalAluno[$serie[$i]["sceid"]])
			$value = $dados[$j]["apaqtdmatriculainicial"];
			else
			$value = $qtdTotalAluno[$serie[$i]["sceid"]];
				
			echo "<tr height=\"30px\" align=\"center\">
					  	<td bgcolor=\"#f0f0f0\">
					  		<b>".$serie[$i]["scedescricao"]."</b>
					  		<input type=\"hidden\" name=\"serie_id[]\" value=\"".$serie[$i]["sceid"]."\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  			<input class=\"normal;\"disabled;\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" value=\"$value\" onblur=\"alteraTaxa('apaqtdmatriculainicial',".$serie[$i]["sceid"].",'totalMatInicial');\" onkeypress=\"return somenteNumeros(event);\"readonly=\"true;\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" name=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdadmitidosaposmarco"]."\" onblur=\"alteraTaxa('apaqtdadmitidosaposmarco',".$serie[$i]["sceid"].",'totalAdmitido');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadosabandono"]."\" onblur=\"alteraTaxa('apaqtdafastadosabandono',".$serie[$i]["sceid"].",'totalAbandono');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadostransferencia"]."\" onblur=\"alteraTaxa('apaqtdafastadostransferencia',".$serie[$i]["sceid"].",'totalTransferencia');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" value=\"".(($dados[$j]["apaqtdmatriculafinal"] == "") ? 0 : $dados[$j]["apaqtdmatriculafinal"])."\" readonly=\"true\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdaprovados"]."\"  onblur=\"alteraTaxa('apaqtdaprovados',".$serie[$i]["sceid"].",'totalAprovados'); onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdreprovados"]."\"  onblur=\"alteraTaxa('apaqtdreprovados',".$serie[$i]["sceid"].",'totalReprovados'); onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_aprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_aprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_reprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_reprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_abandono[".$serie[$i]["sceid"]."]\">
					  		".$taxa_abandono."
					  	</td>
					  </tr>";
			}
		}
		
	    $div = ($totalMatInicial + $totalAdmitido - $totalTransferencia );
		    
		if(($div > 0) && ( $div != '')) {
			@$totalTaxaAprovacao = str_replace('.',',',simec_number_format(($totalAprovados / ($div ) * 100)));
			@$totalTaxaReprovacao = str_replace('.',',',simec_number_format(($totalReprovados / ($div ) * 100)));
			@$totaTaxaAbandono = str_replace('.',',',simec_number_format(($totalAbandono / ($div ) * 100)));

		}
		else {
			$totalTaxaAprovacao = 0;
			$totalTaxaReprovacao = 0;
			$totaTaxaAbandono = 0;
		}
			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"totalMatInicial\" bgcolor=\"#DADADA\">".$totalMatInicial."</td>
					<td id=\"totalAdmitido\" bgcolor=\"#DADADA\">".$totalAdmitido."</td>
					<td id=\"totalAbandono\" bgcolor=\"#DADADA\">".$totalAbandono."</td>
					<td id=\"totalTransferencia\" bgcolor=\"#DADADA\">".$totalTransferencia."</td>
					<td id=\"totalMatFinal\" bgcolor=\"#DADADA\">".$totalMatFinal."</td>
					<td id=\"totalAprovados\" bgcolor=\"#DADADA\">".$totalAprovados."</td>
					<td id=\"totalReprovados\" bgcolor=\"#DADADA\">".$totalReprovados."</td>
					<td id=\"totalTaxaAprovacao\" bgcolor=\"#DADADA\">".$totalTaxaAprovacao."%</td>
					<td id=\"totalTaxaReprovacao\" bgcolor=\"#DADADA\">".$totalTaxaReprovacao."%</td>
					<td id=\"totaTaxaAbandono\" bgcolor=\"#DADADA\">".$totaTaxaAbandono."%</td>
			  </tr>";
	?>
</tbody>
</table>
</form>
<br>
<br>

<br>
<br>
<!-- inicio 12_1 -->

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formulario">

<?=subCabecalho('12.1 - Matrícula inicial (ano anterior) - EJA'); 
//matutino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(10) and titid = '1'
		AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?> 	  	 	  	  	  	
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="10%"> Série / Ano / Ciclo</th>
		<th colspan="2">MATUTINO </th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	//$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Vespertino
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid			
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(10) and titid = '2'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
		<tbody>
		<th width="10%">Série / Ano / Ciclo</th>
		<th colspan="2">VESPERTINO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
//Noturno
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid			
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(10) and titid = '3'
		AND
	        pdeanoreferencia is not null
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="10%">Série / Ano / Ciclo</th>
		<th colspan="2">NOTURNO</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
<br>
<?
$sql = "SELECT DISTINCT
			scedescricao, 
			maiqtdturma, 
			maiqtdaluno, 
			CASE
			 WHEN maianee IS NOT NULL THEN maianee
			 ELSE 0
			END AS maianee, 
			pdeanoreferencia
		FROM 
			pdeescola.matriculainicial m
		LEFT JOIN 
			pdeescola.seriecicloescolar s ON s.sceid = m.sceid
		LEFT JOIN 
			pdeescola.pdepreenchimento p ON p.pdeid = m.pdeid			
		WHERE 
			m.pdeid = ".$pdeid." and s.tmeid in(10) and titid = '4'
		AND
	        pdeanoreferencia is not null			
		ORDER BY 
			scedescricao";
?>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="border-top-width:0px;">
	<tbody>
		<th width="10%">Série / Ano / Ciclo</th>
		<th colspan="2">INTEGRAL</th>
		<th> </th>
		<th> </th>
	</tbody>
	<tbody>
		<th> </th>
		<th width="3%">Turmas</th>
		<th width="4%">Alunos</th>
		<th width="4%">ANEE</th>
		<th width="5%">Ano</th>
	</tbody>
	<?
	$db->monta_lista_simples( $sql, '', 50, 10, 'N', '', '' );
	?>
</table>
</form>
<!--fim 12_1-->
<br>
<br>

<br>
<br>
<!--inicio 12_2-->
<?=subCabecalho('12.2 - Aproveitamento dos alunos da 1ª - 8ª Etapa / Segmento / Totalidade - EJA (ano anterior) '); ?>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
  	<td colspan="11">
  		
		<?php 
	  		 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
	  		 $anoex2  = $anoex +1;
    		 $anoex3  = $anoex2 +1;
//  		 $anoex4  = $anoex3 -1;
	  		 $arrUrl  = $_SERVER['argv'][0];
		  	 $arrUrl  = explode("&", $arrUrl);
			 $arrItem = explode("/", $arrUrl[0]);	 
			 $indice  = (count($arrItem) - 1);
		  	 $ppritem = $arrItem[$indice];
	  		 
	  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
	  		 					  FROM pdeescola.pdepreenchimento 
	  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
	  		 					  AND ppritem = '$ppritem'";
	  		 
			 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>  
    </td>
  </tr>

	<tr>
		<th>ETAPA</th>
		<th>Matrícula Inicial</th>
		<th>Admitidos após mês de maio</th>
		<th>Afastados por Abandono</th>
		<th>Afastados por Transferências</th>
		<th>Matrícula Final</th>
		<th>Aprovados</th>
		<th>Reprovados</th>
		<th>Taxa de Aprovação</th>
		<th>Taxa de Reprovação</th>
		<th>Taxa de Abandono</th>
	</tr>
</thead>
<tbody>
	<?php
		$serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
								WHERE tmeid = 10 AND scetipo = 'S' ORDER BY sceid");
		
		$totalMatInicial = 0;
		$totalAdmitido = 0;
		$totalAbandono = 0;
		$totalTransferencia = 0;
		$totalMatFinal = 0;
		$totalAprovados = 0;
		$totalReprovados = 0;
		
		for($i=0; $i<count($serie); $i++) {
			//echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			
			$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			 
			for($j=0; $j<count($dados); $j++) {
				$div = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"]);
				
				if($div > 0) {
					@$taxa_aprovacao = str_replace('.',',',simec_number_format(($dados[$j]["apaqtdaprovados"] / ($div ) * 100),1))."%";
					@$taxa_reprovacao = str_replace('.',',',simec_number_format(($dados[$j]["apaqtdreprovados"] / ($div ) * 100),1))."%";
					@$taxa_abandono = str_replace('.',',',simec_number_format(($dados[$j]["apaqtdafastadosabandono"] / ($div ) * 100),1))."%";
				}
				else {
					$taxa_aprovacao = "0%";
					$taxa_reprovacao = "0%";
					$taxa_abandono = "0%";
				}
				
				$totalMatInicial += $dados[$j]["apaqtdmatriculainicial"];
				$totalAdmitido += $dados[$j]["apaqtdadmitidosaposmarco"];
				$totalAbandono += $dados[$j]["apaqtdafastadosabandono"];
				$totalTransferencia += $dados[$j]["apaqtdafastadostransferencia"];
				$totalMatFinal += $dados[$j]["apaqtdmatriculafinal"];
				$totalAprovados += $dados[$j]["apaqtdaprovados"];
				$totalReprovados += $dados[$j]["apaqtdreprovados"];
				
				$sqlTotal = "SELECT distinct m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(10) 
								AND m.sceid = ".$serie[$i]["sceid"]."
								order by m.sceid
								";
			$rsTotal = $db->carregar( $sqlTotal);

			for( $x = 0; $x <count( $rsTotal ); $x++)
			{
				$qtdTotalAluno[$serie[$i]["sceid"]] += $rsTotal[$x]['maiqtdaluno'];
			}

			if( !$qtdTotalAluno[$serie[$i]["sceid"]])
			$value = $dados[$j]["apaqtdmatriculainicial"];
			else
			$value = $qtdTotalAluno[$serie[$i]["sceid"]];
			
			if(!$value){
				$readOnly = 'readonly="true;"';
			}else{
				unset($readOnly);
			}
			
			echo "<tr height=\"30px\" align=\"center\">
					  	<td bgcolor=\"#f0f0f0\">
					  		<b>".$serie[$i]["scedescricao"]."</b>
					  		<input type=\"hidden\" name=\"serie_id[]\" value=\"".$serie[$i]["sceid"]."\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal;\"disabled;\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" value=\"$value\" onblur=\"alteraTaxa('apaqtdmatriculainicial',".$serie[$i]["sceid"].",'totalMatInicial');\" onkeypress=\"return somenteNumeros(event);\"readonly=\"true;\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" name=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdadmitidosaposmarco"]."\" onblur=\"alteraTaxa('apaqtdadmitidosaposmarco',".$serie[$i]["sceid"].",'totalAdmitido');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadosabandono"]."\" onblur=\"alteraTaxa('apaqtdafastadosabandono',".$serie[$i]["sceid"].",'totalAbandono');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadostransferencia"]."\" onblur=\"alteraTaxa('apaqtdafastadostransferencia',".$serie[$i]["sceid"].",'totalTransferencia');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  <td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" value=\"".(($dados[$j]["apaqtdmatriculafinal"] == "") ? 0 : $dados[$j]["apaqtdmatriculafinal"])."\" readonly=\"true\" >
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdaprovados"]."\"  onblur=\"alteraTaxa('apaqtdaprovados',".$serie[$i]["sceid"].",'totalAprovados');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdreprovados"]."\"  onblur=\"alteraTaxa('apaqtdreprovados',".$serie[$i]["sceid"].",'totalReprovados');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_aprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_aprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_reprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_reprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_abandono[".$serie[$i]["sceid"]."]\">
					  		".$taxa_abandono."
					  	</td>
					  </tr>";
		}
	}
		
		$div = ($totalMatInicial + $totalAdmitido - $totalTransferencia );
		if(($div != 0) && ( $div != '')) {
			@$totalTaxaAprovacao = str_replace('.',',',simec_number_format(($totalAprovados / ($div) * 100)));
			@$totalTaxaReprovacao = str_replace('.',',',simec_number_format(($totalReprovados / ($div ) * 100)));
			@$totaTaxaAbandono = str_replace('.',',',simec_number_format(($totalAbandono / ($div ) * 100)));
		}
		else {
			$totalTaxaAprovacao = 0;
			$totalTaxaReprovacao = 0;
			$totaTaxaAbandono = 0;
		}
			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"totalMatInicial\" bgcolor=\"#DADADA\">".$totalMatInicial."</td>
					<td id=\"totalAdmitido\" bgcolor=\"#DADADA\">".$totalAdmitido."</td>
					<td id=\"totalAbandono\" bgcolor=\"#DADADA\">".$totalAbandono."</td>
					<td id=\"totalTransferencia\" bgcolor=\"#DADADA\">".$totalTransferencia."</td>
					<td id=\"totalMatFinal\" bgcolor=\"#DADADA\">".$totalMatFinal."</td>
					<td id=\"totalAprovados\" bgcolor=\"#DADADA\">".$totalAprovados."</td>
					<td id=\"totalReprovados\" bgcolor=\"#DADADA\">".$totalReprovados."</td>
					<td id=\"totalTaxaAprovacao\" bgcolor=\"#DADADA\">".$totalTaxaAprovacao."%</td>
					<td id=\"totalTaxaReprovacao\" bgcolor=\"#DADADA\">".$totalTaxaReprovacao."%</td>
					<td id=\"totaTaxaAbandono\" bgcolor=\"#DADADA\">".$totaTaxaAbandono."%</td>
			  </tr>";
	?>
</tbody>

</table>
</form>
<!--fim 12_2-->
<br>
<br>

<br>
<br>

<?
 
$sql = "SELECT 
			pdcid, pdeid, pdcdatainicio, pdecdatafim, pdcanoreferencia, 
			pdcdataatualizacao, date_part('month', pdcdatainicio) as mes_inicio,  
			date_part('month', pdecdatafim) as mes_fim
		FROM
		 pdeescola.periododisciplinacritica
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);
extract($dados);

if($dicid){
	$sql = "SELECT
			 *
			FROM
			 pdeescola.disciplinacritica
			WHERE
			 dicid = ".$dicid;
	$dados = (array) $db->pegaLinha($sql);
	extract($dados);
}

?>

<?=subCabecalho('13 - Disciplinas críticas'); ?>
<form action="" method="post" name="formulario">
<input type='hidden' name='dicid' value='<?=$dicid ?>' />
<input type='hidden' name='pdcid' value='<?=$pdcid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Ano de Referencia: :</td>
		<td width="60%">
		  		 <?php 
  		 $anoex   = $_SESSION['exercicio_atual'];
  		 $anoex2  = $anoex -1;
  		 $anoex3  = $anoex2 -1;
  		 $anoex4  = $anoex3 -1;
  		 $arrUrl  = $_SERVER['argv'][0];
	  	 $arrUrl  = explode("&", $arrUrl);
		 $arrItem = explode("/", $arrUrl[0]);	 
		 $indice  = (count($arrItem) - 1);
	  	 $ppritem = $arrItem[$indice];
  		 
  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
  		 					  FROM pdeescola.pdepreenchimento 
  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
  		 					  AND ppritem = '$ppritem'";
  		 
		 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
		 </select>  
		 
		<input type="hidden" name="pdcanoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Período:</td>
		<td>
			<select name="mes_inicio" >
				<option value="">Escolha...</option>
				<option value="01" <?if($mes_inicio == '1') echo 'selected';?>>JANEIRO</option>
				<option value="02" <?if($mes_inicio == '2') echo 'selected';?>>FEVEREIRO</option>
				<option value="03" <?if($mes_inicio == '3') echo 'selected';?>>MARÇO</option>
				<option value="04" <?if($mes_inicio == '4') echo 'selected';?>>ABRIL</option>
				<option value="05" <?if($mes_inicio == '5') echo 'selected';?>>MAIO</option>
				<option value="06" <?if($mes_inicio == '6') echo 'selected';?>>JUNHO</option>
				<option value="07" <?if($mes_inicio == '7') echo 'selected';?>>JULHO</option>
				<option value="08" <?if($mes_inicio == '8') echo 'selected';?>>AGOSTO</option>
				<option value="09" <?if($mes_inicio == '9') echo 'selected';?>>SETEMBRO</option>
				<option value="10" <?if($mes_inicio == '10') echo 'selected';?>>OUTUBRO</option>
				<option value="11" <?if($mes_inicio == '11') echo 'selected';?>>NOVEMBRO</option>
				<option value="12" <?if($mes_inicio == '12') echo 'selected';?>>DEZEMBRO</option>
			</select>
			&nbsp;&nbsp;à&nbsp;&nbsp;
			<select name="mes_fim" >
				<option value="">Escolha...</option>
				<option value="01" <?if($mes_fim == '1') echo 'selected';?>>JANEIRO</option>
				<option value="02" <?if($mes_fim == '2') echo 'selected';?>>FEVEREIRO</option>
				<option value="03" <?if($mes_fim == '3') echo 'selected';?>>MARÇO</option>
				<option value="04" <?if($mes_fim == '4') echo 'selected';?>>ABRIL</option>
				<option value="05" <?if($mes_fim == '5') echo 'selected';?>>MAIO</option>
				<option value="06" <?if($mes_fim == '6') echo 'selected';?>>JUNHO</option>
				<option value="07" <?if($mes_fim == '7') echo 'selected';?>>JULHO</option>
				<option value="08" <?if($mes_fim == '8') echo 'selected';?>>AGOSTO</option>
				<option value="09" <?if($mes_fim == '9') echo 'selected';?>>SETEMBRO</option>
				<option value="10" <?if($mes_fim == '10') echo 'selected';?>>OUTUBRO</option>
				<option value="11" <?if($mes_fim == '11') echo 'selected';?>>NOVEMBRO</option>
				<option value="12" <?if($mes_fim == '12') echo 'selected';?>>DEZEMBRO</option>
			</select>

		</td>
	</tr>
	<tr>
		<td colspan=2 class="SubTituloDireita">&nbsp;</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Disciplina:</td>
		<td>
		<?
			echo $db->monta_combo('tdiid',
                                "SELECT  tdiid AS codigo, 
									tdidescricao AS descricao
								FROM pdeescola.tipodisciplina
								ORDER BY tdidescricao",
                                'S', 'Escolha...', '', '', '', '', 'N', 'tdiid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Série/Ciclo:</td>
		<td>
		<?
			$sql_serie_ciclo = "SELECT 
									sce.sceid AS codigo,
									(CASE sce.scetipo 
									WHEN 'C' THEN sce.scedescricao
									WHEN 'S' THEN sce.scedescricao || ' (' || tme.tmedescricao || ')'
									END) AS descricao
								FROM 
									pdeescola.seriecicloescolar sce
								INNER JOIN
									pdeescola.tiponivelmodalidadeensino tme ON tme.tmeid = sce.tmeid
								ORDER BY
									sce.sceid";
						
			echo $db->monta_combo('sceid', $sql_serie_ciclo, 'S', 'Escolha...', '', '', '', '', 'N', 'sceid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Turma:</td>
		<td><?= campo_texto( 'dicturma', 'N', 'S', '', 70, 10, '', ''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Turno:</td>
		<td>
		<?
			echo $db->monta_combo('titid',
                                'SELECT  titid AS codigo, 
										 titdescricao AS descricao
								FROM pdeescola.tipoturno
								ORDER BY titdescricao',
                                'S', 'Escolha...', '', '', '', '', 'N', 'titid', true);
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Taxa de Reprovação:</td>
		<td><?= campo_texto( 'dictaxareprovacao', 'N', 'S', '', 4, 3, '', '', '', '', 0, 'onkeypress="return somenteNumeros(event);"'); ?> %</td>
	</tr>
	<?php //echo $cForm->montarbuttons("validaForm()");?>
	<tr>
		<td><br></td>
	</tr>			
</table>
<?
if($pdcid){
	$sql = 'SELECT
			\'<center> \' 
			|| 
			\'  \'
			as acao,
			td.tdidescricao,
			(CASE sc.scetipo 
			WHEN \'C\' THEN sc.scedescricao
			WHEN \'S\' THEN sc.scedescricao || \' (\' || tme.tmedescricao || \')\'
			END) AS scedescricao,
			dc.dicturma, 
			tp.titdescricao, 
			\'<center>\'||dc.dictaxareprovacao||\'%</center>\'
			FROM 
				pdeescola.disciplinacritica dc
			INNER JOIN 
				pdeescola.tipodisciplina td ON td.tdiid = dc.tdiid
			INNER JOIN 
				pdeescola.seriecicloescolar sc ON sc.sceid = dc.sceid
			INNER JOIN
				pdeescola.tiponivelmodalidadeensino tme ON tme.tmeid = sc.tmeid
			INNER JOIN 
				pdeescola.tipoturno tp ON tp.titid = dc.titid
			WHERE
			 dc.pdcid = '.$pdcid.'
			order by dc.dicid
			';
	
	
	$cabecalho = array( "Ação", 
						"Disciplinas",
						"Série/Ciclo",
						"Turma",
						"Turno",
						"Taxa de Reprovação");
	
	$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
}

?>
<br>
<br>

<br>
<br>
<?
$arCargos = $db->carregar( 'select cafid, cafdescricao from pdeescola.cargofuncao order by cafid' );

$TotalQuantidade  = 0;
$TotalFunComp     = 0;
$TotalFunInComp   = 0;
$TotalMedComp     = 0;
$TotalMedInComp   = 0;
$TotalMedOutra    = 0;
$TotalSupComp     = 0;
$TotalSupInComp   = 0;
$TotalSupOutra    = 0;
$TotalEspecComp   = 0;
$TotalEspecInComp = 0;
$TotalMestComp    = 0;
$TotalMestInComp  = 0;


$sqlTotais = "SELECT
		  *
		FROM
		 pdeescola.pessoaltecnicoformacao
		WHERE
		 pdeid = ".$pdeid;
$rsTotais = $db->carregar( $sqlTotais );
for( $x = 0; $x < count( $rsTotais ); $x ++)
{
	$TotalQuantidade 	+= $rsTotais[$x]['ptfquantidade']; 
    $TotalFunComp    	+= $rsTotais[$x]['ptfqtdensinofundfcompleto'];
    $TotalFunInComp  	+= $rsTotais[$x]['ptfqtdensinofundfincompleto'];
    $TotalMedComp    	+= $rsTotais[$x]['ptfqtdensinomediocompleto'];
    $TotalMedInComp  	+= $rsTotais[$x]['ptfqtdensinomedioincompleto'];
    $TotalMedOutra   	+= $rsTotais[$x]['ptfqtdensinomediooutrahab'];
    $TotalSupComp    	+= $rsTotais[$x]['ptfqtdensinosuperiorcompleto'];
    $TotalSupInComp  	+= $rsTotais[$x]['ptfqtdensinosuperiorincompleto'];
    $TotalSupOutra   	+= $rsTotais[$x]['ptfqtdensinosuperiorsemlic'];
    $TotalEspecComp  	+= $rsTotais[$x]['ptfqtdespecializacaocompleto'];
    $TotalEspecInComp	+= $rsTotais[$x]['pftqtdespecializacaoincompleto'];
    $TotalMestComp  	+= $rsTotais[$x]['pftqtdmestradocompleto'];
    $TotalMestInComp  	+= $rsTotais[$x]['pftqtdmestradoincompleto'];
}

#dbg( $dadosBanco, true );
foreach( $dadosBanco as $arBanco ){
    $arDadosBanco[$arBanco['cafid']]['ptfid'] = $arBanco['ptfid'];
    $arDadosBanco[$arBanco['cafid']]['pdeid'] = $arBanco['pdeid'];
    $arDadosBanco[$arBanco['cafid']]['cafid'] = $arBanco['cafid'];
    $arDadosBanco[$arBanco['cafid']]['ptfquantidade'] = $arBanco['ptfquantidade'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinofundfcompleto'] = $arBanco['ptfqtdensinofundfcompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinofundfincompleto'] = $arBanco['ptfqtdensinofundfincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomediocompleto'] = $arBanco['ptfqtdensinomediocompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomedioincompleto'] = $arBanco['ptfqtdensinomedioincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinomediooutrahab'] = $arBanco['ptfqtdensinomediooutrahab'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorcompleto'] = $arBanco['ptfqtdensinosuperiorcompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorincompleto'] = $arBanco['ptfqtdensinosuperiorincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdensinosuperiorsemlic'] = $arBanco['ptfqtdensinosuperiorsemlic'];
    $arDadosBanco[$arBanco['cafid']]['ptfqtdespecializacaocompleto'] = $arBanco['ptfqtdespecializacaocompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdespecializacaoincompleto'] = $arBanco['pftqtdespecializacaoincompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdmestradocompleto'] = $arBanco['pftqtdmestradocompleto'];
    $arDadosBanco[$arBanco['cafid']]['pftqtdmestradoincompleto'] = $arBanco['pftqtdmestradoincompleto'];
    $arDadosBanco[$arBanco['cafid']]['ptfanoreferencia'] = $arBanco['ptfanoreferencia'];
    $arDadosBanco[$arBanco['cafid']]['ptfdataatualizacao'] = $arBanco['ptfdataatualizacao'];
    
	# Soma totais
	
    /*
	echo $pdeid;
	echo 'Quantidade  = :'.$TotalQuantidade;
    echo '<br>';
    
    $TotalQuantidade 	+= $arBanco['ptfquantidade']; 
    $TotalQuantidade 	+= $dadosbanco['ptfquantidade']; 
    $TotalFunComp    	+= $arBanco['ptfqtdensinofundfcompleto'];
    $TotalFunInComp  	+= $arBanco['ptfqtdensinofundfincompleto'];
    $TotalMedComp    	+= $arBanco['ptfqtdensinomediocompleto'];
    $TotalMedInComp  	+= $arBanco['ptfqtdensinomedioincompleto'];
    $TotalMedOutra   	+= $arBanco['ptfqtdensinomediooutrahab'];
    $TotalSupComp    	+= $arBanco['ptfqtdensinosuperiorcompleto'];
    $TotalSupInComp  	+= $arBanco['ptfqtdensinosuperiorincompleto'];
    $TotalSupOutra   	+= $arBanco['ptfqtdensinosuperiorsemlic'];
    $TotalEspecComp  	+= $arBanco['ptfqtdespecializacaocompleto'];
    $TotalEspecInComp	+= $arBanco['pftqtdespecializacaoincompleto'];
    $TotalMestComp  	+= $arBanco['pftqtdmestradocompleto'];
    $TotalMestInComp  	+= $arBanco['pftqtdmestradoincompleto'];
	*/
}
unset( $dadosBanco );
#dbg( $arDadosBanco, true );

?>
<?=subCabecalho('14.1 - Pessoal Técnico de acordo com a formalização'); ?>
<table class="tabela" id="itensDeMatricula" border="0" align="center" cellpadding="0" cellspacing="2" width="100%" style="text-align: center">
  <tr>
    <th>Cargo / Função</th>
    <th>Quantidade</th>
    <th colspan="2">Ensino Fundamental</th>
    <th colspan="3">Ensino Médio</th>
    <th colspan="3">Ensino Superior</th>
    <th colspan="2">Especialização</th>
    <th colspan="2">Mestrado</th>
  </tr>
  <tr>
  <th>&nbsp;</th>
  <th colspan="3">&nbsp;</th>
  <th colspan="2">Habilitação Magisterio</th>
  <th rowspan="2">Outra<br> Habilitação</th>
  <th colspan="2">Com Licenciatura</th>
  <th rowspan="2">Sem<br> licenciatura</th>
  <th colspan="5">&nbsp;</th>
  </tr>
  <tr>
    <th>&nbsp;</th>
    <th>&nbsp;</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
    <th>Completo</th>
    <th>Incompleto</th>
  </tr>
  
<?php
foreach( $arCargos as $arCargo ) {
?>
  <tr>
    <th><?php echo $arCargo['cafdescricao']; ?></th>
    <td align="center"><?php echo campo_texto('arQuantidade['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, 'onblur="totalizar( \'spanTotalQuantidade\', \'arQuantidade\' );"', '', $arDadosBanco[$arCargo['cafid']]['ptfquantidade'] ); ?></td>
    <td align="center"><?php echo campo_texto('arFunComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalFunComp\', \'arFunComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinofundfcompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arFunInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalFunInComp\', \'arFunInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinofundfincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedComp\', \'arMedComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomediocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedInComp\', \'arMedInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomedioincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMedOutra['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMedOutra\', \'arMedOutra\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinomediooutrahab'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupComp\', \'arSupComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorcompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupInComp\', \'arSupInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arSupOutra['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalSupOutra\', \'arSupOutra\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdensinosuperiorsemlic'] ); ?></td>
    <td align="center"><?php echo campo_texto('arEspComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalEspComp\', \'arEspComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['ptfqtdespecializacaocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arEspInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalEspInComp\', \'arEspInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdespecializacaoincompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMestComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMestComp\', \'arMestComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdmestradocompleto'] ); ?></td>
    <td align="center"><?php echo campo_texto('arMestInComp['.$arCargo['cafid'].']', '', 'S' , '', 3, 3, '###', '', 'left', '', 0, ' onblur="totalizar( \'spanTotalMestInComp\', \'arMestInComp\' ); "', '', $arDadosBanco[$arCargo['cafid']]['pftqtdmestradoincompleto'] ); ?></td>
  </tr>
<?php } ?>
  <tr>
    <th><b>TOTAL</b></th>
    <td align="center"><span id="spanTotalQuantidade"><?php echo $TotalQuantidade; ?></span></td>
    <td align="center"><span id="spanTotalFunComp"><?php echo $TotalFunComp; ?></span></td>
    <td align="center"><span id="spanTotalFunInComp"><?php echo $TotalFunInComp; ?></span></td>
    <td align="center"><span id="spanTotalMedComp"><?php echo $TotalMedComp; ?></span></td>
    <td align="center"><span id="spanTotalMedInComp"><?php echo $TotalMedInComp; ?></span></td>
    <td align="center"><span id="spanTotalMedOutra"><?php echo $TotalMedOutra; ?></span></td>
    <td align="center"><span id="spanTotalSupComp"><?php echo $TotalSupComp; ?></span></td>
    <td align="center"><span id="spanTotalSupInComp"><?php echo $TotalSupInComp; ?></span></td>
    <td align="center"><span id="spanTotalSupOutra"><?php echo $TotalSupOutra; ?></span></td>
    <td align="center"><span id="spanTotalEspComp"><?php echo $TotalEspecComp; ?></span></td>
    <td align="center"><span id="spanTotalEspInComp"><?php echo $TotalEspecInComp; ?></span></td>
    <td align="center"><span id="spanTotalMestComp"><?php echo $TotalMestComp; ?></span></td>
    <td align="center"><span id="spanTotalMestInComp"><?php echo $TotalMestInComp; ?></span></td>
  </tr>
</table>
</form>
<br>
<br>

<br>
<br>
<?
/*----------------- Recupera dados ---------------------*/  
$sql="	SELECT  distinct 
				RDA.titid as turno,
				RCS.rcsalunodocente as alunoOUdocente,
				RS.tmeid as serie,
				RC.cicid as ciclo,
				RS.rasqtd as quantidadeserie,  
				RC.racqtd as quantidadeciclo,   
				RDA.radnaodocente as quantidadealunonaodocente
		FROM pdeescola.relacaoalunodocente RDA
		INNER JOIN pdeescola.radcicloserie RCS ON RCS.radid = RDA.radid 
		LEFT JOIN pdeescola.radciclo RC ON RC.rcsid = RCS.rcsid 
		LEFT JOIN pdeescola.radserie RS ON RS.rcsid = RCS.rcsid
		WHERE RDA.pdeid = ".$pdeid;

$dados =  $db->carregar($sql);
if(is_array($dados)){
	foreach( $dados as $dados){
		if( ($dados['serie'] != NULL) ){ // Se serie
			${"aluno[".$dados['turno']."][S][".$dados['alunooudocente']."][".$dados['serie']."]"} = $dados['quantidadeserie'];	
		}
		if($dados['ciclo']!= NULL){ // Se ciclo
			${"aluno[".$dados['turno']."][C][".$dados['alunooudocente']."][".$dados['ciclo']."]"} = $dados['quantidadeciclo'];
		}
		${"Nnaodocente[".$dados['turno']."]"} = $dados['quantidadealunonaodocente'];
	}
}
?> 
<?=subCabecalho('14.2 - Relação aluno / docente e aluno / não-docente'); ?>	
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<table class="tabela" id="relacao" border="0" align="center" cellpadding="2" cellspacing="1" width="100%">
  <tr>
    <th>Turno</th>
    <th colspan="8">Nº de alunos </th>
    <th colspan="8">Nº de docentes </th>
    <th rowspan="3">N° de não docente (R) </th>
    <th colspan="8">Relacao aluno /  docente </th>
    <th rowspan="3">Relacao aluno / não- docente </th>
  </tr>
  <tr>
    <th></th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
    <th>EI</th>
    <th colspan="5">EF</th>
    <th>EJA</th>
    <th>EM</th>
  </tr>
  <tr>
    <th></th>
    <th>Série / Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
    <th>Série / Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
    <th>Série /Ano</th>
    <th colspan="2">Serie / Ano</th>
    <th colspan="3">Ciclo</th>
    <th>Etapa</th>
    <th>Série / Ano</th>
  </tr>
  <tr>
    <th></th>
    <th>Pré<br>(A)</th>
    <th>Iniciais<br>(B)</th>
    <th>Finais<br>(C)</th>
    <th>1º<br>(D)</th>
    <th>2º<br>(E)</th>
    <th>3º<br>(F)</th>
    <th>1ª a 4ª<br>(G)</th>
    <th>1ª a 3ª<br>(H)</th>
    <th>Pré<br>(I)</th>
    <th>Iniciais<br>(J)</th>
    <th>Finais<br>(L)</th>
    <th>1º<br>(M)</th>
    <th>2º<br>(N)</th>
    <th>3º<br>(O)</th>
    <th>1º à 4º<br>(P)</th>
    <th>1º à 3º<br>(Q)</th>
    <th></th>
    <th>Pré<br>(A/I)</th>
    <th>Iniciais<br>(B/J)</th>
    <th>Finais<br>(C/L)</th>
    <th>1º<br>(D/M)</th>
    <th>2º<br>(E/N)</th>
    <th>3º<br>(F/O)</th>
    <th>1ª a 4ª<br>(G/P)</th>
    <th>1ª a 3ª<br>(H/Q)</th>
    <th>Soma do nº de alunos / R</th>
  </tr>
  <tr>
    <th>Mat.</th>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" " id="aluno['.TURNO_MATUTINO.'][S][A]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO1.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO2.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][A]['.CICLO3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][A]['.ENSINOMEDIO1A3.']" onblur="totalizar();"'); ?></td>
   
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.PRE.']" onblur="totalizar();" '); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO1.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO2.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][C][D]['.CICLO3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_MATUTINO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_MATUTINO.'][S][D]['.ENSINOMEDIO1A3.']" onblur="totalizar();"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_MATUTINO.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="Nnaodocente['.TURNO_MATUTINO.']" onblur="totalizar();"'); ?></td>
    
    <td><?=campo_texto('totalSerieMatPre', '', 'S' , '', 5, 100,'','', 'left', '', 0, 'id="totalSerieMatPre"'); ?></td>
    <td><?=campo_texto('totalSerieMatF1A4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatF1A4"'); ?></td>
    <td><?=campo_texto('totalSerieMatF5A8', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatF5A8"'); ?></td>
    <td><?=campo_texto('totalSerieMatC1', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatC1"'); ?></td>
    <td><?=campo_texto('totalSerieMatC2', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatC2"'); ?></td>
    <td><?=campo_texto('totalSerieMatC3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatC3"'); ?></td>
    <td><?=campo_texto('totalSerieMatJ1A4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatJ1A4"'); ?></td>
    <td><?=campo_texto('totalSerieMatM1A3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatM1A3"'); ?></td>
    <td><?=campo_texto('totalSerieMatNdocente', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieMatNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Vesp.</th>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][A]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, ' onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][A]['.ENSINOMEDIO1A3.']"'); ?></td>
    
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][C][D]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_VESPERTINO.'][S][D]['.ENSINOMEDIO1A3.']"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_VESPERTINO.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="Nnaodocente['.TURNO_VESPERTINO.']"'); ?></td>
    <td><?=campo_texto('totalSerieVesPre', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesPre"'); ?></td>
    <td><?=campo_texto('totalSerieVesF1a4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesF1a4"'); ?></td>
    <td><?=campo_texto('totalSerieVesF5a8', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesF5a8"'); ?></td>
    <td><?=campo_texto('totalSerieVesC1', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesC1"'); ?></td>
    <td><?=campo_texto('totalSerieVesC2', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesC2"'); ?></td>
    <td><?=campo_texto('totalSerieVesC3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesC3"'); ?></td>
    <td><?=campo_texto('totalSerieVesJ1a4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesJ1a4"'); ?></td>
    <td><?=campo_texto('totalSerieVesM1a3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesM1a3"'); ?></td>
    <td><?=campo_texto('totalSerieVesNdocente', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieVesNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Not.</th>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'id="aluno['.TURNO_NOTURNO.'][S][A]['.PRE.']" onblur="totalizar();"');?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][A]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][A]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][A]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][A]['.ENSINOMEDIO1A3.']"'); ?></td>
    
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.PRE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.PRE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL5A8SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.FUNDAMENTAL5A8SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO1.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO1.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO2.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO2.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][C][D]['.CICLO3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][C][D]['.CICLO3.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.ENSINODEJOVENS1A4SERIE.']"'); ?></td>
    <td><?=campo_texto('aluno['.TURNO_NOTURNO.'][S][D]['.ENSINOMEDIO1A3.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="aluno['.TURNO_NOTURNO.'][S][D]['.ENSINOMEDIO1A3.']"'); ?></td>
    <td><?=campo_texto('Nnaodocente['.TURNO_NOTURNO.']', '', 'S' , '', 5, 100, '##########', '', 'left', '', 0, 'onblur="totalizar();" id="Nnaodocente['.TURNO_NOTURNO.']"');?></td>
     <td><?=campo_texto('totalSerieNotPre', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotPre"'); ?></td>
    <td><?=campo_texto('totalSerieNotF1a4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotF1a4"'); ?></td>
    <td><?=campo_texto('totalSerieNotF5a8', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotF5a8"'); ?></td>
    <td><?=campo_texto('totalSerieNotC1', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotC1"'); ?></td>
    <td><?=campo_texto('totalSerieNotC2', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotC2"'); ?></td>
    <td><?=campo_texto('totalSerieNotC3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotC3"'); ?></td>
    <td><?=campo_texto('totalSerieNotJ1a4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotJ1a4"'); ?></td>
    <td><?=campo_texto('totalSerieNotM1a3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotM1a3"'); ?></td>
    <td><?=campo_texto('totalSerieNotNdocente', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalSerieNotNdocente"'); ?></td>
  </tr>
  <tr>
    <th>Total</th>
    <td><?=campo_texto('totalCol1', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol1"'); ?></td>
    <td><?=campo_texto('totalCol2', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol2"'); ?></td>
    <td><?=campo_texto('totalCol3', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol3"'); ?></td>
    <td><?=campo_texto('totalCol4', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol4"'); ?></td>
    <td><?=campo_texto('totalCol5', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol5"'); ?></td>
    <td><?=campo_texto('totalCol6', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol6"'); ?></td>
    <td><?=campo_texto('totalCol7', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol7"'); ?></td>
    <td><?=campo_texto('totalCol8', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol8"'); ?></td>
    <td><?=campo_texto('totalCol9', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol9"'); ?></td>
    <td><?=campo_texto('totalCol10', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol10"'); ?></td>
    <td><?=campo_texto('totalCol11', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol11"'); ?></td>
    <td><?=campo_texto('totalCol12', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol12"'); ?></td>
    <td><?=campo_texto('totalCol13', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol13"'); ?></td>
    <td><?=campo_texto('totalCol14', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol14"'); ?></td>
    <td><?=campo_texto('totalCol15', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol15"'); ?></td>
    <td><?=campo_texto('totalCol16', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol16"'); ?></td>
    <td><?=campo_texto('totalCol17', '', 'N' , '', 5, 100, '', '', 'left', '', 0, 'id="totalCol17"'); ?></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
    <td></td>
  </tr>
</table>
</form>
<br>
<br>

<br>
<br>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.turmasemprofessor
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('14.3 - Há turmas ou disciplinas sem professor? Se a resposta for afirmativa, especifique.'); ?>	
<form action="" method="post" name="formulario">
<input type='hidden' name='tspid' value='<?=$tspid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Há turmas ou disciplinas sem professor?</td>
		<td>
		<input type="radio" class="" name="tspsemprofessor" value="t" onclick="document.getElementById('tr_resposta').style.display = ''; document.formulario.tspresposta.value='<?=$tspresposta?>';" <?if($tspsemprofessor == 't') echo 'checked';?>> Sim
		&nbsp;&nbsp;&nbsp;
		<input type="radio" class="" name="tspsemprofessor" value="f" onclick="document.getElementById('tr_resposta').style.display = 'none'; document.formulario.tspresposta.value='';" <?if($tspsemprofessor == 'f') echo 'checked';?>> Não
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ano:</td>
		<td width="60%">
		<?php
		$ano = $tspid ? $tspanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
		if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
		echo $ano;
		?>
		<input type="hidden" name="tspanoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr id="tr_resposta" style="display:'none';">
		<td class="SubTituloDireita" valign="top">Resposta:</td>
		<td>
		<?= campo_textarea( 'tspresposta', 'S', 'S', '', 80, 10, 350,'' ); ?>
		</td>
	</tr> 
</table>
</form>
<br>
<br>

<br>
<br>
<?
/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.formaselecaodiretor
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>


<?=subCabecalho('15 - Fontes e destinação dos recursos utilizados pela Escola.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" id="tabela">
	<tr bgcolor="#dfdfdf">
		<td align="center">
			<b>Tipo Esfera</b>
		</td>
		<td align="center">
			<b>Fonte</b>
		</td>
		<td align="center">
			<b>Aperfeiçoamento de pessoal</b>
		</td>
		<td align="center">
			<b>Materiais Instrucionais</b>
		</td>
		<td align="center">
			<b>Manutenção</b>
		</td>
		<td align="center">
			<b>Outros</b>
		</td>
		<td align="center">
			<b>Total (R$)</b>
		</td>
	</tr>
	<?

		//pega Esferas
		$sql = sprintf("select tieid, tiedescricao 
				from pdeescola.tipoesfera 
				order by tieid"
		);
		$Esferas = $db->carregar( $sql );
		$Esferas = $Esferas ? $Esferas : array();
		
		$cor = '';
		
		
		
		//gambi
		/*
		echo("<pre>");
		echo("esferas");
		print_r( $Esferas );
		echo("</pre>");
		echo("<br>");
		*/
		for( $i = 0; $i < count($Esferas); $i++)
		{
			
			$query = "select forid, fordescricao
						from pdeescola.fonterecurso 
						where tieid = '{$Esferas[$i]['tieid']}'
						order by forid";
											
			$rspega = $db->carregar( $query );
			/*
			echo("<pre>");
			echo("fontes");
			print_r( $rspega );
			echo("</pre>");
			echo("<br>");
			*/
			for( $l = 0; $l <count( $rspega ); $l++)
			{
			
				$query2 ="select fdraperfeicoamentopessoal as soma_geral2, 
										fdrmaterialinstitucional as soma_geral3, 
       									fdrmanutencao as soma_geral4, 
       									fdroutros as soma_geral5
       									from pdeescola.fontedestinacaorecurso
       							where pdeid = $pdeid
								and forid= '{$rspega[$l]['forid']}'";
				//echo $query2;
 
				$fontessoma = $db->carregar( $query2 );
				/*
			 	echo("<pre>");
				echo("fontes");
				print_r( $fontessoma );
				echo("</pre>");
				echo("<br>");
				die();
				*/
				for( $j = 0; $j< count($fontessoma); $j++ )
				{
					if( count( $fontessoma) > 0)
					{	
						
						$soma_geral2 += $fontessoma[$j]['soma_geral2'];
						$soma_geral3 += $fontessoma[$j]['soma_geral3'];
						$soma_geral4 += $fontessoma[$j]['soma_geral4'];
						$soma_geral5 += $fontessoma[$j]['soma_geral5']; 
					}
				}
				 
			}
			//die();
		}
		
		//end gambi			
		
		
		foreach ( $Esferas as $Esferas2 ) :
			$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;

			
			//pega Fonte
			$sql = sprintf("select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = %d
					order by forid",
					$Esferas2['tieid']
			);
			$Fontes = $db->carregar( $sql );
			$Fontes = $Fontes ? $Fontes : array();
			
			
			$ct=1;
			$total1 = '';
			$total2 = '';
			$total3 = '';
			$total4 = '';
			$total5 = '';
			foreach ( $Fontes as $Fontes2 ) :	
				
						$sql = sprintf("select fdraperfeicoamentopessoal as valor2, 
										fdrmaterialinstitucional as valor3, 
       									fdrmanutencao as valor4, 
       									fdroutros as valor5
       									from pdeescola.fontedestinacaorecurso
       							where pdeid=%d
								and forid=%d
								 GROUP BY fdraperfeicoamentopessoal, fdrmaterialinstitucional, fdrmanutencao, fdroutros
								",
							$pdeid,
							$Fontes2['forid']
						);
						$dados = (array) $db->pegaLinha($sql);
						
						
						$valor1 = '';
						$valor2 = '';
						$valor3 = '';
						$valor4 = '';
						$valor5 = '';

						//CAPTURANDO SOMA TOTAL PARA PORCENTAGEM PARCIAL
						 
						
						if($dados['valor2']){
							$valor2 = simec_number_format($dados['valor2'],2,',','.');
							$total2 = $total2 + $dados['valor2'];
							$total_geral2 = $total_geral2 + $dados['valor2'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
						
						    //$geral_parcial2 = ($total2 * 100)/ $soma_geral2;
						    
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial2 = ( $valor2 / $totparcial ) * 100;
							}							
							$valor1 = $valor1 + $dados['valor2'];
								  
							
						}
						if($dados['valor3']){
							$valor3 = simec_number_format($dados['valor3'],2,',','.');
							$total3 = $total3 + $dados['valor3'];
							$total_geral3 = $total_geral3 + $dados['valor3'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
						
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial3 = ( $valor3 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor3'];
							
							 
						}
						if($dados['valor4']){
							$valor4 = simec_number_format($dados['valor4'],2,',','.');
							$total4 = $total4 + $dados['valor4'];
							$total_geral4 = $total_geral4 + $dados['valor4'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial4 = ( $valor4 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor4'];
							 
						}
						if($dados['valor5']){
							$valor5 = simec_number_format($dados['valor5'],2,',','.');
							$total5 = $total5 + $dados['valor5'];
							$total_geral5 = $total_geral5 + $dados['valor5'];
							$totparcial = ($dados['valor2'] + $dados['valor3'] + $dados['valor4'] + $dados['valor5']);
							if((  $totparcial != 0 ) && ( $totparcial != ''))
							{
								$geral_parcial5 = ( $valor5 / $totparcial ) * 100;	
							}
							$valor1 = $valor1 + $dados['valor5'];
							 
						} 
						if($valor1){
							$total1 = $total1 + $valor1;
							$total_geral1 = $total_geral1 + $valor1;
							$valor1 = simec_number_format($valor1,2,',','.');
							
							$soma_total = ( $soma_geral2 + $soma_geral3 + $soma_geral4 + $soma_geral5 );
							
							if((  $soma_total != 0 ) && ( $soma_total != ''))
							{
								$geral_parcial1 = ($total1 * 100)/ $soma_total;
							}
							
						}
						//echo $soma_total;
//				echo $totparcial;    
//				echo '<br>';
//				 
				
				?>
				<tr bgcolor="<?= $cor ?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';"><!-- onmouseover="this.style.backgroundColor='#ffffcc';"> -->
					<?if($cod_esfera != $Esferas2['tieid']){?>
						<td align="left" rowspan="<?=count($Fontes)+2?>" id="tdr_<?=$Esferas2['tieid']?>">
							<?= $Esferas2['tiedescricao'];?>
						</td>
					<?}?>
					<td align="center" >
						<?= strtoupper($Fontes2['fordescricao']); ?>
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor2?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v2_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();" onblur="calcula('v2_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v3_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v3_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor3?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v3_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); "onblur="calcula('v3_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v4_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v4_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor4?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v4_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); "onblur="calcula('v4_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();">
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v5_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v5_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor5?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v5_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual(); " onblur="calcula('v5_','<?=$Esferas2['tieid']?>','<?=$Fontes2['forid']?>'); calculaPercentual();">
					</td>
					<td align="center" >
						<input disabled class='CampoEstilo' type='text' name='v1_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v1_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor1?>' style='text-align:right;' >
					</td>
				</tr>
				<?

				if($ct == count($Fontes)){
				?>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt2_<?=$Esferas2['tieid']?>' id='vt2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt3_<?=$Esferas2['tieid']?>' id='vt3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total3,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt4_<?=$Esferas2['tieid']?>' id='vt4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total4,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt5_<?=$Esferas2['tieid']?>' id='vt5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total5,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt1_<?=$Esferas2['tieid']?>' id='vt1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total1,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>%Total Geral:</b></td>
						<?php
						/*
						echo $total2.'<br>';
						echo $total_geral2.'<br>';
						echo $soma_geral2.'<br>';
						echo("<br>");
						*/ 
						if( $totparcial == '' )
						{
							$geral_parcial2 = 0; 
							$geral_parcial3 = 0; 
							$geral_parcial4 = 0; 
							$geral_parcial5 = 0;
						}
						?>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg2_<?=$Esferas2['tieid']?>' id='vtg2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial2,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg3_<?=$Esferas2['tieid']?>' id='vtg3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial3,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg4_<?=$Esferas2['tieid']?>' id='vtg4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial4,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg5_<?=$Esferas2['tieid']?>' id='vtg5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial5,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg1_<?=$Esferas2['tieid']?>' id='vtg1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($geral_parcial1,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<!-- 
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>% SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp1_<?=$Esferas2['tieid']?>' id='vp1_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp2_<?=$Esferas2['tieid']?>' id='vp2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp3_<?=$Esferas2['tieid']?>' id='vp3_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp4_<?=$Esferas2['tieid']?>' id='vp4_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp5_<?=$Esferas2['tieid']?>' id='vp5_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					 -->
				<?
				}

				$ct=$ct+1;
				$cod_esfera = $Esferas2['tieid'];

			endforeach;
			
			//echo $geral_parcial2.'<br>';
			
			$g2 += $geral_parcial2;
			$g3 += $geral_parcial3;
			$g4 += $geral_parcial4;
			$g5 += $geral_parcial5;
			
			
			
			if( $total_geral1 != 0)
			{
			  	$vttg2 = ($total_geral2 * 100)/ $total_geral1; 
				$vttg3 = ($total_geral3 * 100)/ $total_geral1;
				$vttg4 = ($total_geral4 * 100)/ $total_geral1;
				$vttg5 = ($total_geral5 * 100)/ $total_geral1;
			}
/*			
			if( $total1 != 0)
			{
			  	$vttg2 = ($total2 / $total1) * 100; 
				$vttg3 = ($total3 / $total1) * 100;
				$vttg4 = ($total4 / $total1) * 100;
				$vttg5 = ($total5 / $total1) * 100;
			}
*/
		 
		//die();
		endforeach;
		
	?>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot2' id='tot2' size='17' maxlength='14' value='<?=simec_number_format($total_geral2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot3' id='tot3' size='17' maxlength='14' value='<?=simec_number_format($total_geral3,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot4' id='tot4' size='17' maxlength='14' value='<?=simec_number_format($total_geral4,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot5' id='tot5' size='17' maxlength='14' value='<?=simec_number_format($total_geral5,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot1' id='tot1' size='17' maxlength='14' value='<?=simec_number_format($total_geral1,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>%TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg2' id='totg2' size='17' maxlength='14' value='<?=simec_number_format($vttg2,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg3' id='totg3' size='17' maxlength='14' value='<?=simec_number_format($vttg3,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg4' id='totg4' size='17' maxlength='14' value='<?=simec_number_format($vttg4,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg5' id='totg5' size='17' maxlength='14' value='<?=simec_number_format($vttg5,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg1' id='totg1' size='17' maxlength='14' value='<?=simec_number_format('100','',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<!-- 
	<tr bgcolor='lightblue'>
		<td colspan=2 align=center ><b>% TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per1' id='per1' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per2' id='per2' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per3' id='per3' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per4' id='per4' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per5' id='per5' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	-->
</table>

<br>
<br>

<br>
<br>
<?
/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.formaselecaodiretor
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>
</form>

<?=subCabecalho('16 - Previsão de recursos da escola para o ano corrente, segundo fontes.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" id="tabela">
	<tr bgcolor="#dfdfdf">
		<td align="center">
			<b>Tipo Esfera</b>
		</td>
		<td align="center">
			<b>Fonte</b>
		</td>
		<td align="center">
			<b>Total (R$)</b>
		</td>
	</tr>
	<?

		//pega Esferas
		$sql = sprintf("select tieid, tiedescricao 
				from pdeescola.tipoesfera 
				order by tieid"
		);
		$Esferas = $db->carregar( $sql );
		$Esferas = $Esferas ? $Esferas : array(); 
		$cor = '';
  
		//workaround 
		for( $i = 0; $i < count($Esferas); $i++)
		{ 
			$query = "select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = '{$Esferas[$i]['tieid']}'
					order by forid";
											
			$rspega = $db->carregar( $query );
			 
			for( $m = 0; $m <count( $rspega ); $m++)
			{ 
				$query2 ="select prevalortotal as valor2 
								from pdeescola.previsaorecursosescola 
								where pdeid= '$pdeid'
								and forid = '{$rspega[$m]['forid']}'";
				 
				$fontessoma = $db->carregar( $query2 );
				 
				for( $j = 0; $j< count($fontessoma); $j++ )
				{
					if( count( $fontessoma) > 0)
					{	 
						$soma_geral += $fontessoma[$j]['valor2']; 
					}
				} 
			} 
		} 		
		//end workaround	
		
		
		foreach ( $Esferas as $Esferas2 ) :
			$cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ;

			
			//pega Fonte
			$sql = sprintf("select forid, fordescricao
					from pdeescola.fonterecurso 
					where tieid = %d
					order by forid",
					$Esferas2['tieid']
			);
			$Fontes = $db->carregar( $sql );
			$Fontes = $Fontes ? $Fontes : array();
			
			
			$ct=1;
			$total2 = '';
			foreach ( $Fontes as $Fontes2 ) :	
				
						$sql = sprintf("select prevalortotal as valor2 
								from pdeescola.previsaorecursosescola 
								where pdeid=%d
								and forid=%d
								",
							$pdeid,
							$Fontes2['forid']
						);
						$dados = (array) $db->pegaLinha($sql);
						
						$valor2 = '';
						if($dados['valor2']){
							$valor2 = simec_number_format($dados['valor2'],2,',','.');
							$total2 = $total2 + $dados['valor2'];
							$total_geral2 = $total_geral2 + $dados['valor2'];
						}
				
				
				?>
				<tr bgcolor="<?= $cor ?>" onmouseout="this.style.backgroundColor='<?= $cor ?>';"><!-- onmouseover="this.style.backgroundColor='#ffffcc';"> -->
					<?if($cod_esfera != $Esferas2['tieid']){?>
						<td align="left" rowspan="<?=count($Fontes)+2?>" id="tdr_<?=$Esferas2['tieid']?>">
							<?= $Esferas2['tiedescricao'];?>
						</td>
					<?}?>
					<td align="center" >
						<?= strtoupper($Fontes2['fordescricao']); ?>
					</td>
					<td align="center" >
						<input class='CampoEstilo' type='text' name='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' id='v2_<?=$Esferas2['tieid']?>_<?=$Fontes2['forid']?>' size='17' maxlength='14' value='<?=$valor2?>' style='text-align:right;' onkeyup="this.value=mascaraglobal('###.###.###,##',this.value); calcula('v2_','<?=$Esferas2['tieid']?>');" onblur="calcula('v2_','<?=$Esferas2['tieid']?>');">
					</td>
				</tr>
				
				<?

				if($ct == count($Fontes)){
				?>
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vt2_<?=$Esferas2['tieid']?>' id='vt2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<tr bgcolor='#ffffcc'>
						<?php
						if( $total2 != '')
 						$total_porcent = ($total2 * 100)/$soma_geral;
						?>
						<td align=right ><b>%Total Geral:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vtg2_<?=$Esferas2['tieid']?>' id='vtg2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?=simec_number_format($total_porcent,'',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					<!-- 
					<tr bgcolor='#ffffcc'>
						<td align=right ><b>% SUBTOTAL:</b></td>
						<td align=center ><input disabled class='CampoEstilo' type='text' name='vp2_<?=$Esferas2['tieid']?>' id='vp2_<?=$Esferas2['tieid']?>' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
					</tr>
					 -->
				<?
				}
	
				$ct=$ct+1;
				$cod_esfera = $Esferas2['tieid'];

			endforeach;
			
			if( $total2 != '')
			$vttg = ($total2 * 100)/ $soma_geral;

		endforeach;

	?>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='tot2' id='tot2' size='17' maxlength='14' value='<?=simec_number_format($total_geral2,2,',','.');?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<tr bgcolor='lightblue'>
		<td colspan=2 align=right ><b>%TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='totg2' id='totg2' size='17' maxlength='14' value='<?=simec_number_format('100','',',','.');?>%' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	<!-- 
	<tr bgcolor='lightblue'>
		<td colspan=2 align=center ><b>% TOTAL GERAL:</b></td>
		<td align=center ><input disabled class='CampoEstilo' type='text' name='per2' id='per2' size='17' maxlength='14' value='<?//=$valor?>' style='text-align:right; BACKGROUND-COLOR:#ffffcc' ></td>
	</tr>
	-->
</table>
</form>
<br>
<br>

<br>
<br>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.escolaproveparaaluno
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('17 - A escola provê para os alunos'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='epaid' value='<?=$epaid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Merenda escolar?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('epamerendaescolar',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Serviço médico?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('epaservicomedico',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Serviço oftalmológico?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('epaservicooftalmologico',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Serviço odontológico?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('epaservicoodontologico',$sql,'S',0)
		?>
		</td>
	</tr> 	
</table>

<br>
<br>

<br>
<br>
<?
/*
 * Carrega dados
 */
if($mpaid){
	$sql = "SELECT
			 *
			FROM
			 pdeescola.medidaprojetoatual
			WHERE
			 mpaid = ".$mpaid;
	$dados = (array) $db->pegaLinha($sql);
	extract($dados);
}
?>

<?=subCabecalho('18 - Liste as medidas ou projetos que estão sendo implantados na atual administração'); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" valign="top">Medida/Projeto:</td>
		<td>
		<?= campo_textarea( 'mpamedidaprojeto', 'S', 'S', '', 80, 4, 500,'' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Critério de Eficácia:</td>
		<td>
		<?= campo_textarea( 'mpacriterioeficacia', 'S', 'S', '', 80, 4, 500,'' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Objetivo:</td>
		<td>
		<?= campo_textarea( 'mpaobjetivo', 'S', 'S', '', 80, 4, 500,'' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Resultado:</td>
		<td>
		<?= campo_textarea( 'mparesultado', 'S', 'S', '', 80, 4, 500,'' ); ?>
		</td>
	</tr>
	 				
	<tr>
		<td><br></td>
	</tr>			
</table>
</form>
<?
$sql = 'SELECT
		\'<center>\' 
		|| 
		\'</center>\'
		as acao,
		mpamedidaprojeto,
		mpacriterioeficacia,
		mpaobjetivo,
		mparesultado
		FROM
		 pdeescola.medidaprojetoatual a
		WHERE
		 pdeid = '.$pdeid.'
		order by mpaid
		';

$cabecalho = array( "Ação", 
					"Medida/Projeto",
					"Critério de Eficácia",
					"Objetivo",
					"Resultado");

$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
?>

<br>
<br>

<br>
<br>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.medidaprojetoimplantado
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('19 - Como a escola implantou as medidas ou projetos'); ?>	

<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='mpiid' value='<?=$mpiid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<input type='hidden' name='entid' value='<?=$entid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Redefiniu funções?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mpiredefiniufuncao',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Definiu responsabilidades?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mpidefiniuresponsabilidade',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Capacitou a equipe?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mpicapacitouequipe',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Outros (especificar)</td>
		<td><?= campo_texto( 'mpioutro', 'N', 'S', '', 80, 150, '', ''); ?></td>
	</tr> 					
</table>
</form>
<br>
<br>

<br>
<br>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.mudancamedidaprojeto
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('20 - O que mudou com a implantação das medidas ou projetos em relação à situação anterior (comprovar com fatos e dados)? Se alguma das respostas a seguir for afirmativa, especifique.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='mmpid' value='<?=$mmpid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<input type='hidden' name='entid' value='<?=$entid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">A organização da escola?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmporganizacaoescola',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">As taxas de abandono e reprovação?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmptaxaabandonoreprovacao',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Compromisso dos professores?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmpcompromissoprofessor',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ambiente escolar?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmpambienteescolar',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Envolvimento do pais?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmpenvolvimentopais',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nível de aprendizagem dos alunos?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmpnivelaprendizagemaluno',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Frequência dos alunos?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mmpfrequenciaaluno',$sql,'S',0)
		?>
		</td>
	</tr>								
	<tr>
		<td class="SubTituloDireita">Outros (especificar)</td>
		<td><?= campo_texto( 'mmpoutro', 'N', 'S', '', 80, 150, '', ''); ?></td>
	</tr> 			
</table>
</form>
<br>
<br>

<br>
<br>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.trabalhosecretariaeducacao
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('21 - Como a Secretaria de Educação trabalhou com a escola.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='tseid' value='<?=$tseid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<input type='hidden' name='entid' value='<?=$entid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Discutiu as medidas ou projetos?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('tsediscutiumedidaprojeto',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ouviu a opinião da escola?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('tseouviuopiniaoescola',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Forneceu apoio técnico?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('tseforneceuapoiotecnico',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Forneceu apoio financeiro?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('forneceuapoiofinanceiro',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Capacitou professores, diretores?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('capacitouprofessordiretor',$sql,'S',0)
		?>
		</td>
	</tr>								
	<tr>
		<td class="SubTituloDireita">Outros (especificar)</td>
		<td><?= campo_texto( 'tseoutro', 'N', 'S', '', 80, 150, '', ''); ?></td>
	</tr> 						
</table>

<br>
<br>

<br>
<br>
</form>
<?

$sql = "SELECT
		 *
		FROM
		 pdeescola.participacaoprofessorfuncionari
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>


<?=subCabecalho('22 - Qual tem sido a participação dos professores e demais funcionários nas medidas e projetos implementados pela escola.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='ppfid' value='<?=$ppfid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<input type='hidden' name='entid' value='<?=$entid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Discutem?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('ppfdiscute',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sugerem?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('ppfsugere',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Aprovam?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('ppfaprova',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Executam?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('ppfexecuta',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Avaliam?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('ppfavalia',$sql,'S',0)
		?>
		</td>
	</tr>								 									
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.participacaocolegiadoconselho
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('23 - Qual a participação do Colegiado/Conselho Escolar.'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='pccid' value='<?=$pccid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<input type='hidden' name='entid' value='<?=$entid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Discute?</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('pccdiscute',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Sugere?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('pccsugere',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Aprova?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('pccaprova',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Participa da elaboração dos projetos?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('pccparticipaelaboracao',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Participa da avaliação dos projetos?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('pccparticipaavaliacao',$sql,'S',0)
		?>
		</td>
	</tr> 									
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.medidaprojetoparceria
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('24 - A execução das medidas ou projetos envolveu parceria com outras instituições (ONGs, empresas, sindicatos etc.).'); ?>	
<form action="" method="post" name="formulario">
<input type='hidden' name='mppid' value='<?=$mppid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td width="40%" class="SubTituloDireita">A execução das medidas ou projetos envolveu parceria com outras instituições (ONGs, empresas, sindicatos etc.)?</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('mppenvolveu',$sql,'S',0)
		?>
		</td>
	</tr> 												
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.avaliarelacaosecretaria
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>


<?=subCabecalho('25 - Como a escola avalia sua relação com a Secretaria de Educação.'); ?>	
<form action="" method="post" name="formulario">
<input type='hidden' name='avsid' value='<?=$avsid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Como a escola avalia sua relação com a Secretaria de Educação?</td>
		<td>
		<?php
		$sql = " SELECT
				  coaid AS codigo,
				  coadescricao AS descricao
				from pdeescola.conceitoavaliacao
				ORDER BY
 				 coadescricao ";
		$db->monta_radio('coaid',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Explique:</td>
		<td>
		<?= campo_textarea( 'avsexplicacao', 'S', 'S', '', 80, 10, 300,'' ); ?>
		</td>
	</tr> 																
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.avaliarelacaocomunidade
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>


<?=subCabecalho('26 - Como a escola avalia sua relação com a comunidade.'); ?>	
<form action="" method="post" name="formulario">
<input type='hidden' name='avcid' value='<?=$avcid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Como a escola avalia sua relação com a comunidade?</td>
		<td>
		<?php
		$sql = " SELECT
				  coaid AS codigo,
				  coadescricao AS descricao
				from pdeescola.conceitoavaliacao
				ORDER BY
 				 coadescricao ";
		$db->monta_radio('coaid',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Explique:</td>
		<td>
		<?= campo_textarea( 'avcexplicacao', 'S', 'S', '', 80, 10, 300,'' ); ?>
		</td>
	</tr> 					
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.formaselecaodiretor
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>


<?=subCabecalho('27 - Qual a forma de seleção do diretor(a) para a escola?'); ?>	
<form action="" method="post" name="formulario" onsubmit="return validaForm();">
<input type='hidden' name='fscid' value='<?=$fscid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">a) Indicação:</td>
		<td width="60%">
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('fsdindicacao',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">b) Eleição:</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('fsdeleicao',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">c) Concurso:</td>
		<td>
		<?php
		$sql = " SELECT
				  't' AS codigo,
				  'Sim' AS descricao
				UNION
				 SELECT
				  'f' AS codigo,
				  'Não' AS descricao
				ORDER BY
 				 descricao DESC";
		$db->monta_radio('fscconcurso',$sql,'S',0)
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">d) Outros (especificar):</td>
		<td><?= campo_texto( 'fsaoutro', 'N', 'S', '', 80, 100, '', ''); ?></td>
	</tr> 								
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.rotatividade
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados); ?>


<?=subCabecalho('28 - A taxa de rotatividade dos professores e funcionários, nos últimos três anos, tem afetado o desempenho de escola?'); ?>	
<form action="" method="post" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita"> </td>
		<td>
		<input type="radio" class="" name="rotafetoudesempenho" value="t" onclick="document.getElementById('tr_resposta').style.display = ''; top.obrigatorio = 'true'; document.formulario.rotresposta.value='<?=$rotresposta?>';" <?if($rotafetoudesempenho == 't') echo 'checked';?>> Sim
		&nbsp;&nbsp;&nbsp;
		<input type="radio" class="" name="rotafetoudesempenho" value="f" onclick="document.getElementById('tr_resposta').style.display = 'none'; top.obrigatorio = 'falso'; document.formulario.rotresposta.value='';" <?if($rotafetoudesempenho == 'f') echo 'checked';?>> Não
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Ano:</td>
		<td width="60%">
		<?php
		$ano = $rotid ? $rotanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
		if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
		echo $ano;
		?>
		<input type="hidden" name="rotanoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr id="tr_resposta" style="display:'none';">
		<td class="SubTituloDireita" valign="top">Resposta:</td>
		<td>
		<?= campo_textarea( 'rotresposta', 'S', 'S', '', 80, 10, 350,'' ); ?>
		</td>
	</tr> 	
</table>
</form>
<br>
<br>

<br>
<br>
<?
$sql = "SELECT
		 *
		FROM
		 pdeescola.percentualjornadaintegral
		WHERE
		 pdeid = ".$pdeid;
$dados = (array) $db->pegaLinha($sql);

extract($dados);
?>

<?=subCabecalho('29 - Qual o percentual de professores com jornada de trabalho em tempo integral na escola, atualmente?'); ?>
<form action="" method="post" name="formulario">
<input type='hidden' name='pjiid' value='<?=$pjiid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Ano:</td>
		<td width="60%">
		<?php
		$ano = $pjiid ? $pjianoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
		if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
		echo $ano;
		?>
		<input type="hidden" name="pjianoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Resposta:</td>
		<td>
		<?= campo_textarea( 'pjiresposta', 'S', 'S', '', 80, 10, 350,'' ); ?>
		</td>
	</tr> 			
</table>
</form>

