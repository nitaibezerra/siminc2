<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css"/>
<!-- script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script -->		
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>		
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">

jQuery.noConflict();

jQuery.metadata.setType("attr", "validate");

jQuery(document).ready(function(){
	jQuery("#formulario").validate({
		ignoreTitle: true,
		rules: {
			ptoid: "required",
			preobservacao: "required",
			endcep1: "required",
			estuf1: "required",
			muncod_: "required",
			endnum: "required"
		}				
											
	});
});

function filtraTipo(estuf) {
	if( !estuf ){
		return false;
	}
	select = document.getElementsByName('muncod_')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	var data = new Array();
		data.push({name : 'requisicao', value : 'montaComboMunicipioPorUf'}, 
				  {name : 'db', value : true},
				  {name : 'estuf', value : estuf}
				 );
	 
	jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(res){
						jQuery('#municipio').html(res);
						jQuery('#municipio').css('visibility','visible');
					  }
		 });
	  
	// Espera 100 milisegundos para dar tempo da função AJAX ser executada.
	window.setTimeout('alteraComboMuncod()', 1000);
	
}

function alteraComboMuncod(){
	var muncod = $('muncod1').value;
	if(muncod){
		var comboMunicipio = document.getElementById('muncod_');
		for (var i = 0; i < comboMunicipio.length; i++) {
			var indiceCombo = comboMunicipio.options[i].index;
			var textoCombo = comboMunicipio.options[i].text;
			var valorCombo = comboMunicipio.options[i].value;
			
			if(valorCombo == muncod){
				comboMunicipio.options[i].selected = true;
			}
		}
	}
}

</script>
<?php
$oSubacaoControle = new SubacaoControle();

if($_POST['icoid']){
	$oSubacaoControle->salvarDadosPreObra($_POST);
}
?>
<?php echo carregaAbasItensComposicao("par.php?modulo=principal/popupItensComposicao&acao=A&tipoAba=dados&icoid=".$_REQUEST['icoid'], $_REQUEST['icoid'], $descricaoItem ); ?>
<?php 
	$obSubacaoControle = new SubacaoControle();
	$arDados = $obSubacaoControle->recuperarPreObraPorIcoid($_REQUEST['icoid']);
	$arDados = ($arDados[0]) ? $arDados : array();
	$arDados = current($arDados);
	if($arDados['preid']){
		$_SESSION['par']['preid'] = $arDados['preid'];
	}
	if($arDados['muncod']){
		$municipio = $obSubacaoControle->recuperaDescricaoMunicipio($arDados['muncod']);
	}
?> 
<form action="" method="post" name="formulario" id="formulario">
<input type="hidden" name="icoid" id="icoid" value="<?php echo $_REQUEST['icoid']; ?>" />
<input type="hidden" name="muncod" id="muncod1" value="<?php echo $arDados['muncod']; ?>" />
<input type="hidden" name="entid" id="entid" value="" />
<input readonly="readonly" type="hidden" name="mundescricao" class="CampoEstilo" id="mundescricao1" value="<?php echo $municipio; ?>" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td width="200px" colspan="2" align="center" bgcolor="#dcdcdc"><strong><?php echo $descricaoItem;?></strong></td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo da Obra:</td>
		<td>
			<?php
			 $arTipoObra = $oSubacaoControle->recuperarTiposObraAtivas();
			 $ptoid = $arDados['ptoid'];			 
			 $db->monta_combo( "ptoid", $arTipoObra, 'S', 'Selecione...', '', '', '', '', 'S', 'ptoid',false,$ptoid,'Tipo da Obra');
			 ?>						
		</td>
	</tr>	
	<tr>
		<td class="subtitulodireita">Unidade de Medida:</td>
		<td>
			<?php
			 //$db->monta_combo( "unidadeMedida", $arUnidadeMedida, 'S', 'Selecione...', '', '', '', '', 'S', 'unidadeMedida',false,null,'Unidade de Medida');
			 echo "Unidade Escolar";
			 ?>						
		</td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" valign="top"><label for="observacao">Observação:</label></td>
		<td><?php 
				echo campo_textarea( 'preobservacao', 'S', 'S', 'Observação', 65 , 5, 1000, $funcao = '', $acao = 0, $txtdica = '', $tab = false, 'Observação', $arDados['preobservacao'] ); 
			?>
		</td>
	</tr>
	<?php
        $endereco = new Endereco();
        $entidade->enderecos[0] = $endereco;
        ?>
	<tr>
		<td align="left" colspan="2"><strong>Endereço</strong></td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap"><label>CEP:</label></td>
		<td>
			<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);" onblur="getEnderecoPeloCEP(this.value,'1'); filtraTipo($('estuf1').value); " class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10" /> <img src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Logradouro:</label></td>
		<td>
			<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1" value="<?php echo $arDados['prelogradouro']; ?>" size="48" />
		</td>
	</tr>
	
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
		<td>
			<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1" value="<?php echo $arDados['prenumero']; ?>" size="5" maxlength="8" onkeypress="return somenteNumeros(event);" />
		</td>
	</tr>
	
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
		<td>
			<input type="text" name="endcom" class="CampoEstilo" id="endcom1" value="<?php echo $arDados['precomplemento']; ?>" size="48" maxlength="100" />
		</td>
	</tr>
	
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
		<td>
			<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1" value="<?php echo $arDados['prebairro']; ?>" />
		</td>
	</tr>
	
	<tr id="tr_estado">
		<td class = "subtitulodireita"> 
			 Estado:
		</td>
		<td>
		<?
			 $estuf = $arDados['estuf'];
			 $sql = "select
					 e.estuf as codigo, e.estdescricao as descricao 
					from
					 territorios.estado e 
					order by
					 e.estdescricao asc";
			 $db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
			 ?>						
		</td>
	</tr>
	<tr id="tr_municipio" >
		<td class = "subtitulodireita">
			Município:
			<br/>
		</td>
		<td id="municipio">
			<?
			if ($arDados['estuf']) {
				$sql = "select
						 muncod as codigo, 
						 mundescricao as descricao 
						from
						 territorios.municipio
						where
						 estuf = '".$arDados['estuf']."' 
						order by
						 mundescricao asc";
				$muncod_ = $arDados['muncod'];
				$db->monta_combo( "muncod_", $sql, 'S', 'Selecione...', '', '', '','','S', 'muncod_',false,$muncod_,'Município');
			} else {
				$db->monta_combo( "muncod_", array(), 'S', 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município');				
			}
			?>
		</td>	
	</tr>
	<script> document.getElementById('endcep1').value = mascaraglobal('##.###-###', document.getElementById('endcep1').value);</script>
	<?php                
		$latitude = explode('.',$arDados['prelatitude']);
		$longitude = explode('.',$arDados['prelongitude']);
	?>
	<tr>
		<td class="SubTituloDireita">Latitude :</td><td>
			<input name="latitude[0]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> <span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º 
			<input name="latitude[1]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> <span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span> ' 
			<input name="latitude[2]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> <span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> " 
			<input name="latitude[3]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden"> <span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude :</td><td>
			<input name="longitude[0]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> <span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span> º 
			<input name="longitude[1]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>"  type="hidden"> <span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span> ' 
			<input name="longitude[2]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>"  type="hidden"> <span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> "
			<input name="longitude[3]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> <span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span> 
			<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
		</td>
	</tr>
	<tr><td class="SubTituloDireita">&nbsp;</td><td><a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a> <input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text"></td></tr><tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr bgcolor="#dcdcdc">
		<td style="text-align:center">
			<input type="submit" value="Salvar" />
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
if(jQuery('#muncod1').val()){
	alteraComboMuncod();
}
</script>