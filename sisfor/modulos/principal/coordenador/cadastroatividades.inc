<?php 
if($_REQUEST['requisicao']=='salvarOutrasAtividades'){
	salvarOutrasAtividades($_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarOutrasAtividades'){
	alterarOutrasAtividades($_POST);
	exit();
}

if($_REQUEST['requisicao']=='excluirAtividade'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirOutrasAtividades($_REQUEST);
	listarOutrasAtividades(NULL,FASE01);
	exit();
}

if($_REQUEST['requisicao']=='visualizar'){
	header('Content-Type: text/html; charset=iso-8859-1');
	visualizarOutrasAtividades($_REQUEST['oatid'],'json',FASE01);
	exit();
}

if($_REQUEST['requisicao']=='verificarSaldoSecretaria'){	
	header('Content-Type: text/html; charset=iso-8859-1');
	verificarSaldoSecretaria($_REQUEST);
	exit();	
}

if($_REQUEST['requisicao']=='atualizarSaldo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarSaldo($_REQUEST);
	exit();	
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}
	
if($_REQUEST['oatid']){
	$propostaies = visualizarPropostaIES($_REQUEST);
	extract($propostaies);
	
	if($coordenacao){
		$coord = verificarCoordenacao($coordenacao);
	}
	
	$saldoseb = verificarSaldoSEB();
	$saldosecadi = verificarSaldoSECADI();

	if($coord == 'seb'){
		$saldo = $saldoseb;
	} else {
		$saldo = $saldosecadi;
	} 	
} else {
	$saldoseb = verificarSaldoSEB();
	$saldosecadi = verificarSaldoSECADI();	
}

if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}

if($_SESSION['sisfor']['unicod']){
	$docid = pegarDocidPlanejamento($_SESSION['sisfor']['unicod']);
	$esdid = pegarEstadoDocumento($docid);
} 

if($esdid == WF_PLAN_ANALISE_MEC || in_array(PFL_EQUIPE_MEC,$perfil)){
	$habilitado = "N";
} else {
	$habilitado = "S";
}

if($_REQUEST['oatid']){ 
	$atividades = visualizarOutrasAtividades($_REQUEST['oatid'],'html',FASE01);
	extract($atividades);
}

$seb = carregarDadosSEB();
$secadi = carregarDadosSECADI();
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function salvarOutrasAtividades(){
		if(jQuery('#coordid').val() == ''){
			alert('O campo "Secretaria interessada" é obrigatório!');
			jQuery('#coordid').focus();
			return false;
		}		
		if(jQuery('#oatnome').val() == ''){
			alert('O campo "Nome" é obrigatório!');
			jQuery('#oatnome').focus();
			return false;
		}
		if(jQuery('[name=oatdesc]').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			jQuery('[name=oatdesc]').focus();
			return false;
		}	
		if(jQuery('#oatanoproposta').val() == ''){
			alert('O campo "Ano Proposta" é obrigatório!');
			jQuery('#oatanoproposta').focus();
			return false;
		}			
		if(jQuery('#sifvalorloa').val() == ''){
			alert('O campo "Valor LOA" é obrigatório!');
			jQuery('#sifvalorloa').focus();
			return false;
		}	
		jQuery('#formulario').submit();
	}

	function excluirAtividade(oatid){
		if(confirm("Deseja realmente excluir a Atividade?")){
			divCarregando();
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A',
			data: { requisicao: 'excluirAtividade', oatid: oatid, siftipoplanejamento: 1},
			async: false,
			success: function(data) {
					alert('Atividade excluída com sucesso!');
					jQuery("#divListarOutrasAtividades").html(data);
					divCarregado();
		    	}
			});
		}
	}

	function visualizarAtividade(oatid){
		jQuery.ajax({
			type: 'POST',
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A',
			dataType: 'json',
			data: { requisicao: 'visualizar', oatid: oatid},
			async: false,
			success: function(data){
				if(data){
					jQuery('#oatid').val(oatid);
					jQuery('#oatnome').val(data.oatnome);
					jQuery('#oatatividade').val(trim(data.oatatividade));
					jQuery('[name=oatdesc]').val(data.oatdesc);
					jQuery('#coordid').val(data.coordid);
					jQuery('#sifvalorloa').val(trim(data.sifvalorloa));
					jQuery('#oatanoproposta').val(trim(data.oatanoproposta));
					jQuery('#requisicao').val('alterarOutrasAtividades');
				} else {
					jQuery('#coordid').val('');
					jQuery('#oatnome').val('');
					jQuery('[name=oatdesc]').val('');
					jQuery('#sifvalorloa').val('');
					jQuery('#oatanoproposta').val('');
					jQuery('#oatatividade').val('');
					jQuery('#requisicao').val('salvarOutrasAtividades');
					alert("Atividade não encontrada!");
				}	
			}
		});
	}

	function verificarSaldoSecretaria(coordid){
		divCarregando();
		jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A',
			data: { requisicao: 'verificarSaldoSecretaria', coordid: coordid, oatid: jQuery('#oatid').val()},
			async: false,
			success: function(data){
				if(trim(data) == 'N'){
					alert('Não há mais saldo disponível nesta Secretaria!');	
					jQuery('#coordid').focus();
					jQuery('#coordid').val('');		
					jQuery('#btnSalvar').attr('disabled','disabled');
				} else {
					jQuery('#btnSalvar').removeAttr('disabled','disabled');
				}
				divCarregado();
		    }
		});
	}

	function atualizarSaldo(loa){
		if(jQuery('#coordid').val() == ''){
			alert('Selecione a secretaria!');
			jQuery('#sifvalorloa').val('');
			jQuery('#coordid').focus();
		} else {
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades&acao=A',
				dataType: 'json',				
				data: { requisicao: 'atualizarSaldo', loa: loa, coordid: jQuery('#coordid').val()},
				async: false,
				success: function(data){
					if(trim(data.resultado) == 'N'){
						jQuery('#saldo'+data.secretaria).html('R$ ' + data.saldo);						
						alert('Não há mais saldo disponível nesta Secretaria!');
						jQuery('#sifvalorloa').focus();
						jQuery('#btnSalvar').attr('disabled','disabled');						
					} else {
						jQuery('#saldo'+data.secretaria).html('R$ ' + data.saldo);
						jQuery('#btnSalvar').removeAttr('disabled','disabled');
					}
			    }
			});
		}
	}
</script>

<br>

<?php monta_titulo('Cadastro Outras Atividades',$cies); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro" width="25%">&nbsp;</td>
		<td class="subtituloEsquerda" width="25%">SEB</td>
		<td class="subtituloEsquerda" width="25%">SECADI</td>
	</tr>
	<tr>
		<td class="subtituloDireita" width="25%">Saldo Disponível:</td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $seb['saldoies']; ?></td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $secadi['saldoies']; ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita" width="25%">Recurso Gasto:</td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $seb['valoratividade']; ?></td>
		<td class="subtituloCentro">&nbsp;R$ <?php echo $secadi['valoratividade']; ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita" width="25%">Saldo:</td>
		<td id="saldoseb" class="subtituloCentro">&nbsp;R$ <?php echo formata_valor(desformata_valor($seb['saldoies']) - desformata_valor($seb['valoratividade'])); ?></td>
		<td id="saldosecadi" class="subtituloCentro">&nbsp;R$ <?php echo formata_valor(desformata_valor($secadi['saldoies']) - desformata_valor($secadi['valoratividade'])); ?></td>		
	</tr>		
</table>

<br>

<form id="formulario" method="post" name="formulario" action="" enctype="multipart/form-data">
	<input type="hidden" id="unicod" name="unicod" value="<?php echo $_SESSION['sisfor']['unicod'];?>"/>
	<input type="hidden" id="siftipoplanejamento" name="siftipoplanejamento" value="<?php echo FASE01; ?>"/>
	<?php if($_REQUEST['oatid']){ ?>
	<input type="hidden" id="requisicao" name="requisicao" value="alterarOutrasAtividades"/>
	<?php } else { ?>
	<input type="hidden" id="requisicao" name="requisicao" value="salvarOutrasAtividades"/>
	<?php } ?>
	<input type="hidden" id="oatid" name="oatid" value="<?php echo $_REQUEST['oatid']; ?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>
				<p align="justify" style="line-height: 20px;">
				Neste caso, indique apenas a Secretaria interessada, escolha a atividade e informe o valor dos recursos orçamentários a serem utilizados. 
				Se as atividades forem realizadas integralmente com recursos da Instituição, não é preciso cadastra-las no SisFor. 
				Por fim, clique em "Salvar". As atividades inseridas vão aparecendo na linha inferior. Para excluir uma atividade cadastrada, clique no ícone <img border="0" style="cursor:pointer;" src="../imagens/excluir.gif" title="EXCLUIR ATIVIDADE">. 
				Para edita-la, clique em <img border="0" style="cursor:pointer;" src="../imagens/alterar.gif" title="ALTERAR ATIVIDADE"> e não esqueça de salvar as alterações.
				</p>			
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita" width="25%">Secretaria interessada:</td>
			<td>
	        	<?php $sql = "SELECT coordid AS codigo, coordsigla || ' - ' || coorddesc AS descricao FROM catalogocurso2014.coordenacao WHERE (coordsigla ILIKE '%SEB%' OR coordsigla ILIKE '%SECADI%') ORDER BY coordsigla, coorddesc"; ?>
	        	<?php $db->monta_combo('coordid',$sql,$habilitado,'Selecione a Secretaria','verificarSaldoSecretaria','','','490','S','coordid',''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Atividade:</td>
			<td>
	        	<?php 	
	        		$arrayAtividade = array(
							array('codigo' => '1', 'descricao' => 'Seminário SEB'),
							array('codigo' => '2', 'descricao' => 'Seminário SECADI'),
							array('codigo' => '3', 'descricao' => 'Custeio ComFor')); ?>
	        	<?php $db->monta_combo('oatatividade',$arrayAtividade,$habilitado,'Selecione a Atividade','','','','490','S','oatatividade',''); ?>			
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Nome:</td>
			<td><?php echo campo_texto('oatnome', 'N', $habilitado, 'Nome', '63', '250', '', '', '', '', '','id="oatnome"'); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo campo_textarea('oatdesc', 'S', $habilitado, 'Descrição', '83', '4', '2000'); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Ano Proposta:</td>
			<td><?php echo campo_texto('oatanoproposta', 'S', $habilitado, 'Ano Proposta', '25', '4', '####', '', '', '', '','id="oatanoproposta"','','2014'); ?></td>
		</tr>		
		<tr>
			<td class="subtituloDireita">Valor LOA:</td>
			<td>
				<?php echo campo_texto('sifvalorloa', 'S', $habilitado, 'Valor LOA', '25', '15', '[.###],##', '', '', '', '','id="sifvalorloa"','','','atualizarSaldo(this.value);'); ?>
				<?php if($saldo && $saldo <= 0){ ?>
				&nbsp;<b>Não há saldo suficiente de recursos LOA.</b>
				<?php } ?>			
			</td>
		</tr>	
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarOutrasAtividades();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/>
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
				<?php if(!$_REQUEST['oatid']){ ?>
				<input type="button" value="Novo" id="btnSalvar" onclick="location.reload();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/>
				<?php } ?>
			</td>
		</tr>				
	</table>	
</form>

<br>
<?php if(!$_REQUEST['oatid']){ ?>		
	<div id="divListarOutrasAtividades"><?php listarOutrasAtividades(NULL,FASE01); ?></div>
<?php } ?>

<?php if($saldo && $saldo <= 0){ ?>
<script language="javascript" type="text/javascript">
	jQuery('#sifvalorloa').attr('disabled','disabled');
</script>
<?php } ?>