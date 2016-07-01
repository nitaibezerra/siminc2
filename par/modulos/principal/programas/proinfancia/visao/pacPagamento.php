<?php

require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "www/emenda/fndeWebservice.php";

$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);
$preid 	 = $_SESSION['par']['preid'] ? $_SESSION['par']['preid'] : $_REQUEST['preid'];

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$arObra = $oSubacaoControle->recuperarPreObra($preid);
//ver($arObra);
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	jQuery.noConflict()
	jQuery(document).ready(function(){

		jQuery('.addparcela').click(function(){
			var divCampos = jQuery('#camposParcela').html();
			var nrparcela = jQuery('#tbprincipal').find('tr').length-9;
			var campos = jQuery('#tbprincipal'),
					tr = campos.find('tr:last').clone();
					tr.find("td:first").html("Parcela "+nrparcela);
					tr.find("td:last").html(divCampos);
					
				campos.append(tr);
				
			return false;
		});

		//jQuery('.excluir').live('click',function(){
		jQuery('.excluir').live('click',function(){
//			alert(jQuery('#tbprincipal').find('Parcela').length);
			if(!confirm('Deseja remover esta parcela?')){
				return false;
			}
			jQuery(this).parent().parent().remove();
			return false;
		});
	});
</script>					
<?php echo carregaAbasProInfancia("par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=pagamento&preid=".$preid, $preid, $descricaoItem ); ?>
<table id="tbprincipal" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="subtitulocentro" colspan="2">DADOS DA OBRA</td>		
	</tr>
	<tr>
		<td width="200px;" class="subtitulodireita">UF:</td>
		<td><?php echo $arObra['estuf'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Município:</td>
		<td><?php echo $arObra['mundescricao'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo de Obra:</td>
		<td><?php echo $arObra['ptodescricao'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Endereço do Terreno:</td>
		<td><?php echo $arObra['prelogradouro']." - ".$arObra['prebairro']." - ".$arObra['precomplemento'] ?></td>
	</tr>
	<tr>
		<td class="subtitulodireita">CEP:</td>
		<td><?php echo $arObra['precep'] ?></td>
	</tr>
	<tr>
		<td class="subtitulocentro" colspan="2">INFORMAÇÕES DO EMPENHO</td>		
	</tr>
	<tr>
		<td class="subtitulodireita">Número de Solicitação de Empenho:</td>
		<td></td>
	</tr>
	<tr>
		<td class="subtitulocentro" colspan="2">INFORMAÇÕES PARA O PAGAMENTO</td>		
	</tr>
	<tr>
		<td class="subtitulodireita">Parcelas:</td>
		<td><a href="javascript:void(0)" class="addparcela">Inserir Parcela</a> <img align="absmiddle" src="../imagens/gif_inclui.gif" /></td>
	</tr>		
</table>
<?php 
$arDadosTemporarios = array();
array_push($arDadosTemporarios, array('parcela' => 1,'mes' => '11','ano' => '2010','data' => '2010-11-01 15:22:54','status' => 'Pagamento solicitado'));
array_push($arDadosTemporarios, array('parcela' => 2,'mes' => '12','ano' => '2010','data' => '2010-11-17 16:04:14','status' => 'Cancelado'));
array_push($arDadosTemporarios, array('parcela' => 3,'mes' => '01','ano' => '2010','data' => '2010-12-03 09:07:22','status' => 'Solicitar'));
?>
<br/>
<center>
<div style="width:95%;background:#f0f0f0;">
	<b>Lista de pagamentos</b>
</div>
</center>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
    <th>Parcela</th>
    <th>Mês</th>
    <th>Ano</th>
    <th>Data</th>
    <th>Status</th>
  </tr>
<?php $x=0; ?>
<?php foreach($arDadosTemporarios as $dados): ?>
  <?php $cor = ($x % 2) ? '#f0f0f0' : 'white'; ?>
  <tr style="background:<?php echo $cor; ?>">
    <td align="center"><?php echo $dados['parcela'] ?></td>
    <td align="center"><?php echo $dados['mes'] ?></td>
    <td align="center"><?php echo $dados['ano'] ?></td>
    <td align="center"><?php echo $dados['data'] ?></td>
    <td align="center">
    	<?php if($dados['status'] == 'Solicitar'): ?>
    		<input type="button" value="Solicitar Pagamento" class="solicitar">
    	<?php else: ?>
    		<?php echo $dados['status'] ?>
    	<?php endif; ?>
    		
    </td>
  </tr>
  <?php $x++; ?>	
<?php endforeach; ?>
  <tr>
  	<td colspan="2">Total de registros: <?php echo count($arDadosTemporarios) ?></td>
  </tr>
</table>
<div id="camposParcela" style="display:none;">
	Mês: <?php echo campo_texto('mes', 'S', 'S', $label, 8, 2, '##', $hid)?>
	Ano: <?php echo campo_texto('ano', 'S', 'S', $label, 8, 4, '####', $hid)?>
	Valor: <?php echo campo_texto('mes', 'S', 'S', $label, 12, $max, '###.###.###,##', $hid)?>
	<img src="../imagens/excluir.gif" align="absmiddle" class="excluir" style="cursor:pointer;" />
	<input type="hidden" name="nrparcela" value="">	
</div>