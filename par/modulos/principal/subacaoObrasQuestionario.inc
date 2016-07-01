<?php 
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

//echo carregaAbasProInfancia("par.php?modulo=principal/subacaoObras&acao=A&tipoAba=questionario&preid=".$_REQUEST['preid'], $_REQUEST['preid'], $descricaoItem);

monta_titulo('QUESTIONÁRIO', 'Preencha o questionário');

$preid = $_REQUEST['preid'];
$qrpid = pegaQrpidPAC( $preid, 43 );

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$travaCorrecao = true;

$perfil = pegaPerfilGeral();

$obSubacaoControle = new SubacaoControle();
$obPreObra = new PreObra();

if($preid){
	$arDados = $obSubacaoControle->recuperarPreObra($preid);
}

if($esdid) {
	// Eu, Victor Benzi, comentei essa regra pois ela simplesmente foi copiada do pacQuestionario.inc sem ser alterada! 04/09/2012
	/*if( is_array($respSim) ){
		$travaCorrecao = (in_array(QUESTAO_DOCUMENTO1, $respSim) && in_array(QUESTAO_DOCUMENTO2, $respSim));
	}*/
	
	// Regra passada pelo Daniel - 9/6/11
	#Regras de acesso: Passada por Thiago em 24/05/2012 - PERFIL CONSULTA, APENAS VISUALIZAÇÃO.
	#Regras de acesso: Modificação na estrutura do cógido para melhoria e adequação as regras estabelecidas.

	/* if(in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) && $esdid == WF_PAR_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$habilitado = 'S';	
	}else{
		if( 
			((
				$esdid == WF_PAR_EM_CADASTRAMENTO ||
				$esdid == WF_PAR_EM_DILIGENCIA ||
				$esdid == WF_PAR_OBRA_EM_CADASTRAMENTO_CONDICIONAL ||
				$esdid == WF_PAR_OBRA_EM_REFORMULACAO
			) && 
			(
				in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
				in_array(PAR_PERFIL_PREFEITO, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
				in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
				in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil)
			)) || (in_array(PAR_PERFIL_ADM_OBRAS, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil ))
		){
		//	if($travaCorrecao){
		//		$habilitado = 'N';
		//	}else{
				$habilitado = 'S';
		//	}
		} else {
			$habilitado = 'N';
		}
	} */
} 

// nova situação, se o preobra for uma reformulação... desabilitar
if($arDados['preidpai']) {
	$habilitado = 'N';
	$travaCorrecao = true;
}
?>
<script language="JavaScript">
if (jQuery('#comentariosDiligencia').is(':empty')){
    alert('teste');
}

</script>
<?php echo cabecalho();?>
<?php if($habilitado == 'S' && count($respSim)): ?>
	<?php
	$txtAjuda = "É necessário o preenchimento completo e apresentação das informações complementares solicitadas no Relatório de Vistoria do Terreno disponibilizado no sistema.";
	$imgAjuda = "<img alt=\"{$txtAjuda}\" title=\"{$txtAjuda}\" src=\"/imagens/ajuda.gif\">"; 
	?>
	<table align="center" class="Tabela" cellpadding="2" cellspacing="1">
		<tr>
			<td width="100" style="text-align: right;" class="SubTituloDireita">Ajuda:</td>
			<td width="90%" style="background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;" class="SubTituloDireita">
				<?php echo $imgAjuda ?>
			</td>
		</tr>
	</table>
	<?php endif; ?>
	<table bgcolor="#f5f5f5" align="center" class="tabela" >
		<tr>
			<td>
			<fieldset style="width: 94%; background: #fff;"  >
				<legend>Questionário</legend>
				<?php
					$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $habilitado ) );
				?>
			</fieldset>
		</tr>
	</table>