<?php
pde_verificaSessao();
/*
 * 
Function, salvar/editar
 */
function salva ($fscid = null){
	global $db,$pdeid;
	
	if ($fscid){
		$sql = sprintf("UPDATE
					 	 pdeescola.formaselecaodiretor
					    SET
					     fsdindicacao 	= '%s',
					     fsdeleicao  	= '%s',
					     fscconcurso 	= '%s',
					     fsaoutro 		= '%s'
					    WHERE
					     fscid = %d",
					$_POST['fsdindicacao'],
					$_POST['fsdeleicao'],
					$_POST['fscconcurso'],
					$_POST['fsaoutro'],
					$fscid);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.formaselecaodiretor
					    (
					     pdeid,
					     fsdindicacao,
					     fsdeleicao,
					     fscconcurso,
					     fsaoutro
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s'					    
						)",
					$pdeid,
					$_POST['fsdindicacao'],
					$_POST['fsdeleicao'],
					$_POST['fscconcurso'],
					$_POST['fsaoutro']);
	}

	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p27' AND
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
											 	ppritem = 'p27'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p27','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$fscid = $_POST['fscid'] ? $_POST['fscid'] : $_SESSION['fscid'];
$pdeid = $_POST['pdeid'] ? $_POST['pdeid'] : $_SESSION['pdeid'];

if (!$pdeid){
	die('<script>
			alert(\'Faltam parametros!\nTente novamente.\');
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
$cForm->direcPag('salva( fscid )', array("fscid" => $fscid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($fscid);
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = \'?modulo=principal/instrumento1/p23&acao=A\';
		 </script>');
endif;
*/
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
	<? $cForm->montarbuttons("validaForm()");?>										
</table>
<script type="text/javascript">
d = document;
function validaForm(){

	if (!d.formulario.elements['fsdindicacao'][0].checked && !d.formulario.elements['fsdindicacao'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}else if (!d.formulario.elements['fsdeleicao'][0].checked && !d.formulario.elements['fsdeleicao'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}else if (!d.formulario.elements['fscconcurso'][0].checked && !d.formulario.elements['fscconcurso'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}
	d.formulario.submit();
	return true;	
}
</script>	