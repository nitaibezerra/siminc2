<?php

/**
 * Listar arquivos anexados de acordo com o tipo
 * @return array|null
 */
function listarAnexos($tipo, $pdcid) {
    global $db;

    if (!$tipo || !$pdcid) {
        return null;
    }

    $strSQL = "
        select
	      ag.arqid, ag.pdcid, to_char(ag.arpdtinclusao, 'DD/MM/YYYY') as datainclusao, pa.arqtamanho,
	      pa.arqnome || '.' || pa.arqextensao as descricao, su.usunome
        from sicaj.anexogeral ag
        inner join public.arquivo pa on (pa.arqid = ag.arqid)
        inner join seguranca.usuario su on (su.usucpf = pa.usucpf)
        where ag.angtipoanexo = '{$tipo}' and ag.pdcid = {$pdcid};
    ";
    //ver($strSQL, d);
    $results = $db->carregar($strSQL);
    return ($results) ? $results : null;
}

/**
 * Verifica se existe tipo na base de dados para o processo ativo
 * @return array|null
 */
function existeArquivoTipo($tipo, $pdcid) {
    global $db;

    if (!$tipo || !$pdcid) {
        return null;
    }

    $strSQL = "select * from sicaj.anexogeral ag where ag.angtipoanexo = '%s' and ag.pdcid = %d";
    $result = $db->pegaLinha(sprintf($strSQL, (string) $tipo, (int) $pdcid));
    return ($result) ? $result : null;
}

/**
 * Imprime dados anexos por pedido e tipo de anexo
 * @param $tipo
 * @return string
 */
function imprimeTabelaAnexos($tipo, $pdcid) {

    $results = listarAnexos($tipo, $pdcid);
    if (!$results) {
        return '<tr>
            <td colspan="5" align="center">
                <strong>Nenhum registro encontrado</strong>
            </td>
        </tr>';
    }

    $html = '';
    foreach ($results as $row) {
        $html.= "<tr>
            <td>
                <a href='#{$row['arqid']}' class='_download_'>
                    <span class='glyphicon glyphicon-download-alt'></span>
                </a>";

        if (allowClear($pdcid, $_SESSION['usucpf'])) {
            $html.= "<a href='#{$row['arqid']}' class='_delete_'>
                    <span class='glyphicon glyphicon-trash'></span>
                </a>";
        }

        $arqtamanho = by2M($row['arqtamanho']);

        $html.= "</td>
            <td class='text-left'>{$row['descricao']}</td>
            <td class='text-left'>{$arqtamanho}</td>
            <td class='text-left'>{$row['datainclusao']}</td>
            <td class='text-left'>{$row['usunome']}</td>
        </tr>";
    }

    return $html;
}

/**
 * Pega o docid de um pedido
 * @param $pdcid
 */
function pegaDocid($pdcid) {
    global $db;

    $strSQL = "select docid from sicaj.pedidocdo where pdcid = %d";
    return $db->pegaUm(sprintf($strSQL, (int) $pdcid));
}

/**
 * Recupera todos os dados de um pedido
 * para popular o formulário
 */
function pegaDadosPedido($pdcid) {
    global $db;

    if (!$pdcid)
        return null;

    $strSQL = "
      SELECT
        pdcid, cdo.unicod, pdcnumprocessojudicial,
        pdccodacaojudicial, pdcjuizodacao, pdccodobjeto,
        pdcnumbeneficioacao, TO_CHAR(pdcdatainicio, 'DD/MM/YYYY') AS pdcdatainicio,
        TO_CHAR(pdcdatacadastro, 'DD/MM/YYYY') AS pdcdatacadastro, pdcobservacao,
        pdcstatus, cdo.usucpf, pdcvalorcdo, pdcvalorexecatual, pdcvalorexecpassado,
        (coalesce(pdcvalorexecatual, 0) + coalesce(pdcvalorexecpassado,0)) as total_cdo,
        numprocessoadm, pdctipo, usunome, usuemail, usufoneddd, usufonenum
      FROM sicaj.pedidocdo cdo
        LEFT JOIN seguranca.usuario usu ON(respcpf = usu.usucpf)
      WHERE pdcid = %d
    ";

    $dados = $db->pegaLinha(sprintf($strSQL, (int) $pdcid));
    return ($dados) ? $dados : null;
}

/**
 * imprime a listagem de pedidos
 * tela de listar pedidos
 * @see Simec_Listagem()
 * @return string
 */
function listaPedidos($filtro = array(), $tipo = 'paginado', $where = '') {
    setStorageFilter($filtro);

    if (!empty($where)) {
        $where = " AND {$where}";
    }

    $query = "
        select * from (
            select
                pdc.pdcid,
                lpad(pdc.pdcid::varchar, 5, '0') as codpedido,
                to_char(pdc.pdcdatacadastro, 'DD/MM/YYYY'),
                pdc.pdctipo,
                pdc.unicod ||' - '|| uni.unidsc as unidade,
                pdc.pdccodacaojudicial,
                (select usunome from seguranca.usuario where usucpf = pdc.usucpf) AS usuario,
                (SELECT usunome FROM seguranca.usuario WHERE usucpf = pdc.respcpf) AS responsavel,
                (pdcvalorexecatual + pdcvalorexecpassado) as pdcvalorcdo,
                case when pdc.docid is not null then
                    (select to_char(htddata, 'DD/MM/YYYY')
                    from workflow.historicodocumento
                    where docid = (select docid from sicaj.pedidocdo where pdcid = pdc.pdcid)
                    order by htddata desc limit 1)
                else
                   ' - '
                end
                    AS dataStatus,
                case when pdc.docid is not null then
                    (select distinct
                    ed.esddsc
                    from workflow.documento d
                    inner join workflow.estadodocumento ed on (ed.esdid = d.esdid)
                    where d.docid = pdc.docid)
                else
                   null
                end
                    AS valorStatus,
                pdc.unicod,
                case when pdc.docid is not null then
                    (select distinct
                    to_char(d.docdatainclusao, 'MM') as mes
                    from workflow.documento d
                    inner join workflow.estadodocumento ed on (ed.esdid = d.esdid)
                    where d.docid = pdc.docid)
                else
                   null
                end
                    AS mesreferencia,
                case when pdc.docid is not null then
                    (select distinct
                    ed.esdid
                    from workflow.documento d
                    inner join workflow.estadodocumento ed on (ed.esdid = d.esdid)
                    where d.docid = pdc.docid)
                else
                   null
                end
                   AS esdid,
                case when pdc.docid is not null then
                    (select distinct
                    ed.esdid
                    from workflow.documento d
                    inner join workflow.estadodocumento ed on (ed.esdid = d.esdid)
                    where d.docid = pdc.docid)
                else
                   null
                end
                   AS esdidstatus
            from sicaj.pedidocdo pdc
            inner join public.unidade uni on (uni.unicod = pdc.unicod)
            where pdc.pdcstatus = 'A'
              {$where}
            order by pdc.pdcid desc
        ) vTable
    ";

    if ($filtro['requisicao'] == 'buscar' || $_POST['requisicao'] == 'xls') {

        $where = array();
        if (!empty($filtro['dados']['fonte'])) {
            if ($filtro['dados']['fonte'] == 'todos') {
                $where[] = "vTable.esdid is not null";
            } else {
                $where[] = "vTable.esdid in ({$filtro['dados']['fonte']})";
            }
        }

        if (!empty($filtro['pdccodacaojudicial'])) {
            $where[] = "vTable.pdccodacaojudicial = '{$filtro['pdccodacaojudicial']}'";
        }

        if (!empty($filtro['mesref'])) {
            $where[] = "vTable.mesreferencia = '{$filtro['mesref']}'";
        }

        if (!empty($filtro['dados']['unicod'])) {
            $where[] = "vTable.unicod = '{$filtro['dados']['unicod']}'";
        }

        if (!empty($filtro['dados']['pdctipo'])) {
            $where[] = "vTable.pdctipo = '{$filtro['dados']['pdctipo']}'";
        }

        if (count($where)) {
            $query .= " where " . implode(" and ", $where);
        }
    }

    //ver($where, $query, d);

    /**
     * Componente para listagens.
     * @see Simec_Listagem
     */
    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    $colunms = array(
        'Pedido',
        'Data do Pedido',
        'Tipo',
        'Unidade Orçamentária',
        'Cód <br>SICAJ',
        'Nome Usuário',
        'Responsável',
        'Valor <br/>CDO (R$)',
        'Data <br/>Status',
        'Status'
    );

    if ($tipo == 'xls') {
        $render = Simec_Listagem::RELATORIO_XLS;
        $esconde = array('codpedido', 'unicod', 'esdid', 'mesreferencia', 'esdidstatus');
    } else {
        $render = Simec_Listagem::RELATORIO_PAGINADO;
        $esconde = array('unicod', 'esdid', 'mesreferencia');
    }

    $list = new Simec_Listagem($render);
    if ($tipo != 'xls') {
        $list->addAcao('edit', array('func' => 'abrirPedido', 'extra-params' => array('unicod')))
                ->addAcao('delete', 'apagarPedido')
                ->setAcaoComoCondicional('delete', array(array('campo' => 'esdid', 'op' => 'igual', 'valor' => NAO_ENVIADO)))
                ->turnOnPesquisator();
    }
    $list->setCabecalho($colunms)
            ->esconderColunas($esconde)
            ->addCallbackDeCampo(array('unidade', 'usuario'), 'alinhaParaEsquerda')
            ->addCallbackDeCampo('esdidstatus', 'statusPedido')
            ->addCallbackDeCampo('pdcvalorcdo', 'mascaraMoeda')
            ->addCallbackDeCampo(array('usuario', 'responsavel'), 'shortName')
            ->addCallbackDeCampo('pdctipo', 'formataTipoPedido')
            ->addRegraDeLinha(
                    array('campo' => 'esdidstatus', 'op' => 'igual', 'valor' => array(ACERTOS_UO, AJUSTES_UO), 'classe' => 'acertos_uo')
            )
            ->addRegraDeLinha(
                    array('campo' => 'esdidstatus', 'op' => 'igual', 'valor' => ACERTOS_SEGEP, 'classe' => 'acertos_segep')
            )
            ->addRegraDeLinha(
                    array('campo' => 'esdidstatus', 'op' => 'igual', 'valor' => NAO_ENVIADO, 'classe' => 'nao_enviado')
            )
            ->setQuery($query)
            ->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
            ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
}

/**
 * Seta opção de filtro selecionado na sessão
 * para facilitar a navegação para tras
 * @param array $filtro
 * @return void(0)
 */
function setStorageFilter(array $filtro) {
    if (!empty($filtro['dados']['fonte'])) {
        $_SESSION['filtro']['fonte'] = $filtro['dados']['fonte'];
    } else {
        unset($_SESSION['filtro']['fonte']);
    }

    if (!empty($filtro['dados']['pdctipo'])) {
        $_SESSION['filtro']['pdctipo'] = $filtro['dados']['pdctipo'];
    } else {
        unset($_SESSION['filtro']['pdctipo']);
    }
}

function getStorage($key) {
    if (isset($_SESSION['filtro'][$key])) {
        return $_SESSION['filtro'][$key];
    }
    return '';
}

function clearAllStorage() {
    unset($_SESSION['filtro']);
}

function shortName($name) {
    if (empty($name)) {
        return '-';
    }
    $name = explode(' ', $name);
    return current($name) . ' ' . end($name);
}

function formataTipoPedido($pdctipo) {
    switch ($pdctipo) {
        case 'H':
            return '<span class="label label-success">Inicial</span>';
        case 'D':
            return '<span class="label label-warning">Anulação</span>';
        case 'R':
            return '<span class="label label-info">Reativação</span>';
        default:
            return '<span class="label label-default">Não identificado</span>';
    }
}

/**
 * Função complementear para imprimir icone em coluna do componente de listagem
 * @param $status
 * @param $dados
 * @return string
 */
function statusPedido($status, $dados) {
    $html = <<<HTML
<span class="glyphicon glyphicon-%s" style="color:%s;cursor:pointer" %s data-toggle="popover"
      data-content="%s"></span>
HTML;
    switch ($status) {
        case NAO_ENVIADO:
            return sprintf($html, 'minus', '#FFD700', '', $dados['valorstatus']);
        case ANALISE_SEGEP:
        case ANALISE_SPO:
        case ACERTOS_UO:
        case ACERTOS_SEGEP:
            return sprintf($html, 'transfer', '#428bca', '', $dados['valorstatus']);
        case CONCLUIDO:
            return sprintf($html, 'check', '#5CB85C', '', $dados['valorstatus']);
        case CANCELADO:
            return sprintf($html, 'remove-circle', '#D43F3A', '', $dados['valorstatus']);
    }
}

/**
 * Pega os dados do usuário que registrou o pedido
 * @param $pdcid
 * @return bool
 */
function getUsuarioPedido($pdcid) {
    global $db;

    $strSQL = "
        select
            s.usunome, s.usuemail, s.usufoneddd, s.usufonenum
        from
            sicaj.pedidocdo pdc
        inner join seguranca.usuario s on (s.usucpf = pdc.usucpf)
            where pdc.pdcid = %d
    ";

    $stmt = sprintf($strSQL, (int) $pdcid);
    $result = $db->pegaLinha($stmt);
    return ($result) ? $result : false;
}

/**
 * Função utilizada como condição para tramitação no workflow
 * Verifica se existem 3 tipos de arquivo para o pedido ser tramitado
 * @return bool
 */
function verifica_anexo_uo($docid) {
    global $db;

    $data = $db->pegaLinha("SELECT pdcid, pdctipo FROM sicaj.pedidocdo WHERE docid = {$docid}");
    $pdcid = $data['pdcid'];
    $pdctipo = $data['pdctipo'];

    if (!(true === ($result = verifica_tipo_pedido_homologacao($pdctipo)))) {
        return $result;
    }

    if ($pdcid) {
        $strSQL = "
            select * from (
                select
                    count(*) as total
                from sicaj.anexogeral
                where
                    angtipoanexo in ('DJ', 'PE', 'PF') and pdcid = {$pdcid}
            ) as vtable
            where vtable.total = 3
        ";
        //ver($strSQL, d);
        $result = $db->pegaUm($strSQL);
        return ($result) ? true : "Voce precisa anexar a Decisão Judicial, Parecer de força executória e a Planilha financeira!";
    }

    return "Voce precisa selecionar algum pedido judicial!";
}

function verifica_tipo_pedido_homologacao($pdctipo) {
    return ('H' == $pdctipo) ? true : 'Esta tramitação é apenas para pedidos de homologação.';
}

function verifica_tipo_pedido_reavaliacao($pdctipo) {
    return ('H' !== $pdctipo) ? true : 'Esta tramitação é apenas para pedidos de homologação.';
}

/**
 * Função utilizada como condição para tramitação no workflow
 * Verifica se existem 2 tipos de arquivo para o pedido ser tramitado
 * @return bool
 */
function verifica_anexo_cdo($docid) {
    global $db;

    $pdcid = $pdcid = $db->pegaUm("SELECT pdcid FROM sicaj.pedidocdo WHERE docid = {$docid}");
    if ($pdcid) {

        $anexos = verifica_anexo_uo($docid);
        if (is_string($anexos)) {
            return $anexos;
        }

        $strSQL = "
            select * from (
                select
                    count(*) as total
                from sicaj.anexogeral
                where
                    angtipoanexo in ('HS', 'MS') and pdcid = {$pdcid}
            ) as vtable
            where vtable.total = 2
        ";
        $result = $db->pegaUm($strSQL);
        return ($result) ? true : "Voce precisa anexar a Homologação SICAJ, Mensagem Eletronica SEGEP e preencher o valor do CDO!";
    }

    return false;
}

/**
 * Pega o estado atual do workflow
 * @param integer $pdcid
 * @return integer
 */
function pegarEstadoAtual($pdcid) {
    global $db;

    $docid = pegaDocid($pdcid);
    if ($docid) {
        $strSQL = sprintf("SELECT ed.esdid
                FROM workflow.documento d
                    JOIN workflow.estadodocumento ed ON(ed.esdid = d.esdid)
                WHERE d.docid = %d", (int) $docid);
        $estado = (integer) $db->pegaUm($strSQL);
        return $estado;
    }

    return false;
}

/**
 * Pegar o ID do perfil atual
 * @param string $usucpf
 * @return integer|boolean
 */
function pegarPerfilAtual($usucpf) {
    global $db;

    $strSQL = sprintf("
        select ur.pflcod from sicaj.usuarioresponsabilidade ur where ur.usucpf = '%s'
    ", (string) $usucpf);
    //ver($strSQL);

    $pflcod = (integer) $db->pegaUm($strSQL);
    if (!$pflcod) {

        $strSQL = sprintf("
            SELECT u.pflcod FROM seguranca.perfilusuario u WHERE u.usucpf = '%s' and u.pflcod in (%d, %d)
        ", (string) $usucpf, PERFIL_SUPER_USUARIO, PERFIL_CGO);
        //ver($strSQL);
        $pflcod = (integer) $db->pegaUm($strSQL);
    }

    return ($pflcod) ? $pflcod : FALSE;
}

/**
 * @param $pdcid
 * @return string
 */
function pegarUnicod($pdcid) {
    global $db;

    $strSQL = sprintf("
        select unicod from sicaj.pedidocdo where pdcid = %d
    ", (int) $pdcid);
    return (string) $db->pegaUm($strSQL);
}

/**
 * controla permissão para apagar arquivo anexado
 * @param $pdcid
 * @param $usucpf
 * @return bool
 */
function allowClear($pdcid, $usucpf) {
    global $db;

    $estado = pegarEstadoAtual($pdcid);
    $pflcod = pegarPerfilAtual($usucpf);
    $unicod = pegarUnicod($pdcid);

    if ($pflcod == PERFIL_SUPER_USUARIO) {
        return true;
    }

    if ($estado == CONCLUIDO) {
        return false;
    }

    if ($estado == NAO_ENVIADO) {
        return true;
    }

    if (($pflcod == PERFIL_UO_EQUIPE_TECNICA) && ($estado == ACERTOS_UO || $estado == ACERTOS_SEGEP) && ($_GET['unicod'] == $unicod)) {
        return true;
    }

    if (($pflcod == PERFIL_CGO) && ($estado == ANALISE_SPO || $estado == ANALISE_SEGEP) && ($_GET['unicod'] == $unicod)) {
        return true;
    }

    return false;
}

/**
 * Desabilita os formulários para equipe tecnica
 * quando momento é SPO ou SEGEP
 * @return string
 */
function momento_spo_segep() {

    $estado = pegarEstadoAtual($_GET['id']);
    $pflcod = pegarPerfilAtual($_SESSION['usucpf']);

    if (($estado == ANALISE_SPO || $estado == ANALISE_SEGEP) && $pflcod == PERFIL_UO_EQUIPE_TECNICA) {
        echo '<script type="text/javascript">
                $(function(){
                    $(".file-input-wrapper, #inserir, #limpar").attr("disabled", true);
                });
            </script>';
    }
}

/**
 * Desabilita os formulários para equipe tecnica
 * quando momento é SPO ou SEGEP
 * @return string
 */
function pedido_concluido() {

    $estado = pegarEstadoAtual($_GET['id']);
    $pflcod = pegarPerfilAtual($_SESSION['usucpf']);

    if ($estado == CONCLUIDO) {
        echo '<script type="text/javascript">
                $(function(){
                    $(".file-input-wrapper, #inserir, #limpar").attr("disabled", true);
                });
            </script>';
    }
}

/**
 * Delete pedido SICAJ que ainda nao possui historico de tramitacao
 * @param $codPedido
 */
function apagaPedido($codPedido) {
    global $db;

    $strSQL = sprintf("
        delete from sicaj.pedidocdo where pdcid = %d
    ", (integer) $codPedido);

    $db->executar($strSQL);
    $db->commit();
}

function getAbasPorPerfil($urlBaseDasAbas, &$abaAtiva, $tipo, $id = null) {

    $estado = pegarEstadoAtual($_GET['id']);
    $pflcod = pegarPerfilAtual($_SESSION['usucpf']);

    // -- Configura dados para montar abas
    $listaAbas = array();
    if ('H' == $tipo) {
        $listaAbas[] = array("id" => 1, "descricao" => '<span class="glyphicon glyphicon-file"></span> Dados do Pedidos', "link" => "{$urlBaseDasAbas}pedidos");
    } else {
        $listaAbas[] = array("id" => 1, "descricao" => '<span class="glyphicon glyphicon-refresh"></span> Dados do Pedidos', "link" => "{$urlBaseDasAbas}reavaliacao");
        if ('pedidos' == $abaAtiva) {
            $abaAtiva = 'reavaliacao';
        }
    }
    $listaAbas[] = array('id' => 2, 'descricao' => '<span class="glyphicon glyphicon-download-alt"></span> Documentos Anexados UO', 'link' => "{$urlBaseDasAbas}anexos");

    if (in_array($estado, array(ANALISE_SPO, ANALISE_SEGEP, CONCLUIDO, HOMOLOGADO, ANALISE_COORDENACAO))) {
        $listaAbas[] = array('id' => 3, 'descricao' => '<span class="glyphicon glyphicon-download"></span> CDO', 'link' => "{$urlBaseDasAbas}cdo");
    }

    if (null === $id) {
        unset($listaAbas[1], $listaAbas[2]);
    }

    return montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
}

/**
 * @param $valor
 * @return string
 */
function formataDataUs($valor) {

    if (strlen($valor)) {
        $datePart = explode('/', $valor);
        return "{$datePart[2]}-{$datePart[1]}-{$datePart[0]}";
    }
}

/**
 * Valida formato da data, bem como se a data é valida dentro do calendario gregoriano
 * Valida data
 * @param $date
 * @return bool
 */
function validaData($date) {
    if (strlen($date)) {
        $datePart = explode('/', $date);
        if (count($datePart) === 3) {
            return (checkdate($datePart[1], $datePart[0], $datePart[2])) ? true : false;
        }
    }

    return false;
}

/**
 * Envio de email quando sair de Analise SPO e vai para Analise SEGEP
 * @return bool
 */
function envia_email_pedido_segep($docid) {
    global $db;

    require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
    $fm = new Simec_Helper_FlashMessage('sicaj/anexos');

    $pdcid = $db->pegaUm("SELECT pdcid FROM sicaj.pedidocdo WHERE docid = {$docid}");
    if (!$pdcid) {
        $fm->addMensagem('Não foi encontrado um documento válido para a efetuar a transição..', Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    $strSQL = sprintf("
        select
            pdc.numprocessoadm, pdc.pdcnumprocessojudicial, pdc.pdcjuizodacao,
            pdc.pdccodobjeto, pdc.pdccodacaojudicial, pdc.pdcnumbeneficioacao,
            TO_CHAR(pdc.pdcdatainicio, 'DD/MM/YYYY') as pdcdatainicio, u.usuemail, u.usunome,
            u2.usuemail as respemail
        from sicaj.pedidocdo pdc
        inner join seguranca.usuario u on (u.usucpf = pdc.usucpf)
        left join seguranca.usuario u2 on (u2.usucpf = pdc.respcpf)
        where pdc.pdcid = %d
    ", (integer) $pdcid);
    $dados = $db->pegaLinha($strSQL);
    if (!$dados) {
        $fm->addMensagem("Não foi encontrado um pedido que corresponda ao documento N# {$docid}.", Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    /*
     * Verifica a soma dos tamanhos dos anexos, caso seja maior que 5 MB  não envia pois não presta
     */
    if (retornaTamanhoAnexosObrigatorios($pdcid) < 10) {
        $anexos = pegarCaminhosAnexosUo($pdcid);
    } else {
        $anexos = array();
    }

    $remetente = array('nome' => 'SPO SICAJ', 'email' => 'spo.sicaj@mec.gov.br');

    if (IS_PRODUCAO) {
        #$destinatario = 'diaju.segep@planejamento.gov.br;';
        $destinatario = $dados['respemail']; #demanda 297060 - Enviar email ao responsável pelo pedido.
    } else {
        $destinatario = EMAIL_SIMEC_ANALISTA;
    }

    $assunto = "[SIMEC]  Análise de ação - {#pdccodacaojudicial} (código SICAJ)";
    $template = "
        <table border='0' width='100%' cellspacing='2' cellpadding='2'>
            <thead>
                <tr>
                    <th align='left'>Senhor Chefe da DIAJU,</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <p>Nos termos do contido no item 3 da Mensagem SIAPE número 537500, de 16/03/2010, solicitamos
                        analisar a ação a seguir identificada para fins de cumprimento da respectiva  decisão judicial.</p>
                        <br />
                        <ul>
                            <li>Numero do processo administrativo: {#numprocessoadm}</li>
                            <li>Numero do processo judicial: {#pdcnumprocessojudicial}</li>
                            <li>Juízo da ação: {#pdcjuizodacao}</li>
                            <li>Código do objeto cadastrado no SICAJ: {#pdccodobjeto}</li>
                            <li>Ação cadastrada no SICAJ: {#pdccodacaojudicial}</li>
                            <li>Numero de Benefíciário da ação: {#pdcnumbeneficioacao}</li>
                            <li>Data do ìnicio da eficácia temporal: {#pdcdatainicio}</li>
                        </ul>
                        <br />
                        <p>Seguem em anexo os seguintes arquivos:</p>
                        <br />
                        <ul>
                            <li>Decisão Judicial</li>
                            <li>Parecer de força executória</li>
                            <li>Planilha financeira compatível com o termo inicial para cumprimento da decisão judicial, definido no parecer de força executória.</li>
                        </ul>
                        <br />
                        <p>Atenciosamente,<br />
                           {#usunome}<br />
                           {#usuemail}<br />
                           CGO/SPO<br />
                           Ministério da Educação<br />
                        </p>
                    </td>
                </tr>
            </tbody>
        </table>
    ";

    extract($dados);
    $search = array('{#numprocessoadm}', '{#pdcnumprocessojudicial}', '{#pdcjuizodacao}', '{#pdccodobjeto}',
        '{#pdccodacaojudicial}', '{#pdcnumbeneficioacao}', '{#pdcdatainicio}', '{#usunome}', '{#usuemail}');

    $subject = array($numprocessoadm, $pdcnumprocessojudicial, $pdcjuizodacao, $pdccodobjeto, $pdccodacaojudicial
        , $pdcnumbeneficioacao, $pdcdatainicio, $usunome, $usuemail);

    $conteudo = str_replace($search, $subject, $template);
    $assunto = str_replace('{#pdccodacaojudicial}', $pdccodacaojudicial, $assunto);

    //ver($conteudo, $anexos, d);
    $cc = !empty($_SESSION['usuemail']) ? $_SESSION['usuemail'] : '';
    if (enviar_email($remetente, $destinatario, $assunto, $conteudo, $cc, EMAIL_SIMEC_DESENVOLVEDOR, $anexos)) {
        return true;
    } else {
        $msg = 'Não foi possível enviar e-mail.';
        if(!$dados['respemail']){
            $msg = 'Não foi possível enviar e-mail. O Responsável pelo pedido não possui e-mail.';
        }
        $fm->addMensagem($msg, Simec_Helper_FlashMessage::ERRO);
        return true;
    }
}

/**
 * Envio de email quando sair de Analise SPO e vai para Analise SEGEP
 * @return bool
 */
function envia_email_mensagem_6($docid) {
    global $db;

    require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
    $fm = new Simec_Helper_FlashMessage('sicaj/anexos');

    $pdcid = $db->pegaUm("SELECT pdcid FROM sicaj.pedidocdo WHERE docid = {$docid}");
    if (!$pdcid) {
        $fm->addMensagem('Não foi encontrado um documento válido para a efetuar a transição.', Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    $strSQL = sprintf("
        select
            pdc.numprocessoadm, pdc.pdcnumprocessojudicial, pdc.pdcjuizodacao,
            pdc.pdccodobjeto, pdc.pdccodacaojudicial, pdc.pdcnumbeneficioacao,
            TO_CHAR(pdc.pdcdatainicio, 'DD/MM/YYYY') as pdcdatainicio, u.usuemail, u.usunome
        from sicaj.pedidocdo pdc
        inner join seguranca.usuario u on (u.usucpf = pdc.usucpf)
        where pdc.pdcid = %d
    ", (integer) $pdcid);
    $dados = $db->pegaLinha($strSQL);
    if (!$dados) {
        $fm->addMensagem("Não foi encontrado um pedido que corresponda ao documento N# {$docid}.", Simec_Helper_FlashMessage::ERRO);
        return false;
    }

    /*
     * Verifica a soma dos tamanhos dos anexos, caso seja maior que 5 MB  não envia pois não presta
     */
    if (retornaTamanhoAnexosObrigatorios($pdcid) < 10) {
        $anexos = pegarCaminhosAnexosUo($pdcid);
    } else {
        $anexos = array();
    }

    $remetente = array('nome' => 'SPO SICAJ', 'email' => 'spo.sicaj@mec.gov.br');


    $assunto = "[SIMEC]  Análise de ação - {#pdccodacaojudicial} (código SICAJ)";
    $template = "
        <table border='0' width='100%' cellspacing='2' cellpadding='2'>
            <thead>
                <tr>
                    <th align='left'>Senhor(a) {#usunome},</th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td>
                        <p>A homologação no SICAJ foi concluída. Favor enviar e-mail para a DIAJU/SEGEP/MP com os arquivos em anexo solicitando a análise da ação.</p>
                        <br />
                        <ul>
                            <li>Numero do processo administrativo: {#numprocessoadm}</li>
                            <li>Numero do processo judicial: {#pdcnumprocessojudicial}</li>
                            <li>Juízo da ação: {#pdcjuizodacao}</li>
                            <li>Código do objeto cadastrado no SICAJ: {#pdccodobjeto}</li>
                            <li>Ação cadastrada no SICAJ: {#pdccodacaojudicial}</li>
                            <li>Numero de Benefíciário da ação: {#pdcnumbeneficioacao}</li>
                            <li>Data do ìnicio da eficácia temporal: {#pdcdatainicio}</li>
                        </ul>
                        <br />
                        <p>Seguem em anexo os seguintes arquivos:</p>
                        <br />
                        <ul>
                            <li>Decisão Judicial</li>
                            <li>Parecer de força executória</li>
                            <li>Planilha financeira compatível com o termo inicial para cumprimento da decisão judicial, definido no parecer de força executória.</li>
                        </ul>
                        <br />
                        <p>Atenciosamente,<br />
                           {#usunome_sessao}<br />
                           {#usuemail_sessao}<br />
                           CGO/SPO<br />
                           Ministério da Educação<br />
                        </p>
                    </td>
                </tr>
            </tbody>
        </table>
    ";

    extract($dados);

    if (IS_PRODUCAO) {
        $destinatario = $usuemail; // -- usuário da UO
    } else {
        $destinatario = EMAIL_SIMEC_ANALISTA;
    }

    $search = array('{#usunome}', '{#numprocessoadm}', '{#pdcnumprocessojudicial}', '{#pdcjuizodacao}',
        '{#pdccodobjeto}', '{#pdccodacaojudicial}', '{#pdcnumbeneficioacao}',
        '{#pdcdatainicio}', '{#usunome_sessao}', '{#usuemail_sessao}');

    $subject = array($usonome, $numprocessoadm, $pdcnumprocessojudicial, $pdcjuizodacao, $pdccodobjeto,
        $pdccodacaojudicial, $pdcnumbeneficioacao, $pdcdatainicio,
        ucwords($_SESSION['usunome']), $_SESSION['usuemail']);

    $conteudo = str_replace($search, $subject, $template);

    $assunto = str_replace('{#pdccodacaojudicial}', $pdccodacaojudicial, $assunto);

    //ver($conteudo, $anexos, d);
    $cc = !empty($_SESSION['usuemail']) ? $_SESSION['usuemail'] : '';
    try {
        if (enviar_email($remetente, $destinatario, $assunto, $conteudo, $cc, EMAIL_SIMEC_DESENVOLVEDOR, $anexos)) {
            return true;
        } else {
            $fm->addMensagem('Não foi possível enviar e-mail.', Simec_Helper_FlashMessage::ERRO);
            return true;
        }
    } catch (Exception $ex) {
        return true;
    }
}

/**
 * Pegar os caminhos de todos os anexos do tipo DJ, PE e PF, feitos pela UO
 * @param $pdcid
 * @return array
 */
function pegarCaminhosAnexosUo($pdcid) {
    require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    global $db;

    $strSQL = sprintf("
       select
          s.arqid, a.arqtipo, a.arqnome, a.arqextensao, s.angtipoanexo
       from sicaj.anexogeral s
       join public.arquivo a on (a.arqid = s.arqid)
       where
           s.pdcid = %d
           and angtipoanexo in ('%s', '%s', '%s', '%s')", $pdcid, DECISAO_JUDICIAL, PARECER_EXECUTORIO, PLANILHA_FINANCEIRA, OUTROS_DOCUMENTOS
    );
    $arqPaths = $db->carregar($strSQL);

    $file = new FilesSimec(null, null, 'sicaj');
    $caminhos = array();

    if (is_array($arqPaths) && count($arqPaths)) {
        foreach ($arqPaths as $arq) {
            $populate = array(
                'arquivo' => $file->getCaminhoFisicoArquivo($arq['arqid']),
                'arqtipo' => $arq['arqtipo'],
                'nome' => rename_file($arq['angtipoanexo']) . ".{$arq['arqextensao']}"
            );
            array_push($caminhos, $populate);
        }
    }

    //ver($caminhos, d);
    return $caminhos;
}

/**
 * Renomea o arquivo sem caracteres especiais
 * @param $tipo
 * @return string
 */
function rename_file($tipo) {
    switch ($tipo) {
        case DECISAO_JUDICIAL:
            return 'decisao_judicial';
        case PARECER_EXECUTORIO:
            return 'parecer_executorio';
        case PLANILHA_FINANCEIRA:
            return 'planilha_financeira';
        case OUTROS_DOCUMENTOS:
            return 'outros_docs_' . time();
    }
}

/**
 * Mostra o numero do pedido formatado
 * @param $pdcid
 * @return string
 */
function mostra_numero_pedido($pdcid) {
    return str_pad($pdcid, 5, '0', STR_PAD_LEFT);
}

/**
 * @param $size
 * @return string
 */
function by2M($size) {
    $filesizename = array(" Bytes", " KB", " MB", " GB", " TB", " PB", " EB", " ZB", " YB");
    return $size ? round($size / pow(1024, ($i = floor(log($size, 1024)))), 2) . $filesizename[$i] : '0 Bytes';
}

/**
 * @param $value
 * @param $mask
 * @return string
 */
function mascaraglobal($value, $mask) {
    $casasdec = explode(",", $mask);
// Se possui casas decimais
    if ($casasdec[1])
        $value = sprintf("%01." . strlen($casasdec[1]) . "f", $value);

    $value = str_replace(array("."), array(""), $value);
    if (strlen($mask) > 0) {
        $masklen = -1;
        $valuelen = -1;
        while ($masklen >= -strlen($mask)) {
            if (-strlen($value) <= $valuelen) {
                if (substr($mask, $masklen, 1) == "#") {
                    $valueformatado = trim(substr($value, $valuelen, 1)) . $valueformatado;
                    $valuelen--;
                } else {
                    if (trim(substr($value, $valuelen, 1)) != "") {
                        $valueformatado = trim(substr($mask, $masklen, 1)) . $valueformatado;
                    }
                }
            }
            $masklen--;
        }
    }
    return $valueformatado;
}

/**
 * Envia e-mail com aviso de tramitação do pedido
 * @param $docid
 * @return bool
 */
function enviar_email_altera_estado($docid) {
    global $db;

    //Pega situação atual, e a situação futura do documento
    $strSQLTramite = "
        SELECT
            ed.esddsc AS inicio,
            ae.aeddscrealizada AS fim,
            (SELECT TO_CHAR(htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento WHERE docid = {$docid} ORDER BY hstid DESC LIMIT 1) AS dttramite
        FROM workflow.acaoestadodoc ae
        INNER JOIN workflow.estadodocumento ed ON (ed.esdid = ae.esdidorigem)
        WHERE ae.aedid = (
            SELECT aedid FROM workflow.historicodocumento WHERE docid = {$docid} ORDER BY hstid DESC LIMIT 1
        )
    ";

    //Pega o dono do pedido no SICAJ-WEB
    $strSQLOwner = "
        SELECT
            u.usunome, usuemail
        FROM sicaj.pedidocdo p
        INNER JOIN seguranca.usuario u ON (u.usucpf = p.usucpf)
        WHERE docid = {$docid}
    ";

    //Pega o DociD do pedido
    $pedido = $db->pegaUm("SELECT pdcid FROM sicaj.pedidocdo WHERE docid = {$docid}");
    $tramite = $db->pegaLinha($strSQLTramite);
    $owner = $db->pegaLinha($strSQLOwner);

    if (!$pedido || !$tramite || !$owner) {
        return false;
    }

    //Template de mensagem padrão de troca de estado
    $template = "
        <table border='0' width='100%' cellspacing='2' cellpadding='2'>
            <tbody>
                <tr>
                    <td>
                        <p>O pedido no SICAJ de número: <strong>{#pedido}</strong>, foi tramitado do estado <strong>\"{#inicio}\"</strong> para o estado <strong>\"{#fim}\"</strong>, em: {#data}.</p>
                        <p>Atenciosamente,<br />
                           {#usunome}<br />
                           {#usuemail}<br />
                           CGO/SPO<br />
                           Ministério da Educação<br />
                        </p>
                    </td>
                </tr>
            </tbody>
        </table>
    ";

    extract($tramite);
    $search = array('{#pedido}', '{#inicio}', '{#fim}', '{#data}', '{#usunome}', '{#usuemail}');
    $subject = array($pedido, $inicio, $fim, $dttramite, $_SESSION['usunome'], $_SESSION['usuemail']);

    $conteudo = str_replace($search, $subject, $template);
    $assunto = "Pedido {$pedido} - Aviso de tramitação";
    $destinatario = trim($owner['usuemail']);
    $remetente = array(
        'email' => trim($_SESSION['usuemail']),
        'nome' => 'SIMEC'
    );
    $cc = $cco = '';

    if (!IS_PRODUCAO) {
        $cc = EMAIL_SIMEC_ANALISTA;
        $cco = EMAIL_SIMEC_DESENVOLVEDOR;
    }

    if (enviar_email($remetente, $destinatario, $assunto, $conteudo, $cc, $cco)) {
        return true;
    } else {
        return false;
    }
}

//registrar no estado wf: Reativar Homologação
function desativar_estado_anula_homologacao($docid) {
    global $db;

    $pdcid = $db->pegaUm("select pdcid from sicaj.pedidocdo where docid = {$docid}");

    if (!$pdcid)
        return false;

    $strSQL = "
        select
            pdc.pdcid,pdc.pdctipo
        from sicaj.pedidocdo pdc
        where pdc.pdctipo = 'R' and pdc.pdcid = {$pdcid}
    ";

    return ($db->pegaLinha($strSQL)) ? true : false;
}

//registrar no estado wf: Anular Homologação
function desativar_estado_reativacao_homologacao($docid) {
    global $db;

    $pdcid = $db->pegaUm("select pdcid from sicaj.pedidocdo where docid = {$docid}");

    if (!$pdcid)
        return false;

    $strSQL = "
        select
            pdc.pdcid,pdc.pdctipo
        from sicaj.pedidocdo pdc
        where pdc.pdctipo = 'D' and pdc.pdcid = {$pdcid}
    ";

    return ($db->pegaLinha($strSQL)) ? true : false;
}

/**
 * retorna o dia da semana em pt-br
 * @return string
 */
function pegaDiaSemana() {
    switch (date('N')) {
        case 1:
            return 'Segunda-feira';
        case 2:
            return 'Terça-feira';
        case 3:
            return 'Quarta-feira';
        case 4:
            return 'Quinta-feira';
        case 5:
            return 'Sexta-feira';
        case 6:
            return 'Sábado';
        case 7:
            return 'Domingo';
    }
}

/**
 * retorna mes do ano
 * @return string
 */
function pegaMesAno($mes = null) {
    if(!$mes){
        $mes = date('n');
    }
    switch ($mes) {
        case 1:
            return 'Janeiro';
        case 2:
            return 'Fevereiro';
        case 3:
            return 'Março';
        case 4:
            return 'Abril';
        case 5:
            return 'Maio';
        case 6:
            return 'Junho';
        case 7:
            return 'Julho';
        case 8:
            return 'Agosto';
        case 9:
            return 'Setembro';
        case 10:
            return 'Outubro';
        case 11:
            return 'Novembro';
        case 12:
            return 'Dezembro';
    }
}

/**
 * Retorna o tamanho dos anexos obrigatórios
 * @param $pdcid
 * @return integer
 */
function retornaTamanhoAnexosObrigatorios($pdcid) {
    global $db;

    $strSQL = sprintf("
       select
         (SUM(arqtamanho) /1024) /1024
       from sicaj.anexogeral s
       join public.arquivo a on (a.arqid = s.arqid)
       where
           s.pdcid = %d
           and angtipoanexo in ('%s', '%s', '%s', '%s')", $pdcid, DECISAO_JUDICIAL, PARECER_EXECUTORIO, PLANILHA_FINANCEIRA, OUTROS_DOCUMENTOS
    );
    return $db->pegaUm($strSQL);
}

function removeDocumentoValidacao($pdcid){
    require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    global $db;

    $strSQL = "select count(0) "
            . "from sicaj.pedidocdovalidado pdc "
            . "inner join public.validacaodocumento vd "
            . "on (vd.vldid = pdc.vldid) "
            . "where pdc.pdcid = %d and pdc.pcvstatus = 'A'";
    $sql = sprintf($strSQL, $pdcid);
    $possuiArquivo=$db->pegaLinha($sql);
    if($possuiArquivo['count']){
        $arquivo = new FilesSimec('validacaodocumento', $campos, 'public');
        if($arquivo->existeArquivo($arqid)){
            $arquivo->excluiArquivoFisico($arqid);
        }
        $strSQL = "update sicaj.pedidocdovalidado set pcvstatus = 'I' where pdcid = %d";
        $db->executar(sprintf($strSQL, $pdcid));
        $db->commit();
        return true;
    } else {
        return true;
    }
}