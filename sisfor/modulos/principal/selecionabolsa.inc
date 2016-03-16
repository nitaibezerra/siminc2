<?php 
if($_REQUEST['requisicao']=='salvarBolsa'){
	salvarBolsa($_REQUEST);
	exit();
}

if($_REQUEST['iusd']){ 
	$perfilbolsa = verificarPerfilBolsa($_REQUEST['iusd']);
} 

$bolsaies = verificarBolsa($_REQUEST['iusd'],PFL_COORDENADOR_INST);
$bolsacurso = verificarBolsa($_REQUEST['iusd'],PFL_COORDENADOR_CURSO);
if($bolsaies){
	extract($bolsaies);
}
if($bolsacurso){
	extract($bolsacurso);
}	
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript">
	function selecionarBolsaIES(tpebolsaies){
		if(tpebolsaies == 't'){
			jQuery('input:radio[name=tpebolsacurso]')[1].checked = true;
		} else {
			jQuery('input:radio[name=tpebolsacurso]')[0].checked = true;
		}
	}

	function selecionarBolsaCurso(tpebolsacurso){
		if(tpebolsacurso == 't'){
			jQuery('input:radio[name=tpebolsaies]')[1].checked = true;
		} else {
			jQuery('input:radio[name=tpebolsaies]')[0].checked = true;
		}
	}

	function salvarBolsa(){
		if(jQuery('[name=tpebolsaies]:checked').length == 0 || jQuery('[name=tpebolsacurso]:checked').length == 0){
			alert('Selecione a "Bolsa"!');
			jQuery('#tpebolsaies').focus();
			return false;
		}
		jQuery('#formulario').submit();
	}
</script>

<br>

<?php $usucpf = $db->pegaUM("SELECT iuscpf FROM sisfor.identificacaousuario WHERE iusd = {$_REQUEST['iusd']}"); ?>

<?php monta_titulo('Selecionar Bolsa',$cies); ?>

<form id="formulario" method="post" name="formulario" action="" enctype="multipart/form-data">
	<input type="hidden" id="requisicao" name="requisicao" value="salvarBolsa"/>
	<input type="hidden" id="iusd" name="iusd" value="<?php echo $_REQUEST['iusd']; ?>"/>
	<input type="hidden" id="usucpf" name="usucpf" value="<?php echo $usucpf; ?>"/>
	<input type="hidden" id="tipo" name="tipo" value="<?php echo $_REQUEST['tipo']; ?>"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita" width="25%">Orientações:</td>
			<td>
				<p align="justify" style="line-height: 20px;">
				<?php if(!empty($bolsaies) && !empty($bolsacurso)){ ?>
				Somente uma bolsa permitida.
				<?php } else { ?>
				Selecione a bolsa.
				<?php } ?>
				</p>			
			</td>
		</tr>	
		<tr>
			<td class="subtituloDireita" width="25%" rowspan="2">Funções:</td>
			<td>
				<?php if(in_array(PFL_COORDENADOR_INST,$perfilbolsa)){ ?>
				<b><font color="#FF0000" size="3">Atenção:</font> Coordenador Institucional</b> deseja receber bolsa para esta função? <input id="tpebolsaies" type="radio" name="tpebolsaies" value="t" class="normal" style="margin-top: -1px;" onclick="selecionarBolsaIES(this.value);" <?php echo ($tpebolsaies =='t') ? 'checked' : ''; ?>> Sim <input id="tpebolsaies" type="radio" name="tpebolsaies" value="f" class="normal" style="margin-top: -1px;" onclick="selecionarBolsaIES(this.value);" <?php echo ($tpebolsaies == 'f') ? 'checked' : ''; ?>>Não
				<?php } ?>
			</td>
		</tr>		
			<td>
				<?php if(in_array(PFL_COORDENADOR_CURSO,$perfilbolsa)){ ?>
				<b><font color="#FF0000" size="3">Atenção:</font> Coordenador Curso</b> deseja receber bolsa para esta função? <input id="tpebolsacurso" type="radio" name="tpebolsacurso" value="t" class="normal" style="margin-top: -1px;" onclick="selecionarBolsaCurso(this.value);" <?php echo ($tpebolsacurso =='t') ? 'checked' : ''; ?>> Sim <input id="tpebolsacurso" type="radio" name="tpebolsacurso" value="f" class="normal" style="margin-top: -1px;" onclick="selecionarBolsaCurso(this.value);" <?php echo ($tpebolsacurso == 'f') ? 'checked' : ''; ?>>Não
				<?php } ?>
			</td>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" value="Salvar" id="btnSalvar" onclick="salvarBolsa();" />
				<input type="button" value="Cancelar" id="btnCancelar" onclick="window.close();"/>
			</td>
		</tr>				
	</table>	
</form>

<br>
