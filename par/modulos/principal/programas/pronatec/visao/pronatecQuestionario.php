<?php 
if($_REQUEST['preid']==''&&$_SESSION['par']['preid']==''){
	echo "<script>
			alert('Escolha um imóvel.');
			window.close();
		  </script>";
	die();
}

include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Questionario".$preidTx;
 
echo carregaAbasPronatec($lnkabas);
monta_titulo('Características do Imóvel', 'Preencha o questionário');


$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];
$qrpid = pegaQrpidPAC( $preid, 43 );

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$habilitado = 'N';
$travaCorrecao = true;


if( $esdid ) {
	
	if( is_array($respSim) ){
		$travaCorrecao = (in_array(QUESTAO_DOCUMENTO1,$respSim) && in_array(QUESTAO_DOCUMENTO2,$respSim));
	}
	
	//$obSubacaoControle = new SubacaoControle();
	$obPreObra2 = new PreObra();
	
	if($preid){
		//$arDados = $obPreObra2->recuperarPreObra($preid);
		
		$sql = "SELECT 
					pre.preid,
					pre.docid,
					pre.presistema,
					pre.preidsistema,
					pre.ptoid,
					pre.preobservacao,
					pre.prelogradouro,
					pre.precomplemento,
					pre.estuf,
					pre.muncod,
					pre.precep,
					pre.prelatitude,
					pre.prelongitude,
					pre.predtinclusao,
					pre.prebairro,
					pre.preano,
					pre.qrpid,
					pre.predescricao,
					pre.prenumero,
					pre.pretipofundacao,
					pre.entcodent,
					pto.ptodescricao,
					pto.ptoprojetofnde,
					mun.mundescricao
				FROM obras.preobra pre
				LEFT JOIN territorios.municipio mun ON mun.muncod = pre.muncod
				INNER JOIN obras.pretipoobra pto ON pto.ptoid = pre.ptoid
				WHERE preid = '{$preid}'
				AND prestatus = 'A'";
		
		$arDados = $db->pegaLinha($sql);
		
	}
	
	// Regra passada pelo Daniel - 9/6/11
	if(possuiPerfil($arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL)) && 
	   $esdid == WF_PRONATEC_OBRA_APROVADA && $arDados['ptoprojetofnde'] == 'f') {
		$habilitado = 'S';
	} else {
	
		$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
		$arrSituacao = array(WF_PRONATEC_EM_DILIGENCIA);
		
		if( $esdid == WF_PRONATEC_EM_CADASTRAMENTO ){
			$habilitado = 'S';
		}
		
		if( in_array($esdid, $arrSituacao) && possuiPerfil($arrPerfil) ){
			if($travaCorrecao){
				$habilitado = 'N';
			}else{
				$habilitado = 'S';
			}
		}
	}
}

?>
<script language="JavaScript">

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
			/////////////////////////////////////////////////////////////
				$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'tamDivPx' => 250, 'habilitado' => $habilitado ) );
			/////////////////////////////////////////////////////////////
			//$db->close();
			$db = new cls_banco();
			?>
		</fieldset>
	</tr>
</table>