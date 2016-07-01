<?php

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$preid = ($_SESSION['par']['preid']) ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
$lnkabas = "par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=Analise".$preidTx;

echo carregaAbasPronatec($lnkabas);
monta_titulo( 'Analise', ''  );

$oPreObra = new PreObra();
$oSubacaoControle = new SubacaoControle();
$pacFNDE  = $oSubacaoControle->verificaObraFNDE($preid, SIS_OBRAS);
$arDados  = $oSubacaoControle->recuperarPreObra($preid);

if($preid){	
	
	$qrpid = pegaQrpidPAC( $preid, 43 );	
	
	$pacDados = $oSubacaoControle->verificaTipoObra($preid, SIS_OBRAS);	
	$pacFotos = $oSubacaoControle->verificaFotosObra($preid, SIS_OBRAS);
	$pacDocumentos = $oSubacaoControle->verificaDocumentosObra($preid, SIS_OBRAS, $pacDados);
	$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);	
}

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);	

$arPendencias = array('Dados do imóvel' => 'Falta o preenchimento dos dados.',
					  'Caracteristicas do imóvel' => 'Falta o preenchimento dos dados.',
					  'Cadastro de fotos do imóvel' => 'Deve conter no mínimo 3 fotos do terreno.',
					  'Documentos anexos' => 'Falta anexar os arquivos.');

$arperfil = pegaArrayPerfil($_SESSION['usucpf']);

?>
<?php echo cabecalho();?>
<table class="tabela" align="center">
<?php 
	$x=0; 
	foreach($arPendencias as $k => $v){ 
		$cor = ($x % 2) ? 'white' : '#d9d9d9;'; 
		if(  ( !$pacDados && $k == 'Dados do imóvel' ) || 
			 ( $k == 'Caracteristicas do imóvel' && $pacQuestionario != 22 ) || 
			 ( $pacFotos < 3 && $k == 'Cadastro de fotos do imóvel' ) ||
			 ( ($pacDocumentos['arqid'] != $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' ) 
			 ){
			if(!$boMsg){
?>
	<tr>
		<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
			O sistema verificou que alguns dados não foram preenchidos:
		</td>
	</tr>
<?php 
				$boMsg = true;
			} 
?>
	<tr style="background-color: <?php echo $cor ?>;">
		<td>
<?php 
			switch($k){
				case 'Dados do imóvel':
					$aba = 'dados';
					break;
				case 'Características do imóvel':
					$aba = 'questionario';
					break;
				case 'Cadastro de fotos do imóvel':
					$aba = 'foto';
					break;
				case 'Documentos anexos':
					$aba = 'documento';
					break;
				default:
					$aba = "dados";
					break;
			}
?>
			<a href="par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
			<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
			</a>
		</td>
		<td>
			<b><?php echo $k ?></b>
			<br />
			&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
			<?php echo $v ?>
			<br />
		</td>
		<td style="background:white;width:100px;"></td>
	</tr>
<?php 
			$x++;
		}
	}
	if(!$boMsg){ 
?>
	<tr>
		<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
			O sistema não encontrou pendências. 
		</td>
	</tr>
<?php 
	} 
?>
	<tr>
		<td colspan="3" height="170px;"></td>
	</tr>		
</table>
<?php 
if( $escrita && !$boMsg ){
	echo '<div style="position:relative;margin-top: -250px;margin-left: 830px;">';
	$arSituacao = array(WF_PRONATEC_EM_CADASTRAMENTO, WF_PRONATEC_EM_DILIGENCIA);		
	if($db->testa_superuser()){
		wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ));
	}elseif(in_array($esdid, $arSituacao) && (in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $arperfil) || in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $arperfil)) ){
		wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid ),  array('historico'=>true));	
	}
		
	echo '</div>';
}elseif($db->testa_superuser() && !$boMsg ){
	echo '<div style="position:relative;margin-top: -250px;margin-left: 830px;">';
//	echo '<div style="position:absolute;right:45px;margin-top:55px;">';
		wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid ));
	echo '</div>';
}
?>