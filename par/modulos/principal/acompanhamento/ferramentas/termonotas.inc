<script type="text/javascript">

$(document).ready(function(){
	$('#btnPesquisar').click(function(){	
		if( $('[name=convinculado]').attr("checked"))
		{
			check = true;
		} 
		else
		{
			check = false;
		}
		if($('[name=estuf]').val() == '' && $('[name=mundescricao]').val() == '' && $('[name=ntermo]').val() == '' && $('[name=empnumeroprocesso]').val() == '' && ( ! check)){
			alert('Preencha pelo menos um filtro!');
			return false;
		}
		$('#exportaExcel').val('');	
		$('#formulario').submit();
	});

	$('#btnExportarXLS').click(function(){	
		
		$('#exportaExcel').val('true');

		if( $('[name=convinculado]').attr("checked"))
		{
			$('#checksem').val('true');
		} 
		else
		{
			$('#checksem').val('');
		}
		 
		
		$('#formulario').submit();
	});
	
	$('#btnLimpar').click(function(){
		document.location.href = document.location.href;
	});
});

//@todo acao do mais
function carregasubacoescontratos(idImg, dopid){
	
	var img 	 = $( '#'+idImg );
	
	var tr_nome = 'listaDocumentos2_'+ dopid;
	var td_nome  = 'trI_'+ dopid;

	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "")
	{
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaSubacoes(dopid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != "")
	{
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else 
	{
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}
function carregarcontratositens(idImg, sbdid){
	
	var img 	 = $( '#'+idImg );
	var tr_nome = 'listaDocumentos3_'+ sbdid;
	var td_nome  = 'trII_'+ sbdid;
	
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "")
	{
		$('#'+td_nome).html('Carregando...');
		img.attr ('src','../imagens/menos.gif');
		carregaListaItens(sbdid, td_nome);
	}
	if($('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != "")
	{
		$('#'+tr_nome).css('display','');
		img.attr('src','../imagens/menos.gif');
	} else 
	{
		$('#'+tr_nome).css('display','none');
		img.attr('src','/imagens/mais.gif');
	}
}

function carregaListaSubacoes(dopid, td_nome ){

	$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaDadosSubacoes&dopid="+dopid+"&"+$('#formulario').serialize(),
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function carregaListaItens(sbdid, td_nome ){

	
	$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaDadosNotas&sbdid="+sbdid+"&"+$('#formulario').serialize(),
	   		async: false,
	   		success: function(msg){
	   			$('#'+td_nome).html(msg);
	   			divCarregado();
	   		}
		});
}

function listarSubacao(sbaid, ano, inuid){
	var local = "par.php?modulo=principal/subacao&acao=A&sbaid=" + sbaid + "&anoatual=" + ano + "&inuid=" + inuid;
	janela(local,800,600,"Subação");
}

</script>
<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
	<tr>
		<td width="100%" colspan="3" align="center"><label class="TituloTela" style="color:#000000;">Termos com notas</label></td>
	</tr>
	<tr>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b></b></td>
		<td bgcolor="#e9e9e9" style="width:12%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ><b></b></td>
		<td bgcolor="#e9e9e9" style="width:78%; FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" ></td>
		<td bgcolor="#e9e9e9" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" >
		</td>
	</tr>
</table>
<table style="background-color: #F5F5F5;" align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="2">
	<tr>
		<td>
			<form method="post" name="formulario" id="formulario" action="">
				<input type="hidden" name="anoatual" value="<?=$_SESSION['exercicio']; ?>">
				<table class="tabela" align="center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3>
					<tr>
						<td class="subtituloDireita">UF:</td>
						<td>
							<?php
							$estuf = $_POST['estuf'];
							$sql = "SELECT		
										estuf as codigo,
										estdescricao as descricao
									FROM
										territorios.estado
									ORDER BY 	
										estuf";
							$db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', '');
							?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Município:</td>
						<td>
							<?php
							$mundescricao = $_POST['mundescricao'];
							echo campo_texto('mundescricao', 'N', 'S', '', 70, '', '', '');
							?>
						</td>
					</tr>
					<tr>
						<td class="subtituloDireita">Nº Termo:</td>
						<td>
							<input type="text" style="text-align:left;" name="ntermo"  id="ntermo" size="13" maxlength="10" value="<?= $_POST['ntermo'] ?>" onKeyUp= "this.value=mascaraglobal('##########',this.value);" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" style="text-align : left; width:15ex;"  title='' class='obrigatorio normal' /> <img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
						</td>
					</tr>
				<tr>
					<td class="SubTituloDireita">Número de Processo:</td>
					<td>
						<?php
							$filtro = simec_htmlentities( $_POST['empnumeroprocesso'] );
							$empnumeroprocesso = $filtro;
							echo campo_texto( 'empnumeroprocesso', 'N', 'S', '', 20, 50, '#####.######/####-##', ''); 
						?>
					</td>
				
				</tr>
				<tr>
					<td class="SubTituloDireita">Sem nota vinculada:</td>
					<td>
						<input type="checkbox" value="check" <?php if($_POST['convinculado']){ echo 'checked="checked"'; }  ?> name="convinculado" >
					</td>
				</tr>
					<tr>
						<td class="subtituloDireita">&nbsp;</td>
						<td class="subtituloEsquerda">
							<input type="button" 	value="Pesquisar" 		id="btnPesquisar" />
						 	<input type="button"	value="Limpar" 			id="btnLimpar" />
						 	<input type="button"	value="Exportar Excel" 	id="btnExportarXLS" />
						 	<input type="hidden"	value="" 				id="exportaExcel" 	name="exportaExcel" />
						 	<input type="hidden"	value="" 				id="checksem" 		name="checksem" />
						</td>
					</tr>
				</table>
			</form> 
				<?php 
				if($_REQUEST['exportaExcel'])
				{
					carregaDadosTermos(true);
				}
				else
				{
					carregaDadosTermos();
				}
				?>
			<div class="demo"></div>
<!-- 			<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">
<!-- 				<tr> -->
<!-- 					<td width="100%" colspan="3" align="center"> 
						<label class="TituloTela" style="color:#000000;"><input type="button" onclick="anexarContrato()" value="Anexar Contrato"></label>
<!-- 					</td> -->
<!-- 				</tr> -->
<!-- 			</table> -->
		</td>
	</tr>
</table>