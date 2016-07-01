<?php

include_once( APPRAIZ. "pse/classes/CadEstadoMunicipio.class.inc" );
include_once( APPRAIZ. "pse/classes/MunSecEstadual.class.inc" );
include_once( APPRAIZ. "pse/classes/RepreOutSec.class.inc" );

$muncod = $_SESSION['pse']['muncod'];

$obCadEstadoMunicipio = new CadEstadoMunicipio($muncod);
$obCadEstadoMunicipio->muncod = $muncod;

$capital = $obCadEstadoMunicipio->verificaCapital();

if($capital)
	$obMunSecEstadual 		= new MunSecEstadual( $obCadEstadoMunicipio->muncod  );

//capital
//$capital = verificaCapital($_REQUEST["muncod"]);



/*//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	filtrarMunicipio();
	exit;
}*/

//Carregar lista
if($_REQUEST['listarAtributoFormulario']){
	header('content-type: text/html; charset=ISO-8859-1');
	listarAtributoFormularioAjax();
	exit;
}
/*
//Vefica se o município é capital
if($_REQUEST['verificaCapital']){
	if(verificaCapital())
		echo 'capital';
	exit;	
}*/

//CADASTRANDO MUNICÍPIO
if($_REQUEST['cadmunicipio']){
	
	
	/*---------- CADASTRANDO MUNICIPIO -----------*/
	$arCamposMun = array(  'muncod', 'stmunpse2009', 'nomesecmunsaude', 'endsecmunsaude', 'nomerepsecmunsaude', 'emailrepsecmunsaude', 'telrepsecmunsaude', 
						'carfuncrepsecmunsaude', 'nomesecmunedu', 'endsecmunedu', 'nomerepsecmunedu', 'emailrepsecmunedu', 'telrepsecmunedu', 'carfuncrepsecmunedu', 
						'repoutrasec' );
	
	$obCadEstadoMunicipio->popularObjeto( $arCamposMun ); 
	$obCadEstadoMunicipio->insereAspas();
	
	if( $obCadEstadoMunicipio->inserirCadastro() ){
		$obCadEstadoMunicipio->commit();
	}
	else{
		$obCadEstadoMunicipio->rollback();
		echo "erro";
		die;
	}

	//SE FOR CAPITAL SALVA DADOS ESTADUAIS
	if($capital)
	{
		
		$arCamposEst = array('muncod' , 'nomesecestsaude' ,'endsecestsaude' ,'nomerepsecestsaude' , 'emailrepsecestsaude' ,'telrepsecestsaude' ,'carfuncrepsecestsaude' , 
						'nomesecestedu' ,'endsecestedu' ,	'nomerepsecestedu' ,'emailrepsecestedu' , 'telrepsecestedu' ,'carfuncrepsecestedu' , );
		
		$obMunSecEstadual->popularObjeto($arCamposEst);
		$obMunSecEstadual->insereAspas();
		
		if( $obMunSecEstadual->inserirCadastro() ){
			$obMunSecEstadual->commit();
		}
		else{
			$obMunSecEstadual->rollback();
			echo "erro";
			die;
		}
		
	}
	/*---------- FIM CADASTRANDO MUNICIPIO *MUNICIPAL*-----------*/
	
	
	/*-------- OUTRAS SECRETARIAS ---------------*/
	
	
	/*$secretarias = explode(",",$_POST["secretarias"]);
	
	$obRepreOutSec 			= new RepreOutSec();
	ver($obRepreOutSec,d);
	$obRepreOutSec->muncod 	= $obCadEstadoMunicipio->muncod;
	$obRepreOutSec->removerSecretarias();
	$obRepreOutSec->cadastrarSecretarias($secretarias);
	/*-------- FIM OUTRAS SECRETARIAS ---------------*/


	
	exit;
}


include APPRAIZ. '/includes/Agrupador.php';
// monta cabeçalho
include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "Cadastro de Estado/Município - " . $_REQUEST["mundescricao"] . '/' . $_REQUEST["uf"] ;
echo "<br>";
monta_titulo( $titulo, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<form method="POST" name="formunicipio" action="" id="formunicipio">
	<input type="hidden" value='' id="cadmunicipio" name="cadmunicipio">
	<input type="hidden" value='' id="capital" name="capital">
	<input type="hidden" value="<?php echo $obCadEstadoMunicipio->muncod ?>" id="muncod" name="muncod">
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">



	<!-- ITEM 1
	<tr>
		<td class="SubTituloDireita" valign="top">Estado</td>
		<td><?
		/*$estuf = $_REQUEST['estuf'];
		$sql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						FROM
							territorios.estado";
		$db->monta_combo( "estuf", $sql, 'S','Selecione...', 'filtraMunicipio', '', '', '215','S','estuf','','','Estado' );*/
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Município</td>
		<td id="td_municipio">
		<?
			/*$muncod = $muncod;
			$sql = "SELECT
						ter.muncod AS codigo,
						ter.mundescricao AS descricao
					FROM
						territorios.municipio ter
					WHERE
						ter.estuf = '$_REQUEST[estuf]'
						ORDER BY ter.mundescricao";
			$db->monta_combo( "muncod", $sql, ($_REQUEST["estuf"]) ? 'S' : 'N', 'Selecione...', '', '', 'Município', '215', 'S','muncod', '', $obCadEstadoMunicipio->muncod );*/
		?>
		</td>
	</tr> -->
	<tr>
		<td class="SubTituloEsquerda" colspan="2">1. Identificação</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Participou do PSE/2009?</td>
		<td>
			<?php
				$checkNao = "";
				$checkSim = "";
				$obCadEstadoMunicipio->stmunpse2009 != "1" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
			?> 
			<input type="radio" value="0" <?=$checkNao?> name="stmunpse2009" title="Participou do PSE/2009?" onclick="validaItem(0,'item2[]')" />Não 
			<input type="radio" <?=$checkSim?> value="1" name="stmunpse2009" title="Participou do PSE/2009?" onclick="validaItem(1,'item2[]')" />Sim
		</td>
	</tr>
	
	
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome do Secretário Estadual de Saúde</td>
		<td><?=campo_texto('nomesecestsaude','S','S','Secretário Estadual de Sáude',100,60,'','','','','','id=nomesecestsaude');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Estadual de Saúde</td>
		<td><?=campo_texto('endsecestsaude','S','S','Endereço da Secretaria Estadual de Saúde',100,60,'','','','','','id=endsecestsaude');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome do Secretário Estadual de Educação</td>
		<td><?=campo_texto('nomesecestedu','S','S','Nome do Secretário Estadual de Educação',100,60,'','','','','','id=nomesecestedu');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Estadual de Educação</td>
		<td><?=campo_texto('endsecestedu','S','S','Endereço da Secretaria Estadual de Educação',100,60,'','','','','','id=endsecestedu');?></td>
	</tr>
		
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Secretário Municipal de Saúde</td>
		<td><?=campo_texto('nomesecmunsaude','S','S','Nome do Secretário Municipal de Saúde',100,60,'','','','','','id=nomesecmunsaude', '', $obCadEstadoMunicipio->nomesecmunsaude );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Municipal de Saúde</td>
		<td>
		<?=campo_texto('endsecmunsaude','S','S','Endereço da Secretaria Municipal de Saúde',100,60,'','','','','','id=endsecmunsaude', '', $obCadEstadoMunicipio->endsecmunsaude );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Secretário Municipal de Educação</td>
		<td><?=campo_texto('nomesecmunedu','S','S','Nome do Secretário Municipal de Educação',100,60,'','','','','','id=nomesecmunedu', '', $obCadEstadoMunicipio->nomesecmunedu );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Estadual de Educação</td>
		<td><?=campo_texto('endsecmunedu','S','S','Endereço da Secretaria Estadual de Educação',100,60,'','','','','','id=endsecmunedu', '', $obCadEstadoMunicipio->endsecmunedu );?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td class="SubTituloEsquerda" colspan="2">1.1 Representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial, responsávl pelo PSE</td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecestedu','S','S','Nome do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=nomerepsecestedu');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecestedu','S','S','E-mail do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecestedu');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecestedu','S','S','Telefone do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecestedu');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecestedu','S','S','Cargo/Função do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecestedu');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td class="SubTituloEsquerda" colspan="2">Representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>

	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecestsaude','S','S','Nome do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=nomerepsecestsaude');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecestsaude','S','S','E-mail do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecestsaude');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecestsaude','S','S','Telefones do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecestsaude');?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecestsaude','S','S','Cargo/Função do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecestsaude');?></td>
	</tr>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">1.2 Representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>

	<tr>
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecmunedu','S','S','Nome do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=nomerepsecmunedu','', $obCadEstadoMunicipio->nomerepsecmunedu );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecmunedu','S','S','E-mail do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecmunedu','', $obCadEstadoMunicipio->emailrepsecmunedu );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecmunedu','S','S','Telefones do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecmunedu','',$obCadEstadoMunicipio->telrepsecmunedu);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecmunedu','S','S','Cargo/Função do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecmunedu','',$obCadEstadoMunicipio->carfuncrepsecmunedu);?></td>
	</tr>
	<tr> 
		<td class="SubTituloEsquerda" colspan="2">Representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecmunsaude','S','S','Nome do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial ',100,60,'','','','','','id=nomerepsecmunsaude','',$obCadEstadoMunicipio->nomerepsecmunsaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecmunsaude','S','S','E-mail do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecmunsaude','',$obCadEstadoMunicipio->emailrepsecmunsaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecmunsaude','S','S','Telefones do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecmunsaude','',$obCadEstadoMunicipio->telrepsecmunsaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecmunsaude','S','S','Cargo/Função do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecmunsaude','',$obCadEstadoMunicipio->carfuncrepsecmunsaude);?></td>
	</tr>
		<tr style="background-color: #DCDCDC">
		<td colspan="2" align="center"><input type="button" value="Salvar" onclick="validaFormulario(false);"></td>
	</tr>
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>

<script type="text/javascript">
	if(<?=$capital?> == true)
		document.getElementById('capital').value = true;
	
	validaItem('<?= $capital?>','dadosOpcionais[]');
</script>

