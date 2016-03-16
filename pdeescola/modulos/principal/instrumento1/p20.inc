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
function salvaP16 ($mmpid = null){
	global $db,$pdeid;
	
	if ($mmpid){
		$sql = sprintf("UPDATE
					 	 pdeescola.mudancamedidaprojeto
					    SET
					     mmporganizacaoescola 	 	= '%s',
					     mmptaxaabandonoreprovacao  = '%s',
					     mmpcompromissoprofessor 	= '%s',
					     mmpambienteescolar			= '%s',
					     mmpenvolvimentopais		= '%s',
					     mmpnivelaprendizagemaluno	= '%s',
					     mmpfrequenciaaluno			= '%s',	
					     mmpoutro  					= '%s'
					    WHERE
					     mmpid = %d",
					$_POST['mmporganizacaoescola'],
					$_POST['mmptaxaabandonoreprovacao'],
					$_POST['mmpcompromissoprofessor'],
					$_POST['mmpambienteescolar'],
					$_POST['mmpenvolvimentopais'],
					$_POST['mmpnivelaprendizagemaluno'],
					$_POST['mmpfrequenciaaluno'],
					substr($_POST['mmpoutro'],1,150),
					$mmpid);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.mudancamedidaprojeto
					    (
					     pdeid,
					     mmporganizacaoescola,
					     mmptaxaabandonoreprovacao,
					     mmpcompromissoprofessor,
					     mmpambienteescolar,
					     mmpenvolvimentopais,
					     mmpnivelaprendizagemaluno,
					     mmpfrequenciaaluno,
					     mmpoutro
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s',
						 '%s'					    
						)",
					$pdeid,
					$_POST['mmporganizacaoescola'],
					$_POST['mmptaxaabandonoreprovacao'],
					$_POST['mmpcompromissoprofessor'],
					$_POST['mmpambienteescolar'],
					$_POST['mmpenvolvimentopais'],
					$_POST['mmpnivelaprendizagemaluno'],
					$_POST['mmpfrequenciaaluno'],
					substr($_POST['mmpoutro'],1,150));
	}

	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p20' AND
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
											 	ppritem = 'p20'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1','p20','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
 
		}
		$db->commit();
  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$mmpid = $_POST['mmpid'] ? $_POST['mmpid'] : $_GET['mmpid'];
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
$cForm->direcPag('salvaP16( mmpid )', array("mmpid" => $mmpid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salvaP16($mmpid);
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
		 pdeescola.mudancamedidaprojeto
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
		$db->monta_radio('mmporganizacaoescola',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmptaxaabandonoreprovacao',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmpcompromissoprofessor',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmpambienteescolar',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmpenvolvimentopais',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmpnivelaprendizagemaluno',$sql,$somenteLeitura,0)
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
		$db->monta_radio('mmpfrequenciaaluno',$sql,$somenteLeitura,0)
		?>
		</td>
	</tr>								
	<tr>
		<td class="SubTituloDireita">Outros (especificar)</td>
		<td><?= campo_texto( 'mmpoutro', 'N', $somenteLeitura, '', 80, 150, '', ''); ?></td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>					
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	
	<?php if( $possuiProjetos ) { ?>
		if (!d.formulario.elements['mmporganizacaoescola'][0].checked && !d.formulario.elements['mmporganizacaoescola'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmptaxaabandonoreprovacao'][0].checked && !d.formulario.elements['mmptaxaabandonoreprovacao'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmpcompromissoprofessor'][0].checked && !d.formulario.elements['mmpcompromissoprofessor'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmpambienteescolar'][0].checked && !d.formulario.elements['mmpambienteescolar'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmpenvolvimentopais'][0].checked && !d.formulario.elements['mmpenvolvimentopais'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmpnivelaprendizagemaluno'][0].checked && !d.formulario.elements['mmpnivelaprendizagemaluno'][1].checked) {
			alert('Todas as perguntas devem ser respondidas!');
			return false;
		}else if (!d.formulario.elements['mmpfrequenciaaluno'][0].checked && !d.formulario.elements['mmpfrequenciaaluno'][1].checked) {
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