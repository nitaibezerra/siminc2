<?php 
include  "_funcoes_novasturmas.php";

if($_POST['historico']){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql_historico = "SELECT 		u.usunome, n.ntapareceraprovacao, to_char(n.ntadata,'DD/MM/YYYY') as data, CASE WHEN n.ntastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END ntastatus
				 	  FROM 			proinfantil.novasturmasanalise n
				  	  LEFT JOIN 	seguranca.usuario u ON u.usucpf = n.usucpf 
				  	  WHERE 		n.muncod = '{$_SESSION['proinfantil']['muncod']}'
				  	  ORDER BY 		n.ntaid DESC";
	$historico = $db->carregar($sql_historico);
	$cab = array('Analista','Parecer','Data','Status');
	$db->monta_lista($sql_historico, $cab, 1000, 10, 'N', '', '', '', '', ''); 
	exit();
}

if($_REQUEST['salvar_parecer_turma']){
	
	$sql = "select turid from proinfantil.analisenovasturmasaprovacao where turid = {$_REQUEST['turma']}";
	$temturma = $db->pegaUm($sql);
	
	if($temturma){
		$sql_turma = "UPDATE 	proinfantil.analisenovasturmasaprovacao
				  	  SET 		anatipo = '{$_REQUEST['analise']}',
					 			anaparecer = '".utf8_decode($_REQUEST['parecer'])."'
				  	  WHERE 	turid = {$_REQUEST['turma']}";
	} else {
		$sql_turma = "INSERT INTO proinfantil.analisenovasturmasaprovacao (anatipo, anaparecer, turid)
				  VALUES ('{$_REQUEST['analise']}',
					 		'".utf8_decode($_REQUEST['parecer'])."', {$_REQUEST['turma']})";
	}

	if(	$db->executar($sql_turma)){
		$db->commit();
		echo '1';
	} else {
		$db->rollback();
		echo '0';
	}
	exit();
}

if($_REQUEST['requisicao'] == 'salvarParecer'){
	$ntaid = $_REQUEST['ntaid'];
	$muncod = $_SESSION['proinfantil']['muncod'];
	$ntapareceraprovacao = $_REQUEST['ntapareceraprovacao'];
	$ntamesesrepasse = 0;
	$usuario = $_SESSION['usucpf'];
	$data = date('Y-m-d');
	$sql = "";
	$sql = "SELECT MAX(ntaid) as ntaid
			FROM  proinfantil.novasturmasanalise 
			WHERE muncod = '{$muncod}' AND ntastatus = 'A'";
	
	$res = $db->pegaLinha($sql);
	
	if(empty($res['ntaid'])){
		$sql_ins = "INSERT INTO proinfantil.novasturmasanalise
				(muncod,ntapareceraprovacao, ntamesesrepasse, ntadata, ntastatus, usucpf)
				VALUES
				('{$muncod}','{$ntapareceraprovacao}',{$ntamesesrepasse},'{$data}','A','{$usuario}')";
		$db->executar($sql_ins);
		if($db->commit()){
			$db->sucesso('novasturmas/analise');
		}			
	} else {
		$sql_up = "UPDATE 	proinfantil.novasturmasanalise 
				   SET	   	ntastatus = 'I' 
				   WHERE 	muncod = '{$muncod}'
				   AND 		ntaid = {$ntaid}";
		$db->executar($sql_up);
		
		$sql_in = "INSERT INTO proinfantil.novasturmasanalise
				(muncod, ntapareceraprovacao, ntamesesrepasse, ntadata, ntastatus, usucpf)
				VALUES
				('{$muncod}','{$ntapareceraprovacao}',{$ntamesesrepasse},'{$data}','A','{$usuario}')";
		$db->executar($sql_in);
		
		if($db->commit()){
			$db->sucesso('novasturmas/analise');
		}	
	} 
}

if($_POST['requisicao']){
	$_POST['requisicao']();
}

include_once APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";

echo "<br/>";
?>
<table class="tabela" height="100%" align="center" cellspacing="0" cellpadding="3" >
	<tr class="subtituloEsquerda">
		<td height="10" width="45%">
			<input type="button" value="Voltar" id="btnVoltar" />
		</td>
		<td class="TituloTela">Novas Turmas</td>
	</tr>
</table>
<br/>
<?php
if($_SESSION['baselogin']=="simec_desenvolvimento") {
	$abacod_tela = 57524;
	$arMnuid = array(11125,11766, 11767);
}else{
	$abacod_tela = 57524;
	$arMnuid = array(11125);
}

// Monta abas
$db->cria_aba( $abacod_tela, $url, '', $arMnuid );

$linha1 = 'Análise'; 
$linha2 = 'Novas Turmas';
monta_titulo($linha1, $linha2);

if($_SESSION['proinfantil']['muncod']){
	$sql = "SELECT 	ntaid, muncod, ntamesesrepasse, ntapareceraprovacao 
			FROM 	proinfantil.novasturmasanalise 
			WHERE	muncod = '{$_SESSION['proinfantil']['muncod']}'
			AND 	ntastatus = 'A'";
	$rs = $db->pegaLinha($sql);
	if($rs) extract($rs);	
}

$docid = criaDocumentoNovasTurmas($_SESSION['proinfantil']['muncod']);
$esdid = pegaEstadoAtualNovasTurmas($docid);

$acesso = verificaAcessoPerfil($_SESSION['usucpf'],$esdid);

if( $_SESSION['exercicio'] == date('Y') && $esdid != WF_NOVASTURMAS_EM_DILIGENCIA ){
	$acesso['cadastro'] = 'N';
	$acesso['analise'] = 'N';
}

$rsTurmasMatriculas = recuperarDadosMatriculasTurmas();

$sql = "SELECT 		tur.turid, ana.anatipo 
		FROM   		proinfantil.analisenovasturmasaprovacao ana
		INNER JOIN  proinfantil.turma tur ON tur.turid = ana.turid
		WHERE 		muncod = '{$_SESSION['proinfantil']['muncod']}'
		AND			tur.turstatus = 'A'
		AND			ana.anatipo IS NOT NULL";
$arrAna = $db->carregar($sql);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">
	
	function abrirTurma(turid){
		var html = ('<tr id="linhaTurma'+turid+'"><td colspan="10" ><div id="listaTurma'+turid+'"></div></td></tr>');
		$.ajax({
			type: "POST",
			url: "proinfantil.php?modulo=novasturmas/dadosrepasseturma&acao=A",
			data: {turma:turid},
			async: false,
			success: function(data){
				 if($('#linhaTurma'+turid).html() == null){
				  $('#imgmais'+turid).parent("td").parent("tr").after(html);
				 }
				 $("#listaTurma"+turid).html(data);
				 $("#linhaTurma"+turid).show();
				 $('#imgmais'+turid).hide();
				 $('#imgmenos'+turid).show();
			}
		});
	}
	
	function fecharTurma(turid){
		$('#linhaTurma'+turid).hide();
		$('#imgmais'+turid).show();
		$('#imgmenos'+turid).hide();
	}

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
					url: 'proinfantil.php?modulo=novasturmas/analise&acao=A',
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

	$(function(){
		$('.btnSalvarParecer').click(function(){
			if($('[name=ntapareceraprovacao]').val() == ''){
				alert('O campo descrição da análise é obrigatório!');
				$('[name=ntapareceraprovacao]').focus();
				return false;
			}	
			$('#formulario_parecer').submit();
		});
	});	

	$(function(){
		$('#btnVoltar').click(function(){
			document.location.href = 'proinfantil.php?modulo=novasturmas/listaTurmas&acao=A';
		});
	    $('.abrirTurmaPopup').click(function(){		
			return window.open('proinfantil.php?modulo=novasturmas/dadosturma&acao=A&turma='+this.id,'modelo',"height=600,width=950,scrollbars=yes,top=50,left=200");
	    });
	});
	
	function salvarParecerTurma(turid){
		if($('[name=rdn_turid'+turid+']:checked').val() == ''){
			alert('Informe a Análise.');
			$('[name=rdn_turid'+turid+']:checked').focus();
			return false;
		}	
		
		if($('#parecer'+turid).val() == ''){
			alert('Informe o Parecer.');
			$('#parecer').focus();
			return false;
		}
		$.ajax({
			type: "POST",
			url: 'proinfantil.php?modulo=novasturmas/analise&acao=A',
			data: { salvar_parecer_turma: "true", turma: turid, parecer: $('#parecer'+turid).val(), analise: $('[name=rdn_turid'+turid+']:checked').val()},
			async: false,
			success: function(retornoajax) {
				$('#debug').html(retornoajax);
			if(retornoajax == 1){
				alert("Parecer cadastrado!");
				window.location.href = window.location;
			} else {
				alert("Parecer não cadastrado!");
			}	
		}});
	}
</script>
<style>
.div_historico {
background: #white;
}
.div_historico:hover {
background: #eeeeaa;
}

.div_turma {
background: #white;
}
.div_turma:hover {
background: #eeeeaa;
}

</style>

<div id="debug"></div>
<form name="formulario" id="formulario_parecer" method="post" action="">
<input type="hidden" name="requisicao" value="salvarParecer" />
<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<?php if($_SESSION['proinfantil']['muncod']): 
	$sql = "SELECT 		muncod, 
						mundescricao, 
						estuf 
			FROM 		territorios.municipio 
			WHERE 		muncod = '{$_SESSION['proinfantil']['muncod']}'";
	$rs = $db->pegaLinha($sql); ?>
	<tr>
		<td class="subtituloDireita" width="140">UF</td>
		<td><?php echo $rs['estuf']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">Município</td>
		<td><?php echo $rs['mundescricao']; ?></td>
	</tr>
	<?php elseif(empty($_SESSION['proinfantil']['muncod']) && isset($_SESSION['proinfantil']['estuf'])): 
	$sql = "SELECT 		estdescricao, 
						estuf 
			FROM		territorios.estado 
			WHERE 		estuf = '{$_SESSION['proinfantil']['estuf']}'";
	$rs = $db->pegaLinha($sql); ?>	
	<tr>
		<td class="subtituloDireita" width="140">Estado</td>
		<td colspan="2" ><?php echo $rs['estdescricao']; ?></td>
	</tr>
	<tr>
		<td class="subtituloDireita">UF</td>
		<td colspan="2" ><?php echo $rs['estuf']; ?></td>
	</tr>	
	<?php endif; ?>	
	<?php $rsDadosEscola = recuperarDadosMatriculasTurmas(); ?>
	<tr>
		<td colspan="2">
			<table border="0" bordercolor="#FFFFFF" width="100%">
				<tr>
					<td>
					<div style="border: 1px solid #404040;">
						<table border="1" bordercolor="#FFFFFF" width="100%" class="listagem" height="225px">
							<thead>
								<tr>
									<th colspan="5">Dados - Quantidade de Matrículas Novas / Censo</th>
								</tr>
								<tr>
									<td rowspan="2" style=" background-color:#7BA2B6;" >Tipo de Atendimento</td>
									<th colspan="2" style=" background-color:#FABF40;">Educacenso 2011</th>
									<th colspan="2" style=" background-color:#50A56C;">Matrículas em 2012 - SITUAÇÃO ATUAL</th>
								</tr>
								<tr>
									<th style=" background-color:#FABF40;" >Rede pública municipal</th>
									<th style=" background-color:#FABF40;">Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
									<th style=" background-color:#50A56C;">Rede pública municipal</th>
									<th style=" background-color:#50A56C;">Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
								</tr>
							</thead>
							<tbody>		
								<tr>
									<td align="center" style=" background-color:#7BA2B6;">Creche Integral</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunocrecheintegralpublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunocrecheintegralconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculacrecheintegralpublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculacrecheintegralconveniada']; ?></td>
								</tr>
								<tr>
									<td align="center" style=" background-color:#7BA2B6;">Creche Parcial</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunocrecheparcialpublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunocrecheparcialconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculacrecheparcialpublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculacrecheparcialconveniada']; ?></td>
								</tr>
								<tr>
									<td align="center" style=" background-color:#7BA2B6;">Pré-escola Integral</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunopreescolaintegralpublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunopreescolaintegralconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculapreescolaintegralpublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculapreescolaintegralconveniada']; ?></td>
								</tr>
								<tr>
									<td align="center" style=" background-color:#7BA2B6;">Pré-escola Parcial</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunopreescolaparcialpublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdalunopreescolaparcialconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculapreescolaparcialpublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdmatriculapreescolaparcialconveniada']; ?></td>
								</tr>
							</tbody>
						</table>
						</div>
					</td>
					<td>
					<div style="border: 1px solid #404040; ">
						<table border="1" bordercolor="#FFFFFF" bordercolor="#FFFFFF" width="100%" class="listagem" height="225px">
							<thead>
								<tr>
									<th colspan="5">Dados - Quantidade de Turmas Novas / Censo</th>
								</tr>	
								<tr>
									<td rowspan="2" style=" background-color:#7BA2B6;">Tipo de Atendimento</td>
									<th colspan="2" style=" background-color:#FABF40;">Educacenso 2011 - Número de Turmas</th>
									<th colspan="2" style=" background-color:#50A56C;">Turmas em 2012 - SITUAÇÃO ATUAL</th>
								</tr>	
								<tr>
									<th style=" background-color:#FABF40;">Rede pública municipal</th>
									<th style=" background-color:#FABF40;">Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
									<th style=" background-color:#50A56C;">Rede pública municipal</th>
									<th style=" background-color:#50A56C;">Instituições comunitárias, confessionais ou filantrópicas sem fins lucrativos conveniadas com o Poder Público</th>
								</tr>
							</thead>
							<tbody>		
								<tr>
									<td style=" background-color:#7BA2B6;">Creche</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmacrechepublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmacrecheconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmacrechepublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmacrecheconveniada']; ?></td>
								</tr>
								<tr>
									<td style=" background-color:#7BA2B6;">Pré-escola</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmapreescolapublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmapreescolaconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmapreescolapublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmapreescolaconveniada']; ?></td>
								</tr>
								<tr>
									<td align="center" style=" background-color:#7BA2B6;">Unificada (matrículas de creche e pré-escola na mesma turma)</td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmaunificadapublica']; ?></td>
									<td align="center" style=" background-color:#FABF40;"><?php echo $rsDadosEscola['ntcqtdturmaunificadaconveniada']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmaunificadapublica']; ?></td>
									<td align="center" style=" background-color:#50A56C;"><?php echo $rsDadosEscola['ntmqtdturmaunificadaconveniada']; ?></td>
								</tr>						
							</tbody>
						</table>
						</div>	
					</td>
				</tr>	
			</table>
		</td>
		<?php if($docid): ?>
			<td valign="top" align="center">
				<?php 
				if( $_SESSION['exercicio'] == date('Y') && $esdid != WF_NOVASTURMAS_EM_DILIGENCIA ){
					$arrOcultar = array('acoes' => true, 'historico'=>false);
				} else {
					$arrOcultar = array('acoes' => false, 'historico'=>false);
				}				
				?>				
				<?php wf_desenhaBarraNavegacao( $docid , array('muncod' => $_SESSION['proinfantil']['muncod']),  $arrOcultar ); ?>				
			</td>
		<?php endif; ?>		
	</tr>
	<tr>
		<td colspan="2" >
			<?php 

			if($acesso['analise'] == "S"){
				$radio = "'<div style=\"white-space: nowrap;\" >
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"A\" id=\"rdn_turid_aprovado_' || t.turid || '\" /> Aprovada <br/>
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"I\" id=\"rdn_turid_indeferido_' || t.turid || '\" /> Indeferida <br/>
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"D\" id=\"rdn_turid_diligenciado_' || t.turid || '\" /> Diligenciada</div>' as radio,";
				$textarea = "COALESCE('<textarea  id=\"parecer' || t.turid || '\" name=\"parecer' || t.turid || '\" cols=\"35\" rows=\"5\" />' || n.anaparecer || '</textarea>','<textarea  id=\"parecer' || t.turid || '\" name=\"parecer' || t.turid || '\" cols=\"35\" rows=\"5\" /></textarea>') || '&nbsp;<input type=\"hidden\" id=\"tipid' || t.turid || '\" name=\"tipid' || t.turid || '\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;vertical-align:top;\" onclick=\"popGuiaParecer(\'2\', \'tipid' || t.turid || '\', \'parecer' || t.turid || '\')\">' AS parecer";
				$salvar = ",'<input type=\"button\" class=\"botao\" name=\"btnsalvar_turma\" value=\"Salvar\" onclick=\"salvarParecerTurma('|| t.turid ||');\">' AS salvar";				
				$arrCab = array("Ação","Nome da Turma","Tipo de Rede","Atendimento","Código INEP","Data Início de Funcionamento","Período de Repasse","Análise","Parecer","");
			} else {
				$radio = "'<div style=\"white-space: nowrap;\" >
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"A\" id=\"rdn_turid_aprovado_' || t.turid || '\" disabled /> Aprovada <br/>
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"I\" id=\"rdn_turid_indeferido_' || t.turid || '\" disabled /> Indeferida <br/>
							<input type=\"radio\" name=\"rdn_turid' || t.turid || '\" value=\"D\" id=\"rdn_turid_diligenciado_' || t.turid || '\" disabled /> Diligenciada</div>' as radio,";
				$textarea = "COALESCE('<textarea  id=\"parecer' || t.turid || '\" name=\"parecer' || t.turid || '\" cols=\"35\" rows=\"5\" readonly=\"readonly\"/>' || n.anaparecer || '</textarea>','<textarea  id=\"parecer' || t.turid || '\" name=\"parecer' || t.turid || '\" cols=\"35\" rows=\"5\" readonly=\"readonly\"/></textarea>') || '&nbsp;<input type=\"hidden\" id=\"tipid' || t.turid || '\" name=\"tipid' || t.turid || '\"><img src=\"../imagens/consultar.gif\" style=\"cursor:pointer;vertical-align:top;\" onclick=\"popGuiaParecer(\'2\', \'tipid' || t.turid || '\', \'parecer' || t.turid || '\')\">' AS parecer";
				$salvar = "";	
				$arrCab = array("Ação","Nome da Turma","Tipo de Rede","Atendimento","Código INEP","Data Início de Funcionamento","Período de Repasse","Análise","Parecer");
			}			
			
			$sql = "SELECT  ('<img src=\"../imagens/consultar.gif\" id=\"' || t.turid ||'\" class=\"abrirTurmaPopup\" style=\"cursor:pointer;\"/> ' ||
							'<img id=\"imgmais' || t.turid ||'\" name=\"imgmais' || t.turid ||'\" src=\"../imagens/mais.gif\" style=\"cursor:pointer;\" onclick=\"abrirTurma('|| t.turid ||')\"/>' || 
							'<img id=\"imgmenos' || t.turid ||'\" name=\"imgmenos' || t.turid ||'\" src=\"../imagens/menos.gif\" style=\"cursor:pointer;display: none;\" onclick=\"fecharTurma('|| t.turid ||')\"/></div>') as acao,
							t.turdsc, tr.tirdescricao, 
							CASE WHEN t.ttuid = 1 THEN 'Exclusivo Creche'
								 WHEN t.ttuid = 2 THEN 'Exclusivo Pré-escola'
								 WHEN t.ttuid = 3 THEN 'Creche e Pré-escola (Misto)'
						    END as tipoatendimento,   
							t.entcodent,  to_char(t.turdtinicio,'DD/MM/YYYY') AS turdtinicio,
							(select proinfantil.calculorepasse2012(t.turdtinicio))||'/12' AS periodo_repasse,
							{$radio}
							{$textarea}
							{$salvar}							    
					FROM proinfantil.turma t
					INNER JOIN proinfantil.mdsalunoatendidopbf a ON a.turid = t.turid
					INNER JOIN proinfantil.tiporede tr ON tr.tirid = t.tirid
					LEFT JOIN proinfantil.analisenovasturmasaprovacao n ON n.turid = t.turid
					WHERE muncod = '{$_SESSION['proinfantil']['muncod']}' AND turstatus = 'A'
						AND turano <= '2012'
					GROUP BY t.turid, turdsc, tr.tirdescricao, t.ttuid, t.entcodent, t.turdtinicio, n.anaparecer, radio
					ORDER BY tr.tirdescricao, t.ttuid";
			
			?>
			<table align="center" border="0" bordercolor="#FFFFFF" width="100%" class="tabela" style="width: 100%;" >
					<tr>
						<td class="subtituloCentro"> Lista de Turmas cadastradas</td>
					</tr>
					<tr>
				    	<td style=" padding-top:0px;" >
							<?php $db->monta_lista($sql,$arrCab,100,10,"N","","N","lista_turma"); ?>
						</td>
				    </tr>				    
			</table>

		</td>
	</tr>
	<?php 
	$sql = "SELECT distinct
			    tur.turdsc,
			    (CASE WHEN mds.timid = 1 THEN 'Pré-Escola' ELSE 'Creche' END) || ' ' || (CASE WHEN mds.titid = 5 THEN 'Integral' ELSE 'Parcial' END) AS nome,
			    tr.tirdescricao,
			    ala.total_alunos, 
			    vr.vrtvalor, 
			    tur.periodo_repasse||'/12',
			    (ala.total_alunos*tur.periodo_repasse*vr.vrtvalor)/12 AS valor_total
			FROM proinfantil.mdsalunoatendidopbf mds
			INNER JOIN (SELECT alaid, sum(alaquantidade) AS total_alunos FROM proinfantil.mdsalunoatendidopbf GROUP BY alaid) as ala on ala.alaid = mds.alaid
			INNER JOIN (SELECT turid, muncod, turstatus, ttuid, turdtinicio, tirid, turdsc, (select proinfantil.calculorepasse2012(turdtinicio)) AS periodo_repasse                      
			            FROM proinfantil.turma) AS tur ON tur.turid = mds.turid
			INNER JOIN proinfantil.tiporede tr ON tr.tirid = tur.tirid                                                
			INNER JOIN (     SELECT vrtvalor, ttuid, tirid, tatid  
			                               FROM proinfantil.valorreferencianovasturmas where vrtano = '{$_SESSION['exercicio']}' and vrtstatus = 'A' ) AS vr on vr.ttuid = tur.ttuid and vr.tirid = tur.tirid and ( case when (mds.titid = 5) then  vr.tatid = 1 else vr.tatid = 2 end ) 
							INNER JOIN                   proinfantil.analisenovasturmasaprovacao ana ON ana.turid = tur.turid
							WHERE muncod = '{$_SESSION['proinfantil']['muncod']}'
							                AND ana.anatipo = 'A'
							                AND tur.turstatus = 'A'
			ORDER BY tur.turdsc";
	
	?>
	<tr>
		<td colspan="2" >
			<table align="center" border="1" bordercolor="#FFFFFF" width="100%" class="listagem">
				<thead>
					<tr>
				    	<th colspan="2">Valor de Repasse para o Município</th>
				    </tr>				    
				    <!--  <tr>
				    	<th>Valor Total:</th>
				    	<td width="75%"><?php echo $valor_total ? number_format($valor_total,2,",",".") : '0,00';?></td>
				    </tr>-->
				    <tr>
				    	<td colspan="4"><?
				    	$cabecalho = array('Turma', 'Etapa', 'Tipo Rede', 'Quantidade de Alunos', 'Valor Unitário', 'Período de Repasse (meses)', 'Valor Total');
						$db->monta_lista_simples($sql, $cabecalho, 50, 10, 'S', '100%', 'S', false, '', '', true );
				    	?></td>
				    </tr>
			    </thead>	
			</table>
	
			<br>
	
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="width:95%">
			    <tr>
			    	<td><br><br>
						<table border="0"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
							<tr>
						    	<td class="center bold" align="center" >
						    		Parecer de Aprovação
						    		<br>
						    		<input type="hidden" name="ntaid" value="<?php echo $ntaid ? $ntaid : '' ?>" />	
						    		<?=campo_textarea('ntapareceraprovacao', 'S', $acesso['analise'], '', 150, 10, '', '', '', '', false, '', '', array('id'=>'ntapareceraprovacao')); ?>
						    		
						    		<input type="hidden" name="tipidParecer" id="tipidParecer" value="">
						    	</td>
						    	<td align="left"  valign="top">
						    		<br>
						    		<img src="../imagens/consultar.gif" style="cursor:pointer;vertical-align: top" onclick="popGuiaParecer('2','tipidParecer','ntapareceraprovacao')">
						    	</td>
						    </tr>
						    <?php if($acesso['analise'] == "S"){ ?>
						    <tr>
						    	<td colspan="2" align="center">
						    		<input type="button" value="Salvar Parecer" class="btnSalvarParecer" />
						    	</td>
						    </tr>
						    <?php } ?>
						</table>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
</form>

<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:100%; width:100%; opacity:0.4; filter:alpha(opacity=40)" >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>

<?php 
$sql_historico = "SELECT 		u.usunome, n.ntapareceraprovacao, to_char(n.ntadata,'DD/MM/YYYY') as data, CASE WHEN n.ntastatus = 'A' THEN 'Ativo' ELSE 'Inativo' END ntastatus
				  FROM 			proinfantil.novasturmasanalise n
				  LEFT JOIN 	seguranca.usuario u ON u.usucpf = n.usucpf 
				  WHERE 		n.muncod = '{$_SESSION['proinfantil']['muncod']}'
				  ORDER BY 		n.ntaid DESC";

$historico = $db->pegaLinha($sql_historico);

if(!empty($historico)){?>
<table border="0" class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" >
	<tr>
		<td style="background-color: #CDCDCD;">
			<div id="hist" class="div_historico" style="float:left;cursor:pointer;width:100%">
				<img src="../imagens/mais.gif" style="cursor:pointer; padding-left: 5px; padding-top: 5px;" id="imgmaishist">
				<img src="../imagens/menos.gif" style="cursor:pointer; display: none; padding-left: 5px; padding-top: 5px;" id="imgmenoshist">
				<center><b>Histórico de Pareceres</b></center>
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

<script>
	<?php if($arrAna): ?>
		<?php foreach($arrAna as $ana): ?>
			<?php
				switch($ana['anatipo']){
					case "A":
						$anatipo = "aprovado";
					break;
					case "I":
						$anatipo = "indeferido";
					break;
					case "D":
						$anatipo = "diligenciado";
					break;
					default:
						$anatipo = "";
				}
			?>
			$("#rdn_turid_<?php echo $anatipo ?>_<?php echo $ana['turid'] ?>").attr("checked",true);
		<?php endforeach; ?>
	<?php endif; ?>
	
	function popGuiaParecer(modid,campoTipo,campoParecer){
		var cod = campoParecer.replace("parecer","");
		var codtip = "";
		
		if(campoParecer != 'ntapareceraprovacao'){
			if(document.getElementById("rdn_turid_aprovado_"+cod).checked) codtip = "1";
			if(document.getElementById("rdn_turid_diligenciado_"+cod).checked) codtip = "2";
			if(document.getElementById("rdn_turid_indeferido_"+cod).checked) codtip = "3";
			
			if(!codtip){
				alert("Primeiro, selecione a Análise (Aprovada, Indeferida ou Diligenciada)");
				document.getElementById("rdn_turid_aprovado_"+cod).focus();
				return false;
			}
		}
		
		w = window.open('?modulo=principal/popGuiaParecer&acao=A&modid='+modid+'&filtraTipo='+codtip+'&campoTipo='+campoTipo+'&campoParecer='+campoParecer,'guiaparecer','scrollbars=yes,location=no,toolbar=no,menubar=no,width=550,height=200,left=250,top=125'); 
		w.focus();	
	}
	
	function carregarParcerAnalise( parecer, tipid, campoTipo, campoParecer ){
		jQuery('[name="'+campoParecer+'"]').val(parecer);
		jQuery('[name="'+campoTipo+'"]').val(tipid);	
	}
</script>