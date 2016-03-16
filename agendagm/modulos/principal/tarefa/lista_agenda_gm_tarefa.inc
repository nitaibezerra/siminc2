<?PHP
    include_once APPRAIZ . "agendagm/classes/modelo/Tarefa.class.inc";
    include_once APPRAIZ . "agendagm/classes/modelo/TarefaUsuario.class.inc";
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    #MENSAGENS PARA A INTERFACE DO USUÁRIO
    //require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
    //$fm = new Simec_Helper_FlashMessage('objetosfies/objetos');
    //echo $fm->getMensagens();

    $modelTarefa = new Agendagm_Model_Tarefa();

    switch( $_REQUEST['action'] ){
        case(''):
        break;
    }

    #CHAMADA DE PROGRAMA
    include  APPRAIZ."includes/cabecalho.inc";

    //echo $simec->title('Bem-vindo');

    #CRIA ABAS
    $abacod_tela    = 58332;
    $url            = 'agendagm.php?modulo=principal/tarefa/lista_agenda_gm_tarefa&acao=A';
    $parametros     = '';
    $db->cria_aba($abacod_tela, $url, $parametros);
?>

<style type="text/css">
    table thead > tr > th, .table tbody > tr > th, .table tfoot > tr > th, .table thead > tr > td, .table tbody > tr > td, .table tfoot > tr > td {
        padding: 0px !important;
    }
    .glyphicon-download-alt{
        background-color:#87CEFA;
    }
    .note-editable{
        background-color:#FFFFFF !important;
        height: 240px !important;
    }
    input, textarea, select, multiple, .note-editable,  .chosen-choices, .chosen-default{
        border:1px solid !important;
        border-color:#696969 !important;
    }
    .note-toolbar{
        border-top:1px solid !important;
        border-top-color:#696969 !important;

        border-right:1px solid !important;
        border-right-color:#696969 !important;

        border-left:1px solid !important;
        border-left-color:#696969 !important;
    }
</style>

<script type="text/javascript">

    $(document).ready(function(){
        $('.glyphicon-pencil').removeClass('btn-sm');
        $('.glyphicon-pencil').addClass('btn-xs');

        $('.glyphicon-trash').removeClass('btn-sm');
        $('.glyphicon-trash').addClass('btn-xs');

        $('.glyphicon-info-sign').removeClass('btn-sm');
        $('.glyphicon-info-sign').addClass('btn-xs');

        $('.navbar-listagem').hide();
    });

    function editarTarefa( trfid ){
        window.location.href ='agendagm.php?modulo=principal/tarefa/cad_agenda_gm_tarefa&acao=A&trfid='+trfid;
    }

    function novaTarefa(){
        window.location.href ='agendagm.php?modulo=principal/tarefa/cad_agenda_gm_tarefa&acao=A';
    }

</script>

<div id="div_lista_tarefas_pauta"></div>

<div class="wrapper wrapper-content animated fadeInRight" style="margin-top: -28px;">
    <div class="col-lg-12 col-md-12">
           <div class="ibox float-e-margins">
               <div class="ibox-title">
                   <h5>Gabinete do Ministro - Agenda</h5>
               </div>
           </div>
    </div>

    <div class="col-lg-12 col-md-12">
        <div class="ibox float-e-margins">
            <div class="ibox-title" style="background-color:#E0EEE0; margin-bottom:1px; border-bottom-color:#e7eaec;">
                <h5>Buscar Pendência</h5>
                <div class="ibox-tools">
                    <a class="collapse-link">
                        <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>

            <div class="ibox-content" style="padding-bottom:1px; background-color:#E0EEE0;">
                <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                    <input name="action" type="hidden" value="">
                    <?PHP
                        echo $simec->input('trfdsc', 'Pendência', $modelPauta->ptatema, array('placeHolder' => 'Tarefa', 'maxlengh' => 1000));
                    ?>

                    <div class="form-group">
                        <label class="control-label col-lg-2" for="">Prazo: </label>

                        <div class="col-lg-5">
                            <?PHP
                                echo $simec->data('trfprazo_ini', '');
                            ?>
                        </div>

                        <div class="col-lg-5">
                            <?PHP
                                echo $simec->data('trfprazo_fim', 'Fim');
                            ?>
                        </div>
                    </div>

                    <?PHP
                        $sql = "
                            SELECT  tu.usucpf AS codigo,
                                    su.usunome AS descricao
                            FROM agendagm.tarefausuario AS tu
                            JOIN seguranca.usuario AS su ON su.usucpf = tu.usucpf
                        ";

                        echo $simec->select( 'usucpf', 'Responsável', '', $sql, array('data-placeholder' => 'Selecione...') );

                        $sql_uam = "
                            SELECT  uamid AS codigo,
                                    UPPER(uamdsc) AS descricao
                            FROM public.unidadeareamec
                            WHERE uamstatus = 'A'
                            ORDER BY descricao
                        ";
                        echo $simec->select( 'uamid', 'Unidade MEC', '', $sql_uam, array('data-placeholder' => 'Selecione...') );
                    ?>

                    <div class="form-group">
                        <div class="col-lg-6 text-right">
                            <button type="submit" class="btn btn-sm btn-success btn-lg">
                                <span class="glyphicon glyphicon-search"> </span> Pesquisar
                            </button>
                        </div>
                        <div class="col-lg-6">
                            <button type="submit" class="btn btn-sm btn-danger btn-lg">
                                <span class="glyphicon glyphicon-share"></span> Ver Todos
                            </button>
                        </div>
                    </div>
                </form>
            </div>
        </div>

        <div class="ibox float-e-margins">
            <div class="ibox-title" style="background-color:#E0EEE0; margin-bottom:1px; border-bottom-color:#e7eaec;">
                <h5> Pendência </h5>
                <div class="ibox-tools">
                    <a class="collapse-link">
                        <i class="fa fa-chevron-up"></i>
                    </a>
                </div>
            </div>
            <div class="ibox-content" style="background-color:#E0EEE0;">
                <div>
                    <button type="button" class="btn btn-xs btn-warning btn-lg" onclick="novaTarefa();">
                        <span class="glyphicon glyphicon-plus"></span> Nova Pendência
                    </button>
                </div>
                <div>
                    <?PHP
                        $modelTarefa->listagemGridTarefas( $_REQUEST );
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>