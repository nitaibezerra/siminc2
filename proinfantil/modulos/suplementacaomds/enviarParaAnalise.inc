<?php 
if($_POST['historico']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql_historico = "SELECT 	to_char(m.mjmdata,'DD/MM/YYYY'), m.mjmjustificativa, a.mdaanalisetecnica, m.usucpf, u.usunome  
					  FROM 		proinfantil.mdsjustificativamunicipio m
					  LEFT JOIN proinfantil.mdsanalise a ON m.muncod = a.muncod AND a.mdastatus = 'A' AND a.tipid = 2 and a.mdaano = '{$_SESSION['exercicio']}'
					  LEFT JOIN seguranca.usuario u ON m.usucpf = u.usucpf
					  inner join proinfantil.mdsdadoscriancapormunicipio b on b.muncod = m.muncod and b.cpmano = '".((int)$_SESSION['exercicio'] - 1)."'
					  WHERE 	m.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";
	
	$historico = $db->carregar($sql_historico);
	$cab = array('Data da Diligência','Justificativa','Motivo da Diligência','CPF','Nome');
	$db->monta_lista($sql_historico, $cab, 1000, 10, 'N', '', '', '', '', ''); 
	exit();
}

if($_REQUEST['requisicao'] == 'salvaJustificativa'){
	$sql = "SELECT MAX(mjmid) as mjmid
			FROM  proinfantil.mdsjustificativamunicipio 
			WHERE muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' AND mjmstatus = 'A'";
	
	$rs = $db->pegaLinha($sql);
	
	if(empty($rs['mjmid'])){
		$sql_ins = "INSERT INTO proinfantil.mdsjustificativamunicipio
				(usucpf,mjmjustificativa,muncod,mjmdata,mjmstatus)
			VALUES
				('{$_SESSION['usucpf']}','{$_REQUEST['mjmjustificativa']}','{$_SESSION['proinfantil']['mds']['muncod']}',now(),'A');";
		$db->executar($sql_ins);
	} else {
		$sql_upd = "UPDATE 	proinfantil.mdsjustificativamunicipio  
				    SET	    mjmstatus = 'I' 
				    WHERE 	muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' AND mjmid = {$rs['mjmid']}";
		$db->executar($sql_upd);		
		
		$sql_ins = "INSERT INTO proinfantil.mdsjustificativamunicipio
						(usucpf,mjmjustificativa,muncod,mjmdata,mjmstatus)
					VALUES
						('{$_SESSION['usucpf']}','{$_REQUEST['mjmjustificativa']}','{$_SESSION['proinfantil']['mds']['muncod']}',now(),'A');";		
		$db->executar($sql_ins);
	}
	

	if($db->commit()){
		$db->sucesso('suplementacaomds/enviarParaAnalise');
	}
}

if($_REQUEST['requisicao'] == 'tramitarAnalise'){
	if($_REQUEST['docid']){		
		include_once APPRAIZ . "includes/workflow.php";
		
		$sql = "select ed.esdid from 
					workflow.documento d
					inner join workflow.estadodocumento ed on ed.esdid = d.esdid
				where d.docid = " . $_REQUEST['docid'];
		$esdid = $db->pegaUm( $sql );
		
		if( $esdid == WF_MDS_EM_CADASTRAMENTO ) $esdidAnalise = 1402;
		if( $esdid == WF_MDS_EM_DIGILENCIA ) $esdidAnalise = 1407;		
		
		if(wf_alterarEstado($_REQUEST['docid'], $esdidAnalise, ' ', array())){
			echo '1';
			die();
		}		
	}
	echo '2';
	die();
}

//Chamada de programa
include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once "_funcoes_mds.php";
echo "<br/>";
?>

<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" onclick="javascript:document.location.href='proinfantil.php?modulo=suplementacaomds/listaCreche&acao=A'" id="btnVoltar" />
		</td>
		<td class="TituloTela">Suplementação MDS</td>
	</tr>
</table>
<br/>
<?php 

$perfis = pegaPerfil($_SESSION['usucpf']);

$sql = "SELECT 	cpmid 
		FROM 	proinfantil.mdsdadoscriancapormunicipio cpm		 
		WHERE 	muncod = '{$_SESSION['proinfantil']['mds']['muncod']}' and cpm.cpmano = ({$_SESSION['exercicio']} - 1)";
$cpmid = $db->pegaUm($sql);

$docid = criaDocumento($cpmid);
$esdid = pegaEstadoAtual($docid);

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

if($_SESSION['baselogin'] == "simec_desenvolvimento") {
	$abacod_tela = 57559;
} else {
	$abacod_tela = 57568;
}

$arMnuid = array();

if($esdid != WF_MDS_EM_ANALISE && (in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis))){
	$arMnuid[] = 11127;
}

$db->cria_aba( $abacod_tela, $url, '', $arMnuid );


$sql = "SELECT 	mjmid,
				mjmjustificativa 
		FROM 	proinfantil.mdsjustificativamunicipio 
		WHERE	muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";
$rs = $db->pegaLinha($sql);

if($rs) extract($rs);

if( $esdid == WF_MDS_EM_DIGILENCIA && $_SESSION['exercicio'] == '2013' ){
	$sql = "SELECT
			    hst.hstid,
			    hst.htddata,
			    hst.docid,
			    hst.aedid
			FROM
			    proinfantil.mdsdadoscriancapormunicipio cm
			    inner join workflow.documento 		doc ON doc.docid = cm.docid
			    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
			    inner join workflow.historicodocumento hst on hst.docid = doc.docid 
			WHERE
			    cm.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
			    and cm.cpmano = (".$_SESSION['exercicio']." - 1)
			  order by hst.htddata asc";
	
	$arrWork = $db->carregar( $sql );
    $arrWork = $arrWork ? $arrWork : array();
        	
    $dias = calculaDiasVigencia($arrWork, AEDID_MDS_ENCAMINHAR_ANALISE_DILIGENCIA, AEDID_MDS_ENCAMINHAR_DILIGENCIA_ANALISE);
    $diasDiligencia = (90 - (int)$dias);
    $diasDiligencia = ((int)$diasDiligencia < 0 ? 0 : $diasDiligencia);
}

cabecalhoMunicipio();

$titulo_modulo = "Pendências";
$titulo_modulo2 = '<img src="../imagens/AtencaoP.gif" height="15" align="absmiddle" />&nbsp;Não impeditiva&nbsp;<img src="../imagens/excluir_2.gif" height="18" align="absmiddle" />&nbsp;Impeditiva';
monta_titulo( $titulo_modulo, $titulo_modulo2 );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	$(function(){
		$('#btnSalvar').click(function(){
			if($('[name=mjmjustificativa]').val() == ''){
				alert('O campo Justificativa é obrigatório!');
				$('[name=mjmjustificativa]').focus();
				return false;
			}
			$('[name=requisicao]').val('salvaJustificativa');
			$('#formPendencias').submit();
		});
		$('#btnTramitar').click(function(){

			var docid = $('[name=docid]').val();
			var cpmid = $('[name=cpmid]').val();
			
			$('#btnTramitar').attr('disabled', true);
			$.ajax({
				url		: 'proinfantil.php?modulo=suplementacaomds/enviarParaAnalise&acao=A',
				type	: 'post',
				data	: 'requisicao=tramitarAnalise&docid='+docid+'&cpmid='+cpmid,
				success	: function(e){					
					e = trim(e);
					e = parseInt(e);				
					if(e == 1){
						alert('Enviado para análise do MEC.');
						document.location.href = 'proinfantil.php?modulo=suplementacaomds/enviarParaAnalise&acao=A';
					}else{
						alert('Não foi possível enviar para análise!');
					}
				}
			});
			$('#btnTramitar').attr('disabled', false);
		});
	});

	$(function(){
		function aguarda(val){
			if(val){
				$('#aguardando').show();
			}else{
				$('#aguardando').hide();
			}
		}
		$('.div_historico').click(function(){
			aguarda(true);
			var id = $(this).attr('id');
			if($('#td_'+id).html() != ''){
				if( $('#td_'+id).css('display') == 'none' ){
					$('#td_'+id).show();
					$('#imgmais'+id).hide();
					$('#imgmenos'+id).show();
				} else {
					$('#td_'+id).hide();
					$('#imgmais'+id).show();
					$('#imgmenos'+id).hide();
				}
				aguarda(false);
			} else {
				$.ajax({
					type: "POST",
					url: 'proinfantil.php?modulo=suplementacaomds/enviarParaAnalise&acao=A',
					data: "historico=true&id="+id,
					async: false,
					success: function(msg){
						$('#td_'+id).html(msg);
						$('#td_'+id).show();
						$('#imgmais'+id).hide();
						$('#imgmenos'+id).show();					
						aguarda(false);
					}
				});
			}
		});
		aguarda(false);
	});
</script>
<style>
.legenda{
	width: 100%;
	border: 1px solid black;
	text-align: left;
	float: right;	
}
.div_historico {
background: #white;
}
.div_historico:hover {
background: #eeeeaa;
}
</style>

<center>
	<form id="formPendencias" name="formPendencias" method="post">
		<input type="hidden" name="mjmid" value="<?php echo $mjmid ? $mjmid : ''; ?>" />
		<input type="hidden" name="docid" value="<?php echo $docid; ?>" />
		<input type="hidden" name="cpmid" value="<?php echo $cpmid; ?>" />
		<input type="hidden" name="requisicao" />
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="3" >
			<?if( $esdid == WF_MDS_EM_DIGILENCIA && $_SESSION['exercicio'] == '2013' ){ ?>
			<tr>
				<td class="subtituloEsquerda" colspan="2" style="color: red; ">Dias restantes para o Município responder a diligência: <?=$diasDiligencia; ?> </td>
			</tr>
			<?} ?>
			<tr>
				<td valign="top">
					<?php 
					$sql = "SELECT 		DISTINCT ent.entcodent,
							            ent.entnome,
							            coalesce(pro.prcqtdalunoinfantilintegral,0) as prcqtdalunoinfantilintegral,
							            coalesce(pro.prcqtdalunoinfantilparcial,0) as prcqtdalunoinfantilparcial,
							            coalesce(mdsquantidadepbfparcial,0)+coalesce(mdsquantidadepbfintegral,0) as mdsquantidadepbf,
							            coalesce(mdsquantidadepbfparcial,0) as mdsquantidadepbfparcial,
							            coalesce(mdsquantidadepbfintegral,0) as mdsquantidadepbfintegral
					        FROM        entidade.entidade ent
					        INNER JOIN  entidade.endereco ede ON ede.entid = ent.entid
					        INNER JOIN  territorios.municipio mun ON mun.muncod = ede.muncod
					        INNER JOIN 	proinfantil.procenso pro ON pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
					        LEFT JOIN  	proinfantil.mdssuplementacao sup ON sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'         
							WHERE 		ent.entstatus = 'A'
							AND 		ent.entcodent IS NOT NULL
							AND 		ent.tpcid in (1,2,3)			
							AND			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
							ORDER BY	ent.entnome";
					
					$rsCreches = $db->carregar($sql);
					
					$sql = "SELECT		sum(coalesce(pro.prcqtdalunoinfantilintegral,0) + coalesce(pro.prcqtdalunoinfantilparcial,0) ) as alunos_mat,
							            sum(coalesce(sup.mdsquantidadepbfparcial,0)+coalesce(sup.mdsquantidadepbfintegral,0)) as alunos_pbf,
							            coalesce(cpmnumcriancascompbf,0) as alunos_mun,
							            cpmqtdcrianca03mun as alunos_mes
					        FROM		entidade.entidade ent
					        INNER JOIN  entidade.endereco ede ON ede.entid = ent.entid
					        INNER JOIN  territorios.municipio mun ON mun.muncod = ede.muncod					        
					        INNER JOIN	proinfantil.procenso pro ON pro.entcodent = ent.entcodent and pro.prcano = ({$_SESSION['exercicio']} - 1)
					        LEFT JOIN 	proinfantil.mdssuplementacao sup ON sup.entcodent = ent.entcodent and sup.prcid = pro.prcid AND sup.mdsstatus = 'A'
					        LEFT JOIN  	proinfantil.mdsdadoscriancapormunicipio dcm ON dcm.muncod = mun.muncod and dcm.cpmano = ({$_SESSION['exercicio']} - 1)        
							WHERE 		ent.entstatus = 'A'
							AND 		ent.entcodent IS NOT NULL									
							AND			mun.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'
							GROUP BY 	mun.muncod,cpmnumcriancascompbf,cpmqtdcrianca03mun";
					
					$rs = $db->pegaLinha($sql);			
	
					$arPendencias = array(); ?>
	
					<table class="tabela" width="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 style="width:100%;">
						<?php if($rsCreches): ?>
							<?php foreach($rsCreches as $creches): ?>
								<?php if($creches['mdsquantidadepbfintegral'] > $creches['prcqtdalunoinfantilintegral']): ?>
									<?php $arPendencias[] = 5; ?>
									<tr>
										<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="middle" /></td>
										<td style="background-color:#EDEDED;">&nbsp; Na creche <?php echo $creches['entcodent'].' - '.$creches['entnome']; ?> a quantidade (integral) de PBF é maior que a quantidade de matrículas;</td>
									</tr>                                                             
								<?php endif; ?>
								<?php if($creches['mdsquantidadepbfparcial'] > $creches['prcqtdalunoinfantilparcial']): ?>
									<?php $arPendencias[] = 5; ?>
									<tr>
										<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="middle" /></td>
										<td style="background-color:#EDEDED;">&nbsp; Na creche <?php echo $creches['entcodent'].' - '.$creches['entnome']; ?> a quantidade (parcial) de PBF é maior que a quantidade de matrículas;</td>
									</tr>                                                             
								<?php endif; ?>
							<?php endforeach; ?>
						<?php endif; ?>					
						<?php if(!$rs['alunos_pbf']): ?>
							<?php $arPendencias[] = 1; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="middle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;Necessário preencher a aba Lista de Estabelecimentos;</td>
							</tr>
						<?php endif; ?>
						
						<?php if($rs['alunos_pbf'] > $rs['alunos_mun']): ?>
							<?php $arPendencias[] = 2; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="middle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;O total de crianças informadas como beneficiárias do Programa Bolsa Família é superior aos beneficiários de 0 a 48 meses de famílias que recebem Bolsa Família no Município;</td>
							</tr>
						<?php endif; ?>
						
						<?php if($rs['alunos_pbf'] > $rs['alunos_mat']): ?>
							<?php $arPendencias[] = 3; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/excluir_2.gif" align="middle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;O total de crianças informadas como beneficiárias do Programa Bolsa Família é superior ao quantitativo de matrículas declaradas no Educacenso;</td>
							</tr>
						<?php endif; ?>
						
						<?php 
						$tapa 	 = $rs['alunos_mes']; // Quantidade de crianças de 0 a 48 meses anos no Município
						$tampbf  = $rs['alunos_mun']; // Número de crianças que foram beneficiadas com o Bolsa Família no Município
						$tamc 	 = $rs['alunos_mat']; // Quantidade de alunos (infantil integral + parcial)
						$tamcpbf = $rs['alunos_pbf']; // Quantidade de alunos que foram beneficiados com o Bolsa Família (infantil integral + parcial)
						$calculo1 = $tapa > 1 ? ($tampbf/$tapa)*100 : 0; // Se tem alguma criança de 0 a 48 meses no município eu tiro a porcentagem de quantas crianças de 0 a 48 meses anos que foram beneficiadas com o bolsa familia (divido o numero de crianças que foram beneficiadas com o bolsa familia pelo numero de crianças de 0 a 48 meses no municipio e multiplico por 100). 
						$calculo2 = $tamc > 1 ? ($tamcpbf/$tamc)*100 : 0; // Se tem algum aluno (infantil integral + parcial) eu tiro a porcentagem de quantos alunos que foram beneficiados com o bolsa familia (divido o numero de alunos que foram beneficiados com o bolsa familia (infantil integral + parcial) pelo numero de alunos (infantil integral + parcial) no municipio e multiplico por 100).
						?>
						
						<?php if($calculo1 < $calculo2 && empty($arPendencias)): // Se o percentual de quantas crianças de 0 a 48 meses que foram beneficiadas com o bolsa familia no Município for menor que o percentual de quantos alunos que foram beneficiados com o bolsa familia no Município então tem pendencias! ?>
							<?php $arPendencias[] = 4; ?>
							<tr>
								<td class="subtituloDireita" width="60"><img src="../imagens/AtencaoP.gif" align="middle" /></td>
								<td style="background-color:#EDEDED;">&nbsp;Atenção! Para que sua solicitação seja encaminhada para pagamento, faz-se necessário justificar porque em algumas escolas 
									(ou em todas elas) o percentual médio de crianças de 0 a 48 meses de famílias beneficiárias do Bolsa Família frequentando creches é maior do que o percentual médio de 
									crianças nessa faixa etária, no município, beneficiárias do bolsa família. A justificativa do município será analisada e aceita ou não pelo MEC. Antes de justificar, 
									recomenda-se: 1) leitura do manual de orientações, item 2.3.2.; e 2) verificar se as fontes de informação utilizadas foram o Censo Escolar <?=((int)$_SESSION['exercicio'] - 1) ?>) e o Cadúnico de maio de <?=((int)$_SESSION['exercicio'] - 1) ?>) 
									(checando quais crianças do Cadastro são de famílias beneficiárias do Programa Bolsa Família e, dentre essas, quais freqüentavam creches em <?=((int)$_SESSION['exercicio'] - 1) ?>).</td>
							</tr>
							<tr>
								<td class="subtituloDireita" valign="top">
									&nbsp;&nbsp;&nbsp;&nbsp;<img src="../imagens/seta_filho.gif" />
								</td>
								<td>								
									<?php echo campo_textarea('mjmjustificativa', 'S', $acesso['enviar_analise'], $label, 65, 8, 4000); ?>
								</td>
							</tr>
						<?php endif; ?>
						<?php if(in_array(4, $arPendencias)): ?>
							<?php if($_SESSION['data_hora_atual'] <= $_SESSION['data_hora_final'] || $acesso['enviar_analise'] == 'S'){ ?>
							<tr>
								<td class="subtituloDireita" valign="top">
									&nbsp;
								</td>
								<td class="subtituloEsquerda">								
									<input type="button" value="Salvar" id="btnSalvar" />
								</td>
							</tr>
							<?php } ?>
						<?php endif; ?>
						<?php if(in_array($esdid,array(WF_MDS_EM_CADASTRAMENTO,WF_MDS_EM_DIGILENCIA)) && (empty($arPendencias) || ($mjmjustificativa && !in_array(1,$arPendencias) && !in_array(2,$arPendencias) && !in_array(3,$arPendencias)))): ?>
							<?php if($esdid == WF_MDS_EM_DIGILENCIA): ?>
								<tr>							
									<td colspan="2" align="center">
										<input type="button" value="Devolver para análise" id="btnTramitar"/>											
									</td>
								</tr>
							<?php else: ?>							
								<tr>							
									<td colspan="2">
										<center>
											<p><img src="../imagens/check_checklist.png" /></p>
											<font size="+1">O preenchimento dos dados foi concluído com sucesso! Para dar prosseguimento à solicitação, é necessário enviá-la para análise. Deseja enviá-la agora?</font>
											<p><input type="button" value="Sim" id="btnTramitar"/>
											<input type="button" value="Não" /></p>
										</center>
									</td>
								</tr>
							<?php endif; ?>
						<?php endif; ?>
						
						<?php if($esdid == WF_MDS_EM_ANALISE): ?>
							<tr>							
								<td colspan="2">
									<center>
										<font size="+1">Enviado para análise.</font>
									</center>
								</td>
							</tr>
						<?php endif; ?>
					</table>		
				</td>			
			</tr>
		</table>
	</form>
</center>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<?php 
$sql_historico = "SELECT 	to_char(m.mjmdata,'DD/MM/YYYY'), m.mjmjustificativa, a.mdaanalisetecnica, m.usucpf, u.usunome  
			      FROM 		proinfantil.mdsjustificativamunicipio m
				  LEFT JOIN proinfantil.mdsanalise a ON m.muncod = a.muncod AND a.mdastatus = 'A' AND a.tipid = 2 and a.mdaano = '{$_SESSION['exercicio']}'
				  LEFT JOIN seguranca.usuario u ON m.usucpf = u.usucpf
				  WHERE 	m.muncod = '{$_SESSION['proinfantil']['mds']['muncod']}'";
$historico = $db->pegaLinha($sql_historico);

if(!empty($historico)){?>
<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
	<tr>
		<td style="background-color: #CDCDCD;">
			<div id="hist" class="div_historico" style="float:left;cursor:pointer;width:100%">
				<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmaishist">
				<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenoshist">
				<center><b>Histórico</b></center>
			</div>
		</td>
	</tr>
	<tr>
		<td>
			<div id="td_hist" style="display:none;"></div>
		</td>
	</tr>	
</table>

<br>
<?php } ?>

<?php if(false): ?>
	<div style="position:absolute;right:80px;top:245px;">
		<?php
		$oculta = true;
		if(checkPerfil(array(PERFIL_ADMINISTRADOR))){
			$oculta = false;
		}
		wf_desenhaBarraNavegacao( $docid , array('cpmid' => $cpmid),  array('historico'=>$oculta) );
		?>
	</div>
<?php endif; ?>


<script>
	<?php if($acesso['enviar_analise'] == 'N'){ ?>
		$('#btnSalvar').attr('disabled', true);
		$('#btnTramitar').attr('disabled', true);
		$('#btnTramitar').hide();
		$('#btnSalvar').hide();
	<?php } ?>

	<?php if($_SESSION['data_hora_atual'] >= $_SESSION['data_hora_final'] && $esdid != WF_MDS_EM_DIGILENCIA){ ?>
	$(function(){
		$('input, textarea').attr('disabled', true);
	});	
	<?php } ?>
	$(function(){ $('#btnVoltar').attr('disabled', false); });
</script>	