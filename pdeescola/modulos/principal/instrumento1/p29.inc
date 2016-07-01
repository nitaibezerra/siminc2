<?php
pde_verificaSessao();
/*
 * 
Function, salvar/editar
 */
function salva ($pjiid = null){
	global $db,$pdeid;
	
	if ($pjiid){
		$sql = sprintf("UPDATE
					 	 pdeescola.percentualjornadaintegral
					    SET
					     pjianoreferencia  		 = %s,
					     pjiresposta = '%s',
					     pjidataatualizacao  = %s
					    WHERE
					     pjiid = %d",
					$_POST['pjianoreferencia'] ? $_POST['pjianoreferencia'] : 'null',
					substr($_POST['pjiresposta'],0,350),
					'now()',
					$pjiid
				);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.percentualjornadaintegral
					    (
					     pdeid,
					     pjianoreferencia,
					     pjiresposta,
					     pjidataatualizacao
					    )VALUES(
					     %d,
						 %s,
						 '%s',
						 '%s'
					    )",
					$pdeid,
					$_POST['pjianoreferencia'] ? $_POST['pjianoreferencia'] : 'null',
					substr($_POST['pjiresposta'],0,350),
					'now()'
				);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p29' AND
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
											 	ppritem = 'p29'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p29','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pjiid = $_POST['pjiid'] ? $_POST['pjiid'] : $_GET['pjiid'];
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
$cForm->direcPag('salva( pjiid )', array("pjiid" => $pjiid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($pjiid);
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = \'?modulo=principal/instrumento1/p25&acao=A\';
		 </script>');
endif;
*/

/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.percentualjornadaintegral
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
	<? $cForm->montarbuttons("validaForm()");?>				
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	/*
	if (!d.formulario.elements['rotafetoudesempenho'][0].checked && !d.formulario.elements['rotafetoudesempenho'][1].checked) {
		alert("Campo 'Há turmas ou disciplinas sem professor?' é Obrigatório.");
		return;
	}
	*/
	if (d.formulario.elements['pjiresposta'].value == '') {
		alert("Campo 'Resposta' é Obrigatório.");
		return;
	}
	d.formulario.submit();
	return true;
}
</script>	