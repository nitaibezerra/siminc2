<?php
/**
 * Created by PhpStorm.
 * User: victormachado
 * Date: 02/09/2015
 * Time: 09:59
 */
ob_start();
require_once "../../global/config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/library/simec/Listagem.php";
$db = new cls_banco();

switch($_REQUEST['acao']){
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

    case 'carrega_municipio':
        $estuf  = $_POST['estuf'];
        $selMun = $_POST['selMun'];
        $html = <<<HTML
                <button id="municipio-toggle" class="btn btn-primary" >Selecionar Todos</button>
<!--//                <select multiple="multiple" id="municipio" name="municipio" onchange="javascript:selecionaEstado(this);Mapas.buscaEstadoForm( $('#estado'), $('#tpmid'), this, 'assessoramento' );Mapas.atualizaLegenda( $('#estado'), this, 'assessoramento-legenda' )" class="multiselect" >-->
                <select style="width: 240px;" id="municipio" name="municipio" onchange="javascript:carregaLista();" multiple="multiple" class="multiselect"  >
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

    case 'lista_arquivos':
//        ver($_REQUEST['estado'], $_REQUEST['muncod']);
        $estado = $_REQUEST['estado'] ? " and iu.mun_estuf in ('".implode("','", $_REQUEST['estado'])."')" : '';
        $muncod = $_REQUEST['muncod'] ? " and iu.muncod in ('".implode("','", $_REQUEST['muncod'])."')" : '';
        $sql = <<<DML
                        select
                            mun.muncod,
                            arq.arqid,
                            mun.mundescricao,
                            arq.arqnome
                        from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        inner  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                        inner  join public.arquivo arq on arq.arqid = bna.arqid
                        inner join territorios.municipio mun on mun.muncod = iu.muncod
                        where 1=1
                        {$estado}
                        {$muncod}
                        order by mun.mundescricao
DML;
        $dados = $db->carregar($sql);

        $render = new Simec_Listagem_Renderer_Html();
        $render->setFormFiltros('form');

        $list = new Simec_Listagem();
        $list->setTamanhoPagina(50);
        $list->setCabecalho(array(
            'Municipio',
            'Arquivos'
        ));
        $list->setFormFiltros('form');
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
        $list->setDados($dados);
        $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $list->render();
        exit();

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
}

?>
<script src="/../library/jquery/jquery-1.10.2.js" type="text/javascript" charset="ISO-8895-1"></script>

<script src="../library/chosen-1.0.0/chosen.jquery.min.js"></script>

<script src="../sase/js/bootstrap-multiselect/js/bootstrap-3.0.3.min.js"></script>
<script src="../sase/js/bootstrap-multiselect/js/bootstrap-multiselect.js"></script>
<script src="../includes/funcoes.js"></script>

<link rel="stylesheet" href="../library/simec/css/css_reset.css">
<link rel="stylesheet" href="../library/chosen-1.0.0/chosen.min.css"/>

<!--<link href="/../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">-->
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.css" type="text/css" media='screen'/>

<link rel='stylesheet' type='text/css' href='/../includes/loading.css'/>
<link rel='StyleSheet' href="../sase/css/estilo.css" type="text/css" media='screen'/>
<!-- dependencias -->

<script>
    function selEsfera(esfera){
        switch (esfera){
            case 'E':
                var a = [];
                jQuery('#municipio').val(a);
                jQuery('#municipio').multiselect('refresh');
                jQuery('#municipio-toggle').attr('disabled', 'true');
                jQuery('#municipio').multiselect('disable');
                break;

            case 'M':
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
                    url: '',
                    type: 'POST',
                    data: {
                        acao: 'carrega_municipio',
                        estuf: jQuery(estuf).val(),
                        selMun: jQuery('#municipio').val()
                    },
                    success: function(data){
                        jQuery('#divMunicipio').html(data);
                        jQuery('#municipio').multiselect({
                            numberDisplayed: 14,
                            id: 'municipio'
                        });
                    }
                });
                break;
        }
    }

    function selecionaEstado(muncod){
        mun = jQuery(muncod).val();
            jQuery.ajax({
                url: '',
                type: 'POST',
                dataType: 'json',
                data: {
                    acao: 'pega_estado',
                    muncod: mun
                },
                success: function(data){
                    jQuery('#estado').val(data).multiselect("refresh");
                    carregaMunicipio(jQuery('#estado'));
                    carregaLista();
                }
            });
    }

    function carregaLista(pagina)
    {
        var estado = $('#estado').val();
        var muncod = $('#municipio').val();

        var data = {
            acao: 'lista_arquivos',
            estado: estado,
            muncod: muncod
        };

        if (pagina) {
            data['listagem[p]'] = pagina;
        }
        console.log('Estado: '+estado+' e municíios: '+muncod);
        jQuery.ajax({
            url: '',
            type: 'POST',
            data: data,
            success: function(res){
                jQuery('#divLista').html(res);
            }
        });
    }

    function downloadArquivo(id, arqid, muncod){
        window.open('listaArquivosBaseComum.php?acao=downloadPrincipal&arqid='+arqid+'&muncod='+muncod, "_blank", "toolbar=yes, scrollbars=yes, resizable=yes, top=0, left=0, width=10, height=10");
    }

</script>

<div id="container">
    <form class="form-horizontal" name="form" id="form" role="form" method="POST">
        <div class="row">

            <div class="col-lg-12">
                <div class="well">
                    <fieldset>
<!--                        <legend>teste</legend>-->
                        <div id="divMenu">
                            <div class="form-group col-lg-4">
                                <label class="col-lg-12" for="esfera">Esfera:</label>
                                <div class="col-lg-9">
                                    <select id="esfera" name="esfera" width="100" onchange="selEsfera(this.value)" class="form-control">
                                        <option value="E">Estadual</option>
                                        <option value="M">Municipal</option>
                                    </select>
                                </div>
                            </div>

                            <div class="form-group col-lg-4">
                                <label class="col-lg-12" for="estado">Estados:</label>
                                <div class="col-lg-12">
                                    <button id="estado-toggle" class="btn btn-primary">Selecionar Todos</button>
                                    <select multiple="multiple" id="estado" name="estado" onchange="javascript:carregaMunicipio(this);carregaLista();" class="multiselect">
                                        <?php $sql = "SELECT estuf, estdescricao FROM territorios.estado ";$estados = $db->carregar($sql);

                                        foreach ($estados as $key => $value) {
                                            echo "<option value='".$value['estuf']."'>".$value['estdescricao']."</option>";
                                        } ?>

                                    </select>
                                </div>
                            </div>

                            <div class="form-group col-lg-4">
                                <label class="col-lg-12" for="esfera">Municípios:</label>
                                <div class="col-lg-12" id="divMunicipio">
                                    <button id="municipio-toggle" class="btn btn-primary" disabled>Selecionar Todos</button>
                                    <select style="width: 240px;" id="municipio" name="municipio" onchange="javascript:carregaLista();" multiple="multiple" class="multiselect" disabled>
                                        <option value="">Selecione</option>
                                    </select>
                                </div>
                            </div>

<!--                            <div style='float:left;margin-left:20px;padding-top: 8px;'>Municípios:</div>-->
<!--                            <div style='float:left;margin-left:15px;' id="divMunicipio">-->
<!--                            </div>-->
                        </div>

                    </fieldset>
                </div>
            </div>
        </div>
    </form>
    <div class="col-lg-12 col-md-12 col-sm-12">
        <div id="divLista">
            <?php
            $sql = <<<DML
                        select
                            mun.muncod,
                            arq.arqid,
                            mun.mundescricao,
                            arq.arqnome
                        from par.basenacionalcomum bn
                        inner join par.instrumentounidade iu on iu.inuid = bn.inuid
                        inner  join par.basenacionalcomumarquivo bna on bna.bncid = bn.bncid
                        inner  join public.arquivo arq on arq.arqid = bna.arqid
                        inner join territorios.municipio mun on mun.muncod = iu.muncod
                        where 1=1
                        order by mun.mundescricao
DML;
            $dados = $db->carregar($sql);

            $list = new Simec_Listagem();
            $list->setTamanhoPagina(50)
                ->setFormFiltros('form');
            $list->setCabecalho(array(
                'Municipio',
                'Arquivos'
            ));
            $list->setFormFiltros('form');
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
            $list->setDados($dados);
            $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
            $list->render();
            ?>
        </div>
    </div>
</div>
<script>

    jQuery('document').ready(function(){
        jQuery('#estado').multiselect({
            numberDisplayed: 14,
            id: 'estado'
        });
        jQuery('#municipio').multiselect({
            numberDisplayed: 14,
            id: 'municipio'
        });
        jQuery('#tpmid').multiselect({
            numberDisplayed: 14,
            id: 'tpmid'
        });
        jQuery("#estado-toggle").click(function(e) {
            e.preventDefault();
            if (jQuery('#estado').val() != null) {
                carregaMunicipio(jQuery('#estado'));
            }
        });
        jQuery("#municipio-toggle").click(function(e) {
            e.preventDefault();
        });
    });

    delegatePaginacao = function()
    {
        $('body').on('click', '.container-listing li[class="pgd-item"]:not(".disabled")', function(){
            // -- definindo a nova página
            var novaPagina = $(this).attr('data-pagina');
            // -- Submetendo o formulário
            carregaLista(novaPagina);
        });
    };
</script>
