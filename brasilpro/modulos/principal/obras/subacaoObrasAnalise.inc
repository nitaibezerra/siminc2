<?php 
 
include_once APPRAIZ . 'includes/workflow.php';

$preid  = $_REQUEST['preid'];

$muncod = $_SESSION['par']['muncod'];
$docid  = prePegarDocid($preid);
$esdid  = prePegarEstadoAtual($docid);
$perfil = pegaArrayPerfil($_SESSION['usucpf']);

monta_titulo( 'Enviar para análise', ''  );

$arperfil = pegaArrayPerfil($_SESSION['usucpf']);

$obPreObraControle = new PreObraControle();
$oPreObra = new PreObra();

if($preid){	
	
	$qrpid = pegaQrpid( $preid, OBRAS_QUESTIONARIO );
	
	$pacDados = $oPreObra->verificaTipoObra($preid, OBRAS_SISID);	
	$pacFotos = $oPreObra->verificaFotosObra($preid, OBRAS_SISID);
	$pacDocumentos = $oPreObra->verificaDocumentosObra($preid, OBRAS_SISID, $pacDados);

	$pacQuestionario = $oPreObra->verificaQuestionario($qrpid);	
	//$boPlanilhaOrcamentaria = $oPreObra->verificaPlanilhaOrcamentaria($preid, OBRAS_SISID, $preid);
	$pacCronograma = $oPreObra->verificaCronograma($preid);
}

$boPlanilhaOrcamentaria['faltam'] = $boPlanilhaOrcamentaria['itcid'] - $boPlanilhaOrcamentaria['ppoid'];
$arPendencias = array('Dados do terreno' => 'Falta o preenchimento dos dados.',
					  'Relatório de vistoria' => 'Falta o preenchimento dos dados.',
					  'Cadastro de fotos do terreno' => 'Deve conter no mínimo 3 fotos do terreno.',
					  'Cronograma físico-financeiro' => 'Falta o preenchimento dos dados.',
					  'Documentos anexos' => 'Falta anexar os arquivos.',
					  'Projetos - Tipo A' => 'Falta anexar os arquivos.',
					  'Itens Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
					  'Planilha orçamentária' => 'Falta(m) '.$boPlanilhaOrcamentaria['faltam'].' iten(s) a ser(em) preenchido(s) na planilha orçamentaria.',
					  'Valor da planilha orçamentária' => 'O valor {valor} não confere, deve estar entre R$ '.formata_valor($boPlanilhaOrcamentaria['minimo']).' e R$ '.formata_valor($boPlanilhaOrcamentaria['maximo']).'.');

?>
<script language="javascript" type="text/javascript" src="../includes/tiny_mce.js"></script>
<table class="tabela" align="center">
		
	<?php //ver($pacDocumentos, d) ?>
	<?php $x=0 ?>
	<?php foreach($arPendencias as $k => $v): ?>
		<?php
			$cor = ($x % 2) ? 'white' : '#d9d9d9;'; 
			if(  ( !$pacDados && $k == 'Dados do terreno' ) || 
				 ( $k == 'Relatório de vistoria' && $pacQuestionario != 22 ) || 
				 ( $pacFotos < 3 && $k == 'Cadastro de fotos do terreno' ) ||
				 ( $k == 'Itens Planilha orçamentária' && $boPlanilhaOrcamentaria['faltam'] > 0 ) ||
				 ( $k == 'Planilha orçamentária' && $boPlanilhaOrcamentaria['ppoid'] == 0 && $arDados['ptoprojetofnde'] == 't') ||
				 ( $k == 'Valor da planilha orçamentária' && ($boPlanilhaOrcamentaria['valor'] < $boPlanilhaOrcamentaria['minimo'] || $boPlanilhaOrcamentaria['valor'] > $boPlanilhaOrcamentaria['maximo']) && $pacFNDE == 't' ) ||
				 ( $k == 'Cronograma físico-financeiro' && !$pacCronograma && $arDados['ptoprojetofnde'] == 't' ) ||
				 ( ($pacDocumentos['arqid'] < $pacDocumentos['podid'] || !$pacDocumentos) && $k == 'Documentos anexos' ) 
				 ): ?>
			<?php if(!$boMsg){
			?>
				<tr>
					<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
						O sistema verificou que alguns dados não foram preenchidos:
					</td>
				</tr>
				<?php $boMsg = true; ?>
			<?php }else{
				  } ?>
			<tr style="background-color: <?php echo $cor ?>;">
				<td>
					<?php 
					switch($k){
						case 'Dados do terreno':
							$aba = 'dados';
							break;
						case 'Relatório de vistoria':
							$aba = 'Questionario';
							break;
						case 'Cadastro de fotos do terreno':
							$aba = 'Fotos';
							break;
						case 'Itens Planilha orçamentária':
							$aba = 'PlanilhaOrcamentaria';
							break;
						case 'Planilha orçamentária':
							$aba = 'PlanilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo B 110v':
							$aba = 'PlanilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo B 220v':
							$aba = 'PlanilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo C 110v':
							$aba = 'PlanilhaOrcamentaria';
							break;
						case 'Planilha orçamentária Tipo C 220v':
							$aba = 'PlanilhaOrcamentaria';
							break;							
						case 'Cronograma físico-financeiro':
							$aba = 'CronogramaFisicoFinanceiro';
							break;
						case 'Documentos anexos':
							$aba = 'Documento';
							break;
						default:
							$aba = "dados";
							break;
					}
					?>
					<a href="brasilpro.php?modulo=principal/obras/subacaoObras&acao=A&sbaid=<?php echo $_GET['sbaid'] ?>&ano=<?php echo $_GET['ano'] ?>&aba=<?php echo $aba ?>&preid=<?php echo $preid ?>">
					<img border="0" src='/imagens/consultar.gif' onclick='javascript:void(0)'>
					</a>
				</td>
				<td>
					<b><?php echo $k ?></b>
					<br />&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; - 
					<?php echo str_replace("{valor}","R$ ".formata_valor($boPlanilhaOrcamentaria['valor']), $v) ?><br />
				</td>
				<td style="background:white;width:100px;"></td>
			</tr>
			<?php $x++ ?>
		<?php endif; ?>			
	<?php endforeach; 
		if(!$boMsg): ?>
		<tr>
			<td colspan="3" style="text-align:center;font-size:14px;font-weight:bold;color:#900;height:50px;">
				O sistema não encontrou pendências. 
			</td>
		</tr>
	<?php endif; ?>
	<tr>
		<td colspan="3" height="170px;"></td>
	</tr>		
</table>
<?php 
if( $db->testa_superuser() ){
	echo '<div style="position:relative;margin-top: -200px;margin-left: 50px;">';
		wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'boMsg' => $boMsg ),  array('historico'=>true) );
	echo '</div>';
}elseif( !$boMsg ){
	echo '<div style="position:relative;margin-top: -200px;margin-left: 770px;">';
		$arSituacao = array(WF_EM_CADASTRAMENTO, WF_ANALISE_RET_DILIGENCIA);
		if($db->testa_superuser()){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg,  array('historico'=>true) ));
		}elseif(in_array($esdid, $arSituacao) && (in_array(CTE_PERFIL_EQUIPE_LOCAL, $arperfil) || in_array(CTE_PERFIL_EQUIPE_LOCAL_APROVACAO, $arperfil)) ){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ),  array('historico'=>true));	
		}elseif( in_array(CTE_PERFIL_ADMINISTRADOR, $arperfil) || 
				 in_array(CTE_BRASIL_ANALISTA_FNDE, $arperfil) ||
				 in_array(CTE_BRASIL_COORDENADOR_FNDE, $arperfil) ||
				 in_array(CTE_BRASIL_PROECERISTA_FNDE, $arperfil) ){
			wf_desenhaBarraNavegacao( $docid , array( 'preid' => $preid, 'qrpid' => $qrpid, 'boMsg' => $boMsg ),  array('historico'=>true));
		}
		
	echo '</div>';
}

?>