<?php

//Critica colocada para solucionar um caso onde $_SESSION['obras2']['obrid'] estava populado com 2 obrid. $_SESSION['obras2']['obrid'] = 1001599 101757
if(strstr($_SESSION['obras2']['obrid'], ' ')){
    $obridTemp = trim($_SESSION['obras2']['obrid']);
    $obridTemp = explode(' ', $obridTemp);
    $_SESSION['obras2']['obrid'] = $obridTemp[0];
}

if(empty($_SESSION['obras2']['obrid']) && !empty($_GET['obrid'])){
    $obrid                       = (is_numeric($_GET['obrid'])) ? $_GET['obrid'] : 0;
    $_GET['obrid']               = $obrid;
    $_SESSION['obras2']['obrid'] = $obrid;
    $obr                         = new Obras($obrid);
    $_SESSION['obras2']['empid'] = $obr->empid;
}

if(empty($_SESSION['obras2']['orgid'])){
    $_SESSION['obras2']['orgid'] = 3;
}

if(empty($_SESSION['obras2']['empid'])){
    $obrid                       = (is_numeric($_GET['obrid'])) ? $_GET['obrid'] : 0;
    $obrid                       = (empty($obrid))              ? $_SESSION['obras2']['obrid'] : $obrid;
    $obr                         = new Obras($obrid);
    $_SESSION['obras2']['empid'] = $obr->empid;
    
    if(empty($_SESSION['obras2']['empid'])){
        $_SESSION['obras2']['empid'] = $_SESSION['obras2']['obrid'];
    }
}


// empreendimento || obra || orgao
verificaSessao('obra');

$tomid = '';

if(!empty($_REQUEST['requisicao'])){
    $anexoOs = new AnexoOsMi();
    if($_REQUEST['requisicao'] == 'delete'){
        $arqid = $_REQUEST['arqid'];
        $anexoOs->deletaAnexoPorArqid($arqid);
    } else {
        $anexoOs->salvaArquivosOs();
    }
}

if ($_REQUEST['requisicao'] && possui_perfil(array(PFLCOD_EMPRESA_MI_ADMINISTRATIVO))) {
    echo "<script>
                alert('Você não possui permissão!');
          </script>";
} else {

    switch ($_REQUEST['requisicao']) {

        case 'salvar':

            $tomid = ($_REQUEST['tomid'] != '') ? $_REQUEST['tomid'] : $_SESSION['tomid'];

            $ordemservico = new OrdemServicoMI();
            $os = $ordemservico->recuperarTodos("osmid, docid", array("osmstatus = 'A' AND obrid = " . $_SESSION['obras2']['obrid'], "tomid = " . $tomid), 'osmid ASC');
            $os = array_pop($os);
            if (!checaOsImplantacao($_SESSION['obras2']['obrid'])) {
                break;
            }

            if ($_POST['aceite'] == 'n') {
                $dados['docid'] = $os['docid'];
                $dados['osmid'] = $os['osmid'];
                $arrRecusa = $_POST['recusa'];
                $dados['usucpf'] = $_SESSION['usucpf'];
                $dados['orodtcadastro'] = formata_data_sql(date("d/m/Y"));

                foreach ($arrRecusa as $recusa) {

                    $dados['troid'] = $recusa;

                    $obrarecusa = new ObraRecusaOS();
                    $oroid = $obrarecusa->popularDadosObjeto($dados)->salvar();
                }
                $docid = pegaDocidObra($_SESSION['obras2']['obrid']);


                // Altera OS
                $alteraEstado = wf_alterarEstado($dados['docid'], 2081, 'OS Recusada', array('osmid' => $dados['osmid']));

            } else {

                // Faz a transição de datas
                $ordemservico = new OrdemServicoMI();
                $data = $ordemservico->carregarPorOsmid($os['osmid']);
                $sql = "UPDATE obras2.ordemservicomi SET
                            osmdtiniciomun = '{$data['osmdtinicio']}',
                            osmdtterminomun = '{$data['osmdttermino']}',
                            osmdtinicio = NOW(),
                            osmdttermino = NOW() + '{$data['osmprazo']} days'::interval WHERE osmid = {$os['osmid']}";
                $ordemservico->executar($sql);
                $ordemservico->commit();

                // Cria o cronograma com a data e valor
                $anexoOsMi = new AnexoOsMi();
                $dados = array();
                $dados['docid'] = $os['docid'];
                $dados['osmid'] = $os['osmid'];

                $alteraEstado = wf_alterarEstado($dados['docid'], 2082, 'OS Aceita', array('osmid' => $dados['osmid']));

                if ($tomid == 1) {
                    $ordemservico->atualizaContratoObraMi($_SESSION['obras2']['obrid']);
                }
            }

            $db->commit();

            echo "<script>
                    alert('Operação realizada com sucesso!');
                    window.opener.location = '?modulo=principal/listaObrasMI&acao=A';
                    window.close();
              </script>";
            exit();
            break;
        case 'arquivo':
            $ordemservico = new OrdemServicoMI();
            $tomid = ($_REQUEST['tomid'] != '') ? $_REQUEST['tomid'] : $_SESSION['tomid'];
            $os = $ordemservico->recuperarTodos("osmid, docid", array("osmstatus = 'A' AND obrid = " . $_SESSION['obras2']['obrid'], "tomid = " . $tomid), 'osmid ASC');

            $os = array_pop($os);
            $anexoOsMi = new AnexoOsMi();

            $dados = array();
            $dados = $anexoOsMi->getAnexoExecucao($os['osmid']);

            $arqid = null;
            // Laudo
            $dados = array();
            $dados = $anexoOsMi->getAnexoExecucaoSondagem($os['osmid']);
            $arqid = null;
            $arquivo = $_FILES["arquivo"];

            if ($arquivo["name"]) {
                if ($_FILES["arquivo"] && $arquivo["name"] && $arquivo["type"] && $arquivo["size"]) {
                    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, 'arquivo', false);
                    $arqid = $file->getIdArquivo();
                }

                $dados['osmid'] = $os['osmid'];
                $dados['arqid'] = $arqid;
                $dados['taoid'] = 1;

                $anexoOsMi->popularDadosObjeto($dados);

                $anexoOsMi->salvar();
                $anexoOsMi->commit();
            }

            die('<script>
                    location.href=\'?modulo=principal/popupAceiteOS&acao=A&tomid=' . $tomid . '\';
             </script>');
        case 'arquivoimplantacao':
            $tomid = ($_REQUEST['tomid'] != '') ? $_REQUEST['tomid'] : $_SESSION['tomid'];

            $ordemservico = new OrdemServicoMI();
            $os = $ordemservico->recuperarTodos("osmid, docid", array("osmstatus = 'A' AND obrid = " . $_SESSION['obras2']['obrid'], "tomid = " . $tomid), 'osmid ASC');
            $os = array_pop($os);
            $arquivos = $_FILES;
            $_FILES = array();
            $arqid = array();
            if (!empty($arquivos['arquivo']['name'][0])) {
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                foreach ($arquivos['arquivo']['name'] as $key => $file) {
                    $_FILES[$key] = array(
                        'name' => $arquivos['arquivo']['name'][$key],
                        'type' => $arquivos['arquivo']['type'][$key],
                        'tmp_name' => $arquivos['arquivo']['tmp_name'][$key],
                        'error' => $arquivos['arquivo']['error'][$key],
                        'size' => $arquivos['arquivo']['size'][$key],
                    );

                    $file = new FilesSimec(null, null, "obras2");
                    $file->setPasta('obras2');
                    $file->setUpload(null, $key, false);
                    $arqid = $file->getIdArquivo();
                    $anexoOsMi = new AnexoOsMi();

                    $dados['osmid'] = $os['osmid'];
                    $dados['taoid'] = $_POST['tipoarquivo'][$key];
                    $dados['arqid'] = $arqid;

                    $anexoOsMi->popularDadosObjeto($dados);
                    $anexoOsMi->salvar();
                    $anexoOsMi->commit();
                }
            }

            $qtdItens = new QtdItensComposicaoObraMi();
            $qtdItens->clearAll($_SESSION['obras2']['obrid']);

            $_POST['icmid'] = (empty($_POST['icmid'])) ? array() : $_POST['icmid'];

            foreach ($_POST['icmid'] as $key => $icmid) {
                $qtdItens = new QtdItensComposicaoObraMi();
                $dados = array(
                    'qioquantidade' => str_replace(',', '.', str_replace('.', '', $_POST['icmquantidade'][$key])),
                    'obrid' => $_SESSION['obras2']['obrid'],
                    'icmid' => $icmid,
                );
                $qtdItens->popularDadosObjeto($dados);
                $qtdItens->salvar();
                $qtdItens->commit();
            }

            die('<script>
                    location.href=\'?modulo=principal/popupAceiteOS&acao=A&tomid=' . $_REQUEST['tomid'] . '\';
             </script>');
        case 'removeArquivo':
            $anexoOs = new AnexoOsMi();
            $anexoOs->deletaAnexoPorArqid($_REQUEST['arqid']);
            die($_REQUEST['arqid']);
        case 'downloadArquivo':
            $arqid = $_GET['arqid'];
            if ($arqid) {
                include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
                $file = new FilesSimec(null, null, "obras2");
                $file->getDownloadArquivo($arqid);
            }

            die('<script>
                    window.history.go(-1);
             </script>');
    }

}
// ver($_REQUEST,$_POST,$_SESSION,d);
$_SESSION['acao']  = $_GET['acao']  != '' ? $_GET['acao']  : $_SESSION['acao'];
$_SESSION['tomid'] = $_GET['tomid'] != '' ? $_GET['tomid'] : $_SESSION['tomid'];

$tomid = $_SESSION['tomid'];
$obrid = $_SESSION['obras2']['obrid'];

// busca dados os
$ordemServico = new OrdemServicoMI();
$dados = $ordemServico->carregarPorObridETomid($obrid, $tomid);

if(!empty($_REQUEST['osmid'])){
    $dados = $ordemServico->carregarPorOsmid($_REQUEST['osmid']);
}

$chk_exec = '';
$chk_sond = '';
$chk_prim = '';

switch ($tomid) {
    case 1:
        $tp_os = 'de Execução';
        $chk_exec = 'checked="checked"';
        $habilitaCondicaoArqArtAceite = true;
        break;
    case 2:
        $tp_os = 'de Sondagem';
        $chk_sond = 'checked="checked"';
        $habilitaCondicaoArqArtAceite = false;
        break;
    case 3:
        $tp_os = 'do Projeto de Implantação';
        $chk_prim = 'checked="checked"';
        $habilitaCondicaoArqArtAceite = false;
        break;
}

if (count($dados) == 0) {

    die('<script type="text/javascript">
                alert(\'Não existe OS ' . $tp_os . '.\');
                if(history.length > 1){
                    window.history.go(-1);
                }else{
                    window.close();
                }
          </script>');
} 
else {

    $db->cria_aba(ID_ABA_OS_MI, $url, $parametros);
    echo cabecalhoObra($obrid);
    
    $osMi = new OrdemServicoMI();
    $imgs = $osMi->getBtnSinalizacaoOsMi($obrid);
    
    $htm = '
        <table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
                <tr>
                    <td class="SubTituloDireita" width="20%"><b>Tipo de OS:</b></td>
                    <td>
                          <input type="radio" value="2" name="radio_tipo_os" id="radio_tipo_os_sond" ' . $chk_sond . '> '.$imgs['sondagem'].   ' OS de Sondagem &nbsp; 
                          <input type="radio" value="3" name="radio_tipo_os" id="radio_tipo_os_prim" ' . $chk_prim . '> '.$imgs['implantacao'].' OS do Projeto de Implantação &nbsp;
                          <input type="radio" value="1" name="radio_tipo_os" id="radio_tipo_os_exec" ' . $chk_exec . '> '.$imgs['execucao'].   ' OS de Execução &nbsp;
                    </td>
                </tr>
        </table>        
        ';

    echo $htm;

    $estado = pegaEstadoOsMi($dados['docid']);
    monta_titulo("Dados da OS", "");
    if( possui_perfil(PFLCOD_CALL_CENTER)){
        $somenteLeitura = 'S';
        $desabilita = "disabled = 'disabled'";
    }else{
        $somenteLeitura = 'N';
    }
    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    
    <script type="text/javascript">
        
        jQuery.noConflict();
        jQuery('input:radio[name=radio_tipo_os]').click(function() {
           var tomid = jQuery("input:radio[name=radio_tipo_os]:checked" ).val(); 
           //Linha colocada para no caso de ser acionado o history.back, o radio correto estará marcado
           jQuery('input:radio[name=radio_tipo_os][value=<?php echo $tomid;?>]').attr('checked', true);           
           var url = 'obras2.php?modulo=principal/popupAceiteOS&acao=A&tomid='+tomid;
           window.location = url;
        });
                
    </script>
    
    <table align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
        <tbody>
            <tr>
                <td class="SubTituloDireita" width="20%"><b>Tipo:</b></td>
                <td>
                    <?php
                    $tipoOsMi = new TipoOsMi();
                    echo $tipoOsMi->resgataTipoOSMI($dados['tomid']);
                    ?>
                </td>
                <td style="float:right" rowspan="7" valign="top" align="center">
                    <?php
                    // Barra de estado WORKFLOW                    
                    if ($dados['docid']){
                        wf_desenhaBarraNavegacao($dados['docid'], array('osmid' => $dados['osmid']));
                    }
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" valign="top">Data da OS:</td>
                <td valign="top">
                    <?php
                    $osmdtcadastro = date("d/m/Y H:m:d", strtotime($dados['osmdtcadastro']));
                    echo $osmdtcadastro;
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" valign="top">Data de início da execução:</td>
                <td valign="top">
                    <?php
                    $osmdtinicio = formata_data($dados['osmdtinicio']);
                    echo $osmdtinicio;
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtitulodireita" valign="top">Prazo de execução (dias):</td>
                <td valign="top"><?php echo $dados['osmprazo'] ?></td>
            </tr>
            <tr>
                <td class="subtitulodireita" valign="top">Data de término da execução:</td>
                <td valign="top">
                    <?php
                    $osmdttermino = formata_data($dados['osmdttermino']);
                    echo $osmdttermino;
                    ?>
                </td>
            </tr>
            <tr>
                <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%" valign="top" rowspan="2">Ordem de Serviço:</td>
                <td valign="top">
                    <?php
                    $arquivo = new Arquivo($dados['arqid']);
                    if ($arquivo->arqid) {
                        echo "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                    }
                    ?>
                </td>
            </tr>
            <tr>
                <?php
                if ($dados['tomid'] == 3) {
                    $anexoOsMi = new AnexoOsMi();
                    $anexoOsMi = $anexoOsMi->getAnexoExecucaoImplantacao($dados['osmid']);
                    if (!empty($anexoOsMi) > 0) {
                ?>
                    <tr>
                        <td class="subtitulodireita" valign="top">Anexos da OS: </td>
                        <td>
                            <?php 
                                foreach ($anexoOsMi as $anexo) {
                                    $arquivo = new Arquivo($anexo['arqid']);
                                    if(empty($anexo['arqid']))continue; 
                            ?>
                                <p>
                                    <?php
                                        if ($dados['tomid'] == 3 && $estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO) {
                                            echo'<img style="float:left; margin-right:10px; cursor: pointer" src="/imagens/excluir.gif" alt="Excluir" title="Excluir" class="deleteArquivo" rel="' . $arquivo->arqid . '"/>';
                                        }
                                        echo "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>  (" . $anexo['taonome'] . ") " . $arquivo->arqnome . "." . $arquivo->arqextensao . " </a>";
                                    ?>
                                </p>
                            <?php } ?>
                        </td>
                    </tr>
                    <?php
                    }
                }
                ?>
            </tr>
            <?php
                if ( $estado != ESDID_OS_MI_EXECUCAO && $estado != ESDID_OS_MI_CORRECAO ){
                    showUploadOs($dados, $estado);
                }
                    
                if ( ($dados['tomid'] == 1) && ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO) ){
                    echo '<form name="anexoOs" id="anexoOs" method="post" action=""  enctype="multipart/form-data" >';
                    echo '<input type="hidden" name="requisicao" id="requisicao" value="" />';
                    showUploadOs($dados, $estado);
            ?>
            <tr bgcolor="#C0C0C0">
                <td></td>
                <td colspan="2">
                    <div style="float: left;">
                        <input type="button" value="Salvar" <?php echo $desabilita; ?> style="cursor: pointer" onclick="salvaArquivo();" style="cursor: pointer;"/>
                        <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                    </div>
                </td>
            </tr>
            <?php
                echo '</form>';
            }
            ?>
        </tbody>
    </table>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <?php
        if ($dados['tomid'] == 3):
            $read = 'N';
            $anexoOs = new AnexoOsMi();
            $tipoOs = new TipoAnexoOsMi();
            $anexoOs = $anexoOs->getAnexoExecucaoImplantacao($dados['osmid']);
    ?>
        <form name="laudoOSimplantacao" id="laudoOSimplantacao" method="post" action=""  enctype="multipart/form-data" >
            <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
            <?php $read = 'S'; ?>
                <input type="hidden" name="requisicao" id="requisicao" value="" />
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
                    <tbody>
                        <?php showUploadOs($dados, $estado);?>
                        <tr style="background: lightgray">
                            <td colspan="2" width="100%" align="center">
                                <label class="TituloTela" >Anexos da OS</label>
                            </td>
                        </tr>
                        <tr>
                            <td class="SubTituloDireita" style="vertical-align:top; width:25%">
                                <input type="button" id="addAnexo" value="Adicionar anexo"/>
                            </td>
                            <td style="vertical-align:top;">
                                <table id="anexosimplantacao">
                                    <tr class="anexosimplantacao">
                                        <td class="SubTituloDireita">
                                            <select name="tipoarquivo[]" id="">
                                                <?php foreach ($tipoOs->getTiposPorTomid($tomid) as $tipo): ?>
                                                    <option value="<?= $tipo['taoid'] ?>"><?= $tipo['taonome'] ?></option>
                                                <?php endforeach; ?>
                                            </select>
                                        </td>
                                        <td>
                                            <input type="file" name="arquivo[]" id="arquivo"/>
                                        </td>
                                    </tr>
                                </table>
                            </td>
                        </tr>

                    <?php endif; ?>
                    <?php
                    if ($estado != ESDID_OS_MI_AGUARDANDO_ACEITE):
                        $obra = new Obras($obrid);
                        $end = $obra->getEnderecoObra($obra->obrid);

                        $cronogramaPadrao = new Cronograma_PadraoMi();
                        $cronograma = $cronogramaPadrao->pegaCronogramaPadrao($end['estuf'], $obra->tpoid);
                        //var_dump($cronograma); die;
                        
                        if (!$cronograma) {
                            echo "<script type='text/javascript'>
                                alert('Esta obra não possuí serviços externos cadastrados!');
                                self.close();
                            </script>";
                            die;
                        }

                        $icm = new Itens_Composicao_PadraoMi();
                        $icplistForaEdificacao = $icm->pegaPorCpm($cronograma['cpmid'], 'F');

                        $sql = "SELECT
                                umdid as codigo,
                                umdeesc as descricao
                            FROM obras2.unidademedida
                            ORDER BY umdeesc ASC
                           ";
                        $unidadesDeMedida = $db->carregar($sql);
                        ?>
                        <tr><td colspan="2">
                                <div style="width:95%;margin-top:5px;padding:4px;text-align:center;font-weight:bold;margin: 0 auto;">Serviços Externos</div>
                                <table class="tabela listagem" style="text-align: center" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
                                    <thead>
                                        <tr>
                                            <td><b>Item</b></td>
                                            <td><b>Valor Unitário</b></td>
                                            <td><b>Quantidade</b></td>
                                            <td><b>Unidade de medida</b></td>
                                            <td><b>Valor Total</b></td>
                                        </tr>
                                    </thead>
                                    <tbody id="listaitensForaEdificacao">
                                    <?php
                                        foreach ($icplistForaEdificacao as $icm):
                                            $qtdItens = new QtdItensComposicaoObraMi();
                                            $qtdItem = $qtdItens->pegaPorObridEIcmid($obrid, $icm['icmid']);
                                    ?>
                                            <input type="hidden" value="<?= $icm['icmid'] ?>" name="icmid[]"/>
                                            <tr class='estruturapadraoForaEdificacao'>
                                                <td>
                                                    <?php
                                                        $itens = new ItensComposicao();
                                                        $db->monta_combo("itcid[]", $itens->listaCombo(), 'N', 'Selecione...', '', '', '', 200, 'S', '', '', $icm['itcid']);
                                                    ?>
                                                </td>
                                                <td>
                                                    <?php
                                                        $icmvalorconvertido = ($icm['icmvalor']) ? number_format($icm['icmvalor'], 2, ',', '.') : '';
                                                        echo campo_texto('icmvalor[]', 'N', 'N', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '', $icmvalorconvertido);
                                                    ?>
                                                </td>
                                                <td>
                                                    <?php echo campo_texto('icmquantidade[]', 'N', $read, '', 9, 30, '#######,##', '', 'right', '', 0, '', 'jQuery(\'input[name=icmquantidade\[\]]\').change();', number_format($qtdItem['qioquantidade'], 2, ',', '.')); ?>
                                                </td>
                                                <td>
                                                    <?php echo $db->monta_combo("umdid[]", $unidadesDeMedida, 'N', 'Selecione...', '', '', '', 200, 'N', '', '', $icm['umdid']); ?>
                                                </td>
                                                <td>
                                                    <?php
                                                        $icm['icmvalortotal'] = $qtdItem['qioquantidade'] * $icm['icmvalor'];
                                                        $icmvalortotalconvertido = ($icm['icmvalortotal']) ? number_format($icm['icmvalortotal'], 2, ',', '.') : '';
                                                        echo campo_texto('icmvalortotal[]', 'N', 'N', '', 9, 30, '####.###,##', '', 'right', '', 0, '', '', $icmvalortotalconvertido, '');
                                                    ?>
                                                </td>
                                        </tr>
                                    <?php endforeach; ?>
                                    </tbody>
                                    <tfoot>
                                        <tr>
                                            <td colspan="9" >
                                                Total: <input type="text" disabled="disabled" id="vltot" />
                                            </td>
                                        </tr>
                                    </tfoot>
                                </table>
            </td></tr>
            <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                <tr bgcolor="#C0C0C0">
                    <td></td>
                    <td>
                        <div style="float: left;">
                            <input type="button" value="Salvar" style="cursor: pointer" onclick="salvarArquivoImplantacao();" style="cursor: pointer;"/>
                            <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                        </div>
                    </td>
                </tr>
            <?php endif; ?>
            </tbody>
        </table>
        <?php endif; ?>
    </form>


    <?php endif; ?>


    <?php if ($dados['tomid'] == 2): ?>
        <?php
        $anexoOs = new AnexoOsMi();
        $anexoOs = $anexoOs->getAnexoExecucaoSondagem($dados['osmid']);
        ?>
        <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
            <form name="laudoOS" id="laudoOS" method="post" action=""  enctype="multipart/form-data" >
                <input type="hidden" name="requisicao" id="requisicao" value="" />
        <?php endif; ?>
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">
                <tbody>
                    <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                            <?php showUploadOs($dados, $estado); ?>
                    <?php endif; ?>
                    <tr>
                        <td class="SubTituloDireita" style="vertical-align:top; width:25%">Laudo de sondagem:</td>
                        <td>

                            <?php
                            $arquivo = new Arquivo($anexoOs['arqid']);

                            if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                                <input type="file" name="arquivo" id="arquivo" <?= $crtDisabled ?>/>
                                <br>
                            <?php endif; ?>
                            <?php
                            if ($arquivo->arqid) {
                                echo "<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>";
                            }
                            ?>
                        </td>
                    </tr>
                    <tr bgcolor="#C0C0C0">
                        <td></td>
                        <td>
        <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                                <div style="float: left;">
                                    <input type="button" value="Salvar" style="cursor: pointer" onclick="salvarArquivo();" style="cursor: pointer;"/>
                                    <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                                </div>
        <?php endif; ?>
                        </td>
                    </tr>
                </tbody>
            </table>
        <?php if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
            </form>
        <?php endif; ?>
    <?php endif; ?>
    
    <?php
    if ($estado == ESDID_OS_MI_AGUARDANDO_ACEITE && possui_perfil(array(PFLCOD_EMPRESA_MI_GESTOR))):
        monta_titulo("Aceitação de OS", "");
        ?>
        <form name="aceiteOS" id="aceiteOS" method="post" action="" enctype="multipart/form-data" >
            <input type="hidden" name="requisicao" id="requisicao" value="" />
            <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:0px;">

                <tr>
                    <td colspan=2>
                        <br /><br />
                        <span style="font-size: 12pt; " >Deseja aceitar a OS emitida para esta obra? </span>
                        <br /><br />
                        <input type="radio" name="aceite" id="aceita" value="s" /> <span style="font-size: 11pt; font-weight: bold;" > Sim </span><br/>
                        <input type="radio" name="aceite" id="rejeita" value="n" /> <span style="font-size: 11pt; font-weight: bold;" > Não </span><br/>

                    <tr class="aceita" style="display: none;">
                        <td class="SubTituloDireita" style="vertical-align:top; width:25%">Ordem de serviço Assinada:</td>
                        <td>
                            <input type="file" name="arquivo_s" id="arquivo"/>
                        </td>
                    </tr>
                    <?php
                        if($habilitaCondicaoArqArtAceite){
                    ?>
                        <tr class="aceita" style="display: none;">
                            <td class="SubTituloDireita" style="vertical-align:top; width:25%">ART/RRT:</td>
                            <td>
                                <input type="file" name="arquivo_art" id="arquivo_art"/>
                            </td>
                        </tr>
                    <?php
                        }
                    ?>
                <div id="recusa" style="display: none; margin: 30px;">
                    <p><span style="font-size: 11pt; font-weight: bold;" >Motivo da Recusa</span></p>
                    <br />
                    <?php
                    $tiporecusaos = new TipoRecusaOS();

                    $arTipoRecusa = $tiporecusaos->recuperarTodos();
                    if (is_array($arTipoRecusa)) {
                        foreach ($arTipoRecusa as $tipo) {
                            ?>
                            <input type="checkbox" name="recusa[]" value="<?php echo $tipo['troid']; ?>"> &nbsp; <?php echo $tipo['trodescricao']; ?> <br /><br />
                            <?php
                        }
                    }
                    ?>
                </div>
                </td>
                </tr>
                <tr bgcolor="#C0C0C0">
                    <td></td>
                    <td>
                        <div style="float: left;">
                            <input type="button" value="Salvar" style="cursor: pointer" onclick="salvar();" style="cursor: pointer;"/>
                            <input type="button" value="Fechar" style="cursor: pointer" onclick="window.close();">
                        </div>
                    </td>
                </tr>
            </table>
            <?php
            if (!checaOsImplantacao($obrid) && $dados['tomid'] == 1) {
                echo '<script type="text/javascript">alert("É necessário um OS de implantação concluida para aceitar a OS de execução."); jQuery("form#aceiteOS input:not([value=Fechar])").attr("disabled","disabled");</script>';
            }
            ?>
        </form>
        <?php
    endif;
}
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript">
    jQuery.noConflict();
    formatarNumeroMonetario = function(obj) {
        var valor = new String(obCalc.retiraZeroEsquerda(new String(obj.value)));
        if (valor.length == 2) {
            valor = '0' + valor;
        } else if (valor.length == 1) {
            valor = '00' + valor;
        } else if (valor.length == 0) {
            valor = '000';
        }

        obj.value = mascaraglobal("[###.]###,##", valor);
        return obj.value;
    }
    var obCalc = new Calculo();
    jQuery('input[name=icmquantidade\[\]]').change(function() {
        var qtd = jQuery(this).val();
        var vUnit = jQuery(this).parent().prev().find('input').val();
        valor = obCalc.operacao(vUnit, qtd, '*', 20);
        jQuery(this).parent().next().next().find('input').val(formatarNumeroMonetario({value: new Number(valor).toFixed(2)}));
        soma = 0;
        jQuery('input[name=icmvalortotal\[\]]').each(function() {
            valor = jQuery(this).val();
            soma = obCalc.operacao(valor, soma, '+', 20);
        })

        jQuery('input#vltot').val(formatarNumeroMonetario({value: new Number(soma).toFixed(2)}));
    });
    jQuery('input[name=icmquantidade\[\]]').change();
    jQuery('.deleteArquivo').click(function() {
        var arqid = jQuery(this).attr('rel');
        elm = jQuery(this);
        url = '/obras2/obras2.php?modulo=principal/popupAceiteOS&acao=A&requisicao=removeArquivo&arqid=' + arqid;
        jQuery.get(url, function() {
            jQuery(elm).parent().remove();
        });
    });
    jQuery('#addAnexo').click(function(e) {
        e.preventDefault();
        jQuery('table#anexosimplantacao').append('<tr>' + jQuery('.anexosimplantacao:eq(0)').html() + '</tr>');
    });
    jQuery('#rejeita').click(function() {
        jQuery("#recusa").css("display", "");
        jQuery(".aceita").hide();
    });
    jQuery('#aceita').click(function() {
        jQuery("#recusa").css("display", "none");
        jQuery(".aceita").show();
    });
    function salvarArquivo() {
        jQuery('#requisicao').val('arquivo');
        jQuery('#laudoOS').submit();
    }
    function salvaArquivo() {
        jQuery('#requisicao').val('anexoos');
        jQuery('#anexoOs').submit();
    }
    function salvarArquivoImplantacao() {
        jQuery('#requisicao').val('arquivoimplantacao');
        jQuery('#laudoOSimplantacao').submit();
    }
    function salvar() {
        var aceite = jQuery("input:radio[name='aceite']:checked").val();
        if (!aceite) {
            alert("Favor selecionar uma das opções de aceite para a OS");
            return false;
        }
        if (aceite == 'n') {
            var tiporecusa = jQuery("input:checkbox[name='recusa[]']:checked").val();
            if (!tiporecusa) {
                alert("É Obrigatória a escolha de pelo menos um motivo da Recusa");
                return false;
            }
        } else {
                var arquivo = jQuery('#arquivo').val();
                <?php if($habilitaCondicaoArqArtAceite){ ?>
                    var arquivoArt = jQuery('#arquivo_art').val();
                <?php } ?>
                var msg = '';
                if ( !arquivo ) {msg += 'Selecione a Ordem de Serviço\n';}
                <?php if($habilitaCondicaoArqArtAceite){ ?>
                    else if(!arquivoArt) {msg += 'Selecione o ART/RRT.\n';} 
                <?php } ?>
                if (msg) {
                    alert(msg);
                    return false;
                }
        }
        jQuery('#requisicao').val('salvar');
        jQuery('#aceiteOS').submit();
    }
    
    var somLeit = "<?php echo $somenteLeitura?>";
    if(somLeit == "S"){
        jQuery('#anexo_s').find('input[name=arquivo_s]').attr('disabled','disabled');
        jQuery('#anexo_art').find('input[name=arquivo_art]').attr('disabled','disabled');
    }
</script>
<?php
function checaOsImplantacao($obrid) {
    if ($_GET['tomid'] == 1 || $_SESSION['tomid'] == 1) {
        $osI = new OrdemServicoMI();
        $osI->carregarPorObridETomid($obrid, 3);

        if (!$osI->osmid){
            return false;
        }
        
        $esd = wf_pegarEstadoAtual($osI->docid);
        if ($esd['esdid'] == ESDID_OS_MI_CONCLUIDA) {
            return true;
        }
        return false;
    }
    return true;
}

function showUploadOs($dados, $estado){
    global $habilitaCondicaoArqArtAceite;
    $anexoOs    = new AnexoOsMi();
    $anexoOsAss = $anexoOs->getOsAssinada($dados['osmid']);
    $anexoOsArt = $anexoOs->getOsArt($dados['osmid']);
    
    ?>
        <tr>
            <td class="SubTituloDireita" style="vertical-align:top; width:25%">Ordem de serviço Assinada:</td>
            <td id="anexo_s">
                <?php
                if(!empty($anexoOsAss)){
                    $arquivo = new Arquivo($anexoOsAss['arqid']);
                }

                if ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO): ?>
                    <input type="file" name="arquivo_s" id="arquivo"/>
                    <br>
                <?php endif; ?>
                <?php
                if ($arquivo->arqid && !empty($anexoOsAss)) {
                    echo "
                    <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>
                    ";
                }
                ?>
            </td>
        </tr>
        <?php if($habilitaCondicaoArqArtAceite) {?>
        <tr>
            <td class="SubTituloDireita" style="vertical-align:top; width:25%">ART/RRT:</td>
            <td id="anexo_art">
                <?php
                if(!empty($anexoOsArt)){
                    $arquivo = new Arquivo($anexoOsArt['arqid']);
                }                
                if (($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO) && $habilitaCondicaoArqArtAceite == true): ?>
                    <input type="file" name="arquivo_art" id="arquivo"/>
                    <br>
                <?php endif; ?>
                <?php
                if ($arquivo->arqid && !empty($anexoOsArt)) {
                    echo "
                        <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>
                        ";
                }
                ?>
            </td>
        </tr>        
        <?php
        }
    if ( ($dados['tomid'] == 1) && ($estado == ESDID_OS_MI_EXECUCAO || $estado == ESDID_OS_MI_CORRECAO) ){
        $anexoTR  = $anexoOs->getAnexoExecucaoTermoRecebimento($dados['osmid']);
        $anexoPMA = $anexoOs->getAnexoExecucaoPlanilhaMedicaoAcumulada($dados['osmid']);        
        if($anexoTR){
            $input_arq_tr = '';
            $arquivo = new Arquivo($anexoTR['arqid']);
            if ($arquivo->arqid) {
                if($estado == ESDID_OS_MI_CORRECAO || $estado == ESDID_OS_MI_EXECUCAO){
                    $input_arq_tr = '<input type="file" name="arquivo_os_tr" id="arquivo_os_tr"/> <br>';
                }
                echo "<tr>
                        <td class=\"SubTituloDireita\" style=\"vertical-align:top; width:25%\">Termo de Recebimento:</td>
                        <td>
                        ".$input_arq_tr."<a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>
                         <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=delete&arqid={$arquivo->arqid}'><img class='link' src='../imagens/excluir.gif'></a>
                        </td>
                      </tr>";
            }
        }else{
            echo '<tr>
                    <td class="SubTituloDireita" style="vertical-align:top; width:25%">Termo de Recebimento:</td>
                    <td> <input type="file" name="arquivo_os_tr" id="arquivo_os_tr"/> </td>
                  </tr>';
        }        
        if($anexoPMA){
            $input_arq_pma = '';
            $arquivo = new Arquivo($anexoPMA['arqid']);
            if ($arquivo->arqid) {
                if($estado == ESDID_OS_MI_CORRECAO || $estado == ESDID_OS_MI_EXECUCAO){
                    $input_arq_pma = '<input type="file" name="arquivo_os_pma" id="arquivo_os_pma"/><br>';
                }
                echo "<tr>
                        <td class=\"SubTituloDireita\" style=\"vertical-align:top; width:25%\">Planilha de Medição Acumulada:</td>
                        <td> ".$input_arq_pma."
                        <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=downloadArquivo&arqid={$arquivo->arqid}'>(" . $arquivo->arqnome . "." . $arquivo->arqextensao . ")</a>
                        <a href='?modulo=principal/popupAceiteOS&acao=A&requisicao=delete&arqid={$arquivo->arqid}'><img class='link' src='../imagens/excluir.gif'></a>
                        </td>
                      </tr>";
                
            }
        }else{
            echo '<tr>
                    <td class="SubTituloDireita" style="vertical-align:top; width:25%">Planilha de Medição Acumulada:</td>
                    <td> <input type="file" name="arquivo_os_pma" id="arquivo_os_pma"/> </td>
                  </tr>';
        }        
    }    
}

?>