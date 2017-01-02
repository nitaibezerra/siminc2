<?php
ob_start();
if ($_REQUEST['pdlnl'] && $_REQUEST['requisicao'] == 'aviso') {
    echo verificaPdlnl($_REQUEST['pdlnl']);
    die();
}

function verificaPdlnl($pdlnl) {
    global $db;
    $sql = "select pdlnl from progorc.pedidolimite where pdlnl = '{$pdlnl}' AND to_char(pdldatapedido, 'YYYY') = '{$_SESSION['exercicio']}'";
    $pdlnl = $db->pegaUm($sql);

    if ($pdlnl) {
        $html = 'Esta Nota de Lançamento (NL) foi usada em um pedido cadastrado.';
    } else {
        $html = 'vazio';
    }
    return $html;
}

/**
 * Programação Orçamentária
 * @package simec
 * $Id: formulario.inc 99079 2015-06-23 19:36:03Z lindalbertofilho $
 */
//Download dos anexos
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";

if ($_REQUEST['download'] == 'S') {
    $file = new FilesSimec ();
    $arqid = $_REQUEST['arqid'];
    ob_clean();
    $arquivo = $file->getDownloadArquivo($arqid);
    exit();
}

include APPRAIZ . "www/recorc/_funcoes.php";

// -- Tratamento de perfis
$perfis = pegaPerfilGeral();

/* habilita ou desabilita o formulário conforme regras de acesso abaixo */
$habilitado = true;

if(isset($_POST['requisicao']) && $_POST['requisicao'] == 'pesquisaReferencia'){
    ob_clean();
    echo pesquisaProjecaoReferencia($_POST['unicod'],$_POST['tipo']);
    die;
}
if ($_POST['requisicaoAjax']) {
    ob_clean();
    header('content-type: text/html; charset=ISO-8859-1');
    $_POST['requisicaoAjax']();
    die;
}
if ('inserirAnalise' == $_REQUEST ['execucao']) {
    $stmt = '';
    $sucesso = true;
    $msg = 'Analise cadastrada com sucesso.';

    $sql = <<<DML
UPDATE progorc.pedidolimite
  SET pdlnl = %d
  WHERE pdlid = %d
  RETURNING pdlid
DML;
    $stmt = sprintf($sql, $_REQUEST['pdlnl'], $_REQUEST ['pdlid']);
    $pdlidA = $db->pegaUm($stmt);
    if (!$db->commit()) {
        $db->rollback();
        setFlashMessage(false, 'Não foi possível executar sua requisição.');
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&pdlid=' & $_REQUEST ['pdlid']);
        die();
    }

    if (empty($pdlidA)) { // -- Tanto o update qto o insert retornan o $pdlid, se não existir, eh pq a DML falhou
        setFlashMessage(false, "Ocorreu um evento inesperado durante a execução de sua requisição. Por favor, tente novamente.");
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&pdlid=' . $_REQUEST ['pdlid']);
        die();
    }

//
//recupera anexo para passar como parametro
    $sql = "SELECT distinct anx.anxcod, plm.docid
  FROM progorc.pedidodetalhe pdt
  inner join progorc.contacorrente ccr on ccr.ccrid = pdt.ccrid
  inner join progorc.anexo anx on anx.anxcod = ccr.anxcod
  inner join progorc.pedidolimite plm on  plm.pdlid = pdt.pdlid
  where plm.pdlid  = {$_REQUEST['pdlid']} ";

    $recuperaanexo = $db->pegaLinha($sql);

    $anexorecuperado = $recuperaanexo ['anxcod'];
    $docidrecuperado = $recuperaanexo ['docid'];

// -- Atualizando o status do workflow
    if ($_REQUEST['statusJustificativa'] == 'R') {
        $aedid = AEDID_ANALISE_RECUSADO;
    } else {
        $aedid = AEDID_ANALISE_APROVADO;
    }
    if ($_REQUEST['justificativaDesc'] == null || $_REQUEST['justificativaDesc'] == '') {
        $justificativa = "De acordo.";
    } else {
        $justificativa = $_REQUEST['justificativaDesc'];
    }


//ver($docidrecuperado, $aedid, $justificativa,d);
    wf_alterarEstado($docidrecuperado, $aedid, $justificativa, array());

    setFlashMessage($sucesso, $msg);
    ?>
    <script>window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid= <?php echo $_REQUEST ['pdlid'] ?>&anexo=<?php echo $anexorecuperado ?>';</script>
    <?php
    die();
}

if ('inserirSecretaria' == $_REQUEST ['execucao']) {
    $stmt = '';
    $sucesso = true;
    $msg = 'Analise cadastrada com sucesso.';

    if ($_REQUEST ['pdlaprovadoobras'] == 1) {
        $inputprova = $_REQUEST ['pdlaprovadoobras'];
    } else {
        $inputprova = 0;
    }
    //ver($_REQUEST ['pdldatavalidadeobras'] ,d);

    if ($_REQUEST ['pdldatavalidadeobras'] != '' || $_REQUEST ['pdldatavalidadeobras'] != null) {
        $dataobras = "'{$_REQUEST['pdldatavalidadeobras']}'";
    } else {
        $dataobras = 'null';
    }

    $sql = <<<DML
UPDATE progorc.pedidolimite
  SET
      pdldatavalidadeobras = %s,
      pdlaprovadoobras = '%s',
      pdlobservacaosecretaria = '%s'
  WHERE pdlid = %d
  RETURNING pdlid
DML;
    $stmt = sprintf($sql, $dataobras, $inputprova, $_REQUEST['justificativaSec'], $_REQUEST ['pdlid']);
//ver($stmt,d);
    $pdlidA = $db->pegaUm($stmt);
    if (!$db->commit()) {
        $db->rollback();
        setFlashMessage(false, 'Não foi possível executar sua requisição.');
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&pdlid=' & $_REQUEST ['pdlid']);
        die();
    }

    if (empty($pdlidA)) { // -- Tanto o update qto o insert retornan o $pdlid, se não existir, eh pq a DML falhou
        setFlashMessage(false, "Ocorreu um evento inesperado durante a execução de sua requisição. Por favor, tente novamente.");
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&pdlid=' & $_REQUEST ['pdlid']);
        die();
    }



    setFlashMessage($sucesso, $msg);
    ?>
    <script>window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid= <?php echo $_REQUEST ['pdlid'] ?>&anexo=<?php echo $anexorecuperado ?>';</script>
    <?php
    die();
}

if ('salvar' == $_POST['acao']) {

    $stmt = '';
    $sucesso = true;
    $msg = 'Requisição executada com sucesso.';

    if ($_SESSION['exercicio'] == '2015') {
        $_REQUEST['anexo'] = str_replace('/', '-', $_REQUEST['anexo']);
    }
    
    if ($_REQUEST['pdlreferencia'] == '' || $_REQUEST['pdlreferencia'] == 'undefined' || !isset($_REQUEST['pdlreferencia'])) {
        $referencia = "PROP";
    } else {
        $referencia = $_REQUEST['pdlreferencia'];
    }
    if (!($_POST['alterar'])) {
        if ($_REQUEST['pdlid']) {
            $verificar = "SELECT distinct anx.anxcod
  FROM progorc.pedidodetalhe pdt
  inner join progorc.contacorrente ccr on ccr.ccrid = pdt.ccrid
  inner join progorc.anexo anx on anx.anxcod = ccr.anxcod
  inner join progorc.pedidolimite plm on  plm.pdlid = pdt.pdlid
  where plm.pdlid  = '{$_REQUEST['pdlid']}'";
            $consulta1 = $db->pegaUm($verificar);
        }
        #ver($_POST['ccrid'],d);
        if (is_array($_POST['ccrid'][$_REQUEST['anexo']])) {
            foreach ($_POST['ccrid'][$_REQUEST['anexo']] as $c) {
                if ($consulta1 != $_REQUEST['anexo']) {

                    $atendido = STDOC_ATENDIDO;
                    $recusado = STDOC_RECUSADO;
                    $junta = STDOC_JUNTA_ORCAMENTARIA;

                    $sqlConsulta = "SELECT distinct plm.pdlid
                        FROM progorc.pedidolimite plm
                        inner join progorc.pedidodetalhe pdt on plm.pdlid = pdt.pdlid
                          LEFT JOIN
                          workflow.documento doc
                      ON
                          doc.docid = plm.docid
                        where plm.unicod = '{$_REQUEST['unicod']}' "
                            . " and plm.pdlreferencia = '$referencia' "
                            . " and plm.pdlescolha = '{$_REQUEST['escolha']}' "
                            . " and doc.esdid <> '{$atendido}' "
                            . " and doc.esdid <> '{$recusado}' "
                            . " and doc.esdid <> '{$junta}' "
                            . " and pdt.ccrid = {$c}";
#ver($sqlConsulta,$c, d);
                    $consulta = $db->pegaUm($sqlConsulta);
                    if (($consulta != null || $consulta != '')) {
                        setFlashMessage(false, 'Já possui um pedido de limite para está conta, favor aguardar o atendimento!');
                        ?>
                        <script>window.location = 'progorc.php?modulo=principal/limite/listar&acao=A';</script>
                        <?php
                        die();
                    }
                }
            }
        }
    }
    if ($_POST ['pdlid']) {

// -- update
        $sql = <<<DML
UPDATE progorc.pedidolimite
  SET unicod = %d,
      pdljustificativa = '%s',
      pdlreferencia = %s
  WHERE pdlid = %d
  RETURNING pdlid
DML;
        $stmt = sprintf($sql, $_POST ['unicod'], str_replace("'", "''", $_POST ['pdljustificativa']), "'" . $referencia . "'", $_POST ['pdlid']);
    } else {
        $docid = wf_cadastrarDocumento(TPDID_PROGORC_1, 'Fluxo de solicitação de Limite.');

// -- insert
        $sql = <<<DML
INSERT INTO progorc.pedidolimite(
    unicod,
    pdljustificativa,
    pdlreferencia,
    usucpf, docid, pdlescolha)
values (%d, '%s', '%s', '%s', %d, '%s')
RETURNING pdlid
DML;
        $stmt = sprintf($sql, $_POST ['unicod'], str_replace("'", "", $_POST ['pdljustificativa']), $referencia, $_SESSION ['usucpf'], $docid, $_POST ['escolha']);
    }
//ver($stmt,d);
    $pdlidR = $db->pegaUm($stmt);
    if (!$db->commit()) {
        $db->rollback();
        setFlashMessage(false, 'Não foi possível executar sua requisição.');
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $_POST ['unicod']);
        die();
    }

    if (empty($pdlidR)) { // -- Tanto o update qto o insert retornan o $pdlid, se não existir, eh pq a DML falhou
        setFlashMessage(false, "Ocorreu um evento inesperado durante a execução de sua requisição. Por favor, tente novamente.");
        header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $_POST ['unicod']);
        die();
    }

    $i = 0;
    if ($_REQUEST ['anexo']) {
        if (($_POST ['pdlid'] && !($_POST['alterar'])) && ($consulta1 != $_REQUEST['anexo'])) {
            ver($_REQUEST['anexo'],$_POST ['ccrid'], d);
            $queryDelete = "select pdt.pddid from progorc.pedidolimite plm inner join progorc.pedidodetalhe pdt on plm.pdlid = pdt.pdlid
            inner join progorc.contacorrente ccr on ccr.ccrid = pdt.ccrid
                where plm.pdlid = '{$_POST ['pdlid']}' and anxcod <> '{$_REQUEST['anexo']}'";
            $pddidDelete = $db->carregar($queryDelete);
//ver($queryDelete, $pddidDelete,d);
            foreach ($pddidDelete as $delete) {

                $sqlDelete = "delete from progorc.pedidodetalhe where pddid='%s'";
                $stmt = sprintf($sqlDelete, $delete['pddid']);
                $db->executar($stmt);
            }
            $db->commit();
        }
//ver($_REQUEST['anexo'],$_POST ['ccrid'], d);
        foreach ($_POST ['ccrid'] [$_REQUEST ['anexo']] as $c) {
            $sql = "select ccr.ccrid from progorc.contacorrente ccr inner join progorc.pedidodetalhe pdd on (ccr.ccrid = pdd.ccrid)
			where pdd.pdlid = {$pdlidR} and ccr.ccrid = $c";
            $possuidetalhe = $db->pegaUm($sql);

            $sqlD = "select pddvaloramplicacao,pddvalorreducao from progorc.contacorrente ccr inner join progorc.pedidodetalhe pdd on (ccr.ccrid = pdd.ccrid)
			where pdd.pdlid = {$pdlidR} and ccr.ccrid = $c";
            $pedidoD = $db->pegaLinha($sqlD);

            $amplicacao = $pedidoD ['pddvaloramplicacao'];
            $reducao = $pedidoD ['pddvalorreducao'];


            $valorAmpliacao = str_replace(".", "", $_POST ['ampliacao'] [$_REQUEST ['anexo']] [$i]);
            $valorAmpliacaoF = str_replace(",", ".", $valorAmpliacao);

            if ($valorAmpliacaoF == null || $valorAmpliacaoF == '') {
                $valorAmpliacaoF = '0.00';
            }
            $valorReducao = str_replace(".", "", $_POST ['reducao'] [$_REQUEST ['anexo']] [$i]);
            $valorReducaoF = str_replace(",", ".", $valorReducao);

            if ($valorReducaoF == null || $valorReducaoF == '') {
                $valorReducaoF = '0.00';
            }
            $valorAmpliacaoA = str_replace(".", "", $_POST ['ampliacaoatendido'] [$_REQUEST ['anexo']] [$i]);
            $valorAmpliacaoAF = str_replace(",", ".", $valorAmpliacaoA);

            if ($valorAmpliacaoAF == null || $valorAmpliacaoAF == '') {
                $valorAmpliacaoAF = '0.00';
            }
            $valorReducaoA = str_replace(".", "", $_POST ['reducaoatendido'] [$_REQUEST ['anexo']] [$i]);
            $valorReducaoAF = str_replace(",", ".", $valorReducaoA);

            if ($valorReducaoAF == null || $valorReducaoAF == '') {
                $valorReducaoAF = '0.00';
            }



//ver($c,$valorAmpliacaoF,$valorReducaoF, $valorAmpliacaoAF, $valorReducaoAF,d );

            if ($possuidetalhe) {
                $sql = <<<DML
select lmt.*, doc.esdid,  to_char(pdldatavalidadeobras,'dd/mm/yyyy')   as pdldatavalidadeobras from progorc.pedidolimite lmt LEFT JOIN workflow.documento doc USING(docid)  where pdlid = $pdlidR
DML;
                $pedidolimite = $db->pegaLinha($sql);

                $esdidS = $pedidolimite ['esdid'];


// -- update
                $sql = <<<DML
UPDATE progorc.pedidodetalhe
  SET  pddvaloramplicacao = %s,
      pddvalorreducao = %s,
	pddvaloramplicacaoatendido  = %s,
       pddvalorreducaoatendido  = %s
  WHERE pdlid = %d and ccrid = %s
  RETURNING pddid
DML;

                //
                //ver($esdidS,d);
                //if($esdidS == STDOC_ANALISE_SPO){
                //   $teste = "(".$valorAmpliacaoF." != ".$valorAmpliacaoAF.") || (".$valorReducaoF. "!=".$valorReducaoAF.")";
                //}
                //ver ($esdid,$teste,d);
                //if($amplicacao != $valorAmpliacaoF || $reducao != $valorReducaoF )
                if ($_POST['alterar'] || ($valorAmpliacaoF != $valorAmpliacaoAF) || ($valorReducaoF != $valorReducaoAF)) {
                    $stmt = sprintf($sql, $valorAmpliacaoF, $valorReducaoF, $valorAmpliacaoAF, $valorReducaoAF, $pdlidR, $c);
                } else {
                    $stmt = sprintf($sql, $valorAmpliacaoF, $valorReducaoF, $valorAmpliacaoF, $valorReducaoF, $pdlidR, $c);
                }
#ver($stmt, d);
//VER($_POST['alterar'],$stmt,d);
            } else {
// -- insert
                $sql = <<<DML
INSERT INTO progorc.pedidodetalhe(
    pdlid,
    pddvaloramplicacao,
    pddvalorreducao,
	pddvaloramplicacaoatendido,
    pddvalorreducaoatendido,
	ccrid)
values (%d,%s, %s, %s, %s, %s)
RETURNING pddid
DML;
                if ($_POST['alterar']) {
                    $stmt = sprintf($sql, $pdlidR, $valorAmpliacaoF, $valorReducaoF, $valorAmpliacaoAF, $valorReducaoAF, $c);
                } else {
                    $stmt = sprintf($sql, $pdlidR, $valorAmpliacaoF, $valorReducaoF, $valorAmpliacaoF, $valorReducaoF, $c);
                }
            }

            $pddid = $db->pegaUm($stmt);
            $i ++;
            if (!$db->commit()) {
                $db->rollback();
                setFlashMessage(false, 'Não foi possível executar sua requisição.');
                header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $_POST ['unicod']);
                die();
            }
        }
// -- Depois de gerar o novo PDLID, cria um número de documento para ele
        if (empty($pddid)) { // -- Tanto o update qto o insert retornan o $pdlid, se não existir, eh pq a DML falhou
            setFlashMessage(false, "Ocorreu um evento inesperado durante a execução de sua requisição. Por favor, tente novamente.");
            header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $_POST ['unicod']);
            die();
        }
    }

// -- Verifica se tem algum arquivo para salvar no servidor
    if (!empty($_FILES ["file"] ['name'])) {
// Array com as extensões permitidas
        $_UP ['extensoes'] = array(
            'pdf',
            'xls',
            'xlsx'
        );
// Faz a verificação da extensão do arquivo
        $extensao = strtolower(end(explode('.', $_FILES ['file'] ['name'])));
// ver($extensao, $_UP['extensoes'],$key,d);
        if (array_search($extensao, $_UP ['extensoes']) === false) {
            setFlashMessage(false, "Por favor, envie arquivos com a extensão pdf ou xls.");
            header('Location: ' . $_SERVER ['REQUEST_URI']);
            die();
        } elseif (0 != $_FILES ['file'] ['error']) {
            $uploadMaxFilesize = ini_get('upload_max_filesize');

            $errors = array(
                UPLOAD_ERR_OK => 'Arquivo carregado com sucesso.',
                UPLOAD_ERR_INI_SIZE => "O tamanho do arquivo é maior que o permitido. (Limite: {$uploadMaxFilesize})",
                UPLOAD_ERR_PARTIAL => 'Ocorreu um problema durante a transferência do arquivo.',
                UPLOAD_ERR_NO_FILE => 'O arquivo enviado estava vazio.',
                UPLOAD_ERR_NO_TMP_DIR => 'O servidor não pode processar o arquivo.',
                UPLOAD_ERR_CANT_WRITE => 'O servidor não pode processar o arquivo.',
                UPLOAD_ERR_EXTENSION => 'O arquivo recebido não é um arquivo válido.'
            );

            setFlashMessage(false, "Não foi possível salvar o arquivo enviado.<br />Motivo: {$errors[$_FILES['file']['error']]}");
            header('Location: ' . $_SERVER ['REQUEST_URI']);
            die();
        } else {
            $descricao = explode(".", $_FILES ['file'] ['name']);
            $campos = array(
                "angdsc" => "'" . $descricao [0] . "'",
                "pdlid" => $pdlidR
            );
            $file = new FilesSimec("anexogeral", $campos, "progorc");

            $arquivoSalvo = $file->setUpload($_FILES ['file'] ['name'], '', true);
        }
    }

    setFlashMessage($sucesso, $msg);
    ?>
    <script>window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid= <?php echo $pdlidR ?>&anexo=<?php echo $_REQUEST['anexo'] ?>';</script>
    <?php
//header ( 'Location: progorc.php?modulo=principal/limite/formulario&acao=A&execucao=lista&pdlid=' + $pdlidR );
    die();
}

if ($_REQUEST['execucao'] == 'lista') {
    global $db;
    $pdlid = $_REQUEST ['pdlid'];
    IF ($pdlid) {
        $sql = <<<DML
SELECT
	lmt.*,
	doc.esdid,
	to_char(pdldatavalidadeobras,'dd/mm/yyyy')   	AS pdldatavalidadeobras,
	unig.ungcod										AS ug,
    unig.gescod                                     AS gestao
FROM progorc.pedidolimite lmt
LEFT JOIN
	workflow.documento doc USING(docid)
LEFT JOIN
	progorc.uouggestao uog ON uog.unicod = lmt.unicod
LEFT JOIN
	public.unidadegestora unig ON unig.ungcod = uog.ungcod
WHERE pdlid = $pdlid

DML;
        $pedidolimite = $db->pegaLinha($sql);

        $uggestao = $pedidolimite['ug'] . ' / ' . $pedidolimite['gestao'];
        $unicod = $pedidolimite ['unicod'];
        $pdlreferencia = $pedidolimite ['pdlreferencia'];
        $pdljustificativa = $pedidolimite ['pdljustificativa'];
        $usucpfLimite = $pedidolimite ['usucpf'];
        $pdldatavalidadeobras = $pedidolimite ['pdldatavalidadeobras'];
        $pdlaprovadoobras = $pedidolimite ['pdlaprovadoobras'];
        $docid = $pedidolimite ['docid'];
        $esdid = $pedidolimite ['esdid'];
        $pdlnl = $pedidolimite ['pdlnl'];
        $pdlescolha = $pedidolimite ['pdlescolha'];
        $observacao = $pedidolimite ['pdlobservacaosecretaria'];




        $sqlUsuario = "SELECT
   usu.usucpf,
   usu.usunome,
   usu.usufoneddd,
   usu.usufonenum,
   usu.usuemail
FROM
   seguranca.usuario usu where usucpf = '$usucpfLimite'";
    } else {
        $sqlUsuario = "SELECT
   usu.usucpf,
   usu.usunome,
   usu.usufoneddd,
   usu.usufonenum,
   usu.usuemail
FROM
   seguranca.usuario usu where usucpf = '{$_SESSION['usucpf']}'";
    }

    /* desabilita para edição, caso seja UO e já tenha enviado */
// if (in_array ( PFL_UO_EQUIPE_TECNICA, $perfis ) && isset ( $capid )) {
// $habilitado = false;
// if (pegarEstadoAtual ( $capid ) == STDOC_EM_PREENCHIMENTO || pegarEstadoAtual ( $capid ) == STDOC_ACERTOS_UO) {
// $habilitado = true;
// }
// }

    /* Desabilita por Período, exeto para os em acertos UO */
// if ($habilitado && recuperaPeriodo ( 'atual' ) != $prfid && pegarEstadoAtual ( $capid ) != STDOC_ACERTOS_UO) {
// / $habilitado = false;
// }
    $dadosusuarios = $db->pegaLinha($sqlUsuario);
    $nome = $dadosusuarios ['usunome'];
    $ddd = $dadosusuarios ['usufoneddd'];
    $telefone = $dadosusuarios ['usufonenum'];
    $usucpf = $dadosusuarios ['usucpf'];
    $usuemail = $dadosusuarios ['usuemail'];
//ver($sqlUsuario,$nome,$ddd,$telefone,$usucpf,d);
// -- Redirecionando apenas se acabou de alterar alguma coisa na base
    if (!empty($_POST)) {
        header('Location: ' . $_SERVER ['REQUEST_URI']);
        die();
    }
}

function recuperaConta($anxcod, $esdidR, $pdlescolhaR) {
    $perfis = pegaPerfilGeral();
    $pdlidRe = $_REQUEST ['pdlid'];

    $habilitar = "readonly=readonly";

    /* Apenas para 2015 > */
    if ($_SESSION['exercicio'] == '2015') {
        $anxcod = str_replace('CA-IA', 'CA-IA', $anxcod);
    }

    if ($esdidR == null || $esdidR == '' || (in_array(PFL_SUPER_USUARIO, $perfis))) {
        $habilitar = '';
    }
    if (in_array(PFL_UO_EQUIPE_TECNICA, $perfis)) {
        if ($esdidR == STDOC_NAO_ENVIADO || $esdidR == STDOC_ACERTOS_SPO || $esdidR == STDOC_ACERTOS_SECRETARIA) {
            $habilitar = '';
        }
    }
    if (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
        if ($esdidR == STDOC_ANALISE_SPO) {
            $habilitar = '';
        }
    }
    if (in_array(PFL_SECRETARIA, $perfis)) {
        if ($esdidR == STDOC_ANALISE_SECRETARIA) {
            $habilitar = '';
        }
    }
    $mensagem = retornaMensagemQuadro($anxcod);
    $botao = retornaBotaoQuadro($anxcod, $pdlidRe);

    global $db;
    if ($pdlidRe) {
        $sql = <<<DML
            SELECT pddid FROM progorc.pedidodetalhe prr
            INNER JOIN progorc.contacorrente cont ON (prr.ccrid = cont.ccrid and prr.pdlid = $pdlidRe)
            WHERE cont.anxcod = '$anxcod' AND prr.pdlid = $pdlidRe
DML;
        $pdlidConta = $db->carregar($sql);
    }
    if ($pdlidConta) {
        $sql = <<<DML
            SELECT
                '<input type="hidden" name="ccrid[{$anxcod}][]" value="' || cont.ccrid ||'">' || cont.ccrcod AS conta,
                '<input type="text" style="text-align:right" {$habilitadoC} {$habilitadoI} '
                    || 'onmouseout="MouseOut(this)" onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control zerar" name="ampliacao[{$anxcod}][]" id="ampliacao_' || cont.ccrcod ||'" '
                    || 'value="' || TO_CHAR(prr.pddvaloramplicacao, '999G999G999G990D99') || '" '
                    || 'onchange="somaTotal(''ampliacao'',''{$anxcod}'',''pedido'')" {$habilitar}>' AS ampliacao,
                '<input type="text" style="text-align:right" {$habilitadoC} {$habilitadoI} onmouseout="MouseOut(this)" '
                    || 'onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control zerar" name="reducao[{$anxcod}][]" id="reducao_' || cont.ccrcod ||'" '
                    || 'value="' || TO_CHAR(prr.pddvalorreducao, '999G999G999G990D99') || '" '
                    || 'onchange="somaTotal(''reducao'',''{$anxcod}'',''pedido'')" {$habilitar}>' AS reducao,
                '<input type="text" style="text-align:right" onmouseout="MouseOut(this)" '
                    || 'onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control editavel zerar" name="ampliacaoatendido[{$anxcod}][]" '
                    || 'id="ampliacaoatendido_' || cont.ccrcod || '" '
                    || 'value="' || TO_CHAR(prr.pddvaloramplicacaoatendido, '999G999G999G990D99') || '" readonly="readonly" '
                    || 'onchange="somaTotal(''ampliacao'',''{$anxcod}'',''atendido'')">' AS ampliacaoAtendido,
                '<input type="text" style="text-align:right" onmouseout="MouseOut(this)" '
                    || 'onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control editavel zerar" name="reducaoatendido[{$anxcod}][]" '
                    || 'id="reducaoatendido_' || cont.ccrcod ||'" '
                    || 'value="' || TO_CHAR(prr.pddvalorreducaoatendido, '999G999G999G990D99') || '" readonly="readonly" '
                    || 'onchange="somaTotal(''reducao'',''{$anxcod}'',''atendido'')">' AS reducaoAtendido
            FROM progorc.contacorrente cont
            LEFT JOIN progorc.pedidodetalhe prr ON(prr.ccrid = cont.ccrid AND prr.pdlid = {$pdlidRe})
            WHERE cont.anxcod = '{$anxcod}' and prr.pdlid = {$pdlidRe}
            ORDER BY ccrcod
DML;
    } else {
        $sql = <<<DML
            SELECT DISTINCT
                cont.ccrcod,
                '<input style="text-align:right" type="hidden" name="ccrid[{$anxcod}][]" value="' || cont.ccrid || '">' || cont.ccrcod  AS conta,
                '<input style="text-align:right" type="text" {$habilitar}  onmouseout="MouseOut(this)" '
                    || 'onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value);somaTotal(''ampliacao'',''{$anxcod}'', ''pedido'')" '
                    || 'onmouseout="MouseOut(this)" onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control zerar" onchange="" '
                    || 'name="ampliacao[{$anxcod}][]" id="ampliacao_' || cont.ccrcod || '" value="0,00" >' AS ampliacao,
                '<input style="text-align:right" type="text" {$habilitar} onmouseout="MouseOut(this)" '
                    || 'onfocus="MouseClick(this);this.select()" onmouseover="MouseOver(this)" '
                    || 'onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value);somaTotal(''reducao'',''{$anxcod}'',''pedido'')" '
                    || 'class="form-control zerar" name="reducao[{$anxcod}][]" id="reducao_' || cont.ccrcod ||'" '
                    || 'value="0,00" onchange="" >' AS reducao,
                '<input style="text-align:right" type="text" onmouseout="MouseOut(this)" onfocus="MouseClick(this);this.select()" '
                    || 'onmouseover="MouseOver(this)" onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value);" '
                    || 'class="form-control editavel zerar" name="ampliacaoatendido[{$anxcod}][]" value="0,00" '
                    || 'id="ampliacaoatendido_' || cont.ccrcod ||'" readonly="readonly" '
                    || 'onchange="somaTotal(''ampliacao'',''{$anxcod}'',''atendido'')">' AS ampliacaoAtendido,
                '<input style="text-align:right" type="text" onmouseout="MouseOut(this)" onfocus="MouseClick(this);this.select()" '
                    || 'onmouseover="MouseOver(this)" onkeyup="this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'onblur="MouseBlur(this);this.value=mascaraglobal(''###.###.###.###,##'',this.value)" '
                    || 'class="form-control editavel zerar" name="reducaoatendido[{$anxcod}][]" value="0,00" '
                    || 'id=reducaoatendido_' || cont.ccrcod ||'" readonly="readonly" '
                    || 'onchange="somaTotal(''reducao'',''{$anxcod}'',''atendido'')">' AS reducaoAtendido
            FROM progorc.contacorrente cont
            LEFT JOIN progorc.pedidodetalhe prr ON(prr.ccrid = cont.ccrid)
            WHERE cont.anxcod = '{$anxcod}'
            ORDER BY ccrcod
DML;
    }
    echo <<<HTML
        <table class="table table-striped table-bordered table-hover">
            <thead>
                <tr>
                    <th colspan="5">$mensagem</th>
                </tr>
                <tr>
                    <th colspan="5">$botao</th>
                </tr>
                <tr>
                    <th class="text-center" rowspan="2" align="center">Conta Corrente</th>
                    <th class="text-center" colspan="2">Pedido (R$)</th>
                    <th class="text-center" colspan="2">Atendido (R$)</th>
                </tr>
                <tr>
                    <th class="text-center">Ampliar</th>
                    <th class="text-center">Reduzir</th>
                    <th class="text-center">Ampliar</th>
                    <th class="text-center">Reduzir</th>
                </tr>
            </thead>
            <tbody>
HTML;
    $anexoconta = $db->carregar($sql);
//ver($sql,d);
    if ($anexoconta) {
        foreach ($anexoconta as $conta) {
            if ($pdlescolhaR == 'C1' && $conta['conta'] == 'CA') {
                $habilitadoC = 'readonly=readonly';
            }
            if ($pdlescolhaR == 'I1' && $conta['conta'] == 'IA') {
                $habilitadoI = 'readonly=readonly';
            }
            echo <<<HTML
                <tr>
                    <td class="text-center">{$conta['conta']}</td>
                    <td>{$conta['ampliacao']}</td>
                    <td>{$conta['reducao']}</td>
                    <td>{$conta['ampliacaoatendido']}</td>
                    <td>{$conta['reducaoatendido']}</td>
                </tr>
HTML;
        }
    } else {
        echo <<<HTML
                <tr>
                    <td colspan="5">Não possui registros</td>
                </tr>
HTML;
    }
    echo <<<HTML
                <tr>
                    <td class="text-center"><b>Total:</b></td>
                    <td>
                        <input style="text-align:right" type="text" class="form-control total"
                            name="total" id="ampliacao_{$anxcod}_pedido" readonly="readonly" />
                    </td>
                    <td>
                        <input style="text-align:right" type="text" class="form-control total"
                               name="total" id="reducao_{$anxcod}_pedido" readonly="readonly" />
                    </td>
                    <td>
                        <input style="text-align:right" class="form-control totalatendido" type="text"
                               name="total" id="ampliacao_{$anxcod}_atendido" readonly="readonly" />
                    </td>
                    <td>
                        <input style="text-align:right" type="text" class="form-control totalatendido"
                               name="total" id="reducao_{$anxcod}_atendido" readonly="readonly" />
                    </td>
                </tr>
            </tbody>
        </table>
HTML;
}

function retornaMensagemQuadro($tipo)
{
    $mensagem = '';
    switch($tipo){
        case "I":
            $mensagem = <<<HTML
            Inclui todas as fontes, exceto 112, 150, 250, e suas correspondentes, resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
        case "II":
            $mensagem = <<<HTML
            Fontes 150, 250, e suas correspondentes, resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
        case "III":
            $mensagem = <<<HTML
            Fonte 112 e suas correspondentes resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
        case "IV":
            $mensagem = <<<HTML
            Organismos Internacionais, inclui recursos de todas as fontes e corresponde ao Programa 0910 - Operações Especiais: Gestão da Participação em Organismos Internacionais.
HTML;
            break;
        case "V":
            $mensagem = <<<HTML
            Programa de Aceleração e Crescimento ?PAC, inclui recursos de todas as fontes.
HTML;
            break;
        case "VI":
            $mensagem = <<<HTML
            Despesas obrigatórias, exclusive Benefícios a Servidores, Militares, Empregados e seus Dependentes. Inclui recursos de todas as fontes.
HTML;
            break;
        case "VII":
            $mensagem = <<<HTML
            Benefícios a Servidores, Militares, Empregados e seus Dependentes. Inclui recursos de todas as fontes.
HTML;
            break;
        case "UA":
            $mensagem = <<<HTML
            Benefícios a Servidores, Militares, Empregados e seus Dependentes. Inclui recursos de todas as fontes.
HTML;
            break;
        case "CA-IA-50":
            $mensagem = <<<HTML
            Inclui as fontes 150, 250 e suas correspondentes, resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
        case "CA-IA-80":
            $mensagem = <<<HTML
            Inclui as fontes 180, 280 e suas correspondentes, resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
        case "CA-IA-Demais":
            $mensagem = <<<HTML
            Inclui todas as fontes (exceto 150, 250, 180 e 280) e suas correspondentes, resultantes da incorporação de saldos de exercícios anteriores.
HTML;
            break;
    }
    return <<<HTML
        <p class="alert alert-danger" style="margin-bottom:0;">$mensagem</p>
HTML;
}

function retornaBotaoQuadro($tipo, $pdlidRe)
{
    if ( ($pdlidRe != null && $pdlidRe != '') || !(in_array($tipo, array('I','III','CA-IA-Demais'))) ) {
        return '';
    }
    $checked = '';
    switch($tipo){
        case "I":
            $escolha_c = array('valor' => 'C1', 'onchange' => 'I');
            $escolha_i = array('valor' => 'I1', 'onchange' => 'I');
            $escolha_r = array('valor' => 'R1', 'onchange' => 'I');
            break;
        case "III":
            $escolha_c = array('valor' => 'C3', 'onchange' => 'III');
            $escolha_i = array('valor' => 'I3', 'onchange' => 'III');
            $escolha_r = array('valor' => 'R3', 'onchange' => 'III');
            break;
        case "CA-IA-Demais":
            $escolha_c = array('valor' => 'C1', 'onchange' => 'I');
            $escolha_i = array('valor' => 'I1', 'onchange' => 'I');
            $escolha_r = array('valor' => 'R1', 'onchange' => 'I');
            $checked = "checked=checked";
            break;
    }
    return <<<HTML
        <div class="form-group" style="clear:left" id="escolha">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default">
                    <input type="radio" name="escolha" id="escolha_c" value="{$escolha_c['valor']}" onchange="escolhaFunction(this.value,'{$escolha_c['onchange']}');" >Custeio
                </label>
                <label class="btn btn-default">
                    <input type="radio" name="escolha" id="escolha_i" value="{$escolha_i['valor']}" onchange="escolhaFunction(this.value,'{$escolha_i['onchange']}');"/>Investimento
                </label>
                <label class="btn btn-default active" >
                    <input type="radio" name="escolha" id="escolha_r" value="{$escolha_r['valor']}" onchange="escolhaFunction(this.value,'{$escolha_r['onchange']}');" {$checked}/>Custeio e Investimento
                </label>
            </div>
        </div>
HTML;
}

if ($_REQUEST['execucao'] == 'apagar' && (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_UO_EQUIPE_TECNICA, $perfis))) {
    $acertosUO = STDOC_ACERTOS_SPO;
    $nenviado = STDOC_NAO_ENVIADO;
    $sqlConsulta = "
        SELECT
            distinct plm.pdlid
        FROM progorc.pedidolimite plm
        left join progorc.pedidodetalhe pdt on plm.pdlid = pdt.pdlid
        LEFT JOIN workflow.documento doc ON doc.docid = plm.docid
        WHERE plm.pdlid = '{$_REQUEST['pdlid']}'
            AND (doc.esdid = {$nenviado} OR doc.esdid = {$acertosUO})
    ";
#ver($sqlConsulta,d);
    $consulta = $db->pegaUm($sqlConsulta);
    if ($consulta != null || $consulta != '') {
        /* Apaga o Detalhe */
        $sql = <<<DML
DELETE FROM progorc.pedidodetalhe
WHERE pdlid = %d
DML;
        $stmt = sprintf($sql, $_REQUEST['pdlid']);

        $pdlidR = $db->pegaUm($stmt);
        if (!$db->commit()) {
            $db->rollback();
        }

        /* Apaga o Anexo */
        $sql = <<<DML
            SELECT arqid FROM progorc.anexogeral where pdlid = %d;
DML;
        $stmt = sprintf($sql, $_REQUEST['pdlid']);
        $loteArqid = $db->carregar($stmt);


        $sql = <<<DML
DELETE FROM progorc.anexogeral
WHERE pdlid = %d
DML;
        $stmt = sprintf($sql, $_REQUEST['pdlid']);
        $pdlidR = $db->pegaUm($stmt);
        if (!$db->commit()) {
            $db->rollback();
        } else {
            if ($loteArqid) {
                foreach ($loteArqid as $arqid) {
                    $simecF = new FilesSimec();
                    $simecF->excluiArquivoFisico($arqid['arqid']);
                }
            }
        }

        /* Apaga o Pedido */
        $sql = <<<DML
DELETE FROM progorc.pedidolimite
WHERE pdlid = %d
DML;
        $stmt = sprintf($sql, $_REQUEST['pdlid']);

        $pdlidR = $db->pegaUm($stmt);
        if (!$db->commit()) {
            $db->rollback();
            setFlashMessage(false, 'Não foi possível executar sua requisição.');
            #header('Location: progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + $_POST['unicod']);
            echo "<script>window.location = 'progorc.php?modulo=principal/limite/listar&acao=A&unicod=' + {$_POST['unicod']} ;</script>";
            die();
        } else {
            #header('Location: progorc.php?modulo=principal/limite/listar&acao=A&aviso=Registro Removido com Sucesso.');
            echo "<script>window.location = 'progorc.php?modulo=principal/limite/listar&acao=A&aviso=Registro Removido com Sucesso.';</script>";
        }
    } else {
        setFlashMessage(false, 'O pedido não pode mais ser excluído.');
        ?>
        <script>window.location = 'progorc.php?modulo=principal/limite/listar&acao=A';</script>
        <?php
        die();
    }
}

function apagarArquivos() {
    ob_clean();
    global $db;
    $arqid = $_REQUEST['arqid'];
    $simecF = new FilesSimec('anexogeral', array('angid', 'angdsc', 'arqid' => $_REQUEST['arqid'], 'angtip', 'pdlid', 'angtipoanexo'), 'progorc');

    $sql = "DELETE FROM progorc.anexogeral WHERE arqid = " . $arqid;
    $db->executar($sql);

    $sql = "DELETE FROM public.arquivo WHERE arqid = " . $arqid;
    $db->executar($sql);

    if ($db->commit())
        $simecF->excluiArquivoFisico($arqid);
    else
        return false;
    die;
}
?>
<style>
    #wfmenu{float:right;margin:15px}
    .table{font-size:11px}
    .input-group img,.form-group img{display:none}
</style>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script type="text/javascript" language="javascript">
            function voltarLista()
            {
                window.location = 'progorc.php?modulo=principal/limite/listar&acao=A&mes=<?php echo $_REQUEST['mes'] ?>'
                        + '&anexo=<?php echo $_REQUEST['anexo'] ?>'
                        + '&unicod=<?php echo $_REQUEST['unicod'] ?>';
            }

            /**
             * Função para não deixar preencher amplicação em Custeio e Investimento ao mesmo Tempo
             * para Anexos I e III
             */
            function validaCusteioInvestimento()
            {
                $('#ampliacao_IA').change(function () {
                    $('#ampliacao_CA').val('0,00');
                });
                $('#ampliacao_CA').change(function () {
                    $('#ampliacao_IA').val('0,00');
                });
                $('#reducao_IA').change(function () {
                    $('#reducao_CA').val('0,00');
                });
                $('#reducao_CA').change(function () {
                    $('#reducao_IA').val('0,00');
                });
                return true;
            }

            function escolhaFunction(campo, anexo)
            {
                validaCusteioInvestimento();
                if (campo == 'C3') {
                    $('#ampliacao_CD').prop('readonly', false);
                    $('#reducao_CD').prop('readonly', false);
                    $('#ampliacao_ID').prop('readonly', true);
                    $('#reducao_ID').prop('readonly', true);
                }
                if (campo == 'I3') {
                    $('#ampliacao_CD').prop('readonly', true);
                    $('#reducao_CD').prop('readonly', true);
                    $('#ampliacao_ID').prop('readonly', false);
                    $('#reducao_ID').prop('readonly', false);
                }
                if (campo == 'R3') {
                    $('#ampliacao_CD').prop('readonly', false);
                    $('#reducao_CD').prop('readonly', false);
                    $('#ampliacao_ID').prop('readonly', false);
                    $('#reducao_ID').prop('readonly', false);
                }
                if (campo == 'C1') {
                    $('#ampliacao_CA').prop('readonly', false);
                    $('#reducao_CA').prop('readonly', false);
                    $('#ampliacao_IA').prop('readonly', true);
                    $('#reducao_IA').prop('readonly', true);
                }
                if (campo == 'I1') {
                    $('#ampliacao_CA').prop('readonly', true);
                    $('#reducao_CA').prop('readonly', true);
                    $('#ampliacao_IA').prop('readonly', false);
                    $('#reducao_IA').prop('readonly', false);
                }
                if (campo == 'R1') {
                    $('#ampliacao_CA').prop('readonly', false);
                    $('#reducao_CA').prop('readonly', false);
                    $('#ampliacao_IA').prop('readonly', false);
                    $('#reducao_IA').prop('readonly', false);
                }
            }

            jQuery(document).ready(function () {
                $('#atualizar').click(function () {
                    if(verificaValorReferencia()){
                        window.location = 'progorc.php?modulo=principal/limite/formulario&acao=A&execucao=salvar';
                        document.getElementById("formPrincipal").submit();
                    }else{
                        return false;
                    }
                });
                escolhaFunction('C1', 'I');
                escolhaFunction('C3', 'III');
                // -- Correção paliativa do bug #9920 do bootstrap.
                // -- Correção agendada para a versão 3.0.3 do bootstrap.
                // -- <https://github.com/twbs/bootstrap/issues/9920>
                $("input:radio").change(function () {
                    $(this).prop('checked', true);
                });
                // -- Correção paliativa do bug #9920 do bootstrap.
                // -- Correção agendada para a versão 3.0.3 do bootstrap.
                // -- <https://github.com/twbs/bootstrap/issues/9920>

                $('#btnHabilitarEdicao').click(function () {
                    alert('Edição Habilitada.');
                    botoes = '<div class="form-group" align="center"><div class="col-lg-10"><button class="btn btn-danger" type="button" id="voltar" onclick="voltarLista()">Voltar</button> <button class="btn btn-info" id="atualizar">Gravar</button> </div></div>';
                    $('#botoesHabilitados').html(botoes);
                });
                // -- Selecionando alterar/concordar
<?php if (isset($captipo) && !empty($captipo)): ?>
                    $('#status_<?php echo $captipo; ?>').parent().addClass('active');
<?php endif; ?>
                // -- Selecionando o Status da análise
                $('#statusJustificativa_<?php echo $capretornosof; ?>').click();
            });
</script>
<?php
// -- Gerenciamento de abas
// -- Identificando a aba ativa
/* Troca os tipos para < 2015 e >= 2015 */
if ($_SESSION['exercicio'] < 2015) {
    $abaAtiva = (isset($_REQUEST['aba']) ? $_REQUEST['aba'] : 'pedido');
} else {
    $abaAtiva = (isset($_REQUEST['aba']) ? $_REQUEST['aba'] : 'pedido2015');
}
// -- URL base das abas
$requisicao['execucao'] = "&execucao={$_REQUEST['execucao']}";
$requisicao['pdlid'] = "&pdlid={$_REQUEST['pdlid']}";
$requisicao['anexo'] = "&anexo={$_REQUEST['anexo']}";
$params = implode('', $requisicao);
$urlBaseDasAbas = "/progorc/progorc.php?modulo=principal/limite/formulario&acao=A{$params}&aba=";

$listaAbas = array();

// -- Form principal do pedido
/* Troca os tipos para < 2015 e >= 2015 */
if ($_SESSION['exercicio'] < 2015) {
    $listaAbas[] = array('id' => 1, 'descricao' => 'Pedido', 'link' => "{$urlBaseDasAbas}pedido");
} else {
    $listaAbas[] = array('id' => 1, 'descricao' => 'Pedido', 'link' => "{$urlBaseDasAbas}pedido2015");
}
// -- Form de análise da Secretária
if (in_array(PFL_SECRETARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis)) {
    $listaAbas[] = array('id' => 2, 'descricao' => 'Análise Secretaria', 'link' => "{$urlBaseDasAbas}analise");
}
// -- Form de análise da SPO
if (($esdid == STDOC_ANALISE_SPO) && ($esdid != STDOC_RECUSADO || $esdid != STDOC_ATENDIDO) && (in_array(PFL_CGO_EQUIPE_ORCAMENTARIA, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis))) {
    $listaAbas[] = array('id' => 3, 'descricao' => 'Análise SPO', 'link' => "{$urlBaseDasAbas}spo");
}
// -- Form de análise final da SPO
if ($esdid == STDOC_RECUSADO || $esdid == STDOC_ATENDIDO) {
    $listaAbas[] = array('id' => 4, 'descricao' => 'Análise Final SPO', 'link' => "{$urlBaseDasAbas}final");
}
?>
<div class="row">
    <div class="col-md-12">
        <div class="bs-docs-section">
            <div class="row">
                <div class="col-lg-12">
                    <ol class="breadcrumb">
                        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
                        <li class="active"><a href="progorc.php?modulo=principal/limite/listar&acao=A">Pedidos -  <?php echo $_SESSION ['exercicio']; ?></a></li>
                        <li class="active">Pedido de Limite Orçamentário -  <?php echo $_SESSION ['exercicio']; ?></li>
                    </ol>
                </div>
                <?php
// -- HTML das abas
                echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
                echo getFlashMessage();
                $pdlid = $_GET['pdlid'];
                $usucpf = $_SESSION['usucpf'];
                ?>
                <div class="well col-lg-11">
                    <fieldset>
                        <?php
                        if ('spo' == $abaAtiva) {
                            $abaAtiva = strtoupper($abaAtiva);
                        } else {
                            $abaAtiva = ucfirst($abaAtiva);
                        }
                        require(dirname(__FILE__) . "/formulario/form{$abaAtiva}.inc");
                        ?>
                    </fieldset>
                </div>
                <div class="col-lg-1">
                    <?php
                    if ($docid) {
                        wf_desenhaBarraNavegacao($docid, array('pdlid' => $pdlid, 'usucpf' => $usucpf, 'usucpflimite' => $usucpfLimite));
                    }
                    ?>
                </div>
            </div>
        </div>
    </div>
</div>