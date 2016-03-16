<?php

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST);
    }

    function abrirComentarios( $dados ){
        global $db;

        $hstid = $dados['hstid'];

        $sql = "
            SELECT  cd.cmddsc

            FROM workflow.historicodocumento hd

            JOIN workflow.acaoestadodoc ac on ac.aedid = hd.aedid
            JOIN workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
            JOIN seguranca.usuario us on us.usucpf = hd.usucpf

            LEFT JOIN workflow.comentariodocumento cd on cd.hstid = hd.hstid

            WHERE hd.hstid = {$hstid}

            ORDER BY hd.htddata asc, hd.hstid ASC
        ";
        $result = $db->pegaUm($sql);
        echo "<p style=\"text-align:justify;font-size:14px;color:#000000;\">{$result}</p>";
        die();
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Getão de Pessoas', '<b>PRORROGAÇÃO - Litagem das Cessões e Prorrogações / Cadastro de Prorrogação</b>');
    echo "<br>";

    $abacod_tela    = ABA_CAD_AFASTAMENTO;
//    $url            = 'cap.php?modulo=principal/hist_movimentos_wf&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);

?>
<meta http-equiv="content-type" content="text/html; charset=iso-8859-1"/>

<link rel="stylesheet" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"></link>

<link rel="stylesheet" href="../cap/css/modal_css_basic.css" />

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<script type="text/javascript" src="../cap/js/jquery.simplemodal.js"></script>

<script type="text/javascript">

    function abrirComentarios( hstid ){
        $.ajax({
            type    : "POST",
            url     : window.location,
            data    : "requisicao=abrirComentarios&hstid="+hstid,
            async: false,
            success: function(resp){
                $('#visualizarComentario').html( resp );
                $('#visualizarComentario').modal();
            }
        });
    }

</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

    <div id="visualizarComentario" style="display:none;"> </div>

</form>

<?PHP
    include APPRAIZ . 'includes/workflow.php';

    $docid = criarDocumento($_SESSION['cap']['afpid']);

    $sql = "
        SELECT  row_number() OVER (PARTITION by 0 order by hd.hstid) as seq,
                ed.esddsc,
                ac.aeddscrealizada,
                us.usunome,
                hd.htddata,
                '<a href=\"#\" onclick=\"abrirComentarios(\''|| hd.hstid ||'\')\">' || cd.cmddsc || '</a>'

        FROM workflow.historicodocumento hd

        JOIN workflow.acaoestadodoc ac on ac.aedid = hd.aedid
        JOIN workflow.estadodocumento ed on ed.esdid = ac.esdidorigem
        JOIN seguranca.usuario us on us.usucpf = hd.usucpf

        LEFT JOIN workflow.comentariodocumento cd on cd.hstid = hd.hstid

        WHERE hd.docid = {$docid}

        ORDER BY hd.htddata asc, hd.hstid ASC
    ";
    $cabecalho = array("Seq", "Onde Estava", "O que aconteceu", "Quem fez", "Quando fez", "Comentário");
    $alinhamento = Array('center', '', '', '', '');
    $tamanho = Array('2%', '13%', '13%', '13%', '13%', '45%');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);

?>