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

monta_titulo('Relatório de Controle de Adesão - EJA PRONATEC', '&nbsp;');

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
    
    function ordena(ordem, direcao){
        document.formlista.ordemlista.value=ordem;
        document.formlista.ordemlistadir.value=direcao;
        document.formlista.submit();
    }
    
    function pagina(numero){
        document.formulario.numero.value=numero;
        document.formulario.submit();
    }

</script>

<form name="formulario" id="formulario" action="" method="post">
    <input type="hidden" id="relatorio_excel" name="relatorio_excel" value="">
    <input type="hidden" id="numero" name="numero" value="">

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
        'label' => 'MODALIDADES',
        'colunas' => array(
            'Técnico Integrado','Técnico Concomitante','FIC<br>Médio','FIC<br>Fundamental'
        )
    );

    $coluna_b = array(
        'label' => 'DADOS INSTITUCIONAIS',
        'colunas' => array(
            'CNPJ','Nome','E-mail','CEP','Logradouro','bairro', 'UF','Município'
        )
    );

    $coluna_c = array(
        'label' => 'DADOS DO SUPERVISOR DE DEMANDAS',
        'colunas' => array(
            'Nome','CPF','Matricula','Tel','Cel','E-mail'
        )
    );

    if ($_REQUEST['itrid'] == 1) {
        $JOIN = "INNER JOIN territorios.estado e ON e.estuf = ins.estuf";

        if( $relatorio_excel == 'S' ){
            $COLUNA = "
                e.estuf AS estado,
            ";
            
            $COL_AXU = " ' - ' AS mun_desc,";
            
            $cabecalho = array(
                "UF",
                "Técnico Integrado",
                "Técnico Concomitante",
                "FIC - Médio",
                "FIC - Fundamental",
                "CNPJ",
                "Nome",
                "E-mail",
                "CEP",
                "Telefone",
                "Logradouro",
                "Bairro", 
                "UF",
                "Município",
                "Nome",
                "CPF",
                "Matricula",
                "Tel",
                "Cel",
                "E-mail Institucional",
                "E-mail Pessoal",
                "Estado do Workflow"
            );
        }else{
            $COLUNA = "
                e.estuf,
                '<a style=cursor:pointer; onclick=direcionarQuest('||ins.inuid||','||pfa.pfaid||')>' || e.estuf || '</a>' AS estuf_descricao,
            ";
            
            $COL_AXU = " ' - ' AS mun_desc,";

            $cabecalho = array(
                "UF",
                $coluna_a,
                $coluna_b,
                $coluna_c,
                "Estado do Workflow"
            );
        }

        $alinhamento = Array('center', 'center', 'center', 'center','center');
        $tamanho = Array('5%', '10%', '', '');

        $ORDERBY = "ORDER BY e.estuf";

    } elseif ($_REQUEST['itrid'] == 2) {
        $JOIN = "JOIN territorios.municipio m ON m.muncod = ins.muncod";

        if( $relatorio_excel == 'S' ){
            $COLUNA = "
                m.estuf AS estado,
                m.muncod,
                m.mundescricao AS mundescricao,
            ";
            
            $COL_AXU = "m.mundescricao AS mun_desc,";
            
            $cabecalho = array(
                "UF",
                "Cód. IBGE",
                "Município",
                "Técnico Integrado",
                "Técnico Concomitante",
                "FIC - Médio",
                "FIC - Fundamental",
                "CNPJ",
                "Nome",
                "E-mail",
                "CEP",
                "Telefone",
                "Logradouro",
                "Bairro", 
                "UF",
                "Município",
                "Nome",
                "CPF",
                "Matricula",
                "Tel",
                "Cel",
                "E-mail Institucional",
                "E-mail Pessoal",
                "Estado do Workflow"
            );
        }else{
            $COLUNA = "
                m.estuf AS estuf,
                '<a style=cursor:pointer; onclick=direcionarQuest('||ins.inuid||','||pfa.pfaid||')>'|| m.estuf ||' - '|| m.mundescricao || '</a>' AS estuf_mundescricao,
            ";

            $COL_AXU = "m.mundescricao AS mun_desc,";
            
            $cabecalho = array(
                "UF",
                "Município",
                $coluna_a,
                $coluna_b,
                $coluna_c,
                "Estado do Workflow"
            );
        }
        $ORDERBY = "ORDER BY m.estuf";

        $alinhamento = Array('center', 'left', 'center', 'center');
        $tamanho = Array('5%', '10%', '', '');

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

    
    #PARAMETROS PARA PAGINAÇÃO.
    #NUMÉRO PARA PAGINAÇÃO.
    if( $_REQUEST['numero'] == '' ){
        $numero = 1;
    }else{
        $numero = intval($_REQUEST['numero']);
    }
    
    #$perpage - REGISTROS POR PAGINA;
    #$pages   - NÚMERO MAXIMO DE PAGINAS.    
    $perpage= 5; 
    $pages  = 10;
    
    $sql = "
        SELECT  DISTINCT {$COLUNA}

                CASE WHEN qepejatecnicointegrado = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_1,
                CASE WHEN qepejatecnicoconcomitante = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_2,
                CASE WHEN qepejaficmedio = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_3,
                CASE WHEN qepejaficfundamental = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_4,

                --INSTITUCIONAL
                i.epicnpj,
                i.epidsc,
                i.epiemail,
                i.epicep,
                i.epitelefonenumero,
                i.epilogradouro,
                i.epibairro,
                i.estuf,
                {$COL_AXU}

                --SUPERVISOR DE DEMANDAS
                s.epsnome,
                s.epscpf,
                s.epsmatricula,
                s.epstelefone,
                s.epscelular,
                s.epsemailinstitucional,
                s.epsemailparticular,

                 --wf
                ee.esddsc

        FROM eja.questionarioejapronatec AS q

        JOIN par.instrumentounidade AS ins ON ins.inuid = q.inuid

        LEFT JOIN par.pfadesaoprograma AS pfa ON pfa.inuid = q.inuid AND pfa.adpano = 2014 AND pfaid = 22

        JOIN eja.ejapronatecinstitucional AS i ON i.adpid = pfa.adpid

        JOIN eja.ejapronatecsupervisor AS s ON s.adpid = pfa.adpid

        {$JOIN}

        {$JOINESFERA}

        JOIN workflow.documento AS d ON d.docid = pfa.docid
        JOIN workflow.estadodocumento AS ee ON ee.esdid = d.esdid

        {$WHERE}
        
        $ORDERBY
    ";
        
    #TOTAL DE REGISTROS.
    $sqlCount = "SELECT COUNT(1) FROM({$sql}) RS";
    $totalRegistro = $db->pegaUm($sqlCount,0,$tempocache);
        
    $sql .= "LIMIT {$perpage} OFFSET {$numero}";
    
    $arrDados = $db->carregar($sql);

    if($arrDados != ''){

        foreach ($arrDados as $dados){
?>
            <table class="tabela listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0" width="99%">
                <tr>
                    <?PHP
                        if($_REQUEST['itrid'] == 1) {
                    ?>
                            <td rowspan="12" width="7%" class="SubTituloDireita"><?=$dados['estuf_descricao'];?></td>
                    <?PHP
                        }else{
                    ?>
                            <td rowspan="12" width="7%" class="SubTituloDireita"><?=$dados['estuf_mundescricao'];?></td>
                    <?  } ?>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloCentro" width="16%">MODALIDADES</td>
                    <td colspan="2" class="subTituloCentro" width="34%">DADOS INSTITUCIONAIS</td>
                    <td colspan="2" class="subTituloCentro" width="34%">DADOS DO SUPERVISOR DE DEMANDAS</td>
                    <td rowspan="12" width="5%" class="SubTituloDireita"><?=$dados['esddsc'];?></td>
                </tr>

                <tr>
                    <td class ="SubTituloDireita" width="8%"> Técnico Integrado: </td>
                    <td width="8%"><?=$dados['modalidade_1'];?></td>

                    <td class ="SubTituloDireita" width="16%"> <?=$textoSecretaria;?>: </td>
                    <td width="16%"><?=$dados['epidsc'];?></td>

                    <td class ="SubTituloDireita"  width="16%"> CPF: </td>
                    <td  width="16%"><?=$dados['epscpf'];?></td>
                </tr>

                <tr>
                    <td class ="SubTituloDireita"> Técnico Concomitante: </td>
                    <td><?=$dados['modalidade_2'];?></td>

                    <td class ="SubTituloDireita"> CNPJ: </td>
                    <td><?=$dados['epicnpj'];?></td>

                    <td class ="SubTituloDireita"> Nome: </td>
                    <td><?=$dados['epidsc'];?></td>
                </tr>

                <tr>
                    <td class ="SubTituloDireita"> FIC - Médio: </td>
                    <td><?=$dados['modalidade_3'];?></td>

                    <td class ="SubTituloDireita"> Telefone DDD/Número: </td>
                    <td><?=$dados['epitelefonenumero'];?>
                    </td>

                    <td class ="SubTituloDireita"> Matricula: </td>
                    <td><?=$dados['epsmatricula'];?></td>
                </tr>

                <tr>
                    <td class ="SubTituloDireita"> FIC - Fundamental: </td>
                    <td><?=$dados['modalidade_4'];?></td>

                    <td class ="SubTituloDireita"> E-mail: </td>
                    <td><?=$dados['epiemail'];?>
                    </td>

                    <td class ="SubTituloDireita"> Telefone Institucional: </td>
                    <td><?=$dados['epstelefone']?></td>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloCentro"> - </td>

                    <td class ="SubTituloDireita"> CEP: </td>
                    <td><?=$dados['epicep'];?>
                    </td>

                    <td class ="SubTituloDireita"> Telefone Celular: </td>
                    <td><?=$dados['epscelular'];?></td>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloCentro"> - </td>

                    <td class ="SubTituloDireita"> Logradouro: </td>
                    <td><?=$dados['epilogradouro'];?>
                    </td>

                    <td class ="SubTituloDireita"> E-mail Institucional: </td>
                    <td><?=$dados['epsemailinstitucional'];?></td>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloCentro"> - </td>

                    <td class ="SubTituloDireita"> Bairro: </td>
                    <td><?=$dados['epibairro'];?></td>

                    <td class ="SubTituloDireita"> E-mail Particular: </td>
                    <td><?=$dados['epsemailparticular'];?></td>
                </tr>
                <tr>
                    <td colspan="2" class="subTituloCentro"> - </td>

                    <td class ="SubTituloDireita"> UF: </td>
                    <td><?= $dados['estuf'];?></td>

                    <td colspan="2" class="subTituloCentro"> - </td>
                </tr>

                <tr>
                    <td colspan="2" class="subTituloCentro"> - </td>

                    <td class="SubTituloDireita">Município</td>
                    <td><?=$dados['mun_desc'];?></td>

                    <td colspan="2" class="subTituloCentro"> - </td>
                </tr>

            </table>
<?PHP
        }
    }else {
?>
        <table width="'. $largura . '" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
            <tr>
                <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
            </tr>
        </table>
<?PHP
    }
    
    #DESENA A LINHA DE TOTAL NA PAGINAÇÃO.
    echo '
        <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr bgcolor="#ffffff">
                <td><b>Total de Registros: ' . $totalRegistro . '</b></td>
                <td>
    ';
    
    $total_reg = $totalRegistro;
    
    include APPRAIZ."includes/paginacao.inc";
    
    if( $relatorio_excel == 'S' ){
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');

	$db->sql_to_excel($sql, 'Relatório_Controle_Financeiro_EJA', $cabecalho);
    }
    
?>