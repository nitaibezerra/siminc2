<?php

function pegaQrpid( $cpf, $queid ){
    global $db;

    $sql = "SELECT
                    sq.qrpid
            FROM
                    monitoraseb.sebquestionario sq
            INNER JOIN
                    questionario.questionarioresposta q ON q.qrpid = sq.qrpid
            WHERE
                    sq.usucpf = '{$cpf}' AND
                    q.queid = {$queid}";
    $qrpid = $db->pegaUm( $sql );

    if(!$qrpid){
        $sql = "SELECT
                    usunome
                FROM
                    seguranca.usuario
                WHERE
                    usucpf = '{$cpf}'";
        $nome = $db->pegaUm( $sql );
        $arParam = array ( "queid" => $queid, "titulo" => "SEB (".$nome.")" );
        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
        $sql = "INSERT INTO monitoraseb.sebquestionario (usucpf, qrpid) VALUES ('{$cpf}', {$qrpid})";
        $db->executar( $sql );
        $db->commit();
    }
    return $qrpid;
}

function direcionar($url, $msg=null){
    if($msg){
        echo "<script>
                alert('$msg');
                window.location='$url';
              </script>";
    } else{
        echo "<script>
                window.location='$url';
              </script>";
    }
    exit;
}

function consultarTituloTela($abacod, $url){
    global $db;

    $sql = "select m.mnudsc
              from seguranca.menu m
             inner join seguranca.aba_menu am
                on m.mnuid = am.mnuid
             where am.abacod = $abacod
               and m.mnulink = '$url'";

    return $db->pegaUm($sql);
}

?>