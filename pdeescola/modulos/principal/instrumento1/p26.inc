<?php
pde_verificaSessao();
/*
 * 
Function, salvar/editar
 */
function salva ($avcid = null){
	global $db,$pdeid;
	
	if ($avcid){
		$sql = sprintf("UPDATE
					 	 pdeescola.avaliarelacaocomunidade
					    SET
					     coaid 		 = %d,
					     avcexplicacao = '%s'
					    WHERE
					     avcid = %d",
					$_POST['coaid'],
					substr($_POST['avcexplicacao'], 0, 300 ),
					$avcid
				);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.avaliarelacaocomunidade
					    (
					     pdeid,
					     coaid,
					     avcexplicacao
					    )VALUES(
					     %d,
						 %d,
						 '%s'
					    )",
					$pdeid,
					$_POST['coaid'],
					substr($_POST['avcexplicacao'], 0, 300 )
				);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p26' AND
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
											 	ppritem = 'p26'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p26','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$avcid = $_POST['avcid'] ? $_POST['avcid'] : $_GET['avcid'];
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
$cForm->direcPag('salva( avcid )', array("avcid" => $avcid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($avcid);
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
		 pdeescola.avaliarelacaocomunidade
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
	<? $cForm->montarbuttons("validaForm()");?>							
</table>
<script type="text/javascript">

d = document;
function validaForm(){
	var radio = d.formulario.elements['coaid'];
	var ct = 0;
	for(i=0; i<radio.length; i++)
	{
		if(d.formulario.elements['coaid'][i].checked == true)
		{
			ct = 1;
			break;
		}
	}
	if(ct == 0)
	{
		alert("Campo 'Como a escola avalia...' é Obrigatório.");
		return;
	}

	if (d.formulario.elements['avcexplicacao'].value == '') {
		alert("Campo 'Explique' é Obrigatório.");
		return;
	}

	
	d.formulario.submit();
	return true;
}
</script>	