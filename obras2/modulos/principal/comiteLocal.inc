<?php
require_once APPRAIZ . "includes/classes/entidades.class.inc";
include_once APPRAIZ . 'par/classes/modelo/DadosUnidade.class.inc';
include_once APPRAIZ . 'par/classes/controle/EntidadeParControle.class.inc';

$sql = "select m.muncod, m.mundescricao, m.estuf, iu.inuid, iu.itrid 
		from obras2.obras o
			inner join entidade.endereco e on e.endid = o.endid
		    inner join territorios.municipio m on m.muncod = e.muncod
		    inner join par.instrumentounidade iu on iu.muncod = m.muncod
		where o.obrid = {$_REQUEST['obrid']}";

$dadosObras = $db->pegaLinha($sql);

$inuid = $dadosObras['inuid'];
$entidadePar = $dadosObras['muncod'];
$itrid = $dadosObras['itrid'];

$esfera = $db->pegaUm("select empesfera from obras2.empreendimento where empid = ".$_SESSION['obras2']['empid']);

$oEntidade = new Entidades();
$oDadosUnidade = new DadosUnidade();

print carregaAbaDadosUnidade("/obras2/obras2.php?modulo=principal/comiteLocal&acao=A&obrid={$_REQUEST['obrid']}", $_REQUEST['obrid'], $esfera);

monta_titulo( "Dados da unidade", "Comitê local" );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready(function(){
	var anterior = "<input type=\"button\" name=\"anterior\" value=\"<<\" onclick=\"window.location='/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=<?=$_REQUEST['obrid']?>'\" >";
	var proximo = "<input type=\"button\" name=\"proximo\" value=\">>\" disabled=\"disabled\" >";
	var acoes = jQuery('#tr_acoes').find('td').html();
	jQuery('#tr_acoes').find('td').html(anterior+acoes+proximo);
	
	jQuery('input, textarea, select').attr('disabled', true);
	jQuery('[name="anterior"]').attr('disabled', false);

});

</script>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
	<tr>
		<td style="color: blue; font-size: 22px">
			<?=$dadosObras['estuf'].' - '.$dadosObras['mundescricao'] ?>
		</td>
	</tr>
</table>

<?php 
# Carrega Coordenador
$arDadosCoordenador = $oDadosUnidade->recuperarDadosPorInuid($inuid, 5);
$arDadosCoordenador = ($arDadosCoordenador) ? $arDadosCoordenador : array();

# Carrega Coordenador
$arDadosIntegrantes = $oDadosUnidade->recuperarDadosPorInuid($inuid, 4);
$arDadosIntegrantes = ($arDadosIntegrantes) ? $arDadosIntegrantes : array();

# Carrega Segmentos
$arSegmentos 		= $oDadosUnidade->recuperarSegmentos();
$arSegmentos 		= ($arSegmentos) ? $arSegmentos : array();
?>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="formulario" value="1"/>
	<input type="hidden" name="inuid" value="<?php echo $inuid; ?>"/>

	<table class="listagem" cellspacing="2" cellpadding="2" border="0" align="center" width="95%" >
		<thead>
			<tr align="center" style=" background: #ccc; color: #000;">
				<td colspan="6" style="font-weight: bold; font-size: 15px;">Coordenador</td>	
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
		<tbody>
			<?php 
				if($arDadosCoordenador[0]){
					extract($arDadosCoordenador[0]);				
				} 
			?>
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					<input name="ardun[]" type="hidden" />
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<input onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo formatar_cpf_cnpj($duncpf); ?>" name="duncpf[]" type="text" />
				</td>
				<td>
					<input value="<?php echo $dunnome; ?>" name="dunnome[]" class="dunnome" type="hidden" /><label><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input maxlength="254" size="30" style="width: 30ex;" value="<?php echo $dunfuncao; ?>" name="dunfuncao[]" type="text" class="normal">
				</td>
				<td>
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
		</tbody>
	</table>
	<br />
	
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
		<?php $arDadosIntegrantes = $arDadosIntegrantes ? $arDadosIntegrantes : array(array(1)); ?>
		<?php $count = 1 ?>
		<?php foreach ($arDadosIntegrantes as $dados) : ?>
			<?php extract($dados);?>	
			<tr style="background: none repeat scroll 0% 0% rgb(245, 245, 245);" id="linha_1" align="center">
				<td>
					<img alt="Excluir" title="Excluir" src="/imagens/excluir_01.gif">
					<input value="1" name="ardun[]" type="hidden" />
				</td>
				<td>
					<input value="<?php echo $dunid; ?>" name="dunid[]" type="hidden" />
					<input onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" class="normal classcpf" size="19" style="width: 21ex;" value="<?php echo formatar_cpf_cnpj($duncpf); ?>" name="duncpf[]" type="text" />
				</td>
				<td>
					<input value="<?php echo $dunnome; ?>" name="dunnome[]" class="dunnome" type="hidden" /><label><?php echo $dunnome; ?></label>
				</td>
				<td>
					<input size="30" style="width: 30ex;" value="<?php echo $dunfuncao; ?>" name="dunfuncao[]" type="text" class="normal">
				</td>
				<td>					
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
				<input type="button" class="botao" name="anterior" value='Anterior' onclick="window.location='/obras2/obras2.php?modulo=principal/equipeLocal&acao=A&obrid=<?=$_REQUEST['obrid']?>'" >
				<input type="button" class="botao" name="proximo" value='Próximo' disabled="disabled" >
			</td>
		</tr>
	</table>
</form>