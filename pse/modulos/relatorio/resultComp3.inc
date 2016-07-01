<?php
ini_set("memory_limit", "2048M");

if ($_REQUEST['filtrosession']) {
    $filtroSession = $_REQUEST['filtrosession'];
}

if ($_REQUEST['tipo_relatorio'] != 'html') {
    $file_name = "Relatorio_Componente_3_" . date('d_m_Y') . ".xls";
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
                <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
                <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
                <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
                <body>
                    <center>
                            <!--  Cabeçalho Brasão -->
                            ' . monta_cabecalho_relatorio('100') . ' 
                   <br><b>Relatório Componente 3 - ' . $exercicio . '</b><br><br><table class="tabela" style="width: 85% !important;" align="center" border = "1">';
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

    <br><b>Relatório Componente 3 - ' . $exercicio . '</b><br><br><table border = "1" class="tabela" style="width: 85% !important;" align="center" border = "1">';
            }

            $sqlUF = "SELECT iduf ||' - '|| mundescricao AS descricaoUFMUN, cp3.muncod as municipio
FROM pse.monitoracomp3 cp3
INNER JOIN territorios.municipio mun ON cp3.muncod = mun.muncod where 1 = 1";

            if ((is_array($_REQUEST['estado']) && $_REQUEST['estado'][0] != '')) {
                $sqlUF .= "  AND  mun.estuf  IN ('" . implode("','", $_REQUEST['estado']) . "')";
            }

            if ($_REQUEST['ano']) {
                $sqlUF .= " AND c3anopactuacao = '{$_REQUEST['ano']}'";
            }

            if ((is_array($_REQUEST['municipio']) && $_REQUEST['municipio'][0] != '')) {
                $sqlUF .= "  AND  mun.muncod  IN ('" . implode("','", $_REQUEST['municipio']) . "')";
            }


            $sqlUF .= ' GROUP BY iduf, mun.mundescricao, cp3.muncod
				 
			 ORDER BY iduf ';
            // ver($sqlUF,d);
            global $db;
            $UFs = $db->carregar($sqlUF);

            if ($UFs) {
                foreach ($UFs as $key) {
                    $colspan = '6';
                    if ($exercicio == ' Geral ') {
                        $anoTitulo = '<td style="text-align: center"><b>Ano Pactuação</b></td>';
                        $colspan = '7';
                    }
                    echo '<tr>
                <td style="font-size: 20px;padding: 10px 10px 10px 0;text-align: center" colspan="' . $colspan . '">' . $key[descricaoufmun] . '</td>
            </tr>
    <tr>
    <td style="text-align: center"><b>AÇÃO</b></td>
    ' . $anoTitulo . '
    <td style="text-align: center"><b>Quantidade de Profissionais da Saúde Pactuados</b></td>
    <td style="text-align: center"><b>Quantidade de Profissionais da Saúde Capacitados</b></td>
    <td style="text-align: center"><b>Quantidade de Profissionais da Educação Pactuados </b></td>
    <td style="text-align: center"><b>Quantidade de Profissionais da Educação Capacitados</b></td>
    <td style="text-align: center"><b>Total de Profissionais Capacitados</b></td>
</tr>';
                    $sql = "select acaoid, acaodescricao
			from pse.acao 
			where comid = 3
			and acaoano = '2013' 
			order by 1";
                    $dados = $db->carregar($sql);


                    if ($dados) {
                        foreach ($dados as $d) {
                            $sql = "select c3anopactuacao, idmonc3, c3qtprofsaudecapa, c3qtprofeducapa, c3qtprofsaudepactuado, c3qtprofedupactuado, c3totalcapa 
				from pse.monitoracomp3 
				where muncod = '{$key[municipio]}'
                                and acaoid = $d[acaoid] ";

                            if ($_REQUEST['ano']) {
                                $sql .= " AND c3anopactuacao = '{$_REQUEST['ano']}'";
                            }
                            $sql .= " order by c3anopactuacao, acaoid";

                            $registros = $db->carregar($sql);

//
                            if (!empty($registros)) {
                                foreach ($registros as $value) {
                                    if ($exercicio == ' Geral ') {
                                        $anoValor = ' <td style="text-align: center">' . $value[c3anopactuacao] . '</td>';
                                    }
                                    echo
                                    '<tr>
            <td  style="text-align: left">' . $d[acaodescricao] . '</td>
           ' . $anoValor . '
            <td style="text-align: center">' . $value[c3qtprofsaudepactuado] . '</td>
            <td style="text-align: center">' . $value[c3qtprofsaudecapa] . '</td>
            <td style="text-align: center">' . $value[c3qtprofedupactuado] . '</td>
            <td style="text-align: center">' . $value[c3qtprofeducapa] . '</td>
            <td style="text-align: center">' . $value[c3totalcapa] . '</td>
 
        </tr>';
                                }

                            }
                        }
                    }
                }

            } else {
                echo 'Não constam registros para o ano, Estado e Município selecionados.';
            }
        }
        ?>
    </body>