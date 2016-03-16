<?php

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array();

$sqlEscolasQuadra = $oPreObraControle->recuperarSqlEscolasQuadra($preid);
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array();

//if($preid){
//	$obSubacaoControle = new SubacaoControle();
//	$obPreObra = new PreObra();
//	$arDados = $obSubacaoControle->recuperarPreObra($preid);
//	$docid = prePegarDocid($preid);
//	$esdid = prePegarEstadoAtual($docid);
//}
?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript" src="../includes/entidadesn.js"></script>

<form name="formulario" id="formulario" method="post" action="" >
	<input type="hidden" name="sbaid" value="<?php echo $sbaid ?>" />
	<input type="hidden" name="ano" value="<?php echo $ano?>" />
	<input type="hidden" name="preid" value="<?php echo $preid?>" />
	<input type="hidden" name="requisicao" value="salvarParObras" />
	<input type="hidden" name="entid" id="entid" value="" />
	<input readonly="readonly" type="hidden" name="mundescricao" id="mundescricao1" value="<?php echo $municipio; ?>" />
	<input type="hidden" name="muncod1" id="muncod1" value="<?php echo !empty($arDados['muncod']) ? $arDados['muncod'] : $_SESSION['par']['muncod']; ?>" />
	
	<table class="tabela" style="margin-bottom:15px" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulodireita">Nome do terreno:</td>
			<td>
				<?php			 
				$predescricao = $arDados['predescricao'];
				echo campo_texto( "predescricao", 'S', $boAtivo, '', 40, '', '', '','','','','id="predescricao"','',$predescricao);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo da Obra:</td>
			<td>
				<?php
					$sql = "SELECT DISTINCT
								ptoid as codigo,
								ptodescricao as descricao
							FROM
								obras.pretipoobra 
							WHERE
								tooid = 6
								AND ptostatus = 'A'
							ORDER BY
								descricao";

					$ptoid = $arDados['ptoid'];
					$db->monta_combo( "ptoid", $sql, $boAtivo, 'Selecione...', 'exibeTipoFundacao', '', '', '', 'S', 'ptoid',false,$ptoid,'Tipo da Obra');
				?> 
				<input type="hidden" name="hdn_ptoid" id="hdn_ptoid" value="<?php echo $arDados['ptoid'] ?>" />
			</td>
		</tr>
		<tr id="td_tipo_fundacao" style="display:<?php echo ($ptoid == OBRA_TIPO_B || $ptoid == OBRA_TIPO_B_220v) ? "" : "none" ?>" >
			<td class="subtitulodireita">Tipo De Fundação:</td>
			<td>
				<input type="radio" name="pretipofundacao" <?php echo $arDados['pretipofundacao'] != "E" ? " checked='checked' onclick=\"alertasapata()\"" : ""  ?> value="E" /> Estaca
				<input type="radio" name="pretipofundacao" <?php echo $arDados['pretipofundacao'] != "S" ? " checked='checked' onclick=\"alertasapata()\"" : ""  ?> value="S" /> Sapata
			</td>
		</tr>
		<tr id="td_escolas" style="display:<?php echo in_array($ptoid, Array(OBRA_QUADRA_COBERTURA,9,4,8,10)) ? "" : "none" ?>" >
			<td class="subtitulodireita">Escolas:</td>
			<td>
				<?php
					if(trim($arDados['entcodent']) != ''){
						$sql = "SELECT 
									entcodent as codigo,
									'(' || entcodent || ') - ' || entnome as descricao 
								FROM 
									entidade.entidade 
								WHERE 
									entcodent = '".trim($arDados['entcodent'])."'";		
						$entcodent = $db->carregar($sql); 
					}
					combo_popup('entcodent_', $sqlEscolasQuadra, 'Selecione...', "400x400", 1, array(), "", (($entcodent)?"N":"S"), false, false, 1, 400,
							    null, null, '', '', $entcodent, true, false, '', '', Array('descricao'), Array('dsc') );
				?>
			    <input type="hidden" id="entcodent" name="entcodent" value="<?=$arDados['entcodent'] ?>"/>
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
			<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
				<label>CEP:</label>
			</td>
			<td>
				<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);"
				class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10"
				<?php echo $stAtivo ?> /> 
				<img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Logradouro:</label>
			</td>
			<td>
				<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1"
				value="<?php echo $arDados['prelogradouro']; ?>" size="48" <?php echo $stAtivo ?> />
				<img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Número:</label>
			</td>
			<td>
				<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1"
				value="<?php echo $arDados['prenumero']; ?>" size="6" maxlength="4" onkeypress="return somenteNumeros(event);" <?php echo $stAtivo ?> />
				<img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Complemento:</label>
			</td>
			<td>
				<input type="text" name="endcom" class="CampoEstilo" id="endcom1" value="<?php echo $arDados['precomplemento']; ?>" size="48" maxlength="100" <?php echo $stAtivo ?> />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
			<td>
				<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1" value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
				<img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr id="tr_estado">
			<td class="subtitulodireita">Estado:</td>
			<td>
				<?php
					$estuf = $arDados['estuf'] ? $arDados['estuf'] : $_SESSION['brasilpro']['estuf'];
			
					if($_SESSION['brasilpro']['estuf']){
						$where = " where e.estuf = '{$_SESSION['brasilpro']['estuf']}' ";
					}
			
					$sql = "SELECT
							 	e.estuf as codigo, e.estdescricao as descricao 
							FROM
							 	territorios.estado e
							$where
							ORDER BY
							 	e.estdescricao ASC";
					$db->monta_combo( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
				?>
			</td>
		</tr>
		<tr id="tr_municipio">
			<td class="subtitulodireita">Município: <br />
			</td>
			<td id="municipio">
				<?
					if($estuf){
						$sql = "SELECT
									muncod as codigo, 
								 	mundescricao as descricao 
								FROM
								 	territorios.municipio
								WHERE
								 	estuf = '".$estuf."' 
								ORDER BY
								 	mundescricao asc";
						$muncod_ = $arDados['muncod'];
						$db->monta_combo( "muncod_", $sql, $boAtivo, 'Selecione...', '', '', '','','S', 'muncod1',false,$muncod_,'Município');
					} else {
						$db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod1',false,null,'Município');
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
			<td class="SubTituloDireita">Latitude :</td>
			<td>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden"> 
				<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span>
				º <input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden"> 
				<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>
				' <input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden"> 
				<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span>
				" <input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden"> 
				<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td>
			<td>
				<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden"> 
				<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>
				º <input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden"> 
				<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>
				' <input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden"> 
				<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span>
				" <input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden"> 
				<span id="_pololongitude1"><?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?></span>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<a href="#" onclick="abreMapaEntidade('1');">Visualizar / Buscar No Mapa</a> 
				<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"></td>
			<td class="SubTituloEsquerda" >
				<input type="button" value="Salvar" name="btn_salvar" onclick="salvarDadosObra()" <?php echo $stAtivo ?>/>
				<input type="button" value="Fechar" name="btn_fechar" onclick="window.close()" />
			</td>
		</tr>
	</table>
</form>
<script>

jQuery(document).ready(function(){
		
	var ptoid = $('ptoid').value;
	
	var muncod_usuario = '<?php echo $_SESSION['brasilpro']['muncod']; ?>';
	
	jQuery("#predescricao").addClass("obrigatorio");
	jQuery("#ptoid").addClass("obrigatorio");
	jQuery("#preobservacao").addClass("obrigatorio");
	jQuery("#endcep1").addClass("obrigatorio");
	jQuery("#endlog1").addClass("obrigatorio");
	jQuery("#endnum1").addClass("obrigatorio");
	jQuery("#endbai1").addClass("obrigatorio");
	jQuery("#muncod_").addClass("obrigatorio");
	jQuery("#predescricao").addClass("obrigatorio");
	
	jQuery.ajax({
			   type		: "POST",
			   url		: window.location,
			   data		: "requisicaoAjax=listaObras",
			   async    : false,
			   success	: function(res){
					jQuery('#obras_<?php echo $_GET['ano'] ?>',window.opener.document).html(res);
				}
			 });
	
	jQuery('#endcep1').blur(function(){

		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaCepMunicipio'}, 
				  {name : 'ajaxObras'	  , value : '1'},
				  {name : 'db'		  , value : true},
				  {name : 'cep'		  , value : jQuery(this).val()}
				 );

		var boBuscaEndCep = true;
		jQuery.ajax({
			   type		: "POST",
			   url		: 'ajax.php',
			   data		: data,
			   async    : false,
			   success	: function(res){
						var muncod = res;
						if(muncod != ''&&muncod_usuario != ''){
							if(muncod != muncod_usuario){
								alert('Favor informar um cep do município cadastrado.');
								jQuery('#endlog1').val('');
								jQuery('#endbai1').val('');
								jQuery('#mundescricao1').val('');
								jQuery('#estuf1').val('');
								jQuery('#muncod1').val('');
								boBuscaEndCep = false;
								return false;
							}
						}
					}
			 });
		if(boBuscaEndCep){
			getEnderecoPeloCEP(jQuery(this).val(),'1');
		}
//		filtraTipo(jQuery('#estuf1').val());
    })
	
});

function filtraTipo(estuf) {
	select = document.getElementsByName('muncod_')[0];
	
	if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	
	
	var data = new Array();
		data.push({name : 'ajaxObras', value : 'montaComboMunicipioPorUf'}, 
				  {name : 'db', value : true},
				  {name : 'estuf', value : estuf}
				 );
	 
	jQuery.ajax({
		   type		: "POST",
		   url		: window.location,
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
	var muncod = jQuery('#muncod1').val()
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

if(jQuery('#muncod1').val()){
	alteraComboMuncod();
}

function alertasapata(){
	var preid = jQuery('#preid').val();
	if( preid ){
		alert('Ao alterar o tipo de Obra Fundação, os dados referentes ao tipo antecessor serão perdidos!')
	}
}


function exibeTipoFundacao(value){

	jQuery('.enviar').attr('disabled', false);
	/*
	 * Somente deixar selecionar tipo de obra = Tipo A, se o município da obra estiver estive no grupo 1 do PAC
	 */
	if(value == "<? echo OBRA_TIPO_A?>"){
		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaGrupoMunicipioTipoObra_A'}, 
				  {name : 'db', value : true}
				 );

		var validacao_obrtipoA = true;
		
		jQuery.ajax({
			   type		: "POST",
			   url		: "ajax.php",
			   data		: data,
			   async    : false,
			   success	: function(res){			   				
			   				if(res=="false") {
			   					alert("O município selecionado não pode conter obras do tipo A");
			   					validacao_obrtipoA = false;
			 					jQuery('#ptoid').val('');
			   				}
						  }
			 });
		 
		if(!validacao_obrtipoA) return false;

		
	}
	
	if(value == "<? echo OBRA_TIPO_B?>" || value == "<?php echo OBRA_TIPO_B_220v ?>"){
		jQuery('#td_tipo_fundacao').show();
	}else{
		jQuery('#td_tipo_fundacao').hide();
	}

	var data = new Array();
	data.push({name : 'requisicao', value : 'verificaTipoEscola'}, 
			  {name : 'db', value : true},
			  {name : 'ptoid', value : value}
			 );

	var validacao_obrtipoA = true;

	if(value!=''){
		jQuery.ajax({
			   type		: "POST",
			   url		: "ajax.php",
			   data		: data,
			   async    : false,
			   success	: function(res){
							if(res){
								jQuery('#td_escolas').show();
								<?php if(!count($arEscolasQuadra)): ?>
									jQuery('.enviar').attr('disabled', true);
								<?php endif; ?>
							}else{
								jQuery('#td_escolas').hide();
							}
						  }
			 });
	}
	
	if(jQuery('#hdn_ptoid').val()){
		var preid = jQuery('#preid').val();
		if( preid != '' ){
			alert('Ao alterar o tipo de Obra, os dados referentes ao tipo antecessor serão perdidos!');
		}
	}
}

function salvarDadosObra()
{
	var erro = 0;
	var msg = '';
	
	var arrCamposNaoObrigatorios = new Array();
	arrCamposNaoObrigatorios[0] = "entcodent_disable";
	
	jQuery("[class~=obrigatorio]").each(function() { 
		if( (!this.value || this.value == "Selecione..." || this.value == "") && !inArray(this.name,arrCamposNaoObrigatorios) ){			
			erro = 1;
			alert('Favor preencher todos os campos obrigatórios!');
			this.focus();
			return false;
		}
	});

	var boPodeGravarLatitude =  true;
	var boPodeGravarLongitude =  true;
	jQuery('input[name=latitude[]]').each(function(i){
		if(jQuery(this).val() == ""){
			boPodeGravarLatitude = false;
		}
	});
	
	jQuery('input[name=longitude[]]').each(function(i){
		if(jQuery(this).val() == ""){
			boPodeGravarLongitude = false;
		}
	});

	if(!boPodeGravarLatitude && !boPodeGravarLongitude){
		msg = 'É necessário informar a Latitude e a Longitude.';
	} else if(!boPodeGravarLatitude && boPodeGravarLongitude){
		msg = 'É necessário informar a Latitude.';
	} else if(boPodeGravarLatitude && !boPodeGravarLongitude){
		msg = 'É necessário informar a Longitude.';
	}

	if(msg!=''){
		alert(msg);
		return false;
	}
	
	if(erro == 0){
		jQuery("#formulario").submit();
	}
}

function inArray(needle, haystack) {
    var length = haystack.length;
    for(var i = 0; i < length; i++) {
        if(haystack[i] == needle) return true;
    }
    return false;
}
</script>