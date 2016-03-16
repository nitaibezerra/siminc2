<?php
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

monta_titulo('Relatório de Controle de Financiamento - EJA', 'Integrada a qualificação profissional');

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

    function gerarRelatorio( parm ){
        if(parm == 'S'){
            $('#relatorio_excel').val('S');
        }else{
            $('#relatorio_excel').val('N');        
        }
        $('#formulario').submit();
    }

    function habilitaComboMun( param ){
        var habComboMun = trim(param);

        if( habComboMun == 'S' ){
            /*HABILITA O COMBO DE MUNICÍPIO*/
            $('#muncod').attr("disabled", false);

            /*HABILITA OS CAMPOS RELACIONADOS COM A ESFERA MUNICÍPAL*/
            $('#tr_municipal_n_pri').css('display', '');
            $('#tr_municipal_campo').css('display', '');
            $('#tr_municipal_quilombo').css('display', '');
            $('#tr_municipal_indigina').css('display', '');
            $('#tr_municipal_privada').css('display', '');

            /*DESABILITA OS CAMPOS RELACIONADOS COM A ESFERA ESTADUAL*/
            $('#tr_estadual_n_pri').css('display', 'none');
            $('#tr_estadual_campo').css('display', 'none');
            $('#tr_estadual_quilombo').css('display', 'none');
            $('#tr_estadual_indigina').css('display', 'none');
            $('#tr_estadual_privada').css('display', 'none');

        }else{
            /*DESABILITA O COMBO DE MUNICÍPIO*/
            $('#muncod').attr("disabled", true);

            /*DESABILITA OS CAMPOS RELACIONADOS COM A ESFERA MUNICÍPAL*/
            $('#tr_municipal_n_pri').css('display', 'none');
            $('#tr_municipal_campo').css('display', 'none');
            $('#tr_municipal_quilombo').css('display', 'none');
            $('#tr_municipal_indigina').css('display', 'none');
            $('#tr_municipal_privada').css('display', 'none');

            /*HABILITA OS CAMPOS RELACIONADOS COM A ESFERA ESTADUAL*/
            $('#tr_estadual_n_pri').css('display', '');
            $('#tr_estadual_campo').css('display', '');
            $('#tr_estadual_quilombo').css('display', '');
            $('#tr_estadual_indigina').css('display', '');
            $('#tr_estadual_privada').css('display', '');
        }
    }
</script>

<form name="formulario" id="formulario" action="" method="post">
    <input type="hidden" id="relatorio_excel" name="relatorio_excel" value="">
    
    <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;" border="0">
        <tr id="tr_municipal_n_pri">
            <td class="SubTituloDireita">Matrícula público não prioritário:</td>
            <td class="SubTituloDireita" rowspan="5" width="10%">EJA Integrada a qualificação</td>
            <td>
                <input id="qejpubliconpriorejaqualiprof" name="qejpubliconpriorejaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
            </td>
        </tr>
        <tr id="tr_municipal_campo">
            <td class="SubTituloDireita">B - Estudades das comunidades do Campo:</td>
            <td>
                <input id="qejcampoejaqualiprof" name="qejcampoejaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
            </td>
        </tr>
        <tr id="tr_municipal_quilombo">
            <td class="SubTituloDireita">C - Quilombolas:</td>
            <td>
                <input id="qejquilombolaqualiprof" name="qejquilombolaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
            </td>
        </tr>
        <tr id="tr_municipal_indigina">
            <td class="SubTituloDireita">D - Indígenas:</td>
            <td>
                <input id="qejindigenaqualiprof" name="qejindigenaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
            </td>
        </tr>
        <tr id="tr_municipal_privada">
            <td class="SubTituloDireita">E - Pessoas privadas de Liberdade: </td>
            <td>
                <input id="qejprivliberqualiprof" name="qejprivliberqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
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
                        WHERE tpdid=" . TPD_QUESTIONARIO_EJA . "
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
                    $db->monta_combo('muncod', $sql, 'S', "Todos", '', '', '', 260, 'S', 'muncod', '');
                ?>
            </td>
        </tr>
        <tr>
            <td colspan="3" class="SubTituloCentro">
                <input type="button" class="pesquisar" value="Pesquisar" onclick="gerarRelatorio('N');"/>
                <input type="button" class="pesquisar" value="Gerar Relatório XLS" onclick="gerarRelatorio('S');"/>
            </td>
        </tr>

    </table>
</form>

<?PHP
    extract($_REQUEST);
    
    if( $relatorio_excel == 'S' ){
        $coluna = "m.mundescricao AS mundescricao,";
    }else{
        $coluna = "'<a style=cursor:pointer; onclick=direcionarQuest('||i.inuid||','||p.pfaid||')>' || m.mundescricao || '</a>' AS mundescricao,";
    }    

    if ($esdid) {
        $aryWhere[] = "ee.esdid = {$esdid}";
    }

    if ($estuf) {
        $aryWhere[] = "m.estuf = '{$estuf}'";
    }

    if ($muncod) {
        $aryWhere[] = "m.muncod = '{$muncod}'";
    }

    
    if( !$qejpubliconpriorejaqualiprof && !$qejcampoejaqualiprof && !$qejquilombolaqualiprof && !$qejindigenaqualiprof && !$qejprivliberqualiprof ){
        $arrWhere[] = " qejpubliconpriorejaqualiprof > 0 OR qejcampoejaqualiprof > 0 OR qejquilombolaqualiprof > 0 OR qejindigenaqualiprof > 0 OR qejprivliberqualiprof > 0 ";
    }

    #Matrícula público não prioritário
    if($qejpubliconpriorejaqualiprof == 'S'){
        $arrWhere[] = " qejpubliconpriorejaqualiprof > 0 ";
    }

    #B - Estudades das comunidades do Campo
    if($qejcampoejaqualiprof == 'S'){
        $arrWhere[] = " qejcampoejaqualiprof > 0 ";
    }

    #C - Quilombolas
    if($qejquilombolaqualiprof == 'S'){
        $arrWhere[] = " qejquilombolaqualiprof > 0 ";
    }

    #D - Indígenas
    if($qejindigenaqualiprof == 'S'){
        $arrWhere[] = " qejindigenaqualiprof > 0 ";
    }

    #E - Pessoas privadas de Liberdade
    if($qejprivliberqualiprof == 'S'){
        $arrWhere[] = " qejprivliberqualiprof > 0 ";
    }

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
        SELECT  DISTINCT m.estuf AS estado,
                m.muncod,
                {$coluna}
                
                (COALESCE(qejpublicoprioritariobraalf,0) + COALESCE(tnp.total_nao_prior,0) + COALESCE(tc.total_campo,0) + COALESCE(tq.total_quilombolas,0) + COALESCE(ti.total_indigenas,0) + COALESCE(tl.total_liberdade,0)) AS total_geral

        FROM eja.questionarioeja q

        INNER JOIN(
            SELECT  inuid,
                    CASE WHEN qejpubliconpriorejaqualiprof = 0
                        THEN (qejpubliconprioranosiniciais + qejpubliconprioranosfinais + qejpubliconpriorejaqualiprof_fund + qejpubliconpriorejaqualiprof_medi)
                        ELSE (qejpubliconprioranosiniciais + qejpubliconprioranosfinais + qejpubliconpriorejaqualiprof)
                    END AS total_nao_prior
            FROM eja.questionarioeja
        ) AS tnp ON tnp.inuid = q.inuid

        INNER JOIN(
            SELECT  inuid,
                    CASE WHEN qejcampoejaqualiprof = 0
                        THEN (qejcampoanosiniciais+qejcampoanosfinais+qejcampoejaqualiprof_fund+qejcampoejaqualiprof_medi)
                        ELSE (qejcampoanosiniciais+qejcampoanosfinais+qejcampoejaqualiprof)
                    END AS total_campo
            FROM eja.questionarioeja
        ) AS tc on tc.inuid = q.inuid

        INNER JOIN(
            SELECT  inuid,
                    CASE WHEN qejquilombolaqualiprof = 0
                        THEN (qejquilombolaanosiniciais+qejquilombolaanosfinais+qejquilombolaqualiprof_fund+qejquilombolaqualiprof_medi)
                        ELSE (qejquilombolaanosiniciais+qejquilombolaanosfinais+qejquilombolaqualiprof)
                    END AS total_quilombolas
            FROM eja.questionarioeja
        ) AS tq ON tq.inuid = q.inuid

        INNER JOIN(
            SELECT  inuid,
                    CASE WHEN qejindigenaqualiprof = 0
                        THEN (qejindigenasanosiniciais+qejindigenasanosfinais+qejindigenaqualiprof_fund+qejindigenaqualiprof_medi)
                        ELSE (qejindigenasanosiniciais+qejindigenasanosfinais+qejindigenaqualiprof)
                    END AS total_indigenas
            FROM eja.questionarioeja
        ) AS ti ON ti.inuid = q.inuid

        INNER JOIN(
            SELECT  inuid,
                    CASE WHEN qejprivliberqualiprof = 0
                        THEN (qejprivliberanosiniciais+qejprivliberanosfinais+qejprivliberqualiprof_fund+qejprivliberqualiprof_medi)
                        ELSE (qejprivliberanosiniciais+qejprivliberanosfinais+qejprivliberqualiprof)
                    END AS total_liberdade
            FROM eja.questionarioeja
        ) AS tl ON tl.inuid = q.inuid

        INNER JOIN(
            SELECT  inuid,
                    ( COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0) ) AS qejvlrmec_total,
                    ( COALESCE(qejvlrmecpublnaopri,0) + COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0) ) AS qejvlrmec_total_geral,
                    ( (COALESCE(qejvlrmecpublnaopri,0) + COALESCE(qejvlrmecegresso,0) + COALESCE(qejvlrmeccomcampo,0) + COALESCE(qejvlrmecquilombolas,0) + COALESCE(qejvlrmecindigenas, 0) + COALESCE(qejvlrmecpessoaliberdade, 0)) * 1777.38 ) AS qejvlrmec_valor_liberado
            FROM eja.questionarioeja
        ) AS vmec ON vmec.inuid = q.inuid

        INNER JOIN par.instrumentounidade AS i ON i.inuid = q.inuid
        INNER JOIN par.pfadesaoprograma AS p ON p.inuid = q.inuid AND p.adpano = 2013 AND pfaid = 19

        INNER JOIN territorios.municipio m ON m.muncod = i.muncod

        LEFT JOIN eja.ejamatriculascenso mat ON mat.muncod = i.muncod

        INNER JOIN workflow.documento AS d ON d.docid = p.docid
        INNER JOIN workflow.estadodocumento AS ee ON ee.esdid = d.esdid

        {$WHERE}
            
        ORDER BY m.estuf, mundescricao
    ";
    $cabecalho = array("UF", "Cód. IBGE", "UF/ Município", "Total Geral de matrículas Prioritários/ Não prioritários");
    $alinhamento = Array('left', 'left', 'left', 'right' );
    $tamanho = Array('5%', '10%', '65%', '20%' );
    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento, '');
    
    
    if( $relatorio_excel == 'S' ){
        $dadosExcel = $db->carregar($sql);
        
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	$db->sql_to_excel($dadosExcel, 'Relatório_Qualificacao_Profissional_EJA', $cabecalho);
    }    
    
?>