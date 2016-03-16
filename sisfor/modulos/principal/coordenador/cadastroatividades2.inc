<?php 
if($_REQUEST['requisicao']=='salvarOutrasAtividades'){
	salvarOutrasAtividades($_POST,$_FILES);
	exit();
}

if($_REQUEST['requisicao']=='alterarOutrasAtividades'){
	alterarOutrasAtividades($_POST,$_FILES);
	exit();
}

if($_REQUEST['requisicao']=='excluirAtividade'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirOutrasAtividades($_REQUEST);
	listarOutrasAtividades(NULL,FASE02);
	exit();
}

if($_REQUEST['requisicao']=='excluirArquivo'){
	header('Content-Type: text/html; charset=iso-8859-1');
	excluirArq($_REQUEST['arqid']);
	exit();
}

if($_REQUEST['requisicao']=='visualizar'){
	header('Content-Type: text/html; charset=iso-8859-1');
	visualizarOutrasAtividades($_REQUEST['oatid'],'json',FASE02);
	exit(); 
}

if($_GET['download'] == 'S'){
	$file = new FilesSimec();
    $arquivo = $file->getDownloadArquivo($_GET['arqid']);
    echo"<script>window.location.href = 'sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A';</script>";
    exit;
}

if($_REQUEST['requisicao']=='atualizarSaldoPlanejamento2'){
	header('Content-Type: text/html; charset=iso-8859-1');
	atualizarSaldoPlanejamento2($_REQUEST);
	exit();	
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}
	
if($_SESSION['usucpf']){
	$perfil = pegarPerfil($_SESSION['usucpf']);
}

if($_SESSION['sisfor']['unicod']){
	$docidplan2 = pegarDocidPlanejamento2($_SESSION['sisfor']['unicod']);
	$esdidplan2 = pegarEstadoDocumento($docidplan2);
} 

if($esdidplan2 == WF_PLAN_ANALISE_MEC2 || in_array(PFL_EQUIPE_MEC,$perfil)){
	$habilitado = "N";
} else {
	$habilitado = "S";
}

if($_REQUEST['oatid']){ 
	$atividades = visualizarOutrasAtividades($_REQUEST['oatid'],'html',FASE02);
	extract($atividades);
}

$planejamento = carregarDadosPlanejamento();
extract($planejamento);

$saldo = $saldodisp - $saldoacoes; ?>

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

		if(jQuery('#oatatividade').val() == ''){
			alert('O campo "Atividade" é obrigatório!');
			jQuery('#oatatividade').focus();
			return false;
		}

		if(jQuery('#ateid').val() == ''){
			alert('O campo "Área Temática" é obrigatório!');
			jQuery('#ateid').focus();
			return false;
		}

		if(jQuery('#oateducaoinfantil').attr('checked') == false && jQuery('#oatfundamentalinicial').attr('checked') == false && jQuery('#oatfundamentalfinal').attr('checked') == false && jQuery('#oatensinomedio').attr('checked')== false && jQuery('#oatnaoseaplica').attr('checked') == false){
			alert('Selecione ao menos uma "Etapa"!');
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
			var txt;
			var r = confirm("O Campo Valor LOA está em branco. Tem certeja que deseja salvar?");
			
		}
		
		if(jQuery('#arquivo').val() == '' && jQuery('#divAnexo').is(':hidden')){
			alert('O campo "Arquivo" é obrigatório!');
			jQuery('#arquivo').focus();
			return false;
		}			
		jQuery('#formulario').submit();
	}

	function excluirAtividade(oatid){
		if(confirm("Tem certeza de que deseja excluir esta atividade?")){
			divCarregando();
			jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A',
			data: { requisicao: 'excluirAtividade', oatid: oatid, siftipoplanejamento: 2},
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
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A',
			dataType: 'json',
			data: { requisicao: 'visualizar', oatid: oatid},
			async: false,
			success: function(data) {
				if(data){
					jQuery('#oatid').val(oatid);
					jQuery('#oatnome').val(data.oatnome);
					jQuery('#oatatividade').val(trim(data.oatatividade));
					jQuery('[name=oatdesc]').val(data.oatdesc);
					jQuery('#ateid').val(trim(data.ateid));
					jQuery('#coordid').val(data.coordid);
					jQuery('#sifvalorloa').val(trim(data.sifvalorloa));
					jQuery('#sifvaloroutras').val(trim(data.sifvaloroutras));
					jQuery('#oatanoproposta').val(trim(data.oatanoproposta));					
					jQuery('#requisicao').val('alterarOutrasAtividades');
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
					jQuery('#oatnome').val('');
					jQuery('[name=oatdesc]').val('');
					jQuery('#sifvalorloa').val('');
					jQuery('#sifvaloroutras').val('');
					jQuery('#oatanoproposta').val('');
					jQuery('#oatatividade').val('');
					jQuery('#requisicao').val('salvarOutrasAtividades');
					alert("Atividade não encontrada!");
				}	
			}
		});
	}

	function atualizarSaldo(loa){
		jQuery.ajax({
			url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A',
			dataType: 'json',				
			data: { requisicao: 'atualizarSaldoPlanejamento2', loa: loa},
			async: false,
			success: function(data){
				if(trim(data.resultado) == 'N'){
					jQuery('#saldo').html('R$ ' + data.saldo);
					alert('Não há mais saldo disponível!');
					jQuery('#sifvalorloa').focus();
					jQuery('#btnSalvar').attr('disabled','disabled');
				} else {
					jQuery('#saldo').html('R$ ' + data.saldo);
					jQuery('#btnSalvar').removeAttr('disabled','disabled');
				}
		    }
		});
	}


	function excluirArquivo(arqid){
		if(confirm("Deseja realmente excluir o Arquivo?")){
			divCarregando();
			jQuery.ajax({
				url: 'sisfor.php?modulo=principal/coordenador/cadastroatividades2&acao=A',
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

    function carregarListaCustoAtividade(idImg, sifid){
        var img = $( '#'+idImg );
        var tr_nome = 'listaCusto_'+ sifid;
        var td_nome = 'trA_'+ sifid;

        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            exibirListaCustoAtividade(sifid, td_nome);
        }
        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
            $('#'+tr_nome).css('display','');
            img.attr('src','../imagens/menos.gif');
        } else {
            $('#'+tr_nome).css('display','none');
            img.attr('src','/imagens/mais.gif');
        }
    }
</script>

<br>

<?php

$recursoGasto = pegaValorNaoAplicadoFase2();

monta_titulo('Cadastro Outras Atividades - Fase 2',$cies); ?>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloCentro" width="25%">&nbsp;</td>
		<td class="subtituloEsquerda" width="25%">SEB / SECADI</td>
	</tr>
	<tr>
		<td class="subtituloDireita">Saldo total para a Fase 2:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldodisp); ?></td>
	</tr>	
	<tr>
		<td class="subtituloDireita">Recurso Gasto:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($recursoGasto); ?></td>
	</tr>	
	<!-- 
	<tr>
		<td class="subtituloDireita">Aditivo Fase 1:</td>
		<td class="subtituloCentro">R$ <?php echo formata_valor($saldoaditivo); ?></td>
	</tr>
	-->	
	<tr>
		<td class="subtituloDireita">Saldo:</td>
		<td id="saldo" class="subtituloCentro">R$ <?php echo formata_valor($saldocomp); ?></td>
	</tr>			
</table>

<br>

<form id="formulario" method="post" name="formulario" enctype="multipart/form-data">
	<input type="hidden" id="unicod" name="unicod" value="<?php echo $_SESSION['sisfor']['unicod'];?>"/>
	<input type="hidden" id="siftipoplanejamento" name="siftipoplanejamento" value="<?php echo FASE02; ?>"/>
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
			<td class="subtituloDireita" width="25%">Secretaria Interessada:</td>
			<td>
	        	<?php $sql = "SELECT coordid AS codigo, coordsigla || ' - ' || coorddesc AS descricao FROM catalogocurso2014.coordenacao WHERE (coordsigla ILIKE '%SEB%' OR coordsigla ILIKE '%SECADI%') ORDER BY coordsigla, coorddesc"; ?>
	        	<?php $db->monta_combo('coordid',$sql,$habilitado,'Selecione a Secretaria','','','','490','S','coordid',''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Atividade:</td>
			<td>
	        	<?php 	
	        		$arrayAtividade = array(
							array('codigo' => '1', 'descricao' => 'Seminário'),
							array('codigo' => '2', 'descricao' => 'Estudos e Pesquisas'),
							array('codigo' => '3', 'descricao' => 'Avaliações'),
							array('codigo' => '4', 'descricao' => 'Produção de materiais, meios e/ou conteúdos'),
							array('codigo' => '5', 'descricao' => 'Publicações')); ?>
		        	<?php $db->monta_combo('oatatividade',$arrayAtividade,$habilitado,'Selecione a Atividade','','','','490','S','oatatividade',''); ?>			
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita" width="25%">Área Temática:</td>
			<td>
	        	<?php $sql = "SELECT ateid AS codigo, atedesc AS descricao FROM sisfor.areatematica WHERE atestatus = 'A' ORDER BY atedesc"; ?>
	        	<?php $db->monta_combo('ateid',$sql,$habilitado,'Selecione a Área Temática','','','','490','S','ateid',''); ?>
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Etapa:</td>
			<td>
				<input id="oateducaoinfantil" type="checkbox" name="oateducaoinfantil" value="t" class="normal" style="margin-top: -1px;" <?php echo $oateducaoinfantil == 't' ? 'checked="checked"' : '' ?>/> Educação Infantil<br>
				<input id="oatfundamentalinicial" type="checkbox" name="oatfundamentalinicial" value="t" class="normal" style="margin-top: -1px;" <?php echo $oatfundamentalinicial == 't' ? 'checked="checked"' : '' ?>/> Ensino Fundamental – Anos Iniciais<br>
				<input id="oatfundamentalfinal" type="checkbox" name="oatfundamentalfinal" value="t" class="normal" style="margin-top: -1px;" <?php echo $oatfundamentalfinal == 't' ? 'checked="checked"' : '' ?>/> Ensino Fundamental – Anos Finais<br>
				<input id="oatensinomedio" type="checkbox" name="oatensinomedio" value="t" class="normal" style="margin-top: -1px;" <?php echo $oatensinomedio == 't' ? 'checked="checked"' : '' ?>/> Ensino Médio<br>
				<input id="oatnaoseaplica" type="checkbox" name="oatnaoseaplica" value="t" class="normal" style="margin-top: -1px;" <?php echo $oatnaoseaplica == 't' ? 'checked="checked"' : '' ?>/> Não se aplica (não permite outras escolhas)
			</td>
		</tr>		
		<tr>
			<td class="subtituloDireita" width="25%">Nome:</td>
			<td><?php echo campo_texto('oatnome', 'S', $habilitado, 'Nome', '63', '250', '', '', '', '', '','id="oatnome"'); ?></td>
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
				<?php echo campo_texto('sifvalorloa', 'N', 'S', 'Valor LOA', '25', '15', '[.###],##', '', '', '', '','id="sifvalorloa"','','','atualizarSaldo(this.value);'); ?>
				<?php if($saldo && $saldo <= 0){ ?>
				&nbsp;<b>Não há saldo suficiente de recursos LOA.</b>
				<?php } ?>			
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Valor Outras:</td>
			<td><?php echo campo_texto('sifvaloroutras', 'N', 'S', 'Valor Outras', '25', '15', '[.###],##', '', '', '', '','id="sifvaloroutras"'); ?></td>
		</tr>	
		<!-- 			
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
		 -->		
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

<?php if($sifid){ ?>		
	<div id="divListarCustoAtividade"><?php listarCustoAtividade($sifid,'N'); ?></div>
<?php } ?>