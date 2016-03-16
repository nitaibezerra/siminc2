<?php
//ver($_SESSION['exercicio']);
if ($_REQUEST['lvdid']) {
    $_REQUEST['lvdtitulo'] = str_replace("\'", "", $_REQUEST['lvdtitulo']);
    $_REQUEST['lvdcodigo'] = str_replace("\'", "", $_REQUEST['lvdcodigo']);
    $_REQUEST['lvdtipo'] = str_replace("\'", "", $_REQUEST['lvdtipo']);
    $_REQUEST['lvdanoedicao'] = str_replace("\'", "", $_REQUEST['lvdanoedicao']);
    $_REQUEST['lvdnumeropaginas'] = str_replace("\'", "", $_REQUEST['lvdnumeropaginas']);
    $_REQUEST['lvdautor'] = str_replace("\'", "", $_REQUEST['lvdautor']);
    $_REQUEST['resenharecurso'] = str_replace("\'", "", $_REQUEST['resenharecurso']);

    if ($_REQUEST['lvdtipo'] == 'L')
        $tipo = 'Livro do Aluno';
    if ($_REQUEST['lvdtipo'] == 'M')
        $tipo = 'Manual do Professor';
?>
    <table>
        <tr>
            <td> Titulo: </td>
            <td> <?php echo $_REQUEST['lvdtitulo']; ?> </td>
        </tr>
        <tr>
            <td> Código: </td>
            <td> <?php echo $_REQUEST['lvdcodigo']; ?> </td>
        </tr>
        <tr>
            <td> Tipo: </td>
            <td> <?php echo $tipo; ?> </td>
        </tr>
        <tr>
            <td> Ano Edição: </td>
            <td> <?php echo $_REQUEST['lvdanoedicao']; ?> </td>
        </tr>
        <tr>
            <td> Numero de Paginas: </td>
            <td> <?php echo $_REQUEST['lvdnumeropaginas']; ?> </td>
        </tr>
        <tr>
            <td> Autor: </td>
            <td> <?php echo $_REQUEST['lvdautor']; ?> </td>
        </tr>
    </table>
    <?php
    exit;
}

//Chamada de programa
include APPRAIZ . "includes/cabecalho.inc";
echo "<br>";

$db->cria_aba($abacod_tela, $url, $parametros);

monta_titulo('Coleções / Livros Regionais', $linha2);
?>

<link rel="stylesheet" type="text/css" href="../../includes/superTitle.css"/>
<link rel="stylesheet" href="../includes/jquery-treeview/jquery.treeview.css" />

<script type="text/javascript" src="../../includes/remedial.js"></script>
<script type="text/javascript" src="../../includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/lib/jquery.cookie.js"></script>
<script type="text/javascript" src="../includes/jquery-treeview/jquery.treeview-simec.js"></script>
<script type="text/javascript">
    var janela;

    $(document).ready(function() {
        $("#_arvore").treeview({
            collapsed: true,
            animated: "medium",
            control: "#sidetreecontrol",
            persist: "cookie"
        });

        $('#abrirTodos').click();

        $(window).unload(function() {
            janela.close();
        });
    });

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

    function gerarRelatorio(t) {
        var formulario = document.formulario;

        if (t == 1) {
            selectAllOptions(formulario.editora);
            selectAllOptions(formulario.componente);
            selectAllOptions(formulario.colecao);
            selectAllOptions(formulario.parecer);
            selectAllOptions(formulario.composicao);
            selectAllOptions(formulario.midia);
        } else {
            formulario.descricao.value = '';
        }
        formulario.submit();
    }
</script>

<form name="formulario" id="formulario" action="" method="post">
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <?PHP
                // Filtros do relatório
                $stSql = "
                    SELECT edt.edtid as codigo,
                           edt.edtnome as descricao
                    FROM livro.editora edt
                    WHERE edt.edtstatus = 'A'
                ";

                $stSqlCarregados = "";

                if (is_array($_POST['editora']) && $_POST['editora'][0]) {
                    $stSqlCarregados = "
                            SELECT edt.edtid as codigo,
                                   edt.edtnome as descricao
                            FROM livro.editora edt
                            WHERE edt.edtstatus = 'A'
                            AND   edt.edtid IN (" . implode(", ", $_POST['editora']) . ")
                    ";
                }
                mostrarComboPopup('Editora', 'editora', $stSql, $stSqlCarregados, 'Selecione a Editora');

                $stSql = "
                    SELECT comid as codigo,
                           comdsc as descricao
                    FROM livro.componente
                ";
                $stSqlCarregados = "";
                if (is_array($_POST['componente']) && $_POST['componente'][0]) {
                    $stSqlCarregados = "
                        SELECT comid as codigo,
                               comdsc as descricao
                        FROM livro.componente
                        WHERE comid IN (" . implode(", ", $_POST['componente']) . ")
                    ";
                }
                mostrarComboPopup('Componente', 'componente', $stSql, $stSqlCarregados, 'Selecione o Componente');

                $stSql = "
                    SELECT colid as codigo,
                           colcodigo || ' - ' || col.coltitulo as descricao
                    FROM livro.colecao col
                    WHERE colstatus = 'A' and prsano = '" . $_SESSION['exercicio'] . "';
                ";
                $stSqlCarregados = "";
                if (is_array($_POST['colecao']) && $_POST['colecao'][0]) {
                    $stSqlCarregados = "
                        SELECT colid as codigo,
                               coltitulo as descricao
                        FROM livro.colecao col
                        WHERE colstatus = 'A'
                        AND   colid IN (" . implode(", ", $_POST['colecao']) . ") and prsano = '" . $_SESSION['exercicio'] . "';
                    ";
                }
                mostrarComboPopup('Coleção', 'colecao', $stSql, $stSqlCarregados, 'Selecione a Coleção');

                $stSql = "
                    SELECT tpaid as codigo,
                           tpadsc as descricao
                    FROM livro.tipoparecer
                    WHERE tpastatus = 'A'
                ";
                $stSqlCarregados = "";
                if (is_array($_POST['parecer']) && $_POST['parecer'][0]) {
                    $stSqlCarregados = "
                        SELECT tpaid as codigo,
                               tpadsc as descricao
                        FROM livro.tipoparecer
                        WHERE tpastatus = 'A'
                        AND tpaid IN (" . implode(", ", $_POST['parecer']) . ")
                    ";
                }
                mostrarComboPopup('Tipo documento', 'parecer', $stSql, $stSqlCarregados, 'Selecione o Parecer');

                $stSql = "
                    SELECT cpsid as codigo,
                           cpsdsc as descricao
                    FROM livro.composicao
                    WHERE cpsstatus = 'A'
                ";
                $stSqlCarregados = "";
                if (is_array($_POST['composicao']) && $_POST['composicao'][0]) {
                    $stSqlCarregados = "
                        SELECT  cpsid as codigo,
                                cpsdsc as descricao
                        FROM livro.composicao
                        WHERE cpsstatus = 'A'
                        AND cpsid IN (" . implode(", ", $_POST['composicao']) . ")
                    ";
                }
                mostrarComboPopup('Tipo de Composição', 'composicao', $stSql, $stSqlCarregados, 'Selecione a Composição');

                $stSql = "
                    SELECT tpm_id as codigo,
                           tpm_descricao as descricao
                    FROM livro.tipoparecermidia
                ";
                $stSqlCarregados = "";
                if (is_array($_POST['midia']) && $_POST['midia'][0]) {
                    $stSqlCarregados = "
                        SELECT tpm_id as codigo,
                               tpm_descricao as descricao
                        FROM livro.tipoparecermidia
                        WHERE tpm_id IN (" . implode(", ", $_POST['midia']) . ")
                    ";
                }
                mostrarComboPopup('Parecer de Mídia', 'midia', $stSql, $stSqlCarregados, 'Selecione o de Tipo de Parecer');
            ?>
        <tr>
            <td class="SubTituloDireita" width="10%">Descrição:</td>
            <td>
                <?php
                    if ($_POST['descricao'] != "")
                        $descricao = $_POST['descricao'];

                    echo campo_texto('descricao', 'N', 'S', '', 50, 60, '', '', '', '', '', 'descricao')
                ?>
            </td>
        </tr>
        <tr>
            <td align="center" colspan="2">
                <input type="button" name="Pesquisar" value="Pesquisar" onclick="javascript:gerarRelatorio(1);"/>
                &nbsp;&nbsp;
                <input type="button" name="Ver Todos" value="Ver Todos" onclick="javascript:gerarRelatorio(2);"/>
            </td>
        </tr>
    </table>
</form>

<br>
<!--$_SESSION['exercicio'] -->

<?PHP
    if( $_SESSION['exercicio'] < 2015){
        $texto = "Versão";
        $legenda = '
            <td width="2%" align="center">
                <img border="0" align="absmiddle" src="../imagens/pnld/p_amarelo.gif"/>
            </td>       
            <td width="10%"> Versão universidade </td>
        ';
    }else{
        $texto = "Documento";
    }
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <td class="SubTituloEsquerda" colspan="14">
            Legedas
        </td>
    </tr>
    <tr>
        <td width="2%" align="center">
            <img border="0" align="absmiddle" src="../imagens/alterar.gif"/>
        </td>
        <td width="10%"> Acesso aos Pareceres </td>
        <td width="2%" align="center">
            <img border="0" align="absmiddle" src="../imagens/pnld/p_azul.gif"/>
        </td>
        <td width="8%"> <?=$texto?>  MEC </td>
        <?=$legenda;?>
        <td width="2%" align="center">
            <img border="0" align="absmiddle" src="../imagens/pnld/p_cinza.gif"/>
        </td>
        <td width="8%"> <?=$texto?> editora </td>
        <td width="2%" align="center">
            <img border="0" align="absmiddle" src="../imagens/pnld/p_verde.gif"/>
        </td>
        <td width="8%"> <?=$texto?> final </td>
        <td width="2%" align="center">
            <img border="0" width="18" align="absmiddle" src="../imagens/pnld/dvd_32x32.png"/>
        </td>
        <td width="13%"> Tipo 1 - Coleções Impressas + DVD </td>

        <td width="2%" align="center">
            <img border="0" width="18" align="absmiddle" src="../imagens/pnld/livro_32x32.png"/>
        </td>
        <td> Tipo 2 - Coleções Impressas </td>
    </tr>
</table>

<br>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
    <tr>
        <?PHP
            $arrPerfil = arrayPerfil();

            $cabecalho_parecer = array(
                'label' => 'Parecer',
                'colunas' => array(
                    '<center>Aprovação</center>', '<center>Aprovação Condicionada</center>', '<center>Exclusão</center>', '<center>Recurso</center>', '<center>Resposta</center>', '<center>Aprovação de Mídia</center>'
                )
            );

            if (in_array(PNLD_PERFIL_PARECERISTA, $arrPerfil)) {
                $colunas_colecao = "col.colcodigo AS codigo,";
                $cabecalho = array("Ação", "Editora", "Componente", "Código", "CP", "Tipo", $cabecalho_parecer);
            } else {
                $colunas_colecao = "col.colcodigo AS codigo, col.coltitulo AS colecao,";
                $cabecalho = array("Ação", "Editora", "Componente", "Código", "Coleção", "CP", "Tipo", $cabecalho_parecer);
            }

            extract($_POST);

            $innerColecao = "";

            if ($editora[0] && ( $editora_campo_flag || $editora_campo_flag == '1' )) {
                $where .= " AND edt.edtid " . (( $editora_campo_excludente == null || $editora_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $editora) . "') ";
            }
            if ($componente[0] && ( $componente_campo_flag || $componente_campo_flag == '1' )) {
                //$where .= " AND com.comid " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $componente) . "') ";
                $where .= " AND lvd.comid " . (( $componente_campo_excludente == null || $componente_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $componente) . "') ";
            }
            if ($componente[0] && ( $componente_campo_flag || $componente_campo_flag == '1' )) {
                if (in_array(19,$componente) || in_array(18,$componente) || in_array(5,$componente) || in_array(7,$componente)){
                    $whereReg[0] = " AND com.comid  in ('" . implode("','", $componente) . "')";
                    $whereReg[1] = " AND lvd.comid  in ('" . implode("','", $componente) . "')";
                }
            }
            if ($colecao[0] && ( $colecao_campo_flag || $colecao_campo_flag == '1' )) {
                $where .= " AND col.colid " . (( $colecao_campo_excludente == null || $colecao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $colecao) . "') ";
                $innerColecao = " INNER JOIN livro.colecao col ON edt.edtid = col.edtid ";
            }

            if ($parecer[0] && ( $parecer_campo_flag || $parecer_campo_flag == '1' )) {
                $where .= " AND rav.tpaid " . (( $parecer_campo_excludente == null || $parecer_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $parecer) . "') ";
            }
            
            if ($composicao[0] && ( $composicao_campo_flag || $composicao_campo_flag == '1' )) {
                $where .= " AND lvd.cpsid " . (( $composicao_campo_excludente == null || $composicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $composicao) . "') ";
            }
            if ($midia[0] && ( $midia_campo_flag || $midia_campo_flag == '1' )){
                $where .= " AND rav.tpm_id " . (( $midia_campo_excludente == null || $midia_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " (" . implode(",", $midia) . ")";
            }

            if ($descricao != '') {
                $whereC = "AND edt.edtnome ilike '%{$descricao}%' OR com.comdsc ilike '%{$descricao}%' OR col.coltitulo ilike '%{$descricao}%' OR col.colcodigo ilike '%{$descricao}%'";
                $whereL = "AND edt.edtnome ilike '%{$descricao}%' OR com.comdsc ilike '%{$descricao}%' OR lvd.lvdtitulo ilike '%{$descricao}%' OR lvd.lvdcodigo ilike '%{$descricao}%'";
            }

            if (!$db->testa_superuser()) {
                if (in_array(PNLD_EDITORA, $arrPerfil)) {
                    $inner = "
                        INNER JOIN livro.usuarioresponsabilidade rpu ON rpu.edtid = col.edtid AND rpu.rpustatus = 'A' AND rpu.usucpf = '{$_SESSION['usucpf']}' AND pflcod = ".PNLD_EDITORA."
                    ";
                }

                if (in_array(PNLD_PERFIL_COORDENADOR, $arrPerfil)) {
                    $inner = "
                        INNER JOIN livro.usuarioresponsabilidade rpu ON rpu.comid = lvd.comid AND rpu.rpustatus = 'A' AND rpu.usucpf = '{$_SESSION['usucpf']}' AND pflcod = ".PNLD_PERFIL_COORDENADOR."
                    ";
                }
                
                if (in_array(PNLD_PERFIL_COMISSAO_TECNICA, $arrPerfil)) {
                    $inner = "
                        INNER JOIN livro.usuarioresponsabilidade rpu ON rpu.comid = lvd.comid AND rpu.rpustatus = 'A' AND rpu.usucpf = '{$_SESSION['usucpf']}' AND pflcod = ".PNLD_PERFIL_COMISSAO_TECNICA."
                    ";
                }
            }
            
            /*
             " . (checkPerfil(array(PNLD_PERFIL_PARECERISTA), false) ? (is_array($_SESSION['pnld']['colid']) ? ' AND lvd.colid in (' . implode(',', $_SESSION['pnld']['colid']) . ')' : '') : '' ) . "
             " . (checkPerfil(array(PNLD_PERFIL_COORDENADOR), false) ? (is_array($_SESSION['pnld']['comid']) ? ' AND lvd.comid in (' . implode(',', $_SESSION['pnld']['comid']) . ')' : '') : '' ) . "
            */
            $sql = "
                SELECT  DISTINCT '<a title=\"Resenhas desta coleção.\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&colid=' || col.colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/alterar.gif\"  /></a>&nbsp;&nbsp;' as acao,
                        edt.edtnome as editora,
                        COALESCE(com.comdsc, 'Não cadastrado') as comdsc,

                        {$colunas_colecao}

                        CASE WHEN lvd.cpsid = 2
                            THEN '<img border=\"0\" align=\"absmiddle\" width=\"18\" src=\"../imagens/pnld/livro_32x32.png\"/>'
                            ELSE '<img border=\"0\" align=\"absmiddle\" width=\"18\" src=\"../imagens/pnld//dvd_32x32.png\"/>'
                        END AS cpsid_icone,

                        --cp.cpsdsc,
                        lvd.cpsid,

                        COALESCE(
                            (SELECT '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE colid  = col.colid
                            AND tpaid  = 1 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 1 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as aprovacao,

                        COALESCE(
                            (SELECT '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 2 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 2 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as aprovacaoCondicionada,

                        COALESCE(
                            (SELECT '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 3 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 3 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as exclusao,

                        (
                            SELECT '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_cinza.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 6 AND ravstatus = 'A' AND ( ravseq = 3 OR ravseq = 0 )
                        ) as recurso,

                        COALESCE(
                            (SELECT '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 7 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&colid=' || colid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE colid = col.colid AND tpaid = 7 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as resposta,

                        CASE WHEN lvd.cpsid = 2
                            THEN '<span style=\"color:#CC3300;\">Não se aplica</span>'
                            ELSE
                                COALESCE(
                                    (SELECT DISTINCT '<span style=\"color:#0060BF\">'||tpm_descricao FROM livro.resenhaavaliacao AS r JOIN livro.tipoparecermidia AS tpm ON tpm.tpm_id = r.tpm_id WHERE colid = col.colid AND ravseq = 0 AND ravstatus = 'A' AND r.tpm_id is not null),
                                    '-'
                                )
                        END as parecer_midia

                FROM livro.colecao col
                INNER JOIN livro.editora edt ON edt.edtid = col.edtid AND edt.edtstatus = 'A'
                INNER JOIN livro.livrodidatico lvd ON lvd.edtid = edt.edtid AND lvd.colid = col.colid

                LEFT JOIN livro.composicao AS cp ON cp.cpsid = lvd.cpsid

                LEFT JOIN livro.componente com ON com.comid = lvd.comid
                LEFT JOIN livro.resenharecurso rre ON rre.rreid = col.colid
                LEFT JOIN livro.resenhaavaliacao rav ON rav.colid = col.colid
                LEFT JOIN livro.tipoparecer tpa ON tpa.tpaid = rav.tpaid

                LEFT JOIN livro.tipoparecermidia AS tpm ON tpm.tpm_id = rav.tpm_id

                {$inner}

                WHERE col.colstatus = 'A' and col.prsano = '{$_SESSION['exercicio']}'
                    
                {$where} {$whereC}

                GROUP BY    rre.rredescricaorecurso, rre.rreid, rre.arqidparecercondicionado, rre.tpaid,
                            edt.edtid, edt.edtnome,
                            lvd.cpsid, lvd.rreid,
                            col.colid,
                            com.comdsc,
                            col.colcodigo,
                            col.coltitulo,
                            rav.tpm_id
            UNION ALL
                
                SELECT  DISTINCT '<a title=\"Resenhas desta coleção.\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&lvdid=' || lvd.lvdid || '\"><img border=\"0\" src=\"../imagens/alterar.gif\"  /></a>&nbsp;&nbsp;' as acao,
                        edt.edtnome as editora,
                        COALESCE(com.comdsc, 'Não cadastrado') as comdsc,

                        lvd.lvdcodigo AS codigo,
                        lvd.lvdtitulo AS colecao,

                        CASE WHEN lvd.cpsid = 2
                            THEN '<img border=\"0\" align=\"absmiddle\" width=\"18\" src=\"../imagens/pnld/livro_32x32.png\"/>'
                            ELSE '<img border=\"0\" align=\"absmiddle\" width=\"18\" src=\"../imagens/pnld/dvd_32x32.png\"/>'
                        END AS cpsid_icone,

                        --cp.cpsdsc,
                        lvd.cpsid,

                        COALESCE(
                            (SELECT '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 1 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Aprovação\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 1 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as aprovacao,

                        COALESCE(
                            (SELECT '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 2 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Aprovação Condicionada\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 2 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as aprovacaoCondicionada,

                        COALESCE(
                            (SELECT '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 3 AND ravseq = 0 AND ravstatus = 'A'),
                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Exclusão\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 3 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as exclusao,

                        (
                            SELECT '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_cinza.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE lvdid  = lvd.lvdid AND tpaid = 6 AND ravstatus = 'A' AND ( ravseq = 3 OR ravseq = 0 )
                        ) as recurso,

                        COALESCE(
                            (SELECT '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_verde.gif\"  /></a>&nbsp;&nbsp;'
                            FROM livro.resenhaavaliacao
                            WHERE lvdid  = lvd.lvdid AND tpaid  = 7 AND ravseq = 0 AND ravstatus = 'A'),

                            (SELECT CASE
                                        WHEN mod(ravseq, 2) > 0 THEN '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_amarelo.gif\"  /></a>&nbsp;&nbsp;'
                                        WHEN mod(ravseq, 2) = 0 THEN '<a title=\"Resposta\" href=\"livro.php?modulo=principal/pnld_resenha_colecao&acao=A&tpaid=' || tpaid || '&lvdid=' || lvdid || '\"><img border=\"0\" align=\"absmiddle\" src=\"../imagens/pnld/p_azul.gif\"  /></a>&nbsp;&nbsp;'
                                    END
                            FROM livro.resenhaavaliacao
                            WHERE lvdid = lvd.lvdid AND tpaid = 7 AND ravstatus = 'A'
                            ORDER by ravid DESC
                            LIMIT 1)
                        ) as resposta,

                        CASE WHEN lvd.cpsid = 2
                            THEN '<span style=\"color:red;\">Não se aplica</span>'
                            ELSE
                                COALESCE(
                                    (SELECT DISTINCT '<span style=\"color:#0060BF\">'||tpm_descricao||'</span>' FROM livro.resenhaavaliacao AS r JOIN livro.tipoparecermidia AS tpm ON tpm.tpm_id = r.tpm_id WHERE lvdid = lvd.lvdid AND ravseq = 0 AND ravstatus = 'A' AND r.tpm_id is not  null),
                                    '-'
                                )
                        END as parecer_midia

                FROM  livro.editora edt
                INNER JOIN livro.livrodidatico lvd ON  edt.edtid = lvd.edtid AND edt.edtstatus = 'A'

                LEFT JOIN livro.composicao cp ON cp.cpsid = lvd.cpsid

                LEFT JOIN livro.resenhaavaliacao rav ON rav.lvdid = lvd.lvdid

                LEFT JOIN livro.tipoparecermidia AS tpm ON tpm.tpm_id = rav.tpm_id

                LEFT JOIN livro.componente com ON  com.comid = lvd.comid {$whereReg[0]}
                LEFT JOIN livro.resenharecurso rre ON rre.rreid = lvd.rreid
                LEFT JOIN livro.tipoparecer tpa ON tpa.tpaid = rre.tpaid
                {$inner}
                {$innerColecao}
                WHERE lvd.prsano = '{$_SESSION['exercicio']}' AND   lvd.lvdtipo = 'L' {$whereReg[1]} and coalesce (lvd.colid, 0) = 0
                {$where} {$whereL}
                ORDER BY editora, comdsc, codigo, colecao
            ";
/*
" . (checkPerfil(array(PNLD_PERFIL_PARECERISTA), false) ? (is_array($_SESSION['pnld']['colid']) ? ' AND lvd.colid in (' . implode(',', $_SESSION['pnld']['colid']) . ')' : '') : '' ) . "
" . (checkPerfil(array(PNLD_PERFIL_COORDENADOR), false) ? (is_array($_SESSION['pnld']['comid']) ? ' AND lvd.comid in (' . implode(',', $_SESSION['pnld']['comid']) . ')' : '') : '' ) . "
*/                
            //ver($sql);
            $alinhamento = Array('center', 'left', 'left', 'center', 'left', 'center', 'center', 'center', 'center', 'center', 'center', 'center', 'center' );
            $tamanho = Array('2%', '10%', '5%', '2%', '12%', '2%', '2%', '2%', '2%', '2%', '2%', '2%', '2%' );
            $db->monta_lista($sql, $cabecalho, 50, 20, 'N', 'left', 'N', '', $tamanho, $alinhamento, '');
        ?>
    </tr>
</table>

<?
//menssagem de aviso
if ($_POST["editora_campo_flag"] != '0') {
        $texto = "
            <div style=\"font-size:14px\" >
                <center><b><font color=red><p>Programa Nacional do Livro Didático - PNLD</p></font></b></center>
                    <div style=\"margin: 0 auto; padding: 0; height: 260px; width: 100%; border: none;\" class=\"div_rolagem\">
                        <br>
                    	<p style=\"margin-left: 15px; margin-right: 15px;\">
                            Informamos aos detentores de direitos autorais de obras inscritas e avaliadas no Programa Nacional do Livro Didático 2015 que somente serão julgados pela Secretaria de Educação Básica os recursos interpostos com base no conteúdo expresso no Edital do PNLD 2015: “7.4.3.1. O parecer referente à análise da obra não aprovada, hipótese prevista no subitem 7.4.1.6.3, poderá ser objeto de recurso fundamentado por parte do editor, no prazo de dez dias, a contar da notificação, vedados pedidos genéricos de revisão da avaliação”.
                        </p>
                        <p style=\"margin-left: 15px; margin-right: 15px;\">
                            O prazo para interposição de recursos à avaliação será observado a partir da data de publicação da portaria 21 - Diário Oficial de 17 de junho de 2014. Cogeam/DCE/SEB/MEC. 20/06/2014.
                        </p>
                    </div>
              </div>
        ";
        //popupAlertaGeral($texto, '640px', "340px");
}
?>
