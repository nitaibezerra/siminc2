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
function salva ($mppid = null){
	global $db,$pdeid;
	
	if ($mppid){
		$sql = sprintf("UPDATE
					 	 pdeescola.medidaprojetoparceria
					    SET
					     mppenvolveu 		 = '%s'
					    WHERE
					     mppid = %d",
					$_POST['mppenvolveu'],
					$mppid
				);
	}else{
		$sql = sprintf("INSERT INTO pdeescola.medidaprojetoparceria
					    (
					     pdeid,
					     mppenvolveu
					    )VALUES(
					     %d,
						 '%s'
					    )",
					$pdeid,
					$_POST['mppenvolveu']
				);
	}
	$db->executar($sql);
	
	$epfid = $db->pegaUm("SELECT
							epfid
						  FROM
						  	pdeescola.estruturaperfilfuncionamento
						  WHERE
						  	trim(epfnomearquivo) = 'p24' AND
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
											 	ppritem = 'p24'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento,ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p24','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
}

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$mppid = $_POST['mppid'] ? $_POST['mppid'] : $_GET['mppid'];
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
$cForm->direcPag('salva( mppid )', array("mppid" => $mppid));

/*
///// Faz inserção/atualização e retorna à árvore /////
if ($_POST):
	salva($mppid);
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
		 pdeescola.medidaprojetoparceria
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
<?=subCabecalho('24 - A execução das medidas ou projetos envolveu parceria com outras instituições (ONGs, empresas, sindicatos etc.).'); ?>	
<form action="" method="post" name="formulario">
<input type='hidden' name='mppid' value='<?=$mppid ?>' />
<input type='hidden' name='pdeid' value='<?=$pdeid ?>' />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td width="40%" class="SubTituloDireita">A execução das medidas ou projetos envolveu parceria com outras instituições (ONGs, empresas, sindicatos etc.)?</td>
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
		$db->monta_radio('mppenvolveu',$sql,$somenteLeitura,0)
		?>
		</td>
	</tr>
	<? $cForm->montarbuttons("validaForm()");?>														
</table>
<script type="text/javascript">
d = document;
function validaForm(){
	<?php if( $possuiProjetos ) { ?>
		if (!d.formulario.elements['mppenvolveu'][0].checked && !d.formulario.elements['mppenvolveu'][1].checked) {
			alert("Campo 'A execução das medidas ou projetos...' é Obrigatório.");
			return;
		}
	
		d.formulario.submit();
		return true;
	<?
	}
	?>
}
</script>	