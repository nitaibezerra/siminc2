<?php
$_SESSION['par']['arMuncod'] = pegaResponssabilidade( '2' );
if(!$_SESSION['par']['boPerfilSuperUsuario'] && !$_SESSION['par']['boPerfilConsulta']){
	if( (!isset($_SESSION['par']['arMuncod']) && !count($_SESSION['par']['arMuncod'])) && $_SESSION['par']['muncod'] ){
		header( "Location: par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore" );
		exit;
	}
}

include APPRAIZ."includes/cabecalho.inc";
include APPRAIZ . 'includes/Agrupador.php';
echo'<br>';

echo montarAbasArray( criaAbaPar(), "par.php?modulo=principal/listaMunicipios&acao=A" );

monta_titulo( $titulo_modulo, '' );


if( $_SESSION['usucpf'] == '' && $_REQUEST['testa'] == 1 ){
	$sql = "select * from (
			select sbaid, icoano, picid, count(icoid) as contador FROM par.subacaoitenscomposicao group by sbaid, icoano, picid
			) as foo
			where
				foo.contador > 1 order by sbaid";
	
	$dados = $db->carregar($sql);
	
	$sbaid = "";
	$picid = "";
	$ano = "";
	$sequencial = 0;

	foreach($dados as $dado){
		$sql = "SELECT * FROM par.subacaoitenscomposicao WHERE sbaid = ".$dado['sbaid']." AND icoano = ".$dado['icoano']." AND picid = ".$dado['picid'];
		$novoDado = $db->carregar($sql);
		
		foreach( $novoDado as $nd ){
			if( $nd['sbaid'] == $sbaid && $nd['picid'] == $picid && $nd['icoano'] == $ano ){
				$sequencial++;
				
				$sql = "UPDATE par.subacaoitenscomposicao SET icosequencia = ".$sequencial." WHERE icoid = ".$nd['icoid'];
				$db->executar($sql);
				$db->commit();
			} else {
				$sequencial = 1;
				
				$sbaid = $nd['sbaid'];
				$picid = $nd['picid'];
				$ano = $nd['icoano'];
			
				$sql = "UPDATE par.subacaoitenscomposicao SET icosequencia = ".$sequencial." WHERE icoid = ".$nd['icoid'];
				$db->executar($sql);
				$db->commit();
			}
		}
	}
}


if( $_SESSION['usucpf'] == '' && 1 == 2){

$sql = "SELECT DISTINCT esdid FROM obras2.sigarpsituacao";
			$arresdid = $db->carregarColuna($sql);
			
			$arresdid2 = array( 873, 690, 874, 691, 864, 872 );

	ver($arresdid, $arresdid2, 'teste', d);
}


# Municipio
$_SESSION['par']['itrid'] = 2;

if( is_array($_POST['ptoid']) ){
	$_SESSION['par']['ptoid'] = ($_SESSION['par']['ptoid'] == $_POST['ptoid']) ? $_SESSION['par']['ptoid'] : $_POST['ptoid'];
}

//$_SESSION['par']['muncod'];

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	$obEntidadeParControle = new EntidadeParControle();
	$sql = $obEntidadeParControle->listaMunicipio($_POST);
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$cabecalho = array("Código","Município", "UF", "Situação");
	$db->sql_to_excel($sql, 'relListaMunPAR', $cabecalho, $formato);
	unset($_SESSION['par']['sql']);
	exit;

}
$exporta = "false";
?>
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>-->
<!--<script src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>-->
<!--<script src="../includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.js"></script>-->
<!--<script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/funcoes.js"></script>-->

<!-- Add mousewheel plugin -->
<script type="text/javascript" src="/library/fancybox/helpers/jquery.mousewheel-3.0.6.pack.js"></script>

<!-- Add fancyBox main JS and CSS files -->
<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>
<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen" />

<!-- Add Button helper (this is optional) -->
<link rel="stylesheet" type="text/css" href="/library/fancybox/helpers/jquery.fancybox-buttons.css" />
<script type="text/javascript" src="/library/fancybox/helpers/jquery.fancybox-buttons.js"></script>

<script type="text/javascript" src="../par/js/par.js"></script>

<style type="text/css">
.fancybox img{
	box-shadow: 1px 1px 4px 1px #000;
	width: 10px !important;
	height: 10px !important;
	margin-right: 10px;
}

.fancybox img {
    box-shadow: 0px 0px 0px 0px ;
    height:17px !important;
    margin-right: 0;
    width: 17px !important;
}

.tr_detalhe{
	display:none;
}
</style>

<script type="text/javascript">

jQuery.noConflict();

jQuery(document).ready( function(){
	
	jQuery( "input[name='municipio']" ).focus();
	jQuery('.fancybox').fancybox();
	jQuery('#formulario').submit(function() {
		selectAllOptions( document.getElementById( 'grupo' ) );
        selectAllOptions( document.getElementById( 'ideb' ) );
    });

	jQuery('a').mouseover(function() {
		selectAllOptions( document.getElementById( 'grupo' ) );
    	selectAllOptions( document.getElementById( 'ideb' ) );
    } );

	jQuery('.busca_avancada').click(function(){
		if( jQuery('.tr_detalhe').css('display') == 'none' ){
			jQuery('.tr_detalhe').show();
		}else{
			jQuery('.tr_detalhe').hide();
		}
	});
	
});

function exportarExcel(){
	document.getElementById('exporta').value = "true";
	selectAllOptions( document.getElementById( 'grupo' ) );
	selectAllOptions( document.getElementById( 'ideb' ) );
	document.getElementById('formulario').submit();
}

function submitFormulario(){
	document.getElementById('exporta').value = "false";
	selectAllOptions( document.getElementById( 'grupo' ) );
	selectAllOptions( document.getElementById( 'ideb' ) );
	document.getElementById('formulario').submit();
}

function abrePlanoTrabalhoM(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'},
				  {name : 'entidadePar', value : entidadePar},
				  {name : 'estuf', value : estuf},
				  {name : 'muncod', value : muncod},
				  {name : 'inuid', value : inuid}
				 );
		jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
									var inuid = msg;
									if(inuid > 0){
										//return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
										return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa');
									}
					  }
		 });

}

function abreRestricao(entidadePar, estuf, muncod, inuid){
    var data = new Array();
    data.push(
        {name : 'requisicao', value : 'verificaInstrumentoUnidade'},
        {name : 'entidadePar', value : entidadePar},
        {name : 'estuf', value : estuf},
        {name : 'muncod', value : muncod},
        {name : 'inuid', value : inuid}
    );
    jQuery.ajax({
        type		: "POST",
        url		: "ajax.php",
        data		: data,
        async    : false,
        success	: function(msg){
            var inuid = msg;
            if(inuid > 0){
                return jQuery(location).attr('href', 'par.php?modulo=sistema/tabelasapoio/restricoes_fnde/acompanhamentoRestricao&acao=A&inuid='+inuid);
            }
        }
    });

}

function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

</script>
<form method="post" name="formulario" id="formulario">
	<input type="hidden" name="pesquisa" value="1>"/>
	<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tbody>
			<tr>
				<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
					<div style="float: left;width: 100%">
						<table border="0" cellpadding="3" cellspacing="0" width="100%" >
							<tr>
								<td valign="bottom" width="50%" >
									Município
									<br/>
									<?php 
										$filtro = simec_htmlentities( $_POST['municipio'] );
										$municipio = $filtro;
										echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
									?>
								</td>
								<td valign="bottom">
									Estado
									<br/>
									<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
									?>
								</td>
							</tr>
							<tr>
								<td valign="bottom" style="padding: 10px;" >
									<a class="busca_avancada" >[Busca avançada]</a>
								</td>
							</tr>
							<tr class="tr_detalhe">
								<td valign="bottom"><br/>Grupos de Municípios
									<br/>
								<?php
								// Filtros do relatório
								$stSql = "SELECT
											tpmid AS codigo,
											tpmdsc AS descricao
										  FROM
										  	territorios.tipomunicipio mtq
										  WHERE
										  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 156, 162, 167)";
								
								if( $_REQUEST['grupo'][0] != '' ){
									$grupos = implode( $_REQUEST['grupo'], "," );
									$sql = "SELECT
											tpmid AS codigo,
											tpmdsc AS descricao
										  FROM
										  	territorios.tipomunicipio mtq
										  WHERE
										  	tpmid IN ({$grupos})";
									$grupo = $db->carregar( $sql );
								}
	
								combo_popup( "grupo", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400 );
											
								?>
								</td>
							</tr>
							<tr class="tr_detalhe">
								<td colspan="2"> 
									<div style="padding-bottom:10px;padding-top:15px;">	
									<label><input type="checkbox" name="naocomecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['naocomecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Não Iniciou preenchimento</label>
									<br><p></p>
									<label><input type="checkbox" name="comecoudiagnostico" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['comecoudiagnostico'] == 1 ? 'checked="checked"' : '' ?>/> Iniciou preenchimento</label>
									<br><p></p>
									<label><input type="checkbox" name="dadosunidadecheck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['dadosunidadecheck'] == 1 ? 'checked="checked"' : '' ?>/> 100% dados da unidade preenchido</label>
									<br><p></p>
									<label><input type="checkbox" name="questoespontuaischeck" value="1" class="normal" style="margin-top: -1px;" <?= $_POST['questoespontuaischeck'] == 1 ? 'checked="checked"' : '' ?>/> 100% questões pontuais respondido / iniciou a pontuação</label>
								</td>
							</tr>
							<tr class="tr_detalhe">
								<td colspan="2">
									Situação<br/>
									<?php
							
									
									$esdid = $_POST["esdid"];
									$sql = "select codigo, descricao from ((select esdid as codigo, esddsc as descricao, esdordem from workflow.estadodocumento where tpdid = 44 order by esdordem )
											union all (select 99 as codigo, 'Em Atualização', 10 as descricao)) as foo";
									
									$res = $db->carregar( $sql );
									
									$db->monta_combo( "esdid", $res , 'S', 'Todas as Situações', '', '' );
									?>
								</td>
								<td colspan="1">
								</td>
							</tr>
							<tr class="tr_detalhe">
								<td valign="bottom" colspan="2">
									Com PAR / Sem PAR
									<br/>
									<?php 
										$comsempar = $_POST['comsempar'];
										$arrcomsempar = array(0 => array("codigo"=>"compar","descricao"=>"Com Par"), 
														   1 => array("codigo"=>"sempar","descricao"=>"Sem Par"));
										$db->monta_combo( "comsempar", $arrcomsempar, 'S', 'Selecione', '', '' ); 
									?>
								</td>
							</tr>
							<tr class="tr_detalhe">
								<td>
									<br />
									<style> #ideb_, #ideb { width: 300px; } </style>
									<table border="0" cellpadding="3" cellspacing="0">
										<tr>
											<td width="342">
												Classificação IDEB
											</td>
											<td>
												Filtrar por:
											</td>
										</tr>
									</table>
									<?php
									$agrupador = new Agrupador( "formulario" );
									
									$sql = sprintf( "SELECT tpmid, tpmdsc FROM territorios.tipomunicipio WHERE gtmid = 2 AND tpmstatus = 'A'" );
									$tipos = $db->carregar( $sql );
									
									$origem = array();
									$destino = array();
									if ( $tipos ) {
										foreach ( $tipos as $tipo ) {
											if ( in_array( $tipo['tpmid'], (array) $_REQUEST['ideb'] ) ) {
												$destino[] = array(
													'codigo'    => $tipo['tpmid'],
													'descricao' => $tipo['tpmdsc']
												);
											} else {
												$origem[] = array(
													'codigo'    => $tipo['tpmid'],
													'descricao' => $tipo['tpmdsc']
												);
											}
										}
									}
									
									$agrupador->setOrigem( "ideb_", null, $origem );
									$agrupador->setDestino( "ideb", null, $destino );
									$agrupador->exibir();
									?>
								</td>
							</tr>
							<tr>
								<td>
									<!--  <input type="button" name="pesquisar" value="Pesquisar" onclick="submitFormulario();"/> -->
									<input type="submit" name="pesquisar" value="Pesquisar" onclick="submitFormulario();"/>
									<?php 
									$arrayPerfil = pegaArrayPerfil($_SESSION['usucpf']);
									if( in_array(PAR_PERFIL_SUPER_USUARIO,$arrayPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrayPerfil)  ){
									?>
									<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
									<?php } ?>
								</td>
							</tr>
						</table>
					</div>
				</td>
			</tr>
		</tbody>
	</table>
</form>

<?php
$obEntidadeParControle = new EntidadeParControle();
$obEntidadeParControle->listaMunicipio($_POST);
?>
</table>
<div id="divDebug"></div>