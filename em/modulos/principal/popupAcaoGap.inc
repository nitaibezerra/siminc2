<?php

if(!$_SESSION['emiid']){
	echo "<script>
			alert('Erro de variável, refaça o procedimento!');
			window.close();
		  </script>";
	exit;
}

if($_REQUEST['mcpid']){
	$_SESSION['mcpid'] = $_REQUEST['mcpid']; 
}else{
	$_REQUEST['mcpid'] = $_SESSION['mcpid'];
	$_REQUEST['emiid'] = $_SESSION['emiid'];
}

if($_REQUEST['requisicao'] == 'recuperaracao'){
	
	$sql = "select 
				* 
			from 
				em.emigap 
			where 
				papid = ".$_REQUEST['papid'];
	$rsAcao = $db->pegaLinha($sql);

	$sql = "select 
				b.* 
			from 
				em.emigap g
			inner join 
				em.emibeneficiario b on b.benid = g.benid
			where 
				g.papid = ".$_REQUEST['papid'];
	$rsBeneficiarios = $db->pegaLinha($sql);
	
	//$rsAcao['papcaoatividade'] = iconv( "UTF-8", "ISO-8859-1", $rsAcao['papcaoatividade']);
	//$rsAcao['papmeta'] = iconv( "UTF-8", "ISO-8859-1", $rsAcao['papmeta']);
	
	$rsAcao['papcaoatividade'] = utf8_encode($rsAcao['papcaoatividade']);
	$rsAcao['papmeta'] = utf8_encode($rsAcao['papmeta']);
	
	$rsAcao['ben'] = $rsBeneficiarios;
	echo simec_json_encode($rsAcao);
	die;
}

if($_REQUEST['requisicao'] == 'excluirAcao'){
	
	$sql  = "delete from em.emimatrizdistribuicaoorcamentargap where papid = ".$_REQUEST['papid'].";";	
	$sql .= "delete from em.emicritricagap where papid = ".$_REQUEST['papid'].";";
	$sql .= "delete from em.emigap where papid = ".$_REQUEST['papid'].";";
	
	$db->executar($sql);
	if($db->commit()){
		die('true');
	}
	die('false');
}

$docid = pegaDocid( $_SESSION['entid'], $_SESSION['emiid'] );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && (in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false)))){
	$ativo = 'S';	
}

if($_REQUEST["requisicao"] == 'cadastrarAcao'){
	
	
	
	$sql = "SELECT 
				count(1) as count 
			FROM 
				workflow.historicodocumento 
			WHERE 
				aedid = '".WF_AEDID_RETURN_PREENCHIMENTO."' 
			AND 
				docid = '".$docid."' ";
	
	$boCorrecao = $db->pegaUm($sql);
	
	$papflagalterado = ($boCorrecao) ? 'true':'false';
	
	$dados = $_REQUEST;
	
	$dados["papcaoatividade"] = substr($dados["papcaoatividade"], 0, 1000);
	$dados["papmeta"] = substr($dados["papmeta"], 0, 1000);
	
	if( $dados["papid"] ){
		
		$sql = "UPDATE 
					em.emigap
				SET
					papcaoatividade = '{$dados["papcaoatividade"]}',
					papmeta = '{$dados["papmeta"]}',
					papflagalterado = {$papflagalterado},
					papexercicio = '{$_SESSION['exercicio']}' 
				WHERE
					papid = {$dados["papid"]}";
		
		$db->executar( $sql );
		
	}else{
		
		$sql = "INSERT INTO em.emigap( mcpid, 
								  emiid, 
								  papcaoatividade, 
								  papmeta, 
								  papflagalterado,
								  papstatus ,
								  papexercicio )
						  VALUES( '".$dados["mcpid"]."', 
								  '".$dados["emiid"]."', 
								  ".(($dados["papcaoatividade"])?"'".$dados["papcaoatividade"]."'":"NULL").", 
								  ".(($dados["papmeta"])?"'".$dados["papmeta"]."'":"NULL").", 
								  ".$papflagalterado.", 
								  'A' ,
								  '{$_SESSION['exercicio']}' ) returning papid;";
							
		$dados["papid"] = $db->pegaUm( $sql );  
	}	
	$db->commit();

	if(!$dados["papid"]){
		$dados["papid"] = $id;
	}
	
	// Salva beneficiarios
	$sql = "select benid from em.emigap where papid = {$dados['papid']};";
	$benid = $db->pegaUm($sql);
	
	$benqtd1anovesp = $dados['vesp_1'] === '0' || !empty($dados['vesp_1']) ? str_replace(".","",$dados['vesp_1']) : "null";
	$benqtd1anomat  = $dados['mat_1']  === '0' || !empty($dados['mat_1'])  ? str_replace(".","",$dados['mat_1'])  : "null";
	$benqtd1anonot  = $dados['not_1']  === '0' || !empty($dados['not_1'])  ? str_replace(".","",$dados['not_1'])  : "null";
	$benqtd2anovesp = $dados['vesp_2'] === '0' || !empty($dados['vesp_2']) ? str_replace(".","",$dados['vesp_2']) : "null";
	$benqtd2anomat  = $dados['mat_2']  === '0' || !empty($dados['mat_2'])  ? str_replace(".","",$dados['mat_2'])  : "null";
	$benqtd2anonot  = $dados['not_2']  === '0' || !empty($dados['not_2'])  ? str_replace(".","",$dados['not_2'])  : "null";
	$benqtd3anovesp = $dados['vesp_3'] === '0' || !empty($dados['vesp_3']) ? str_replace(".","",$dados['vesp_3']) : "null";
	$benqtd3anomat  = $dados['mat_3']  === '0' || !empty($dados['mat_3'])  ? str_replace(".","",$dados['mat_3'])  : "null";
	$benqtd3anonot  = $dados['not_3']  === '0' || !empty($dados['not_3'])  ? str_replace(".","",$dados['not_3'])  : "null";
	$benqtd4anovesp = $dados['vesp_4'] === '0' || !empty($dados['vesp_4']) ? str_replace(".","",$dados['vesp_4']) : "null";
	$benqtd4anomat  = $dados['mat_4']  === '0' || !empty($dados['mat_4'])  ? str_replace(".","",$dados['mat_4'])  : "null";
	$benqtd4anonot  = $dados['not_4']  === '0' || !empty($dados['not_4'])  ? str_replace(".","",$dados['not_4'])  : "null";
	
	if($benid){
		
		$sql = "update
					em.emibeneficiario
				set
					benqtd1anovesp = $benqtd1anovesp,
					benqtd1anomat = $benqtd1anomat,
					benqtd1anonot = $benqtd1anonot,
					benqtd2anovesp = $benqtd2anovesp,
					benqtd2anomat = $benqtd2anomat,
					benqtd2anonot = $benqtd2anonot,
					benqtd3anovesp = $benqtd3anovesp,
					benqtd3anomat = $benqtd3anomat,
					benqtd3anonot = $benqtd3anonot,
					benqtd4anovesp = $benqtd4anovesp,
					benqtd4anomat = $benqtd4anomat,
					benqtd4anonot = $benqtd4anonot
				where
					benid = $benid";
		
		$db->executar( $sql );
		
	}else{
		
		$sql = "insert into 
					em.emibeneficiario 
				(benqtd1anovesp,benqtd1anomat,benqtd1anonot,
				 benqtd2anonot,benqtd2anovesp,benqtd2anomat,
				 benqtd3anovesp,benqtd3anomat,benqtd3anonot,
				 benqtd4anovesp,benqtd4anomat,benqtd4anonot) 
					values 
				($benqtd1anovesp,$benqtd1anomat,$benqtd1anonot,
				 $benqtd2anonot,$benqtd2anovesp,$benqtd2anomat,
				 $benqtd3anovesp,$benqtd3anomat,$benqtd3anonot,
				 $benqtd4anovesp,$benqtd4anomat,$benqtd4anonot)
					returning benid;";
				 
		$benid = $db->pegaUm($sql);
		$sql = "update
					em.emigap
				set
					benid = $benid
				where
					papid = {$dados['papid']}";
		$db->executar( $sql );
	}
	
	$db->commit();
	$db->sucesso( "principal/popupAcaoGap" );
	
	header("Location: em.php?modulo=principal/popupAcaoGap&acao=A");
}
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<script type="text/javascript" src="../includes/funcoes.js"></script>	    	    
	    <script type="text/javascript" src="/includes/estouvivo.js"></script>
	    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
		<script type="text/javascript" src="/includes/prototype.js"></script>	    
	    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script>
			jQuery.noConflict();
			function salvaAcao()
			{
				var erro = 0;
				if(document.formulario.papcaoatividade.value == ''){
					alert('Informe a Atividade!');
					erro = 1;
					return false;
				}
				if(document.formulario.papmeta.value == ''){
					alert('Informe a Meta!');
					erro = 1;
					return false;
				}
				if(erro == 0){
					document.formulario.submit();
				}
			}
			function preencheMatrizGAP( papid )
			{				
				window.location = 'em.php?modulo=principal/matrizGap&acao=A&papid=' + papid;				
			}
			function preencheCriticaGap( papid ) 
			{
				var janela = window.open("?modulo=principal/popupCriticaGap&acao=A&papid=" + papid, "critica", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
				janela.focus();
			}
			function alterarAcao(papid)
			{
				jQuery.ajax({
					url: 'em.php?modulo=principal/popupAcaoGap&acao=A',
					type: 'post',
					dataType: 'json',
					data: 'requisicao=recuperaracao&papid='+papid,
					success: function(e){		
												
						jQuery('#papmeta').val(e.papmeta);
						jQuery('#papcaoatividade').val(e.papcaoatividade);
						jQuery('#mcpid').val(e.mcpid);
						jQuery('#emiid').val(e.emiid);
						jQuery('#emiid').val(e.emiid);
						jQuery('#papid').val(e.papid);
						
						if(e.ben){
						
							jQuery('input[name=mat_1]').val(e.ben.benqtd1anomat);
							jQuery('input[name=vesp_1]').val(e.ben.benqtd1anovesp);
							jQuery('input[name=not_1]').val(e.ben.benqtd1anonot);
							
							jQuery('input[name=mat_2]').val(e.ben.benqtd2anomat);
							jQuery('input[name=vesp_2]').val(e.ben.benqtd2anovesp);
							jQuery('input[name=not_2]').val(e.ben.benqtd2anonot);
							
							jQuery('input[name=mat_3]').val(e.ben.benqtd3anomat);
							jQuery('input[name=vesp_3]').val(e.ben.benqtd3anovesp);
							jQuery('input[name=not_3]').val(e.ben.benqtd3anonot);
							
							jQuery('input[name=mat_4]').val(e.ben.benqtd4anomat);
							jQuery('input[name=vesp_4]').val(e.ben.benqtd4anovesp);
							jQuery('input[name=not_4]').val(e.ben.benqtd4anonot);		
												
						}else{
						
							jQuery("[name^='mat_']").val('');
							jQuery("[name^='vesp_']").val('');
							jQuery("[name^='not_']").val('');
						}
					}
				});
			}
			
			function excluirAcao(papid)
			{
				if(confirm('Deseja excluir a ação?')){
					jQuery.ajax({
						url: 'em.php?modulo=principal/popupAcaoGap&acao=A',
						type: 'post',
						data: 'requisicao=excluirAcao&papid='+papid,
						success: function(e){
							if(e == 'true'){						
								alert('Ação excluída com sucesso.');
								document.location.href='em.php?modulo=principal/popupAcaoGap&acao=A';
							}
						}
					});				
				}
			}
		</script>
		<!--  onunload="window.opener.location.reload();" -->
	</head>
	<body>
		<?php
		monta_titulo("Ação / Atividade e Meta", "PRC 2012");
		echo montaCabecalhoPRC($_SESSION['emiid'], $_REQUEST['mcpid'], null, false);
		echo "<br/>";
		
		$abacod_tela = 57532;
		$url 		 = 'em.php?modulo=principal/popupAcaoGap&acao=A';
		$parametros  = null;
		$arMnuid 	 = array(11188, 11189);
		$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
		?>		
		<form id="formulario" name="formulario" method="post" action="">
			<input type="hidden" value="cadastrarAcao" name="requisicao"/>
			<input type="hidden" value="<?=$_REQUEST['mcpid'];?>" name="mcpid"/>
			<input type="hidden" value="<?=$_REQUEST['emiid'];?>" name="emiid"/>
			<input type="hidden" value="<?=$_REQUEST['papid'];?>" name="papid" id="papid">			
			<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
				<tr>
					<td class="subtitulodireita">Ação / Atividade</td>
					<td><?php echo campo_textarea( 'papcaoatividade', 'S', $ativo, '', 80, 5, 1000, '', 0, '' ); ?></td>
				</tr>
				<tr>
					<td class="subtitulodireita">Meta</td>
					<td><?php echo campo_textarea( 'papmeta', 'S', $ativo, '', 80, 5, 1000, '', 0, '' ); ?></td>
				</tr>
				
				<tr>
					<td bgcolor="#e9e9e9" align="center" colspan="7" ">
						<b>Beneficiários <br /> Número de Alunos Beneficiários</b>
					</td>
				</tr>
				<tr>
					<td colspan="7" ">
						<table bgcolor="#f5f5f5" width="100%" cellspacing="1" cellpadding="1" align="center" >
							<tr bgcolor="#e9e9e9">
								<td colspan="3" width="25%" align="center" ><b>1º Ano</b></td>
								<td colspan="3" width="25%" align="center" ><b>2º Ano</b></td>
								<td colspan="3" width="25%" align="center" ><b>3º Ano</b></td>
								<td colspan="3" width="25%" align="center" ><b>4º Ano</b></td>
								<!-- td width="10%" rowspan="2" align="center" ><b>Total</b></td -->
							</tr>
							<tr>
								<td align="center" >Mat.</td>
								<td align="center" >Vesp.</td>
								<td align="center" >Not.</td>
								<td align="center" >Mat.</td>
								<td align="center" >Vesp.</td>
								<td align="center" >Not.</td>
								<td align="center" >Mat.</td>
								<td align="center" >Vesp.</td>
								<td align="center" >Not.</td>
								<td align="center" >Mat.</td>
								<td align="center" >Vesp.</td>
								<td align="center" >Not.</td>
							</tr>
							<?php $width = 6; ?>
							<tr>
								<td align="center" ><?php echo campo_texto("mat_1","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
								<td align="center" ><?php echo campo_texto("vesp_1","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
								<td align="center" ><?php echo campo_texto("not_1","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(1)") ?></td>
								<td align="center" ><?php echo campo_texto("mat_2","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
								<td align="center" ><?php echo campo_texto("vesp_2","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
								<td align="center" ><?php echo campo_texto("not_2","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(2)") ?></td>
								<td align="center" ><?php echo campo_texto("mat_3","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
								<td align="center" ><?php echo campo_texto("vesp_3","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
								<td align="center" ><?php echo campo_texto("not_3","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(3)") ?></td>
								<td align="center" ><?php echo campo_texto("mat_4","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(4)") ?></td>
								<td align="center" ><?php echo campo_texto("vesp_4","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(4)") ?></td>
								<td align="center" ><?php echo campo_texto("not_4","N",$ativo,"",$width,"20","[.###]","","right","","","","totalBeneficiarios(4)") ?></td>
							</tr>
						</table>
						<br/>
					</td>
				</tr>				
				<tr bgcolor="#C0C0C0">
					<td colspan="2">
						<input type="button" value="Salvar" onclick="salvaAcao();" style="cursor: pointer;" <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?>/>						
					</td>
				</tr>
				<tr><td colspan="2" class="subtitulocentro">Lista de Ações/Atividades e Metas</td></tr>
			</table>
		</form>
		<?php 
		
			if($ativo == 'N'){
				$title = 'Visualizar';
			}else{
				$title = 'Preencher';
			}
		
			$btnAcao = '';
			if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
				$btnAcao .= "<img src=\"/imagens/check_p.gif\" onclick=\"alterarAcao(' || papid || ');\" style=\"cursor:pointer;\" title=\"Alterar\">
							 <img src=\"/imagens/exclui_p.gif\" onclick=\"excluirAcao(' || papid || ');\" style=\"cursor:pointer;\" title=\"Excluir\">";
			}
			
			if(checkPerfil(array(PERFIL_ANALISTA_SEDUC, PERFIL_SUPER_USUARIO)) && $esdid == WF_ESDID_EM_ANALISE_SEDUC){
				$btnAcao .= "<a href=\"em.php?modulo=principal/popupCriticaGap&acao=A&papid=' || papid || '\"><img title=\"{$title} Crítica\" alt=\"{$title} Crítica\" style=\"cursor: pointer;\" src=\"/imagens/atencao.png\"></a>";	
			}elseif($esdid != WF_ESDID_EM_PREENCHIMENTO && checkPerfil(array(PERFIL_CADASTRADOR))){
				$btnAcao .= "<a href=\"em.php?modulo=principal/popupCriticaGap&acao=A&papid=' || papid || '\"><img title=\"{$title} Crítica\" alt=\"{$title} Crítica\" style=\"cursor: pointer;\" src=\"/imagens/atencao.png\"></a>";
			}
		
			if($_REQUEST['mcpid']){
				
				$sql = "SELECT
							'<center>
								<span style=\'white-space:nowrap;\' >
								{$btnAcao}
								<a href=\"em.php?modulo=principal/matrizGap&acao=A&papid=' || papid || '\"><img border=\"0\" title=\" {$title} Matriz\" alt=\" {$title} Matriz\" style=\"cursor: pointer;\" src=\"/imagens/ico_config.gif\"></a>							
								</span>
							</center>' as acao,
							trim(papcaoatividade) as atividade,
							trim(papmeta) as meta
						FROM
							em.emigap
						WHERE
							mcpid = {$_REQUEST['mcpid']} AND
							emiid = {$_REQUEST['emiid']} AND
							papstatus = 'A'
						ORDER BY
							papid DESC";
				
				$montaCabecalhoPRC = array( "Ação", "Ação/Atividade", "Meta" );
				$db->monta_lista( $sql, $montaCabecalhoPRC, 5, 10, 'N','center', '', '', array('100px'), '' );
				
			}else{
				
				echo "<script>
						alert('Erro de variável, refaça o procedimento!');
						window.close();
					  </script>";
			}
		?>
		<center>
			<p><input type="button" value="Fechar" onclick="window.close()" /></p>
		</center>
	</body>
</html>