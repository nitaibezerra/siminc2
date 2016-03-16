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
print '<br>';

monta_titulo('Relatório de Controle de Financiamento - EJA', '&nbsp;');

if ($_REQUEST['itrid'] == '' || $_REQUEST['itrid'] == 1){
    $_REQUEST['itrid'] = 1;
    $habilita = 'N';
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
        
        <!-- Inicio Estadual-->
            <tr id="tr_estadual_n_pri">
                <td class="SubTituloDireita">Matrícula público não prioritário:</td>
                <td class="SubTituloDireita" rowspan="5" width="10%">EJA Integrada a qualificação</td>
                <td>
                    <input id="qejpubliconpriorejaqualiprof_fund" name="qejpubliconpriorejaqualiprof_fund" type="checkbox" value="F">  Profissional/ Fundamental
                    <input id="qejpubliconpriorejaqualiprof_medi" name="qejpubliconpriorejaqualiprof_medi" type="checkbox" value="M"> Profissional/ Médio
                </td>
            </tr>
            <tr id="tr_estadual_campo">
                <td class="SubTituloDireita">B - Estudades das comunidades do Campo:</td>
                <td>
                    <input id="qejcampoejaqualiprof_fund" name="qejcampoejaqualiprof_fund" type="checkbox" value="F"> Profissional/ Fundamental
                    <input id="qejcampoejaqualiprof_medi" name="qejcampoejaqualiprof_medi" type="checkbox" value="M"> Profissional/ Médio
                </td>
            </tr>
            <tr id="tr_estadual_quilombo">
                <td class="SubTituloDireita">C - Quilombolas:</td>
                <td>
                    <input id="qejquilombolaqualiprof_fund" name="qejquilombolaqualiprof_fund" type="checkbox" value="F"> Profissional/ Fundamental
                    <input id="qejquilombolaqualiprof_medi" name="qejquilombolaqualiprof_medi" type="checkbox" value="M"> Profissional/ Médio
                </td>
            </tr>
            <tr id="tr_estadual_indigina">
                <td class="SubTituloDireita">D - Indígenas:</td>
                <td>
                    <input id="qejindigenaqualiprof_fund" name="qejindigenaqualiprof_fund" type="checkbox" value="F"> Profissional/ Fundamental
                    <input id="qejindigenaqualiprof_medi" name="qejindigenaqualiprof_medi" type="checkbox" value="M"> Profissional/ Médio
                </td>
            </tr>
            <tr id="tr_estadual_privada">
                <td class="SubTituloDireita">E - Pessoas privadas de Liberdade: </td>
                <td>
                    <input id="qejprivliberqualiprof_fund" name="qejprivliberqualiprof_fund" type="checkbox" value="F"> Profissional/ Fundamental
                    <input id="qejprivliberqualiprof_medi" name="qejprivliberqualiprof_medi" type="checkbox" value="M"> Profissional/ Médio
                </td>
            </tr>
        <!-- fim -->

        <!-- Inicio Municipal-->
            <tr id="tr_municipal_n_pri" style="display: none;">
                <td class="SubTituloDireita">Matrícula público não prioritário:</td>
                <td class="SubTituloDireita" rowspan="5" width="10%">EJA Integrada a qualificação</td>
                <td>
                    <input id="qejpubliconpriorejaqualiprof" name="qejpubliconpriorejaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
                </td>
            </tr>
            <tr id="tr_municipal_campo" style="display: none;">
                <td class="SubTituloDireita">B - Estudades das comunidades do Campo:</td>
                <td>
                    <input id="qejcampoejaqualiprof" name="qejcampoejaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
                </td>
            </tr>
            <tr id="tr_municipal_quilombo" style="display: none;">
                <td class="SubTituloDireita">C - Quilombolas:</td>
                <td>
                    <input id="qejquilombolaqualiprof" name="qejquilombolaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
                </td>
            </tr>
            <tr id="tr_municipal_indigina" style="display: none;">
                <td class="SubTituloDireita">D - Indígenas:</td>
                <td>
                    <input id="qejindigenaqualiprof" name="qejindigenaqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
                </td>
            </tr>
            <tr id="tr_municipal_privada" style="display: none;">
                <td class="SubTituloDireita">E - Pessoas privadas de Liberdade: </td>
                <td>
                    <input id="qejprivliberqualiprof" name="qejprivliberqualiprof" type="checkbox" value="S"> EJA Integrada a qualificação profissional
                </td>
            </tr>
        <!-- fim -->
        
        <tr>
            <td class="SubTituloDireita" colspan="2">Estado do workflow:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  esdid AS codigo, 
                                esddsc AS descricao 
                        FROM workflow.estadodocumento 
                        WHERE tpdid = ".WF_FLUXO_EJA_FINANCIA." 
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
        <tr>
            <td class="SubTituloDireita" colspan="2">Demanda de Vagas (Média Censo):</td>
            <td>
                <input id="demanda" name="demanda" type="checkbox" value="S" <?php echo (($_REQUEST['demanda'] == 'S') ? "checked" : "") ?>>
            </td>
        </tr>        
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
    $n_prioritario = array(
        'label' => 'Matrícula público não prioritário',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_a = array(
        'label' => 'A - Egresso do Programa Brasil Alfabetizado',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_b = array(
        'label' => 'B - Estudades das comunidades do Campo',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_c = array(
        'label' => 'C - Quilombolas',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_d = array(
        'label' => 'D - Indígenas',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_e = array(
        'label' => 'E - Pessoas privadas de Liberdade',
        'colunas' => array(
            'Total Informado', 'Total MEC'
        )
    );

    $coluna_atendimento = array(
        'label' => 'Atendimento previsto',
        'colunas' => array(
            'Catadores de Materiais Recicláveis', 'Trabalhadores rurais empregados', 'Pessoas em situação de rua'
        )
    );

    if ($_REQUEST['itrid'] == 1) {
        $JOIN = "INNER JOIN territorios.estado e ON e.estuf = i.estuf";
        
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
        
        if( $relatorio_excel == 'S' ){
            $COLUNA = "
                e.estuf AS estado,
                e.estdescricao AS estdescricao,
            ";
            $cabecalho = array(
                "UF",
                "UF/ Município",
                "Data de Início",
                "Publico nao prioritario - Total Informado",
                "Publico nao prioritario - Total MEC",
                "A - Egresso do Programa Brasil Alfabetizado - Total Informado",
                "A - Egresso do Programa Brasil Alfabetizado - Total MEC",
                "B - Estudades das comunidades do Campo - Total Informado",
                "B - Estudades das comunidades do Campo - Total MEC",
                "C - Quilombolas - Total Informado",
                "C - Quilombolas - Total MEC",
                "D - Indígenas - Total Informado",
                "D - Indígenas - Total MEC",
                "E - Pessoas privadas de Liberdade - Total Informado",
                "E - Pessoas privadas de Liberdade - Total MEC",
                "Catadores de Materiais Recicláveis",
                "Trabalhadores rurais empregados",
                "Pessoas em situação de rua",
                "Valor Liberado MEC",
                "Estado do Workflow",
                "Total Geral",
                "Média Censo"
            );
        }else{
            $COLUNA = "
                e.estuf AS estado,
                '<a style=cursor:pointer; onclick=direcionarQuest('||i.inuid||','||p.pfaid||')>' || e.estdescricao || '</a>' AS estdescricao,
            ";
            
            $cabecalho = array(
                "UF",
                "UF/ Município",
                "Data de Início",
                $n_prioritario,
                $coluna_a,
                $coluna_b,
                $coluna_c,
                $coluna_d,
                $coluna_e,
                $coluna_atendimento,
                "Valor Liberado MEC",
                "Estado do Workflow",
                "Total Geral",
                "Média Censo"
            );
        }
        
        $ORDERBY = "ORDER BY e.estuf";
        
        
        
    } elseif ($_REQUEST['itrid'] == 2) {
        $JOIN = "INNER JOIN territorios.municipio m ON m.muncod = i.muncod";
        
        $JOINESFERA = "LEFT JOIN eja.ejamatriculascenso mat ON mat.muncod = i.muncod";
        
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
                "Data de Início",
                "Publico nao prioritario - Total Informado",
                "Publico nao prioritario - Total MEC",
                "A - Egresso do Programa Brasil Alfabetizado - Total Informado",
                "A - Egresso do Programa Brasil Alfabetizado - Total MEC",
                "B - Estudades das comunidades do Campo - Total Informado",
                "B - Estudades das comunidades do Campo - Total MEC",
                "C - Quilombolas - Total Informado",
                "C - Quilombolas - Total MEC",
                "D - Indígenas - Total Informado",
                "D - Indígenas - Total MEC",
                "E - Pessoas privadas de Liberdade - Total Informado",
                "E - Pessoas privadas de Liberdade - Total MEC",
                "Catadores de Materiais Recicláveis",
                "Trabalhadores rurais empregados",
                "Pessoas em situação de rua",
                "Valor Liberado MEC",
                "Estado do Workflow",
                "Total Geral",
                "Média Censo"
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
                "Data de Ínicio",
                $n_prioritario,
                $coluna_a,
                $coluna_b,
                $coluna_c,
                $coluna_d,
                $coluna_e,
                $coluna_atendimento,
                "Valor Liberado MEC",
                "Estado do Workflow",
                "Total Geral",
                "Média Censo"
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
    
    if($demanda){
        $aryWhere[] = "((mat.emc2010 + mat.emc2011 + mat.emc2012 + mat.emc2013)/4 + (mat.emc2010 + mat.emc2011 + mat.emc2012 + mat.emc2013)/4) <= (COALESCE(qejpublicoprioritariobraalf,0) + COALESCE(tnp.total_nao_prior,0) + COALESCE(tc.total_campo,0) + COALESCE(tq.total_quilombolas,0) + COALESCE(ti.total_indigenas,0) + COALESCE(tl.total_liberdade,0))";
    }
    
    if( $itrid == 1 ){
        #Matrícula público não prioritário      
        if($qejpubliconpriorejaqualiprof_fund == 'F'){
            $arrWhere[] = " qejpubliconpriorejaqualiprof_fund > 0 ";
        }
        if($qejpubliconpriorejaqualiprof_medi == 'M'){
            $arrWhere[] = " qejpubliconpriorejaqualiprof_medi > 0 ";
        }
        
        #B - Estudades das comunidades do Campo
        if($qejcampoejaqualiprof_fund == 'F'){
            $arrWhere[] = " qejcampoejaqualiprof_fund > 0 ";
        }
        if($qejcampoejaqualiprof_medi == 'M'){
            $arrWhere[] = " qejcampoejaqualiprof_medi > 0 ";
        }
        
        #C - Quilombolas
        if($qejquilombolaqualiprof_fund == 'F'){
            $arrWhere[] = " qejquilombolaqualiprof_fund > 0 ";
        }
        if($qejquilombolaqualiprof_medi == 'M'){
            $arrWhere[] = " qejquilombolaqualiprof_medi > 0 ";
        }

        #D - Indígenas
        if($qejindigenaqualiprof_fund == 'F'){
            $arrWhere[] = " qejindigenaqualiprof_fund > 0 ";
        }
        if($qejindigenaqualiprof_medi == 'M'){
            $arrWhere[] = " qejindigenaqualiprof_medi > 0 ";
        }

        #E - Pessoas privadas de Liberdade
        if($qejprivliberqualiprof_fund == 'F'){
            $arrWhere[] = " qejprivliberqualiprof_fund > 0 ";
        }
        if($qejprivliberqualiprof_medi == 'M'){
            $arrWhere[] = " qejprivliberqualiprof_medi > 0 ";
        }
    }else{
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
        SELECT  DISTINCT {$COLUNA}
                TO_CHAR(qejqejprevinicio, 'DD/MM/YYYY') AS qejqejprevinicio,

                tnp.total_nao_prior,
                qejvlrmecpublnaopri,

                qejpublicoprioritariobraalf,
                qejvlrmecegresso,

                tc.total_campo,
                qejvlrmeccomcampo,

                tq.total_quilombolas,
                qejvlrmecquilombolas,

                ti.total_indigenas,
                qejvlrmecindigenas,

                tl.total_liberdade,
                qejvlrmecpessoaliberdade,

                qejcatmatreciclaqtd,
                qejtrabruralqtd,
                qejpessoaruaqtd,

                vmec.qejvlrmec_valor_liberado,
                ee.esddsc,
                
                (COALESCE(qejpublicoprioritariobraalf,0) + COALESCE(tnp.total_nao_prior,0) + COALESCE(tc.total_campo,0) + COALESCE(tq.total_quilombolas,0) + COALESCE(ti.total_indigenas,0) + COALESCE(tl.total_liberdade,0)) AS total_geral,
                
		COALESCE(((mat.emc2010 + mat.emc2011 + mat.emc2012 + mat.emc2013)/4),0) AS media

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

        {$JOIN}
		
        {$JOINESFERA}
        
        INNER JOIN workflow.documento AS d ON d.docid = p.docid
        INNER JOIN workflow.estadodocumento AS ee ON ee.esdid = d.esdid

        {$WHERE}

        $ORDERBY
    ";
    $db->monta_lista($sql, $cabecalho, '100', '10', 'N', 'center');
    
        //ver($cabecalho, d);
    
    if( $relatorio_excel == 'S' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
        
	$db->sql_to_excel($sql, 'Relatório_Controle_Financeiro_EJA', $cabecalho);
    }
?>