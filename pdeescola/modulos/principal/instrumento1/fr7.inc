<?php
pde_verificaSessao();
/*
 * anmid, pdeid, onmresposta, onmanoreferencia, onmdataatualizacao
Function, salvar/editar
 */
function salva ($onmid = null){
	global $db,$pdeid;
	
	if ($onmid){
		$sql = sprintf("UPDATE
					 	 pdeescola.outronivelmodalidadeensino
					    SET
					     onmanoreferencia  		 = %s,
					     onmresposta = '%s',
					     onmdataatualizacao  = %s
					    WHERE
					     onmid = %d",
					ANO_EXERCICIO_PDE_ESCOLA ? ANO_EXERCICIO_PDE_ESCOLA : 'null',
					substr($_POST['onmresposta'], 0, 500 ),
					'now()',
					$onmid
				);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.outronivelmodalidadeensino
					    (
					     pdeid,
					     onmanoreferencia,
					     onmresposta,
					     onmdataatualizacao
					    )VALUES(
					     %d,
						 %s,
						 '%s',
						 '%s'
					    )",
					$pdeid,
					$_SESSION['onmanoreferencia'] ? $_SESSION['onmanoreferencia'] : 'null',
					substr($_POST['onmresposta'], 0, 500 ),
					'now()'
				);
	}
	//dbg($sql, 1);
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'fr7' AND
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
											 	ppritem = 'fr7'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'fr1', 'fr7','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
		 
		}
		
	$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$onmid = $_POST['onmid'] ? $_POST['onmid'] : $_GET['onmid'];
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
$dados = $db->carregar("SELECT
							pi.pridescricao,
							pi.priid,
							tp.tprid,
							tp.tprdescricao
						FROM
							pdeescola.problemainstrumento1 pi
						INNER JOIN
							pdeescola.tipoproblema tp ON tp.tprid = pi.tprid
													  AND tp.tprvisivel = 't'
						WHERE
							pi.pdeid = ".$pdeid."
						ORDER BY
							tp.tprdescricao");

if(!$dados) {
	$vis = "style='visibility:hidden;'";
	echo "<script language=\"javascript\" type=\"text/javascript\">
			var qtdItens = 0;
		  </script>";
} else {
	$vis = "style='visibility:visible;'";
	echo "<script language=\"javascript\" type=\"text/javascript\">
			var qtdItens = ".count($dados).";
		  </script>";
}
$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva( onmid )', array("onmid" => $onmid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($onmid);
	die('<script>
			alert(\'Operação realizada com sucesso!\');
			location.href = \'?modulo=principal/instrumento1/fr7&acao=A\';
		 </script>');
endif;
*/

/*
 * Carrega dados
 */
$sql = "SELECT
		 *
		FROM
		 pdeescola.outronivelmodalidadeensino
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
<?=subCabecalho('7 - Outros Níveis e Modalidades de Ensino'); ?>
<form action="" method="post" name="formulario">
<input type='hidden' name='onmid' value='<?=$onmid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita">Ano:</td>
		<td width="60%">
		<?php
		$ano = $onmid ? $onmanoreferencia : ANO_EXERCICIO_PDE_ESCOLA;
		if(!$ano) $ano = ANO_EXERCICIO_PDE_ESCOLA;
		echo $ano;
		?>
		<input type="hidden" name="pjianoreferencia" value="<?=$ano?>">
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top"><div align="justify">Caso o Estado ou o Município ofereça algum nível ou modalidade de ensino diferenciado, e foi necessária uma adequação na inserção de dados nos quadros anteriores, insira aqui uma nota explicativa:</div></td>
		<td>
		<?= campo_textarea( 'onmresposta', 'S', 'S', '', 80, 10, 500,'id="onmresposta"' ); ?>
		</td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>				
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	var onmr = document.formulario.onmresposta.value.length ;
	
	if(onmr >= 501){
		alert('Texto muito grande. Limite máximo de 500 caracteres.');
		return false;
	}
	if (d.formulario.elements['onmresposta'].value == '') {
		alert("Campo 'Resposta' é Obrigatório.");
		return false;
	}
	d.formulario.submit();
	return true;
}
</script>	