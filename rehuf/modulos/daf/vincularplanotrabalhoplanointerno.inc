<?php
if($_GET['arquivo'])
{
	include_once APPRAIZ . "includes/classes/file.class.inc";
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$file-> getDownloadArquivo($_GET['arquivo']);
	exit;
}

if($_POST['requisicao'])
{
	$arrDados = $_POST['requisicao']();
	if($arrDados){
		extract($arrDados);
	}
}
if($_POST['requisicaoAjax'])
{
	$_POST['requisicaoAjax']();
	exit;
}

include APPRAIZ ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba($abacod_tela,$url,'');
monta_titulo( "DAF", "Vincular Portaria-Plano de Trabalho a Plano Interno");
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>
	jQuery(function() {
			<?php if($_SESSION['rehuf']['altert']): ?>
				alert('<?php echo $_SESSION['rehuf']['altert'] ?>');
				<?php unset($_SESSION['rehuf']['altert']) ?>
			<?php endif; ?>
			<?php if($plitid): ?>
				escondetr(<?php echo $plitid ?>);
			<?php endif; ?>
		});
		
	function escondetr(plitid){
	
		if(plitid == '1'){
			jQuery('.planotrabalho').hide();
			jQuery('.portaria').show();
		}else{
			jQuery('.portaria').hide();
			jQuery('.planotrabalho').show();
		}
	}
	
	function salvarPlanoInterno()
	{
		var erro = 0;
		if(jQuery("[name='plitid']").val() == 1){
			if(!trim(jQuery("[name='prtnumero']").val())){
				alert('Favor informar o Número da Portaria.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='prtdata']").val())){
				alert('Favor informar a Data da Portaria.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='prtnumerodou']").val())){
				alert('Favor informar o Número do DOU.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='prtdtpublicacao']").val())){
				alert('Favor informar a Data do DOU.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='plinumero']").val())){
				alert('Favor informar o Plano Interno.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='plidtinicio']").val())){
				alert('Favor informar a Data Início do Período para Responder.');
				erro = 1;
				return false;
			}
			
			if(trim(jQuery("[name='prtdata']").val()) && trim(jQuery("[name='prtdtpublicacao']").val())){
				var data_inicio   = jQuery("[name='prtdata']").val();
				var data_fim      = jQuery("[name='prtdtpublicacao']").val();
				
			var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
			var compara2 = parseInt(data_fim.split("/")[2].toString() + data_fim.split("/")[1].toString() + data_fim.split("/")[0].toString());

				if(compara2 < compara1){ 
					alert('A Data da Portaria deverá ser menor ou igual à do DOU.');
					erro = 1;
					return false;
				}
			}
			
			if(trim(jQuery("[name='plidtinicio']").val()) && trim(jQuery("[name='prtdtpublicacao']").val())){
				var data_inicio   = jQuery("[name='plidtinicio']").val();
				var data_fim      = jQuery("[name='prtdtpublicacao']").val();
				
			var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
			var compara2 = parseInt(data_fim.split("/")[2].toString() + data_fim.split("/")[1].toString() + data_fim.split("/")[0].toString());

				if(compara1 < compara2){ 
					alert('A data início do “Período para Responder” deverá ser igual ou maior que a data do DOU.');
					erro = 1;
					return false;
				}
			}
			
			if(!jQuery("[name='vpiid']").val()){
			
				if(trim(jQuery("[name='plidtinicio']").val())){
					var data_inicio   = jQuery("[name='plidtinicio']").val();
					var date		  =	new Date();
					var data_fim      = date.getFullYear() + '' + strpad00((date.getMonth() + 1)) + '' + strpad00(date.getDate());
					
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
	
					if(compara1 < data_fim){ 
						alert('No campo “Período para Responder” a data início deverá ser igual ou maior que a data atual.');
						erro = 1;
						return false;
					}
				}
			}
			
			if(trim(jQuery("[name='plidttermino']").val())){
					var data_inicio   = jQuery("[name='plidttermino']").val();
					var date		  =	new Date();
					var data_fim      = date.getFullYear() + '' + strpad00((date.getMonth() + 1)) + '' + strpad00(date.getDate());
					
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
	
					if(compara1 < data_fim){ 
						alert('No campo “Período para Responder” a data fim deverá ser igual ou maior que a data atual.');
						erro = 1;
						return false;
					}
				}
			
			
			if(trim(jQuery("[name='plidtinicio']").val()) && trim(jQuery("[name='plidttermino']").val())){
				var data_inicio   = jQuery("[name='plidtinicio']").val();
				var data_fim      = jQuery("[name='plidttermino']").val();
	 
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
				var compara2 = parseInt(data_fim.split("/")[2].toString() + data_fim.split("/")[1].toString() + data_fim.split("/")[0].toString());
				
				
				if (compara1 > compara2){
					alert('No Campo “Período para Responder” a data fim deverá ser maior que a data início.');
					erro = 1;
					return false;
				}
			}
			
			if(!trim(jQuery("[name='plidttermino']").val())){
				alert('Favor informar a Data Fim do Período para Responder.');
				erro = 1;
				return false;
			}
			selectAllOptions( document.getElementById('queid') );
			
			if(jQuery("#queid option:selected").length == 0){
				alert('Favor informar o Questionário!');
				erro = 1;
				return false;
			}
			jQuery("#queid option:selected").each(function() {
				if( jQuery(this).val() == ""){
					alert('Favor informar o Questionário!');
					erro = 1;
					return false;
				}
			});

			
//			if(!jQuery("[name='queid']").val() ||  document.getElementById('queid').options.length <= 1){
//				alert('Favor informar o Questionário');
//				erro = 1;
//				return false;
//			}
		}else{
			if(!trim(jQuery("[name='plttitulo']").val())){
				alert('Favor informar o Plano de Trabalho.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='plinumerop']").val())){
				alert('Favor informar o Plano Interno.');
				erro = 1;
				return false;
			}
			if(!trim(jQuery("[name='plidtiniciop']").val())){
				alert('Favor informar a Data Início do Período para Responder.');
				erro = 1;
				return false;
			}
				
			if(trim(jQuery("[name='plidtiniciop']").val()) && trim(jQuery("[name='prtdtpublicacao']").val())){
				var data_inicio   = jQuery("[name='plidtiniciop']").val();
				var data_fim      = jQuery("[name='prtdtpublicacao']").val();
				
			var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
			var compara2 = parseInt(data_fim.split("/")[2].toString() + data_fim.split("/")[1].toString() + data_fim.split("/")[0].toString());

				if(compara1 < compara2){ 
					alert('A Data de início do périodo para responder não pode ser menor que a data do DOU.');
					erro = 1;
					return false;
				}
			}
			if(!jQuery("[name='vpiid']").val()){
				if(trim(jQuery("[name='plidtiniciop']").val())){
					var data_inicio   = jQuery("[name='plidtiniciop']").val();
					var date = new Date();
					var data_fim      = date.getFullYear() + '' + strpad00((date.getMonth() + 1)) + '' + date.getDate();
					
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
	
					if(compara1 < data_fim){ 
						alert('No campo “Período para Responder” a data início deverá ser igual ou maior que a data atual.');
						erro = 1;
						return false;
					}
				}
			}
			
			if(trim(jQuery("[name='plidtterminop']").val())){
					var data_inicio   = jQuery("[name='plidtterminop']").val();
					var date		  =	new Date();
					var data_fim      = date.getFullYear() + '' + strpad00((date.getMonth() + 1)) + '' + strpad00(date.getDate());
					
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
	
					if(compara1 < data_fim){ 
						alert('No campo “Período para Responder” a data fim deverá ser igual ou maior que a data atual.');
						erro = 1;
						return false;
					}
				}
			
			if(trim(jQuery("[name='plidtiniciop']").val()) && trim(jQuery("[name='plidtterminop']").val())){
				var data_inicio   = jQuery("[name='plidtiniciop']").val();
				var data_fim      = jQuery("[name='plidtterminop']").val();
	 
				var compara1 = parseInt(data_inicio.split("/")[2].toString() + data_inicio.split("/")[1].toString() + data_inicio.split("/")[0].toString());
				var compara2 = parseInt(data_fim.split("/")[2].toString() + data_fim.split("/")[1].toString() + data_fim.split("/")[0].toString());
				
				
				if (compara1 > compara2){
					alert('No Campo “Período para Responder” a data fim deverá ser maior que a data início.');
					erro = 1;
					return false;
				}
			}
			
			if(!trim(jQuery("[name='plidtterminop']").val())){
				alert('Favor informar a Data Fim do Período para Responder.');
				erro = 1;
				return false;
			}
			
			selectAllOptions( document.getElementById('queidp') );
			
			if(jQuery("#queidp option:selected").length == 0){
				alert('Favor informar o Questionário!');
				erro = 1;
				return false;
			}
			jQuery("#queidp option:selected").each(function() {
			if( jQuery(this).val() == ""){
				alert('Favor informar o Questionário!');
				erro = 1;
				return false;
			}
			});
		}
		if(!trim(jQuery("[name='arquivo']").val())){
				alert('Favor anexar o documento (Portaria ou Plano de Trabalho).');
				erro = 1;
				return false;
			}
	if(erro == 0){
			jQuery("#formulario").submit();
		}	
}

function strpad00(s)
{
    s = s + '';
    if (s.length === 1) s = '0'+s;
    return s;
}

function carregarPlanoInterno(vpiid){
	jQuery("[name='requisicao']").val("carregarPlanoInterno");
	jQuery("[name='vpiid']").val(vpiid);
	jQuery("#formulario").submit();
}

function excluirPlanoInterno(vpiid){
	jQuery("[name='requisicao']").val("excluirPlanoInterno");
	jQuery("[name='vpiid']").val(vpiid);
	var r=confirm("Tem certeza que deseja excluir esse vínculo? Ao excluir este vínculo o formulário correspondente também será excluído.");
	if(r==true){
		jQuery("#formulario").submit();
	}
}

function removerArquivo(vpiid,arqid){
	jQuery("[name='requisicao']").val("removerArquivo");
	jQuery("[name='vpiid']").val(vpiid);
	jQuery("[name='arqid']").val(arqid);
	var r=confirm("Tem certeza que deseja excluir esse arquivo?");
	if(r==true){
		jQuery("#formulario").submit();
	}
}

function removerArquivo(vpiid,arqid){
	var r=confirm("Tem certeza que deseja excluir esse arquivo?");
	if(r==true){
		jQuery.ajax({
			   type: "POST",
			   url: window.location,
			   data: "requisicaoAjax=removerArquivo&vpiid="+vpiid+"&arqid="+arqid,
			   success: function (result) {
						jQuery("#td_arquivo").html("<input type=\"file\" name=\"arquivo\" id=\"arquivo\"> <img border=\"0\" title=\"Indica campo obrigatório.\" src=\"../imagens/obrig.gif\">");
	                }
		});
	}
}

// Somente letras maiúsculas, minúsculas, numeros e espaço
jQuery(document).ready(function() {
	 jQuery("[name='plinumero']").keyup(function() {
        var valor = jQuery(this).val().replace(/[^a-z0-9]+/gi,'');
        jQuery(this).val(valor);
    });
	 jQuery("[name='plinumerop']").keyup(function() {
        var valor = jQuery(this).val().replace(/[^a-z0-9]+/gi,'');
        jQuery(this).val(valor);
    });
    
	jQuery("[name='plinumero']").addClass("maiusculo");
});

</script>
<style>
.link{cursor:pointer;}
</style>
<table class='tabela' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
	<tr>
		<td class="SubTituloCentro">Vincular Plano Interno</td>
	</tr>
</table>
<form method="POST" id="formulario" name="formulario" enctype="multipart/form-data" >
	<input type="hidden" name="requisicao" value="salvarPlanoInterno">
	<input type="hidden" name="vpiid" id="vpiid" value="<?=$vpiid?>">
	<input type="hidden" name="pliid" id="pliid" value="<?=$pliid?>">
	<input type="hidden" name="pltid" id="pltid" value="<?=$pltid?>">
	<input type="hidden" name="prtid" id="prtid" value="<?=$prtid?>">
	<input type="hidden" name="arqid" id="arqid" value="<?=$arqid?>">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td width="25%" class="SubTituloDireita">Tipo:</td>
			<td>
				<?php
					$sql = "SELECT
							 p.plitid AS codigo,
							 p.plitdsc AS descricao
							FROM
							 rehuf.planointernotipo p
							WHERE
							 p.plitstatus = 'A'
							ORDER BY
							 p.plitdsc desc";
					$db->monta_combo("plitid",$sql, 'S' ,'','escondetr','');
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<!-- Portaria Início -->
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">Portaria N°:</td>
			<td><?=campo_texto('prtnumero','S','S','',5,10,'#####','');?> de <?php echo campo_data2( 'prtdata', 'S', 'S', '', 'S', '', '' ); ?></td>
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">DOU N°:</td>
			<td><?=campo_texto('prtnumerodou','S','S','',5,10,'#####','');?> de <?php echo campo_data2( 'prtdtpublicacao', 'S', 'S', '', 'S', '', '', '' ); ?></td>
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">Plano Interno:</td>
			<td id="pi"><?= campo_texto( 'plinumero', 'S', 'S', '', 10, 11, '', '', '','','','','' ); ?></td>
			
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">Período para Responder:</td>
			<td><?php echo campo_data2( 'plidtinicio', 'S', 'S', '', 'S', '', '', '' ); ?> a <?php echo campo_data2( 'plidttermino', 'S', 'S', '', 'S', '', '', '' ); ?></td>
		</tr>
		<tr class="portaria">
			<td width="25%" class="SubTituloDireita">Questionários:</td>
			<td>
				<?php
					$queid = recuperarQuestionarioPorVinculo($vpiid);
					$sql = "SELECT
							 queid AS codigo,
							 quetitulo AS descricao
							FROM
							 questionario.questionario gp
							WHERE 1=1
								--sisid = 27
							ORDER BY
							quetitulo";
					combo_popup( 'queid', $sql, 'Selecione o(s) Questionário(s)', '400x400', 0, array(), '', 'S', true, true );
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<!-- Fim da Portaria -->
		<!-- Plano de trabalho Início -->
		
		<tr class="planotrabalho" style="display:none">
			<td width="25%" class="SubTituloDireita">Plano Trabalho:</td>
			<td><?=campo_texto('plttitulo','S','S','',80,500,'','');?></td>
		</tr>
		<tr class="planotrabalho" style="display:none">
			<td width="25%" class="SubTituloDireita">Plano Interno:</td>
			<td id="pi">
				<?php $plinumerop = $plinumero ?>
				<?= campo_texto( 'plinumerop', 'S', 'S', '', 10, 11, '', '', '','','','','' ); ?>
			</td>
		</tr>
		<tr class="planotrabalho" style="display:none">
			<td width="25%" class="SubTituloDireita">Período para Responder:</td>
			<td>
				<?php $plidtiniciop = $plidtinicio ?>
				<?php $plidtterminop = $plidttermino ?>
				<?php echo campo_data2( 'plidtiniciop', 'S', 'S', '', 'S', '', '', '' ); ?> a <?php echo campo_data2( 'plidtterminop', 'S', 'S', '', 'S', '', '', '' ); ?></td>
		</tr>
		<tr class="planotrabalho" style="display:none">
			<td width="25%" class="SubTituloDireita">Questionários:</td>
			<td>
				<?php
					$queidp = recuperarQuestionarioPorVinculo($vpiid);
								
					$sql = "SELECT
							 queid AS codigo,
							 quetitulo AS descricao
							FROM
							 questionario.questionario gp
							WHERE 1=1
								--sisid = 27
							ORDER BY
							quetitulo";
					combo_popup( 'queidp', $sql, 'Selecione o(s) Questionário(s)', '400x400', 0, array(), '', 'S', true, true );
				?>
				<?= obrigatorio(); ?>
			</td>
		</tr>
		<!-- Fim trabalho Início -->
		<tr>
			<td class="SubTituloDireita" >Anexo:</td>
			<td id="td_arquivo">
			<?php if(!$arqid){ ?>
				<input type="file" name="arquivo" id="arquivo" size="40"><?= obrigatorio(); ?>
			<?php }else{?>
				<?=$arqnome;?> <img class="link" onclick="removerArquivo('<?=$vpiid; ?>','<?=$arqid; ?>');" border=0 src="../imagens/excluir.gif" title="Remover Arquivo" />
			<?php }?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td>
				<input type="button" name="btn_salvar" value="Salvar" onclick="salvarPlanoInterno()" />
				<?php if($_POST): ?>
					<input type="button" name="btn_cancelar" value="Cancelar" onclick="window.location=window.location" />
				<?php endif; ?>
			</td>
		</tr>
	</table>
</form>
<?php
$sql = "
		SELECT DISTINCT 
				'<center>'
					'<img class=\"link\" onclick=\"carregarPlanoInterno('|| vpi.vpiid ||');\" border=0 src=\"../imagens/alterar.gif\" title=\"Editar Vínculo\" /> 
					 <img class=\"link\" onclick=\"excluirPlanoInterno('|| vpi.vpiid ||');\" border=0 src=\"../imagens/excluir.gif\" title=\"Excluir Vínculo\" />
					 ' as acao,
					 prt.prtnumero,
					 prt.prtdata,
					 prt.prtnumerodou,
					 prt.prtdtpublicacao,
					 plt.plttitulo,
					 vpi.vpiid,
					 vpi.plitid,
					 vpi.arqid,
					 arq.arqnome,
					pli.plinumero as PI,
					'' as arquivo,
				'</center>'
		FROM
				rehuf.vinculoplanointerno vpi
		LEFT JOIN 
				rehuf.planointerno pli ON vpi.pliid=pli.pliid
		LEFT JOIN 
				rehuf.planotrabalho plt ON vpi.pltid=plt.pltid
		LEFT JOIN 
				rehuf.portaria prt ON vpi.prtid=prt.prtid
		LEFT JOIN
				public.arquivo arq ON arq.arqid = vpi.arqid AND arq.arqstatus = 'A'
		LEFT JOIN 
				rehuf.vinculointernoquestionario vpiq ON vpi.vpiid = vpiq.vpiid
		LEFT JOIN
				questionario.questionario que ON vpiq.queid=que.queid
		ORDER BY prt.prtdata";

$arrDados = $db->carregar($sql);
if($arrDados){
	$n = 0;
	foreach($arrDados as $dado)
	{
		$arrLista[$n]['acao'] = $dado['acao'];
		if($dado['plitid'] == 2){ //Plano de Trabalho
			$arrLista[$n]['descricao'] = $dado['plttitulo'];
		}else{ //Portaria
			$arrLista[$n]['descricao'] = "Portaria Nº {$dado['prtnumero']}, de ".exibeDataPorExtenso($dado['prtdata'])." - DOU {$dado['prtnumerodou']}, de ".formata_data($dado['prtdtpublicacao']).".";
		}
		$arrLista[$n]['pi'] = "<span></span>".mb_strtoupper($dado['pi']);
		if($dado['arqid']){
			$arrLista[$n]['arquivo'] = "<a href='rehuf.php?modulo=daf/vincularplanotrabalhoplanointerno&acao=A&arquivo=".$dado['arqid']."' ><img border=0 src=\"../imagens/anexo.gif\" title=\"{$dado['arqnome']}\" /> {$dado['arqnome']}</a> ";
		}else{
			$arrLista[$n]['arquivo'] = "N/A";
		}
		$n++;
	}
}

$arCabecalho = array("Ações","Portaria - Plano Trabalho","PI","Arquivo");
$alinhamento	= array( 'center', 'left' );
$tamanho		= array( '10%', '40%' );	
$db->monta_lista($arrLista, $arCabecalho, 25, 10, 'N', 'center','','',$tamanho,$alinhamento);

?>
