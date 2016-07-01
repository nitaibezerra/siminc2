<?PHP
    $entidcampus = $_REQUEST['entidcampus'];
    $dinid       = $_REQUEST['dinid'];

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }


    function buscarNomeCampus( $entidcampus ){
        global $db;

        if( $_SESSION['academico']['orgid'] == '1' ){
            $funid = ACA_ID_CAMPUS;
        }elseif( $_SESSION['academico']['orgid'] == '2' ){
            $funid = ACA_ID_UNED;
        }

        $sql = "
            SELECT e.entnome as descricao
            FROM entidade.entidade e2
            INNER JOIN entidade.entidade e ON e2.entid = e.entid
            INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid
            WHERE ea.entid = {$_SESSION['academico']['entid']} AND e.entstatus = 'A' AND ef.funid = {$funid} AND e.entid = {$entidcampus}
            ORDER BY e.entnome
        ";
        $nome_campus = $db->pegaUM( $sql );

        return $nome_campus;
    }

    function montaListaObras( $dados ){
        global $db;

        $acao  = $dados['acao'];
        $obrid = $dados['obrid'];

        $dinid = $dados['dinid'];

        if( $obrid != '' ){
            if( $acao == 'I' ){
                $_SESSION['arrayObras'][] .= $obrid;
            }else{
                $key = array_search($obrid, $_SESSION['arrayObras']);
                unset($_SESSION['arrayObras'][$key]);
            }
        }

        $IN .= implode(', ', $_SESSION['arrayObras']);

        if( $dinid ){
            $complemento = "OR dd.dinid = {$dinid}";
        }

        $sql = "
            SELECT  DISTINCT oi.obrid,
                    upper(oi.obrdesc) as Nome_Da_Obra,
                    st.stodesc as SituacaoObra,
                    oi.obrpercexec as Percentual_Executado

            FROM obras.obrainfraestrutura AS oi

            JOIN entidade.entidade AS ee ON oi.entidunidade = ee.entid
            JOIN entidade.entidade AS ec ON oi.entidcampus = ec.entid
            JOIN entidade.endereco AS ed ON oi.endid = ed.endid
            JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
            JOIN obras.situacaoobra  AS st ON oi.stoid = st.stoid

            LEFT JOIN academico.demandaobras AS dd ON dd.obrid = oi.obrid

            LEFT JOIN(
                SELECT  rsuid,
                        obrid,
                        supdtinclusao
                FROM obras.supervisao s
                WHERE supvid = (SELECT max(supvid) FROM obras.supervisao ss WHERE ss.obrid = s.obrid)
            ) AS su ON su.obrid = oi.obrid

            WHERE oi.obrid IN ( {$IN} ) {$complemento}

            ORDER BY 2
        ";

        if($IN != ''){
            $dados = $db->carregar($sql);

            echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem tabela">';
            echo "
                <thead>
                    <tr>
                        <td>&nbsp</td>
                        <td><b>ID da Obra</b></td>
                        <td><b>Nome da Obra</b></td>
                        <td><b>Situação</b></td>
                        <td><b>Executado</b></td>
                    </tr>
                </thead>
            ";
            foreach($dados as $dado){
                echo "
                    <tr>
                        <td align='center'><img border=\"0\" title=\"Apagar Obra\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" /></td>
                        <td><input type='hidden' name='obrid[]' value='{$dado['obrid']}'>{$dado['obrid']}</td>
                        <td>{$dado['nome_da_obra']}</td>
                        <td>{$dado['situacaoobra']}</td>
                        <td>{$dado['percentual_executado']}</td>
                    </tr>
                ";
            }
            echo '</table>';
        }else{
            echo '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem tabela">';
            echo '<tr><td align="center" style="color:#cc0000;" colspan="5">Não foram encontrados Registros.</td></tr>';
            echo '</table>';
        }
        die();
    }

    monta_titulo('Demanda/Infraestrutura', 'Seleção de Obras');
    echo '<br>';

    #BUSCAS NOME DO CAMPOS.
    $nome_campus = buscarNomeCampus( $entidcampus );
?>

<title>Seleção de Obras</title>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<link rel="stylesheet" type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" ></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">
    function montaListaObras( obrid ){
        var acao;
        var dinid = $('#dinid').val();
        var check = $('input:checkbox[name=idobra_'+obrid+']:checked');

        if( check.attr('checked') ){
            acao = 'I'
        }else{
            acao = 'D'
        }

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=montaListaObras&acao="+acao+"&obrid="+obrid+'&dinid='+dinid,
            async: false,
            success: function(resp){
                $(opener.document.getElementById("td_listagem_obras_relacionados")).html(resp);
            }
        });
    }

    function pesquisarServidor(param){
        if(trim(param) == 'fil'){
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" value="<?= $dinid; ?>" name="dinid" id="dinid" />

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloCentro" width="50%" style="text-transform: uppercase;"><?=$_SESSION['MPA']['nome_univercidade'];?></td>
            <td class ="SubTituloCentro" width="50%" style="text-transform: uppercase;"><?=$nome_campus;?></td>
        </tr>
    </table>

    <table align="center" border="0" class="tabela listagem" cellpadding="3" cellspacing="1">
        <tr>
            <td class ="SubTituloDireita" width="30%">ID Obra:</td>
            <td>
                <?= campo_texto('obrid', 'N', 'S', '', 33, 20, '', '', '', '', 0, 'id="obrid"', '', $obrid, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%">Nome da Obra:</td>
            <td>
                <?= campo_texto('obrdesc', 'N', 'S', '', 33, 20, '', '', '', '', 0, 'id="obrdesc"', '', $obrdesc, null, '', null); ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita">Situação da Obras:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  stoid AS codigo,
                                stodesc AS descricao
                        FROM obras.situacaoobra
                        ORDER BY 1
                    ";
                    $db->monta_combo("stoid", $sql, 'S', 'Selecione...', '', '', '', '340', 'N', 'stoid', false, $stoid, null);
                ?>
            </td>
        </tr>
    </table>

    <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarServidor('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarServidor('ver');"/>
                <!-- <input type="button" name="fechar" value="Fechar" onclick="javascript: window.opener.location.reload();"/> -->
                <input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();"/>
            </td>
        </tr>
    </table>
</form>

<?PHP

    if ($_REQUEST['obrid']) {
        $obrid = $_REQUEST['obrid'];
        $where[] = "oi.obrid = '{$obrid}'";
    }

    if ($_REQUEST['obrdesc']) {
        $obrdesc = $_REQUEST['obrdesc'];
        $where[] = " public.removeacento(oi.obrdesc) ILIKE public.removeacento( ('%{$obrdesc}%') ) ";
    }

    if ($_REQUEST['stoid']) {
        $stoid = $_REQUEST['stoid'];
        $where[] = " oi.stoid = '{$stoid}'";
    }

    $WHERE = "WHERE oi.obsstatus = 'A' AND oi.orgid = 1 AND oi.entidcampus = {$entidcampus} ";

    if($where != ""){
        $WHERE .= "AND " . implode(' AND ', $where);
    }

    if( $dinid ){
        $complemento = "AND dd.dinid = {$dinid}";

        $acao_case = "
            CASE WHEN dd.obrid IS NULL
                THEN '<input type=\"checkbox\" class=\"listaObras\" name=\"idobra_'||oi.obrid||'\" value=\"'||oi.obrid||'\" onclick=\"montaListaObras('||oi.obrid||')\">'
                ELSE '<input type=\"checkbox\" class=\"listaObras\" checked=\"checked\" disabled=\"disabled\" name=\"idobra_'||oi.obrid||'\" value=\"'||oi.obrid||'\" onclick=\"montaListaObras('||oi.obrid||')\">'
            END AS acao
        ";
    }else{
        $acao_case = "
            '<input type=\"checkbox\" class=\"listaObras\" name=\"idobra_'||oi.obrid||'\" value=\"'||oi.obrid||'\" onclick=\"montaListaObras('||oi.obrid||')\">' AS acao
        ";
    }

    $sql = "
            SELECT  {$acao_case},
                    oi.obrid,
                    upper(oi.obrdesc) as Nome_Da_Obra,
                    st.stodesc as SituacaoObra,
                    oi.obrpercexec as Percentual_Executado

            FROM obras.obrainfraestrutura AS oi

            JOIN entidade.entidade AS ee ON oi.entidunidade = ee.entid
            JOIN entidade.entidade AS ec ON oi.entidcampus = ec.entid
            JOIN entidade.endereco AS ed ON oi.endid = ed.endid
            JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
            JOIN obras.situacaoobra  AS st ON oi.stoid = st.stoid

            LEFT JOIN academico.demandaobras AS dd ON dd.obrid = oi.obrid {$complemento}

            LEFT JOIN(
                SELECT  rsuid,
                        obrid,
                        supdtinclusao
                FROM obras.supervisao s
                WHERE supvid = (SELECT max(supvid) FROM obras.supervisao ss WHERE ss.obrid = s.obrid)
            ) AS su ON su.obrid = oi.obrid

            {$WHERE}

            ORDER BY ee.entnome
    ";
    $cabecalho = Array("&nbsp", "ID da Obra", "Nome da Obra", "Situação", "% Executado");
    //$whidth = Array('20%', '60%', '20%');
    $align  = Array('center', 'center', '', '', '');
    $db->monta_lista($sql, $cabecalho, 15, 10, 'N', '', 'N', 'formulario_obras', $whidth, $align, '');

?>