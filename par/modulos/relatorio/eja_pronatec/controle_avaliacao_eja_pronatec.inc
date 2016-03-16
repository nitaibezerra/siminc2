<?PHP
function listarRespostaQuestionarioEja_PRONATEC($dados){

    header('Content-type: text/html; charset=ISO-8859-1');

    global $db;

    $estuf = $dados['estuf'];
    
    if($dados['mudsc'] == 'ND'){
       $mudsc = '';
    }else{
       $mudsc = ' - '.$dados['mudsc'];
    }

    $arrPergunta = array(
        '<b>INFORME A(S) MODALIDADE(S) DE DEMANDA</b>',
        '<span style="margin:10px;"><b>Modalidades</b></span>',
        '<span style="margin:22px;">PRONATEC EJA - Técnico Integrado</span>',
        '<span style="margin:22px;">PRONATEC EJA - Técnico Concomitante</span>',
        '<span style="margin:22px;">PRONATEC EJA FIC - Médio</span>',
        '<span style="margin:22px;">PRONATEC EJA FIC - Fundamental</span>',
        '<b>PUBLÍCO NÃO PRIORITÁRIO</b>',
        '<span style="margin:10px;">Publíco não Prioritário</span>',
        '<b>PUBLÍCO PRIORITÁRIO</b>',
        '<span style="margin:10px;">A - Atendidos no Programa Brasil Alfabetizado</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',
        '<span style="margin:10px;">B - Populações do Campo</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">C - Quilombolas</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">D - Indiginas</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">E - Pessoas que cumprem pena em privação de liberdade</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">F - Jovem em Cumprimento de Medidas Socioeducativas</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">G - Catadores de Materiais Recicláveis</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">H - Populaçao em situação de Rua</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<span style="margin:10px;">I - Pescadores e Aquicultores</span>',
        '<span style="margin:22px; color:green;">Ensino Fundamental</span>',
        '<span style="margin:22px; color:blue;">Ensino Médio</span>',            
        '<b>NOS QUANTITATIVOS ACIMA ESTÁ PREVISTO O ATENDIMENTO DE</b>',
        '<span style="margin:10px;">A) Trabalhadores rurais empregados</span>'
    );

    $sql = "
        SELECT  '-' AS info,
                '-' AS modalidade,
                CASE WHEN qepejatecnicointegrado = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_1, 
                CASE WHEN qepejatecnicoconcomitante = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_2, 
                CASE WHEN qepejaficmedio = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_3, 
                CASE WHEN qepejaficfundamental = 't' THEN 'Sim' ELSE 'Não' END AS modalidade_4,
                
                '-' AS PLB_N_PRIO,
                qeppnpensinofund, 

                '-' AS PLB_PRIORITARIO,
                '-' AS A,
                qepppbraalfensfund, 
                qepppbraalfensmedio,

                '-' AS B,
                qeppppopcampoensfund, 
                qeppppopcampoensmedio, 

                '-' AS C,
                qepppquilombolaensfund, 
                qepppquilombolaensmedio, 

                '-' AS D,
                qepppindigenasensfund, 
                qepppindigenasensmedio, 

                '-' AS E,
                qepppprovliberensfund, 
                qepppprovliberensmedio, 

                '-' AS F,
                qepppmedsocioensfund, 
                qepppmedsocioensmedio, 

                '-' AS G,
                qepppmatreciclensfund, 
                qepppmatreciclensmedio, 

                '-' AS H,
                qepppsitruaensfund, 
                qepppsitruaensmedio, 

                '-' AS I,
                qeppppescadoresensfund, 
                qeppppescadoresensmedio, 

                '-' AS J,
                qepquestao01qtd as quantidade

        FROM eja.questionarioejapronatec

        WHERE qepid = {$dados['qepid']}
    ";
        
        
    $arrDados = $db->pegaLinha($sql);

    foreach ($arrDados as $resposta){
        $arrResp[] = $resposta;
    }

    foreach ($arrPergunta as $i => $pergunta){
        foreach ($arrResp as $k => $resposta){
            if( $k == $i ){
                $arrGrid[] = array('pergunta'=>$pergunta, 'resposta'=>$resposta);
            }
        }
    }
?>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
        <tr>
            <td class="SubTituloCentroAzul" style="font-weight: bold;">EJA PRONATEC</td>
        </tr>
        <tr>
            <td bgcolor="#e9e9e9">
                <center><?PHP echo $estuf.$mudsc; ?></center>
            </td>
        </tr>
    </table>

<?PHP
    $cabecalho  = array("Pergunta", "Resposta");
    $alinhamento= array('', 'right');
    $db->monta_lista($arrGrid, $cabecalho, 50000, 1, 'S', 'center', '', '', $tamanho, $alinhamento);
    die();
}

function listarAdesoesEja( $request ) {
    header('Content-type: text/html; charset=ISO-8859-1');
    global $db;

    extract($request);

    if ($esfera == 'E'){
        $join = 'INNER JOIN territorios.estado e ON e.estuf = i.estuf';
        $campo = 'e.estdescricao';
        $campo_a = 'e.estuf';
        $titulo = 'UF';

        if ($uf[0] != '') {
            $where[] = "i.estuf in ('" . implode("', '", $uf) . "')";
        }else{
            $where[] = '1=1';
        }
    }else{
        $join = 'INNER JOIN territorios.municipio m ON m.muncod = i.muncod';
        $campo = 'm.estuf||\' - \'||m.mundescricao as municipio';
        $campo_a = "m.estuf||'_'||m.mundescricao";
        $titulo = 'Município';

        if ($muncod[0] != '') {
            $where[] = "i.muncod in ('" . implode("', '", $muncod) . "')";
        }
        if ($uf[0] != '') {
            $where[] = "m.estuf in ('" . implode("', '", $uf) . "')";
        }
    }

    $acao = "
        <img align=\"absmiddle\" src=\"/imagens/lista_verde.gif\" title=\"Vizualizar Questionário\" style=\"cursor:pointer\" class=\"questionario\" id=\"'||q.qepid||'_'||{$campo_a}||'\">
    ";

    $sql = "
        SELECT  '{$acao}' as acao,
                $campo,
                ee.esddsc
        FROM eja.questionarioejapronatec q

        JOIN par.instrumentounidade AS i ON i.inuid = q.inuid
        JOIN par.pfadesaoprograma AS p ON p.inuid = q.inuid AND p.adpano = 2014 AND pfaid = 22

        {$join}

        JOIN workflow.documento AS d ON d.docid = p.docid
        JOIN workflow.estadodocumento AS ee ON ee.esdid = d.esdid

        WHERE ".implode(' AND ', $where)."
        ORDER BY 2
    ";
    $alinhamento = array('center', 'left', 'left');
    $cabecalho = array("Questionário", $titulo, "Situação" );
    $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
}

if ($_REQUEST['req']) {
    $_REQUEST['req']($_REQUEST);
    die();
}

if ($_POST['ajaxUf']) {

    if ($_POST['ajaxUf'] != 'XX') {
        echo "<b>Estado(s): {$_POST['ajaxUf']}</b><br>";
        $stAnd = "AND regcod in ('" . str_replace(",", "','", $_POST['ajaxUf']) . "')";
    }

    $sql_combo = "
        SELECT  muncod AS codigo,
		regcod ||' - '|| mundsc AS descricao

        FROM public.municipio
        WHERE munstatus = 'A' $stAnd
        ORDER BY 2 ASC;
    ";
    combo_popup('muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
    exit;
}

//include APPRAIZ . '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo('Relatório Controle de Avaliação - EJA ', '&nbsp;');

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
    <head>
        <!--meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1"-->
        <title>Relatório</title>

        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <script type="text/javascript" src="../includes/funcoes.js"></script>

        <script type="text/javascript">

            function onOffCampo(campo){
                var div_on = document.getElementById(campo + '_campo_on');
                var div_off = document.getElementById(campo + '_campo_off');
                var input = document.getElementById(campo + '_campo_flag');

                if (div_on.style.display == 'none'){
                    div_on.style.display = 'block';
                    div_off.style.display = 'none';
                    input.value = '1';
                }else{
                    div_on.style.display = 'none';
                    div_off.style.display = 'block';
                    input.value = '0';
                }
            }

            function filtraMunicipio(){

                mostraMunicipio();

                var listauf = "";
                $('#uf').children().each(function() {
                    listauf = listauf + "," + $(this).val();
                });

                if (listauf)
                    listauf = listauf.substr(1);

                if (!listauf)
                    listauf = 'XX';

                $.ajax({
                    type: "POST",
                    url: window.location,
                    data: '&ajaxUf=' + listauf,
                    async: false,
                    success: function(msg) {
                        $('#td_municipio').html(msg);
                    }
                });
            }

            function mostraMunicipio() {

                var tr_municipio = document.getElementById('tr_municipio');
                var td_municipio = document.getElementById('td_municipio');

                if (document.formulario.esfera[1].checked == true) {
                    tr_municipio.style.display = '';
                    td_municipio.style.display = '';

                }else{
                    td_municipio.style.display = 'none';
                    tr_municipio.style.display = 'none';
                }
            }

            $(document).ready(function() {
                $('.pesquisar').click(function(){
                    var esfera = $('input:radio[name=esfera]:checked');
                    
                    var formulario = document.formulario;
	
                    var uf = formulario.elements['uf'][0].value;
                    
                    if( trim(esfera.val()) == 'M' ){
                        if( uf == null || uf == '' ){
                            alert('O campo "EStado(s)" é um campo obrigatório!');
                            return false;
                        }
                    }
                    
                    selectAllOptions(formulario.uf);
                    selectAllOptions(formulario.muncod);
                    
                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "&req=listarAdesoesEja&" + $('#formulario').serialize(),
                        async: false,
                        success: function(msg) {
                            $('#td_lista').html(msg);
                        }
                    });
                });

                $('.questionario').live('click', function() {
                    var obj = $(this).attr('id');
                    var array = obj.split('_');

                    var qepid = trim(array[0]);
                    var estuf = trim(array[1]);
                    var mudsc;

                    if(trim(array[2]) == 'undefined'){
                        mudsc = 'ND';
                    }else{
                        mudsc = trim(array[2]);
                    }

                    $.ajax({
                        type: "POST",
                        url: window.location,
                        data: "&req=listarRespostaQuestionarioEja_PRONATEC&qepid=" + qepid +'&estuf='+estuf+'&mudsc='+escape(mudsc),
                        async: false,
                        success: function(msg) {
                            $('#td_questionario').html(msg);
                        }
                    });
                });

                $('.limpar').click(function() {
                    window.location = window.location;
                });
            });

        </script>
    </head>
    <body>
        <form name="formulario" id="formulario" action="" method="post">
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
                <tr>
                    <td width="25%"class="SubTituloDireita">Esfera:</td>
                    <td>
                        <input type="radio" value="E" name="esfera" id="esfera_e" onclick="mostraMunicipio();" onchange="mostraMunicipio();" checked="checked"><b>Estadual</b>
                        <br>
                        <input type="radio" value="M" name="esfera" id="esfera_m" onclick="mostraMunicipio();" onchange="mostraMunicipio();"><b>Municipal</b>
                    </td>
                </tr>
                <tr>
                    <td class="SubTituloDireita" valign="top">Estado:</td>
                    <td >
                        <?php
                            $sql_combo = "
                                SELECT  regcod AS codigo,
                                        descricaouf AS descricao
                                FROM public.uf
                                WHERE idpais = 1
                                ORDER BY 2 ASC
                            ";
                            combo_popup('uf', $sql_combo, 'Selecione o(s) Estado(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, 'filtraMunicipio()', false);
                        ?>
                    </td>
                </tr>
                <tr id="tr_municipio" style="display:none;">
                    <td class="SubTituloDireita" valign="top">Município:</td>
                    <td id="td_municipio">
                        <?php
                            $sql_combo = "
                                SELECT  muncod AS codigo,
                                        regcod ||' - '|| mundsc AS descricao
                                FROM public.municipio
                                WHERE munstatus = 'A' AND regcod = 'XX'
                                ORDER BY 2 ASC
                            ";
                            combo_popup('muncod', $sql_combo, 'Selecione o(s) Municípios(s)', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, false);
                        ?>
                    </td>
                </tr>
                <tr>
                    <td align="center" colspan="2">
                        <input type="button" class="pesquisar" value="Pesquisar"/>
                        <input type="button" class="limpar" value="Limpar Pesquisa"/>
                    </td>
                </tr>
                <tr>
                    <td id="td_lista" colspan="2">
                    </td>
                </tr>
            </table>
            
            <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
                <tr>
                    <td id="td_questionario" colspan="2">
                    </td>
                </tr>
            </table>
        </form>
    </body>
</html>