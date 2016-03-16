<?php
$_REQUEST['preid'] = simec_strip_tags($_REQUEST['preid']);

$oWSSigarp = new WSSigarp();

$campo = $_SESSION['par']['muncod'] ? 'muncod' 					 : 'estuf';
$valor = $_SESSION['par']['muncod'] ? $_SESSION['par']['muncod'] : $_SESSION['par']['estuf'];
$_SESSION['par']['tooid'] = 1;

$preid = $_REQUEST['preid'] ? $_REQUEST['preid'] : $_SESSION['par']['preid'];

$docid = prePegarDocid($preid);
$esdid = prePegarEstadoAtual($docid);

$oSubacaoControle = new SubacaoControle();
$oPreObraControle = new PreObraControle();

$busca = Array( 'campo' => $campo , 'valor' => $valor );
$arEscolasQuadraSelecionadas = $oPreObraControle->verificaEscolasQuadraSelecionadas($busca);
$arEscolasQuadraSelecionadas = $arEscolasQuadraSelecionadas ? $arEscolasQuadraSelecionadas : array();

$boTipo_A = $oPreObraControle->verificaGrupoMunicipioTipoObra_A($_SESSION['par']['muncod']);

$arEscolasQuadra = $oPreObraControle->recuperarEscolasQuadra($preid);
$arEscolasQuadra = $arEscolasQuadra ? $arEscolasQuadra : array();

if( $_SESSION['par']['esfera'] == 'M' ){
	$_SESSION['par']['inuid'] = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE muncod = '".$_SESSION['par']['muncod']."'" );
	$_SESSION['par']['itrid'] = 2;
} elseif( $_SESSION['par']['esfera'] == 'E' ){
	$_SESSION['par']['inuid'] = $db->pegaUm( "SELECT inuid FROM par.instrumentounidade WHERE estuf = '".$_SESSION['par']['estuf']."'" );
	$_SESSION['par']['itrid'] = 1;
}

$sqlEscolasQuadra = $oPreObraControle->recuperarSqlEscolasQuadra($preid);
$sqlEscolasQuadra = $sqlEscolasQuadra ? $sqlEscolasQuadra : "SELECT '' as codigo, '' as descricao";

?>

<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<!-- script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script -->
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<script type="text/javascript">

//jQuery.metadata.setType("attr", "validate");

var arrTiposFundacao = [<?=OBRA_TIPO_B ?>,
						<?=OBRA_TIPO_B_220v ?>];

jQuery(document).ready(function(){

	var ptoid = $('ptoid').value;
	var ptoclassificacaoobra = $('hdn_ptoclassificacaoobra').value;

	var muncod_usuario = '<?php echo $_SESSION['par']['muncod']; ?>';

	jQuery("#predescricao").addClass("required");
	jQuery("#ptoid").addClass("required");
	jQuery("#preobservacao").addClass("required");
	jQuery("#endcep1").addClass("required");
	jQuery("#endlog1").addClass("required");
	jQuery("#endreferencia1").addClass("required");
	jQuery("#endnum1").addClass("required");
	jQuery("#endbai1").addClass("required");
	jQuery("#estuf1").addClass("required");
	jQuery("#muncod_").addClass("required");

	jQuery("#predescricao").attr("required","required");
	jQuery("#ptoid").attr("required","required");
	jQuery("#preobservacao").attr("required","required");
	jQuery("#endcep1").attr("required","required");
	jQuery("#endlog1").attr("required","required");
	jQuery("#endreferencia1").attr("required","required");
	jQuery("#endnum1").attr("required","required");
	jQuery("#endbai1").attr("required","required");
	jQuery("#estuf1").attr("required","required");
	jQuery("#muncod_").attr("required","required");

	jQuery("#formulario").validate();

	jQuery("#formulario").submit(function(){

		var ptoid = jQuery('#ptoid').val();
		var msg = "";

		jQuery('#entcodent').val(jQuery.trim(jQuery('#entcodent_ option:first').val()));
//		alert(jQuery('#entcodent_ option:first').val());
//		return false;
		
		if( in_array(ptoid, arrTiposFundacao) ){
			jQuery('#pretipofundacao_s').addClass("required");
			jQuery('#pretipofundacao_e').addClass("required");
			jQuery('#pretipofundacao_s').attr("required","required");
			jQuery('#pretipofundacao_e').attr("required","required");
		}else{
			jQuery('#pretipofundacao_s').removeClass("required");
			jQuery('#pretipofundacao_e').removeClass("required");
			jQuery('#pretipofundacao_s').attr("required","");
			jQuery('#pretipofundacao_e').attr("required","");
		}

		var data = new Array();
		data.push({name : 'requisicao', value : 'verificaCepMunicipio'},
				  {name : 'db', 		value : true},
				  {name : 'cep', 		value : jQuery('#endcep1').val()}
				 );

		jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(res){
					var muncod = res;
					if(muncod == '' ){
						//msg = 'CEP Inválido.';
					}
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

		var ptoid = jQuery('select[name=ptoid]').val();
		var tdf = jQuery('input[name=pretipofundacao]').val();

		/* Comentado de acordo com decisão de Daniel e Victor dia 8/11/2013
		if(jQuery('#ptoid').val() == 2){
			if(!jQuery('input[type=radio][name=pretipofundacao]:checked').val()){
				alert('O campo Tipo de Fundação é obrigatório');
				return false;
			}
		}
		*/

		if(!boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude e a Longitude.';
		} else if(!boPodeGravarLatitude && boPodeGravarLongitude){
			msg = 'É necessário informar a Latitude.';
		} else if(boPodeGravarLatitude && !boPodeGravarLongitude){
			msg = 'É necessário informar a Longitude.';
		}

		//if( ptoid == 5 || ptoid == 9 || ptoid == 4 || ptoid == 8 || ptoid == 10 || ptoid == 21 || ptoid == 28 || ptoid == 23 ){
		if( ptoclassificacaoobra == 'Q' || ptoclassificacaoobra == 'C' ){
			jQuery("#entcodent").addClass("required");
			jQuery("#formulario").valid();
		}else{
			jQuery("#entcodent").removeClass("required");
			jQuery("#formulario").valid();
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

		var preid = <?php echo ($_REQUEST['preid']) ?  $_REQUEST['preid'] : 'nulo'?>;

		if(this.value == 'Próximo'){
			aba = 'questionario';
		}

		if(preid != 'nulo'){
			preid = '&preid='+preid;
		}else{
			preid = '';
		}

		document.location.href = 'par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba='+aba+preid;
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
						if(trim(muncod) != '' && trim(muncod_usuario) != ''){
							if(trim(muncod) != trim(muncod_usuario)){
								alert('Favor informar um cep do município cadastrado.');
								jQuery('#endlog1').val('');
								jQuery('#endreferencia1').val('');
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
		filtraTipo(jQuery('#estuf1').val());
    });

    jQuery('#salvar').click(function(){
		jQuery('#formulario').submit();
	});
});

function filtraTipo(estuf) {
	/*if( !estuf ){
		return false;
	}*/
	//select = document.getElementsByName('muncod_')[0];

	/*if (select){
		select.disabled = true;
		select.options[0].text = 'Aguarde...';
		select.options[0].selected = true;
	}	*/

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
						jQuery("#muncod_").addClass("required");
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
function alertasapata(){
	var preid = $('preid').value;
	if( preid != '' ){
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

	if( in_array(value, arrTiposFundacao) ){
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


	if(jQuery('#hdn_ptoid').val()){
		var preid = $('preid').value;
		if( preid != '' ){
			alert('Ao alterar o tipo de Obra, os dados referentes ao tipo antecessor serão perdidos!');
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

		$reformulaMI = verificaMi( $preid );

		// Regra passada pelo Daniel - 9/6/11
		$arrPerfil = array(PAR_PERFIL_COORDENADOR_GERAL, PAR_PERFIL_ENGENHEIRO_FNDE);
		$arrPtoTipoA = Array(OBRA_TIPO_A);
		if(possuiPerfil($arrPerfil) && $esdid == WF_TIPO_OBRA_APROVADA && in_array($arDados['ptoid'], $arrPtoTipoA) ) {
			$boAtivo = 'S';
			$stAtivo = '';
		} else {
			$arrPerfil = array(PAR_PERFIL_EQUIPE_MUNICIPAL, PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO,PAR_PERFIL_EQUIPE_ESTADUAL,PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO,PAR_PERFIL_PREFEITO,PAR_PERFIL_SUPER_USUARIO);
			$arrEsdid = Array(
							WF_TIPO_EM_CADASTRAMENTO,
							WF_TIPO_EM_CORRECAO,
							WF_TIPO_EM_REFORMULACAO,
							WF_TIPO_EM_REFORMULACAO_OBRAS_MI,
							WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL,
							WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
						);
			if( in_array($esdid, $arrEsdid)  && possuiPerfil($arrPerfil) ){
				$arrEsdid = Array(
								WF_TIPO_EM_CORRECAO,
								WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
							);
				if($esdid == WF_TIPO_EM_CORRECAO){
					if( verificaRespostasQuestPAC( $preid ) || verificaQuestionarioAnaliseEngenhariaTerreno( $preid ) ){
						$boAtivo = 'S';
						$stAtivo = '';
					} else {
						$boAtivo = 'N';
						$stAtivo = 'disabled';
					}
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

	// nova situação, se o preobra for uma reformulação ou se tiver mais de 0% de execução da obra... desabilitar
	if($arDados['preidpai'] || (float)$arDados['percexec'] > 0) {
		$boAtivo = 'N';
		$stAtivo = 'disabled';
	}

	// Testa se é projeto B ou C Novo. Se sim não possui escola.
	$arrTipoSemEscola = Array(OBRA_TIPO_B_NOVO,OBRA_TIPO_C_NOVO);
	$nEscola = false;
	if( in_array( $arDados['ptoid'], $arrTipoSemEscola ) ){
		$nEscola = true;
	}

	# Código refeito em 22/10/2012. Regra para liberação da tela para (cadastramento e ateração) dos perfil abaixos listados nas seguintes situações também listadas abaixo.
	# Foi inserido os perfis Estaduais e a situação em Diligência.
	$perfil = pegaArrayPerfil($_SESSION['usucpf']);
	$arrEsdid = Array(
					WF_TIPO_EM_CORRECAO,
					WF_TIPO_EM_CADASTRAMENTO,
					WF_TIPO_EM_CORRECAO,
					WF_TIPO_EM_REFORMULACAO,
					WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL,
					WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
				);
	if(	in_array($esdid, $arrEsdid) &&
		(
			in_array(PAR_PERFIL_SUPER_USUARIO, $perfil) ||
			in_array(PAR_PERFIL_ADMINISTRADOR, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_PREFEITO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO, $perfil) ||
			in_array(PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, $perfil) ||
			in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) ||
			in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil)
		)
	){
		$boAtivo = 'S';
		$stAtivo = '';
	}
	
	$arrReformulacao = Array(
							WF_TIPO_EM_REFORMULACAO_OBRAS_MI, 
							WF_TIPO_ANALISE_DE_REFORMULACAO_OBRAS_MI, 
							WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL,
							WF_TIPO_EM_DILIGENCIA_REFORMULACAO_MI_PARA_CONVENCIONAL
						);
	if( in_array($esdid, $arrReformulacao) || $reformulaMI ){
		if( in_array(PAR_PERFIL_COORDENADOR_GERAL, $perfil) || in_array(PAR_PERFIL_ENGENHEIRO_FNDE, $perfil) || in_array(PAR_PERFIL_SUPER_USUARIO, $perfil)){
			$boAtivo = 'S';
			$stAtivo = '';
		}
	}

	if($preid){
		if( $arDados['ptoid'] == 'Q' ){
			$travaTipo = 'S';
		} elseif( $arDados['ptoid'] == 'P' ){
			$travaTipo = 'N';
		}
		//$travaTipo = $db->pegaUm('SELECT \'N\' FROM obras.preobra WHERE premcmv IS TRUE AND preid = '.$preid);
		// Vamos voltar e obrigar a usar os tipos B e C
		//$travaTipo = 'N';
	}
	$travaTipo = $travaTipo ? $travaTipo : 'S';

	if($_POST['predescricao'] && $boAtivo == 'S'){

		$oSubacaoControle->salvarDadosProInfancia($_POST);
	}

	if($preid){
		$lnkabas = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&tipoAba=dados&preid=".$preid;
	}else{
		$lnkabas = "par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A";
	}

	echo carregaAbasProInfancia($lnkabas, $preid, $descricaoItem );

	monta_titulo( 'Dados do terreno', $obraDescricao  );

	if(count($arDados)){

		if($arDados['preid']){
			$_SESSION['par']['preid'] = $arDados['preid'];
		}

		if($arDados['muncod']){
			$municipio = $obSubacaoControle->recuperaDescricaoMunicipio($arDados['muncod']);
		}
	}

	$muncod = !empty($arDados['muncod']) ? $arDados['muncod'] : $_SESSION['par']['muncod'];
?>
<body>

<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="preid" id="preid" value="<?php echo $preid; ?>" />
	<input type="hidden" name="muncod" id="muncod1" value="<?php echo $muncod ?>" />
	<input type="hidden" name="entid" id="entid" value="" />
	<input type="hidden" name="acao" id="acao" value="" />
	<input type="hidden" name="preano" id="preano" value="<?php echo $preano ?>" />
	<input readonly="readonly" type="hidden" name="mundescricao" class="CampoEstilo" id="mundescricao1" value="<?php echo $municipio; ?>" />
	<input type="hidden" name="origem" value="<?=ORIGEM_OBRA_PAC2 ?>" />
	<?php if(empty($preid) && $_REQUEST['prog']){ ?>
	<input type="hidden" name="preanoselecao" id="preanoselecao" value="2014" />
	<?php } ?>

	<?php echo cabecalho(); ?>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">

	<?php if(!count($arEscolasQuadra) && !count($arEscolasQuadraSelecionadas) && (in_array($_SESSION['par']['tipo'],Array('Q','C')) ) ): ?>
		<tr>
			<td colspan="2" align="center">
				<span style="color: red;">
					<b>Este  município não possui escolas com 500 ou mais alunos sem quadras escolares declaradas no Educacenso.</b>
				</span>
			</td>
		</tr>
	<?php endif;
	 ?>
	<tr>
		<td class="subtitulodireita">Nome do terreno:</td>
		<td>
			<?php
				if($preid){
					$ativo = testaMCMV($preid);
				}
				$ativo = $ativo ? $ativo : $boAtivo;
				$ativo = $boAtivo == 'N' ? $boAtivo : $ativo;
				$predescricao = $arDados['predescricao'];
				echo campo_texto( "predescricao", 'S', $ativo, '', 40, '', '', '','','','','id="predescricao"','',$predescricao);
			?>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Tipo da Obra:</td>
		<td>
			<?php
				$arTipoObra = $oSubacaoControle->recuperarTiposObraAtivas( ORIGEM_OBRA_PAC2 );
				
				if($preid){
					$sql = "SELECT ptoid FROM obras.preobra WHERE preid = $preid AND tooid = 1";
					$ptoidObra = $db->pegaUm($sql);
					$ptoidObraProinfanciaFixas = Array(	OBRA_TIPO_A,
														OBRA_TIPO_B,
														OBRA_TIPO_C,
														OBRA_TIPO_C_220v,
														OBRA_TIPO_B_220v,
														OBRA_TIPO_B_NOVO,
														OBRA_TIPO_C_NOVO );
					if( in_array($ptoidObra, $ptoidObraProinfanciaFixas )){
						$travaTipo = 'N';
					}
				}

				$grupo = $preid ? verificaGrupoMunicipio( $preid ) : verificaGrupoMunicipioMUNCOD( $muncod );
				if($travaTipo == 'N'){
					if( $esdid == WF_TIPO_EM_CADASTRAMENTO || $esdid == WF_TIPO_EM_CORRECAO ){
						$sql = "SELECT ptodescricao FROM obras.pretipoobra WHERE ptoid = $ptoidObra";
						$ptodescricao = $db->pegaUm($sql);
						if( $grupo == TIPOMUN_GRUPO1_2011 ){
							$arTipoObra = Array(
								Array('codigo' => 1, 'descricao' => 'Escola Infantil - Tipo A' ),
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras')
							);
							if( !in_array($ptoidObra, Array(1, 43,42)) ){
								array_push($arTipoObra, Array('codigo' => $ptoidObra, 'descricao' => $ptodescricao ));	
							}
						} else {
							$arTipoObra = Array(
								Array('codigo' => 43, 'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
								Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras')
							);
							if( !in_array($ptoidObra, Array(43,42)) ){
							
								array_push($arTipoObra, Array('codigo' => $ptoidObra, 'descricao' => $ptodescricao ));	
							}
						}
					}
				}
				$naoReformulacao = ' AND pto.ptoid NOT IN (71, 72)';
				if( $esdid == WF_TIPO_EM_REFORMULACAO ){
					$naoReformulacao = '';
				}
				if( $esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI   ){ // || $reformulaMI
					$categoriaObra = $oWSSigarp->listarCategoria( $preid );
					if($categoriaObra){
						if( $grupo == TIPOMUN_GRUPO1_2011 ){
							$anosel = '';
							if( $_REQUEST['preid'] ){
								$sql = "SELECT preanoselecao FROM obras.preobra where preid = ".$_REQUEST['preid'];
								$anosel = $db->pegaUm($sql);
							}
//							if( $anosel == 2014 ){
								$arTipoObra = array_merge(array(array('codigo' => 1, 'descricao' => 'Escola Infantil - Tipo A')), $categoriaObra);
//							} else {
//								$arTipoObra = $categoriaObra;
//							}
						} else {
							$arTipoObra = $categoriaObra;
						}
					} else {
						$sql = "SELECT ptoid as codigo, ptodescricao as descricao FROM obras.pretipoobra pto WHERE ptocategoria IS NOT NULL AND tooid = 1 $naoReformulacao";
						$arTipoObra = $db->carregar($sql);
					}
				}
				
				$arrSugestaoPtoid = Array( WF_TIPO_EM_REFORMULACAO_OBRAS_MI, WF_TIPO_EM_REFORMULACAO );
				if( in_array( $esdid, $arrSugestaoPtoid ) ){
					$sql = "SELECT DISTINCT * FROM (
							SELECT
								pto.ptoid as codigo,
								pto.ptodescricao as descricao
							FROM
								obras.pretipoobra pto
							INNER JOIN obras.pretipoobradiligencia tod ON tod.ptoid = pto.ptoid
							WHERE
								preid = $preid
								AND todstatus = 'A'
								$naoReformulacao
							UNION ALL
							SELECT
								pto.ptoid as codigo,
								pto.ptodescricao as descricao
							FROM
								obras.pretipoobra pto
							INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
							WHERE
								preid = $preid ) as foo";
					
					$arrSugestao = $db->carregar( $sql );
					
					if( $arrSugestao[0]['codigo'] != '' ){
						$arTipo = $arrSugestao;
					}
				}
				// REGRA PARA 2 ABERTURA DO PAC2 A PARTIR DE AGORA APENAS O TIPO C É PERMITIDO.
				if( ($esdid == 0 || $esdid == WF_TIPO_EM_CADASTRAMENTO) && ( $arDados['ptoclassificacaoobra'] != 'Q' && $arDados['ptoclassificacaoobra'] != 'C' )   ){
					$arrTiposMunicipio = pegaTiposMunicipio( $muncod );
					if( in_array(1, $arrTiposMunicipio) ){
						$arTipoObra = Array(
							Array('codigo' => 1, 	'descricao' => 'Escola Infantil - Tipo A'),
							Array('codigo' => 43, 	'descricao' => 'Escola Proinfância B - Metodologias Inovadoras'),
							Array('codigo' => 42, 	'descricao' => 'Escola Proinfância C - Metodologias Inovadoras')
						);
					}else{
						$arTipoObra = Array(
							Array('codigo' => 42, 'descricao' => 'Escola Proinfância C - Metodologias Inovadoras')
						);
					}
				}
				
				if( $preid ){
					
					$arTipoObra = $arTipo ? $arTipo : $arTipoObra;
					if( in_array($esdid, Array(WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL, WF_TIPO_EM_ANALISE_REFORMULACAO_MI_PARA_CONVENCIONAL) ) ){
						$sql = "SELECT
									ptoid as codigo,
									ptodescricao as descricao
								FROM
									obras.pretipoobra 
								WHERE
									ptoid IN (73,74) AND
									ptostatus = 'A'";
						$arTipoObra = $db->carregar( $sql );
					}else{
						$sql = "SELECT
									pto.ptoid as codigo,
									pto.ptodescricao as descricao
								FROM
									obras.pretipoobra pto
								INNER JOIN obras.preobra pre ON pre.ptoid = pto.ptoid
								WHERE
									preid = $preid";
						$arTipoObra[] = $db->pegaLinha( $sql );
					}
					$ptoid = $arDados['ptoid'];
				}
				
				$sql = "SELECT count(sr.preid)
						FROM par.solicitacaoreformulacaoobras sr 
						    inner join obras.preobra pr on pr.preid = sr.preid and pr.prestatus = 'A'
						    inner join workflow.documento d on d.docid = pr.docid and d.esdid = ".WF_TIPO_EM_REFORMULACAO_MI_PARA_CONVENCIONAL."
						WHERE
							pr.preid = $preid
						    and sr.sfostatus = 'A'";
				$boReforMISoli = $db->pegaUm($sql);
				
				$db->monta_combo( "ptoid", $arTipoObra, ( ((int)$boReforMISoli > 0) ? 'N' : $boAtivo ), 'Selecione...', 'exibeTipoFundacao', '', '', '', 'S', 'ptoid',false,$ptoid,'Tipo da Obra');
				unset($_SESSION['par']['tooid']);
				
				if( ( ((int)$boReforMISoli > 0) ? 'N' : $boAtivo ) == 'N' ){
					?>
					<input type="hidden" name="ptoid" id="ptoid" value="<?php echo $arDados['ptoid'] ?>" />
					<?php 
				}
			?>
			<input type="hidden" name="hdn_ptoid" id="hdn_ptoid" value="<?php echo $arDados['ptoid'] ?>" />
			<input type="hidden" name="hdn_ptoclassificacaoobra" id="hdn_ptoclassificacaoobra" value="<?php echo $arDados['ptoclassificacaoobra'] ?>" />
		</td>
	</tr>
	<? /*
	    * Comentado a pedido do Daniel no dia 03/01/2014.
	    * if(!$reformulaMI){ */
	    /* Reativado e adicionado regra de funcionamento no dia 28/01/2014 
	     * Só mostra fundação para tipos de obra antigos
	     * A pedido do Thiago efeito por Eduardo
	     * */

	$arrPtosFundacao = Array(OBRA_TIPO_B,
							OBRA_TIPO_B_220v);
	
	$display = 'style="display:none"';
	if( in_array( $arDados['ptoid'], $arrPtosFundacao ) ){ 
		$display = '';
	} 
	?>
	<tr id="td_tipo_fundacao" <?=$display ?>>
		<td class="subtitulodireita">Tipo De Fundação:</td>
		<td>
			<input type="radio" 
			<?php echo $arDados['pretipofundacao'] == "E" ? "checked='checked'" : "" ?>
			name="pretipofundacao" id="pretipofundacao_e"
			<?php echo $arDados['pretipofundacao'] != "E" ? "onclick=\"alertasapata()\"" : ""  ?>
			value="E" <?php echo $stAtivo ?>/> Estaca

			<input type="radio" 
			<?php echo $arDados['pretipofundacao'] == "S" ? "checked='checked'" : "" ?>
			name="pretipofundacao" id="pretipofundacao_s"
			<?php echo $arDados['pretipofundacao'] != "S" ? "onclick=\"alertasapata()\"" : ""  ?>
			value="S" <?php echo $stAtivo ?> /> Sapata

			<label for="pretipofundacao" class="error">Este campo é requerido</label>
		</td>
	</tr>
	<?php if( !$nEscola ){?>
	<tr id="td_escolas" style="display:<?php echo $arDados['ptoexisteescola'] == 't' ? "" : "none" ?>" >
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
								entcodent = '".trim($arDados['entcodent'])."' AND
								(entcodent IN ( SELECT trim(eop.codigoescola::character(10)) FROM par.escolasadesaopac eop ) OR entcodent IN ( SELECT entcodent FROM obras.preescolasquadraesporte ))";
					$entcodent = $db->carregar($sql);
				}

				//Adiciona escola selecionada.
				$sqlEscolas =  "SELECT  entcodent as codigo, entcodent || ' - ' || entnome as descricao
								FROM 	entidade.entidade
								WHERE	entcodent = '".trim($arDados['entcodent'])."' AND (
										entcodent IN ( SELECT trim(eop.codigoescola::character(10)) FROM par.escolasadesaopac eop ) OR 
										entcodent IN ( SELECT entcodent FROM obras.preescolasquadraesporte ) OR
										entcodent IN ( SELECT entcodent FROM obras.preobra WHERE preidpai = ".$_REQUEST['preid']." )
									)
								UNION ALL " .$sqlEscolasQuadra;
				//$db->monta_combo( "entcodent", $arEscolasQuadra, $boAtivo, 'Selecione...', '', '', '', '', 'S', 'entcodent',false,$entcodent,'Escolas');
// 		ver($sqlEscolas);		
				combo_popup('entcodent_', $sqlEscolas, 'Selecione...', "400x400", 1, array(), "", $boAtivo, false, false, 1, 400, null, null, '', '', $entcodent, true, false, '', '', Array('descricao'), Array('dsc') );
			?>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
		    <input type="hidden" id="entcodent" name="entcodent" value="<?=$arDados['entcodent'] ?>"/>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td class="subtitulodireita">Unidade de Medida:</td>
		<td>Unidade Escolar</td>
	</tr>
	<?php
		$endereco = new Endereco();
		$entidade->enderecos[0] = $endereco;
	?>

	<tr>
		<td align="left" colspan="2"><strong>Endereço do terreno</strong></td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 25%; white-space: nowrap">
			<label>CEP:</label>
		</td>
		<td>
			<input type="text" name="endcep1" title="CEP" onkeyup="this.value=mascaraglobal('##.###-###', this.value);" class="CampoEstilo" id="endcep1" value="<?php echo $arDados['precep']; ?>" size="13" maxlength="10" <?php echo $stAtivo ?> />
			<!--<img src="../imagens/obrig.gif" />-->
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Logradouro:</label>
		</td>
		<td>
			<input type="text" title="Logradouro" name="endlog" class="CampoEstilo" id="endlog1" value="<?php echo $arDados['prelogradouro']; ?>" size="48" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Número:</label>
		</td>
		<td>
			<input type="text" name="endnum" title="Número" class="CampoEstilo" id="endnum1" value="<?php echo $arDados['prenumero']; ?>" size="6" maxlength="4" onkeypress="return somenteNumeros(event);" <?php echo $stAtivo ?> />
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
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Ponto de Referência:</label>
		</td>
		<td>
			<input type="text" name="endreferencia" class="CampoEstilo" id="endreferencia1" value="<?php echo $arDados['prereferencia']; ?>" size="48" maxlength="100" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>
	<tr>
		<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
			<label>Bairro:</label>
		</td>
		<td>
			<input type="text" title="Bairro" name="endbai" class="CampoEstilo" id="endbai1" value="<?php echo $arDados['prebairro']; ?>" <?php echo $stAtivo ?> />
			<img src="../imagens/obrig.gif" />
		</td>
	</tr>

	<tr id="tr_estado">
		<td class="subtitulodireita">Estado:</td>
		<td>
			<?php
				$estuf = $arDados['estuf'];

				if($_SESSION['par']['estuf']){
					$where = " where e.estuf = '{$_SESSION['par']['estuf']}' ";
				}

				$sql = "
					Select	e.estuf as codigo,
							e.estdescricao as descricao
					from territorios.estado e
					$where
					order by  e.estdescricao asc
				";
				$db->monta_combo( "estuf", $sql, $boAtivo, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
			?>
		</td>
	</tr>
	<tr id="tr_municipio">
		<td class="subtitulodireita">Município:<br/></td>
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
					$db->monta_combo( "muncod_", $sql, $boAtivo, 'Selecione o Estado', '', '', '','','S', 'muncod_',false,$muncod_,'Município');
				} else {
					$db->monta_combo( "muncod_", array(), $boAtivo, 'Selecione o Estado', '', '', '', '', 'S', 'muncod_',false,null,'Município');
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
				<span id="_graulatitude1"><?php echo ($latitude[0]) ? $latitude[0] : 'XX'; ?></span> º
			<input name="latitude[]" id="minlatitude1" size="3" maxlength="2" value="<? echo $latitude[1]; ?>" class="normal" type="hidden">
				<span id="_minlatitude1"><?php echo ($latitude[1]) ? $latitude[1] : 'XX'; ?></span>  '
			<input name="latitude[]" id="seglatitude1" size="3" maxlength="2" value="<? echo $latitude[2]; ?>" class="normal" type="hidden">
				<span id="_seglatitude1"><?php echo ($latitude[2]) ? $latitude[2] : 'XX'; ?></span> "
			<input name="latitude[]" id="pololatitude1" value="<? echo $latitude[3]; ?>" type="hidden">
				<span id="_pololatitude1"><?php echo ($latitude[3]) ? $latitude[3] : 'X'; ?></span>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude :</td>
		<td>
			<input name="longitude[]" id="graulongitude1" maxlength="2" size="3" value="<? echo $longitude[0]; ?>" type="hidden">
				<span id="_graulongitude1"><?php echo ($longitude[0]) ? $longitude[0] : 'XX'; ?></span>	º
			<input name="longitude[]" id="minlongitude1" size="3" maxlength="2" value="<? echo $longitude[1]; ?>" type="hidden">
				<span id="_minlongitude1"><?php echo ($longitude[1]) ? $longitude[1] : 'XX'; ?></span>  '
			<input name="longitude[]" id="seglongitude1" size="3" maxlength="2" value="<? echo $longitude[2]; ?>" type="hidden">
				<span id="_seglongitude1"><?php echo ($longitude[2]) ? $longitude[2] : 'XX'; ?></span> "
			<input name="longitude[]" id="pololongitude1" value="<? echo $longitude[3]; ?>" type="hidden">
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

</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"  align="center">
	<tr bgcolor="#dcdcdc">
		<td style="text-align: center">
			<table width="100%">
				<tr>
					<td align="left">
						<?php if($preid){ ?>
						<input class="navegar" type="button" value="Anterior" disabled />
						<?php } ?>
					</td>
					<td align="center">
					<?php
						if( $boAtivo == 'S' ){
					?>
						<input class="enviar" type="button" id="salvar" value="Salvar" />
					<?php
						}
					?>
						<input class="fechar" type="button" value="Fechar" onclick="atualizarObra();" />
					</td>
					<td align="right">
						<?php if($preid){ ?>
							<input class="navegar" type="button" value="Próximo" />
						<?php } ?>
					</td>
				</tr>
			</table>
		</td>
	</tr>
</table>
<?php if($esdid == WF_TIPO_EM_REFORMULACAO_OBRAS_MI ){ ?>
	<br>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td colspan="2">
				<span style="color: red; font-size: 13; text-align: justify;">
					<b>A presente ação teve aprovação “a priori” para os Projetos Padrão FNDE, seguindo os preceitos de construção de obras civis utilizando métodos
					tradicionais de execução. Após aprovação da reformulação requerida pelo município esta ação integra o grupo de ações do Programa Proinfância – Padrão
					FNDE que serão executadas utilizando Metodologias Inovadoras (MI).</b>
				</span>
			</td>
		</tr>
	</table>
<?php } ?>
</form>

<script type="text/javascript">
	if(jQuery('#muncod1').val()){
		alteraComboMuncod();
	}
</script>

<div id="divDebug"></div>
</body>
