<?php

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $where = array();
    
    if (!empty($obrid)) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
        $where[] = " ( ( UPPER(public.removeacento(o.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                                    o.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
    }

    if (!empty($tpoid)) {
        $where[] = " o.tpoid = $tpoid ";
    }

    if (!empty($estuf)) {
        if (is_array($estuf)) {
            foreach ($estuf as $k => $v) {
                $estuf[$k] = "'{$v}'";
            }
        } else {
            $estuf = array("'{$estuf}'");
        }
        foreach ($estuf as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($estuf[$k]);
            }
        }
        if (!empty($estuf)) {
            $where[] = " m.estuf IN ( " . implode(",", $estuf) . " ) ";
        }
    }

    if (!empty($muncod)) {
        if (is_array($muncod)) {
            foreach ($muncod as $k => $v) {
                $muncod[$k] = "'{$v}'";
            }
        } else {
            $muncod = array("'{$muncod}'");
        }
        foreach ($muncod as $k => $v) {
            if (trim($v) == '' || $v == "''" || empty($v)) {
                unset($muncod[$k]);
            }
        }
        if (!empty($muncod)) {
            $where[] = " m.muncod IN ( " . implode(",", $muncod) . " ) ";
        }
    }

    if (!empty($esdid_obr) && is_array($esdid_obr) && $esdid_obr[0] != '') {
        $where[] = " et.esdid IN ( " . implode(",", $esdid_obr) . " ) ";
    }

    if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
        $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
    }

    if ( !empty($tooid) && count($tooid) > 0 && $tooid[0] !== ''){
        $where[] = " too.tooid IN('" . implode("', '", $tooid) . "')";
    }

    $strWhere = (!empty($where)) ? ' AND '.implode(' AND ', $where) : '';

    $sql = "SELECT 
                    o.obrid as \"ID\",
                    o.preid as \"Pré-ID\",
                    o.obrnome as \"Nome\",
                    et.esddsc as \"Situação\",
                    m.estuf as \"UF\",
                    m.mundescricao as \"Município\",
                    e.empesfera as \"Esfera\",
                    ent.entnome as \"Unidade Implantadora\",
                    CASE WHEN too.tooid = 2 THEN
                            o.obrnumprocessoconv
                    ELSE 
                            po.numeroprocesso
                    END as \"Nº Processo/Convênio\",
                    prf.prfdesc as \"Programa\",
                    too.toodescricao as \"Fonte\",
                    tpo.tpodsc as \"Tipologia\",
                    o.obridvinculado as \"ID Vinculada\",
                    o.obrperccontratoanterior as \"Perc. Contrato Anterior\",
                    coalesce(o.obrpercentultvistoria, 0) as \"Perc. Atual\",
                    ((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as \"Percentual Total\",
                    TO_CHAR(o.obrdtinclusao, 'DD/MM/YYYY') as obrdtinclusao,
                    usu.usunome --o.usucpfinclusao
            FROM obras2.obras o
            JOIN obras2.empreendimento e ON e.empid = o.empid
            LEFT JOIN obras2.programafonte      pf ON pf.prfid = e.prfid          
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN entidade.entidade ent ON ent.entid = e.entidunidade
            JOIN territorios.municipio m ON ed.muncod = m.muncod
            JOIN workflow.documento d ON d.docid = o.docid
            JOIN workflow.estadodocumento et ON et.esdid = d.esdid

            LEFT JOIN seguranca.usuario         usu ON o.usucpfinclusao = usu.usucpf

            LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
            LEFT JOIN obras2.programafonte prf ON prf.prfid  = e.prfid
            LEFT JOIN obras2.tipoorigemobra too ON e.tooid = too.tooid
            LEFT JOIN (SELECT po.pronumeroprocesso numeroprocesso , pop.preid
                    FROM par.processoobraspaccomposicao pop
                    JOIN par.processoobra po on po.proid = pop.proid and po.prostatus = 'A'
                    UNION 
                    SELECT po.pronumeroprocesso numeroprocesso, pop.preid
                    FROM par.processoobrasparcomposicao pop
                    JOIN par.processoobraspar po on po.proid = pop.proid and po.prostatus = 'A') po ON po.preid = o.preid

            WHERE 
            o.obridpai IS NULL 
            AND o.obrstatus = 'A' 
            AND o.obridvinculado IS NOT NULL
            ". $strWhere ."
            ORDER BY m.estuf, m.mundescricao;";

    return $sql;
}

// exibe consulta
if ($_REQUEST['tiporelatorio']) {
    
    
//    ver($_REQUEST,d);
    
    $sql = monta_sql();

    $cabecalho = array("ID", "Pré-ID", "Nome", "Situação", "UF", "Município", "Esfera", "Unidade Implantadora", "Nº Processo/Convênio",
        "Programa", "Fonte", "Tipologia", "ID Vinculada", "Perc. Contrato Anterior", "Perc. Atual", "Percentual Total", "Data Criação", "Usuário que criou a Obra");

    switch ($_REQUEST['tiporelatorio']) {
        case 'xls':
            ob_clean();
            ini_set("memory_limit", "512M");
            header("Content-type: application/excel; name=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            header("Content-Disposition: attachment; filename=ListaDeAtividadesDeMonitoramentoEspecial.xls");
            $db->sql_to_xml_excel($sql, 'rel_obrasmi', $cabecalho);
            die();
            break;
        case 'html':
            echo '<html>
                    <head>
                        <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                        <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                        <body>
                            <center>
                                    <!--  Cabeçalho Brasão -->
                                    '.monta_cabecalho_relatorio('95').' 
                            </center>
                ';
            $db->monta_lista($sql, $cabecalho, 30, 50, 'N', 'center', 'N', 'N');
            echo '</body>';
            die();
            break;
    }
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo("Relatório de Obras Vinculadas", 'Selecione os filtros');
?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('tooid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));
        selectAllOptions(document.getElementById('esdid_obr'));

        formulario.action = 'obras2.php?modulo=relatorio/relatorioObrasVinculadas&acao=A&tiporelatorio=' + tipo;
        window.open('', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
        formulario.target = 'relatorio';
        formulario.submit();
    }


    /* Função para substituir todos */
    function replaceAll(str, de, para) {
        var pos = str.indexOf(de);
        while (pos > -1) {
            str = str.replace(de, para);
            pos = str.indexOf(de);
        }
        return (str);
    }

    /**
     * Alterar visibilidade de um bloco.
     * 
     * @param string indica o bloco a ser mostrado/escondido
     * @return void
     */
    function onOffBloco(bloco)
    {
        var div_on = document.getElementById(bloco + '_div_filtros_on');
        var div_off = document.getElementById(bloco + '_div_filtros_off');
        var img = document.getElementById(bloco + '_img');
        var input = document.getElementById(bloco + '_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '0';
            img.src = '/imagens/menos.gif';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '1';
            img.src = '/imagens/mais.gif';
        }
    }

    /**
     * Alterar visibilidade de um campo.
     * 
     * @param string indica o campo a ser mostrado/escondido
     * @return void
     */
    function onOffCampo(campo)
    {
        var div_on = document.getElementById(campo + '_campo_on');
        var div_off = document.getElementById(campo + '_campo_off');
        var input = document.getElementById(campo + '_campo_flag');
        if (div_on.style.display == 'none')
        {
            div_on.style.display = 'block';
            div_off.style.display = 'none';
            input.value = '1';
        }
        else
        {
            div_on.style.display = 'none';
            div_off.style.display = 'block';
            input.value = '0';
        }
    }

//-->
</script>

<form action="" method="post" name="formulario" id="filtro"> 

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" style="width: 490px;">Nome da Obra/ID:</td>
            <td>
                <?php echo campo_texto('obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        <tr>
                <?php
                $sql_obr = "SELECT esdid as codigo, esddsc as descricao 
                            FROM workflow.estadodocumento 
                            WHERE tpdid='" . TPDID_OBJETO . "' 
                              AND esdstatus='A' 
                            ORDER BY esdordem";
                $stSqlCarregados = '';
                $arr_obr_wf = array();
                if (!empty($_POST['esdid_obr']) && is_array($_POST['esdid_obr']) && $_POST['esdid_obr'][0] != '') {
                    foreach ($_POST['esdid_obr'] as $key => $value) {
                        $arr_obr_wf[$key] = "'" . $value . "'";
                    }
                    $str_colecao = (!empty($arr_obr_wf)) ? " AND esdid IN (" . implode(',', $arr_obr_wf) . ") " : '';
                    $stSqlCarregados = "SELECT esdid as codigo, esddsc as descricao 
                                            FROM workflow.estadodocumento 
                                            WHERE tpdid='" . TPDID_OBJETO . "' 
                                              AND esdstatus='A' 
                                            {$str_colecao}
                                            ORDER BY
                                                    esdid";
                }
                mostrarComboPopup('Situação da Obra:', 'esdid_obr', $sql_obr, $stSqlCarregados, 'Selecione a(s) Situação(ões)', null, '', false);
                ?>
        </tr>
        <tr id="idUF">
        <?php
        #UF
        $ufSql = " SELECT 	estuf as codigo, estdescricao as descricao
                               FROM territorios.estado est
                               ORDER BY estdescricao
                             ";
        $stSqlCarregados = '';
        $arr_uf = array();
        if ($_POST['estuf'][0] != '') {
            foreach ($_POST['estuf'] as $key => $value) {
                $arr_uf[$key] = "'" . $value . "'";
            }
            $stSqlCarregados = "SELECT
                                                    estuf as codigo, estdescricao as descricao
                                            FROM territorios.estado est
                                            WHERE 
                                                    estuf IN (" . implode(',', $arr_uf) . ")
                                            ORDER BY
                                                    2";
        }
        mostrarComboPopup('UF:', 'estuf', $ufSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', null, 'ajaxEstado', false);
        ?>
        </tr>
        <tr id="idMunicipio">
            <?php
            #Municipio
            $munSql = " SELECT muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                FROM territorios.municipio 
                                ORDER BY
                                    mundescricao";
            $stSqlCarregados = '';
            $arr_muncod = array();
            if (is_array($_POST['muncod'])) {
                foreach ($_POST['muncod'] as $key => $value) {
                    $arr_muncod[$key] = "'" . $value . "'";
                }
            }

            $where_mun = (!empty($_POST['muncod'])) ? " muncod IN (" . implode(',', $arr_muncod) . ") " : '';
            $where_uf = (!empty($arr_uf)) ? " estuf IN (" . implode(',', $arr_uf) . ") " : '';

            $where = '';
            if (trim($where_mun) != '') {
                $where .= $where_mun;
            }

            if (trim($where_uf) != '' && trim($where) !== '') {
                $where .= ' AND ' . $where_uf;
            } elseif (trim($where_uf) !== '') {
                $where .= $where_uf;
            }

            if (trim($where) !== '') {
                $stSqlCarregados = "SELECT
                                                    muncod as codigo, estuf || ' - ' || mundescricao as descricao 
                                            FROM territorios.municipio
                                            WHERE 
                                                    {$where}
                                            ORDER BY
                                                    mundescricao";
            }
            mostrarComboPopup('Município:', 'muncod', $munSql, $stSqlCarregados, 'Selecione os municípios', null, '', false);
            ?>
        </tr>
            <?php
            // Programa
            $stSql = "SELECT
                    prfid AS codigo,
                    prfdesc AS descricao
                 FROM 
                    obras2.programafonte
                 ORDER BY
                    prfdesc ";
            if (!empty($_POST['prfid'][0])) {
                $stSqlSelecionados = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                     FROM 
                        obras2.programafonte
                     WHERE prfid IN (" . implode(', ', $_POST['prfid']) . ")
                     ORDER BY
                        prfdesc ";
            }
            mostrarComboPopup('Programa', 'prfid', $stSql, $stSqlSelecionados, 'Selecione o(s) Programa(s)');
            // Fonte
            $stSql = "SELECT
                    tooid AS codigo,
                    toodescricao AS descricao
                 FROM 
                    obras2.tipoorigemobra
                 WHERE
                    toostatus = 'A'
                 ORDER BY
                    toodescricao ";

            if (!empty($_POST['tooid'][0])) {
                $sql_carregados = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                     FROM 
                        obras2.tipoorigemobra
                     WHERE
                        toostatus = 'A'
                        AND tooid IN (" . implode(', ', $_POST['tooid']) . ")
                     ORDER BY
                        toodescricao ";
            }
            mostrarComboPopup('Fonte', 'tooid', $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)');
            ?>     
        <tr>
            <td class="subtituloDireita" style="width: 40%;">Tipologia:</td>
            <td>
        <?php
        $tipologiaObra = new TipologiaObra();
        $param = array("orgid" => $_SESSION['obras2']['orgid']);
        $val = (!empty($_POST['tpoid'])) ? $_POST['tpoid'] : '';
        $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid", false, $val);
        ?>
            </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC"></td>
            <td bgcolor="#CCCCCC">
                <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/> 
                <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
            </td>
        </tr>
    </table>
    
</form>