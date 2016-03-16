<?php

function monta_sql() {
    global $db;

    extract($_REQUEST);

    $where = array();
    
    if (!empty($obrid)) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($obrid))));
        $where[] = " ( o.obrnome::CHARACTER VARYING ILIKE ('%" . $obrbuscatextoTemp . "%') OR o.obrid::CHARACTER VARYING ILIKE ('%" . $obrid . "%') ) ";
    }
    
    if (!empty($preid)) {
        $preidbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($preid))));
        $where[] = " ( o.preid::CHARACTER VARYING ILIKE ('%" . $preidbuscatextoTemp . "%')  ) ";
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
            $where[] = " mun.estuf IN ( " . implode(",", $estuf) . " ) ";
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
            $where[] = " mun.muncod IN ( " . implode(",", $muncod) . " ) ";
        }
    }

    if ( !empty($prfid) && count($prfid) > 0 && $prfid[0] !== ''){
        $where[] = "pf.prfid IN('" . implode("', '", $prfid) . "')";
    }

    if ( !empty($tooid) ){
        $where[] = " vm.\"Fonte\" = '".$tooid."'";
    }

    $strWhere = (!empty($where)) ? ' AND '.implode(' AND ', $where) : '';

    $sql = "SELECT
                  vm.*
                FROM obras2.vm_termo_obras vm
                JOIN obras2.obras o ON o.obrid = vm.\"ID Obra\"
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN entidade.endereco ed ON ed.endid = o.endid
                LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid

                WHERE
                1 = 1
                 ".$strWhere."

				";
    return $sql;
}

// exibe consulta
if ($_REQUEST['tiporelatorio']) {
    
    $sql = monta_sql();
    
//    ver($sql);

    $cabecalho = array("ID Obra", "Preid", "Número do Termo", "Nome", "Município", "UF", "Fonte", "Programa", "Situação", "% Ult. Vistoria",
                       "Dt. Ult. Vistoria", "Data Primeiro Pagamento", "Data Último Pagamento", "Pagamento Solicitado Por", "Início Vigência da Obra",
                       "Fim Vigência da Obra", "Houve Prorrogação?", "Prorrogação Solicitada Por", "Data de Validação do Termo", "Validado Por",
                       "Início Vigência Termo", "Fim Vigência Termo");

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
monta_titulo("Relatório do Vencimento do Termo", 'Selecione os filtros');
?>

<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>

<script type="text/javascript">


    function exibeRelatorioGeral(tipo) {
        var formulario = document.formulario;
        selectAllOptions(document.getElementById('prfid'));
        selectAllOptions(document.getElementById('estuf'));
        selectAllOptions(document.getElementById('muncod'));

        formulario.action = 'obras2.php?modulo=relatorio/relatorioVencimentoTermo&acao=A&tiporelatorio=' + tipo;
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
            <td class="SubTituloDireita" style="width: 490px;">Pré-ID da Obra:</td>
            <td>
                <?php echo campo_texto('preid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
            </td>
        </tr>
        
        <tr id="idUF">
        <?php
        #UF
        $ufSql = " SELECT estuf as codigo, estdescricao as descricao
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
        ?>
        <tr>
            <td class="SubTituloDireita" style="width: 490px;">Fonte:</td>
            <td>
                <?php 
                    $arrayCombo = array( array("codigo" => "PAC", "descricao" => "PAC"),
                                         array("codigo" => "PAR", "descricao" => "PAR")
                                       );
                    $db->monta_combo( "tooid", $arrayCombo, "S", "Selecione", "", "", "", "", "N", "tooid" );	
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