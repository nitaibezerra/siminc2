<?php
pde_verificaSessao();
$possuiProjetos = verificaProjetos($_SESSION['pdeid']);
if( $possuiProjetos ){
	$somenteLeitura = 'S';
}else{
	$somenteLeitura = 'N';
}
/*
 * 
Function, salvar/editar
 */
function salvaP19 ($pccid = null){
	global $db,$pdeid;
	
	if ($pccid){
		$sql = sprintf("UPDATE
					 	 pdeescola.participacaocolegiadoconselho
					    SET
					     pccdiscute 			= '%s',
					     pccsugere  			= '%s',
					     pccaprova 				= '%s',
					     pccparticipaelaboracao	= '%s',
					     pccparticipaavaliacao	= '%s'
					    WHERE
					     pccid = %d",
					$_POST['pccdiscute'],
					$_POST['pccsugere'],
					$_POST['pccaprova'],
					$_POST['pccparticipaelaboracao'],
					$_POST['pccparticipaavaliacao'],
					$pccid);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.participacaocolegiadoconselho
					    (
					     pdeid,
					     pccdiscute,
					     pccsugere,
					     pccaprova,
					     pccparticipaelaboracao,
					     pccparticipaavaliacao
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s'					    
						)",
					$pdeid,
					$_POST['pccdiscute'],
					$_POST['pccsugere'],
					$_POST['pccaprova'],
					$_POST['pccparticipaelaboracao'],
					$_POST['pccparticipaavaliacao']);
	}

	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p23' AND
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
											 	ppritem = 'p23'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p23','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pccid = $_POST['pccid'] ? $_POST['pccid'] : $_SESSION['pccid'];
$pdeid = $_POST['pdeid'] ? $_POST['pdeid'] : $_SESSION['pdeid'];
$entid = $_POST['entid'] ? $_POST['entid'] : $_SESSION['entid'];

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
$cForm->direcPag('salvaP19( pccid )', array("pccid" => $pccid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salvaP19($pccid);
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = \'?modulo=principal/estrutura_avaliacao&acao=A&entid='.$entid.'\';
		 </script>');
endif;
*/

/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.participacaocolegiadoconselho
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
		$db->monta_radio('pccdiscute',$sql,$somenteLeitura,0)
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
		$db->monta_radio('pccsugere',$sql,$somenteLeitura,0)
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
		$db->monta_radio('pccaprova',$sql,$somenteLeitura,0)
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
		$db->monta_radio('pccparticipaelaboracao',$sql,$somenteLeitura,0)
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
		$db->monta_radio('pccparticipaavaliacao',$sql,$somenteLeitura,0)
		?>
		</td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>										
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	<?php if( $possuiProjetos ) { ?>
		if (!d.formulario.elements['pccdiscute'][0].checked && !d.formulario.elements['pccdiscute'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['pccsugere'][0].checked && !d.formulario.elements['pccsugere'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['pccaprova'][0].checked && !d.formulario.elements['pccaprova'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['pccparticipaelaboracao'][0].checked && !d.formulario.elements['pccparticipaelaboracao'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['pccparticipaavaliacao'][0].checked && !d.formulario.elements['pccparticipaavaliacao'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}
		d.formulario.submit();
		return true;	
	<?php 
	}
	?>
}
</script>	