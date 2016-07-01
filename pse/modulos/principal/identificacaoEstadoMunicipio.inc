<?php
include  APPRAIZ . 'pse/modulos/principal/permissao.inc';
include_once( APPRAIZ. "pse/classes/EstadoMunicipioPSE.class.inc" );
include_once( APPRAIZ. "pse/classes/SecretariaEstMun.class.inc" );
include APPRAIZ . "includes/classes/dateTime.inc";

$nmMunEst = $_SESSION['pse']['flagmun'];

$obEstadomunicipiopse = new EstadoMunicipioPSE('',$_SESSION['pse']["muncod"], $nmMunEst );

$obEstadomunicipiopse->muncod = $_SESSION['pse']["muncod"];
$obEstadomunicipiopse->empflagestmun = $nmMunEst;

$muncod = $_SESSION['pse']["muncod"];

$obSecMuncipal = new SecretariaEstMun('');
$obSecEstadual = new SecretariaEstMun('');

//CARREGANDO OBJETOS MUNICIPAL E ESTADUAL
for($i = 0; $i< count($obEstadomunicipiopse->secretariaEstMun); $i++)
{
	//if($pflcod == SECRETARIA_MUNICIPAL)
	if($nmMunEst == 'm')
	{
		if($obEstadomunicipiopse->secretariaEstMun[$i]->semsecretariaestadualmunicipal == 'M')
			$obSecMuncipal = $obEstadomunicipiopse->secretariaEstMun[$i];
	} else {
		if($obEstadomunicipiopse->secretariaEstMun[$i]->semsecretariaestadualmunicipal == 'E')
			$obSecEstadual = $obEstadomunicipiopse->secretariaEstMun[$i];	
			
		if($obEstadomunicipiopse->secretariaEstMun[$i]->semsecretariaestadualmunicipal == 'x')
			$obSecMuncipal = $obEstadomunicipiopse->secretariaEstMun[$i];	
	}
}
//VERIFICANDO SE O MUNICÍPIO É CAPITAL
$capital = $obEstadomunicipiopse->verificaCapital();

//CADASTRANDO MUNICÍPIO
if($_REQUEST['cadmunicipio'])
{
	//POPULANDO OBJETO EstadoMunicipioPSE
	//$empparticipapse2009 = $_REQUEST["empparticipapse2009"];
	
	//if($pflcod == SECRETARIA_ESTADUAL)
	if($nmMunEst == 'e')
	{
		$sql = "select empid from pse.estadomunicipiopse where muncod = '$muncod' and empflagestmun = 'e' and empano = ".$_SESSION['exercicio'];
		$empid = $db->pegaUm($sql);
		if($empid == ''){
			$sql = "INSERT INTO pse.estadomunicipiopse (muncod, empparticipapse2009, empflagestmun, empano) VALUES ($muncod, '0', 'e', ".$_SESSION['exercicio'].") RETURNING empid";
			$empid = $db->pegaUm($sql);
		} 
		//else {
		//	$sql = "UPDATE pse.estadomunicipiopse set empparticipapse2009 = $empparticipapse2009 WHERE empid = $empid";
		//	$db->executar($sql);
		//}
	} else {
		$sql = "select empid from pse.estadomunicipiopse where muncod = '$muncod' and empflagestmun = 'm' and empano = ".$_SESSION['exercicio'];
		$empid = $db->pegaUm($sql);
		if($empid == ''){
			$sql = "INSERT INTO pse.estadomunicipiopse (muncod, empparticipapse2009, empflagestmun, empano) VALUES ($muncod, '0', 'm', ".$_SESSION['exercicio'].") RETURNING empid";			
			$empid = $db->pegaUm($sql);
		} else {
			//$sql = "UPDATE pse.estadomunicipiopse set empparticipapse2009 = $empparticipapse2009 WHERE empid = $empid";
			//$db->executar($sql);
			$sql = "select semid from pse.secretariaestmun where empid = $empid";
			$semid = $db->pegaUm($sql);
			unset($obSecMuncipal);
			$obSecMuncipal = new SecretariaEstMun($semid);
		}
	}
	$_SESSION['pse']['empid'] = $empid;
	$db->commit();
	echo "<pre>".$empid."</pre>";
	if( $empid <> '' )
	{
		//POPULANDO OBJETO SecretariaEstMun (MUNICIPAL)
		$arCamposMun = array('semnomesecretariosaude', 'semendsecretariasaude','semnomerepresecretariasaude','sememailrepresecretariasaude','semtelefonerepresecretariasaude',
							'semcargofuncaorepresecretariasa','semnomesecretarioeducacao','semendsecretariaeducacao','semnomerepresecretariaeducacao','sememailrepresecretariaeducacao',
							'semtelefonerepresecretariaeduca','semcargofuncaorepresecretariaed');
		
		$obSecMuncipal->popularObjeto( $arCamposMun );
		$obSecMuncipal->empid = $empid;
		//if($pflcod == SECRETARIA_ESTADUAL)
		if($nmMunEst == 'e')
		{
			$obSecMuncipal->semsecretariaestadualmunicipal = "x";
		} else {
			$obSecMuncipal->semsecretariaestadualmunicipal = "M";
		}
		$obSecMuncipal->corrigeCharset();
		

		if($obSecMuncipal->salvar())
		{
			//SE FOR CAPITAL SALVA DADOS ESTADUAIS
			//if($pflcod == SECRETARIA_ESTADUAL)
			if($nmMunEst == 'e')
			{
				//POPULANDO OBJETO SecretariaEstMun (ESTADUAL)
				$arCamposMun = array(  'empid','semsecretariaestadualmunicipal', 'semnomesecretariosaude', 'semendsecretariasaude','semnomerepresecretariasaude','sememailrepresecretariasaude','semtelefonerepresecretariasaude',
									'semcargofuncaorepresecretariasa','semnomesecretarioeducacao','semendsecretariaeducacao','semnomerepresecretariaeducacao','sememailrepresecretariaeducacao',
									'semtelefonerepresecretariaeduca','semcargofuncaorepresecretariaed');
				
				$arDados = array();
				$arDados['empid'] 								= $empid;
				$arDados['semsecretariaestadualmunicipal'] 		= "E";
				$arDados['semnomesecretariosaude'] 				= $_REQUEST["nomesecestsaude"];
				$arDados['semendsecretariasaude']	 			= $_REQUEST["endsecestsaude"];
				$arDados['semnomesecretarioeducacao']			= $_REQUEST["nomesecestedu"];
				$arDados['semendsecretariaeducacao']			= $_REQUEST["endsecestedu"];
				$arDados['semnomerepresecretariaeducacao']		= $_REQUEST["nomerepsecestedu"];
				$arDados['sememailrepresecretariaeducacao']		= $_REQUEST["emailrepsecestedu"];
				$arDados['semtelefonerepresecretariaeduca']		= $_REQUEST["telrepsecestedu"];
				$arDados['semcargofuncaorepresecretariaed']		= $_REQUEST["carfuncrepsecestedu"];
				$arDados['semnomerepresecretariasaude']			= $_REQUEST["nomerepsecestsaude"];
				$arDados['sememailrepresecretariasaude']		= $_REQUEST["emailrepsecestsaude"];
				$arDados['semtelefonerepresecretariasaude']		= $_REQUEST["telrepsecestsaude"];
				$arDados['semcargofuncaorepresecretariasa']		= $_REQUEST["carfuncrepsecestsaude"];
				
				$obSecEstadual->popularObjeto( $arCamposMun,$arDados );
				$obSecEstadual->corrigeCharset();
				
				$obSecEstadual->salvar();
			}
			
			$obEstadomunicipiopse->commit();
		}	
	}
	else
	{
		$obEstadomunicipiopse->rollback();
		echo "erro";
		die;
	}

	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';

$titulo = "PSE - Programa Saúde na Escola";
$titulo2 = "IDENTIFICAÇÃO";
echo "<br>";
monta_titulo( $titulo, '' );
monta_titulo( $titulo2, '<img border="0" src="../imagens/obrig.gif"/>Indica Campo Obrigatório.' );

?>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<form method="POST" name="formunicipio" action="" id="formunicipio">
	<input type="hidden" value='' id="cadmunicipio" name="cadmunicipio">
	<input type="hidden" value='' id="capital" name="capital">
	<input type="hidden" value="<?php echo $obEstadomunicipiopse->muncod ?>" id="muncod" name="muncod">
	<?php Cabecalho($muncod,2,$nmMunEst); ?>
	<table style="border-bottom: 0px" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloEsquerda" colspan="2">1. Identificação</td>
	</tr>
	<!--<tr>
		<td align='right' class="SubTituloDireita">Participou do PSE/2009?</td>
		<td>
			<?php
				$checkNao = "";
				$checkSim = "";
				$obEstadomunicipiopse->empparticipapse2009 != "1" ? $checkNao = "checked=checked" : $checkSim = "checked=checked";
			?> 
			 <input type="radio" value="0" <?=$checkNao?> name="empparticipapse2009" title="Participou do PSE/2009?" onclick="validaItem(0,'item2[]')" />Não  
			 <input type="radio" <?=$checkSim?> value="1" name="empparticipapse2009" title="Participou do PSE/2009?" onclick="validaItem(1,'item2[]')" />Sim 
			<input type="radio" value="0" <?=$checkNao?> name="empparticipapse2009" title="Participou do PSE/2009?" />Não 
			<input type="radio" <?=$checkSim?> value="1" name="empparticipapse2009" title="Participou do PSE/2009?" />Sim
		</td>
	</tr>
	--><?php //if($pflcod == SECRETARIA_ESTADUAL)

	if($nmMunEst == 'e')
	{ ?>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome do Secretário Estadual de Saúde</td>
		<td><?=campo_texto('nomesecestsaude','S','S','Secretário Estadual de Sáude',100,60,'','','','','','id=nomesecestsaude', '', $obSecEstadual->semnomesecretariosaude);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Estadual de Saúde</td>
		<td><?=campo_texto('endsecestsaude','S','S','Endereço da Secretaria Estadual de Saúde',100,60,'','','','','','id=endsecestsaude', '', $obSecEstadual->semendsecretariasaude);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome do Secretário Estadual de Educação</td>
		<td><?=campo_texto('nomesecestedu','S','S','Nome do Secretário Estadual de Educação',100,60,'','','','','','id=nomesecestedu', '', $obSecEstadual->semnomesecretarioeducacao);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Estadual de Educação</td>
		<td><?=campo_texto('endsecestedu','S','S','Endereço da Secretaria Estadual de Educação',100,60,'','','','','','id=endsecestedu', '',$obSecEstadual->semendsecretariaeducacao);?></td>
	</tr>
	<?php } ?>
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Secretário Municipal de Saúde</td>
		<td><?=campo_texto('semnomesecretariosaude','S','S','Nome do Secretário Municipal de Saúde',100,60,'','','','','','id=semnomesecretariosaude', '', $obSecMuncipal->semnomesecretariosaude );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Municipal de Saúde</td>
		<td>
		<?=campo_texto('semendsecretariasaude','S','S','Endereço da Secretaria Municipal de Saúde',100,60,'','','','','','id=semendsecretariasaude', '', $obSecMuncipal->semendsecretariasaude );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome do Secretário Municipal de Educação</td>
		<td><?=campo_texto('semnomesecretarioeducacao','S','S','Nome do Secretário Municipal de Educação',100,60,'','','','','','id=semnomesecretarioeducacao', '', $obSecMuncipal->semnomesecretarioeducacao );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Endereço da Secretaria Muncipal de Educação</td>
		<td><?=campo_texto('semendsecretariaeducacao','S','S','Endereço da Secretaria Estadual de Educação',100,60,'','','','','','id=semendsecretariaeducacao', '',$obSecMuncipal->semendsecretariaeducacao );?></td>
	</tr>
	<?php //if($pflcod == SECRETARIA_ESTADUAL)
	if($nmMunEst == 'e')
	{ ?>
	<tr name="dadosOpcionais[]">
		<td class="SubTituloEsquerda" colspan="2">1.1 Representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial, responsávl pelo PSE</td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecestedu','S','S','Nome do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=nomerepsecestedu','', $obSecEstadual->semnomerepresecretariaeducacao );?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecestedu','S','S','E-mail do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecestedu','', $obSecEstadual->sememailrepresecretariaeducacao );?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecestedu','S','S','Telefone do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecestedu','',$obSecEstadual->semtelefonerepresecretariaeduca);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecestedu','S','S','Cargo/Função do representante da Secretaria Estadual de Educação no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecestedu','',$obSecEstadual->semcargofuncaorepresecretariaed);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td class="SubTituloEsquerda" colspan="2">Representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('nomerepsecestsaude','S','S','Nome do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=nomerepsecestsaude','',$obSecEstadual->semnomerepresecretariasaude);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('emailrepsecestsaude','S','S','E-mail do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=emailrepsecestsaude','',$obSecEstadual->sememailrepresecretariasaude);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('telrepsecestsaude','S','S','Telefones do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=telrepsecestsaude','',$obSecEstadual->semtelefonerepresecretariasaude);?></td>
	</tr>
	<tr name="dadosOpcionais[]">
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('carfuncrepsecestsaude','S','S','Cargo/Função do representante da Secretaria Estadual de Saúde no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=carfuncrepsecestsaude','',$obSecEstadual->semcargofuncaorepresecretariasa);?></td>
	</tr>
	<?php } ?>
	<tr>
		<td class="SubTituloEsquerda" colspan="2">1.2 Representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('semnomerepresecretariaeducacao','S','S','Nome do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=semnomerepresecretariaeducacao','', $obSecMuncipal->semnomerepresecretariaeducacao );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('sememailrepresecretariaeducacao','S','S','E-mail do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=sememailrepresecretariaeducacao','', $obSecMuncipal->sememailrepresecretariaeducacao );?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('semtelefonerepresecretariaeduca','S','S','Telefones do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=semtelefonerepresecretariaeduca','',$obSecMuncipal->semtelefonerepresecretariaeduca);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('semcargofuncaorepresecretariaed','S','S','Cargo/Função do representante da Secretaria Municipal de Educação no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=semcargofuncaorepresecretariaed','',$obSecMuncipal->semcargofuncaorepresecretariaed);?></td>
	</tr>
	<tr> 
		<td class="SubTituloEsquerda" colspan="2">Representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial, responsável pelo PSE</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Nome</td>
		<td><?=campo_texto('semnomerepresecretariasaude','S','S','Nome do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial ',100,60,'','','','','','id=semnomerepresecretariasaude','',$obSecMuncipal->semnomerepresecretariasaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">E-mail</td>
		<td><?=campo_texto('sememailrepresecretariasaude','S','S','E-mail do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,60,'','','','','','id=sememailrepresecretariasaude','',$obSecMuncipal->sememailrepresecretariasaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Telefones</td>
		<td><?=campo_texto('semtelefonerepresecretariasaude','S','S','Telefones do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,20,'','','','','','id=semtelefonerepresecretariasaude','',$obSecMuncipal->semtelefonerepresecretariasaude);?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita">Cargo/Função</td>
		<td><?=campo_texto('semcargofuncaorepresecretariasa','S','S','Cargo/Função do representante da Secretaria Municipal de Saúde no Grupo de Trabalho Intersetorial',100,30,'','','','','','id=semcargofuncaorepresecretariasa','',$obSecMuncipal->semcargofuncaorepresecretariasa);?></td>
	</tr>
	<?php if(($nmMunEst=='e' && $pflcod == SECRETARIA_ESTADUAL) || ($nmMunEst=='m' && $pflcod == SECRETARIA_MUNICIPAL) || $pflcod == SUPER_USUARIO){?>
	<?php 
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	if( $resp == NULL ){ ?>
		<tr style="background-color: #DCDCDC">
			<td  align="center" colspan="2">
				<input type="button" value="Salvar" onclick="validaFormulario(false);">
			</td>
		</tr>
	<?php } ?>
	<?php }?>
	<?=navegacao('cadastroEstadoMunicipioArvore','representante');?>	
</table>
</form>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="/pse/geral/_funcoes.js"></script>
<div id="divTeste"></div>