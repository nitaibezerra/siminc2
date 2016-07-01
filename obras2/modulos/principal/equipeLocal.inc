<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . 'par/classes/modelo/DadosUnidade.class.inc';
include_once APPRAIZ . 'par/classes/controle/EntidadeParControle.class.inc';

$sql = "select m.muncod, m.mundescricao, m.estuf, iu.inuid, iu.itrid, o.empid 
		from obras2.obras o
			inner join entidade.endereco e on e.endid = o.endid
		    inner join territorios.municipio m on m.muncod = e.muncod
		    inner join par.instrumentounidade iu on iu.muncod = m.muncod
		where o.obrid = {$_REQUEST['obrid']}";

$dadosObras = $db->pegaLinha($sql);

$dadosObras = (!empty($dadosObras)) ? $dadosObras : array();

$inuid       = $dadosObras['inuid'];
$entidadePar = $dadosObras['muncod'];
$itrid       = $dadosObras['itrid'];
$empid       = !empty($dadosObras['empid']) ? $dadosObras['empid'] : ( empty($_SESSION['obras2']['empid']) ? $_REQUEST['obrid'] : $_SESSION['obras2']['empid'] );

$esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = ".$empid);

$oDadosUnidade = new DadosUnidade();

$arDadosUnidade = $oDadosUnidade->recuperarDadosPorInuid($inuid, 3);
$arSegmentos 	= $oDadosUnidade->recuperarSegmentos();
$arSegmentos 	= ($arSegmentos) ? $arSegmentos : array();

print carregaAbaDadosUnidade("/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid={$_REQUEST['obrid']}", $_REQUEST['obrid'], $esfera);

monta_titulo( "Dados da unidade", "Equipe local" );

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<?=$dadosObras['estuf'].' - '.$dadosObras['mundescricao'] ?>
		</td>
	</tr>
</table>

<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $inuid; ?>"/>

	<table id="tabelaEquipeLocal" class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Integrantes</td>	
			</tr>
			<tr align="center">
				<td width="3%">&nbsp;</td>	
				<td width="12%">CPF</td>	
				<td width="15%">Nome</td>	
				<td width="15%">Função/Cargo</td>	
				<td width="30%">Segmento</td>	
				<td width="20%">E-mail</td>	
			</tr>	
		</thead>
		<tbody id="tbodyTabela">
		<?php $arDadosUnidade = $arDadosUnidade ? $arDadosUnidade : array(array(1)); ?>
		<?php $count = 1 ?>
		<?php foreach ($arDadosUnidade as $dados) : ?>
			<?php extract($dados);?>	
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					<img alt="Excluir" title="Excluir" src="/imagens/excluir_01.gif" style="margin-left:5px; cursor:pointer">					
					<input value="1" name="ardun[]" type="hidden" />
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<input onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo $duncpf; ?>" name="duncpf[]" id="duncpf[]" type="text" />
				</td>
				<td>
					<input value="<?php echo $dunnome; ?>" name="dunnome[]" id="dunnome[]" class="dunnome" type="hidden" /><label><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input maxlength="254" size="30" style="width: 30ex;" value="<?php echo $dunfuncao; ?>" name="dunfuncao[]" type="text" class="normal">
				</td>
				<td>
					<?php //echo $db->monta_combo( 'sgmid[]', $arSegmentos, 'S', 'Selecione...', 'alternarExibicaoSegmento', '', '', '', 'S','',false,$sgmid); ?>
					
					<select class="CampoEstilo exibicaoSegmento" name="sgmid[]" style="width: 56ex;">
						<option value="">Selecione</option>
						<?php 
							foreach ($arSegmentos as $segmento) {
								echo "<option value='" . $segmento['codigo'] . "' " . (($segmento['codigo'] == $sgmid) ? "selected=\"selected\"" : "")  . ">" . $segmento['descricao'] . "</option>";
							}
						?>
					</select>
					<input size="50" class="objetoOculto" style="width: 50ex; display: none;" value="<?php echo $dunsegmento; ?>" name="dunsegmento[]" type="text">
					<a class="exibicaoSegmento" style="color:blue; margin-left: 20px; width: 50ex; display: none; cursor: pointer;" >Exibir Opções</a>
				</td>
				<td>
					<input size="30" style="width: 33ex;" value="<?php echo $dunemail; ?>" name="dunemail[]" type="text" class="normal">
				</td>
			</tr>
			<?php $count++ ?>
		<?php endforeach; ?>
		</tbody>
	</table>
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">		
		<tr>
			<td style="">
					<span style="margin-left:5px; cursor:pointer">
						<img src="/imagens/gif_inclui_d.gif" align="top" style="border: none" />
						Inserir Integrante
					</span>
			</td>
		</tr>
		<tr bgcolor="#C0C0C0" align="center">
			<td>
				<?php if($itrid == 1): ?>
					<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=25&obrid=<?=$_REQUEST['obrid'] ?>'" >
				<?php else: ?>
					<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='/obras2/obras2.php?modulo=principal/popupContatosPar&acao=A&funid=15&obrid=<?=$_REQUEST['obrid'] ?>'" >
				<?php endif; ?>
					<input type="button" class="botao" name="proximo" value='Próximo' onclick="window.location='/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid=<?=$_REQUEST['obrid'] ?>'" >
			</td>
		</tr>
	</table>
</form>	

<script type="text/javascript">

jQuery.noConflict();

//jQuery('input').disable();

jQuery(document).ready(function(){

	
	// IE8 DOES NOT SUPPORT THE CHANGE EVENT WHILE USING LIVE
	//var evt = jQuery.browser.msie ? "click" : "change";
	
	var is_chrome = navigator.userAgent.toLowerCase().indexOf('chrome') > -1;
	
	if(jQuery.browser.msie){
		var evt = "click";
	}
	if(jQuery.browser.mozilla){
		var evt = "change";
	}
	if(is_chrome){
		var evt = "blur";
	}
		
	if(jQuery("select.[name=sgmid[]]").length > 0){
		jQuery("select.[name=sgmid[]] option:selected").each(function(){
			if(jQuery(this).val() == 10){
				jQuery('.exibicaoSegmento').change();
			}
		});
	}
	
	if(jQuery("input.[name=duncpf[]]").length > 0){
		jQuery("input.[name=duncpf[]]").each(function(){
			if(jQuery(this).val() != ''){
				jQuery(this).val(mascaraglobal('###.###.###-##',jQuery(this).val()));
			}
		});
	}
		
	jQuery('input, textarea, select').attr('disabled', true);
	jQuery('.botao').attr('disabled', false);

});
</script>