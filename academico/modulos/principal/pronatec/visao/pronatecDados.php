<?php
//$escrita = verificaPermissãoEscritaUsuarioPreObra($_SESSION['usucpf'], $_REQUEST['preid']);

$campo = $_SESSION['par']['muncod'] ? 'muncod' 					 : 'estuf';
$valor = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : $_SESSION['par']['estuf'];

$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$busca = Array( 'campo' => $campo , 'valor' => $valor );
$arEscolasQuadraSelecionadas = $oPreObraControle->verificaEscolasQuadraSelecionadas($busca);
$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array();

//$arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra($busca, $preid);
$arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra($preid);
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array();

?>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">

jQuery(document).ready(function(){

	var ptoid = $('ptoid').value;

	var muncod_usuario = '<?php echo $_SESSION['par']['muncod']; ?>';

	jQuery("#predescricao").addClass("required");
	jQuery("#ptoid").addClass("required");
	jQuery("#endcep1").addClass("required");
	jQuery("#estuf1").addClass("required");
	jQuery("#muncod_").addClass("required");

	jQuery("#formulario").validate();

	jQuery("#formulario").submit(function(){

		var ptoid = $('ptoid').value;

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

		var msg = "";
		var ptoid = jQuery('select[name=ptoid]').val();

		if(!boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude e a Longitude.';
		} else if(!boPodeGravarLatitude && boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude.';
		} else if(boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Longitude.';
		}

		jQuery("#formulario").validate();

		if( msg || !jQuery("#formulario").valid()){
			if(msg){
				alert(msg);
			}
			return false;
		}


	});

	jQuery('.enviar').click(function(){
		if(this.value == 'Salvar'){
			jQuery('#acao').val('proximo');
		}
	});

	jQuery('.navegar').click(function(){

		var preid = '<?php echo ($_REQUEST['preid']) ?  $_REQUEST['preid'] : 'nulo'?>';

		if(this.value == 'Anterior'){
			aba = 'TermoCompromisso';
		}

		if(this.value == 'Próximo'){
			aba = 'questionario';
		}

		if(preid != 'nulo'){
			preid = '&preid='+preid;
		}else{
			preid = '';
		}

		document.location.href = 'par.php?modulo=principal/programas/pronatec/popupPronatec&acao=A&tipoAba='+aba+preid;
	});

	jQuery('#endcep1').blur(function(){

		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaCepMunicipio'},
				  {name : 'db', value : true},
				  {name : 'cep', value : jQuery(this).val()}
				 );

		var boBuscaEndCep = true;
		jQuery.ajax({
			   type		: "POST",
			   url		: "ajax.php",
			   data		: data,
			   async    : false,
			   success	: function(res){
						var muncod = res;
						if(muncod != ''){
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
//		if(boBuscaEndCep){
//			getEnderecoPeloCEP(jQuery(this).val(),'1');
//		}
		filtraTipo($('estuf1').value);
    })

});

function filtraTipo(estuf) {
	/*if( !estuf ){
		return false;
	}*/
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

		$obSubacaoControle = new SubacaoControle();
		$obPreObra = new PreObra();

		if($preid){
			$arDados = $obSubacaoControle->recuperarPreObra($preid);
		}

		$arDados = ($arDados) ? $arDados : array();


		$boAtivo = 'N';
		$stAtivo = 'disabled="disabled"';
		if($esdid){
			// Regra passada pelo Daniel - 9/6/11
			if(possuiPerfil($arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL)) &&
			   $esdid == WF_PRONATEC_OBRA_APROVADA &&
			   $arDados['ptoid'] == OBRA_TIPO_A) {
				$boAtivo = 'S';
				$stAtivo = '';
			} else {
				$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_SUPER_USUARIO);
				if( ($esdid == WF_PRONATEC_EM_CADASTRAMENTO || $esdid == WF_PRONATEC_EM_DILIGENCIA)  && possuiPerfil($arrPerfil) ){
					if($esdid == WF_PRONATEC_EM_DILIGENCIA){
						$boAtivo = 'N';
						$stAtivo = 'disabled';
					}else{
						$boAtivo = 'S';
						$stAtivo = '';
					}
				}
			}
		}else{
			$boAtivo = 'S';
			$stAtivo = '';
		}

		if($_POST['predescricao'] && $boAtivo == 'S'){

			$oSubacaoControle->salvarDadosProInfancia($_POST);
		}

		$preidTx = $_SESSION['par']['preid'] ? '&preid='.$_SESSION['par']['preid'] : '';
		$lnkabas = "academico.php?modulo=principal/pronatec/popupPronatec&acao=A&tipoAba=Dados".$preidTx;

		echo carregaAbasPronatec($lnkabas);
		monta_titulo( 'Dados do Imóvel', ''  );

		if(count($arDados)){

			if($arDados['preid']){
				$_SESSION['par']['preid'] = $arDados['preid'];
			}

			if($arDados['muncod']){
				$municipio = $obSubacaoControle->recuperaDescricaoMunicipio($arDados['muncod']);
			}
		}

		?>
<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="preid" id="preid" value="<?php echo $preid; ?>" />
	<input type="hidden" name="muncod" id="muncod1" value="<?php echo !empty($arDados['muncod']) ? $arDados['muncod'] : $_SESSION['par']['muncod']; ?>" />
	<input type="hidden" name="entid" id="entid" value="" />
	<input type="hidden" name="acao" id="acao" value="" />
	<input type="hidden" name="preano" id="preano" value="<?php echo $_SESSION['exercicio'] ?>" />
	<input readonly="readonly" type="hidden" name="mundescricao" class="CampoEstilo" id="mundescricao1" value="<?php echo $municipio; ?>" />
	<input type="hidden" name="origem" value="<?=ORIGEM_OBRA_PRONATEC ?>" />
	<?php echo cabecalho();?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulodireita">Nome do imóvel:</td>
			<td>
				<?php
					$predescricao = $arDados['predescricao'];
					echo campo_texto( "predescricao", 'S', $boAtivo, '', 40, '', '', '','','','','id="predescricao"','',$predescricao);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo do imóvel:</td>
			<td>
				<?php
				$arTipoObra = $oSubacaoControle->recuperarTiposObraAtivas( ORIGEM_OBRA_PRONATEC );
				$arTipoObra = $arTipo ? $arTipo : $arTipoObra;
				$ptoid = $arDados['ptoid'];
				$db->monta_combo( "ptoid", $arTipoObra, $boAtivo, 'Selecione...', '', '', '', '', 'S', 'ptoid',false,$ptoid,'Tipo da Obra');
				?>
				<input type="hidden" name="hdn_ptoid" id="hdn_ptoid" value="<?php echo $arDados['ptoid'] ?>" />
			</td>
		</tr>
		<?php
		$endereco = new Endereco();
		$entidade->enderecos[0] = $endereco;
		?>
		<tr>
			<td align="left" colspan="2">
				<strong>Endereço do imóvel</strong>
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
				<label>CEP:</label>
			</td>
			<td>
				<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);"
					   class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10"
					   <?php echo $stAtivo ?> /> <img src="../imagens/obrig.gif" />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Logradouro:</label>
			</td>
			<td>
				<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1"
					   value="<?php echo $arDados['prelogradouro']; ?>" size="48" <?php echo $stAtivo ?> />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Número:</label>
			</td>
			<td>
				<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1" value="<?php echo $arDados['prenumero']; ?>"
					   size="6" maxlength="4" onkeypress="return somenteNumeros(event);" <?php echo $stAtivo ?> />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Complemento:</label>
			</td>
			<td>
				<input type="text" name="endcom" class="CampoEstilo" id="endcom1" value="<?php echo $arDados['precomplemento']; ?>"
					   size="48" maxlength="100" <?php echo $stAtivo ?> />
			</td>
		</tr>
		<tr>
			<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
				<label>Bairro:</label>
			</td>
			<td>
				<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1" value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
			</td>
		</tr>
		<tr id="tr_estado">
			<td class="subtitulodireita">Estado:</td>
			<td>
				<?php
				$estuf = $arDados['estuf'];

				if($_SESSION['par']['estuf']){
					$where = " WHERE e.estuf = '{$_SESSION['par']['estuf']}' ";
				}

				$sql = "SELECT
							 e.estuf as codigo, e.estdescricao as descricao
						FROM
							 territorios.estado e
							 $where
						ORDER BY
							 e.estdescricao asc";
				$db->monta_combo( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
				?>
			</td>
		</tr>
		<tr id="tr_municipio">
			<td class="subtitulodireita">
				Município: <br />
			</td>
			<td id="municipio">
				<?php
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
					$db->monta_combo( "muncod_", $sql, $boAtivo, 'Selecione...', '', '', '','','S', 'muncod_',false,$muncod_,'Município');
				} else {
					$db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município');
				}
				?>
			</td>
		</tr>
		<script>
			document.getElementById('endcep1').value = mascaraglobal('##.###-###', document.getElementById('endcep1').value);
		</script>
		<?php
		$latitude = explode('.',$arDados['prelatitude']);
		$longitude = explode('.',$arDados['prelongitude']);
		?>
		<tr>
			<td class="SubTituloDireita">Latitude :</td>
			<td>
				<input name="latitude[]" id="graulatitude1" maxlength="2" size="3" value="<? echo $latitude[0]; ?>" class="normal" type="hidden">
				<span id="_graulatitude1">
					<?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?>
				</span>
				º
				<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden">
				<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>
				' <input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden">
				<span id="_seglatitude1">
					<?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?>
				</span>
				" <input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
				<span id="_pololatitude1">
					<?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?>
				</span>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Longitude :</td>
			<td><input name="longitude[]" id="graulongitude1" maxlength="2"
				size="3" value="<? echo $longitude[0]; ?>" type="hidden">
				<span
				id="_graulongitude1">
				<?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?>
				</span>
				º <input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden">
				<span id="_minlongitude1">
					<?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?>
				</span>
				' <input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden">
				<span id="_seglongitude1">
					<?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?>
				</span>
				" <input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden">
				<span id="_pololongitude1">
					<?php echo ($longitude[3]) ? $longitude[3] : 'X'; ?>
				</span>
				<input type="hidden" name="endzoom" id="endzoom" value="<? echo $obCoendereCoentrega->endzoom; ?>" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<a href="#" onclick="abreMapaEntidade('1');"> Visualizar / Buscar No Mapa</a>
				<input style="display: none;" name="endereco[1][endzoom]" id="endzoom1" value="" type="text">
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr bgcolor="#dcdcdc">
			<td style="text-align: center">
				<table width="100%">
					<tr>
						<td align="left"><input class="navegar" type="button" value="Anterior" /></td>
						<td align="center">
						<?php if( $boAtivo == 'S' ){ ?>
							<input class="enviar" type="submit" value="Salvar" />
						<?php } ?>
						</td>
						<td align="right">
							<input class="navegar" type="button" value="Próximo" />
						</td>
					</tr>
				</table>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript">
if(jQuery('#muncod1').val()){
	alteraComboMuncod();
}
</script>
<div id="divDebug"></div>
