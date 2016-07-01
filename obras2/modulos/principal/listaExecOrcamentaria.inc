<?php
$empid = $_SESSION['obras2']['empid'];
$obrid = $_SESSION['obras2']['obrid'];

if(empty($obrid) && empty($empid)){
    die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
}

if($empid && empty($obrid)){
    $obr = new Obras();
    $obrid = $obr->pegaIdObraPorEmpid($empid);
    
    $obrid = (is_array($obrid) && count($obrid)) ? $obrid[0] : $obrid;
    
    if (empty($obrid)) {
        die("<script>
            alert('Faltam parâmetros para acessar esta tela!');
            location.href = '?modulo=inicio&acao=C';
         </script>");
    }
}

$obra1     = new Obras( $obrid );
$crtid    = $obra1->pegaContratoPorObra( $obrid );
$contrato = new Contrato( $crtid );

$dados1    = $contrato->getDados();

//dbg($dados, d);
if ( $dados1['crtid'] ){
      	$empresa 	= new Entidade( $dados1['entidempresa'] );        
	$entnomeempresa = "(". mascaraglobal($empresa->entnumcpfcnpj,"##.###.###/####-##") .") ".$empresa->entnome;

}

$empreendimento = new Empreendimento($empid);
$execOrcamento = new ExecucaoOrcamentaria();


include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

function uploadArquivosExecucaoOrc() {

    $arrRetornoIdsArquivos = array();
//    ver($_FILES,d);
    if ($_REQUEST['requisicao'] == 'salvar') {
        $arquivo_ordbanc    = $_FILES['arquivo_ordbanc'];
        $arquivo_notafiscal = $_FILES['arquivo_notafiscal'];
        $arquivo_boletim    = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    } elseif ($_REQUEST['requisicao'] == 'update') {
        $arquivo_ordbanc    = (isset($_FILES['arquivo_ordbanc'])) ? $_FILES['arquivo_ordbanc'] : array();
        $arquivo_notafiscal = (isset($_FILES['arquivo_notafiscal'])) ? $_FILES['arquivo_notafiscal'] : array();
        $arquivo_boletim    = (isset($_FILES['arquivo_boletim']) ? $_FILES['arquivo_boletim'] : array());
    }

    // Arquivo da ordem bancária
    if ($arquivo_ordbanc["name"] && $arquivo_ordbanc["type"] && $arquivo_ordbanc["size"] && $arquivo_ordbanc["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoordembancaria_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_ordbanc', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_ordembancaria'] = 'NULL';
        if($_REQUEST['requisicao'] == 'salvar'){
            die("<script>
                        alert('O arquivo Comprovante da Transferência é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo da nota fiscal
    if ($arquivo_notafiscal["name"] && $arquivo_notafiscal["type"] && $arquivo_notafiscal["size"] && $arquivo_notafiscal["error"] == 0) {
        $file2 = new FilesSimec();
        $file2->setPasta('obras2');
        $nome_arq2 = "anexonotafiscal_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file2->setUpload($nome_arq2, 'arquivo_notafiscal', false, false);
        $arqid2 = $file2->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = $arqid2;
    } else {
        $arrRetornoIdsArquivos['peoarqid_notafiscal'] = 'NULL';
        if($_REQUEST['requisicao'] == 'salvar'){
            die("<script>
                        alert('O arquivo da Nota Fiscal é obrigatório!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
    }

    // Arquivo do Boletim de Medição
    if ($arquivo_boletim["name"] != '' && $arquivo_boletim["type"] != '' && $arquivo_boletim["size"] != '' && $arquivo_boletim["error"] == 0) {
        $file = new FilesSimec();
        $file->setPasta('obras2');
        $nome_arq = "anexoboletimmedicao_" . $_SESSION['obras2']['obrid'] . '_' . time();
        $file->setUpload($nome_arq, 'arquivo_boletim', false, false);
        $arqid = $file->getIdArquivo();
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = $arqid;
    } else {
        $arrRetornoIdsArquivos['peoarqid_boletimmedicao'] = '';
    }
    
    return $arrRetornoIdsArquivos;
}

function getArrayDadosForm() {

    $arArquivos = uploadArquivosExecucaoOrc();
    $arDado = array();
    // Dados do formulário
    $arDado['obrid']                   = $_SESSION['obras2']['obrid'];
    $arDado['peodtpagamento']          = ajusta_data($_POST['peodtpagamento']);
    $arDado['peonrordembancaria']      = trim($_POST['peonrordembancaria']);
    $arDado['peovlordembancaria']      = str_replace(' ', '',MoedaToBd($_POST['peovlordembancaria']));
    $arDado['peodtnotafiscal']         = ajusta_data($_POST['peodtnotafiscal']);
    $arDado['peonrnotafiscal']         = trim($_POST['peonrnotafiscal']);
    $arDado['peovlnotafiscal']         = str_replace(' ', '',MoedaToBd($_POST['peovlnotafiscal']));
    $arDado['peovlordembancaria']      = (float)str_replace(' ', '',$arDado['peovlordembancaria']);
    $arDado['peovlnotafiscal']         = (float)str_replace(' ', '',$arDado['peovlnotafiscal']);

    $arDado['peopercentfispagamento']         = MoedaToBd($_POST['peopercentfispagamento']);
    $arDado['peopercentfisacumulado']         = MoedaToBd($_POST['peopercentfisacumulado']);

//    $arDado['peovlordembancaria']      = MoedaToBd($_POST['peovlordembancaria']);
//    $arDado['peodtnotafiscal']         = ajusta_data($_POST['peodtnotafiscal']);
//    $arDado['peonrnotafiscal']         = trim($_POST['peonrnotafiscal']);
//    $arDado['peovlnotafiscal']         = MoedaToBd($_POST['peovlnotafiscal']);
    // Dados dos arquivos
    $arDado['peoarqid_ordembancaria']  = $arArquivos['peoarqid_ordembancaria'];
    $arDado['peoarqid_notafiscal']     = $arArquivos['peoarqid_notafiscal'];
    $arDado['peoarqid_boletimmedicao'] = (($arArquivos['peoarqid_boletimmedicao'] != '') ? $arArquivos['peoarqid_boletimmedicao'] : 'NULL') ;
    // Dados de inclusão
    $arDado['peodtinclusao']           = date('Y-m-d');
    $arDado['peodtupdate']             = date('Y-m-d');
    $arDado['usucpf']                  = $_SESSION['usucpf'];
    $arDado['peostatus']               = 'A';

    return $arDado;
}

switch ($_REQUEST['requisicao']) {
    case 'salvar':
        //$arArquivos = uploadArquivosExecucaoOrc();
        $arDado = getArrayDadosForm();        
        try {

            $sql_insert = " 
                            INSERT INTO obras2.pagamentosexecucaoorcamentaria 
                            (obrid,
                             peodtpagamento,
                             peonrordembancaria,
                             peovlordembancaria,
                             peodtnotafiscal,
                             peonrnotafiscal,
                             peovlnotafiscal,
                             peoarqid_ordembancaria,
                             peoarqid_notafiscal,
                             peoarqid_boletimmedicao,
                             peodtinclusao,
                             peodtupdate,
                             usucpf,
                             peostatus,
                             peopercentfispagamento,
                             peopercentfisacumulado)
                            VALUES ( " . $arDado['obrid'] . ", 
                                    '" . $arDado['peodtpagamento'] . "',
                                    '0', 
                                     " . $arDado['peovlordembancaria'] . ",
                                    '" . $arDado['peodtnotafiscal'] . "', 
                                    '" . $arDado['peonrnotafiscal'] . "',
                                     " . $arDado['peovlnotafiscal'] . ",
                                     " . $arDado['peoarqid_ordembancaria'] . ",
                                     " . $arDado['peoarqid_notafiscal'] . ", 
                                     " . $arDado['peoarqid_boletimmedicao'] . ",
                                    '" . $arDado['peodtinclusao'] . "', 
                                    '" . $arDado['peodtupdate'] . "',
                                    '" . $arDado['usucpf'] . "', 
                                    '" . $arDado['peostatus'] . "',
                                    '" . $arDado['peopercentfispagamento'] . "',
                                    '" . $arDado['peopercentfisacumulado'] . "')
                     ";

            $db->executar($sql_insert);
            $db->commit();
            die("<script>
                        alert('Operação realizada com sucesso!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        } catch (Exception $ex) {
            $db->rollback();
            die("<script>
                        alert('Erro ao realizar a operação!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }

    case "apagar":
        try {
            $peoid = $_REQUEST['peoid'];
            $sql = " UPDATE obras2.pagamentosexecucaoorcamentaria SET peostatus = 'I' WHERE peoid = $peoid";
            $db->executar($sql);
            $db->commit();
            die("<script>
                        alert('Operação realizada com sucesso!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        } catch (Exception $ex) {
            $db->rollback();
            die("<script>
                        alert('Erro ao realizar a operação!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }

    case "download":
        include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
        $arqid = $_REQUEST['arqid'];
        $file = new FilesSimec();
        $arquivo = $file->getDownloadArquivo($arqid);
        die();

    case "update":
        $arDado = getArrayDadosForm();
        
        try {

            $set_ids_arquivos = '';

            if ($arDado['peoarqid_ordembancaria'] != 'NULL' && $arDado['peoarqid_ordembancaria'] != NULL && $arDado['peoarqid_ordembancaria'] != '') {
                $set_ids_arquivos .= ' peoarqid_ordembancaria = ' . $arDado['peoarqid_ordembancaria'] . ", ";
            }
            if ($arDado['peoarqid_notafiscal'] != 'NULL' && $arDado['peoarqid_notafiscal'] != NULL && $arDado['peoarqid_notafiscal'] != '') {
                $set_ids_arquivos .= ' peoarqid_notafiscal = ' . $arDado['peoarqid_notafiscal'] . ", ";
            }
            if ($arDado['peoarqid_boletimmedicao'] != 'NULL' && $arDado['peoarqid_boletimmedicao'] != NULL && $arDado['peoarqid_boletimmedicao'] != '') {
                $set_ids_arquivos .= ' peoarqid_boletimmedicao = ' . $arDado['peoarqid_boletimmedicao'] . ", ";
            }


            $sql_update = " 
                            UPDATE obras2.pagamentosexecucaoorcamentaria 
                            SET 
                                peodtpagamento     = '" . $arDado['peodtpagamento'] . "',
                                peonrordembancaria = '0',
                                peovlordembancaria =  " . $arDado['peovlordembancaria'] . ",
                                
                                peodtnotafiscal    = '" . $arDado['peodtnotafiscal'] . "',
                                peonrnotafiscal    = '" . $arDado['peonrnotafiscal'] . "',
                                peovlnotafiscal    =  " . $arDado['peovlnotafiscal'] . ",

                                peopercentfispagamento    =  " . $arDado['peopercentfispagamento'] . ",
                                peopercentfisacumulado    =  " . $arDado['peopercentfisacumulado'] . ",

                                " . $set_ids_arquivos . " 
                                
                                peodtupdate             = '" . $arDado['peodtupdate'] . "',
                                usucpf                  = '" . $arDado['usucpf'] . "',
                                peostatus               = 'A'

                            
                            WHERE
                             peoid = " . $_REQUEST['peoid'] . " ;
                            
                     ";

            $db->executar($sql_update);
            $db->commit();
            die("<script>
                        alert('Operação realizada com sucesso!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        } catch (Exception $ex) {
            $db->rollback();
            die("<script>
                        alert('Erro ao realizar a operação!'); 
                        window.location = '?modulo=principal/listaExecOrcamentaria&acao={$_GET['acao']}';
                 </script>");
        }
        break;
}

$param = array('teoid' => TIPO_EXEC_ORCAMENTARIA_OBRA);
if ($_GET['acao'] == 'A') {
    // empreendimento || obra || orgao
    verificaSessao('empreendimento');

    extract($execOrcamento->carregaPorEmpid($empid, $param));
    $msgTitle = (!$eorid ? 'Salve o orçamento para o empreendimento e então será liberado o cadastro de detalhamento do orçamento' : '');
    $displayEmpenhado = (!$eorid ? 'none' : '');
    $displayLiquidado = (!$eorid ? 'none' : '');
} else {
    // empreendimento || obra || orgao
    verificaSessao('obra');

    extract($execOrcamento->carregaPorObrid($obrid, $param));
    $msgTitle = (!$eorid ? 'Salve o orçamento para a obra e então será liberado o cadastro de detalhamento do orçamento' : '');
    $displayEmpenhado = (!$eorid ? 'none' : '');
    $displayLiquidado = (!$eorid ? 'none' : '');
}

if ($_GET['acao'] != 'V') {
    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    if ($_GET['acao'] == 'A') {
        $db->cria_aba(ID_ABA_EMP_CADASTRADO, $url, $parametros);

        $empreendimento->montaCabecalho();
    } else {
        if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
        } else {
            $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
        }

        echo cabecalhoObra($obrid);
    }

    $habilitado = true;
    $habilita = 'S';
} else {
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <?php
    $db->cria_aba($abacod_tela, $url, $parametros);
    echo cabecalhoObra($obrid);
//	criaAbaVisualizacaoObra();
//	$somenteLeitura = true;
    $habilitado = false;
    $habilita = 'N';
}

if (possui_perfil(array(PFLCOD_CONSULTA_UNIDADE, PFLCOD_CONSULTA_ESTADUAL, PFLCOD_CONSULTA_TIPO_DE_ENSINO))) {
    $habilitado = false;
    $habilita = 'N';
    $exclui = "       <img' +
                    '          align=\"absmiddle\"' +
                    '          src=\"/imagens/excluir.gif\"' +
                    '          style=\"cursor: pointer; margin-left: 3px;\"' +
                    '          onclick=\"javascript: excluirExec( ' + dadosEmEdicao.peoid + '  );\"' +
                    '          title=\"Excluir\">";
}
//demanda 311298 -- O perfil call center não pode alterar/inserir nenhuma informação.
if (possui_perfil(PFLCOD_CALL_CENTER)){
    $habilitado = false;
    $habilita = 'N';
    $exclui = "       <img' +
                    '          align=\"absmiddle\"' +
                    '          src=\"/imagens/excluir_01.gif\"' +
                    '          style=\"cursor: pointer; margin-left: 3px;\"' +
                    '          title=\"Excluir\">";
}

//$neorvlrtotal = $eorvlrcapital + $eorvlrcusteio;

monta_titulo($titulo_modulo, '</td></tr><tr><td style="background: #f00; color: #fff"><img src="/imagens/atencao.png" /> Todos os valores retirados da conta corrente da obra deverão estar discriminados documentados abaixo. Valores retirados da conta sem a inserção de documentação comprobatória (Notas Fiscais, Comprovantes de Pagamentos e Boletins de medição) resultar  em abertura de Tomada de Contas Especial.');
?>
<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<form name="formulario" method="post" id="formulario" action="" enctype="multipart/form-data">

    <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
    <input type="hidden" name="peoid"      id="peoid"      value="0"/>


    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td colspan="2" style="text-align: right"><a id="mostraDivDadosAntigos" href="#dados_antigos_div">[-] Esconde Dados Antigos</a></td>
        </tr>
        <tr>
            <td class="subtitulocentro" colspan="2">Pagamento</td>
        </tr>
         <tr>
            <td class="subtitulodireita" width="190px">Empresa Contratada:</td>
            <!--<td class="subtitulodireita" width="190px">Valor da OB (R$):</td>-->
            <td>
                <?php echo $entnomeempresa ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data da Transferência:</td>
            <td>
                <?php echo str_replace('size="12"', 'size="15"', campo_data2('peodtpagamento', 'S', true, '', 'S', '', ' ', '', '')); ?>
            </td>
<!--        <tr>
            <td class="subtitulodireita" width="190px">N° da ordem bancária:</td>
            <td>
                <?php //echo campo_texto('peonrordembancaria', 'S', 'false', '', 17, 35, '###########', '', 'left', '', 0, 'id="peonrordembancaria"', '', null, ''); ?>
            </td>
        </tr>-->
            
            
            
        <tr>
            <td class="subtitulodireita" width="190px">Valor da Transferência (R$):</td>
            <!--<td class="subtitulodireita" width="190px">Valor da OB (R$):</td>-->
            <td>
                <?php echo campo_texto('peovlordembancaria', 'S', false, '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="peovlordembancaria"', '', null, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Data da NF:</td>
            <td>
                <?php echo str_replace('size="12"', 'size="15"', campo_data2('peodtnotafiscal', 'S', true, '', 'S', '', ' ', '', '')); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Número da NF:</td> 
            <td>
                <?php echo campo_texto('peonrnotafiscal', 'S', 'false', '', 17, 35, '###########', '', 'left', '', 0, 'id="peonrnotafiscal"', '', null, ''); ?>
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Valor da NF (R$):</td>
            <td>
                <?php echo campo_texto('peovlnotafiscal', 'S', 'false', '', 17, 15, '###.###.###,##', '', 'left', '', 0, 'id="peovlnotafiscal"', '', null, ''); ?>
            </td>
        </tr>


        <tr>
            <td class="subtitulodireita" width="190px">Percentual de medição física correspondente a esse pagamento: </td>
            <td>
                <?php echo campo_texto('peopercentfispagamento', 'S', 'false', '', 17, 15, '###,##', '', 'left', '', 0, 'id="peopercentfispagamento"', '', null, ''); ?>
            </td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="190px">Percentual de medição física acumuluada até esse pagamento: </td>
            <td>
                <?php echo campo_texto('peopercentfisacumulado', 'S', 'false', '', 17, 15, '###,##', '', 'left', '', 0, 'id="peopercentfisacumulado"', '', null, ''); ?>
            </td>
        </tr>


        <tr>
            <td class="subtitulocentro" colspan="2">Arquivos Anexo</td>
        </tr>

        <tr>
            <td class="subtitulodireita" width="190px">Comprovante da Transferência:</td>
            <td id="td_arquivo_ordbanc">
                <input type="file" name="arquivo_ordbanc" id="arquivo_ordbanc">
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."> 
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Nota Fiscal:</td>
            <td id="td_arquivo_notafiscal">
                <input type="file" name="arquivo_notafiscal" id="arquivo_notafiscal">
                <img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."> 
            </td>
        </tr>
        <tr>
            <td class="subtitulodireita" width="190px">Boletim de Medição:</td>
            <td id="td_arquivo_boletim">
                <input type="file" name="arquivo_boletim" id="arquivo_boletim">
            </td>
        </tr>

        <tr bgcolor="#c0c0c0">
            <td colspan="2" style="text-align: center">
                <input type="button" value="Salvar"          id="registraExecOrc" style="cursor:pointer;"/>
                <input type="button" value="Cancelar Edição" id="cancelaEdicao"   style="display: none; cursor:pointer;"/>
            </td>
        </tr>

    </table>

</form>
<br />

<?php
        $objObras = new Obras($obrid);
        $blockEdicao = $objObras->verificaObraVinculada();
        if (possui_perfil(PFLCOD_CALL_CENTER) ){
            $blockEdicao = true;
        }
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#cancelaEdicao').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" id="listaPagamentosExecucaoOrc">
    <thead>
        <tr>
            <td class="subtitulocentro" colspan="10">Lista de Pagamentos</td>
        </tr>
        <tr>
            <th style="width: 50px">Ação</th>
            <th>Data</th>
            <!--<th>N° da Ordem Bancária</th>-->
            <th>Valor da Transferência (R$)</th>
            <th>Data da NF</th>
            <th>Número da NF</th>
            <th>Valor da NF (R$)</th>

            <th>Arquivo Comprovante da Transferência</th>
            <th>Arquivo Nota Fiscal</th>
            <th>Arquivo Boletim de Medição</th>

            <th>% medição física</th>
            <th>% medição física acumuluada</th>
        </tr>
    </thead>
    <tbody>
    <?php
    
    
        if($blockEdicao){
            $acao = "'<center>
                                        <img
                                                align=\"absmiddle\"
                                                src=\"/imagens/alterar.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: alterarExec(\'' || peo.peoid || '\');\"
                                                title=\"Alterar\">
                                  </center>'";
            
        }else{
            $acao = "'<center>
                                        <img
                                                align=\"absmiddle\"
                                                src=\"/imagens/alterar.gif\"
                                                style=\"cursor: pointer\"
                                                onclick=\"javascript: alterarExec(\'' || peo.peoid || '\');\"
                                                title=\"Alterar\">
                                        <img
                                                align=\"absmiddle\"
                                                src=\"/imagens/excluir.gif\"
                                                style=\"cursor: pointer; margin-left: 3px;\"
                                                onclick=\"javascript: excluirExec(\'' || peo.peoid || '\');\"
                                                title=\"Excluir\">
                                  </center>'";
        }
    

        $sql = "  
                                      SELECT 
                                        " . $acao . " as acao, 
                                        peoid,
                                        --obrid integer ,
                                        to_char(peodtpagamento, 'DD/MM/YYYY') as peodtpagamento,
                                        --peo.peonrordembancaria,
                                        peo.peovlordembancaria,
                                        to_char(peo.peodtnotafiscal, 'DD/MM/YYYY') as peodtnotafiscal,
                                        --peo.peodtnotafiscal ,
                                        peo.peonrnotafiscal ,
                                        peo.peovlnotafiscal ,

                                        peoarqid_ordembancaria,
                                        peoarqid_notafiscal,
                                        peoarqid_boletimmedicao,

                                        arq1.arqdescricao as arqdesc_ordembancaria,
                                        arq2.arqdescricao as arqdesc_notafiscal,
                                        arq3.arqdescricao as arqdesc_boletimmedicao,

                                        peo.peopercentfispagamento ,
                                        peo.peopercentfisacumulado

                                        --peodtinclusao,
                                        --peodtupdate,
                                        --usucpf,
                                        --peostatus ,

                                      FROM obras2.pagamentosexecucaoorcamentaria peo

                                      INNER JOIN arquivo arq1 on arq1.arqid = peo.peoarqid_ordembancaria
                                      INNER JOIN arquivo arq2 on arq2.arqid = peo.peoarqid_notafiscal
                                      LEFT  JOIN arquivo arq3 on arq3.arqid = peo.peoarqid_boletimmedicao

                                      WHERE
                                        peostatus = 'A'
                                      AND 
                                        obrid = ".$obrid."
                                       ";

        $result = $db->carregar($sql);
        $soma_ob = 0;
        $soma_nf = 0;
        $saldo = 0;
        if (!$result) {
            echo '<tr><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>
                      <td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td>
                      <td>&nbsp;</td>
                  </tr>';
        } else {
            foreach ($result as $k => $v) {
                $soma_ob += $v['peovlordembancaria'];
                $soma_nf += $v['peovlnotafiscal'];

                $soma_mf += $v['peopercentfispagamento'];
                $soma_mfa += $v['peopercentfisacumulado'];

                echo '<tr id="tr_' . $v['peoid'] . '">';
                echo '<td>' . $v['acao'] . '</td>';
                echo '<td>' . $v['peodtpagamento'] . '</td>';
//                echo '<td>' . $v['peonrordembancaria'] . '</td>';
                echo '<td>' . number_format($v['peovlordembancaria'], 2, ',', '.') . '</td>';
                echo '<td>' . $v['peodtnotafiscal'] . '</td>';
                echo '<td>' . $v['peonrnotafiscal'] . '</td>';
                echo '<td>' . number_format($v['peovlnotafiscal'], 2, ',', '.') . '</td>';

                echo '<td><a href="#" onclick="downloadArquivo(\'' . $v['peoarqid_ordembancaria'] . '\')">' . $v['arqdesc_ordembancaria'] . '</a></td>';
                echo '<td><a href="#" onclick="downloadArquivo(\'' . $v['peoarqid_notafiscal'] . '\')">' . $v['arqdesc_notafiscal'] . '</a></td>';
                echo '<td><a href="#" onclick="downloadArquivo(\'' . $v['peoarqid_boletimmedicao'] . '\')">' . $v['arqdesc_boletimmedicao'] . '</a></td>';

                echo '<td>' . number_format($v['peopercentfispagamento'], 2, ',', '.') . '</td>';
                echo '<td>' . number_format($v['peopercentfisacumulado'], 2, ',', '.') . '</td>';

                echo '</tr>';
            }
            $saldo = $soma_ob - $soma_nf;
        }
    ?>
    </tbody>
    <tfoot>
        <tr>
            <th>&nbsp;</th>
            <!--<th>&nbsp;</th>-->
            <th>Total Transferência:</th>
            <th id="th_soma_ob"><?php echo number_format($soma_ob, 2, ',', '.'); ?></th>
            <th>&nbsp;</th>
            <th>Total N.F.:</th>
            <th id="th_soma_nf"><?php echo number_format($soma_nf, 2, ',', '.'); ?></th>
            <th>&nbsp;</th>
            <th></th>
            <th></th>
            <th id="th_soma_mf"><?php echo number_format($soma_mf, 2, ',', '.'); ?></th>
            <th id="th_soma_mfa"><?php echo number_format($soma_mfa, 2, ',', '.'); ?></th>
            <!-- Removido a atendimento da demanda 177
            <th>Saldo:</th>
            <th id="th_saldo"><?php echo number_format($saldo, 2, ',', '.'); ?></th>
            -->
        </tr>
    </tfoot>
</table>

<div id="dados_antigos">
    <a href="dados_antigos_div"> <p>&nbsp;</p> </a>

    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr style="display: <?php echo $displayEmpenhado ?>;">
            <td class="subtitulocentro" colspan="2">
                Detalhamento Orçamentário Empenhado
            </td>
        </tr>
        <tr style="display: <?php echo $displayEmpenhado ?>;">
            <td colspan="2">

        <center>
            <div class="div_rolagem" style="width: 95%; height: 150px;" id="divDetalhamentoEmpenhado">
            <?php
                $param = $_POST;
                if ($_GET['acao'] == 'A') {
                    $param['empid'] = $_SESSION['obras2']['empid'];
                } else {
                    $param['obrid'] = $_SESSION['obras2']['obrid'];
                }
                $param['eorid'] = $eorid;
                $param['ioctipodetalhamento'] = TIPO_DETALHAMENTO_EXECUCAO_EMPENHADO;
                if ($habilitado == false) {
                    $param['block_imgs_acao'] = true;
                }
                $itemExecOrcamentaria = new ItensExecucaoOrcamentaria();
                $sql = $itemExecOrcamentaria->listaSql($param);

                $cabecalho = array("Data", "Valor Empenhado (R$)", "% Empenhado", "Arquivo", "Inserido por");
                $db->monta_lista($sql, $cabecalho, 100, 5, 'S', 'center', 'S');
            ?>
            </div>
        </center>

        </td>
        </tr>
        <tr style="display: <?php echo $displayLiquidado ?>;">
            <td class="subtitulocentro" colspan="2" style="border-top: 1px solid #000000;">
                Detalhamento Orçamentário Liquidado
            </td>
        </tr>
        <tr style="display: <?php echo $displayLiquidado ?>;">
            <td colspan="2">

        <center>
            <div class="div_rolagem" style="width: 95%; height: 150px;" id="divDetalhamentoLiquidado">
            <?php
                $param = $_POST;
                if ($_GET['acao'] == 'A') {
                    $param['empid'] = $_SESSION['obras2']['empid'];
                } else {
                    $param['obrid'] = $_SESSION['obras2']['obrid'];
                }
                $param['eorid'] = $eorid;
                $param['ioctipodetalhamento'] = TIPO_DETALHAMENTO_EXECUCAO_LIQUIDADO;
                if ($habilitado == false) {
                    $param['block_imgs_acao'] = true;
                }
                $itemExecOrcamentaria = new ItensExecucaoOrcamentaria();
                $sql = $itemExecOrcamentaria->listaSql($param);

                $cabecalho = array("Data", "Valor Liquidado (R$)", "% Liquidado", "Arquivo", "Inserido por");
                $db->monta_lista($sql, $cabecalho, 100, 5, 'S', 'center', 'S');
            ?>
            </div>
        </center>

        </td>
        </tr>	
        <tr bgcolor="#c0c0c0">
            <td>&nbsp;</td>
            <td>&nbsp;</td>
        </tr>
    </table>

</div>

<script type="text/javascript">

    $(document).ready(function() {
        $('.download').click(function() {
            var id = $(this).attr('id');
            downloadArquivo(id);
        });

        $("#mostraDivDadosAntigos").click(function() {
            $("#dados_antigos").toggle("fast", function() {
                if ($("#dados_antigos").is(':visible')) {
                    $("#mostraDivDadosAntigos").text('[-] Esconde Dados Antigos');
                } else {
                    $("#mostraDivDadosAntigos").text('[+] Mostra Dados Antigos');
                }
            });
        });

        $('#cancelaEdicao').click(function() {
            var str_tr = '';
            var str_acao = '';

            str_acao = '<center>' +
                    ' <img' +
                    '          align="absmiddle"' +
                    '          src="/imagens/alterar.gif"' +
                    '          style="cursor: pointer"' +
                    '          onclick="javascript: alterarExec( ' + dadosEmEdicao.peoid + ' );"' +
                    '          title="Alterar">' +
                    '<?php echo $exclui;?> ' +
                    '</center>';

            str_tr += '<tr id="tr_' + dadosEmEdicao.peoid + '">';
            str_tr += '  <td>' + str_acao + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peodtpagamento + '</td>';
//            str_tr += '  <td>' + dadosEmEdicao.peonrordembancaria + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peovlordembancaria + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peodtnotafiscal + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peonrnotafiscal + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peovlnotafiscal + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.arquivo_ordbanc + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.arquivo_notafiscal + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.arquivo_boletim + '</td>';

            str_tr += '  <td>' + dadosEmEdicao.peopercentfispagamento + '</td>';
            str_tr += '  <td>' + dadosEmEdicao.peopercentfisacumulado + '</td>';
            str_tr += '</tr>';

            $('#listaPagamentosExecucaoOrc > tbody:last').append(str_tr);

            $('#peodtpagamento').val('');
//            $('#peonrordembancaria').val('');
            $('#peovlordembancaria').val('');
            $('#peodtnotafiscal').val('');
            $('#peonrnotafiscal').val('');
            $('#peovlnotafiscal').val('');

            $('#td_arquivo_ordbanc').html('<input type="file" name="arquivo_ordbanc" id="arquivo_ordbanc"><img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."> ');
            $('#td_arquivo_notafiscal').html('<input type="file" name="arquivo_notafiscal" id="arquivo_notafiscal"><img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório."> ');
            $('#td_arquivo_boletim').html('<input type="file" name="arquivo_boletim" id="arquivo_boletim">');

            resetDadosEdicao();
            $('#cancelaEdicao').hide();

            atualizaValoresRodape();

        });

        var dadosEmEdicao = {
            peoid: 0,
            peodtpagamento: '',
//            peonrordembancaria: 0,
            peovlordembancaria: 0,
            peodtnotafiscal: '',
            peonrnotafiscal: 0,
            peovlnotafiscal: 0,
            arquivo_ordbanc: '',
            arqid_ordbanc: 0,
            arquivo_notafiscal: '',
            arqid_notafiscal: 0,
            arquivo_boletim: '',
            arqid_boletim: 0,

            peopercentfispagamento: 0,
            peopercentfisacumulado: 0
        };

        resetDadosEdicao = function() {
            dadosEmEdicao.peoid = 0;
            dadosEmEdicao.peodtpagamento = '';
//            dadosEmEdicao.peonrordembancaria = 0;
            dadosEmEdicao.peovlordembancaria = 0;
            dadosEmEdicao.peodtnotafiscal = '';
            dadosEmEdicao.peonrnotafiscal = 0;
            dadosEmEdicao.peovlnotafiscal = 0;
            dadosEmEdicao.arquivo_ordbanc = '';
            dadosEmEdicao.arqid_ordbanc = 0;
            dadosEmEdicao.arquivo_notafiscal = '';
            dadosEmEdicao.arqid_notafiscal = 0;
            dadosEmEdicao.arquivo_boletim = '';
            dadosEmEdicao.arqid_boletim = 0;

            dadosEmEdicao.peopercentfispagamento = 0;
            dadosEmEdicao.peopercentfisacumulado = 0;
        };

        validaArquivos = function() {
            if ($("#arquivo_ordbanc").val() === '') {
                alert('É obrigatório o envio do arquivo Comprovante da Transferência.');
//                alert('É obrigatório o envio do arquivo da Ordem Bancária.');
                return false;
            }
            if ($("#arquivo_notafiscal").val() === '') {
                alert('É obrigatório o envio do arquivo da Nota Fiscal.');
                return false;
            }
            if ($("#arquivo_notafiscal").val() !== '' && $("#arquivo_ordbanc").val() !== '') {
                return true;
            }
        };

        validaCampos = function() {

            if ($('#peopercentfispagamento').val() == '' || $('#peopercentfisacumulado').val() == '' || $('#peovlnotafiscal').val() == '' || $('#peonrnotafiscal').val() == '' || $('#peodtnotafiscal').val() == '' ||
                    $('#peovlordembancaria').val() == '' || $('#peodtpagamento').val() == '') {
//                    $('#peovlordembancaria').val() == '' || $('#peonrordembancaria').val() == '' || $('#peodtpagamento').val() == '') {
                return false;
            } else {
                return true;
            }
        };

        $('#registraExecOrc').click(function() {

            if ($('#requisicao').val() === 'update' && validaCampos()) {
                document.getElementById("formulario").submit();
            } else if ($('#requisicao').val() === 'salvar' && validaArquivos() && validaCampos()) {
                document.getElementById("formulario").submit();
            } else {
                alert('Existem campos obrigatórios em branco.');
            }

        });

        atualizaValoresRodape = function() {
            var vl_ob = 0;
            var vl_nf = 0;

            var vl_mf = 0;
            var vl_mfa = 0;

            var soma_vl_ob = 0;
            var soma_vl_nf = 0;
            var saldo = 0;

            $('#listaPagamentosExecucaoOrc > tbody  > tr').each(function() {

                var cellLength = this.cells.length;

                for (var y = 0; y < cellLength; y += 1) {
                    var cell = this.cells[y];
                    if (y === 3) {
                        vl_ob = cell.innerHTML;
                    }
                    if (y === 6) {
                        vl_nf = cell.innerHTML;
                        break;
                    }

                    if (y === 9) {
                        vl_mf = cell.innerHTML;
                        break;
                    }
                    if (y === 10) {
                        vl_mfa = cell.innerHTML;
                        break;
                    }
                }

                vl_ob = vl_ob.replace('.', '');
                vl_ob = vl_ob.replace(',', '.');
                vl_ob = parseFloat(vl_ob);

                vl_nf = vl_nf.replace('.', '');
                vl_nf = vl_nf.replace(',', '.');
                vl_nf = parseFloat(vl_nf);

                vl_mf = vl_mf.replace('.', '');
                vl_mf = vl_mf.replace(',', '.');
                vl_mf = parseFloat(vl_mf);

                vl_mfa = vl_mfa.replace('.', '');
                vl_mfa = vl_mfa.replace(',', '.');
                vl_mfa = parseFloat(vl_mfa);

                soma_vl_ob += vl_ob;
                soma_vl_nf += vl_nf;

                soma_vl_mf += vl_mf;
                soma_vl_mfa += vl_mfa;

            });

            saldo = soma_vl_ob - soma_vl_nf;

            soma_vl_ob = soma_vl_ob.toString().replace('.', ',');
            soma_vl_nf = soma_vl_nf.toString().replace('.', ',');

            soma_vl_mf = soma_vl_mf.toString().replace('.', ',');
            soma_vl_mfa = soma_vl_mfa.toString().replace('.', ',');

            saldo = saldo.toString().replace('.', ',');

            if (soma_vl_ob.search(',') == -1) {
                soma_vl_ob = soma_vl_ob + ',00';
            }
            if (soma_vl_nf.search(',') == -1) {
                soma_vl_nf = soma_vl_nf + ',00';
            }

            if (soma_vl_mf.search(',') == -1) {
                soma_vl_mf = soma_vl_mf + ',00';
            }
            if (soma_vl_mfa.search(',') == -1) {
                soma_vl_mfa = soma_vl_mfa + ',00';
            }

            if (saldo.search(',') == -1) {
                saldo = saldo + ',00';
            }

            $('#th_soma_ob').html(soma_vl_ob);
            $('#th_soma_nf').html(soma_vl_nf);

            $('#th_soma_mf').html(soma_vl_mf);
            $('#th_soma_mfa').html(soma_vl_mfa);

            $('#th_saldo').html(saldo);

        };


        alterarExec = function(peoid) {

            if (dadosEmEdicao.peoid !== 0) {
                alert('Já existe dados sendo editados, salve ou cancele antes de fazer uma nova edição.');
            } else {
                $('#requisicao').val('update');
                $('#peoid').val(peoid);

                $('#cancelaEdicao').show();

                dadosEmEdicao.peoid = peoid;
                dadosEmEdicao.peodtpagamento = $('#tr_' + peoid).children(":eq(1)").text();
//                dadosEmEdicao.peonrordembancaria = $('#tr_' + peoid).children(":eq(2)").text();
                dadosEmEdicao.peovlordembancaria = $('#tr_' + peoid).children(":eq(2)").text();
                dadosEmEdicao.peodtnotafiscal = $('#tr_' + peoid).children(":eq(3)").text();
                dadosEmEdicao.peonrnotafiscal = $('#tr_' + peoid).children(":eq(4)").text();
                dadosEmEdicao.peovlnotafiscal = $('#tr_' + peoid).children(":eq(5)").text();
                dadosEmEdicao.arquivo_ordbanc = $('#tr_' + peoid).children(":eq(6)").html();
                dadosEmEdicao.arquivo_notafiscal = $('#tr_' + peoid).children(":eq(7)").html();
                dadosEmEdicao.arquivo_boletim = $('#tr_' + peoid).children(":eq(8)").html();

                $('#peodtpagamento').val($('#tr_' + peoid).children(":eq(1)").text());
//                $('#peonrordembancaria').val($('#tr_' + peoid).children(":eq(2)").text());
                $('#peovlordembancaria').val($('#tr_' + peoid).children(":eq(2)").text());

                $('#peodtnotafiscal').val($('#tr_' + peoid).children(":eq(3)").text());
                $('#peonrnotafiscal').val($('#tr_' + peoid).children(":eq(4)").text());
                $('#peovlnotafiscal').val($('#tr_' + peoid).children(":eq(5)").text());

                $('#peopercentfispagamento').val($('#tr_' + peoid).children(":eq(9)").text());
                $('#peopercentfisacumulado').val($('#tr_' + peoid).children(":eq(10)").text());

                var str_modificar_ob = '<span style="cursor:pointer" id="mostraCampoArquivo_ob"> Modificar Arquivo. </a>';
                var str_modificar_nf = '<span style="cursor:pointer" id="mostraCampoArquivo_nf"> Modificar Arquivo. </a>';
                var str_modificar_bm = '<span style="cursor:pointer" id="mostraCampoArquivo_bm"> Modificar Arquivo. </a>';

                //Dados dos arquivos  
                $('#td_arquivo_ordbanc').html($('#tr_' + peoid).children(":eq(6)").html() + str_modificar_ob);
                $('#td_arquivo_notafiscal').html($('#tr_' + peoid).children(":eq(7)").html() + str_modificar_nf);
                $('#td_arquivo_boletim').html($('#tr_' + peoid).children(":eq(8)").html() + str_modificar_bm);

                $("#mostraCampoArquivo_ob").click(function() {
                    mostraCampoArquivoUpload('ob');
                });
                $("#mostraCampoArquivo_nf").click(function() {
                    mostraCampoArquivoUpload('nf');
                });
                $("#mostraCampoArquivo_bm").click(function() {
                    mostraCampoArquivoUpload('bm');
                });

                $('#tr_' + peoid).remove();

                atualizaValoresRodape();
            }

        };

        mostraCampoArquivoUpload = function(campo) {
            switch (campo) {
                case 'ob':
                    $('#td_arquivo_ordbanc').html('');
                    $('#td_arquivo_ordbanc').html('<input type="file" name="arquivo_ordbanc" id="arquivo_ordbanc">');

                    break;
                case 'nf':
                    $('#td_arquivo_notafiscal').html('');
                    $('#td_arquivo_notafiscal').html('<input type="file" name="arquivo_ordbanc" id="arquivo_notafiscal">');

                    break;
                case 'bm':
                    $('#td_arquivo_boletim').html('');
                    $('#td_arquivo_boletim').html('<input type="file" name="arquivo_ordbanc" id="arquivo_boletim">');
                    break;
            }
        };

    });

    downloadArquivo = function(arqid) {
        window.location = '?modulo=principal/listaExecOrcamentaria&acao=<?php echo $_GET['acao'] ?>&requisicao=download&arqid=' + arqid;
    }

    function excluirExec(peoid) {
        if (confirm('Deseja apagar este pagamento orçamentário?')) {
            window.location = '?modulo=principal/listaExecOrcamentaria&acao=<?php echo $_GET['acao'] ?>&requisicao=apagar&peoid=' + peoid;
        }
    }

</script>
