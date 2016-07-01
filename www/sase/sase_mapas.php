<?php
if(isset($_GET['uf']) && $_GET['uf'] == 'DF'){
	echo "<script> alert('Distrito Federal - Sem municípios para exibição.');window.close();</script>";
    die();
}

// carrega as funções gerais
require_once '../../global/config.inc';

// CPF do administrador de sistemas
if(!$_SESSION['usucpf']){
	$_SESSION['usucpf'] = '';
	$_SESSION['usucpforigem'] = '';
	$auxusucpf = '';
	$auxusucpforigem = '';
}else{
	$auxusucpf = $_SESSION['usucpf'];
	$auxusucpforigem = $_SESSION['usucpforigem'];
}

$_SESSION["sisid"] = 183;

include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$db = new cls_banco();

// Classes
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/MetaDados.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/Poligonos.class.inc';
include_once APPRAIZ . 'sase/classes/Mapa/Mapas.class.inc';
include_once APPRAIZ . 'sase/classes/Assessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'sase/classes/QuestoesPontuaisPar.class.inc';
include_once APPRAIZ . 'sase/classes/Territorio.class.inc';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

if (!defined('LINK_PADRAO')) { define('LINK_PADRAO', 'sase_mapas.php'); }

if($_REQUEST['acao']){
    switch($_REQUEST['acao']){
        case 'download':
            $estuf = $_REQUEST['estuf'];
            $muncod = $_REQUEST['muncod'];
            $sql = "select assleipne from sase.assessoramento where muncod = '{$muncod}'";
            $arqid = $db->pegaUm($sql);
            $file = new FilesSimec('assessoramento', array(), 'sase');
            if ($arqid) {
                ob_clean();
                $arquivo = $file->getDownloadArquivo($arqid);
                echo "<script>window.location.href = '".LINK_PADRAO."?&uf={$estuf}';</script>";
            } else {
                echo "<script>alert('Lei do PNE ainda não disponível para download.');window.location.href = '".LINK_PADRAO."?uf={$estuf}';</script>";
            }
            exit();
        break;
		case 'downloadEstado':
            $estuf = $_REQUEST['estuf'];
            $sql = "select aseleipne from sase.assessoramentoestado where estuf = '{$estuf}'";
            $arqid = $db->pegaUm($sql);
            $file = new FilesSimec('assessoramentoestado', array(), 'sase');
            
            if ($arqid) {
            	ob_clean();
            	$arquivo = $file->getDownloadArquivo($arqid);
           		echo "<script>window.location.href = '".LINK_PADRAO."?&uf={$estuf}';</script>";
            } else {
            	echo "<script>alert('Lei do PNE ainda não disponível para download.');window.location.href = '".LINK_PADRAO."?uf={$estuf}';</script>";
           	}
           	exit();
		break;            
    }
}

//ver( $_GET['uf'],d);
if( $_GET['uf'] ){
        if($_GET['uf'] == 'DF'){
            echo '<h3>Distrito Federal - </h3>Sem municípios para exibição';
            die();
        }
	$estado = $db->pegaUm( " select estdescricao from territorios.estado where estuf = '{$_GET['uf']}' " );
}else{
	echo "<h3>Estado não especificado.</h3>";exit;
}

get_header(); ?>

<style> .conteudo-sase{ width:100%; } </style>

<!-- SASE -->
<link rel='StyleSheet' href="/sase/css/estilo_alternativo.css" type="text/css" media='screen'/>
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/font-awesome/4.4.0/css/font-awesome.min.css">
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.css">
<link rel="stylesheet" href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" media="screen">
<link rel="stylesheet" href="../library/chosen-1.0.0/chosen.css" media="screen" >

<!-- dependencias -->
<script src="../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>
<script src="http://maps.google.com/maps/api/js?sensor=false" type="text/javascript" ></script>
<script src="/../includes/gmaps/gmaps.js" type="text/javascript"></script>
<script src="/sase/js/Mapas.js"></script>
<script src="/sase/js/jquery.blockUI.js"></script>
<script src="/sase/js/functions.js"></script>   
<script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>

<style>
	#componentesMapa{
		top:62px;
		left:13px;
		position:absolute;
		border-radius: 2px;
	}
	#legendaMapa {
		margin-top: 15px;
	}
	#muncod {
		margin: 1px 0px 0px 0px;
		padding: 8px;
	}
	#map_canvastxt {
		margin-top: 9px;
		margin-left: -10px;
		font-size: 20px;
	}
	.download {
		float: right;
		font-family: Arial;
		margin-top: -30px;
	}
	.download a {
		text-decoration: none;
		color: #000;
		font-weight: bold;
	}
</style>

    <div class="loading-dialog" id="loading" style="display: none;">
        <div id="overlay" class="loading-dialog-content">
            <div class="ui-dialog-content">
                <img src="../library/simec/img/loading.gif">
            </div>
        </div>
    </div>

<!-- /dependencias -->
<form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
    <input type="hidden" name="acao" id="acao" />
    <input type="hidden" name="estuf" id="estuf" value="<?= $_GET['uf'] ?>" />
    <input type="hidden" name="muncod" id="muncod" />
    <div class="conteudo-sase">

	<div id="container" style="overflow:none;height:560px;">
		<div class="download">
			<a href="sase_mapas.php?acao=downloadEstado&estuf=<?= $_GET['uf'] ?>"> <i class="fa fa-download"></i> Baixar Lei do plano estadual - <?= $_GET['uf'] ?></a>
		</div>
		<div class="panel panel-default">
			<div class="panel-body">
				
				<div id="map_canvas"></div>

				<div id="legendaMapa"></div>

				<div id="componentesMapa"></div>
			</div>
		</div>

	</div>

	<!-- <div id="footer"></div> -->
	<!-- /html -->


	<!-- js -->
	<input type="hidden" id="uf" value="<?=$_GET['uf']?>"/>
	<script>
		var MapaEstado = Mapas;

        function downloadLei(muncod){
            jQuery('[name=acao]').val('download');
            jQuery('[name=muncod]').val(muncod);
            jQuery('[name=formCadastroLista]').submit();
        }

		function centralizaNoEstado(){
			MapaEstado.mostraPosicao( 'centralizaNoEstado' );

			var url = MapaEstado.origensDasRequisicoes();
			jQuery.ajax({
				type: 'POST',
				url: url.url,
				data: {chamadoMapas:1,origemRequisicao:'sase-mapas-estado-externo',params:<?=simec_json_encode(array('estado'=>$_GET['uf']))?>},
				success: function( resposta ){
					resposta = JSON.parse( resposta );

					var multipoligonos = resposta[0].poli.coordinates;
					var boundbox = new google.maps.LatLngBounds();

					var maior = [];
					maior[0] = 0;
					maior[1] = undefined;
					for (var i = multipoligonos.length - 1; i >= 0; i--) {
						if( multipoligonos[i][0].length > maior[0] ){
							maior[0] = multipoligonos[i][0].length;
							maior[1] = i;
						}

						if( i == 0 ){
							// console.log(multipoligonos[maior[1]][0].length);

							for ( var is = 0; is < multipoligonos[maior[1]][0].length; is++ ){
								boundbox.extend(new google.maps.LatLng(multipoligonos[maior[1]][0][is][1],multipoligonos[maior[1]][0][is][0]));
								// console.log(multipoligonos[i][0][is][0]);

								if( is == multipoligonos[maior[1]].length-1 ){
									setTimeout(function(){
										MapaEstado.bounds = boundbox;
										// console.log(boundbox);
										MapaEstado.map.map.fitBounds( boundbox );
										MapaEstado.verificaSeCentroPermaneceNoBound();
									},10);
								}
							}
						}
					};



				}
			});

			jQuery('#legendaMapa').css('marginRight','18px');  

		}

		jQuery('documento').ready(function(){
			MapaEstado.estilo = 'externo_blank';
			MapaEstado.inicializar( '#map_canvas' );
			MapaEstado.posDesenhoPoligono = 'centralizaNoEstado';
			MapaEstado.buscaEstadoForm( '#uf', 'assessoramento-externo' );
			MapaEstado.atualizaLegenda( '#uf', 'assessoramento-legenda-externo' );
			MapaEstado.aplicaComponente( '#uf','componente-buscamunicipios', 'assessoramento-externo');
		});
	</script>
	<!-- /js -->

</div>
</form>