<?php
ob_start();
$_SESSION['sislayoutbootstrap'] = true;
// --------------- Dependências
include_once '../../par/classes/Mapa/MetaDados.class.inc';
include_once '../../par/classes/Mapa/Poligonos.class.inc';
include_once '../../par/classes/Mapa/Mapas.class.inc';
//include_once '../../sase/classes/Assessoramento.class.inc';
//include_once '../../sase/classes/SituacaoAssessoramento.class.inc';

if (!defined('LINK_PADRAO')) { define('LINK_PADRAO', 'par.php?modulo=principal/mapa&acao=A'); }

switch ($_REQUEST['acao']){
    case 'downloadPrincipal':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        if ($_REQUEST['arqid']) {
            ob_clean();
            $arquivo = $file->getDownloadArquivo($_REQUEST['arqid']);
            //ver($_REQUEST['arqid'], d);
            echo "		<script>
							window.close();
						</script>";
        } else {
            echo "		<script>
		                    alert('Arquivo inválido.');
		                    window.close();
						</script>";
        }
        exit();
        break;
    case 'download':
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $file = new FilesSimec();
        if ($_REQUEST['arqid']) {
            ob_clean();
            $arquivo = $file->getDownloadArquivo($_REQUEST['arqid']);
            //ver($_REQUEST['arqid'], d);
            echo "		<script>
							window.location.href = '".LINK_PADRAO."&acao=montaBalao&muncod=".$_REQUEST['muncod']."';
						</script>";
        } else {
            echo "		<script>
		                    alert('Arquivo inválido.');
							window.location.href = '".LINK_PADRAO."&acao=montaBalao&muncod=".$_REQUEST['muncod']."';
						</script>";
        }
        exit();
        break;
    case 'montaBalao':
        global $db;
        require_once APPRAIZ . 'includes/library/simec/Listagem.php';

        $sql = "select
                bn.bncid,
                bna.bnaid,
                bna.arqid,
                arq.arqnome,
                arq.arqdescricao,
                mun.muncod,
                mun.mundescricao,
                case
                        when bncperg1 = 'a' then 'a - Sim'
                        when bncperg1 = 'b' then 'b - Não'
                        when bncperg1 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg1 = 'd' then 'd - Não possuo informações sobre o assunto'
                        --else bncperg1
                        else 'Não respondeu'
                end as bncperg1,
                case
                        when bncperg2 = 'a' then 'a - Sim'
                        when bncperg2 = 'b' then 'b - Não'
                        when bncperg2 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg2 = 'd' then 'd - Não possuo informações sobre o assunto'
                        --else bncperg2
                        else 'Não respondeu'
                end as bncperg2,
                case
                        when bncperg3 = 'a' then 'a - Sim'
                        when bncperg3 = 'b' then 'b - Não'
                        when bncperg3 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg3 = 'd' then 'd - Não possuo informações sobre o assunto'
                        --else bncperg3
                        else 'Não respondeu'
                end as bncperg3,
                case
                        when bncperg4 = 'a' then 'a - Sim'
                        when bncperg4 = 'b' then 'b - Não'
                        when bncperg4 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg4 = 'd' then 'd - Não possuo informações sobre o assunto'
                        --else bncperg4
                        else 'Não respondeu'
                end as bncperg4,
                case
                        when bncperg5 = 'a' then 'a - Sim'
                        when bncperg5 = 'b' then 'b - Não'
                        when bncperg5 = 'c' then 'c - Não, mas está em elaboração'
                        when bncperg5 = 'd' then 'd - Não possuo informações sobre o assunto'
                        --else bncperg5
                        else 'Não respondeu'
                end as bncperg5
                from par.basenacionalcomum bn
                    inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                    --left join territorios.municipio mun on mun.muncod = iu.muncod
                    left  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                    left  join public.arquivo arq on arq.arqid = bna.arqid
                    right join territorios.municipio mun on mun.muncod = iu.muncod
                where mun.muncod = '".$_REQUEST['muncod']."'";

        $dados = $db->carregar($sql);

        $dadosA = array();
        $i = 0;
        if (is_array($dados)) {
            foreach ($dados as $dado) {
                if ($dado['arqid'] != '') {
                    $dadosA[$i]['arqid'] = $dado['arqid'];
                    $dadosA[$i]['arqnome'] = $dado['arqnome'];
                    $dadosA[$i]['muncod'] = $dado['muncod'];
                    $i++;
                }
            }
        }


        echo <<<HTML
            <script>
                function downloadArquivo(id, arqid, muncod){
                    //alert(id);
                    jQuery('[name=acao]').val('download');
                    jQuery('[name=arqid]').val(arqid);
                    jQuery('[name=muncod]').val(muncod);
                    jQuery('[name=formCadastroLista]').submit();
                }
            </script>
            <link rel='StyleSheet' href="../../library/bootstrap-3.0.0/css/bootstrap.css" type="text/css" media='screen'/>
            <script src="../library/jquery/jquery-1.11.1.min.js" type="text/javascript" charset="ISO-8895-1"></script>
            <script src="../library/jquery/jquery-ui-1.10.3/jquery-ui.min.js"></script>
            <form id="form-save" method="post" name="formCadastroLista" role="form" class="form-horizontal">
                <input type="hidden" name="acao" id="acao" />
                <input type="hidden" name="arqid" id="arqid" />
                <input type="hidden" name="muncod" id="muncod" />
                <div class="col-lg-12">
                    <div class="well">
                        <fieldset>
                            <legend>Respostas - {$dados[0]['mundescricao']}</legend>
                            <ul style="list-style-type: none; padding-left: 10px !important;">
                                <li style="font-weight: bold;">EDUCAÇÃO INFANTIL</li>
                                <li style="padding-left: 10px;">R: {$dados[0]['bncperg1']}</li>
                                <li style="font-weight: bold;">ENSINO FUNDAMENTAL - ANOS INICIAIS</li>
                                <li style="padding-left: 10px;">R: {$dados[0]['bncperg2']}</li>
                                <li style="font-weight: bold;">ENSINO FUNDAMENTAL - ANOS FINAIS</li>
                                <li style="padding-left: 10px;">R: {$dados[0]['bncperg4']}</li>
                                <li style="font-weight: bold;">ENSINO MÉDIO</li>
                                <li style="padding-left: 10px;">R: {$dados[0]['bncperg3']}</li>
                                <li style="font-weight: bold;">EDUCAÇÃO DE JOVENS E ADULTOS</li>
                                <li style="padding-left: 10px;">R: {$dados[0]['bncperg5']}</li>
                            </ul>
                        </fieldset>
                    </div>

                    <div>
HTML;
        //ver($dadosA);
        $list = new Simec_Listagem();
        $list->setTamanhoPagina(50);
        $list->setCabecalho(array(
            'Arquivos'
        ));
        $list->esconderColunas(array(
            'arqid',
            'muncod'
        ));
        $list->addAcao('download', array(
            'func' => 'downloadArquivo',
            'titulo' => 'Download do arquivo',
            'extra-params' => array(
                'arqid',
                'muncod'
            )
        ));
        $list->setDados($dadosA);
        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->render();

        echo <<<HTML
                    </div>
                </div>
            </form>
HTML;
;
        exit();
        break;

    case 'carrega_municipio':
        $estuf  = $_POST['estuf'];
        $selMun = $_POST['selMun'];
        $html = <<<HTML
                <button id="municipio-toggle" class="btn btn-primary" >Selecionar Todos</button>
<!--//                <select multiple="multiple" id="municipio" name="municipio" onchange="javascript:selecionaEstado(this);Mapas.buscaEstadoForm( $('#estado'), $('#tpmid'), this, 'assessoramento' );Mapas.atualizaLegenda( $('#estado'), this, 'assessoramento-legenda' )" class="multiselect" >-->
                <select style="width: 240px;" id="municipio" name="municipio" onchange="javascript:selecionaEstado(this);Mapas.buscaEstadoForm( $('#estado'), $('#tpmid'), this, 'assessoramento' );Mapas.atualizaLegenda( $('#estado'), this, 'assessoramento-legenda' )" class="chosen-select" >
                    <option value="">Selecione</option>
HTML;
        if (is_array($estuf)) {
            $sql = "SELECT muncod, mundescricao FROM territorios.municipio WHERE estuf in ('".implode("','", $estuf)."') ORDER BY mundescricao";
        } else {
            $sql = "SELECT muncod, mundescricao FROM territorios.municipio ORDER BY mundescricao";
        }

        $mun = $db->carregar($sql);

        foreach ($mun as $key => $value) {
            if (is_array($selMun)) {
                if (in_array($value['muncod'], $selMun)){
                    $html .= "<option value='" . $value['muncod'] . "' selected>" . $value['mundescricao'] . "</option>";
                } else {
                    $html .= "<option value='" . $value['muncod'] . "'>" . $value['mundescricao'] . "</option>";
                }
            } else {
                $html .= "<option value='" . $value['muncod'] . "'>" . $value['mundescricao'] . "</option>";
            }
        }

        $html .= <<<HTML
                </select>
HTML;
        echo $html;
        exit();

    case 'pega_estado':
        $muncod = $_POST['muncod'];
        if (is_array($muncod)) {
            $sql = "select distinct estuf from territorios.municipio where muncod in ('" . implode("','", $muncod) . "')";
        } else {
            $sql = "select distinct estuf from territorios.municipio where muncod = '$muncod'";
        }
        $estuf = $db->carregar($sql);

        $et = array();
        if (is_array($estuf)) {
            foreach ($estuf as $k => $v) {
                $et[] = $v['estuf'];
            }
        }

        echo json_encode($et);
        //ver($estuf, $et, implode(",", $et), d);
        exit();
}

include APPRAIZ . 'includes/cabecalho.inc';

$sql = " SELECT  ";

?>
<script src="/sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<!-- dependencias -->
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<!-- http://hpneo.github.io/gmaps/ -->
<script type="text/javascript" src="/../includes/gmaps/gmaps.js"></script>
<script src="/par/js/Mapas.js"></script>
<!-- http://www.downscripts.com/bootstrap-multiselect_javascript-script.html -->
<script src="/sase/js/functions.js"></script>
<script>
    html_municipio = "<div style=\"padding:5px\" ><iframe src=\"<?=LINK_PADRAO ?>&acao=montaBalao&muncod={muncod}\" frameborder=0 scrolling=\"auto\" height=\"300px\" width=\"420px\" ></iframe></div>";
    function selEsfera(esfera){
        switch (esfera){
            case 'E':
                var a = [];
                jQuery('#municipio').val(a);
                jQuery('#municipio').multiselect('refresh');
                jQuery('#municipio-toggle').attr('disabled', 'true');
                jQuery('#municipio').multiselect('disable');
                Mapas.buscaEstadoForm( $('#estado'), $('#tpmid'), $('#municipio'), 'assessoramento' );
                Mapas.atualizaLegenda( $('#estado'), 'assessoramento-legenda' );
                break;

            case 'M':
                console.log(jQuery('#estado').val());
                if (jQuery('#estado').val() != null) {
                    carregaMunicipio(jQuery('#estado'));
                } else {
                    carregaMunicipio('');
                }
                jQuery('#municipio-toggle').removeAttr('disabled');
                jQuery('#municipio').multiselect('enable');
                break;
        }
    }

    function carregaMunicipio(estuf){
        //console.log(jQuery('#municipio').val());

        switch (jQuery('#esfera').val()){
            case 'M':
                jQuery.ajax({
                    url: '<?= LINK_PADRAO ?>',
                    type: 'POST',
                    data: {
                        acao: 'carrega_municipio',
                        estuf: jQuery(estuf).val(),
                        selMun: jQuery('#municipio').val()
                    },
                    success: function(data){
                        jQuery('#divMunicipio').html(data);
                        jQuery('#municipio').chosen();
                    }
                });
                break;
        }
    }

    function selecionaEstado(muncod){
        mun = jQuery(muncod).val();
            url: '<?= LINK_PADRAO ?>',
        jQuery.ajax({
            type: 'POST',
            dataType: 'json',
            data: {
                acao: 'pega_estado',
                muncod: mun
            },
            success: function(data){
                jQuery('#estado').val(data).multiselect("refresh");
                carregaMunicipio(jQuery('#estado'));
            }
        });
        console.log('2');
    }

    function downloadArquivo(id, arqid, muncod){
        window.open('<?= LINK_PADRAO ?>&acao=downloadPrincipal&arqid='+arqid+'&muncod='+muncod, "_blank", "toolbar=yes, scrollbars=yes, resizable=yes, top=0, left=0, width=10, height=10");
    }
</script>
<!-- /dependencias -->
<div class="page-header">
    <h4 id="forms">Mapa Base Nacional Comum</h4>
</div>
<br>
<form id="form-save" method="post" name="teste" role="form" class="form-horizontal">
    <input type="hidden" name="acao" id="acao" />
    <input type="hidden" name="arqid" id="arqid" />
    <input type="hidden" name="muncod" id="muncod" />

</form>
<div class="col-lg-12">
<div class="well">
    <fieldset>
          	<div id="local-mapa">
		<div id="menu">
            <div style="float: left">Esfera:</div>
            <div style='float:left;margin-left:15px;'>
                <select id="esfera" name="esfera" width="100" onchange="selEsfera(this.value)" class="campoEstilo">
                    <option value="E">Estadual</option>
                    <option value="M">Municipal</option>
                </select>
            </div>
			<div style='float:left;margin-left:15px;'>Estados:</div>
			<div style='float:left;margin-left:15px;'>
				<button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
				<select multiple="multiple" id="estado" name="estado" onchange="javascript:carregaMunicipio(this);Mapas.buscaEstadoForm( this, $('#tpmid'), $('#municipio'), 'assessoramento' );Mapas.atualizaLegenda( this, $('#municipio'), 'assessoramento-legenda' )" class="multiselect">
					<?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql); 
					
					foreach ($estados as $key => $value) {
						echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
					} ?>

				</select>
            </div>
            <div style='float:left;margin-left:15px;'>Municípios:</div>
            <div style='float:left;margin-left:15px;' id="divMunicipio">
                <button id="municipio-toggle" class="btn btn-primary" disabled>Selecionar Todos</button>
                <select style="width: 240px;" id="municipio" name="municipio" onchange="javascript:selecionaEstado(this);Mapas.buscaEstadoForm( $('#estado'), $('#tpmid'), this, 'assessoramento' );Mapas.atualizaLegenda( $('#estado'), this, 'assessoramento-legenda' )" class="chosen-select" disabled>
                    <option value="">Selecione</option>
                </select>
            </div>
            <!-- div style='float:left;margin-left:15px;'>Tipo de Municípios:</div>
            <div style='float:left;margin-left:15px;'>
                <select multiple="multiple" id="tpmid" name="tpmid" onchange="javascript:Mapas.buscaEstadoForm( $('#estado'), this, 'assessoramento' );Mapas.atualizaLegenda( $('#estado'), 'assessoramento-legenda' )" class="multiselect">
                    <?php $sql = "select tpmid, tpmdsc from territorios.tipomunicipio where tpmid in (1,170,172,153) order by tpmdsc";$tipos = $db->carregar($sql);

                    foreach ($tipos as $key => $value) {
                        echo "<option value='".$value['tpmid']."'>".$value['tpmdsc']."</option>";
                    } ?>

                </select>
			</div -->
			<div id="map_canvastxt"></div>
		</div>

		<div class="panel panel-default">
			<!-- <div class="panel-heading">Mapa</div> -->
			<div class="panel-body">
				<div id="directionsPanel" style="width: 300px"></div>
				<div id="map_canvas"></div>
			</div>
		</div>
	</div>
<!--<div style="clear:both"></div>
<div id="container">
	<div id="local-mapa">
		<div id="menu">
			<div style='float:left'>Estados:</div>
			<div style='float:left;margin-left:15px;'>
				<button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
				<select multiple="multiple" id="estado" name="estado" onchange="javascript:Mapas.buscaEstadoForm( this, 'assessoramento' );Mapas.atualizaLegenda( this, 'assessoramento-legenda' )" class="multiselect">
					<?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql); 
					
					foreach ($estados as $key => $value) {
						echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
					} ?>

				</select>
			</div>
			<div id="map_canvastxt"></div>
		</div>

		<div class="panel panel-default">
			 <div class="panel-heading">Mapa</div> 
			<div class="panel-body">
				<div id="directionsPanel" style="width: 300px"></div>
				<div id="map_canvas"></div>
			</div>
		</div>
	</div>-->

	<div id="legendaMapa">
		<div id="legendaMapaContainer">
			<div id="tituloLegenda"><h5>&nbsp;Legenda:</h5></div>
			<ul>
				<?php
				$sql = "select *,
	case 
		when legenda = 'Sim' then 'green'
		when legenda = 'Não' then 'red'
		else 'white'
	end as cor, COUNT(legenda) as quantidade
 from (
 select  case when (bncperg1 =  'a' or bncperg2 =  'a'  or bncperg3 =  'a' or   bncperg4 = 'a' or bncperg5 = 'a') then 'Sim' 
when bncid is null then 'Não respondeu'
else  'Não' end as legenda
           from territorios.municipio mun 
                  inner join par.instrumentounidade iu on mun.muncod = iu.muncod
                   left join par.basenacionalcomum bn on iu.inuid = bn.inuid
              -- " . (($estuf!=''&&count($estuf)>0)?" WHERE MUN.ESTUF IN ( '". (implode( "','", $estuf )) ."' ) ":"") . "

                    
) as cor
group by legenda order by cor";
		$lista = $db->carregar( $sql );
				foreach ($lista as $key => $value) {
					echo "<li ><table><tr><td><span style='background:" . $value['cor'] . ";' class='elementoCor'>&nbsp;&nbsp;&nbsp;</span>&nbsp;&nbsp;<b>" . (($value['quantidade']!='')?$value['quantidade']:'0') . "</b>&nbsp;&nbsp;</td><td>" . $value['legenda'] . "</td></tr></table></li>";
				}
				?>
			</ul>
		</div>
	</div>

</div>
</div>
</div>
<div style="clear:both"></div>

<div id="footer"></div>
<!-- /html -->


<!-- js -->
<script>

	jQuery('documento').ready(function(){
		Mapas.inicializar( '#map_canvas' );
	});

	jQuery('document').ready(function(){
        jQuery('#esfera').multiselect({
            numberDisplayed: 1,
            id: esfera
        });
		jQuery('#estado').multiselect({
	      numberDisplayed: 14,
	      id: 'estado'
	    });
//        jQuery('#municipio').multiselect({
//            numberDisplayed: 14,
//            id: 'municipio'
//        });
        jQuery('#municipio').chosen();
        jQuery('#tpmid').multiselect({
            numberDisplayed: 14,
            id: 'tpmid'
        });
	    jQuery("#estado-toggle").click(function(e) {
			e.preventDefault();
            if (jQuery('#estado').val() != null) {
                carregaMunicipio(jQuery('#estado'));
            }
			multiselect_toggle(jQuery("#estado"), jQuery(this));
            multiselect_toggle(jQuery("#tpmid"), jQuery(this));
			Mapas.buscaEstadoForm( jQuery("#estado"), jQuery("#tpmid"), jQuery("#municipio"), 'assessoramento' );
			Mapas.atualizaLegenda( jQuery("#estado"), jQuery('#municipio'), 'assessoramento-legenda' )
		});
        jQuery("#municipio-toggle").click(function(e) {
            e.preventDefault();
            Mapas.buscaEstadoForm( jQuery("#estado"), jQuery("#tpmid"), jQuery("#municipio"), 'assessoramento' );
            Mapas.atualizaLegenda( jQuery("#estado"), jQuery('#municipio'), 'assessoramento-legenda' )
        });
	});

</script>
