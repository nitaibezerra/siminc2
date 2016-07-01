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
function salvaP17 ($tseid = null){
	global $db,$pdeid;
	#dbg($pdeid,1);
	if ($tseid){
		$sql = sprintf("UPDATE
					 	 pdeescola.trabalhosecretariaeducacao
					    SET
					     tsediscutiumedidaprojeto 	= '%s',
					     tseouviuopiniaoescola  	= '%s',
					     tseforneceuapoiotecnico 	= '%s',
					     forneceuapoiofinanceiro	= '%s',
					     capacitouprofessordiretor	= '%s',
					     tseoutro  					= '%s'
					    WHERE
					     tseid = %d",
					$_POST['tsediscutiumedidaprojeto'],
					$_POST['tseouviuopiniaoescola'],
					$_POST['tseforneceuapoiotecnico'],
					$_POST['forneceuapoiofinanceiro'],
					$_POST['capacitouprofessordiretor'],
					substr($_POST['tseoutro'], 0, 150 ),
					$tseid);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.trabalhosecretariaeducacao
					    (
					     pdeid,
					     tsediscutiumedidaprojeto,
					     tseouviuopiniaoescola,
					     tseforneceuapoiotecnico,
					     forneceuapoiofinanceiro,
					     capacitouprofessordiretor,
					     tseoutro
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s'					    
						)",
					$pdeid,
					$_POST['tsediscutiumedidaprojeto'],
					$_POST['tseouviuopiniaoescola'],
					$_POST['tseforneceuapoiotecnico'],
					$_POST['forneceuapoiofinanceiro'],
					$_POST['capacitouprofessordiretor'],
					substr($_POST['tseoutro'], 0, 150 ));
	}

	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p21' AND
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
											 	ppritem = 'p21'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p21','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$tseid = $_POST['tseid'] ? $_POST['tseid'] : $_GET['tseid'];
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
$cForm->direcPag('salvaP17( tseid )', array("tseid" => $tseid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salvaP17($tseid);
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
		 pdeescola.trabalhosecretariaeducacao
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
		$db->monta_radio('tsediscutiumedidaprojeto',$sql,$somenteLeitura,0)
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
		$db->monta_radio('tseouviuopiniaoescola',$sql,$somenteLeitura,0)
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
		$db->monta_radio('tseforneceuapoiotecnico',$sql,$somenteLeitura,0)
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
		$db->monta_radio('forneceuapoiofinanceiro',$sql,$somenteLeitura,0)
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
		$db->monta_radio('capacitouprofessordiretor',$sql,$somenteLeitura,0)
		?>
		</td>
	</tr>								
	<tr>
		<td class="SubTituloDireita">Outros (especificar)</td>
		<td><?= campo_texto( 'tseoutro', 'N', $somenteLeitura, '', 80, 150, '', ''); ?></td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>								
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	<?php if( $possuiProjetos ) { ?>
		if (!d.formulario.elements['tsediscutiumedidaprojeto'][0].checked && !d.formulario.elements['tsediscutiumedidaprojeto'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['tseouviuopiniaoescola'][0].checked && !d.formulario.elements['tseouviuopiniaoescola'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['tseforneceuapoiotecnico'][0].checked && !d.formulario.elements['tseforneceuapoiotecnico'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['forneceuapoiofinanceiro'][0].checked && !d.formulario.elements['forneceuapoiofinanceiro'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['capacitouprofessordiretor'][0].checked && !d.formulario.elements['capacitouprofessordiretor'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}
		d.formulario.submit();
		return true;
	<?
	}
	?>	
}
</script>	