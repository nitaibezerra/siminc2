<?php
verificaSessao('orgao');
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
include_once APPRAIZ . "obras2/classes/controle/CampoExternoControle.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
//include  	 APPRAIZ . "includes/cabecalho.inc";
$orgid = $_SESSION['obras2']['orgid'];
$empid = $_SESSION['obras2']['empid'];
$_SESSION['obras2']['obrid'] = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
$obrid = $_SESSION['obras2']['obrid'];

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');

        $municipio = new Municipio();
        echo $db->monta_combo("muncod", $municipio->listaCombo(array('estuf' => $_POST['estuf'])), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
        exit;
}





if ($_POST['salvar_questionario'] && $_POST['identExterno'] && $_FILES) {


    $obMonta = new CampoExternoControle();
    $obMonta->salvar();
}

if ($_POST['check_cad_novo_end']) {
    
    

    global $db;

    $endereco = new Endereco();

    $dadosEnd = $_POST['endereco'];
    $dadosEnd['tpeid'] = TIPO_ENDERECO_OBJETO;
    if(!empty($_POST['muncod'])){
    $dadosEnd['muncod'] = $_POST['muncod'];
    }
    $dadosEnd['endcep'] = str_replace(Array('.', '-'), '', $_POST['endereco']['endcep']);
    //$dadosEnd['medlatitude'] = $_POST['graulatitude'] . "." . $_POST['minlatitude'] . "." . $_POST['seglatitude'] . ".S";
    //$dadosEnd['medlongitude'] = $_POST['graulongitude'] . "." . $_POST['minlongitude'] . "." . $_POST['seglongitude'] . ".W";
    $endid = $endereco->popularDadosObjeto($dadosEnd)->salvar();

    if (!empty($endid)) {

        $sql = "update obras2.cumprimenoobjobra set endid = $endid where obrid = {$obrid}";
        $testa_endereco = $db->carregar($sql);
    }
}

if ($_POST['projeto_proprio_text']) {
    $sql = "update obras2.cumprimenoobjobra set tpodsc = '{$_POST['projeto_proprio_text']}' where obrid = {$obrid}";
    $testa_tpo = $db->carregar($sql);
}





switch ($_REQUEST['requisicao']) {
    case "resetchecklist":
        unset($_SESSION['obras2']['checklistfnde']);
        die('<script>window.close();</script>');
        break;
    case "checklist":
        $chkFnde = new ChecklistCumprimento();
        $tipo = $_GET['tipo'];                                 // tec/adm/2p
        $obrid = $_SESSION['obras2']['obrid'];                  // ID obrid
        $ckfid = (!empty($_GET['ckfid'])) ? $_GET['ckfid'] : 0; // ID ckfid
        $acao = $_GET['acao'];                                 // duplicar/editar/cadastrar/excluir;
        $chkFnde->mostraPopupChecklistQuestionario($tipo, $obrid, $ckfid, $acao);
        die();
        break;
}
if ($_REQUEST['arquivo_download']) {
    ob_get_clean();
    $file = new FilesSimec();
    $file->getDownloadArquivo($_REQUEST['arquivo_download']);
}

if ($_REQUEST['deletar_arquivo']) {
    ob_get_clean();
    $file = new FilesSimec('questoesanexascumprimento', 'obras2');
    $file->setRemoveUpload($_REQUEST['deletar_arquivo']);
}

//tabela que relaciona o documento de workflow com a obra
$sql1 = "select count(coo.obrid)  from obras2.cumprimenoobjobra coo where coo.obrid =" . $obrid;
$qt_registros = $db->pegaUm($sql1);

if ($qt_registros == 0) {

    $doc_id = wf_cadastrarDocumento(200, "Fluxo de cumprimento de objeto");
    $sql2 = "insert into obras2.cumprimenoobjobra (obrid ,docid ,coostatus ) values ({$obrid},{$doc_id},'A') returning cooid";



    $cooid = $db->pegaUm($sql2);


    $sql3 = "select count(clcid) as qt from obras2.checklistcumprimento where obrid = " . $obrid;
    $qt_questionatios = $db->pegaUm($sql3);



    if ($qt_questionatios == 0) {
        $chkFnde = new ChecklistCumprimento();

        $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid,  NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
        $clcid = $db->pegaUm($sql);
        $chkFnde->pegaQrpidChk($clcid, QUEID_DOCUMENTOS_INSERIDOS);


        $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid,  NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
        $clcid = $db->pegaUm($sql);
        $chkFnde->pegaQrpidChk($clcid, QUEID_DADOS_LICITACAO);



        $sql1 = "select count(*) from obras2.obras o where o.obridvinculado::text like '90%' and o.obrstatus = 'A' AND o.obridpai IS NULL and o.obridvinculado::text like '%{$obrid}' ";
        $qtds_vinculos = $db->pegaUm($sql1);

        $i = 0;
        while ($i < $qtds_vinculos) {

            $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid,  NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
            $clcid = $db->pegaUm($sql);
            $chkFnde->pegaQrpidChk($clcid, QUEID_NOVA_LICITACAO);
            $i++;
        }

        $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid,  NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
        $clcid = $db->pegaUm($sql);
        $chkFnde->pegaQrpidChk($clcid, QUEID_REFORMULACOES_IMPLEMENTADAS);


        $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid,  NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
        $clcid = $db->pegaUm($sql);
        $chkFnde->pegaQrpidChk($clcid, QUEID_REFORMULACAO_RECURSO);


        $sql = "INSERT INTO obras2.checklistcumprimento (obrid, qrpid, usucpf, clcdatainclusao, clcstatus, cooid )
           VALUES($obrid, NULL, '" . $_SESSION['usucpf'] . "', NOW(), 'A',$cooid) RETURNING clcid";
        $clcid = $db->pegaUm($sql);
        $chkFnde->pegaQrpidChk($clcid, QUEID_DOCUMENTOS_FINAIS);
        $db->commit();
    }
}

$sql1 = "select * from obras2.cumprimenoobjobra coo where coo.obrid =" . $obrid;
$dados_coo = $db->pegaLinha($sql1);


$sql = "select * from workflow.documento where docid=" . $dados_coo[docid];
$dados_documento = $db->pegaLinha($sql);





if ($_REQUEST['requisicao'] == 'ajaxInserirGestor') {
    $ent_id = $_POST['entid'];

    $sql2 = "insert into obras2.cumprimentogestoresadicionais (entid ,cooid ) values ({$ent_id},{$dados_coo[cooid]})";
    $cooid = $db->pegaUm($sql2);
    $db->commit();
    die();
}

function recuperaEntidadePorTipo($dutid, $estuf = null, $muncod = null) {
    global $db;

    $where = '';
    if (!empty($estuf))
        $where = "iu.estuf = '{$estuf}'";
    else
        $where = "iu.muncod = '{$muncod}'";

    $sql = "select e.entid
            from par.entidade  e
            inner join par.dadosunidadetipo dt on dt.dutid = e.dutid
            inner join par.instrumentounidade iu on iu.inuid = e.inuid
            left join  territorios.municipio m on m.muncod = iu.muncod
            where entstatus = 'A' and {$where} and dt.dutid = {$dutid}
            ";

    return $db->pegaUm($sql);
}

if ($esfera == 'M') {
    $dutid = '7';
} else {
    $dutid = '8';
};


$sql = "SELECT
                        (SELECT 
                        
                         CASE WHEN po1.pronumeroprocesso IS NOT NULL THEN
                         to_char(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN po2.pronumeroprocesso IS NOT NULL THEN
                         to_char(Replace(Replace(Replace(po2.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            WHEN o.tooid = 2 AND TRIM(o.obrnumprocessoconv) != '' THEN
                         to_char(Replace(Replace(Replace( TRIM(o.obrnumprocessoconv),'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                            END as processoobra
                                FROM obras2.obras o
                                LEFT JOIN par.processoobrasparcomposicao pop1 ON pop1.preid = o.preid AND pop1.pocstatus = 'A'
                         LEFT JOIN par.processoobraspar po1 on po1.proid = pop1.proid and po1.prostatus = 'A'
                         LEFT JOIN par.processoobraspaccomposicao pop2 ON pop2.preid = o.preid AND pop2.pocstatus = 'A'
                         LEFT JOIN par.processoobra po2 on po2.proid = pop2.proid and po2.prostatus = 'A' 
                         where o.obrid = {$obrid}) as  convenioprocesso,
                        og.*,
                        ent.*,
                        e.*,
                        o.*,
                        mun.*,
                        ende.*,
                        tob.tobdesc,
                        p.prfdesc,
                        co.clodsc,
                        tpo.tpodsc,
                        tf.frpdesc,
                        wf.esdid,
                        to_char(obrvalorprevisto, '9G999G990D99') as valorprevisto
                FROM
                        obras2.obras o
                JOIN obras2.empreendimento e                 ON e.empid   = o.empid
                INNER JOIN entidade.endereco	  ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
                LEFT JOIN territorios.municipio    mun ON mun.muncod = ende.muncod
                LEFT JOIN obras2.orgao og                    ON og.orgid  = e.orgid
                LEFT JOIN entidade.entidade ent              ON ent.entid = o.entid
                LEFT JOIN obras2.tipoobra tob                ON tob.tobid = o.tobid
                LEFT JOIN obras2.programafonte p             ON p.prfid   = e.prfid
                LEFT JOIN obras2.classificacaoobra co        ON co.cloid  = o.cloid
                LEFT JOIN obras2.tipologiaobra tpo           ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.tipoformarepasserecursos tf ON tf.frpid  = o.frpid
                LEFT JOIN workflow.documento wf ON wf.docid  = o.docid
                WHERE
                        o.obrid = {$obrid}";
$dados = $db->pegaLinha($sql);

// Inclusão de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
//require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

$_SESSION['obras2']['empid'] = ($_REQUEST['empid'] ? $_REQUEST['empid'] : $_SESSION['obras2']['empid']);

if ($_GET['acao'] != 'V') {
    //Chamada de programa
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";

//	$arMenuBlock = bloqueiaMenuObjetoPorSituacao( $obrid );
    $arMenuBlock = array();

    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros, $arMenuBlock);

    $dados_documento = wf_pegarDocumento($dados[docid]);

   


    $habilitado = true;
    $habilita = 'S';
} else {
    ?>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $_SESSION['obras2']['obrid'] = ($_REQUEST['obrid'] ? $_REQUEST['obrid'] : $_SESSION['obras2']['obrid']);
    $db->cria_aba($abacod_tela, $url, $parametros);
//	$somenteLeitura = false;
    $habilitado = false;
    $habilita = 'N';
}

//if( possui_perfil( array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL) ) ){
//	$habilitado = false;
//	$habilita = 'N';
//}

$_SESSION['obras2']['empid'] = $obra->empid ? $obra->empid : $_SESSION['obras2']['empid'];
$empreendimento = new Empreendimento($_SESSION['obras2']['empid']);
$empid = $empreendimento->empid;

if (empty($_SESSION['obras2']['obrid'])) {
    $dadoEmpreendimento = $empreendimento->getDados();
    ver($dadoEmpreendimento);
}

if (($obra->preid || !possui_perfil(PFLCOD_SUPER_USUARIO)) && $obra->obrid) {
    $habilitado = false;
    $habilita = 'N';
}

$orgid = $_SESSION['obras2']['orgid'];
$orgao = new Orgao($orgid);
$tipologiaObra = new TipologiaObra();
$programaFonte = new ProgramaFonte();
$modalidadeEnsino = new ModalidadeEnsino();
$tipoObra = new TipoObra();
$classificacaoObra = new ClassificacaoObra();
$tipoOrigemObra = new TipoOrigemObra();
$endereco = new Endereco(($obra->endid ? $obra->endid : $dadoEmpreendimento['endid']));

//$orgid = $_SESSION['obras2']['orgid'];
$docid = pegaDocidObra($obrid);
if ($obrid) {
    echo cabecalhoObra($obrid);
}

monta_titulo('Cumprimento de objeto', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
?>



<div style="background-color: red;text-align: center;color:#FFF;padding: 5px;font-size: 16px;margin-bottom: 20px;margin-top: 20px">
    Está tela ainda não é funcional, trata-se de um protótipo em desenvolvimento
</div>    

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>


<?php  if (ESDID_OBJ_CONCLUIDO != $dados_documento[esdid]) {
        echo '<div style="background-color: #ccc;text-align: center;color:#000;padding: 5px;font-size: 16px;margin-bottom: 20px;margin-top: 20px">
            Essa aba só pode ser acessada quando a obra for concluída
        </div>';


        exit();
        
    }
 ?>

<script>

    function inserirVistoriador(entid) {
        var caminho_atual = '/obras2/obras2.php';
        if (entid) {
            return windowOpen(caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A&busca=entnumcpfcnpj&entid=' + entid, 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        } else {
            return windowOpen(caminho_atual + '?modulo=principal/inserir_vistoriador&acao=A', 'blank', 'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        }
    }

    function carregarMunicipio(estuf) {
        var td = $('#td_municipio');
        if (estuf != '') {
            var url = location.href;
            $.ajax({
                url: url,
                type: 'post',
                data: {ajax: 'municipio',
                    estuf: estuf},
                dataType: "html",
                async: false,
                beforeSend: function() {
                    //divCarregando();
                    td.find('select option:first').attr('selected', true);
                },
                error: function() {
                    // divCarregado();
                },
                success: function(data) {
                    td.html(data);
                    //divCarregado();
                }
            });
        } else {
            td.find('select option:first').attr('selected', true);
            td.find('select').attr('selected', true)
                    .attr('disabled', true);
        }
    }


    $(document).ready(function() {




        $(".projeto_proprio_check").click(function() {

            if ($(this).is(':checked'))
            {
                $("#tpoid").hide();
                document.getElementById("projeto_proprio_text").disabled = false;


            } else {

                document.getElementById("projeto_proprio_text").disabled = true;
                $("#tpoid").show();
            }
        });

    });


    $(document).ready(function() {
        $("#check_cad_novo_end").click(function() {
            if ($(this).is(':checked'))
            {
                document.getElementById("estuf_end").disabled = false;
                document.getElementById("endcep").disabled = false;
                document.getElementById("endlog").disabled = false;
                document.getElementById("endnum").disabled = false;
                document.getElementById("endcom").disabled = false;
                document.getElementById("endbai").disabled = false;
                document.getElementById("mundescricao").disabled = false;




            } else {
                document.getElementById("estuf_end").disabled = true;
                document.getElementById("endcep").disabled = true;
                document.getElementById("endlog").disabled = true;
                document.getElementById("endnum").disabled = true;
                document.getElementById("endcom").disabled = true;
                document.getElementById("endbai").disabled = true;
                document.getElementById("mundescricao").disabled = true;


            }
        });

    });


    function popupChecklist(tipo, ckfid, acao) {
        var url = "/obras2/obras2.php?modulo=principal/cadCumprimento&acao=A";
        var requisicao = 'checklist';
        var obrid = <?php echo $_SESSION['obras2']['obrid'] ?>;
        if (acao.trim() === '') {
            acao = 'cadastro';
        }

        url = url + '&requisicao=' + requisicao + '&tipo=' + tipo + '&obrid=' + obrid + '&ckfid=' + ckfid + '&acao=' + acao;


        window.open(url, "_blank");


    }


    function editarChecklist(tipo, ckfid) {


        var acao = 'editar';
        popupChecklist(tipo, ckfid, acao);
    }

</script>


<?php
if ($dados_documento[esdid] == ESDID_CUMPRIMENTO_CADASTRAMENTO || $dados_documento[esdid] == ESDID_CUMPRIMENTO_CORRECAO || possui_perfil(PFLCOD_SUPER_USUARIO)) {
    $habilita = "S";
} else {
    $habilita = "N";
}

if (possui_perfil(PFLCOD_GESTOR_MEC) || possui_perfil(PFLCOD_SUPER_USUARIO)) {
    $mostra_relatorio = true;

    if ($dados_documento[esdid] == ESDID_CUMPRIMENTO_VALIDACAO) {
        $habilita = "N";
    }
} else {
    $mostra_relatorio = false;
}
?>

<table width="100%">

    <tr><td width="90%">




            <table width="100%">
                <tr>
                    <td style="text-align: center">
                        <h4>DADOS A SEREM COMPILADOS DO SIMEC</h4>
                    </td>
                </tr>
            </table>
            <form  method="post" method="post">
                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">


                    <input type="hidden" name="req" value="salvar"/>
                    <input type="hidden" name="obra[obridvinculado]" id="obridvinculado" value="<?php echo $obridVinculado ?>"/>
                    <input type="hidden" name="obra[obrid]" id="obrid" value="<?php echo $obra->obrid ?>"/>
                    <input type="hidden" name="obra[empid]" value="<?php echo ($obra->empid ? $obra->empid : $_SESSION['obras2']['empid']) ?>"/>

                    <tr>
                        <td class="SubTituloDireita">
                            Entidade
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('entnome', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="entnome"', '', $dados[entnome]); ?>

                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            UF
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('estuf5', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="estuf"', '', $dados[estuf]); ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            CNPJ
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('entnumcpfcnpj', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="entnumcpfcnpj"', '', $dados[entnumcpfcnpj]) ?>

                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">
                            Fonte de origem
                        </td>
                        <td  align="left">
                            <?php
                            $tipoOrigemObra = new TipoOrigemObra();
                            $sql = $tipoOrigemObra->listaCombo(true);
                            $db->monta_combo('tooid', $sql, 'N', 'Selecione...', "", "", "", 200, 'N', 'tooid', '', $dados[tooid], '');
//                                $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
//                                $db->monta_combo('frpid', $tipoFormaRepasseRecursos->listaCombo(), $habilita, "Selecione...", 'abreCamposRecursos', '', '', '200', 'N', 'frpid');
                            ?>	 
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Local da obra
                        </td>
                        <td  align="left">
                            <?php
                            $classificacaoObra = new ClassificacaoObra();
                            $db->monta_combo("cloid", $classificacaoObra->listaCombo(), 'N', "Todos", "", "", "", 200, "N", "cloid", "", $dados[cloid]);
                            ?>
                        </td>
                    </tr>


                    <tr>
                        <td class="SubTituloDireita">
                            Nível de ensino
                        </td>
                        <td  align="left">
                            <?php
                            $modalidade = new ModalidadeEnsino();
                            $db->monta_combo("moeid", $modalidade->listaCombo(), 'N', "Todos", "", "", "", 200, "N", "moeid", "", $dados[moeid]);
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Tipo de obra
                        </td>
                        <td  align="left">
                            <?php
                            $tipoObra = new TipoObra();
                            $db->monta_combo("tobid", $tipoObra->listaCombo(), 'N', "Todos", "", "", "", 200, "N", "tobid", "", $dados[tobid]);
                            ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Tipologia
                        </td>
                        <td  align="left">
                            <?php
                            $tipologiaObra = new TipologiaObra();
                            $param = array("orgid" => $_SESSION['obras2']['orgid']);
                            $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), 'N', "Todas", "", "", "", 200, "N", "tpoid", "", $dados[tpoid]);
                            ?>
                            <table> 
                                <tr>
                                    <td>
                                        Projeto próprio
                                    </td>
                                    <td  align="left">


                                        <?php if (empty($dados_coo[tpodsc])) { ?>

                                            <input type="checkbox" class="projeto_proprio_check" >

                                            <input type="text" value="<?php echo $dados[projeto_proprio_text] ?>" id="projeto_proprio_text" name="projeto_proprio_text" disabled="disable">

                                        <?php } else { ?>
                                            <input type="checkbox" class="projeto_proprio_check" checked="checked">

                                            <input type="text" value="<?php echo $dados_coo[tpodsc] ?>" id="projeto_proprio_text" name="projeto_proprio_text">

                                        <?php } ?>

                                    </td>
                                </tr>
                            </table>


                        </td>
                    </tr>





                    <tr>
                        <td class="SubTituloDireita">
                            Número Processo/Convênio
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('convenioprocesso', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="convenioprocesso"', '', $dados[convenioprocesso]) ?> 

                        </td>
                    </tr>


                    <tr>
                        <td class="SubTituloDireita">
                            ID número
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('obrid', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="obrid"', '', $dados[obrid]) ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Termo de compromisso número
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('numerocompromisso', 'N', 'N', '', 20, 20, '', '', 'left', '', '', 'id="numerocompromisso"', '', "") ?>
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Valor contratado
                        </td>
                        <td  align="left">
                            <?php echo campo_texto('obrvalorprevisto', 'N', 'N', '', 20, 20, '###.###.###,##', '', 'right', '', '', 'id="obrvalorprevisto"', '', $dados[valorprevisto]); ?>

                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">
                            Situação da obra
                        </td>
                        <td  align="left">
                            <?php
                            $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                            $db->monta_combo("esdid", $sql, 'N', "Todos", "", "", "", "200", "N", "esdid", "", $dados['esdid']);
                            ?>
                        </td>
                    </tr>
                    <?php if (empty($dados_coo[endid])) {
                        ?>

                        <tr>
                            <td colspan="3">Local da Obra</td>
                        </tr>

                        <tr>
                            <td align="right" class="SubTituloDireita"><label>Desejo cadastrar outro endereço:</label></td>
                            <td colspan="2">
                                <input type="checkbox" id="check_cad_novo_end" name="check_cad_novo_end">
                            </td>
                        </tr>

                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
                            <td colspan="2">
                                <input type="text" value="<?php echo $dados[endcep] ?>" maxlength="7" id="endcep" disabled="disable" name="endereco[endcep]">




                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                                <label id="lbLogadouro"> Logradouro: </label>
                            </td>
                            <td colspan="2">
                                <input type="text" value="<?php echo $dados[endlog] ?>" id="endlog" name='endereco[endlog]' disabled="disabled">


                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
                            <td colspan="2">
                                <input type="text" value="<?php echo $dados[endnum] ?>" id="endnum" name='endereco[endnum]' disabled="disabled">


                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
                            <td colspan="2">
                                <input type="text" value="<?php echo $dados[endcom] ?>" id="endcom" name='endereco[endcom]' disabled="disabled">


                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
                            <td colspan="2">
                                <input type="text" value="<?php echo $dados[endbai] ?>" id="endbai"  name='endereco[endbai]' disabled="disabled">
                            </td>
                        </tr>

                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>UF: </label></td>
                            <td colspan="2">
                                <select name='endereco[estuf]' disabled="disabled" class="CampoEstilo" style="width: auto" onchange="carregarMunicipio(this.value);" id="estuf_end" name='estuf_end'><option value="">Selecione...</option><option value="AC">Acre</option>
                                    <option value="AL" <?php if ($dados[estuf] == "AL") echo "selected='selected'" ?>>Alagoas</option>
                                    <option value="AP" <?php if ($dados[estuf] == "AP") echo "selected='selected'" ?>>Amapá</option>
                                    <option value="AM" <?php if ($dados[estuf] == "AM") echo "selected='selected'" ?>>Amazonas</option>
                                    <option value="BA" <?php if ($dados[estuf] == "BA") echo "selected='selected'" ?>>Bahia</option>
                                    <option value="CE" <?php if ($dados[estuf] == "CE") echo "selected='selected'" ?>>Ceará</option>
                                    <option value="DF" <?php if ($dados[estuf] == "DF") echo "selected='selected'" ?>>Distrito Federal</option>
                                    <option value="ES" <?php if ($dados[estuf] == "ES") echo "selected='selected'" ?>>Espírito Santo</option>
                                    <option value="GO" <?php if ($dados[estuf] == "GO") echo "selected='selected'" ?>>Goiás</option>
                                    <option value="MA" <?php if ($dados[estuf] == "MA") echo "selected='selected'" ?>>Maranhão</option>
                                    <option value="MT" <?php if ($dados[estuf] == "MT") echo "selected='selected'" ?>>Mato Grosso</option>
                                    <option value="MS" <?php if ($dados[estuf] == "MS") echo "selected='selected'" ?>>Mato Grosso do Sul</option>
                                    <option value="MG" <?php if ($dados[estuf] == "MG") echo "selected='selected'" ?>>Minas Gerais</option>
                                    <option value="PA" <?php if ($dados[estuf] == "PA") echo "selected='selected'" ?>>Pará</option>
                                    <option value="PB" <?php if ($dados[estuf] == "PB") echo "selected='selected'" ?>>Paraíba</option>
                                    <option value="PR" <?php if ($dados[estuf] == "PR") echo "selected='selected'" ?>>Paraná</option>
                                    <option value="PE" <?php if ($dados[estuf] == "PE") echo "selected='selected'" ?>>Pernambuco</option>
                                    <option value="PI" <?php if ($dados[estuf] == "PI") echo "selected='selected'" ?>>Piauí</option>
                                    <option value="RJ" <?php if ($dados[estuf] == "RJ") echo "selected='selected'" ?>>Rio de Janeiro</option>
                                    <option value="RN" <?php if ($dados[estuf] == "RN") echo "selected='selected'" ?>>Rio Grande do Norte</option>
                                    <option value="RS" <?php if ($dados[estuf] == "RS") echo "selected='selected'" ?>>Rio Grande do Sul</option>
                                    <option value="RO" <?php if ($dados[estuf] == "RO") echo "selected='selected'" ?>>Rondônia</option>
                                    <option value="RR" <?php if ($dados[estuf] == "RR") echo "selected='selected'" ?>>Roraima</option>
                                    <option value="SC" <?php if ($dados[estuf] == "SC") echo "selected='selected'" ?>>Santa Catarina</option>
                                    <option value="SP" <?php if ($dados[estuf] == "SP") echo "selected='selected'" ?>>São Paulo</option>
                                    <option value="SE" <?php if ($dados[estuf] == "SE") echo "selected='selected'" ?>>Sergipe</option>
                                    <option value="TO" <?php if ($dados[estuf] == "TO") echo "selected='selected'" ?>>Tocantins</option>
                                </select>
                            </td>
                        </tr>




                        <tr >

                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município: </label></td>
                            <td colspan="2" id="td_municipio"><?php echo $dados[mundescricao] ?>

                            </td>
                        </tr>

                        <?php
                    } else {

                       $sql = "select * from obras2.cumprimenoobjobra coo "
                                . "INNER JOIN entidade.endereco ende ON ende.endid = coo.endid AND ende.endstatus = 'A' "
                                . "AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . " "
                                . "LEFT JOIN territorios.municipio    mun ON mun.muncod = ende.muncod"
                                . " where coo.endid = " . $dados_coo[endid];

                        $dados_endereco = $db->pegaLinha($sql);
                        //ver($dados_endereco,d);
                        ?>

                        <tr>
                        <input type="hidden" name="check_cad_novo_end" value="1">
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>
                            <td colspan="2">

                                <?php echo campo_texto('endereco[endcep]', 'N', $habilita, '', 20, 8, '', '', 'left', '', '', 'id="endcep"', '', $dados_endereco[endcep]) ?>



                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                                <label id="lbLogadouro"> Logradouro: </label>
                            </td>
                            <td colspan="2">

                                <?php echo campo_texto('endereco[endlog]', 'N', $habilita, '', 20, 100, '', '', 'left', '', '', 'id="endlog"', '', $dados_endereco[endlog]) ?>

                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>
                            <td colspan="2">
                                <?php echo campo_texto('endereco[endnum]', 'N', $habilita, '', 20, 20, '', '', 'left', '', '', 'id="endnum"', '', $dados_endereco[endnum]) ?>

                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>
                            <td colspan="2">
                                <?php echo campo_texto('endereco[endcom]', 'N', $habilita, '', 20, 100, '', '', 'left', '', '', 'id="endcom"', '', $dados_endereco[endcom]) ?>

                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>
                            <td colspan="2">
                                <?php echo campo_texto('endereco[endbai]', 'N', $habilita, '', 20, 20, '', '', 'left', '', '', 'id="endbai"', '', $dados_endereco[endbai]) ?>

                            </td>
                        </tr>
                        <tr>
                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>UF: </label></td>
                            <td colspan="2">
                                
                                <select name='endereco[estuf]' class="CampoEstilo" style="width: auto" onchange="carregarMunicipio(this.value);" id="estuf_end" name='estuf_end'><option value="">Selecione...</option><option value="AC">Acre</option>
                                    <option value="AL" <?php if ($dados_endereco[estuf] == "AL") echo "selected='selected'" ?>>Alagoas</option>
                                    <option value="AP" <?php if ($dados_endereco[estuf] == "AP") echo "selected='selected'" ?>>Amapá</option>
                                    <option value="AM" <?php if ($dados_endereco[estuf] == "AM") echo "selected='selected'" ?>>Amazonas</option>
                                    <option value="BA" <?php if ($dados_endereco[estuf] == "BA") echo "selected='selected'" ?>>Bahia</option>
                                    <option value="CE" <?php if ($dados_endereco[estuf] == "CE") echo "selected='selected'" ?>>Ceará</option>
                                    <option value="DF" <?php if ($dados_endereco[estuf] == "DF") echo "selected='selected'" ?>>Distrito Federal</option>
                                    <option value="ES" <?php if ($dados_endereco[estuf] == "ES") echo "selected='selected'" ?>>Espírito Santo</option>
                                    <option value="GO" <?php if ($dados_endereco[estuf] == "GO") echo "selected='selected'" ?>>Goiás</option>
                                    <option value="MA" <?php if ($dados_endereco[estuf] == "MA") echo "selected='selected'" ?>>Maranhão</option>
                                    <option value="MT" <?php if ($dados_endereco[estuf] == "MT") echo "selected='selected'" ?>>Mato Grosso</option>
                                    <option value="MS" <?php if ($dados_endereco[estuf] == "MS") echo "selected='selected'" ?>>Mato Grosso do Sul</option>
                                    <option value="MG" <?php if ($dados_endereco[estuf] == "MG") echo "selected='selected'" ?>>Minas Gerais</option>
                                    <option value="PA" <?php if ($dados_endereco[estuf] == "PA") echo "selected='selected'" ?>>Pará</option>
                                    <option value="PB" <?php if ($dados_endereco[estuf] == "PB") echo "selected='selected'" ?>>Paraíba</option>
                                    <option value="PR" <?php if ($dados_endereco[estuf] == "PR") echo "selected='selected'" ?>>Paraná</option>
                                    <option value="PE" <?php if ($dados_endereco[estuf] == "PE") echo "selected='selected'" ?>>Pernambuco</option>
                                    <option value="PI" <?php if ($dados_endereco[estuf] == "PI") echo "selected='selected'" ?>>Piauí</option>
                                    <option value="RJ" <?php if ($dados_endereco[estuf] == "RJ") echo "selected='selected'" ?>>Rio de Janeiro</option>
                                    <option value="RN" <?php if ($dados_endereco[estuf] == "RN") echo "selected='selected'" ?>>Rio Grande do Norte</option>
                                    <option value="RS" <?php if ($dados_endereco[estuf] == "RS") echo "selected='selected'" ?>>Rio Grande do Sul</option>
                                    <option value="RO" <?php if ($dados_endereco[estuf] == "RO") echo "selected='selected'" ?>>Rondônia</option>
                                    <option value="RR" <?php if ($dados_endereco[estuf] == "RR") echo "selected='selected'" ?>>Roraima</option>
                                    <option value="SC" <?php if ($dados_endereco[estuf] == "SC") echo "selected='selected'" ?>>Santa Catarina</option>
                                    <option value="SP" <?php if ($dados_endereco[estuf] == "SP") echo "selected='selected'" ?>>São Paulo</option>
                                    <option value="SE" <?php if ($dados_endereco[estuf] == "SE") echo "selected='selected'" ?>>Sergipe</option>
                                    <option value="TO" <?php if ($dados_endereco[estuf] == "TO") echo "selected='selected'" ?>>Tocantins</option>
                                </select>
                                

                            </td>




                        </tr>
                        
                        <tr >

                            <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município: </label></td>
                            <td colspan="2" id="td_municipio"><?php echo $dados_endereco[mundescricao] ?>

                            </td>
                        </tr>



                    <?php } ?>
                    <tr>
                        <td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"></td>
                        <td colspan="2">
                            <input type="submit" value="Salvar">
                        </td>
                    </tr>
                </table>
            </form>
            <table width="100%">
                <tr>
                    <td style="text-align: center">
                        <h4>DADOS A SEREM COMPILADOS DO SAPE / SIGEF</h4>
                    </td>
                </tr>
            </table>
            <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td class="SubTituloDireita">
                        Data inicial
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Data final
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Valor Pactuado
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Valor FNDE
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Valor Entidade
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Datas pagamentos
                    </td>
                    <td  align="left">
                        <input type="text"  disabled="disable">
                    </td>
                </tr>

                <tr>
                    <td class="SubTituloDireita">
                        Valores pagos
                    </td>
                    <td  align="left">
                        <input type="text" disabled="disable">
                    </td>
                </tr>

            </table>


            <?php if ($mostra_relatorio) { ?>
                <table width="100%">
                    <tr>
                        <td style="text-align: center">
                            <h4>DADOS A SEREM INSERIDOS PELA ENTIDADE NO SIMEC</h4>
                        </td>
                    </tr>
                </table>

                <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
                    <tr>
                        <td class="SubTituloDireita">
                            Gestor(es) Responsável(is):
                        </td>
                        <td  align="left">


                            <?php $entid = ( $dados[empesfera] == 'E' ) ? recuperaEntidadePorTipo(7, $estuf) : recuperaEntidadePorTipo(7, null, $dados[muncod]); ?>
                            <?php
                            $sql = "select e.* from par.entidade e WHERE e.entid = {$entid}";
                            $entidade = $db->pegaLinha($sql);
                            ?>
                                                                        <!--<input type="text" value="<?php echo $entidade[entnome]; ?>" disabled="disable">-->

                            <?php echo "<li>" . $entidade[entnome] . "</li>"; ?>





                            <?php
                            $sql = "select * from obras2.cumprimentogestoresadicionais cga join entidade.entidade ent on cga.entid = ent.entid  where cooid = " . $dados_coo[cooid];
                            ?>
                            <?php $dados_gestores = $db->carregar($sql);
                            ?>

                            <?
                            if (!empty($dados_gestores)) {
                                foreach ($dados_gestores as $key) {
                                    echo "<br><li>" . $key[entnome] . "</li>";
                                }
                            }
                            ?>
                            <br>
                            <input type="hidden" name="entidvistoriador" id="entidvistoriador" value="">

                            <?php if ($habilita == "S") { ?>
                                <input type="button" name="pesquisar_entidade" value="Adicionar gestor" style="cursor: pointer;" onclick="inserirVistoriador(document.getElementById('entidvistoriador').value);"/>
                            <?php } ?>

                        </td>
                    </tr>
                </table>
                </form>
    <?php if ($habilita == "S") { ?>
                    <table width="100%">
                        <tr>
                            <td style="text-align: center">
                                <h4>Questionário</h4>
                            </td>
                        </tr>
                    </table>

                    <table class="listagem"cellSpacing="1" cellPadding="3" align="center">
                        <tr>
                            <td class="subtituloesquerda" colspan="8">Checklist</td>
                        </tr>

                        <tr>
                            <td class="subtituloesquerda" style="width: 65px;"> Ação </td>

                            <td class="subtituloesquerda"> Tipo </td>
                            <td class="subtituloesquerda"> Responsável </td>
                            <td class="subtituloesquerda"> Data de Inclusão </td>

                        </tr>
        <?php
        $campos = ' clc.clcid, 
                            clc.qrpid,
                            que.queid, 
                            usu.usunome, 
                            clc.clcdatainclusao, 
                            COALESCE(to_char(clc.clcdatainclusao, \'DD/MM/YYYY\')) AS clcdatainclusao';

        $sql = " SELECT {$campos} 
                        FROM obras2.checklistcumprimento                    clc
                        INNER JOIN questionario.questionarioresposta qrp ON clc.qrpid  = qrp.qrpid
                        INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                        INNER JOIN seguranca.usuario                 usu ON clc.usucpf = usu.usucpf
                        WHERE clc.clcstatus = 'A' 
                        AND clc.obrid       = " . $obrid . " 
                        {$where}
                        ORDER BY clc.clcdatainclusao
                         ";

        $lista = $db->carregar($sql);

        if (!empty($lista)) {
            $c = 0;
            foreach ($lista as $key => $value) {
                $c++;
                switch ($value['queid']) {
                    case QUEID_DOCUMENTOS_INSERIDOS:
                        $tipo = 'Documentos Inseridos no Simec/Processo';
                        $tipo_abrev = 'adm';
                        break;
                    case QUEID_DADOS_LICITACAO:
                        $tipo = 'Dados Licitação Original';
                        $tipo_abrev = 'tec';
                        break;
                    case QUEID_NOVA_LICITACAO:
                        $tipo = 'Dados da nova licitação';
                        $tipo_abrev = '2p';
                        break;
                    case QUEID_REFORMULACOES_IMPLEMENTADAS:
                        $tipo = 'Reformulações implementadas na obra';
                        $tipo_abrev = 'obr_vinc';
                        break;
                    case QUEID_REFORMULACAO_RECURSO:
                        $tipo = 'Reformulação de recusos adionais ao pactuado';
                        $tipo_abrev = 'obr_vinc';
                        break;
                    case QUEID_DOCUMENTOS_FINAIS:
                        $tipo = 'Documentos finais comprobatórios da obra';
                        $tipo_abrev = 'obr_vinc';
                        break;
                    default:
                        $tipo = 'Checklist - Modelo Antigo';
                        $tipo_abrev = 'old';
                        break;
                }
                $data = (!empty($value['htddata'])) ? $value['htddata'] : $value['ckfdatainclusao'];

                $server = $_SERVER['SERVER_NAME'];
                $endereco = $_SERVER ['REQUEST_URI'];
                $url = "http://" . $server . $endereco;

                echo '<tr>';
                echo '<td>';
                ?>
                                <?php if ($_REQUEST['qprid_aberto'] == $value['qrpid']) { ?>
                                    <a href="?modulo=principal/cadCumprimento&acao=A&obrid=<?php echo $_REQUEST['obrid'] ?>">
                                        <img src="/imagens/menos.gif" style="cursor: pointer;"  >
                                    </a>
                <?php } else { ?>
                                    <a href="<?php echo $url ?>&qprid_aberto=<?php echo $value['qrpid'] ?>">
                                        <img src="/imagens/mais.gif" style="cursor: pointer;"  >
                                    </a>
                <?php } ?>
                                <?php
                                echo ' </td>';

                                echo '  <td> ' . $tipo . ' </td>';
                                echo '  <td> ' . $value['usunome'] . ' </td>';
                                echo '  <td> ' . $value['clcdatainclusao'] . ' </td>';

                                echo '</tr>';

                                echo "<tr><td colspan='4'>";

                                if (!empty($_REQUEST['qprid_aberto'])) {
                                    if ($_REQUEST['qprid_aberto'] == $value['qrpid']) {

                                        $tela = new Tela(array('qrpid' => $value['qrpid'],
                                            'tamDivArvore' => 25,
                                            'tamDivPx' => 500,
                                            'habilitado' => "S"));
                                        echo "</td></tr>";
                                    }
                                }
                            }

                            echo '<tr> <td colspan="8" style="color: black; text-align: left; border-top: #000 2px solid"> <strong> Total de Registros: ' . $c . ' </strong> </td> </tr>';
                        } else {
                            echo '<tr> <td colspan="8" style="color: red; text-align: center;"> Não foram encontrados Registros. </td> </tr>';
                        }
                        ?>
                    </table>

    <?php } ?>
                <?php
                include_once APPRAIZ . "includes/classes/modelo/seguranca/Sistema.class.inc";
                include_once APPRAIZ . "includes/classes/modelo/questionario/QQuestionario.class.inc";
                include_once APPRAIZ . "includes/classes/modelo/questionario/QGrupo.class.inc";
                include_once APPRAIZ . "includes/classes/modelo/questionario/QPergunta.class.inc";
                include_once APPRAIZ . "includes/classes/modelo/questionario/QItemPergunta.class.inc";
                include_once APPRAIZ . "includes/classes/modelo/questionario/QResposta.class.inc";
                include_once APPRAIZ . "includes/classes/questionario/QImpressao.class.inc";
                include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";


                $campos = ' clc.clcid, 
                            clc.qrpid,
                            que.queid, 
                            usu.usunome, 
                            clc.clcdatainclusao, 
                            COALESCE(to_char(clc.clcdatainclusao, \'DD/MM/YYYY\')) AS clcdatainclusao';

                $sql = " SELECT {$campos} 
                        FROM obras2.checklistcumprimento                    clc
                        INNER JOIN questionario.questionarioresposta qrp ON clc.qrpid  = qrp.qrpid
                        INNER JOIN questionario.questionario         que ON qrp.queid  = que.queid
                        INNER JOIN seguranca.usuario                 usu ON clc.usucpf = usu.usucpf
                        WHERE clc.clcstatus = 'A' 
                        AND clc.obrid       = " . $obrid . " 
                        {$where}
                        ORDER BY clc.clcdatainclusao
                         ";

                $lista = $db->carregar($sql);



                foreach ($lista as $key => $value) {
                    echo "<br><br>";



                    $_REQUEST['queid'] = $value['queid']; //SOLUÇÃO ALTERNATIVA POR CONTA DA FORMA QUE O CONSTRUTOR DA CLASSE ABAIXO FOI IMPLEMENTADA
                    $obImprime = new QImpressao(array('cabecalho' => 'N', 'qrpid' => $value['qrpid']));
                    echo $obImprime->montaArvore();
                }
                ?>

                <?php ?>


            </td>


            <td width="10%" valign="top" style="padding-top: 30px;"><?php wf_desenhaBarraNavegacao($dados_coo[docid], array('cooid' => $dados_coo[cooid])); ?></td>
        </tr>
    </table>

    <form method="post" action=""  enctype="multipart/form-data">
        <input type="hidden" name="form_conclusao" value="1">
        <table width="100%">
            <tr>
                <td style="text-align: center">
                    <h4>DADOS A SEREM PREENCHIDOS PELO FNDE</h4>
                </td>
            </tr>
        </table>

    <?php
//VERIFICA SE A obras2.conclusaocumprimentoobjeto já tem um registro, caso não tenha é criada uma relação de 1 pra 1 usando o cooid.

    $sql = "select count(cco.cooid) from obras2.conclusaocumprimentoobjeto cco where cco.cooid = " . $dados_coo['cooid'];
    $qtd_registros = $db->pegaUm($sql);
    if ($qtd_registros == 0) {

        $sql2 = "insert into obras2.conclusaocumprimentoobjeto (cooid) values ({$dados_coo['cooid']})";
        $db->carregar($sql2);
    } else {
        $sql = "select * from obras2.conclusaocumprimentoobjeto cco where cco.cooid = " . $dados_coo['cooid'];
        $dados_cco = $db->pegaLinha($sql);
    }

    if ($_POST['form_conclusao'] == 1) {
        
        $file = new FilesSimec("conclusaocumprimentoobjeto", array('' => ''));
        if (is_file($_FILES["parecer_final_infra"]["tmp_name"])) {
            $arquivoSalvo1 = $file->setUpload($_FILES["parecer_final_infra"]["name"], "parecer_final_infra", NULL);
            $sql3 = "update obras2.conclusaocumprimentoobjeto set ccoparecerfinalinfra = {$file->getIdArquivo()} where cooid = " . $dados_coo['cooid'];
            
            $db->carregar($sql3);
        }
        if (is_file($_FILES["parecer_final_revisado"]["tmp_name"])) {
            $arquivoSalvo2 = $file->setUpload($_FILES["parecer_final_revisado"]["name"], "parecer_final_revisado", NULL);
            $sql3 = "update obras2.conclusaocumprimentoobjeto set ccoparecerfinalrevisado = {$file->getIdArquivo()} where cooid = " . $dados_coo['cooid'];
           
            $db->carregar($sql3);
        }
        if (is_file($_FILES["contraditorio"]["tmp_name"])) {

            $arquivoSalvo3 = $file->setUpload($_FILES["contraditorio"]["name"], "contraditorio", NULL);
            $sql3 = "update obras2.conclusaocumprimentoobjeto set ccocontraditorio = {$file->getIdArquivo()} where cooid = " . $dados_coo['cooid'];
            $db->carregar($sql3);
        }
    }
    ?>

        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td class="SubTituloDireita">Parecer Final Infra - Estrutura FNDE</td><td><input type="file" name="parecer_final_infra" style="float:left">
    <?php if (!empty($dados_cco['ccoparecerfinalinfra'])) { ?><a style="margin-top: 6px;margin-left: 10px; float: left" href="?modulo=principal/cadCumprimento&acao=A&obrid=<?php echo $obrid ?>&arquivo_download=<?php echo $dados_cco['ccoparecerfinalinfra'] ?>">Download</a><?php } ?>

                </td> 
            </tr>
            <tr>
                <td class="SubTituloDireita"> Parecer Final Revisado - Infra Estrutura FNDE</td><td><input type="file" name="parecer_final_revisado" style="float:left">
    <?php if (!empty($dados_cco['ccoparecerfinalrevisado'])) { ?><a style="margin-top: 6px;margin-left: 10px; float: left" href="?modulo=principal/cadCumprimento&acao=A&obrid=<?php echo $obrid ?>&arquivo_download=<?php echo $dados_cco['ccoparecerfinalrevisado'] ?>">Download</a><?php } ?>

                </td> 
            </tr>
            <tr>
                <td class="SubTituloDireita"> Contraditório</td><td><input type="file"  name="contraditorio" style="float:left">
    <?php if (!empty($dados_cco['ccocontraditorio'])) { ?><a style="margin-top: 6px;margin-left: 10px; float: left" href="?modulo=principal/cadCumprimento&acao=A&obrid=<?php echo $obrid ?>&arquivo_download=<?php echo $dados_cco['ccocontraditorio'] ?>">Download</a><?php } ?>
                </td> 
            </tr>
        </table>





        <table width="100%">
            <tr>
                <td style="text-align: center">
                    <h4>CONCLUSÃO DADOS A SEREM COMPILADOS PARA O SIGPC</h4>
                </td>
            </tr>
        </table>

        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">

            <tr>
                <td class="SubTituloDireita"><input name="aprovacao" type="checkbox" value="t"></td> <td>Aprovado 100% - Objetivo Cumprido</td> 
            </tr>
            <tr>
                <td class="SubTituloDireita">Data do recebimento do termo:</td><td> <input type="text" name="data_recebimento_termo"> </td>
            </tr>

            <tr>
                <td class="SubTituloDireita"><input name="reprovacao" type="checkbox" value="t"></td><td> Reprovado 100% - Objetivo Não Cumprido</td>
            </tr>

            <tr>
                <td class="SubTituloDireita"><input name="aprovacao_parcial" type="checkbox" value="t"></td><td> Aprovação parcial 100% - Objetivo Não Cumprido</td>
            </tr>
            <tr>
                <td class="SubTituloDireita"> % de cumprimento do objeto</td><td><input type="text" name="percentual_de_cumprimento"></td> 
            </tr>

            <tr>
                <td class="SubTituloDireita">Parecer Final - Infraestrutura FNDE</td><td><input type="file" name="parecer_final_infra_fnde"></td> 
            </tr>

            <tr>
                <td class="SubTituloDireita"> Parecer Final Revisado - Infraestrutura FNDE</td><td><input type="file" name="parecer_final_revisado_FNDE"></td> 
            </tr>
            <tr>
                <td class="SubTituloDireita"> Valor a Devolver ao Erário Público</td><td><input type="text" name="valor_a_devolver"></td> 
            </tr>
            <tr>
                <td class="SubTituloDireita"> Data da Ocorrência dos Eventos</td><td><input type="text" name="data_da_ocorrencia"> 
            </tr>
            <tr>
                <td class="SubTituloDireita"> </td><td><input type="submit">
            </tr>
        </table>



    </form>


    <script>
        $(document).ready(function() {

            $("#aaa").click(function() {

                $("#entidvistoriador").val("bbbbb");
                $("#entidvistoriador").trigger("change");
            });



            $1_11("#entidvistoriador").change(function() {
                console.log('asdada');

                var entid = $("#entidvistoriador").val();

                $1_11.post(window.location.href, {entid: entid, requisicao: "ajaxInserirGestor"}, function() {
                    location.reload();
                });
            });
        });
    </script>


    <?php
    if (!empty($_REQUEST['qprid_aberto'])) {
        ?>

        <script>
            $(document).ready(function() {
                var body = $("body");
                body.animate({scrollTop: 1600}, '1000');
            });

        </script>

    <?php } ?>
<?php } ?>