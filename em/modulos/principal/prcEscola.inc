<?php

if(isset($_REQUEST['emiid']) && isset($_REQUEST['entid'])){
	$_SESSION['emiid'] = $_REQUEST['emiid'];
	$_SESSION['entid'] = $_REQUEST['entid'];
}else{
	$_REQUEST['emiid'] = $_SESSION['emiid'];
	$_REQUEST['entid'] = $_SESSION['entid'];
}

if(!$_SESSION['emiid']) {
	die("<script>
			alert('Problemas com variáveis. Refaça o procedimento.');
			window.location='em.php?modulo=inicio&acao=C';
		  </script>");
}

if($_REQUEST['salvaTurno'] == 'true'){	
	if($_SESSION['emiid']){
		$sql = " DELETE FROM em.emiescolaturno WHERE emiid = ".$_SESSION['emiid'].'; ';		
		if(count($_POST['ettid'])){			
			foreach($_POST['ettid'] as $ettid){
				$sql .= " INSERT INTO em.emiescolaturno (ettid, emiid) VALUES ({$ettid}, {$_SESSION['emiid']}); ";
			}
		}	
		$db->executar($sql);
		if($db->commit()){
			die('true');		
		}	
	}
	die('false');
}

if($_REQUEST['salvaJornada'] == 'true'){
	
	if($_SESSION['emiid']){
		
		$sql = "UPDATE em.emiensinomedioinovador SET etjid = {$_REQUEST['etjid']} WHERE emiid = ".$_SESSION['emiid'];
		$db->executar($sql);
		
		if($db->commit()){
			
			$sql = "SELECT 
						etjjornada 
					FROM 
						em.emiensinomedioinovador ei
					INNER JOIN 
						em.emitipojornada tj ON tj.etjid = ei.etjid 
					WHERE 
						ei.emiid = ".$_SESSION['emiid'];
			
			$jornada = $db->pegaUm($sql);
			
			$sql = "SELECT 
						coalesce(sum(emcquantidadealunos),0) as total 
					FROM 
						em.emicenso 
					WHERE 
						entid = '".$_SESSION['entid']."'";
			
			$totalAluno = $db->pegaUm($sql);
			
			if(!empty($totalAluno)){
				
				$sql = "SELECT 
							* 
						FROM 
							em.emireccusteiocapital 
						WHERE 
							emrjornada = {$jornada} 
						AND 
							{$totalAluno} BETWEEN emrqtdinialunos 
						AND 
							emrqtdfinalunos;";
				
				$reccusteiocapital = $db->pegaLinha($sql);			
				echo number_format($reccusteiocapital['emrcusteio']+$reccusteiocapital['emrcapital'], 2, ",", ".");
				
			}else{
				
				echo '<font color="red">Dados não encontrados do censo 2011</font>';						
			}
			die;
		}
	}
	die('false');
}

//Chamada de programa
include  APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/workflow.php";
print "<br/>";

$abacod_tela = 57523;
$url 		 = 'em.php?modulo=principal/prcEscola&acao=A';
$parametros  = null;
$arMnuid 	 = array();
$db->cria_aba($abacod_tela,$url,$parametros, $arMnuid);
	
monta_titulo("PRC {$_SESSION['exercicio']}", "");
montaCabecalhoPRC( $_SESSION['emiid'] );

$docid = criaDocumento( $_SESSION['entid'], $_SESSION['emiid'] );
$esdid = pegaEstadoAtual( $docid );

$ativo = 'N';
if(checkPerfil(array(PERFIL_SUPER_USUARIO, PERFIL_CADASTRADOR)) && in_array($esdid, array(WF_ESDID_EM_PREENCHIMENTO, WF_ESDID_EM_CORRECAO, false))){
	$ativo = 'S';	
}
?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script>
jQuery.noConflict();
function addAcaoAtividade(emiid,mcpid)
{
	jornada = jQuery('select[name=etjid]').val();
	if(jornada == ''){
		alert('Informe a jornada primeiro!');
		jQuery('select[name=etjid]').focus();
		return false;
	}
	
	var janela = window.open("?modulo=principal/popupAcaoGap&acao=A&emiid=" + emiid + "&mcpid=" + mcpid, "prcEscola", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");
	janela.focus();
}

function popupProfissionaisEducacao(emiid,mcpid)
{
	jornada = jQuery('select[name=etjid]').val();
	if(jornada == ''){
		alert('Informe a jornada primeiro!');
		jQuery('select[name=etjid]').focus();
		return false;
	}
	
	var janela = window.open("?modulo=principal/profissionaisEducacao&acao=A&emiid=" + emiid + "&mcpid=" + mcpid, "prcEscola", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=250");
	janela.focus();
}

jQuery(function(){
	jQuery('input[name=ettid[]]').click(function(){
		jQuery.ajax({
			url: 'em.php?modulo=principal/prcEscola&acao=A',
			type: 'post',
			data: 'salvaTurno=true&'+jQuery('#cabecalho_Form').serialize(),
			success: function(e){
				//alert(e);
			}
		});
	});
	jQuery('select[name=etjid]').change(function(){		
		if(this.value){
			jQuery.ajax({
				url: 'em.php?modulo=principal/prcEscola&acao=A',
				type: 'post',
				data: 'salvaJornada=true&etjid='+this.value,
				success: function(e){
					//alert(e);
					if(e != 'false'){
						jQuery('#valor_repasse').html(e);
					}
				}
			});
		}
	});
});
</script>
<?php wfVerificarPendencias(); ?>
<form method="post" name="escolas_Form" id="escolas_Form"  action="">
	<input type="hidden" id="requisicao" name="requisicao" value="salvaProfissionais"/>
	<input type="hidden" id="emiid" name="emiid" value="<?=$_SESSION['emiid']?>"/>
	<table class="tabela" height="100%" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
		<tr>
			<td valign="top" >
				<br />
				<table class="tabela" bgcolor="#f5f5f5" cellspacing="2" cellpadding="4" align="center" >
					<tr bgcolor="#DCDCDC">
						<td align="center" rowspan="2"><b>Macrocampo</b></td>
						<td align="center" colspan="3"><b>Projeto de Reestruturação Curricular</b></td>
					</tr>
					<tr bgcolor="#DCDCDC">										
						<td align="center"><b>Ação/Atividade e Meta</b></td>
						<td align="center"><b>Profissionais</b></td>
						<td align="center"><b>Situação</b></td>
					</tr>
					<?php
					$n = 1;
					$sql = "select 
								* 
							from 
								em.macrocampo 
							where 
								mcpstatus = 'A' 
							order by 
								mcpid";
					
					$arrMacroCampos = $db->carregar($sql);
					$arrMacroCampos = $arrMacroCampos ? $arrMacroCampos : array();
					?>
					<?php foreach($arrMacroCampos as $mc): ?>
						<?php $cor = $n%2 == 0 ? "#e9e9e9" : ""; ?>
						<?php
						$sql = "select 
									m.mcpid,
									m.mcpdsc, 
									g.papid, 
									t.mdoid, 
									b.benid, 
									p.preid 
								from 
									em.macrocampo m
								left join 
									em.emigap g on g.mcpid = m.mcpid and g.emiid = {$_SESSION['emiid']}
								left join 
									em.emimatrizdistribuicaoorcamentargap t on t.papid = g.papid
								left join 
									em.emibeneficiario b on b.benid = g.benid
								left join 
									em.emiprofissionalenvolvido p on p.mcpid = m.mcpid and p.emiid = {$_SESSION['emiid']}
								where 
									m.mcpid = {$mc['mcpid']}";
						
						$rsMacrocampo = $db->pegaLinha($sql);
						
						if((empty($rsMacrocampo['papid']) && empty($rsMacrocampo['mdoid']) && empty($rsMacrocampo['benid']) && empty($rsMacrocampo['preid']))){
							$img = '<img title="Sem preenchimento" alt="Sem preenchimento" style="cursor:pointer;background-color:#FFFFFF" align="absmiddle" src="/imagens/p_vermelho.gif">';
						}else
						if((empty($rsMacrocampo['papid']) || empty($rsMacrocampo['mdoid']) || empty($rsMacrocampo['benid']) || empty($rsMacrocampo['preid']))){
							$img = '<img title="Preenchimento incompleto" alt="Preenchimento incompleto" style="cursor:pointer;background-color:#FFFFFF" align="absmiddle" src="/imagens/p_amarelo.gif">';
						}else{
							$img = '<img title="Preenchimento completo" alt="Preenchimento completo" style="cursor:pointer;background-color:#FFFFFF" align="absmiddle" src="/imagens/p_verde.gif">';
						}
						?>
						<tr bgcolor="<?php echo $cor ?>" >
							<?php if($ativo == 'N'){ $title = 'Visualizar'; }else{ $title = 'Inserir/Editar'; } ?>
							<td><?php echo $n.' - '.$mc['mcpdsc'] ?></td>							
							<td align="center"><img alt="<?php echo $title ?> Ação/Atividade e Meta" title="<?php echo $title ?> Ação/Atividade e Meta" style="cursor:pointer;background-color:#FFFFFF" onclick="addAcaoAtividade(<?php echo $_SESSION["emiid"] ?>,<?php echo $mc["mcpid"] ?>)" align="absmiddle" src="/imagens/gif_inclui.gif"></td>
							<td align="center"><img alt="<?php echo $title ?> Profissionais de Educação" title="<?php echo $title ?> Profissionais de Educação" style="cursor:pointer;background-color:#FFFFFF" onclick="popupProfissionaisEducacao(<?php echo $_SESSION["emiid"] ?>,<?php echo $mc["mcpid"] ?>)" align="absmiddle" src="/imagens/usuario.gif"></td>
							<td align="center"><?php echo $img ?></td>
						</tr>	
						<?php $n++; ?>
					<?php endforeach; ?>
				</table>
				<br />
			</td>
			<td valign="top" align="center" width="100" id="td_workflow">
				<br />				
				<?php wf_desenhaBarraNavegacao( $docid , array("emiidPai" => $_SESSION['emiid']) ); ?>
				<br />				
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr bgcolor="#C0C0C0">
			<td colspan="5">
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>