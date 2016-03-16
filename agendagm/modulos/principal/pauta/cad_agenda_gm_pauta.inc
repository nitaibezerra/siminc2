<?PHP
    include_once APPRAIZ . "agendagm/classes/modelo/Pauta.class.inc";
    include_once APPRAIZ . "agendagm/classes/modelo/UsuarioAssessor.class.inc";

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

    $modelPauta = new Agendagm_Model_Pauta( $_REQUEST['ptaid'] );
    $modelUsuarioAssessor = new Agendagm_Model_UsuarioAssessor();

    $modelTarefa = new Agendagm_Model_Tarefa( $_REQUEST['trfid'] );
    $modelTarefaUsuario = new Agendagm_Model_TarefaUsuario();

    $modelArquivoTarefa = new Agendagm_Model_ArquivoTarefa();

    #"CONTROLE"
    switch ($_REQUEST['action']) {
        #EXECUTA O DOWLOAD DO ARQUIVO ANEXADO DA TAREFA.
        case('downloadArquivoTarefa'):
            $resp = $modelArquivoTarefa->downloadArquivoTarefa( $_REQUEST['arqid'] );
            break;

        #EXCLUI O ANEXO DA PAUTA
        case ('excluirAnexoPauta'):
            $modelPauta->excluirAnexoPauta( $_REQUEST['ptaid'] );
            break;

        #EXCLUI O ARQUIVO ANEXO DA TAREFA
        case ('excluirArquivoTarefa'):
            $resp = $modelArquivoTarefa->excluirArquivoTarefa( $_REQUEST['arqid'] );
            $url .= '&ptaid=' . $modelPauta->ptaid;

            if($resp){
                $db->commit();
                simec_redirecionar($url, 'success');
            }else{
                $db->rollback();
                simec_redirecionar($url, 'error');
            }
            break;

        #EXCLUI A TAREFA VINCULADA A PAUTA
        case ('excluirTarefaPauta'):
            $modelTarefa->excluirTarefaPauta( $_REQUEST['trfid'] );
            break;

        #SALVA A PAUTA
        case('salvar_pauta'):
            $modelPauta->popularDadosObjeto();

            #TRATAMENTO CPF - USUÁRIO QUE ESTA INSERINDO O SISTEMA
            $modelPauta->usucpf = substr($_SESSION['usucpf'], 0, 11);

            $result = $modelPauta->salvar();

            #SALVAR USUARIO ASSESSORIS.
            if( $_REQUEST['usucpf'] ){
                $modelUsuarioAssessor->ptaid  = $modelPauta->ptaid;
                $modelUsuarioAssessor->usucpf = $_REQUEST['usucpf'];

                $modelUsuarioAssessor->salvar();
            }

            #ANEXAR ARQUIVOS
            if(is_array( $_FILES['arquivos'] ) ){
                $modelPauta->arqid = $_FILES;
                $modelPauta->anexarArquivoPauta();
            }

            $url .= '&ptaid=' . $modelPauta->ptaid;
            if($result){
                $modelPauta->commit();
                simec_redirecionar($url, 'success');
            }else{
                $modelPauta->rollback();
                simec_redirecionar($url, 'error');
            }
        break;

        #SALVA A TAREFA
        case('salvar_tarefa'):
            $modelTarefa->popularDadosObjeto();

            #TRATAMENTO CPF
            $modelTarefa->usucpf = substr($_SESSION['usucpf'], 0, 11);
            $result = $modelTarefa->salvar();

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

        #HABILITA A TELA OS "FORMULÁRIOS DE ACORDO COM AS REGRAS"
        case('habilitaFormTela'):
            $modelPauta->habilitaFormTela( $_REQUEST );
        break;
    }

    #CHAMADA DE PROGRAMA
    include  APPRAIZ."includes/cabecalho.inc";
    //echo $simec->title('Bem-vindo');

    #CRIA ABAS
    $abacod_tela    = 58332;
    $url            = 'agendagm.php?modulo=principal/pauta/cad_agenda_gm_pauta&acao=A';
    $parametros     = '';
    $db->cria_aba($abacod_tela, $url, $parametros);

    #EXECUTA A CONFIGURAÇÃO DO FORMULÁRIO "OBRIGATÓRIO" - DEFININDO CLASS DO COMPONENTES EM BOOTSTRAP NA MONTAGEM DOS ELEMENTOS DO FORM
    $simec->setFormTipo(Simec_View_Helper::K_FORM_TIPO_VERTICAL);
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

<link rel="stylesheet" type="text/css" href="/library/fancybox/jquery.fancybox.css" media="screen"/>

<script type="text/javascript" src="/library/fancybox/jquery.fancybox.js"></script>

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

    function editaTarefaPauta( trfid ){
        $('#formulario_tarefa').find('#trfid').val(trfid);
        $('#formulario_tarefa').find('#action').val('');
        $('#formulario_tarefa').submit();
    }

    function excluirTarefaPauta( trfid ){
        var confirma = confirm("Deseja realmente excluir a Tarefa?");
        if( confirma ){
            $('#formulario_tarefa').find('#action').val('excluirTarefaPauta');
            $('#formulario_tarefa').find('#trfid').val(trfid);
            $('#formulario_tarefa').submit();
        }
    }

    function excluirAnexoPauta( arqid ){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#formulario_tarefa').find('#action').val('excluirAnexoPauta');
            $('#formulario_tarefa').find('#arqid').val(arqid);
            $('#formulario_tarefa').submit();
        }
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

    function habilitaFormTela(){
        var ptaid = $('#ptaid').val();

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "action=habilitaFormTela&ptaid="+ptaid,
            async   : false,
            success: function(resp){
                if( resp === 'true' ){
                    $('#fild_formulario_pauta_2').attr('disabled', false);
                    $('#fild_formulario_ata').attr('disabled', false);
                    $('#fild_formulario_tarefas').attr('disabled', false);
                }
            }
        });
    }
    
    function salvarDados( opc ){
        switch(opc) {
            case 'P1':
                $('#formulario_pauta_1').find('#action').val('salvar_pauta');
                $('#formulario_pauta_1').submit();
            break;
            case 'P2':
                $('#formulario_pauta_2').find('#action').val('salvar_pauta');
                $('#formulario_pauta_2').submit();
            break;
            case 'P3':
                $('#formulario_pauta_3').find('#action').val('salvar_pauta');
                $('#formulario_pauta_3').submit();
            break;
            case 'T':
                $('#formulario_tarefa').find('#action').val('salvar_tarefa');
                $('#formulario_tarefa').submit();
            break;
        }
    }

    function novaTarefaPauta(){
        $('#formulario_tarefa').find('#trfid').val('');
        $('#formulario_tarefa').find('#action').val('');
        $('#formulario_tarefa').submit();
    }

</script>

<div class="wrapper wrapper-content animated fadeInRight" style="margin-top: -28px;">
    <div class="row">
        <div class="col-lg-12 col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Gabinete do Ministro - Agenda</h5>
                </div>
            </div>
        </div>

        <div class="col-lg-6 col-md-6">
            <!-- REGISTRO PREVIO - DATA DO LANÇAMENTO/TEMA/SUBTEMAPARTICIPANTES/ASSESSORES #F0FFF0-->
            <div style="margin-top:-10px;">
                <div class="ibox float-e-margins">
                    <div class="ibox-title" style="background-color:#F0FFF0;">
                        <h5>Registro Prévio 1</h5>
                        <div class="ibox-tools">
                            <a class="collapse-link">
                                <i class="fa fa-chevron-up"></i>
                            </a>
                        </div>
                    </div>

                    <div class="ibox-content" style="background-color:#F0FFF0;">
                        <fieldset id="fild_formulario_pauta_1">
                            <form method="post" name="formulario_pauta_1" id="formulario_pauta_1">
                                <input name="action" id="action" type="hidden" value="">
                                <input id="ptaid" name="ptaid" type="hidden" value="<?=$modelPauta->ptaid;?>">

                                <?PHP
                                    echo $simec->data('ptadtlancamento', 'Data', $modelPauta->ptadtlancamento, array('required'));

                                    echo $simec->textarea('ptatema', 'Tema', $modelPauta->ptatema, array('required', 'rows'=>'3', 'placeHolder' => 'Tema', 'maxlengh' => 1000) );

                                    echo $simec->textarea('ptasubtema', 'Subtema', $modelPauta->ptasubtema, array('required','rows'=>'3','placeHolder' => 'Subtema', 'maxlengh' => 1000));

                                    echo $simec->textarea('ptaparticipantes', 'Participantes', $modelPauta->ptaparticipantes, array('required', 'rows'=>'9', 'placeHolder' => 'Participantes', 'maxlengh' => 1000));

                                    $sql = "
                                        SELECT  u.usucpf AS codigo,
                                                u.usunome AS descricao
                                        FROM seguranca.usuario AS u
                                        LEFT JOIN seguranca.perfilusuario AS p ON p.usucpf = u.usucpf
                                        WHERE p.pflcod = 1343
                                    ";
                                    echo $simec->select( 'usucpf[]', 'Assessores', $modelUsuarioAssessor->recuperarPorPauta($modelPauta->ptaid), $sql, array('required', 'data-placeholder' => 'Selecione...') );

                                ?>

                                <div class="form-group">
                                    <div class="col-lg-6 text-right">
                                        <button type="button" class="btn btn-sm btn-success btn-lg" onclick="salvarDados('P1');">
                                            &nbsp; <span class="glyphicon glyphicon-floppy-disk"> </span> Salvar Dados &nbsp;
                                        </button>
                                    </div>
                                    <div class="col-lg-6">
                                        <button type="button" class="btn btn-sm btn-danger btn-lg">
                                            <span class="glyphicon glyphicon-share"></span> Cancelar
                                        </button>
                                    </div>
                                </div>

                            </form>
                        </fieldset>
                        <div class="clearfix"></div>

                    </div>
                </div>
            </div>

            <!-- REGISTRO PREVIO - JUSTIFICATIVA/ANTECEDENTES -->
            <div style="margin-top:-5px;">
                <div class="ibox float-e-margins">
                    <div class="ibox-title" style="background-color:#F0FFF0;">
                        <h5>Registro Prévio 2</h5>
                        <div class="ibox-tools">
                            <a class="collapse-link">
                                <i class="fa fa-chevron-up"></i>
                            </a>
                        </div>
                    </div>

                    <div class="ibox-content" style="background-color:#F0FFF0;">
                        <fieldset id="fild_formulario_pauta_2" disabled>
                            <form method="post" name="formulario_pauta_2" id="formulario_pauta_2">
                                <input name="action" id="action" type="hidden" value="">
                                <input name="ptaid" type="hidden" value="<?=$modelPauta->ptaid;?>">

                                <div class="form-group">
                                    <div class=" alert alert-success bg-primary">
                                        <label class="control-label" style="padding-bottom:10px; font-size:14px; height: "> Justificativa </label>

                                        <p> <span class="glyphicon glyphicon-asterisk"> </span> <em style="font-size:12px;"> Motivo da reunião. Qual a solicitação/reclamação/colocação/proposta a ser apresentada? </em> </p>
                                        <br> <br>
                                        <label class="control-label" style="padding-bottom:10px; font-size:14px;"> Antecedentes </label>

                                        <p> <span class="glyphicon glyphicon-asterisk"> </span> <em style="font-size:12px;"> Caracterização da Organização  </em> </p>
                                        <p> <span class="glyphicon glyphicon-asterisk"> </span> <em style="font-size:12px;"> Resposta do MEC à solicitação/proposta </em> </p>
                                        <p> <span class="glyphicon glyphicon-asterisk"> </span> <em style="font-size:12px;"> Questões adicionais </em> </p>
                                        <p> <span class="glyphicon glyphicon-asterisk"> </span> <em style="font-size:12px;"> Levantamentos prévios a respeito da figura/instituição/ideia a ser recebida/tratada na reunião. Posição da mídia e dos principais atores envolvidos. Identificar as respostas prévias do MEC e o posicionamento atual em atendimento à solicitação/reclamação/colocação/proposta a ser recebida.  </em> </p>

                                    </div>
                                </div>

                                <?PHP

                                    echo $simec->textarea('ptajustificativa', 'Justificativa', $modelPauta->ptajustificativa, array('required', 'rows'=>'9', 'placeHolder' => 'Justificativa', 'maxlengh' => 1000));

                                    echo $simec->textarea('ptaantecedentes', 'Antecedentes', $modelPauta->ptaantecedentes, array('required', 'rows'=>'9', 'placeHolder' => 'Antecedentes', 'maxlengh' => 1000));

                                ?>

                                <div class="form-group">
                                    <div class="col-lg-6 text-right">
                                        <button type="button" class="btn btn-sm btn-success btn-lg" onclick="salvarDados('P2');">
                                            &nbsp; <span class="glyphicon glyphicon-floppy-disk"> </span> Salvar Dados &nbsp;
                                        </button>
                                    </div>
                                    <div class="col-lg-6">
                                        <button type="button" class="btn btn-sm btn-danger btn-lg">
                                            <span class="glyphicon glyphicon-share"></span> Cancelar
                                        </button>
                                    </div>
                                </div>

                            </form>
                        </fieldset>

                        <div class="clearfix"></div>

                    </div>
                </div>
            </div>
        </div>

        <div class="col-lg-6 col-md-6">
            <!-- ATA - ATA/ORIENTAÇÕES DO MINISTRO/OBSERVAÇÕES -->
            <div  style="margin-top:-10px;">
                <div class="ibox float-e-margins">
                    <div class="ibox-title" style="background-color:#F0FFFF;">
                        <h5>Nova Reunião/Ata 1</h5>
                        <div class="ibox-tools">
                            <a class="collapse-link">
                                <i class="fa fa-chevron-up"></i>
                            </a>
                        </div>
                    </div>

                    <div class="ibox-content" style="background-color:#F0FFFF;">
                        <fieldset id="fild_formulario_ata" disabled>
                            <form method="post" name="formulario_pauta_3" id="formulario_pauta_3" enctype="multipart/form-data">
                                <input name="action" id="action" type="hidden" value="">
                                <input name="ptaid" type="hidden" value="<?=$modelPauta->ptaid;?>">

                                <?PHP
                                    echo $simec->textarea('ptadscata', 'Registro', $modelPauta->ptadscata, array('required', 'class'=>'summernote', 'placeHolder' => 'Ata', 'maxlengh' => 1000));

                                    $disabled = $modelPauta->verificaAxenoHabilita( $modelPauta->ptaid );
                                ?>
                                <fieldset <?=$disabled;?>>
                                    <div class="form-group">
                                        <label class="control-label" for="">Anexar Arquivo: </label>
                                        <div class="">
                                            <div class="input-group">
                                                <span class="input-group-btn">
                                                    <span class="btn btn-primary btn-file">
                                                        Buscar Arquivo <input type="file" id="arquivos" name="arquivos">
                                                    </span>
                                                </span>
                                                <input type="text" class="form-control" readonly>
                                            </div>
                                        </div>
                                    </div>
                                </fieldset>
                                <div class="clearfix"></div>

                                <legend style="margin-top:20px; border-bottom-color:#BEBEBE;"><h4 style="color:#676a6c;">Anexo</h4></legend>
                                <div class="form-group" style="margin-top:20px;">
                                    <div class="col-sm-12 col-md-12 col-lg-12">
                                        <div>
                                            <?PHP
                                                $modelPauta->listaAnexoPauta( $modelPauta->ptaid );
                                            ?>
                                        </div>
                                    </div>
                                </div>

                                <div class="clearfix"></div> <br>

                                <?PHP
                                    echo $simec->textarea('ptaorientaministro', 'Orientação do Ministro', $modelPauta->ptaorientaministro, array('required', 'class'=>'summernote', 'placeHolder' => 'Orientação', 'maxlengh' => 200));

                                    echo $simec->textarea('ptaobservacao', 'Observações', $modelPauta->ptaobservacao, array('required', 'class'=>'summernote', 'placeHolder' => 'Observação', 'maxlengh' => 1000));
                                ?>

                                <div class="form-group">

                                    <div class="col-lg-6 text-right">
                                        <button type="button" class="btn btn-sm btn-success btn-lg" onclick="salvarDados('P3');">
                                            &nbsp; <span class="glyphicon glyphicon-floppy-disk"> </span> Salvar Dados &nbsp;
                                        </button>
                                    </div>
                                    <div class="col-lg-6">
                                        <button type="button" class="btn btn-sm btn-danger btn-lg">
                                            <span class="glyphicon glyphicon-share"></span> Cancelar
                                        </button>
                                    </div>
                                </div>

                            </form>
                        </fieldset>

                        <div class="clearfix"></div>
                    </div>
                </div>
            </div>

            <!-- ENCAMINHAMENTOS - DESCRIÇÃO/RESPOSÁVEL/PRAZO/ANEXO-->
            <div  style="margin-top:-5px;">
                <div class="ibox float-e-margins">
                    <div class="ibox-title" style="background-color:#E0EEE0; margin-bottom:1px; border-bottom-color:#e7eaec;">
                        <h5>Nova Reunião/Enacaminhamentos</h5>
                        <div class="ibox-tools">
                            <a class="collapse-link">
                                <i class="fa fa-chevron-up"></i>
                            </a>
                        </div>
                    </div>

                    <div class="ibox-content" style="background-color:#E0EEE0;">
                        <legend style="border-bottom-color:#BEBEBE;"><h4 style="color:#676a6c;">Pendência</h4></legend>
                        <fieldset id="fild_formulario_tarefas" disabled>
                            <form method="post" id="formulario_tarefa" name="formulario_tarefa" enctype="multipart/form-data">
                                <input name="action" id="action" type="hidden" value="">
                                <input id="requisicao" name="requisicao" type="hidden" value="">
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
                                    <label class="control-label" for="">Anexar Arquivos:</label>
                                    <div class="">
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

                                <legend style="border-bottom-color:#BEBEBE;"><h4 style="color:#676a6c;">Anexos</h4></legend>
                                <div class="form-group" style="margin-top:-8px;">
                                    <div class="col-sm-12 col-md-12 col-lg-12">

                                        <div style="margin-left:1px;">
                                            <?PHP
                                                $modelTarefa->listagemAnexosTarefas( $modelTarefa->trfid );
                                            ?>
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <div class="col-lg-6 text-right">
                                        <button type="button" class="btn btn-sm btn-success btn-lg" onclick="salvarDados('T');">
                                            &nbsp; <span class="glyphicon glyphicon-floppy-disk"> </span> Salvar Dados &nbsp;
                                        </button>
                                    </div>
                                    <div class="col-lg-6">
                                        <button type="button" class="btn btn-sm btn-warning btn-lg" onclick="novaTarefaPauta();">
                                            <span class="glyphicon glyphicon-refresh"></span> Nova Tarefa
                                        </button>
                                    </div>
                                </div>
                            </form>
                        </fieldset>

                        <div class="clearfix"></div> <br>

                        <legend style="margin-top:20px; border-bottom-color:#BEBEBE;"><h4 style="color:#676a6c;">Pendência da Reunião</h4></legend>
                        <div class="form-group" style="margin-top:20px;">
                            <div class="col-sm-12 col-md-12 col-lg-12">
                                <div>
                                    <?PHP
                                        $modelTarefa->listagemTarefasPautas( $modelPauta->ptaid );
                                    ?>
                                </div>
                            </div>
                        </div>
                        <div class="clearfix"></div>

                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script type="text/javascript">
    executarCores();
    habilitaFormTela();
</script>
