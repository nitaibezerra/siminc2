<?PHP
    include_once APPRAIZ . "agendagm/classes/modelo/Tarefa.class.inc";
    include_once APPRAIZ . "agendagm/classes/modelo/TarefaUsuario.class.inc";
    include_once APPRAIZ . "agendagm/classes/modelo/ArquivoTarefa.class.inc";

    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    require_once APPRAIZ . "zimec/library/Zend/Filter/Input.php";
    require_once APPRAIZ . "zimec/library/Zend/Validate/StringLength.php";
    require_once APPRAIZ . "zimec/library/Zend/Validate/Db/NoRecordExists.php";
    
    #MENSAGENS PARA A INTERFACE DO USUÁRIO
    //require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
    //$fm = new Simec_Helper_FlashMessage('objetosfies/objetos');
    //echo $fm->getMensagens();

    $modelTarefa = new Agendagm_Model_Tarefa( $_REQUEST['trfid'] );
    $modelTarefaUsuario = new Agendagm_Model_TarefaUsuario();

    $modelArquivoTarefa = new Agendagm_Model_ArquivoTarefa();

    switch ($_REQUEST['action']) {
        #EXECUTA O DOWLOAD DO ARQUIVO ANEXADO DA TAREFA.
        case('downloadArquivoTarefa'):
            $resp = $modelArquivoTarefa->downloadArquivoTarefa( $_REQUEST['arqid'] );
            break;
        
        #EXCLUI O ARQUIVO ANEXO DA TAREFA
        case ('excluirArquivoTarefa'):
            $resp = $modelArquivoTarefa->excluirArquivoTarefa( $_REQUEST['arqid'] );
            $url .= '&trfid=' . $modelTarefa->trfid;

            if($resp){
                $db->commit();
                simec_redirecionar($url, 'success');
            }else{
                $db->rollback();
                simec_redirecionar($url, 'error');
            }
            break;
        
        #SALVA A TAREFA
        case('salvarDadosTarefa'):
            $modelTarefa->popularDadosObjeto();
            
            #TRATAMENTO CPF e ID PAUTA
            $modelTarefa->usucpf = substr($_SESSION['usucpf'], 0, 11);
            $campo_n_usado = array('ptaid');
            $result = $modelTarefa->salvar(NULL, NULL, $campo_n_usado);
            
            #usuarios
            if( $_REQUEST['usucpf'] ){
                $modelTarefaUsuario->trfid  = $modelTarefa->trfid;
                $modelTarefaUsuario->usucpf = $_REQUEST['usucpf'];
                $modelTarefaUsuario->salvar();
            }

            #arquivos
            if(is_array($_FILES['arquivos'])){
                $modelArquivoTarefa->trfid = $modelTarefa->trfid;
                $modelArquivoTarefa->anexarArquivoTarefa();
            }

            $url .= '&ptaid=' . $modelTarefa->ptaid . '&trfid='. $modelTarefa->trfid;
            if( $result > 0 ){
                $modelTarefa->commit();
                simec_redirecionar($url, 'success');
            }else{
                $modelTarefa->rollback();
                simec_redirecionar($url, 'error');
            }
        break;
    }

    #CHAMADA DE PROGRAMA
    include  APPRAIZ."includes/cabecalho.inc";

    //echo $simec->title('Bem-vindo');

    #CRIA ABAS
    $abacod_tela    = 58332;
    $url            = 'agendagm.php?modulo=principal/tarefa/cad_agenda_gm_tarefa&acao=A';
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
    .btn-file {
        position: relative;
        overflow: hidden;
    }
    .btn-file input[type=file] {
        position: absolute;
        top: 0;
        right: 0;
        min-width: 100%;
        min-height: 100%;
        font-size: 100px;
        text-align: right;
        filter: alpha(opacity=0);
        opacity: 0;
        background: red;
        cursor: inherit;
        display: block;
    }
    input[readonly] {
        background-color: white !important;
        cursor: text !important;
    }
    .nenhum-registro{
        margin-top:10px !important;
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
    $(document).ready(function () {
        $('.btn-file :file').on('fileselect', function(event, numFiles, label) {
            var input = $(this).parents('.input-group').find(':text'),
                log = numFiles > 1 ? numFiles + ' files selected' : label;

            if( input.length ) {
                input.val(log);
            } else {
                if( log ) alert(log);
            }
        });
    });

    $(document).on('change', '.btn-file :file', function() {
        var input = $(this),
        numFiles = input.get(0).files ? input.get(0).files.length : 1,
        label = input.val().replace(/\\/g, '/').replace(/.*\//, '');
        input.trigger('fileselect', [numFiles, label]);
    });

    function downloadArquivoTarefa( arqid ){
        $('#formulario_tarefa').find('#arqid').val(arqid);
        $('#formulario_tarefa').find('#action').val('downloadArquivoTarefa');
        $('#formulario_tarefa').submit();
    }

    function excluirArquivoTarefa( arqid ){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#formulario_tarefa').find('#action').val('excluirArquivoTarefa');
            $('#formulario_tarefa').find('#arqid').val(arqid);
            $('#formulario_tarefa').submit();
        }
    }

    function executarCores(){
        $('.glyphicon-pencil').removeClass('btn-sm');
        $('.glyphicon-pencil').addClass('btn-xs');

        $('.glyphicon-minus').removeClass('btn-sm');
        $('.glyphicon-minus').addClass('btn-xs');

        $('.glyphicon-trash').removeClass('btn-sm');
        $('.glyphicon-trash').addClass('btn-xs');

        $('.glyphicon-download-alt').removeClass('btn-sm');
        $('.glyphicon-download-alt').addClass('btn-xs');

        $('.nenhum-registro').removeClass('col-md-offset-4');
        $('.nenhum-registro').removeClass('col-md-4');
        $('.nenhum-registro').addClass('col-md-12');

        $('.navbar-listagem').hide();
    }

    function novaTarefaPauta(){
        $('#formulario_tarefa').find('#trfid').val('');
        $('#formulario_tarefa').find('#action').val('');
        $('#formulario_tarefa').submit();
    }
    
    function salvarDadosTarefa(){
        $('#formulario_tarefa').find('#action').val('salvarDadosTarefa');
        $('#formulario_tarefa').submit();
    }

</script>

<?PHP
    $simec->setFormTipo(Simec_View_Helper::K_FORM_TIPO_HORIZONTAL);
?>
<div class="wrapper wrapper-content animated fadeInRight" style="margin-top: -28px;">
    <div class="col-lg-12 col-md-12">
           <div class="ibox float-e-margins">
               <div class="ibox-title">
                   <h5>Gabinete do Ministro - Agenda</h5>
               </div>
           </div>
    </div>

    <div class="col-lg-12 col-md-12">
        <div  style="margin-top:-5px;">
            <div class="ibox float-e-margins">
                <div class="ibox-title" style="background-color:#E0EEE0; margin-bottom:1px; border-bottom-color:#e7eaec;">
                    <h5>Nova pendência</h5>
                    <div class="ibox-tools">
                        <a class="collapse-link">
                            <i class="fa fa-chevron-up"></i>
                        </a>
                    </div>
                </div>

                <div class="ibox-content" style="background-color:#E0EEE0;">
                    <fieldset id="formulario_tarefas">
                        <form method="post" id="formulario_tarefa" name="formulario_tarefa" enctype="multipart/form-data" class="form form-horizontal">
                            <input id="action" name="action" type="hidden" value="">
                            <input id="arqid" name="arqid" type="hidden" value="">
                            <input id="ptaid" name="ptaid" type="hidden" value="<?=$modelPauta->ptaid;?>">
                            <input id="trfid" name="trfid" type="hidden" value="<?=$modelTarefa->trfid;?>">
                            <?PHP
                                echo $simec->input('trfdsc', 'Descrição', $modelTarefa->trfdsc, array('required', 'placeHolder' => 'Descrição', 'maxlengh' => 1000));

                                $sql_usu = "
                                    SELECT  u.usucpf AS codigo,
                                            u.usunome AS descricao

                                    FROM seguranca.usuario AS u
                                    LEFT JOIN seguranca.perfilusuario AS p ON p.usucpf = u.usucpf

                                    WHERE p.pflcod = 1343
                                ";
                                echo $simec->select( 'usucpf[]', 'Responsável', $modelTarefaUsuario->recuperarPorTarefa($modelTarefa->trfid), $sql_usu, array('required', 'data-placeholder' => 'Selecione...') );

                                echo $simec->data('trfprazo', 'Prazo', $modelTarefa->trfprazo, array('required') );

                                $sql_uam = "
                                    SELECT  uamid AS codigo,
                                            UPPER(uamdsc) AS descricao
                                    FROM public.unidadeareamec
                                    WHERE uamstatus = 'A'
                                    ORDER BY descricao
                                ";
                                echo $simec->select( 'uamid', 'Unidade MEC', $modelTarefa->uamid, $sql_uam, array('required', 'data-placeholder' => 'Selecione...') );
                            ?>

                            <div class="form-group">
                                <label class="control-label col-lg-2" for="">Anexar Arquivos:</label>
                                <div class="col-lg-10">
                                    <div class="input-group">
                                        <span class="input-group-btn">
                                            <span class="btn btn-primary btn-file">
                                                Buscar Arquivo <input type="file" id="arquivos" name="arquivos[]" multiple>
                                            </span>
                                        </span>
                                        <input type="text" class="form-control" readonly>
                                    </div>
                                </div>
                            </div>
                            
                            <div class="form-group">
                                <div class="col-lg-6 text-right">
                                    <button type="button" class="btn btn-sm btn-success btn-lg" onclick="salvarDadosTarefa();">
                                        &nbsp; <span class="glyphicon glyphicon-floppy-disk"> </span> Salvar &nbsp;
                                    </button>
                                </div>
                                <div class="col-lg-6">
                                    <button type="button" class="btn btn-sm btn-warning btn-lg" onclick="novaTarefaPauta();">
                                        <span class="glyphicon glyphicon-refresh"></span> Nova Tarefa
                                    </button>
                                </div>
                            </div>

                            <div class="clearfix"></div>
                            <br>
                            <legend style="border-bottom-color:#BEBEBE;"> <h4 style="color:#676a6c;">Anexos</h4> </legend>
                            <div class="form-group" style="margin-top:-8px;">
                                <div class="col-sm-12 col-md-12 col-lg-12">

                                    <div style="margin-left:1px;">
                                        <?PHP
                                            #GERA LISTAGEM DOS ANEXOS DAS TAREFAS
                                            if( $modelTarefa->trfid != '' ){
                                                $WHERE = "WHERE t.trfid = {$modelTarefa->trfid}";
                                            }else{
                                                $WHERE = "WHERE 1<>1";
                                            }

                                            $sql = "
                                                SELECT  t.arqid,
                                                        a.arqnome||'.'||arqextensao AS arquivo,
                                                        u.usunome

                                                FROM agendagm.arquivotarefa  AS t

                                                JOIN public.arquivo AS a ON a.arqid = t.arqid
                                                JOIN seguranca.usuario AS u ON u.usucpf = a.usucpf
                                                {$WHERE}
                                            ";
                                            $cabecalho = array("Arquivo", "Responsável");

                                            $listagem = new Simec_Listagem();
                                            $listagem->setCabecalho($cabecalho);
                                            $listagem->setQuery($sql);
                                            $listagem->addAcao('download', 'downloadArquivoTarefa');
                                            $listagem->addAcao('delete', 'excluirArquivoTarefa');
                                            $listagem->turnOffForm();
                                            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
                                        ?>
                                    </div>
                                </div>
                            </div>                            
                        </form>
                    </fieldset>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    executarCores();
</script>
