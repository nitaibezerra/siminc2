<?php
pde_verificaSessao();
/*----------------- Verificando Perfil ---------------------*/ 
$perfis = arrayPerfil();
$boSuperUsuario = in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis );
$boPerfilPerm = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));

/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );
$estadoPerm = (($estado == 76) || ($estado == 37));	
/*
 * 
Function, salvar/editar
 */
function salva ($mpaid = null)
{
	global $db,$pdeid;
  
	if( $_REQUEST['Update'])
	{  
						$sql = " UPDATE
							 	 		pdeescola.medidaprojetoatual
						 		 SET
						 				mpamedidaprojeto = '".substr($_POST['mpamedidaprojeto'], 0, 500 )."',
										mpacriterioeficacia ='".substr($_POST['mpacriterioeficacia'], 0, 500 )."',
										mpaobjetivo = '".substr($_POST['mpaobjetivo'], 0, 500 )."',
										mparesultado = '".substr($_POST['mparesultado'], 0, 500 )."'
							 	WHERE
							     		mpaid = '{$_REQUEST['Update']}'"; 
		$db->executar( $sql );
	 
		echo"
		<script>
			location.href='pdeescola.php?modulo=principal/instrumento1/p18&acao=A';
		</script>
		";
		
	}
	else
	{
		$sql = sprintf("INSERT INTO pdeescola.medidaprojetoatual
					    (
					     pdeid,
					     mpamedidaprojeto,
					     mpacriterioeficacia,
					     mpaobjetivo,
					     mparesultado
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s'
					    )",
					$pdeid,
					substr($_POST['mpamedidaprojeto'], 0, 500 ),
					substr($_POST['mpacriterioeficacia'], 0, 500 ),
					substr($_POST['mpaobjetivo'], 0, 500 ),
					substr($_POST['mparesultado'], 0, 500 )
				);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p18' AND
						  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
	
	$existe = $db->pegaUm("SELECT
							pepid
						 FROM
						 	pdeescola.pdeepf
						 WHERE
						 	pdeid = ".$pdeid." AND
						 	epfid = ".$epfid);
	
	if($existe == NULL) {
		$sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
		$db->executar($sql);
	}
		 
	$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p18'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p18','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();
  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$mpaid = $_POST['mpaid'] ? $_POST['mpaid'] : $_GET['mpaid'];
$pdeid = $_POST['pdeid'] ? $_POST['pdeid'] : $_SESSION['pdeid'];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

/***********************************************************
 * Componente que irá montar os butões do formulário
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serão carregados pelo 
 * 										   __construct "Padrões à classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   não precisa dar include do arquivo na página.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos métodos 
 * 																		1º parametro
 * 																		passa a função que será aplicada nos botões 
 * 																		"anterior, próximo, salvar anterior, salvar próximo e salvar".
 * 																		2º parametro
 * 																		será um array de "índice" igual ao parametro da funcão 
 * 																		passada no 1º parametro e o valor será o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulário "dentro da tabela ainda", este monta os butões.
 * 											  O parametro passado refere-se a função javascript que validará o formulário,
 * 											  devendo esta submeter o formulário. 
 * 
 ***********************************************************/

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva( mpaid )', array("mpaid" => $mpaid ) );

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST){
	salva($mpaid);
}else{
	if($_REQUEST['Excluir']){
		salva($mpaid, $_REQUEST['Excluir']);
	}
}
*/
if($_REQUEST['Excluir']) {
	$sql = "DELETE from
				 	 pdeescola.medidaprojetoatual
				    WHERE
				     mpaid = '{$_REQUEST['Excluir']}'";
	$db->executar( $sql );
	
	# Se não tiver registro deletamos da tabela preenchimento 
  	$sql = "select * from pdeescola.medidaprojetoatual WHERE pdeid = $pdeid";
  	
  	$arMedidaProjetoAtual = $db->carregar( $sql );
  	if(!is_array($arMedidaProjetoAtual)){
  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'p18'");
  		if($pdeanoreferencia){
  			$sql = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'p18' AND pdeanoreferencia = '".$pdeanoreferencia."'";
  			$db->executar($sql);
  		}
  	}
	
	$db->commit();
	
	echo"
	<script>
		location.href='pdeescola.php?modulo=principal/instrumento1/p18&acao=A';
	</script>
	";
	
}
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
if($_REQUEST['Update'])
{
	$sql = "SELECT
			 *
			FROM
			 pdeescola.medidaprojetoatual
			WHERE
			 mpaid = '{$_REQUEST['Update']}'";
	$dados = (array) $db->pegaLinha($sql);
	extract($dados);
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
?>
<?=cabecalhoPDE(); ?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formulario">
<?=subCabecalho('18 - Liste as medidas ou projetos que estão sendo implantados na atual administração'); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	
	<input type="hidden" name="update" id="update" value="0">
	<tr>
		<td class="SubTituloDireita" valign="top">Medida/Projeto:</td>
		<td>
		<?= campo_textarea( 'mpamedidaprojeto', 'S', 'S', '', 80, 4, 500,'id="mpamedidaprojeto"' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Critério de Eficácia:</td>
		<td>
		<?= campo_textarea( 'mpacriterioeficacia', 'S', 'S', '', 80, 4, 500,'id="mpacriterioeficacia"' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Objetivo:</td>
		<td>
		<?= campo_textarea( 'mpaobjetivo', 'S', 'S', '', 80, 4, 500,'id="mpaobjetivo"' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Resultado:</td>
		<td>
		<?= campo_textarea( 'mparesultado', 'S', 'S', '', 80, 4, 500,'id="mparesultado"' ); ?>
		</td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>					
	<tr>
		<td><br></td>
	</tr>			
</table>
<?
if( $estadoPerm || $boSuperUsuario )
{ 
	if( $boSuperUsuario || $boPerfilPerm )
	{
	$sql_estado = "'<center><img style=\"cursor:hand\" onclick=Atualizar(' || mpaid || ') src=\"/imagens/alterar.gif\" style=\"border:none\" alt=\"Alterar Item\" />
	'||' <img style=\"cursor:hand\" onclick=Excluir(' || mpaid || ') src=\"/imagens/excluir.gif\" style=\"border:none\" alt=\"Excluir item\" /></center>'
		as acao,";
	}else{
	$sql_estado = ' \'\' as acao,';
	}
}
else{
	$sql_estado = ' \'\' as acao,';
}

$sql = 'SELECT
		'.$sql_estado.'
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

<script type="text/javascript">

d = document;

function validaForm(){

	if (d.formulario.elements['mpamedidaprojeto'].value != '') {
		if (d.formulario.elements['mpacriterioeficacia'].value == '') {
			alert("Campo 'Critério de Eficácia' é Obrigatório.");
			return;
		}
	}
	if (d.formulario.elements['mpacriterioeficacia'].value != '') {
		if (d.formulario.elements['mpaobjetivo'].value == '') {
			alert("Campo 'Objetivo' é Obrigatório.");
			return;
		}
	}
	if (d.formulario.elements['mpaobjetivo'].value != '') {	
		if (d.formulario.elements['mparesultado'].value == '') {
			alert("Campo 'Resultado' é Obrigatório.");
			return;
		}
	}
	if (d.formulario.elements['mparesultado'].value != '') {	
		if (d.formulario.elements['mpamedidaprojeto'].value == '') {
			alert("Campo 'Medida/Projeto' é Obrigatório.");
			return;
		}
	}

	
	var mpamedidaprojeto = document.formulario.mpamedidaprojeto.value.length ;
	var mpacriterioeficacia = document.formulario.mpacriterioeficacia.value.length;
	var mpaobjetivo = document.formulario.mpaobjetivo.value.length ;
	var mparesultado = document.formulario.mparesultado.value.length;

	if( mpamedidaprojeto > 500 ){
		alert('Você excedeu o nº máximo de caracteres (500) para o campo Medida/Projeto.');
		return false;
	} 
	if( mpacriterioeficacia > 500 ){
		alert('Você excedeu o nº máximo de caracteres (500) para o campo Critério de Eficácia.');
		return false;
	} 
	if( mpaobjetivo > 500 ){
		alert('Você excedeu o nº máximo de caracteres (500) para o campo Objetivo.');
		return false;
	} 
	if( mparesultado > 500 ){
		alert('Você excedeu o nº máximo de caracteres (500) para o campo Resultado.');
		return false;
	} 
	 
	d.formulario.submit();
	return true;
}

function Atualizar(id)
{
	location.href="pdeescola.php?modulo=principal/instrumento1/p18&acao=A&Update="+id;
}

function Excluir(id)
{
	if(confirm('Deseja realmente excluir este item?'))
	{
		location.href="pdeescola.php?modulo=principal/instrumento1/p18&acao=A&Excluir="+id;
	}
}
</script>	