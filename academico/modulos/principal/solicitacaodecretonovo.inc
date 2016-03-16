<?php

$_SESSION['academico']['sbsid'] = $_REQUEST['sbsid'];

//enviarEmailEstadoDocumentoConjur();

require_once APPRAIZ . 'includes/workflow.php';

if ($_SESSION['baselogin'] == "simec_desenvolvimento") {
    define("TPD_BENSSERVICOS", 70);
    define("ENT_SAA", "742767");
} else {
    define("TPD_BENSSERVICOS", 71);
    define("ENT_SAA", "742798");
}

if($_REQUEST['requisicao']) {
   $_REQUEST['requisicao']($_REQUEST, $_FILES);
   exit;
}

if($_REQUEST['requisicaoAjax']){
    header('content-type: text/html; charset=ISO-8859-1');
    $_REQUEST['requisicaoAjax']();
    exit;
}

# - anexarDocumentos: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
function anexarDocumentos($dados, $files, $sbsid, $tadid) {
    global $db;

    include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';

    $sbsid          = $sbsid;
    //$tadid          = 1; #O TIPO DE ARQUIVO, POR ENGUANTO É FIXO EM "1". ATÉ O MOMENTO NÃO NECESSIDADE, EXISTE APENAS O TIPO "1".
    $usucpf         = $_SESSION['usucpf'];
    $aqddtinclusao  = "'".gmdate('Y-m-d')."'";

    $campos = array(
        'sbsid'         => $sbsid,
        'tadid'         => $tadid,
        'usucpf'        => "'".$usucpf."'",
        //'usucpf'        => $usucpf,
        'aqdstatus'     => "'A'",
        'aqddtinclusao' => $aqddtinclusao
    );

    $file = new FilesSimec('arquivodecreto', $campos, 'academico');

    if ( $files ) {
        $arquivoSalvo = $file->setUpload('Rede Federal - Solicitação de Descreto');
        if ($arquivoSalvo) {
            return 'S';
        }else{
            return 'N';
        }
    }
    exit;
}

# - dowloadDocAnexo: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
function dowloadDocAnexo( $dados ){

    $arqid = $dados['arqid'];

    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if ( $arqid ){
        $file = new FilesSimec("arquivodecreto", $campos, "academico");
        $file->getDownloadArquivo( $arqid );
    }
}

# - excluirDocAnexo: TELA SOLICITAÇÃO DE DECRETO (DECRETO 7689) - ANEXAR DOCUMENTOS.
function excluirDocAnexo2( $dados ) {
   global $db;

   $arqid = $dados['arqid'];
   $sbsid = $dados['sbsid'];

   include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

   if ($arqid != '') {
       $sql = " UPDATE academico.arquivodecreto SET aqdstatus = 'I' WHERE arqid = {$arqid} ";
   }

   if( $db->executar($sql) ){
       $file = new FilesSimec("arquivodecreto", $campos, "academico");
       $file->excluiArquivoFisico( $arqid );

       $db->commit();
       $db->sucesso('principal/solicitacaodecretonovo', '&acao=A&sbsid='.$sbsid);
   }
}

function comboModalidades($tpcid = null) {
    global $db;
    $tpcid = !$tpcid ? $_POST['tpcid'] : $tpcid;
    $sql = "
        select mdl.mdlid  as codigo,
               mdl.mdldsc as descricao
        from academico.modalidadelicitacao    mdl
        where mdl.mdlstatus = 'A'
        order by mdl.mdldsc
    ";
    $db->monta_combo('mdlid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'mdlid');
}

function campoValorTag($tpcid = null) {
    $tpcid = !$tpcid ? $_POST['tpcid'] : $tpcid;
    switch ($tpcid){
        case 1:
            echo 'Valor Anual R$:';
            break;
        case 2:
            echo 'Valor Mensal R$:';
            break;
        default:
            echo 'Valor R$:';
            break;
    }
}

function campoJustificativaTag($tpcid = null) {
    $tpcid = !$tpcid ? $_POST['tpcid'] : $tpcid;
    switch ($tpcid){
        case 1:
            echo 'Justificativa para a necessidade de Contratação:';
            break;
        case 2:
            echo 'Justificativa para a necessidade de Locação:';
            break;
        default:
            echo 'Justificativa:';
            break;
    }
}

function inserirSolitacaoDecreto($dados, $files) {
    global $db;

    $sbsdtiniciovigencia = ajusta_data($dados['sbsdtiniciovigencia']);
    $sbsdtfimvigencia    = ajusta_data($dados['sbsdtfimvigencia']);
    $sbsvalor            = str_replace(array(".",","),array("","."),$dados['sbsvalor']);

    if($_FILES['arquivoDecreto']['name'] == ''){
        $db->insucesso('O arquivo é obrigatório! Não foi possível realizar a operação.', '', 'principal/solicitacaodecretonovo&acao=A');
        exit();
    }

    $sql = "
        INSERT INTO academico.solicitacaobensservicos(
                tpcid,
                epcid,
                mdlid,
                sbsdsc,
                sbsjustificativa,
                sbsnumprocesso,
                sbsobjeto,
                sbsdtiniciovigencia,
                sbsdtfimvigencia,
                usucpfinclusao,
                sbsdtinclusao,
                sbsvalor,
                entid
            )VALUES(
                {$dados['tpcid']},
                {$dados['epcid']},
                {$dados['mdlid']},
                '{$dados['sbsdsc']}',
                '{$dados['sbsjustificativa']}',
                '{$dados['sbsnumprocesso']}',
                '{$dados['sbsobjeto']}',
                '{$sbsdtiniciovigencia}',
                '{$sbsdtfimvigencia}',
                '{$_SESSION['usucpf']}',
                'now()',
                '{$sbsvalor}',
                {$_SESSION['academico']['entid']}
        ) RETURNING sbsid;
    ";

    $sbsid = $db->pegaUm($sql);
    $docid = criarDocumentoSBS($sbsid,TPD_BENSSERVICOS);
    $returnParams = array();
    $result = anexarDocumentos($_REQUEST, $_FILES, $sbsid, $tadid = 2);

    if ($result == 'S') {
        $returnParams['urlGet'] = '&sbsid='.$sbsid;
        $returnParams['message'] = 'Dados inserido e Documentos Anexados com sucesso!';
        $db->commit();
        //alertaPerfilEmailSBS($sbsid);
    } else {
        $returnParams['urlGet'] = '&sbsid='.$sbsid;
        $returnParams['message'] = 'Os dados não foram inseridos por que houve falha no upload do documento anexado, por favor tente novamente, se o problema persistir, contate a equipe técnica!';
        $db->rollback();
    }

//    $arDados = array('sbsid' => $sbsid);
//    switch ($_SESSION['academico']['orgid']) {
//        case 1:
//            if(wf_alterarEstado($docid, WF_AEDID_ENVIAR_ANALISE_SESU, '', $arDados)) {
//                alertaPerfilEmailSBS($sbsid);
//            }
//            break;
//        case 2:
//            if (wf_alterarEstado($docid, WF_AEDID_ENVIAR_ANALISE_SETEC, '', $arDados)) {
//                alertaPerfilEmailSBS($sbsid);
//            }
//            break;
//        case 3:
//            wf_alterarEstado($docid, WF_AEDID_ENVIAR_ANALISE_CONJUR, '', $arDados);
//            break;
//    }

    $db->sucesso('principal/solicitacaodecretonovo', $returnParams['urlGet'], $returnParams['message']);
}

function atualizarSolitacaoDecreto($dados, $files) {
    global $db;

    $sql = "
        UPDATE academico.solicitacaobensservicos SET
        epcid               = '".$dados['epcid']."',
        mdlid               = '".$dados['mdlid']."',
        tpcid               = '".$dados['tpcid']."',
        sbsjustificativa    = '".$dados['sbsjustificativa']."',
        sbsnumprocesso      = '".$dados['sbsnumprocesso']."',
        sbsobjeto	    = '".$dados['sbsobjeto']."',
        sbsdtiniciovigencia = '".ajusta_data($dados['sbsdtiniciovigencia'])."',
        sbsdtfimvigencia    = '".ajusta_data($dados['sbsdtfimvigencia'])."',
        usucpfalteracao     = '".$_SESSION['usucpf']."',
        sbsdtalteracao      = 'now()',
        sbsvalor            = '".str_replace(array(".",","),array("","."),$dados['sbsvalor'])."'
        WHERE sbsid         = '".$dados['sbsid']."' RETURNING sbsid;
    ";
    $sbsid = $db->pegaUm($sql);

    if ($_FILES['arquivoDecreto']['name'] != '') {

        $tadid = ($_REQUEST['tadid']) ? $_REQUEST['tadid'] : 2;

        $result = anexarDocumentos($_REQUEST, $_FILES, $sbsid, $tadid);

        if ($result == 'S') {
            $returnParams['urlGet'] = '&sbsid='.$sbsid;
            $returnParams['message'] = 'Dados inserido e Documentos Anexados com sucesso!';
            $db->commit();
        } else {
            $returnParams['urlGet'] = '&sbsid='.$sbsid;
            $returnParams['message'] = 'Os dados não foram inseridos por que houve falha no upload do documento anexado, por favor tente novamente, se o problema persistir, contate a equipe técnica!';
            $db->rollback();
        }
    } else {
        $returnParams['urlGet'] = '&sbsid='.$sbsid;
        $returnParams['message'] = 'Os dados não foram inseridos por que houve falha no upload do documento anexado, por favor tente novamente, se o problema persistir, contate a equipe técnica!';
        $db->rollback();
    }

    $db->sucesso('principal/solicitacaodecretonovo', $returnParams['urlGet'], $returnParams['message']);
}

function pegarMomento() {
    global $db;
    $sql = "SELECT slfid FROM academico.sldfase WHERE slfdatainicio<='".date("Y-m-d")."' AND slfdatafim>='".date("Y-m-d")."'";
    $slfid = $db->pegaUm($sql);
    return $slfid;
}

function excluirSolicitacaoDecreto($dados, $files) {
    global $db;

    $sql = "
        UPDATE academico.solicitacaobensservicos
            SET sbsstatus = 'I'
        WHERE sbsid = '".$dados['sbsid']."'
    ";

    $db->executar($sql);
    $db->commit();

    echo "<script>alert('Removido com sucesso');window.location='academico.php?modulo=principal/solicitacaodecretonovo&acao=A';</script>";

}

function permissaoAlterar() {
    global $db;

    $arrPerfil = pegaPerfilGeral();
    if( in_array(PERFIL_SUPERUSUARIO,$arrPerfil)  ||
        in_array(PERFIL_ALTA_GESTAO,$arrPerfil) ||
        in_array(PERFIL_REDE_FEDERAL_SECR_EXECUTIVO,$arrPerfil) ||
        in_array(PERFIL_REDE_FEDERAL_MINISTRO,$arrPerfil) ||
        in_array(PERFIL_ADMINISTRADOR,$arrPerfil) ||
        in_array(PERFIL_REITOR,$arrPerfil)        ||
        in_array(PERFIL_IFESCADBOLSAS,$arrPerfil) ||
        in_array(PERFIL_IFESCADCURSOS,$arrPerfil) ||
        in_array(PERFIL_IFESCADASTRO,$arrPerfil)  ||
        in_array(PERFIL_CADASTROGERAL,$arrPerfil) ||
        in_array(PERFIL_PROREITOR,$arrPerfil) ||
        in_array(PERFIL_INTERLOCUTOR_INSTITUTO,$arrPerfil)){
            return true;
    }else{
        return false;
    }

}

function listaJustificativas(){
    global $db;

    $sql = "
        SELECT  slfdsc,
                sljdsc,
                to_char(sljdataatualizacao, 'DD/MM/YYYY')
        FROM academico.sldfasejustificativa slj

        INNER JOIN academico.sldfase slf ON slf.slfid = slj.slfid

        WHERE sljstatus = 'A'
    ";

    if($permissoes){
        $cabecalho = array("Ações", "Momento", "Justificativa", "Ultima atualização");
    }else{
        $cabecalho = array("Momento", "Justificativa", "Ultima atualização");
    }
    return $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$_SESSION['academico']['entidadenivel'] = 'unidade';

$perfilNotBloq = array( PERFIL_CADASTROGERAL, PERFIL_MECCADBOLSAS, PERFIL_MECCADCURSOS, PERFIL_MECCADASTRO, PERFIL_ADMINISTRADOR, PERFIL_REITOR, PERFIL_ALTA_GESTAO, PERFIL_ASSESSORIA_ALTA_GESTAO, PERFIL_PROREITOR, PERFIL_INTERLOCUTOR_INSTITUTO );
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança

if(!$_SESSION['academico']['entid']) {
    die("
        <script>
            alert('Entidade não encontrada. Navegue novamente.');
            window.location='academico.php?modulo=inicio&acao=C';
        </script>"
    );
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br>';

$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57334' : '57335';

#$db->cria_aba($abacod_tela,$url,$parametros);
#academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Solicitação de Autorização - Decreto 7.689/2012');

$permissaoAlterar = permissaoAlterar();

?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8.9/themes/base/jquery-ui.css"/>
<script>
    function validarFormulario() {
        if($('#tpcid').val() == '') {
            alert('Tipo de contrato obrigatório.');
            return false;
        }
        if($('#epcid').val() == '') {
            alert('Espécie da contratação obrigatória.');
            return false;
        }
        if($('#mdlid').val() == '') {
            alert('Modalidade da licitação obrigatória.');
            return false;
        }
        if($('#sbsnumprocesso').val() == '') {
            alert('Nº do processo obrigatório.');
            return false;
        }
        if(document.getElementById('sbsnumprocesso').value.length!=20) {
            alert('Nº do processo deve ter 20 caracteres');
            return false;
        }

        if($('#sbsobjeto').val() == '') {
            alert('Objeto do contrato obrigatório');
            return false;
        }

        if($('#sbsvalor').val() == '') {
            alert('Valor obrigatório.');
            return false;
        }else{
            var valor = $('#sbsvalor').val().replace(/\./g, '').replace(/\,/g, '');

            switch($('#tpcid').val()){
                case '1':
                    if(valor < 1000000000){
                        alert('O valor dever igual ou superior a R$ 10.000.000,00');
                        return false;
                    }
                    break;
                case '2':
                    if(valor < 3000000){
                        alert('O valor dever igual ou superior a R$ 30.000,00');
                        return false;
                    }
                    break;
                case '3':
                    if(valor < 100000000 || valor > 999999900){
                        alert('O valor deve ser maior R$ 1.000.000,00 e menor que 9.999.999,00');
                        return false;
                    }
                    break;
            }
        }

        if($('#sbsdtiniciovigencia').val() == ''){
            alert('Início da vigência obrigatório.');
            return false;
        }
        if($('#sbsdtfimvigencia').val() == ''){
            alert('Fim da vigência obrigatório.');
            return false;
        }
        if($('#sbsjustificativa').val() == ''){
            alert('Justificativa para a necessidade da locação obrigatória.');
            return false;
        }
        if($('#arquivoDecreto').val() == ''){
            alert('O Arquivo é obrigatório.');
            return false;
        }

        document.getElementById('sbsvalor').onkeyup();
        document.getElementById('formulario').submit();
    }

    function dowloadDocAnexo( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexo');
        $('#formulario').submit();
    }

    function excluirDocAnexo(arqid){
        var confirma = confirm("Deseja realmente excluir o Arquivo?");
        if( confirma ){
            $('#arqid').val(arqid);
            $('#requisicao').val('excluirDocAnexo2');
            $('#formulario').submit();
        }
    }

    function excluir(sbsid) {
        var conf = confirm('Deseja realmente excluir?');
        if(conf) {
            document.getElementById('requisicao').value="excluirSolicitacaoDecreto";
            document.getElementById('sbsid').value=sbsid;
            document.getElementById('formulario').submit();
        }
    }

    function comboModalidades(tpcid){
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=comboModalidades&tpcid=" + tpcid,
            success: function(retorno){
                $('#td_mdl').html(retorno);
            }
        });
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=campoValorTag&tpcid=" + tpcid,
            success: function(retorno){
                $('#td_sbs_tag').html(retorno);
            }
        });
        $.ajax({
            type: "POST",
            url: window.location,
            data: "requisicaoAjax=campoJustificativaTag&tpcid=" + tpcid,
            success: function(retorno){
                $('#td_sbsjustificativa_tag').html(retorno);
            }
        });
    }

    function selecionarDecreto(decreto){
        if(decreto == 7446){
            window.location = 'academico.php?modulo=principal/solicitacaodecreto&acao=A&decreto=7446';
        }else if(decreto == 7689){
            window.location = 'academico.php?modulo=principal/solicitacaodecretonovo&acao=A';
        }else{
            window.location = 'academico.php?modulo=principal/solicitacaodecreto&acao=A';
        }
    }

    /**
     * Impementa modal Dialog
     * dependencia: Jquery UI
     */
   var openModal = function(title, content, arquivo) {

      jQuery("#modal-space").attr('style', 'width:550px;height:250px;display:none;');
      var $modal = jQuery("#modal-space")
        , positionX = (jQuery(window).width()/2) - ($modal.width()/2)
        , positionY = 80;

      $modal.dialog({
          position: [positionX, positionY],
          height: 250,
          width: 550,
          title: title,
          modal: true,
          open: function(event, ui) {
              jQuery("#modal-space").html(content);
          },
          buttons:{
            "Ok": function() {
                window.open('http://<?=$_SERVER['HTTP_HOST']?>/academico/arquivos/'+arquivo);
                jQuery(this).dialog("close");
            },
            "Cancelar": function() {
                jQuery(this).dialog("close");     
            }
          }
      });
   };

    $(function(){

        var _content = "<p>ATENÇÃO !!!</p><p>-O documento digitado deverá ser impresso, assinado e escaneado para posterior inserção no cadastro do decreto.</p>";

        $(".modelo-integral").click(function(e){
            e.preventDefault();
            openModal("MODELO DE TERMO - Declaração de Regularidade Processual e Disponibilidade Orçamentária – Integral", _content, 'modelo_01_-_integral.docx');
        });

        $(".modelo-parcial").click(function(e){
            e.preventDefault();
            openModal("MODELO DE TERMO - Declaração de Regularidade Processual e Disponibilidade Orçamentária – Parcial", _content, 'modelo_02_-_parcial.docx');
        });
    });
</script>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<?php
monta_titulo( "Solicitação de Autorização - Decreto 7.689/2012", "");
?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    // cria o cabeçalho padrão do módulo
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
    ?>

    <form id="formulario" name="formulario" method="post" action="academico.php?modulo=principal/solicitacaodecretonovo&acao=A" enctype="multipart/form-data">
        <?php
        $req = 'inserirSolitacaoDecreto';

        if ($_REQUEST['sbsid']) {

            $sql = "
                SELECT  sbs.tpcid,
                        sbs.epcid,
                        sbs.mdlid,
                        sbs.sbsnumprocesso,
                        sbs.sbsobjeto,
                        sbs.sbsvalor,
                        sbs.sbsdtiniciovigencia,
                        sbs.sbsdtfimvigencia,
                        sbs.sbsjustificativa,
                        sbs.docid
                FROM academico.solicitacaobensservicos sbs
                LEFT JOIN academico.modalidadelicitacao mdl on sbs.mdlid = mdl.mdlid
                WHERE sbs.sbsid = {$_REQUEST['sbsid']}
            ";
            $solicitacaobensservicos = $db->pegaLinha($sql);

            $sbsid              = $_REQUEST['sbsid'];
            $tpcid              = $solicitacaobensservicos['tpcid'];
            $epcid              = $solicitacaobensservicos['epcid'];
            $mdlid              = $solicitacaobensservicos['mdlid'];
            $sbsnumprocesso     = $solicitacaobensservicos['sbsnumprocesso'];
            $sbsobjeto          = $solicitacaobensservicos['sbsobjeto'];
            $sbsjustificativa   = $solicitacaobensservicos['sbsjustificativa'];
            $sbsdtiniciovigencia= $solicitacaobensservicos['sbsdtiniciovigencia'];
            $sbsdtfimvigencia   = $solicitacaobensservicos['sbsdtfimvigencia'];
            $sbsvalor           = number_format($solicitacaobensservicos['sbsvalor'], 2, ',', '.');
            $docid              = $solicitacaobensservicos['docid'];

            $req   = 'atualizarSolitacaoDecreto';
            $esdid = pegaEstadoObra($docid);
        }

        if ($permissaoAlterar) {
        ?>

        <input type="hidden" name="requisicao" id="requisicao" value="<?php echo $req; ?>">
        <input type="hidden" name="sbsid" id="sbsid" value="<?php echo $sbsid; ?>">
        <input type="hidden" name="docid" id="docid" value="<?php echo $docid; ?>">

        <input type="hidden" id="salvarComAnexo" name="salvarComAnexo" value="">
        <input type="hidden" id="arqid" name="arqid" value="">

        <table class="tabela" align="center"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 >
            <tr>
                <td valign="top" >
                    <br />
                    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
                        <tr>
                            <td width="25%" class="SubTituloDireita">Decreto:</td>
                            <td>
                                <select name="tipo_decreto" onChange="selecionarDecreto(this.value)" class="CampoEstilo obrigatorio" style="width:300px">
                                    <option value="">Selecione</option>
                                    <option value="7446">Solicitação de Autorização - Decreto 7.446/2011</option>
                                    <option value="7689" selected>Solicitação de Autorização - Decreto 7.689/2012</option>
                                </select>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Tipo de Contrato:</td>
                            <td>
                                <?php
                                    $arrTpcid = array("1", "2");

                                    if ($_SESSION['academico']['entid'] == ENT_SAA) {
                                        $arrTpcid[] = "3";
                                    }

                                    $sql = "
                                        select tpcid  as codigo,
                                               tpcdsc as descricao
                                        from academico.tipocontrato
                                        where tpcstatus = 'A' and tpcid in('" . implode("','", $arrTpcid) . "')
                                    ";
                                    $db->monta_combo('tpcid', $sql, 'S', 'Selecione', '', '', '', '400', 'S', 'tpcid', false, $tpcid);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Espécie da Contratação:</td>
                            <td>
                                <?php
                                    $sql = "
                                        select epcid  as codigo,
                                               epcdsc as descricao
                                        from academico.especiecontratacao
                                        where epcstatus = 'A'
                                    ";
                                    $db->monta_combo('epcid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'epcid');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Modalidade da Licitação:</td>
                            <td id="td_mdl" >
                                <?php
                                    comboModalidades($tpcid);
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Nº do Processo:</td>
                            <td>
                                <?php
                                    echo campo_texto('sbsnumprocesso', "S", "S", "Nº do Processo", 30, 20, "#####.######/####-##", "", '', '', 0, 'id="sbsnumprocesso"');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Objeto do Contrato:</td>
                            <td>
                                <?php
                                    echo campo_textarea('sbsobjeto', 'S', 'S', '', '70', '4', '700');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" id="td_sbs_tag">
                                <?php
                                    if ($tpcid) {
                                        campoValorTag($tpcid);
                                    } else {
                                        echo 'Valor R$:';
                                    }
                                ?>
                            </td>
                            <td>
                                <?php
                                    echo campo_texto('sbsvalor', "S", "S", "Valor R$", 30, 18, "###.###.###.###,##", "", '', '', 0, 'id="sbsvalor"');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Início da Vigência:</td>
                            <td>
                                <?php
                                    echo campo_data2('sbsdtiniciovigencia', 'S', 'S', '', 'S');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Fim da Vigência:</td>
                            <td>
                                <?php
                                    echo campo_data2('sbsdtfimvigencia', 'S', 'S', '', 'S');
                                ?>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" id="td_sbsjustificativa_tag">
                                <?php
                                    if ($tpcid){
                                        campoJustificativaTag($tpcid);
                                    }else{
                                        echo 'Justificativa:';
                                    }
                                ?>
                            </td>
                            <td>
                                <?php
                                    echo campo_textarea('sbsjustificativa', 'S', 'S', '', '70', '4', '1000');
                                ?>
                            </td>
                        </tr>
                        <!-- Legenda tipo de arquivo aceitos -->
                        <tr>
                            <td class="SubTituloDireita" width="25%">MODELO DE TERMO - Declaração de Regularidade <br />Processual e Disponibilidade Orçamentária</td>
                            <td>
                                <table border="0" width="20%">
                                    <tr>
                                        <td width="10">
                                            <a href="#" class="modelo-integral">
                                                Integral <img src="../imagens/icone_word.png" title="Integral" />
                                            </a>
                                        </td>
                                        <td width="10">
                                            <a href="#" class="modelo-parcial">
                                                Parcial <img src="../imagens/icone_word.png" title="Parcial" />
                                            </a>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita">Arquivo:</td>
                            <td colspan="2">
                                <input type="file" size="150" name="arquivoDecreto" id="arquivoDecreto">
                            </td>
                        </tr>

                    <?PHP
                        if (isset($_REQUEST['sbsid'])) {

                            if( $esdid == WF_EM_ANALISE_PELA_CONJUR ){ //TIPOS DE ARQUIVOS QUANDO ESTIVER EM ANALISE PELA CONJUR 
                    ?>
                                <tr>
                                    <td class="SubTituloDireita">Tipo de Arquivo:</td>
                                    <td colspan="2">
                                        <?php
                                            $sqlTadid = "SELECT tadid AS codigo, taddsc AS descricao FROM academico.tipoarquivodecreto WHERE tadid IN (1,3)";

                                            $db->monta_combo('tadid', $sqlTadid, 'S', 'Selecione', '', '', '', '200', 'S', 'tadid');
                                        ?>
                                    </td>
                                </tr>
                    <?PHP }elseif( $esdid == WF_EM_AJUSTE_PELO_DEMANDANTE ){ ?>
                                <tr>
                                    <td class="SubTituloDireita">Tipo de Arquivo:</td>
                                    <td colspan="2">
                                        <?php
                                            $sqlTadid = "SELECT tadid AS codigo, taddsc AS descricao FROM academico.tipoarquivodecreto WHERE tadid IN (2)";

                                            $db->monta_combo('tadid', $sqlTadid, 'S', 'Selecione', '', '', '', '200', 'S', 'tadid');
                                        ?>
                                    </td>
                                </tr>
                    <?PHP   }
                        }

                        if ($sbsid != '') {
                        ?>
                            <tr>
                                <td class="SubTituloDireita" width="25%">Lista de Arquivos:</td>
                                <td colspan="2">
                                    <?PHP
                                        $acao = "
                                            <img border=\"0\" title=\"Apagar arquivo\" onclick=\"excluirDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                                            <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                        ";

                                        $desativado = "
                                            <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
                                            <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                                        ";

                                        $down = "<a title=\"Baixar arquivo\" href=\"#\" onclick=\"dowloadDocAnexo('|| arq.arqid ||');\">' || arq.arqnome || '</a>";

                                        $sql = "
                                            SELECT  CASE WHEN anx.usucpf = '{$_SESSION['usucpf']}'
                                                        THEN '{$acao}'
                                                        ELSE '{$desativado}'
                                                    END as acao,
                                                    '{$down}' as descricao,
                                                    td.taddsc as tipo_arquivo_decreto,
                                                    arq.arqnome||'.'||arq.arqextensao,
                                                    su.usunome,
                                                    to_char(aqddtinclusao, 'DD/MM/YYYY') as aqddtinclusao
                                            FROM academico.arquivodecreto anx
                                            JOIN academico.tipoarquivodecreto td on td.tadid = anx.tadid
                                            JOIN public.arquivo arq on arq.arqid = anx.arqid
                                            JOIN seguranca.usuario su ON (su.usucpf = anx.usucpf)
                                            WHERE anx.aqdstatus = 'A' AND sbsid = {$sbsid}
                                        ";
                                        $cabecalho = Array("Ação", "Descrição",  "Tipo de Documento", "Nome do arquivo", "Usuário Responsável", "Data da Inclusão");
                                        //$whidth = Array('20%', '60%', '20%');
                                        $align  = Array('center', 'left', 'left', 'left', 'right');
                                        $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
                                    ?>
                                </td>
                            </tr>
                    <?PHP } ?>
                        <tr>
                            <td align="center" colspan="2">
                                <input type="button" name="salvar" value="Salvar" onclick="validarFormulario();">
                                <input type="button" name="novo" value="Novo" onclick="window.location='academico.php?modulo=principal/solicitacaodecretonovo&acao=A';">
                            </td>
                        </tr>
                    </table>
                    <br />
                </td>
                <?PHP if ($_REQUEST['sbsid']) { ?>
                    <td valign="top" align="center" width="100" id="td_workflow">
                        <br>
                        <?php wf_desenhaBarraNavegacao($docid, array('tpcid' => $tpcid, 'secretario' => 1, 'ministro' => 2, 'sbsid' => $_REQUEST['sbsid'], 'aveid' => 0, 'docid'=>$docid )); ?>
                        <br>
                    </td>
                <?PHP } ?>
            </tr>
        </table>
    <?php
        }
    ?>
</form>

<?php

$sql = "
    SELECT '<span style=\"white-space:nowrap;\" ><img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.location=\'academico.php?modulo=principal/solicitacaodecretonovo&acao=A&sbsid='||sbs.sbsid||'\';\"> <img src=../imagens/excluir.gif style=cursor:pointer; onclick=\"excluir(\''||sbs.sbsid||'\');\"></span>' as acao,
            tpc.tpcdsc,
            epc.epcdsc,
            mdl.mdldsc,
            sbs.sbsnumprocesso,
            sbs.sbsvalor,
            to_char(sbs.sbsdtiniciovigencia, 'DD/MM/YYYY') as sbsdtiniciovigencia,
            to_char(sbs.sbsdtfimvigencia, 'DD/MM/YYYY') as sbsdtfimvigencia
     FROM academico.solicitacaobensservicos sbs

     LEFT JOIN academico.modalidadelicitacao mdl on sbs.mdlid = mdl.mdlid
     LEFT JOIN academico.tipocontratomodalidade tcm on mdl.mdlid = tcm.mdlid
     LEFT JOIN academico.tipocontrato tpc on sbs.tpcid = tpc.tpcid
     LEFT JOIN academico.especiecontratacao epc on sbs.epcid = epc.epcid

     WHERE sbs.entid = '{$_SESSION['academico']['entid']}' AND sbs.sbsstatus = 'A'

     ORDER BY sbs.sbsid
";

$cabecalho = array("&nbsp","Tipo de Contrato","Espécie de Contratação","Modalidade da Licitação","Nº do Processo","Valor R$","Início da Vigência","Fim da Vigência");
$db->monta_lista_simples($sql,$cabecalho,1000,5,'N','95%',$par2);

?>

<div id="modal-space"></div>

<?
academico_trata_workflow($tpcid, $esdid);
?>
</div>