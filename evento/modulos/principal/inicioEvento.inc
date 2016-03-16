<?PHP

    $manutencao = "N";
    if ($manutencao == 'S' && !$db->testa_superuser()) {
        $titulo_modulo = "Eventos";
        //Chamada de programa
        include APPRAIZ . "includes/cabecalho.inc";
        echo'<br>';

        echo '
            <p style="margin: 0 0 0 0;text-align: center; color: red; font-size: 30px">Sistema em manutenção.</p>
            <p style="text-align: center; font-size: 15px">Volte em alguns instantes</p>
        ';
        return false;
    }

    #CADASTRAR evento
    if ($_REQUEST['evento']) {
        $evetitulo = $_REQUEST['evento'];
        if (!empty($evetitulo)) {

            $numEventos = verificaEventos();
            if ($numEventos < MAX_EVENTOS_SEM_NOTA) {
                $sql = "INSERT INTO evento.evento ( evetitulo, usucpf, sevid, evestatus ) VALUES ( '$evetitulo', '{$_SESSION['usucpf']}', 1, 'A' ) RETURNING eveid";
                if ($eveid = $db->pegaUm($sql)) {
                    evtCriarDoc($eveid);
                    //atividade_atribuir_responsavel( $atiid, PERFIL_GESTOR, array( $_SESSION['usucpf'] ) );
                    $db->commit();
                }
            } else {
                echo '
                    <script type="text/javascript">
                        alert( "Relatórios Técnicos em Aberto" );
                        window.location.href = "?modulo=inicio&acao=C"
                    </script>
                ';
            }
        }
    }

    #REMOVER
    if ($_REQUEST['remover']) {
        $eveid = (integer) $_REQUEST['remover'];
        $sql = "update evento.evento SET evestatus = 'I' WHERE eveid = " . $eveid;
        $db->executar($sql);
        $db->commit();

        redirecionar($_REQUEST['modulo'], $_REQUEST['acao']);
    }

    #ATUALIZA O COMBO EVENTO.
    if( $_REQUEST['atualizarComboEmpenho'] ){
        header('content-type: text/html; charset=ISO-8859-1');

        $sql = "
            SELECT  emuid AS codigo,
                    empnumero ||' - '|| empdescricao AS descricao
            FROM evento.empenho_unidade

            WHERE empstatus = 'A' AND coeid = {$_POST['atualizarComboEmpenho']}
            ORDER BY codigo
        ";
        $db->monta_combo("emuid", $sql, 'S', 'Selecione...', '', '', '', 470, '', 'emuid');
        die();
    }

    $ungcod = getUnidadeByCpf();
    $perfis = arrayPerfil();

    $titulo_modulo = "Eventos";
    include APPRAIZ . "includes/cabecalho.inc";
    echo'<br>';
    monta_titulo( $titulo_modulo, 'Clique no nome do evento para iniciar o trabalho.' );

?>

<head>
    <style type="text/css">
        a.dcontexto{
            position:relative;
            padding:0;
            color:#039;
            text-decoration: none;
            cursor: pointer;
            margin-right: -1px;
            z-index:24;
        }
        a.dcontexto:hover{
            background:transparent;
            text-decoration: none;
            z-index:25;
        }
        a.dcontexto span{
            display: none;
            text-decoration: none;
        }
        a.dcontexto:hover span{
            display: block;
            position:absolute;
            width: 200px;
            font-size: 10px;
            top:2em;
            text-align:center;
            /*left:50px;*/
            padding:4px 0px;
            border:1px solid #000;
            background:#eee;
            color:#000;
            text-decoration: none;
        }
    </style>

    <link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
    <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

    <script src="../includes/prototype.js"></script>

    <script type="text/javascript">

        function exibirHistorico( docid ){
            var url = 'http://<?PHP echo $_SERVER['SERVER_NAME'] ?>/geral/workflow/historico.php?modulo=principal/tramitacao&acao=C&docid='+docid;
            window.open( url, 'alterarEstado', 'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no' );
        }

        function atualizarEmpenho( coeid ){
            var td = document.getElementById('td_empenho');

            if( coeid > 0 ){
                var req = new Ajax.Request(
                        'evento.php?modulo=principal/inicioEvento&acao=A', {
                        method:     'post',
                        parameters: '&atualizarComboEmpenho=' + coeid,
                        onComplete: function ( resp ){
                            td.innerHTML = resp.responseText;
                        }
                });
            }
        }

        function selecionarEvento( eveid ){
            window.location.href = '?modulo=inicio&acao=C&selecionar='+eveid;
        }

        function cadastrarEvento(){
            var req = new Ajax.Request(
                '?modulo=principal/cadEvento&acao=A', {
                method: 'POST',
                parameters: 'ajaxunsetsession=1',
                onComplete: function (res) {
                    window.location.href = '?modulo=principal/cadEvento&acao=A';
                }
            });
        }

        function efetuarPesquisa( opcao ){
            var formulario = document.getElementById('formulario');

            if( opcao == 'P' ){
                formulario.submit();
            }else{
                formulario.filtro_evetitulo.value = "";
                formulario.filtro_eveid.value = "";
                formulario.filtro_evenumeroprocesso.value = "";
                formulario.filtro_unidade.value = "";
                formulario.filtro_coeid.value = "";
                formulario.filtro_emuid.value = "";
                formulario.filtro_situacao.value = "";
                formulario.filtro_eveurgente.value = "T";
                formulario.evedatainclusao_ini.value = "";
                formulario.evedatainclusao_fim.value = "";
                formulario.submit();
            }
        }

        function selecionaEvento( id ){
            var req = new Ajax.Request(
                'evento.php?modulo=inicio&acao=C', {
                    method:     'post',
                    parameters: '&ajaxsession=' + id,
                    onComplete: function (res) {
                        window.location.href = '?modulo=principal/cadEventoAnexo&acao=A';
                    }
                });
        }

        function toolTypeAjax(id){
            if(!id){
                return false;
            }
            var anexo = document.getElementById('anexo_'+id);

            var req = new Ajax.Request(
                'evento.php?modulo=inicio&acao=C', {
                    method:     'post',
                    parameters: '&toolTypeAjax='+id,
                    onComplete: function (res){
                        $('span_'+id).innerHTML = res.responseText;
                    }
            });
        }

    </script>

</head>

<form action="" method="POST" name="formulario" id="formulario">
    <input type="hidden" name="acao" value="<?= $_REQUEST['acao'] ?>"/>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" border="1">
        <tr>
            <td class="SubTituloDireita" width="25%"> Título do Evento: </td>
            <td>
                <?= campo_texto('filtro_evetitulo', 'N', 'S', '', 73, 200, '', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Número do evento: </td>
            <td>
                <?= campo_texto('filtro_eveid', 'N', 'S', '', 36, 7, '#######', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Número do processo: </td>
            <td>
                <?= campo_texto('filtro_evenumeroprocesso', 'N', 'S', '', 36, 20, '#####.######/####-##', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Unidade Demandante: </td>
            <td valign="bottom" colspan="2-">
                <?PHP
                    $filtro_unidade = $_POST['filtro_unidade'];
                    $sql = "
                        SELECT
                            ungcod AS codigo,
                            ungdsc AS descricao
                        FROM
                            public.unidadegestora
                        WHERE
                            orgcod = 'E'
                        ORDER BY
                            ungdsc
                    ";
                    $db->monta_combo("filtro_unidade", $sql, 'S', 'Selecione...', '', '', '', 470, '', 'filtro_unidade');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Contrato: </td>
            <td valign="bottom" colspan="2-">
                <?PHP
                    $filtro_coeid = $_POST['filtro_coeid'];
                    $sql = "
                        SELECT  '999' AS codigo,
                                'MEC - GV2 Produções S/A' AS descricao
                        UNION
                        SELECT  coeid AS codigo,
                                coenumcontrato ||' - '|| coerazaosocial AS descricao
                        FROM evento.contratopregao

                        WHERE coestatus = 'A'
                        ORDER BY codigo desc
                    ";
                    $db->monta_combo("filtro_coeid", $sql, 'S', 'Selecione...', 'atualizarEmpenho', '', '', 470, '', 'filtro_coeid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Empenho: </td>
            <td id="td_empenho">
                <?PHP
                    $filtro_emuid = $_POST['filtro_emuid'];
                    $sql = "
                        SELECT  emuid AS codigo,
                                empnumero ||' - '|| empdescricao AS descricao
                        FROM evento.empenho_unidade

                        WHERE empstatus = 'A'
                        ORDER BY codigo
                    ";
                    $db->monta_combo("filtro_emuid", $sql, 'S', 'Selecione...', '', '', '', 470, '', 'filtro_emuid');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Situação: </td>
            <td>
                <?PHP
                    $filtro_situacao = $_POST['filtro_situacao'];
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = ".WF_TPDID_EVENTOS."  AND esdstatus = 'A'
                        ORDER BY esdordem
                    ";
                    $db->monta_combo("filtro_situacao", $sql, 'S', 'Selecione...', '', '', '', 470, '', 'filtro_situacao');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> AD Referendum: </td>
            <td>
                <input type="radio" name="filtro_eveurgente" value="T" <?= ((!$_REQUEST['eveurgente'] || $_REQUEST['eveurgente'] == 'T') ? 'checked="checked"' : '') ?> />Todos
                &nbsp;
                <input type="radio" name="filtro_eveurgente" value="S" <?= (($_REQUEST['eveurgente'] == 'S') ? 'checked="checked"' : '') ?> />Sim
                &nbsp;
                <input type="radio" name="filtro_eveurgente" value="N" <?= (($_REQUEST['eveurgente'] == 'N') ? 'checked="checked"' : '') ?> />Não
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Período de Inclução do Evento: </td>
            <td>
                <?PHP
                    $evedatainclusao_ini = $_REQUEST['evedatainclusao_ini'];
                    $evedatainclusao_fim = $_REQUEST['evedatainclusao_fim'];
                    echo campo_data2('evedatainclusao_ini', 'N', 'S', 'Período de Inicío', '##/##/####', '', '', $evedatainclusao_ini, '', '', 'evedatainclusao_ini');
                    echo 'à';
                    echo campo_data2('evedatainclusao_fim', 'N', 'S', 'Período de Fim', '##/##/####', '', '', $evedatainclusao_fim, '', '', 'evedatainclusao_fim');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Período de Realização do Evento: </td>
            <td>
                <?PHP
                    $evedatarealizacao_ini = $_REQUEST['evedatarealizacao_ini'];
                    $evedatarealizacao_fim = $_REQUEST['evedatarealizacao_fim'];
                    echo campo_data2('evedatarealizacao_ini', 'N', 'S', 'Período de Inicío', '##/##/####', '', '', $evedatarealizacao_ini, '', '', 'evedatarealizacao_ini');
                    echo 'à';
                    echo campo_data2('evedatarealizacao_fim', 'N', 'S', 'Período de Fim', '##/##/####', '', '', $evedatarealizacao_fim, '', '', 'evedatarealizacao_fim');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisa" value="Pesquisar" onclick="return efetuarPesquisa('P');"/>
                <input type="button" name="vertodos" value="Ver Todos" onclick="return efetuarPesquisa('T');"/>
            </td>
        </tr>
    </table>
</form>

<br>

<?PHP
    if (!temPerfilEmpresa()) { ?>
        <table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" style="border-bottom:none;">
            <tr>
                <td style="padding: 10px;">
                    <span style="cursor: pointer" onclick="cadastrarEvento();" title="novo Evento">
                        <img align="absmiddle" src="/imagens/gif_inclui.gif"/> Cadastrar Evento
                    </span>
                </td>
            </tr>
        </table>
<?PHP
    }

    $evestatus = " ev.evestatus = 'A' ";

    if( $_REQUEST['filtro_evetitulo']){
        $titulo = trim( $_REQUEST['filtro_evetitulo'] );
	$where = " AND public.removeacento(ev.evetitulo) ilike public.removeacento('%{$titulo}%') ";
    }

    if( $_REQUEST['filtro_eveid'] ){
	$where .= " AND ev.eveid = '{$_POST['filtro_eveid']}' ";
    }

    if( $_REQUEST['filtro_evenumeroprocesso']){
        $n_processo = $_REQUEST['filtro_evenumeroprocesso'];
	$where .= " AND public.removeacento(ev.evenumeroprocesso) ilike public.removeacento('%{$n_processo}%') ";
    }

    if( $_REQUEST['filtro_unidade']){
	$where .= " AND ev.ungcod = '{$_POST['filtro_unidade']}' ";
    }

    if( $_REQUEST['filtro_coeid'] != '' ){
        $coeid = $_REQUEST['filtro_coeid'];

        if( $coeid == '999' ){
            $where .= " AND ev.evedatainclusao <= '2014-08-26 11:09:41.107953-03' AND ev.emuid IS NULL";
        }else{
            $where .= " AND emp.coeid = '{$coeid}'";
        }
    }

    if( $_REQUEST['filtro_emuid'] != '' ){
        $emuid = $_REQUEST['filtro_emuid'];
        $where .= " AND ev.emuid = '{$emuid}'";
    }

    if ( $_REQUEST['filtro_situacao'] != '' ){
        $esdid = $_REQUEST['filtro_situacao'];

        if( $esdid == 534 ){
            $evestatus = " ( ev.evestatus = 'I' OR est.esdid = {$esdid} ) ";
        }else{
            $where .= " AND est.esdid = {$esdid} ";
        }
    }

    if( $_REQUEST['filtro_eveurgente'] != '' ){
	$urgente = trim( $_REQUEST['filtro_eveurgente'] );

        if( $urgente != 'T'){
            if( $urgente == 'S'){
                $where .= " AND ev.eveurgente = 't' ";
            }else{
                $where .= " AND ev.eveurgente = 'f' ";
            }
        }
    }

    if( $_REQUEST['evedatainclusao_ini'] != '' && $_REQUEST['evedatainclusao_fim'] != '' ){
        $evedatainclusao_ini = formata_data_sql($_REQUEST['evedatainclusao_ini']);
        $evedatainclusao_fim = formata_data_sql($_REQUEST['evedatainclusao_fim']);

        $where .= " AND ev.evedatainclusao BETWEEN '{$evedatainclusao_ini}' AND '{$evedatainclusao_fim}'";
    }
    
    if( $_REQUEST['evedatarealizacao_ini'] != '' && $_REQUEST['evedatarealizacao_fim'] != '' ){
        $evedatarealizacao_ini = formata_data_sql($_REQUEST['evedatarealizacao_ini']);
        $evedatarealizacao_fim = formata_data_sql($_REQUEST['evedatarealizacao_fim']);

        $where .= " AND ( ( ev.evedatainicio BETWEEN '{$evedatarealizacao_ini}' AND '{$evedatarealizacao_fim}' ) OR ( ev.evedatafim BETWEEN '{$evedatarealizacao_ini}' AND '{$evedatarealizacao_fim}' ) )";
    }

    #FILTRA OS EVENTOS DE ACORDO COM A INSTITUIÇÃO. SELECIONAS SOMENTE OS RESPCTIVOS.
    if (( ( in_array(PERFIL_SEE, $perfis) ) || in_array(PERFIL_SAA, $perfis) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis) ) || in_array(PERFIL_EMPRESA, $perfis) || in_array(EVENTO_PERFIL_CONSULTA, $perfis)) {
        $condicao = '';
    } elseif (in_array(PERFIL_DEMANDANTE, $perfis)) {
        $condicao = " AND ( ( ev.ungcod = '$ungcod' ) OR ev.usucpf = '" . $_SESSION['usucpf'] . "' ) ";
    } else {
        $condicao = " AND (ev.ungcod = '$ungcod' OR ev.usucpf = '" . $_SESSION['usucpf'] . "' )";
    }

    $acao = "
        <div align=\"center\">
            <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: selecionarEvento('||ev.eveid||');\" title=\"selecionar Evento\">
        </div>
    ";

    $acao_disab = "
        <div align=\"center\">
            <img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\" title=\"selecionar Evento\">
        </div>
    ";

    $sql = "
        SELECT  DISTINCT
                CASE WHEN ev.evestatus = 'I'
                    THEN '{$acao_disab}'
                    ELSE '{$acao}'
                END AS eveid,

                CASE
                    WHEN aev.eveid IS NULL THEN ''
                    WHEN aev.eveid IS NOT NULL THEN '<center><a class=\"dcontexto\" id=\"anexo_'||ev.eveid||'\" onmouseover=\"return toolTypeAjax('||ev.eveid||');\"> <img src=\"/imagens/anexo.gif\" style=\"cursor: pointer\" onclick=\"selecionaEvento('||ev.eveid||');\"> </img><span><div id=\"span_'||ev.eveid||'\"></div></span></a></center>'
                END as doc,

                CASE
                    WHEN ev.eveurgente != 't' THEN ''
                    WHEN ev.eveurgente = 't' THEN '<img src=\"/imagens/check_p.gif\" border=0 title=\"Preenchida\">'
                END as referendun,

                ev.eveid AS ID,
                ev.evenumeroprocesso,

                CASE WHEN ev.evestatus = 'A'
                    THEN
                        '<div align=\"left\"> <a href=\"javascript:selecionarEvento('||ev.eveid||');\" title=\"selecionar Evento\"> ' || replace(ev.evetitulo,chr(34),'') || ' </a>
                        <br>
                        <a href=\"#\" onclick=\"enviar_email(' || case when coalesce(u.usucpf, '') = '' then 'Não há Gestor'else u.usucpf end ||' ); return false;\" style=\"text-decoration: none;\">
                            <span style=\"font-size: 9px; font-weight: normal; color: #808080;\" title=\"Clique para enviar e-mail.\">
                                Responsável: '||
                                    case when coalesce (ev.everespnome, '') = ''
                                        then 'Não há Responsável'
                                        else ev.everespnome
                                    end ||'
                            </span>
                        </a> </div>'
                    ELSE
                        '<div align=\"left\"> ' || replace(ev.evetitulo,chr(34),'') || '
                        <br>
                        <span style=\"font-size: 9px; font-weight: normal; color: #808080;\" title=\"Clique para enviar e-mail.\">
                            Responsável: '||
                                case when coalesce (ev.everespnome, '') = ''
                                    then 'Não há Responsável'
                                    else ev.everespnome
                                end ||'
                        </span>
                        </div>'
                END AS evetitulo,

                to_char(ev.evedatainclusao,'YYYY'),
		uni.ungdsc,
                to_char( ev.evedatainicio, 'DD/MM/YYYY' ) AS datainicio,
                to_char( ev.evedatafim, 'DD/MM/YYYY' ) AS datafim,
                ev.evepublicoestimado,
		(m.mundescricao||' - '||ev.estuf) as local,
                ev.evecustoprevisto,

                '<span onclick=\"exibirHistorico('|| ev.docid ||');\" style=\"cursor: pointer; color:#4682B4; \" ><b>'||est.esddsc ||'</b></span>' as status,

                CASE WHEN hd.htddata IS NULL
                    THEN '<div align=\"center\"> -- </div>'
                    ELSE '<span style=\"display:none\">'|| hd.htddata || '</span> ' || to_char( hd.htddata, 'DD/MM/YYYY HH24:MI' )
                END AS datatramite,

                CASE WHEN evedatainclusao IS NULL
                    THEN '<div align=\"center\"> -- </div>'
                    ELSE '<span style=\"display:none\">'|| evedatainclusao || '</span> ' || to_char( evedatainclusao, 'DD/MM/YYYY HH24:MI' )
                END AS evedatainclusao

        FROM evento.evento as ev

        JOIN territorios.municipio m ON m.muncod = ev.muncod

        JOIN seguranca.usuario AS u ON u.usucpf = ev.usucpf

        LEFT JOIN evento.empenho_unidade AS emp ON emp.emuid = ev.emuid

        LEFT JOIN evento.anexoevento AS aev ON aev.eveid = ev.eveid
        LEFT JOIN public.unidadegestora AS uni ON uni.ungcod = ev.ungcod

        LEFT JOIN workflow.documento d ON d.docid = ev.docid and d.tpdid = 36
        LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid and est.tpdid = 36

        LEFT JOIN (
            select docid, max(htddata) as htddata
            from workflow.historicodocumento
            where docid in (select docid from workflow.documento where tpdid = 36)
            group by docid
        ) hd ON hd.docid = d.docid

        WHERE {$evestatus} {$condicao} {$where}

        ORDER BY ev.eveid desc
    ";
    $cabecalho = array("Ação", "Anexo", "AD", "Cód", "Nº Processo", "Evento", "Ano", "Unidade Gestora", "Início do Evento", "Fim do Evento", "Publico", "Local", "Custo", "Situação", "Data da Situação", "Data da Inclusão");

    $alinhamento = Array('center', 'center', '', '', '', '', '', '', '', '', 'center', '', '', '', '', '');
    $tamanho = Array('', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '');

    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
?>