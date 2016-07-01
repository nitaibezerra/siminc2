<?php
pde_verificaSessao();
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$epaid = $_POST['epaid'] ? $_POST['epaid'] : $_GET['epaid'];
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
$cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid));

/*
 * 
Function, salvar/editar
 */
function salvaP13 ($epaid = null){
	global $db,$pdeid;

	if ($epaid){
		$sql = sprintf("UPDATE
					 	 pdeescola.escolaproveparaaluno
					    SET
					     epamerendaescolar 		 = '%s',
					     epaservicomedico  		 = '%s',
					     epaservicooftalmologico = '%s',
					     epaservicoodontologico  = '%s'
					    WHERE
					     epaid = %d",
					$_POST['epamerendaescolar'],
					$_POST['epaservicomedico'],
					$_POST['epaservicooftalmologico'],
					$_POST['epaservicoodontologico'],
					$epaid);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.escolaproveparaaluno
					    (
					     pdeid,
					     epamerendaescolar,
					     epaservicomedico,
					     epaservicooftalmologico,
					     epaservicoodontologico
					    )VALUES(
					     %d,
						 '%s',
						 '%s',
						 '%s',
						 '%s'
					    )",
					$pdeid,
					$_POST['epamerendaescolar'],
					$_POST['epaservicomedico'],
					$_POST['epaservicooftalmologico'],
					$_POST['epaservicoodontologico']);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p17' AND
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
											 	ppritem = 'p17'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p17','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
	 
		}
		$db->commit();
  
}



/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salvaP13($epaid);
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
		 pdeescola.escolaproveparaaluno
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
	<? $cForm->montarbuttons("validaForm()");?>				
</table>
</form>
<script type="text/javascript">
d = document;
function validaForm(){
	if (!d.formulario.elements['epamerendaescolar'][0].checked && !d.formulario.elements['epamerendaescolar'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}else if (!d.formulario.elements['epaservicomedico'][0].checked && !d.formulario.elements['epaservicomedico'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}else if (!d.formulario.elements['epaservicooftalmologico'][0].checked && !d.formulario.elements['epaservicooftalmologico'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}else if (!d.formulario.elements['epaservicoodontologico'][0].checked && !d.formulario.elements['epaservicoodontologico'][1].checked) {
		alert('Todas as perguntas devem ser respondidas!');
		return false;
	}
	d.formulario.submit();	
	return true;	
}
</script>	