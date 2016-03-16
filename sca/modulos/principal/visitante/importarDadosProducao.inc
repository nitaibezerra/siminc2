<?php

ini_set("memory_limit", "1024M");
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$count = $_REQUEST['count'] ? $_REQUEST['count'] : 10;

//busca fotos de visitantes
$sql = "select vttid, im_foto_visitante
          from ((select t1.vttid
                       ,t3.im_foto_visitante as im_foto_visitante
                   from sca.visitante t1
                  inner join sca.tmp_tb_spt_visitante t3
                     on t1.vttid = t3.vttidnew
                   left join sca.visitantefoto t2
                     on t1.vttid = t2.vttid
                  where t2.svfid is null
                    and t3.possuifoto = true
                  limit $count)
                  union
                (select t1.vttid
                       ,t4.im_foto_autorizado as im_foto_visitante
                   from sca.visitante t1
                  inner join sca.tmp_tb_sca_autorizado t4
                     on t1.vttdoc = t4.nu_cpf
                   left join sca.visitantefoto t2
                     on t1.vttid = t2.vttid
                  where t2.svfid is null
                    and t4.possuifoto = true
                  limit $count)) as t0
         limit $count";

$visitantes = $db->carregar($sql);

if ($visitantes){
    for($i = 0; $i < count($visitantes); $i++){
        $visitante = $visitantes[$i];

        $foto = new FilesSimec("visitantefoto", array("vttid" => $visitante['vttid']) , "sca");
        $foto->setStream("Foto de visitante", pg_unescape_bytea($visitante['im_foto_visitante']));
    }
    echo count($visitantes) . " fotos de visitantes importadas!";
} else {
    //busca fotos de servidores
    $sql = "select t1.nu_matricula_siape
                  ,t3.foto
              from sca.vwservidorativo t1
              left join sca.visitantefoto t2
                on t1.nu_matricula_siape = t2.nu_matricula_siape
              left join sca.tmp_tb_scf_cadastro_cartao t3
                on t1.nu_matricula_siape = t3.nu_matricula_siape
             where t2.svfid is null
               and t3.nu_matricula_siape is not null and t3.foto is not null
             limit $count";

    $servidores = $db->carregar($sql);

    if ($servidores){
        for($i = 0; $i < count($servidores); $i++){
            $servidor = $servidores[$i];

            $foto = new FilesSimec("visitantefoto", array("nu_matricula_siape" => $servidor['nu_matricula_siape']) , "sca");
            $foto->setStream("Foto de servidor", pg_unescape_bytea($servidor['foto']));
        }
        echo count($servidores) . " fotos de servidores importadas!";
    } else {
        echo "Nada a importar!";
    }
}
die();