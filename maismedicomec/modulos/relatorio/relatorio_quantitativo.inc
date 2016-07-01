<?PHP
    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
        exit;
    }
/*
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

        $db->monta_combo('muncod', $sql, $disab, "Todos", '', '', '', 450, 'N', 'muncod', '');
        die();
    }
*/
    function iniciarSessaoAvaliacao( $dados ){
        $etqid = $dados['etqid'];
        $qstid = $dados['qstid'];
        $muncod= $dados['muncod'];

        unset( $_SESSION['maismedicomec'] );

        if( $etqid != '' || $qstid != '' || $muncod != '' ){
            $_SESSION['maismedicomec']['etqid']     = $etqid;
            $_SESSION['maismedicomec']['qstid']     = $qstid;
            $_SESSION['maismedicomec']['muncod']    = $muncod;

            $retorno = 'S';
        }else{
            $retorno = 'N';
        }
        echo $retorno;

        die();
    }

    require_once APPRAIZ . "includes/cabecalho.inc";
    echo '<br/>';

    $db->cria_aba($abacod_tela,$url,$parametros);

    monta_titulo( 'Mais Médico - MEC', 'Instrumento de Avaliação' );

?>

<link type="text/css" rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript">

    function atualizaComboMunicipio( estuf ){
        var disab;

        if( $('#muncod').attr('disabled') == true ){
            disab = 'N';
        } else {
            disab = 'S';
        }

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=atualizaComboMunicipio&estuf="+estuf+'&disab='+disab,
            async: false,
            success: function(resp){
                $('#td_muncod').html(resp);
            }
        });
    }

    /*
     * CARREGAR SESSAO.
     * @param {integer} sbsid
     * @param {integer} entid
     * @returns {undefined} redirecionamento
    */
    function iniciarSessaoAvaliacao( etqid, qstid, muncod ){

        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=iniciarSessaoAvaliacao&etqid="+etqid+"&qstid="+qstid+"&muncod="+muncod,
            success: function( resp ){
                if( trim(resp) == 'S' ){
                    window.location.href = "maismedicomec.php?modulo=principal/instrumentoavaliacao/perg_aval_documental&acao=A";
                }else{
                    alert('Não foi possivél, realizar essa ação. Tente novamente mais tarde!');
                }
            }
        });
    }

    function pesquisarAutorizacaoDecreto( param ){
        if(trim(param) == 'fil'){
            var formulario = document.formulario;
            selectAllOptions( formulario.usucpf );
            $('#formulario').submit();
        }else{
            $('#formulario').submit();
        }
    }

    /*
     * Alterar visibilidade de um campo.
     *
     * @param string indica o campo a ser mostrado/escondido
     * @return void
    */
    function onOffCampo( campo ){
            var div_on = document.getElementById( campo + '_campo_on' );
            var div_off = document.getElementById( campo + '_campo_off' );
            var input = document.getElementById( campo + '_campo_flag' );
            if ( div_on.style.display == 'none' ){
                div_on.style.display = 'block';
                div_off.style.display = 'none';
                input.value = '1';
            }else{
                div_on.style.display = 'none';
                div_off.style.display = 'block';
                input.value = '0';
            }
    }

</script>

<form name="formulario" id="formulario" method="post" action="" />
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <table class="Tabela Listagem" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1">
        <tr>
            <td class="SubTituloDireita">UF:</td>
            <td>
                <?php
                    $sql = "
                        SELECT  estuf as codigo,
                            	estuf as descricao
                        FROM territorios.estado
                        ORDER BY estuf
                    ";
                    $db->monta_combo('estuf', $sql, 'S', "Todos", 'atualizaComboMunicipio', '', '', 450, 'N', 'estuf', ''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Município:</td>
            <td id="td_muncod">
                <?php
                    $sql = "
                        SELECT  m.muncod AS codigo,
                                m.mundescricao AS descricao
                        FROM maismedicomec.municipioliberado AS l
                        LEFT JOIN territorios.municipio AS m ON substr(m.muncod, 1, 6) = l.muncod
                        LEFT JOIN territorios.estado AS u ON u.estuf = m.estuf
                        ORDER BY u.estuf, m.mundescricao
                    ";
                    $db->monta_combo('muncod', $sql, $habilita, "Todos", '', '', '', 450, 'N', 'muncod', '');
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" width="30%" > Avaliadores MEC: </td>
            <td>
               <?PHP
                    $sql = "
                        SELECT  ur.usucpf AS codigo,
                                u.usunome AS descricao
                        FROM maismedicomec.usuarioresponsabilidade AS ur
                        JOIN seguranca.usuario AS u ON u.usucpf = ur.usucpf
                        WHERE 1=1
                    ";
                    $stSqlCarregados = "";
                    $camposPesq = array(
                        Array('codigo'=>'usunome', 'descricao'=>'Nome do Avaliador')
                    );

                    combo_popup( 'usucpf', $sql, 'Selecione o(s) Avaliador(es)', '400x600', 0, '', '', 'S', false, false, 10, 450, null, null, false, $camposPesq, $usucpf, true, true, null, false, null, null, null);
                ?>
            </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" style="height: ">Situação do WorkFlow:</td>
            <td>
                <?PHP
                    $sql = "
                        SELECT  esdid AS codigo,
                                esddsc AS descricao
                        FROM workflow.estadodocumento
                        WHERE tpdid = ".WF_FLUXO_AVALIACAO_IN_LOCO_MM."
                    ";
                    $db->monta_combo("esdid", $sql, 'S', 'Selecione...', '', '', '', 450, 'N', 'esdid', false, $esdid, null);
                ?>
            </td>
        </tr>
    </table>

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
        <tr>
            <td class="SubTituloCentro" colspan="2">
                <input type="button" name="pesquisar" value="Pesquisar" onclick="pesquisarAutorizacaoDecreto('fil');"/>
                <input type="button" name="vertodos" value="Ver todos" onclick="pesquisarAutorizacaoDecreto('ver');"/>
            </td>
        </tr>
    </table>

</form>


<?PHP
    extract($_POST);

    if( $usucpf[0] ){
        $where[] = " ur.usucpf " . (( $tpsid_campo_excludente == null || $tpsid_campo_flag == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $usucpf ) . "') ";
    }
    if ($_REQUEST['muncod']) {
        $muncod = $_REQUEST['muncod'];
        $where[] = " m.muncod = '{$muncod}' ";
    }
    if ($_REQUEST['estuf']){
        $estuf = $_REQUEST['estuf'];
        $where[] = " m.estuf = '{$estuf}'";
    }
    if ($_REQUEST['esdid']) {
        $esdid = $_REQUEST['esdid'];
        $where[] = " d.esdid = '{$esdid}'";
    }

    if($where != ""){
        $WHERE = "AND " . implode(' AND ', $where);
    }

    $acao = "<img style=\"cursor:pointer;\"src=\"../imagens/alterar.gif\" title=\"Ir para a Avaliação\" onclick=\"iniciarSessaoAvaliacao(' || e.etqid || ',' || e.qstid || ',' || e.muncod || ' )\"/>";

    $sql = "
        SELECT  DISTINCT '{$acao}' AS acao,
                m.estuf,
                m.mundescricao,
                usu_r.usu_responsabilidade,
                est.esddsc,
                
                CASE WHEN (resp_fav.rptresposta = '' OR resp_fav.rptresposta IS NULL)
                    THEN 'Não respondido'
                    ELSE 
                        CASE WHEN resp_fav.rptresposta = 'S'
                            THEN 'Favorável'
                            ELSE 'Não Favorável'
                        END
                END AS rptresposta
                
                ----p1.total_n_resp

        FROM maismedicomec.entidadequestionario AS e

        JOIN territorios.municipio AS m ON m.muncod = e.muncod
        
        LEFT JOIN maismedicomec.usuarioresponsabilidade AS ur ON ur.muncod = e.muncod
        LEFT JOIN seguranca.usuario AS u ON u.usucpf = ur.usucpf
        
        JOIN workflow.documento AS d ON d.docid = e.docid
        JOIN workflow.estadodocumento AS est ON est.esdid = d.esdid
        
        --TRÁS OS RESPONSAVEIS PELA AVALIAÇÃO DO MUNICÍPIO.
        LEFT JOIN(
            SELECT  muncod,
                    TRIM(array_to_string(
                        array(
                            SELECT  ' - ' || u.usunome                                    
                            FROM maismedicomec.usuarioresponsabilidade ur                            
                            JOIN seguranca.usuario as u on u.usucpf = ur.usucpf                             
                            WHERE rpustatus = 'A' AND ur.muncod = ee.muncod
                        ), '<br>'
                    ) ) AS usu_responsabilidade
            FROM maismedicomec.entidadequestionario ee
            ORDER BY 1
        ) AS usu_r On usu_r.muncod = e.muncod

        --TRÁS A RESPOSTA REFERENTE A PERGUNTA (ID=220) SE O PARECER É FAVORAVEL OU NÃO.
        LEFT JOIN(
            SELECT  e.etqid,
                    r.rptresposta 
            FROM maismedicomec.pergunta as p

            JOIN maismedicomec.questionario AS q ON q.qstid = p.qstid
            JOIN maismedicomec.entidadequestionario AS e ON e.qstid = q.qstid            
            LEFT JOIN maismedicomec.resposta as r on r.prgid = p.prgid AND r.qstid = q.qstid AND r.etqid = e.etqid

            WHERE prgstatus = 'A' AND p.prgid = 220
        ) AS resp_fav ON resp_fav.etqid = e.etqid         

        /*
        --TRÁS O NÚMERO DE PERGUNTAS NÃO RESPONDIDAS DE SUA RESPECTIVA MODALIDADE.
        LEFT JOIN(
            SELECT  e.etqid, 
                    COUNT(p.prgid) AS total_n_resp
            FROM maismedicomec.pergunta as p

            JOIN maismedicomec.questionario AS q ON q.qstid = p.qstid
            JOIN maismedicomec.entidadequestionario AS e ON e.qstid = q.qstid
            LEFT JOIN maismedicomec.resposta AS r ON r.prgid = p.prgid AND r.qstid = q.qstid AND r.etqid = e.etqid

            WHERE prgstatus = 'A' AND prgmodalidade = 'N' AND (rptresposta IS NULL OR rptresposta = '')
            
            GROUP BY e.etqid
        ) AS p1 ON p1.etqid = e.etqid
        */
        WHERE rpustatus = 'A' {$WHERE}
            
    ";//ver($sql);
    $cabecalho = array("Ação","Estado","Município","Avaliador Responsável","Status WorkFlow", "Favorável");
    $alinhamento = Array('center', '','', '', '');
    $tamanho = Array('3%', '', '', '', '');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>
