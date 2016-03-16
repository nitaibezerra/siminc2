<?php
$perfilEmenda = pegaPerfil($_SESSION["usucpf"]);

unset($_POST['acao']);
unset($_SESSION['emenda']['post']);

if ($_REQUEST['limpaSession']) {
    die;
}

require_once APPRAIZ . '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br/>';

monta_titulo('Relatórios Geral - Eventos', '<b>Filtros de Pesquisa</b>');

function agrupador() {
    return array(
        array('codigo' => 'eveid',
            'descricao' => 'Título do Evento'),
        array('codigo' => 'ungcod',
            'descricao' => 'Unidade Gestora'),
        array('codigo' => 'rcoid',
            'descricao' => 'Reunião do Comitê'),
        array('codigo' => 'tpeid',
            'descricao' => 'Tipo'),
        array('codigo' => 'evequantidadedias',
            'descricao' => 'Nº de dias do Evento'),
        array('codigo' => 'evepublicoestimado',
            'descricao' => 'Público'),
        array('codigo' => 'estuf',
            'descricao' => 'UF'),
        array('codigo' => 'muncod',
            'descricao' => 'Município'),
        array('codigo' => 'evecustoprevisto',
            'descricao' => 'Custo(R$)'),
        array('codigo' => 'eveanopi',
            'descricao' => 'Ano do PI'),
        array('codigo' => 'plicod',
            'descricao' => 'Nº PI'),
        array('codigo' => 'evenumeroprocesso',
            'descricao' => 'Nº Processo')
    );
}

function colunas() {
    return array(
        array('codigo' => 'eveid',
            'descricao' => 'Título do Evento'),
        array('codigo' => 'ungcod',
            'descricao' => 'Unidade Gestora'),
        array('codigo' => 'rcoid',
            'descricao' => 'Reunião do Comitê'),
        array('codigo' => 'tpeid',
            'descricao' => 'Tipo'),
        array('codigo' => 'evequantidadedias',
            'descricao' => 'Nº de dias do Evento'),
        array('codigo' => 'evepublicoestimado',
            'descricao' => 'Público'),
        array('codigo' => 'estuf',
            'descricao' => 'UF'),
        array('codigo' => 'muncod',
            'descricao' => 'Município'),
        array('codigo' => 'evecustoprevisto',
            'descricao' => 'Custo(R$)'),
        array('codigo' => 'eveanopi',
            'descricao' => 'Ano do PI'),
        array('codigo' => 'plicod',
            'descricao' => 'Nº PI'),
        array('codigo' => 'evenumeroprocesso',
            'descricao' => 'Nº Processo'),
        array('codigo' => 'evedatainclusao',
            'descricao' => 'Ano do Evento')
    );
}
?>

    <script type="text/javascript" src="/includes/prototype.js"></script>

    <script type="text/javascript">
        function onOffCampo(campo) {
            var div_on = document.getElementById(campo + '_campo_on');
            var div_off = document.getElementById(campo + '_campo_off');
            var input = document.getElementById(campo + '_campo_flag');

            if (div_on.style.display == 'none') {
                div_on.style.display = 'block';
                div_off.style.display = 'none';
                input.value = '1';
            } else {
                div_on.style.display = 'none';
                div_off.style.display = 'block';
                input.value = '0';
            }
        }
        function geraPopRelatorio() {
            var form = $('formulario');

            if ($('evedatainclusao_campo_flag').value == "1") {
                selectAllOptions(form.evedatainclusao);
            }
            if ($('eveid_campo_flag').value == "1") {
                selectAllOptions(form.eveid);
            }
            if ($('ungcod_campo_flag').value == "1") {
                selectAllOptions(form.ungcod);
            }
            if ($('rcoid_campo_flag').value == "1") {
                selectAllOptions(form.rcoid);
            }
            if ($('tpeid_campo_flag').value == "1") {
                selectAllOptions(form.tpeid);
            }
            if ($('evequantidadedias_campo_flag').value == "1") {
                selectAllOptions(form.evequantidadedias);
            }
            if ($('evepublicoestimado_campo_flag').value == "1") {
                selectAllOptions(form.evepublicoestimado);
            }
            if ($('estuf_campo_flag').value == "1") {
                selectAllOptions(form.estuf);
            }
            if ($('muncod_campo_flag').value == "1") {
                selectAllOptions(form.muncod);
            }
            if ($('evecustoprevisto_campo_flag').value == "1") {
                selectAllOptions(form.evecustoprevisto);
            }
            if ($('eveanopi_campo_flag').value == "1") {
                selectAllOptions(form.eveanopi);
            }
            if ($('plicod_campo_flag').value == "1") {
                selectAllOptions(form.plicod);
            }
            if ($('evenumeroprocesso_campo_flag').value == "1") {
                selectAllOptions(form.evenumeroprocesso);
            }
            
            selectAllOptions(form.agrupador);
            selectAllOptions(form.coluna);
            form.target = 'page';
            var janela = window.open('evento.php?modulo=relatorio/popUpRelatorioGeralEvento=I', 'page', 'toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes');
            janela.focus();
            form.submit();
        }
    </script>

    <form action="evento.php?modulo=relatorio/popUpRelatorioGeralEvento&acao=I" method="post" name="formulario" id="formulario">
        <input type="hidden" name="limpaSession" id="limpaSession" value="true">
        <table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td width="25%" class="SubTituloDireita">Ordenação</td>
                <td>
                    <?php
                        $matriz = agrupador();
                        $campoAgrupador = new Agrupador('formulario');
                        $campoAgrupador->setOrigem('agrupadorOrigem', null, $matriz);
                        $campoAgrupador->setDestino('agrupador', null, array(
                            /* array('codigo' => 'autid',
                              'descricao' => 'Autor') */
                        ));
                        $campoAgrupador->exibir();
                    ?>

                </td>
            </tr>
            <tr>
                <td width="25%" c class="SubTituloDireita">Colunas</td>
                <td>
                    <?php
                        $coluna = colunas();
                        $campoColuna = new Agrupador('formulario');
                        $campoColuna->setOrigem('colunaOrigem', null, $coluna);
                        $campoColuna->setDestino('coluna', null, array(
                        /* array('codigo' => 'eveid',
                          'descricao' => 'Título do Evento'),
                          array('codigo' => 'ungcod',
                          'descricao' => 'Unidade Gestora') */
                        ));
                        $campoColuna->exibir();
                    ?>
                </td>
            </tr>

            <?php
                #ANO DO EVENTO
                $evedatainclusao = $_REQUEST["evedatainclusao"];
                $sql = "
                    SELECT  DISTINCT CAST( DATE_PART('year', evedatainclusao) AS integer ) AS codigo,
                            CAST(DATE_PART('year', evedatainclusao) AS VARCHAR(4) ) as descricao
                    FROM evento.evento
                    WHERE evedatainclusao IS NOT NULL 
                    ORDER BY 1
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Ano do Evento', 'evedatainclusao', $sql, $stSqlCarregados, 'Selecione o "ano" do Evento');
                
                //Título do Evento
                $eveid = $_REQUEST["eveid"];

                $sql = "
                    SELECT  DISTINCT eveid as codigo,
                            evetitulo as descricao
                    FROM evento.evento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Título do Evento', 'eveid', $sql, $stSqlCarregados, 'Selecione o(s) Título do Evento(s)');

                //Unidade Gestora
                $ungcod = $_REQUEST["ungcod"];
                $sql = "
                    SELECT  DISTINCT u.ungcod as codigo,
                            u.ungdsc as descricao
                    FROM public.unidadegestora AS u
                    INNER JOIN evento.evento AS ev ON u.ungcod = ev.ungcod
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Unidade Gestora', 'ungcod', $sql, $stSqlCarregados, 'Selecione a(s) Unidade Gestora(s)');

                //Reunião do Comitê
                
                $rcoid = $_REQUEST["rcoid"];
                $sql = "
                    SELECT  DISTINCT rc.rcoid AS codigo,
                            rc.rcodescricao AS descricao
                    FROM evento.reuniaocomite as rc
                    INNER JOIN evento.evento AS ev ON ev.rcoid = rc.rcoid
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Reunião do Comitê', 'rcoid', $sql, $stSqlCarregados, 'Selecione o(s) Reunião do Comitê(s)');

                //Tipo:
                $tpeid = $_REQUEST["tpeid"];
                $sql = "
                    SELECT  DISTINCT tpeid AS codigo,
                            tpedescricao AS descricao
                    FROM evento.tipoevento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Tipo', 'tpeid', $sql, $stSqlCarregados, 'Selecione o(s) Tipo(s)');

                //Nº de dias do Evento
                $evequantidadedias = $_REQUEST["evequantidadedias"];
                $sql = "
                    SELECT  DISTINCT evequantidadedias AS codigo,
                            evequantidadedias AS descricao
                    FROM evento.evento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Nº de dias do Evento', 'evequantidadedias', $sql, $stSqlCarregados, 'Selecione o(s) Nº de dias do Evento(s)');

                //Público
                $evepublicoestimado = $_REQUEST["evepublicoestimado"];
                $sql = "
                    SELECT  DISTINCT evepublicoestimado AS codigo,
                            evepublicoestimado AS descricao
                    FROM evento.evento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Público', 'evepublicoestimado', $sql, $stSqlCarregados, 'Selecione o(s) Público(s)');

                //UF
                $estuf = $_REQUEST["estuf"];
                $sql = "
                    SELECT  DISTINCT estuf AS codigo,
                            estuf AS descricao
                    FROM evento.evento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('UF', 'estuf', $sql, $stSqlCarregados, 'Selecione o(s) UF(s)');

                //Município
                $muncod = $_REQUEST["muncod"];

                $sql = "
                    SELECT  DISTINCT tm.muncod AS codigo,
                            tm.mundescricao AS descricao
                    FROM evento.evento AS ev
                    INNER JOIN territorios.municipio AS tm ON tm.muncod = ev.muncod
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Município', 'muncod', $sql, $stSqlCarregados, 'Selecione o(s) Município(s)');

                //Custo(R$)
                $evecustoprevisto = $_REQUEST["evecustoprevisto"];
                $sql = "
                    SELECT  DISTINCT evecustoprevisto AS codigo,
                            evecustoprevisto AS descricao
                    FROM evento.evento
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Custo(R$)', 'evecustoprevisto', $sql, $stSqlCarregados, 'Selecione o(s) Custo(R$)(s)');

                //Ano do PI
                $eveanopi = $_REQUEST["eveanopi"];
                $sql = "
                    SELECT  DISTINCT eveanopi AS codigo,
                            eveanopi AS descricao
                    FROM evento.evento
                    WHERE eveanopi <> ''
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Ano do PI', 'eveanopi', $sql, $stSqlCarregados, 'Selecione o(s) Ano do PI(s)');

                //Nº PI
                $plicod = $_REQUEST["plicod"];
                $sql = "
                    SELECT  DISTINCT mp.plicod AS codigo,
                            mp.plicod AS descricao
                    FROM monitora.pi_planointerno as mp
                    INNER JOIN public.unidadegestora AS u ON u.ungcod = mp.ungcod
                    INNER JOIN evento.evento AS ev ON ev.ungcod = u.ungcod
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Nº PI', 'plicod', $sql, $stSqlCarregados, 'Selecione o(s) Tipo(s)');

                //Nº Processo
                $evenumeroprocesso = $_REQUEST["evenumeroprocesso"];

                $sql = "
                    SELECT  DISTINCT evenumeroprocesso AS codigo,
                            evenumeroprocesso AS descricao
                    FROM evento.evento
                    WHERE  evenumeroprocesso <> ''
                    ORDER BY descricao ASC
                ";
                $stSqlCarregados = "";
                mostrarComboPopup('Nº Processo', 'evenumeroprocesso', $sql, $stSqlCarregados, 'Selecione o(s) Nº Processo(s)');
            ?>

            <tr bgcolor="#D0D0D0">
                <td colspan="2" style="text-align: center;">
                    <input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
                </td>
            </tr>
        </table>
    </form>