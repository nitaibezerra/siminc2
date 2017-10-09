<?php
ini_set("memory_limit", "2048M");

if ($_REQUEST['filtrosession']) {
    $filtroSession = $_REQUEST['filtrosession'];
}

if ($_REQUEST['tipo_relatorio'] != 'html') {
    $file_name = "Relatorio_Nutrisus_" . date('d_m_Y') . ".xls";
    header("Pragma: public");
    header("Expires: 0");
    header("Cache-Control: must-revalidate, post-check=0, pre-check=0");
    header("Content-Type: application/force-download");
    header("Content-Type: application/octet-stream");
    header("Content-Type: application/download");
    header("Content-Disposition: attachment;filename=" . $file_name);
    header("Content-Transfer-Encoding: binary ");
}

if ($_REQUEST['tipo_relatorio'] == 'html') {
    header('Content-Type: text/html; charset=iso-8859-1');
    ?>
    <html>
        <head>
            <script type="text/javascript" src="../includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
        </head>
        <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	
            <?php
        }

        $sql = monta_sql($isXls);

        function monta_sql($tipoRelatorio) {
            if ($_REQUEST['ano']) {
                $exercicio = $_REQUEST['ano'];
            } else {
                $exercicio = ' Geral ';
            }
            if (!$tipoRelatorio) {
                echo '<html>
            <head>
         
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

  </style>
                <title> <?php echo NOME_SISTEMA; ?> </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Relatório Nutrisus - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 85% !important;" align="center" border = "1">';
            } else {
                echo '<html>
            <head>
         
             <style type="text/css">
  #tb_render {
    width: 100%;
     border: 1px solid black;
     text-align: center;
     }
     #tb_render td:nth-child(5){
     text-align: right;
     }

    <br><b>Relatório Nutrisus - ' . $exercicio . '</b><br><br><table border = "1" class="tabela" style="width: 85% !important;" align="center" border = "1">';
            }

            $sqlUF = "SELECT mun.estuf ||' - '|| mundescricao AS descricaoUFMUN, mun.estuf,
       mun.mundescricao, mun.muncod
FROM pse.escolabasepse esc
INNER JOIN entidade.entidade ent ON ent.entcodent = esc.idecodinep
LEFT JOIN entidade.endereco ede ON ede.entid = ent.entid
LEFT JOIN territorios.municipio mun ON ede.muncod = mun.muncod
inner join pse.nutrisus nut ON nut.idebp = esc.idebp
WHERE 1 = 1";

            if ((is_array($_REQUEST['estado']) && $_REQUEST['estado'][0] != '')) {
                $sqlUF .= "  AND  ede.estuf  IN ('" . implode("','", $_REQUEST['estado']) . "')";
            }

            if ($_REQUEST['ano']) {
                $sqlUF .= " AND to_char(nutdatregistro, 'YYYY') =  '{$_REQUEST['ano']}'";
            }

            if ((is_array($_REQUEST['municipio']) && $_REQUEST['municipio'][0] != '')) {
                $sqlUF .= "  AND  ede.muncod  IN ('" . implode("','", $_REQUEST['municipio']) . "')";
            }


            $sqlUF .= ' GROUP BY mun.estuf,
         mun.mundescricao, mun.muncod
ORDER BY estuf,
         mun.mundescricao';
            //ver($sqlUF,d);

            global $db;
            $UFs = $db->carregar($sqlUF);

            if ($UFs) {
                foreach ($UFs as $key) {
                    $colspan = '6';
                    if ($exercicio == ' Geral ') {
                        $anoTitulo = '<td style="text-align: center"><b>Ano Pactuação</b></td>';
                        $colspan = '7';
                    }
                    $sqlEscola = "select esc.idebp,ede.muncod, ede.estuf, idenomeescola
							   from pse.escolabasepse esc
							   inner join entidade.entidade ent on ent.entcodent = esc.idecodinep
							   left join entidade.endereco ede on ede.entid = ent.entid
							   where ede.muncod = '{$key[muncod]}' order by idebp";
                    $escolas = $db->carregar($sqlEscola);
                    // ver($sqlEscola,d);
                    if ($escolas) {
                        echo '<tr>
                <td style="font-size: 20px;padding: 10px 10px 10px 0;text-align: center;color:blue;" colspan="' . $colspan . '">' . $key[descricaoufmun] . '</td>
            </tr>';
                        foreach ($escolas as $e) {



                            $sqlNutrisus = "SELECT
					            	   
					            	'<center>'||to_char(nutdatregistro::TIMESTAMP,'DD/MM/YYYY')||'<center>' as dt,
                                                        nutprimciclo, nutsegunciclo   
                                                        , array_agg(cnecodigocnes||' - '||cnenomefantasia||' - '||cneseqequipe) as nome
  FROM pse.nutrisus nut 
    inner join municipio mun on nut.muncod = mun.muncod
    left join pse.scnesnutrisus scn on nut.nutid = scn.nutid
    left join pse.scnes sce on scn.cneid = sce.cneid
  where idebp = '{$e[idebp]}' AND to_char(nutdatregistro, 'YYYY') = '{$_REQUEST['ano']}' group by nut.nutid, iduf, mundsc, nutdatregistro,  nutprimciclo, nutsegunciclo";
                            // ver($sqlNutrisus);
                            $nutrisus = $db->carregar($sqlNutrisus);
                            if ($nutrisus) {

                                    
                                if ($nutrisus[0][nome]) {
                                    if($nutrisus[0][nome] == '{NULL}'){
                                     $nutrisus[0][nome] = 'Nenhuma equipe cadastrada';
                                    }else{
                                    $nutrisus[0][nome] = str_replace('{"', "", $nutrisus[0][nome]);
                                    $nutrisus[0][nome] = str_replace('"}', "", $nutrisus[0][nome]);
                                    $nutrisus[0][nome] = str_replace('","', " / ", $nutrisus[0][nome]);
                                    }
                                } else {
                                    $nutrisus[0][nome] = 'Nenhuma equipe cadastrada';
                                }

                                echo '<tr>
                <td style="font-size: 12px;padding: 10px 10px 10px 0;text-align: center;" colspan="' . $colspan . '"><b>' . $e[idenomeescola] . '</b></td>
            </tr><tr>
    ' . $anoTitulo . '
    <td style="text-align: center"><b>Data Registro</b></td>
    <td style="text-align: center"><b>Primeiro Ciclo</b></td>
    <td style="text-align: center"><b>Segundo Ciclo</b></td>
    <td style="text-align: center"><b>Equipes</b></td>
</tr><tr>
            <td  style="text-align: left">' . $nutrisus[0][dt] . '</td>
           ' . $anoValor . '
            <td style="text-align: center">' . $nutrisus[0][nutprimciclo] . '</td>
            <td style="text-align: center">' . $nutrisus[0][nutsegunciclo] . '</td>
            <td style="text-align: center">' . $nutrisus[0][nome] . '</td>
 
        </tr>';
                            }
                        }
                    }
                }
            } else {
                echo 'Não constam registros para o ano, Estado e Município selecionados.';
            }
        }
        ?>