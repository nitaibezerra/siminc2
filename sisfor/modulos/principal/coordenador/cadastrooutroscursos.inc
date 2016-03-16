<?php 
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if($_REQUEST['requisicao']=='salvarOutrosCursos'){
	salvarOutrosCursos($_FILES,$_POST);
	exit();
}

if($_REQUEST['requisicao']=='alterarOutrosCursos'){
	alterarOutrosCursos($_FILES,$_POST);
	exit();
}

if($_REQUEST['requisicao']=='excluirCursoOutros'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirOutrosCursos($_REQUEST['ocuid']);
	listarOutrosCursos(NULL,FASE01);
	exit();
}

if($_REQUEST['requisicao']=='excluirArquivo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirArq($_REQUEST['arqid']);
	exit();
}

if($_REQUEST['requisicao']=='visualizar'){
	header('Content-Type: text/html; charset=iso-8859-1');
	visualizarOutrosCursos($_REQUEST['ocuid'],'json',FASE01);
	exit();
}

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A';</script>";
    exit;
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

if($_REQUEST['ocuid']){
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

if($_REQUEST['ocuid']){ 
	$cursos = visualizarOutrosCursos($_REQUEST['ocuid'],'html',FASE01);
	if($cursos){
		extract($cursos);
	}
}

$seb = carregarDadosSEB();
$secadi = carregarDadosSECADI();
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function salvarOutrosCursos(){
		if(jQuery('#coordid').val() == ''){
			alert('O campo "Secretaria/ Diretoria responsável" é obrigatório!');
			jQuery('#coordid').focus();
			return false;
		}			
		if(jQuery('#ocunome').val() == ''){
			alert('O campo "Nome" é obrigatório!');
			jQuery('#ocunome').focus();
			return false;
		}
		if(jQuery('[name=ocudesc]').val() == ''){
			alert('O campo "Descrição" é obrigatório!');
			jQuery('[name=ocudesc]').focus();
			return false;
		}	
		if(jQuery('#sifqtdvagas').val() == ''){
			alert('O campo "Qtd. Vagas" é obrigatório!');
			jQuery('#sifqtdvagas').focus();
			return false;
		}	
		if(jQuery('#sifvalorloa').val() == ''){
			alert('O campo "Valor LOA" é obrigatório!');
			jQuery('#sifvalorloa').focus();
			return false;
		}	
		if(jQuery('#sifvaloroutras').val() == ''){
			alert('O campo "Valor Outras" é obrigatório!');
			jQuery('#sifvaloroutras').focus();
			return false;
		}			
		if(jQuery('#arquivo').val() == '' && jQuery('#divAnexo').is(':hidden')){
			alert('O campo "Arquivo" é obrigatório!');
			jQuery('#arquivo').focus();
			return false;
		}	
		jQuery('#formulario').submit();
	}

	function excluirCursoOutros(ocuid){
		if(confirm("Deseja realmente excluir o Curso?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A',
				data: { requisicao: 'excluirCursoOutros', ocuid: ocuid},
				async: false,
				success: function(data) {
					alert('Curso excluído com sucesso!');
					jQuery("#divListarOutrosCursos").html(data);
					divCarregado();
			    }
			});
		}
	}

	function excluirArquivo(arqid){
		if(confirm("Deseja realmente excluir o Arquivo?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A',
				data: { requisicao: 'excluirArquivo', arqid: arqid},
				async: false,
				success: function(data) {
					if(trim(data)=='S'){
						alert('Arquivo excluído com sucesso!');
						jQuery("#arquivo").removeAttr("disabled");
						jQuery('#divArquivo').show();
						jQuery('#divAnexo').hide();				
						jQuery("#divArquivo").removeAttr('disabled');
					} else {
						alert('Não foi possível excluir o arquivo!');
					}
					divCarregado();
			    }
			});
		}
	}	

	function visualizarCurso(ocuid){
		jQuery.ajax({
			type: 'POST',
			url: 'sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A',
			dataType: 'json',
			data: { requisicao: 'visualizar', ocuid: ocuid},
			async: false,
			success: function(data) {
				if(data){
					jQuery('#ocuid').val(ocuid);
					jQuery('#ocunome').val(data.ocunome);
					jQuery('[name=ocudesc]').val(data.ocudesc);
					jQuery('[name=coordid]').val(data.coordid);
					jQuery('#sifqtdvagas').val(trim(data.sifqtdvagas));
					jQuery('#sifvalorloa').val(trim(data.sifvalorloa));
					jQuery('#sifvaloroutras').val(trim(data.sifvaloroutras));
					jQuery('#requisicao').val('alterarOutrosCursos');
					if(data.arqid == null){
						jQuery("#arquivo").removeAttr("disabled");
						jQuery('#divArquivo').show();
						jQuery('#divAnexo').hide();				
						jQuery("#divArquivo").removeAttr('disabled');
					} else {
						jQuery("#arquivo").attr("disabled","disabled");
						jQuery('#divArquivo').hide();
						jQuery('#divAnexo').show();
						jQuery('#arqid').html(data.arqid);	
						jQuery("#divArquivo").attr('disabled','disabled');
					} 
				} else {
					jQuery('#coordid').val('');
					jQuery('#ocunome').val('');
					jQuery('[name=ocudesc]').val('');
					jQuery('#sifqtdvagas').val('');
					jQuery('#sifvalorloa').val('');
					jQuery('#sifvaloroutras').val('');
					jQuery('#requisicao').val('salvarOutrosCursos');
					jQuery('#divArquivo').show();
					jQuery('#divAnexo').hide();
					alert("Curso não encontrado!");
				}	
			}
		});
	}

	function verificarSaldoSecretaria(coordid){
		divCarregando();
		jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastrooutroscursos&acao=A',
			data: { requisicao: 'verificarSaldoSecretaria', coordid: coordid, ocuid: jQuery('#ocuid').val()},
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

<?php monta_titulo('Cadastro Outros Cursos',$cies); ?>

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
	<?php if($_REQUEST['ocuid']){ ?>
	<input type="hidden" id="requisicao" name="requisicao" value="alterarOutrosCursos"/>
	<?php } else { ?>
	<input type="hidden" id="requisicao" name="requisicao" value="salvarOutrosCursos"/>
	<?php } ?>
	<input type="hidden" id="ocuid" name="ocuid" value="<?php echo $_REQUEST['ocuid']; ?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>
				<p align="justify" style="line-height: 20px;">
				Informe a Secretaria do MEC e a Diretoria para a qual a proposta de curso será submetida. Informe o nome do curso, uma síntese do seu objetivo principal, 
				o número de vagas a serem disponibilizadas e o valor estimado. Também é obrigatório anexar um documento com a descrição detalhada. Por fim, clique em "Salvar".
				</p>			
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Secretaria/ Diretoria responsável:</td>
			<td>
	        	<?php $sql = "SELECT coordid AS codigo, coordsigla || ' - ' || coorddesc AS descricao FROM catalogocurso2014.coordenacao WHERE (coordsigla ILIKE '%SEB%' OR coordsigla ILIKE '%SECADI%') ORDER BY coordsigla, coorddesc"; ?>
	        	<?php $db->monta_combo('coordid',$sql,$habilitado,'Selecione a Diretoria','verificarSaldoSecretaria','','','490','S','coordid',''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Nome:</td>
			<td><?php echo campo_texto('ocunome', 'S', $habilitado, 'Nome', '73', '255', '', '', '', '', '','id="ocunome"'); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Descrição:</td>
			<td><?php echo campo_textarea('ocudesc', 'S', $habilitado, 'Descrição', '83', '4', '1000'); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Qtd. Vagas:</td>
			<td><?php echo campo_texto('sifqtdvagas', 'S', $habilitado, 'Quantidade de Vagas', '25', '15', '#####', '', '', '', '','id="sifqtdvagas"'); ?></td>
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
			<td class="subtituloDireita">Valor Outras:</td>
			<td><?php echo campo_texto('sifvaloroutras', 'S', $habilitado, 'Valor Outras', '25', '15', '[.###],##', '', '', '', '','id="sifvaloroutras"'); ?></td>
		</tr>					
		<tr id="divArquivo">
			<td class="subtituloDireita">Arquivo:</td>
			<td>
				<?php if($arqid){ 
					echo $arqid;				
				} else { ?>
					<input id="arquivo" type="file" name="arquivo" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?>/>&nbsp;
					<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				<?php } ?>
			</td>
		</tr>	
		<tr id="divAnexo" style="display:none;">
			<td class="subtituloDireita">Arquivo:</td>
			<td id="arqid"></td>
		</tr>	
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarOutrosCursos();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?> />
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
				<?php if(!$_REQUEST['ocuid']){ ?>
				<input type="button" value="Novo" id="btnSalvar" onclick="location.reload();" <?php echo $habilitado == 'N' ? 'disabled="disabled"' : '' ?> />
				<?php } ?>
			</td>
		</tr>				
	</table>	
</form>

<br>
<?php if(!$_REQUEST['ocuid']){ ?>	
	<div id="divListarOutrosCursos"><?php listarOutrosCursos(NULL,FASE01); ?></div>
<?php } ?>	

<?php if($saldo && $saldo <= 0){ ?>
<script language="javascript" type="text/javascript">
	jQuery('#sifvalorloa').attr('disabled','disabled');
</script>
<?php } ?>
