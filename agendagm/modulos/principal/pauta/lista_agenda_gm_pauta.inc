<?PHP
    include_once APPRAIZ . "agendagm/classes/modelo/Pauta.class.inc";
    include_once APPRAIZ . "agendagm/classes/modelo/UsuarioAssessor.class.inc";
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    #MENSAGENS PARA A INTERFACE DO USUÁRIO
    //require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
    //$fm = new Simec_Helper_FlashMessage('objetosfies/objetos');
    //echo $fm->getMensagens();

    $modelPauta = new Agendagm_Model_Pauta( $_REQUEST['ptaid'] );

    switch( $_REQUEST['action'] ){
        case('infoTarefas'):
            infoTarefas( $_REQUEST );
        break;
    }

    #CHAMADA DE PROGRAMA
    include  APPRAIZ."includes/cabecalho.inc";

    //echo $simec->title('Bem-vindo');
    
    #CRIA ABAS
    $abacod_tela    = 58332;
    $url            = 'agendagm.php?modulo=principal/pauta/lista_agenda_gm_pauta&acao=A';
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

    function infoTarefas( ptaid ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "action=infoTarefas&ptaid="+ptaid,
            async   : false,
            success: function(resp){
                $('#div_lista_tarefas_pauta').html(resp);
                $('#lista_tarefas_pauta').modal();
            }
        });
    }

    function editarAgendaPauta( ptaid ){
        window.location.href ='agendagm.php?modulo=principal/pauta/cad_agenda_gm_pauta&acao=A&ptaid='+ptaid;
    }

    function novaAgendaPauta(){
        window.location.href ='agendagm.php?modulo=principal/pauta/cad_agenda_gm_pauta&acao=A';
    }

</script>

<div id="div_lista_tarefas_pauta"></div>

<div class="wrapper wrapper-content animated fadeInRight" style="margin-top: -28px;">
    <div class="row">
        <div class="col-lg-12 col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Gabinete do Ministro - Agenda</h5>
                </div>
            </div>

            <div class="ibox float-e-margins">
                <div class="ibox-title" style="background-color:#E0EEE0; margin-bottom:1px; border-bottom-color:#e7eaec;">
                    <h5>Filtros</h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                    </div>
                </div>

                <div class="ibox-content" style="background-color:#E0EEE0;">
                    <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                        <input name="action" type="hidden" value="salvar">
                        <input name="ptaid" type="hidden" value="<?=$modelPauta->ptaid;?>">

                        <?PHP
                            echo $simec->input('ptatema', 'Tema', $modelPauta->ptatema, array('placeHolder' => 'Tema', 'maxlengh' => 1000));

                            echo $simec->input('ptasubtema', 'Subtema', $modelPauta->ptasubtema, array('placeHolder' => 'Subtema', 'maxlengh' => 1000));
                        ?>

                        <div class="form-group">
                            <label class="control-label col-lg-2" for="">Data: </label>

                            <div class="col-lg-5">
                                <?PHP
                                    echo $simec->data('ptadtlancamento_ini', 'Início');
                                ?>
                            </div>

                            <div class="col-lg-5">
                                <?PHP
                                    echo $simec->data('ptadtlancamento_fim', 'Fim');
                                ?>
                            </div>
                        </div>

                        <?PHP
                            $sql = "
                                SELECT  ua.usucpf AS codigo,
                                        su.usunome AS descricao
                                FROM agendagm.usuarioassessor AS ua
                                JOIN seguranca.usuario AS su ON su.usucpf = ua.usucpf
                            ";
                            echo $simec->select( 'usucpf', 'Assessores', '', $sql, array('data-placeholder' => 'Selecione...') );

                            $sql = "
                                SELECT  1 AS codigo,
                                        'Com 3 dias para o atendimento' AS descricao
                                UNION
                                SELECT  2 AS codigo,
                                        'Menos de 3 dias para o atendimento' AS descricao
                                UNION
                                SELECT  3 AS codigo,
                                        'Em atraso para o atendimento' AS descricao
                                ORDER BY codigo
                            ";
                            echo $simec->select( 'trfprazo', 'Pendência em Atraso', '', $sql, array('data-placeholder' => 'Selecione...') );
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
                    <h5> Reuniões </h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                    </div>
                </div>
                <div class="ibox-content" style="background-color:#E0EEE0;">
                    <div>
                        <button type="button" class="btn btn-xs btn-warning btn-lg" onclick="novaAgendaPauta();">
                            <span class="glyphicon glyphicon-plus"></span> Nova Reunião
                        </button>
                    </div>
                    <div>
                        <?PHP
                            $modelPauta->listagemGridPautas( $_REQUEST );
                        ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>