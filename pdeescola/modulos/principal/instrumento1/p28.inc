<?php
pde_verificaSessao();
/*
 * 
Function, salvar/editar
 */
function salva ($rotid = null){
	global $db,$pdeid;
	
	if ($rotid){
		$sql = sprintf("UPDATE
					 	 pdeescola.rotatividade
					    SET
					     rotafetoudesempenho 		 = '%s',
					     rotanoreferencia  		 = %s,
					     rotresposta = '%s',
					     rotdataatualizacao  = %s
					    WHERE
					     rotid = %d",
					$_POST['rotafetoudesempenho'],
					$_POST['rotanoreferencia'] ? $_POST['rotanoreferencia'] : 'null',
					substr($_POST['rotresposta'], 0, 350),
					'now()',
					$rotid
				);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.rotatividade
					    (
					     pdeid,
					     rotafetoudesempenho,
					     rotanoreferencia,
					     rotresposta,
					     rotdataatualizacao
					    )VALUES(
					     %d,
						 '%s',
						 %s,
						 '%s',
						 '%s'
					    )",
					$pdeid,
					$_POST['rotafetoudesempenho'],
					$_POST['rotanoreferencia'] ? $_POST['rotanoreferencia'] : 'null',
					substr($_POST['rotresposta'], 0, 350),
					'now()'
				);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p28' AND
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
											 	ppritem = 'p28'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p28','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$rotid = $_POST['rotid'] ? $_POST['rotid'] : $_GET['rotid'];
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
$cForm->direcPag('salva( rotid )', array("rotid" => $rotid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($rotid);
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
		 pdeescola.rotatividade
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
<?=subCabecalho('28 - A taxa de rotatividade dos professores e funcionários, nos últimos três anos, tem afetado o desempenho da escola?'); ?>	
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
		<?= campo_textarea( 'rotresposta', 'S', 'S', '', 80, 10, 350,'id="rotresposta"' ); ?>
		</td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>			
</table>
<script type="text/javascript">
<?if($rotafetoudesempenho=='t') echo "document.getElementById('tr_resposta').style.display = '';";?>

d = document;
function validaForm(){
	var rotr = document.formulario.rotresposta.value.length ;
	
	if(rotr >= 351){
		alert('Texto muito grande. Limite máximo de 350 caracteres.');
		return false;
	} 
	if (!d.formulario.elements['rotafetoudesempenho'][0].checked && !d.formulario.elements['rotafetoudesempenho'][1].checked) {
		alert("Campo 'Há turmas ou disciplinas sem professor?' é Obrigatório.");
		return;
	}
	if( top.obrigatorio == 'true')
	{
		if (d.formulario.elements['rotresposta'].value == '') {
			alert("Campo 'Resposta' é Obrigatório.");
			return;
		}
	}

	d.formulario.submit();
	return true;
}
</script>	