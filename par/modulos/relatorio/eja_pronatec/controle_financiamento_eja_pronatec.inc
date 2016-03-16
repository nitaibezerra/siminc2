<?php
//qualificacao_profissional_eja
function atualizaComboMun( $dados ){
    global $db;

    $estuf = $dados['estuf'];
    $disab = $dados['disab'];

    $sql = "
        SELECT  muncod as codigo,
                mundescricao as descricao
        FROM territorios.municipio
        WHERE estuf = '{$estuf}'
        ORDER BY mundescricao";

    $db->monta_combo('muncod', $sql, $disab, "Todos", '', '', '', 260, 'S', 'muncod', '');
    die();
}

function redirecionarQuestionario($dados) {
    global $db;

    $sql = "SELECT * FROM par.instrumentounidade WHERE inuid='" . $dados['inuid'] . "'";
    $arr = $db->pegaLinha($sql);

    if ($arr) {
        $_SESSION['par']['itrid'] = $arr['itrid'];
        $_SESSION['par']['inuid'] = $arr['inuid'];
        $_SESSION['par']['estuf'] = $arr['estuf'];
        $_SESSION['par']['muncod'] = $arr['muncod'];
    }

    $sql = "SELECT * FROM par.pfadesao WHERE pfaid='" . $dados['pfaid'] . "'";
    $arr = $db->pegaLinha($sql);

    if ($arr) {
        $_SESSION['par']['pfaid'] = $arr['pfaid'];
        $_SESSION['par']['prgid'] = $arr['prgid'];
        $_SESSION['par']['pfaano'] = $arr['pfaano'];
    }

    echo "<script>window.location='par.php?modulo=principal/programas/feirao_programas/termoadesao&acao=A&pfaid=" . $dados['pfaid'] . "';</script>";
}

if($_REQUEST['requisicao']){
    $_REQUEST['requisicao']($_REQUEST);
    exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

define("TPD_QUESTIONARIO_EJA", 84);

monta_titulo('Relatório de Controle de Financiamento - EJA PRONATEC', '&nbsp;');

if ($_REQUEST['itrid'] == '' || $_REQUEST['itrid'] == 1){
    $_REQUEST['itrid'] = 1;
    $habilita = 'N';
}else{
    $_REQUEST['itrid'] = 2;
    $habilita = 'S';
}

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script>
    function atualizaComboMun( estuf ){
        var disab;

        if( $('#muncod').attr('disabled') == true ){
            disab = 'N';
        } else {
            disab = 'S';
        }

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMun&estuf="+estuf+'&disab='+disab,
            async: false,
            success: function(resp){
                $('#td_muncod').html(resp);
            }
        });
    }

    function direcionarQuest(inuid, pfaid) {
        window.location = 'par.php?modulo=relatorio/financiamento_eja/controleFinanciamento&acao=A&requisicao=redirecionarQuestionario&inuid=' + inuid + '&pfaid=' + pfaid;
    }

    function habilitaComboMun( param ){
        var habComboMun = trim(param);

        if( habComboMun == 'S' ){
            /*HABILITA O COMBO DE MUNICÍPIO*/
            $('#muncod').attr("disabled", false);
            //$('#muncod').attr("name", 'muncod');

        }else{
            /*DESABILITA O COMBO DE MUNICÍPIO*/
            $('#muncod').attr("disabled", true);
        }
    }

    function gerarRelatorio( parm ){
        if(parm == 'S'){
            $('#relatorio_excel').val('S');
        }else{
            $('#relatorio_excel').val('N');
        }
        $('#formulario').submit();
    }

</script>

<form name="formulario" id="formulario" action="" method="post">
    <input type="hidden" id="relatorio_excel" name="relatorio_excel" value="">

    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;" border="0">
        <tr>
            <td width="35%" class="SubTituloDireita" colspan="2">Esfera:</td>
            <td>
                <input id="estadual" name="itrid" type="radio" value="1" onclick="habilitaComboMun('N');" <?php echo (($_REQUEST['itrid'] == 1) ? "checked" : "") ?>> Estadual
                <input id="municipal" name="itrid" type="radio" value="2" onclick="habilitaComboMun('S');" <?php echo (($_REQUEST['itrid'] == 2) ? "checked" : "") ?>> Municipal
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2">Estado do workflow:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = ".WF_FLUXO_EJA_PRONATEC."
                        ORDER BY esdordem
                    ";
                    $db->monta_combo('esdid', $sql, 'S', "Todos", '', '', '', 260, 'S', 'esdid', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2">UF:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  estuf as codigo,
                            	estuf as descricao
                        FROM territorios.estado
                        ORDER BY estuf
                    ";
                    $db->monta_combo('estuf', $sql, 'S', "Todos", 'atualizaComboMun', '', '', 260, 'S', 'estuf', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" colspan="2">Município:</td>
            <td id="td_muncod">
                <?php
                    $sql = "
                        SELECT  muncod as codigo,
                               	mundescricao as descricao
                        FROM territorios.municipio
                        ORDER BY mundescricao
                    ";
                    $db->monta_combo('muncod', $sql, $habilita, "Todos", '', '', '', 260, 'S', 'muncod', '');
                ?>
            </td>
        </tr>
        <!--
        <tr>
            <td class="SubTituloDireita" colspan="2">Demanda de Vagas (Média Censo):</td>
            <td>
                <input id="demanda" name="demanda" type="checkbox" value="S" <?php echo (($_REQUEST['demanda'] == 'S') ? "checked" : "") ?>>
            </td>
        </tr>
        -->
        <tr>
            <td colspan="3" class="SubTituloCentro">
                <input type="button" class="pesquisar" value="Pesquisar" onclick="gerarRelatorio('N');"/>
                <input type="submit" value="Ver Todos"/>
                <input type="button" class="pesquisar" value="Gerar Relatório XLS" onclick="gerarRelatorio('S');"/>
            </td>
        </tr>

    </table>
</form>

<?PHP
    #VERIFICA A ESFERA E CASO SEJA MUNICÍPAL CHAMA A FUNÇÃO "habilitaComboMun" PARA DESABILITAR OS CAMPOS ESTADUAIS E HABILITAR OS MUNICIPAIS.
    if($_REQUEST['itrid'] == 2){
        echo "<script>habilitaComboMun('S');</script>";
    }

    extract($_REQUEST);

    #CABEÇALHO.
    $coluna_a = array(
        'label' => 'A - Atendidos no Programa Brasil Alfabetizado',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_b = array(
        'label' => 'B - Populações do Campo',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_c = array(
        'label' => 'C - Quilombolas',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_d = array(
        'label' => 'D - Indiginas',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_e = array(
        'label' => 'E - Pessoas que cumprem pena em privação de liberdade',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_f = array(
        'label' => 'F - Jovem em Cumprimento de Medidas Socioeducativas',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_g = array(
        'label' => 'G - Catadores de Materiais Recicláveis',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_h = array(
        'label' => 'H - Populaçao em situação de Rua',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_i = array(
        'label' => 'I - Pescadores e Aquicultores',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    $coluna_QT = array(
        'label' => 'Quantitativo Previsto',
        'colunas' => array(
            'Trabalhadores rurais empregados'
        )
    );

    $coluna_total = array(
        'label' => 'Total Geral',
        'colunas' => array(
            'Fundamental', 'Médio'
        )
    );

    if ($_REQUEST['itrid'] == 1) {
        $JOIN = "INNER JOIN territorios.estado e ON e.estuf = i.estuf";

        /*
        $JOINESFERA = "
            LEFT JOIN (
                SELECT  SUM(m.emc2010) AS emc2010,
                        SUM(m.emc2011) AS emc2011,
                        SUM(m.emc2012) AS emc2012,
                        SUM(m.emc2013) AS emc2013,
                        mc.estuf
                FROM eja.ejamatriculascenso m
                INNER JOIN territorios.municipio mc ON m.muncod = mc.muncod
                GROUP BY estuf
            ) AS mat ON mat.estuf = i.estuf
        ";
        */

        if( $relatorio_excel == 'S' ){
            $COLUNA = "
                e.estuf AS estado,
                e.estdescricao AS estdescricao,
            ";
            $cabecalho = array(
                "UF",
                "UF/ Município",
                "Publico nao prioritario",
                "A - Atendidos no Programa Brasil Alfabetizado - Fundamental",
                "A - Atendidos no Programa Brasil Alfabetizado - Médio",

                "B - Populações do Campo - Fundamental",
                "B - Populações do Campo - Médio",

                "C - Quilombolas - Fundamental",
                "C - Quilombolas - Médio",

                "D - Indiginas - Fundamental",
                "D - Indiginas - Médio",

                "E - Pessoas que cumprem pena em privação de liberdade - Fundamental",
                "E - Pessoas que cumprem pena em privação de liberdade - Médio",

                "F - Jovem em Cumprimento de Medidas Socioeducativas - Fundamental",
                "F - Jovem em Cumprimento de Medidas Socioeducativas - Médio",

                "G - Catadores de Materiais Recicláveis - Fundamental",
                "G - Catadores de Materiais Recicláveis - Médio",

                "H - Populaçao em situação de Rua - Fundamental",
                "H - Populaçao em situação de Rua - Médio",

                "I - Pescadores e Aquicultores - Fundamental",
                "I - Pescadores e Aquicultores - Médio",

                "Trabalhadores rurais empregados",

                "Total Geral - Fundamental",
                "Total Geral - Médio",

                "Estado do Workflow"
            );
        }else{
            $COLUNA = "
                e.estuf AS estado,
                '<a style=cursor:pointer; onclick=direcionarQuest('||i.inuid||','||p.pfaid||')>' || e.estdescricao || '</a>' AS estdescricao,
            ";

            $cabecalho = array(
                "UF",
                "UF/ Município",
                "Publíco não Prioritário",
                $coluna_a,
                $coluna_b,
                $coluna_c,
                $coluna_d,
                $coluna_e,
                $coluna_f,
                $coluna_g,
                $coluna_h,
                $coluna_i,
                $coluna_QT,
                $coluna_total,
                "Estado do Workflow"
            );
        }

        $ORDERBY = "ORDER BY e.estuf";

    } elseif ($_REQUEST['itrid'] == 2) {
        $JOIN = "INNER JOIN territorios.municipio m ON m.muncod = i.muncod";

        //$JOINESFERA = "LEFT JOIN eja.ejamatriculascenso mat ON mat.muncod = i.muncod";

        if( $relatorio_excel == 'S' ){
            $COLUNA = "
                m.estuf AS estado,
                m.muncod,
                m.mundescricao AS mundescricao,
            ";

            $cabecalho = array(
                "UF",
                "Cód. IBGE",
                "UF/ Município",
                "Publico nao prioritario",
                "A - Atendidos no Programa Brasil Alfabetizado - Fundamental",
                "A - Atendidos no Programa Brasil Alfabetizado - Médio",

                "B - Populações do Campo - Fundamental",
                "B - Populações do Campo - Médio",

                "C - Quilombolas - Fundamental",
                "C - Quilombolas - Médio",

                "D - Indiginas - Fundamental",
                "D - Indiginas - Médio",

                "E - Pessoas que cumprem pena em privação de liberdade - Fundamental",
                "E - Pessoas que cumprem pena em privação de liberdade - Médio",

                "F - Jovem em Cumprimento de Medidas Socioeducativas - Fundamental",
                "F - Jovem em Cumprimento de Medidas Socioeducativas - Médio",

                "G - Catadores de Materiais Recicláveis - Fundamental",
                "G - Catadores de Materiais Recicláveis - Médio",

                "H - Populaçao em situação de Rua - Fundamental",
                "H - Populaçao em situação de Rua - Médio",

                "I - Pescadores e Aquicultores - Fundamental",
                "I - Pescadores e Aquicultores - Médio",

                "Trabalhadores rurais empregados",

                "Total Geral - Fundamental",
                "Total Geral - Médio",

                "Estado do Workflow"
            );
        }else{
            $COLUNA = "
                m.estuf AS estado,
                m.muncod,
                '<a style=cursor:pointer; onclick=direcionarQuest('||i.inuid||','||p.pfaid||')>' || m.mundescricao || '</a>' AS mundescricao,
            ";

            $cabecalho = array(
                "UF",
                "Cód. IBGE",
                "UF/ Município",
                "Publíco não Prioritário",
                $coluna_a,
                $coluna_b,
                $coluna_c,
                $coluna_d,
                $coluna_e,
                $coluna_f,
                $coluna_g,
                $coluna_h,
                $coluna_i,
                $coluna_QT,
                $coluna_total,
                "Estado do Workflow"
            );
        }

        $ORDERBY = "ORDER BY m.estuf, mundescricao";

    }

    if ($esdid) {
        $aryWhere[] = "ee.esdid = {$esdid}";
    }

    if ($estuf && $itrid == 1) {
        $aryWhere[] = "e.estuf = '{$estuf}'";
    }

    if ($estuf && $itrid == 2) {
        $aryWhere[] = "m.estuf = '{$estuf}'";
    }

    if ($muncod) {
        $aryWhere[] = "m.muncod = '{$muncod}'";
    }

    #WHERE
    if( is_array($aryWhere) || is_array($arrWhere) ){
        $WHERE = "WHERE ";
    }

    if( is_array($aryWhere) ){
        $WHERE .= (is_array($aryWhere) ? implode(' AND ', $aryWhere) : '');
    }

    if( is_array($aryWhere) && is_array($arrWhere) ){
        $WHERE .= " AND ";
    }

    if( is_array($arrWhere) ){
        $WHERE .= (is_array($arrWhere) ? '('.implode(' AND ', $arrWhere).')' : '');
    }

    $sql = "
        SELECT  DISTINCT {$COLUNA}
        --SELECT
                qeppnpensinofund,
                --A,
                qepppbraalfensfund,
                qepppbraalfensmedio,
                --B,
                qeppppopcampoensfund,
                qeppppopcampoensmedio,
                --C,
                qepppquilombolaensfund,
                qepppquilombolaensmedio,
                --D,
                qepppindigenasensfund,
                qepppindigenasensmedio,
                --E,
                qepppprovliberensfund,
                qepppprovliberensmedio,
                --F,
                qepppmedsocioensfund,
                qepppmedsocioensmedio,
                --G,
                qepppmatreciclensfund,
                qepppmatreciclensmedio,
                --H,
                qepppsitruaensfund,
                qepppsitruaensmedio,
                --I,
                qeppppescadoresensfund,
                qeppppescadoresensmedio,

                --J,
                qepquestao01qtd as quantidade,

                --TOTAL GERAL
                (
                    qeppnpensinofund+qepppbraalfensfund+qeppppopcampoensfund+qepppquilombolaensfund+
                    qepppindigenasensfund+qepppprovliberensfund+qepppmedsocioensfund+qepppmatreciclensfund+
                    qepppsitruaensfund+qeppppescadoresensfund
                ) AS total_geral_fundamental,

                (
                    qepppbraalfensmedio+qeppppopcampoensmedio+qepppquilombolaensmedio+qepppindigenasensmedio+
                    qepppprovliberensmedio+qepppmedsocioensmedio+qepppmatreciclensmedio+qepppsitruaensmedio+
                    qeppppescadoresensmedio
                ) AS total_geral_medio,

                --wf
                ee.esddsc

        FROM eja.questionarioejapronatec AS q

        INNER JOIN par.instrumentounidade AS i ON i.inuid = q.inuid
        INNER JOIN par.pfadesaoprograma AS p ON p.inuid = q.inuid AND p.adpano = 2014 AND pfaid = 22

        {$JOIN}

        {$JOINESFERA}

        INNER JOIN workflow.documento AS d ON d.docid = p.docid
        INNER JOIN workflow.estadodocumento AS ee ON ee.esdid = d.esdid

        {$WHERE}

        $ORDERBY
    ";
    $db->monta_lista($sql, $cabecalho, '100', '10', 'N', 'center');

    if( $relatorio_excel == 'S' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');

	$db->sql_to_excel($sql, 'Relatório_Controle_Financeiro_EJA', $cabecalho);
    }
?>