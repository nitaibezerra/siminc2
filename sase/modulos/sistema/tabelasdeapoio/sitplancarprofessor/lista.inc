<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 26/08/2015
 * Time: 10:21
 */


include_once '../../sase/classes/Sitplancarprofessor.class.inc';
$situacao = new Sitplancarprofessor();
$pfls     = arrayPerfil();

if($_POST['acao']){
    ob_clean();
    switch($_POST['acao']){
        case 'salvar':
            $situacao->carregarPorId($_POST['spcid']);
            $situacao->spccor = $_POST['spccor'];
            $situacao->alterar();
            echo "
                <script>
                    alert('Situação alterada com sucesso.');
                    window.location.href = '/sase/sase.php?modulo=sistema/tabelasdeapoio/sitplancarprofessor/lista&acao=A';
                </script>";
            break;

        case 'edicao':
            $situacao->carregarPorId($_POST['spcid']);
            $arr = array(
                'spcdsc' => $situacao->spcdsc,
                'spccor' => $situacao->spccor
            );
            echo simec_json_encode($arr);
            exit;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';

global $db;

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";
$titulo = 'Situação de Plano de Carreirao do Professor';

?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

<div id='cabecalhoSistema'>
    <div id='cabecalhoSistema'>
        <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
        <center><h3><?=$titulo?></h3></center>
    </div>
</div>

<script>
    function editSituacao(spcid){
        jQuery.ajax({
            url: '',
            type: 'POST',
            data:{acao:'edicao',spcid:spcid},
            success: function(data){
                var d = JSON.parse(data);
                jQuery('#spcid').val(spcid);
                jQuery('#spcdsc').val(d['spcdsc']);
                jQuery('#spccor').val(d['spccor']);
                jQuery('#btnSalvar').removeAttr('disabled');
            }
        });
    }

    function salvaSituacao(){
        jQuery('#acao').val('salvar');
        jQuery('#formSituaLista').submit();
    }
</script>
<div id="container">
    <div id="formularioFiltros">
        <form class="form-horizontal" name="formSituaLista" id="formSituaLista" role="form" method="POST">
            <input type="hidden" name="acao" id="acao" value=""/>
            <input type="hidden" name="spcid" id="spcid" value=""/>

            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Descrição:</label>
                <div class="col-sm-9 inputarea">
                    <input name="spcdsc" id="spcdsc" style="width: 550px;" disabled value="<?=($_POST['spcdsc'])?$_POST['spcdsc']:''?>"/>
                </div>
            </div>

            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroStadsc" class="col-sm-2 control-label">Cor:</label>
                <div class="col-sm-9 inputarea">
                    <input type="color" name="spccor" id="spccor" value="<?=($_POST['spccor'])?$_POST['spccor']:''?>"/>
                </div>
            </div>

            <div class="form-group">
                <label class="col-sm-2 control-label">&nbsp;</label>
                <div class="col-sm-9">
                    <input type="button" class="btn btn-primary" disabled onclick="salvaSituacao()" id="btnSalvar" value="Salvar" />
                </div>
            </div>

        </form>
    </div>
    <div id="lista">
        <?php $situacao->getLista('B'); ?>
    </div>
</div>