<?php
    $anoref = $_REQUEST['anoref'] ? $_REQUEST['anoref'] : date('Y')-1;

    $sql = "
        select entid
        from academico.usuarioresponsabilidade
        where usucpf = '$_SESSION[usucpf]' and rpustatus = 'A'
    ";
    $entid = $db->pegaUm($sql);

    $sql2 = "
        select  tcuid,tcucodigo,tcutipo
        from academico.indicadorestcu
        order by tcutipo, tcuid
    ";
    $dados2 = $db->carregar($sql2);

    #Pega os indicadores TCU do tipo Componentes
    $sql = "
        select  tcuid,tcucodigo,tcutipo
        from academico.indicadorestcu

        where tcutipo = 'C'
        order by tcutipo, tcuid
    ";
    $dadosComponentes = $db->carregar($sql);

    #Pega os indicadores TCU do tipo Indicadores
    $sql = "
        select  tcuid,tcucodigo,tcutipo
        from academico.indicadorestcu

        where tcutipo = 'I'
        order by tcutipo, tcuid
    ";
    $dadosIndicadores = $db->carregar($sql);

    $sql3 = "
        select  max(tcuid) as maiorcomp,
                count(*) as quantcomp,
                (select count(*) as quantind from academico.indicadorestcu where tcutipo='I') as quantind
        from academico.indicadorestcu

        where tcutipo='C'
    ";
    $dados3 = $db->pegaLinha($sql3);

    $maiorcomponente = $dados3['maiorcomp'];
    $quantcomponente = $dados3['quantcomp'];
    $quantindicador  = $dados3['quantind'];


    #SQL PARA CÁLCULO DA MÉDIA DOS VALORES
    $sqlmedia = "SELECT ";
    $i = 1;

    foreach($dados2 as $tipo){
        $sqlmedia .= "ROUND(SUM(a._".str_replace(".","",trim($tipo['tcuid']))."),2) AS _col".$i.",";
        $i++;
    }
    $sqlmedia = substr($sqlmedia,0,-1);

    $sqlmedia .= " FROM (SELECT it.tcuid, ";

    foreach($dados2 as $tipo){
        $sqlmedia .= "CASE WHEN it.tcuid = ".$tipo['tcuid']." THEN avg(coalesce(li.lcivalor,0)) ELSE 0 END as _".str_replace(".","",$tipo['tcuid']).",";
    }
    $sqlmedia = substr($sqlmedia,0,-1);

    $sqlmedia .= "
        FROM academico.lancamentoindicador li
        INNER JOIN academico.indicadorestcu it ON it.tcuid = li.tcuid
    ";

    #Caso seja superusuario
    if( verificaPerfilAcademico( Array() ) ){
        $sqlmedia .= "
            INNER JOIN academico.movimentoindicador mi ON mi.mviid = li.mviid WHERE li.lciano = '{$anoref}'
        ";
    #Caso o usuário esteja ligado a uma instituição
    }elseif(!empty($entid)){
        $sqlmedia .= "
            INNER JOIN academico.movimentoindicador mi ON mi.mviid = li.mviid WHERE mi.entid = '$entid' AND li.lciano = '{$anoref}'
        ";
    #Pegar todos que estão cadastrados
    }else {
	$sqlmedia .= "
            INNER JOIN academico.movimentoindicador mi ON mi.mviid = li.mviid

            WHERE mi.entid in (
                select distinct entid
                from academico.usuarioresponsabilidade
                where rpustatus = 'A' and pflcod=".PERFIL_IFESCADTCU.") AND li.lciano = '{$anoref}'
        ";
    }

    $sqlmedia .= "
        GROUP BY it.tcuid
        ORDER BY tcuid) as a
    ";
    //ver($sqlmedia,d);
    $dadosmedia = $db->pegaLinha($sqlmedia);

    $sqlgeral =  "
        SELECT  a.entid,
                a.entsig,
    ";
    $i = 1;

    foreach($dados2 as $tipo){
        $sqlgeral .= "SUM(a._".str_replace(".","",$tipo['tcuid']).") AS _col".$i.",";
        $i++;
    }
    $sqlgeral = substr($sqlgeral,0,-1);

    $sqlgeral .= "
        FROM (
            SELECT  e.entid,
                    e.entsig,
    ";

    foreach($dados2 as $tipo){
	$sqlgeral .= "CASE WHEN it.tcuid = ".$tipo['tcuid']." THEN COALESCE(li.lcivalor, 0) ELSE 0 END AS _".str_replace(".","",$tipo['tcuid']).",";
    }
    $sqlgeral = substr($sqlgeral,0,-1);

    $sqlgeral .= "
        FROM entidade.entidade e

        INNER JOIN academico.movimentoindicador mi ON mi.entid = e.entid
        LEFT JOIN academico.lancamentoindicador li ON mi.mviid = li.mviid
        LEFT JOIN academico.indicadorestcu it ON it.tcuid = li.tcuid
    ";

    #Caso seja superusuario
    if( verificaPerfilAcademico(Array()) ){
        $sqlgeral .= " WHERE li.lciano = '{$anoref}'";
    #Caso o usuário esteja ligado a uma instituição
    }elseif(!empty($entid)){
        $sqlgeral .= " where e.entid = '$entid' AND li.lciano = '{$anoref}'";
    #Pegar todos que estão cadastrados
    }else{
        $sqlgeral .= "
            WHERE e.entid in (
                SELECT DISTINCT entid
                FROM academico.usuarioresponsabilidade
                WHERE rpustatus = 'A' AND pflcod=".PERFIL_IFESCADTCU.") AND li.lciano = '{$anoref}'
        ";
    }

    $sqlgeral .= "
        ) AS a
	GROUP BY entid, entsig
        ORDER BY entsig
    ";
    $dados = $db->carregar($sqlgeral);


    if ($_REQUEST) {
        if (!empty($dados)) {
            switch (substr($_POST['exibir'], 10, 3)) {
                case 'xls':
                    $nomeDoArquivoXls = "indicadores_TCU_". gmdate("d-m-Y-H:i:s");
                    $xls = new GeraExcel();

                    $xls->MontaConteudoString(0, 2, "Componentes");
                    $xls->MontaConteudoString(0, 9, "Indicadores");


                    $xls->MontaConteudoString(1, 0, "CódIFES");
                    $xls->MontaConteudoString(1, 1, "SiglaIFES");
                    $n = 2;
                    foreach ($dados2 as $codigos) {
                        echo $xls->MontaConteudoString(1, $n, $codigos['tcucodigo']);
                        $n++;
                    }

                    $i = 2;
                    foreach ($dados as $registro) {
                        #Gerar linha XLS
                        $xls->MontaConteudoString($i, 0, $registro['entid']);
                        $xls->MontaConteudoString($i, 1, $registro['entsig']);
                        $total = ($quantcomponente + $quantindicador + 1);
                        for ($n = 1; $n <= $total; $n++) {
                            //$numero = number_format($registro['_col'.$n], 2, ',', '.');
                            echo $xls->MontaConteudonumero($i, ($n + 1), $registro['_col' . $n]);
                        }
                        $i++;
                    }

                    #média
                    $xls->MontaConteudoString($i, 0, '');
                    $xls->MontaConteudoString($i, 1, 'Média');
                    $total = ($quantcomponente + $quantindicador + 1);
                    for ($n = 1; $n <= $total; $n++) {
                        //$numero = number_format($dadosmedia['_col'.$n], 2, ',', '.');
                        echo $xls->MontaConteudonumero($i, ($n + 1), $dadosmedia['_col' . $n]);
                    }

                    ob_clean();
                    $xls->GeraArquivo();
                    break;

                case 'pdf':
                    ob_clean();
                    require('../../includes/fpdf/fpdf.inc');
                    $data = date('j/m/Y H:i:s');

                    $PDF = new FPDF('L', 'cm', "A4");
                    $PDF->SetMargins(0.3, 1, 0);
                    $PDF->SetAuthor('SIMEC');
                    $PDF->SetTitle('SIMEC');
                    $PDF->AddPage();

                    #inicio do cabeçalho
                    $PDF->SetFont('Arial', 'B', 6);

                    $PDF->Cell(6, 0.3, 'SIMEC - Sistema de Integrado de Planejamento, Orçamento e Finanças', 0, 'L');
                    $PDF->Cell(22.5, 0.3, $data, 0, 'L', 'R');
                    $PDF->ln(0.3);
                    $PDF->Cell(6, 0.3, 'Ministério da Educação - MEC', 0, 'L');
                    $PDF->ln(0.3);
                    $PDF->Cell(6, 0.3, 'Relatório de Indicadores do TCU', 0, 'L');
                    #fim do cabeçalho

                    $arrTipoIndicadores = array("Componentes", "Indicadores");


                    foreach ($arrTipoIndicadores as $tipoIndicadores):

                        if ($tipoIndicadores == 'Componentes') {
                            $total = $quantcomponente;
                            $dadosCabecalho = $dadosComponentes;
                            $iniLoop = 1;
                            $tamCol = 3.77;
                        } else {
                            $total = ($quantcomponente + $quantindicador);
                            $dadosCabecalho = $dadosIndicadores;
                            $iniLoop = $quantcomponente + 1;
                            $tamCol = 2.2;
                        }

                        $PDF->ln(1);
                        $PDF->SetFont('Arial', 'B', 6);

                        #Cabeçalho das células
                        $PDF->SetFillColor(207, 207, 207);
                        $PDF->Cell(29.2, 0.30, $tipoIndicadores, 1, 'L', 'C', 1);
                        $PDF->ln();
                        $PDF->Cell(1.4, 0.6, 'CódIFES', 1, 'L', 'C', 1);
                        $PDF->Cell(1.4, 0.6, 'SiglaIFES', 1, 'L', 'C', 1);
                        
                        //$n = 2;
                        //foreach( $dados2 as $codigos){
                        reset($dadosCabecalho);
                        foreach ($dadosCabecalho as $codigos) {
                            $PDF->Cell($tamCol, 0.6, $codigos['tcucodigo'], 1, 'L', 'C', 1);
                        //$n++;
                        }
                        $PDF->ln();
                        $PDF->SetFont('Arial', '', 5);

                        #Dados
                        //$i=1;
                        reset($dados);
                        foreach ($dados as $registro) {
                            $PDF->Cell(1.4, 0.5, $registro['entid'], 1, 'L', 'C');
                            $PDF->Cell(1.4, 0.5, $registro['entsig'], 1, 'L', 'L');
                            //$total = ($quantcomponente + $quantindicador);
                            //$total = $quantcomponente;
                            for ($n = $iniLoop; $n <= $total; $n++) {
                                $PDF->Cell($tamCol, 0.5, number_format($registro['_col' . $n], 2, ',', '.'), 1, 'L', 'R');
                            }
                            //$i++;
                            $PDF->ln();
                        }
                        
                        #Média
                        $PDF->SetFont('Arial', 'B', 6);
                        $PDF->SetFillColor(207, 207, 207);
                        $PDF->Cell(2.8, 0.5, 'Média', 1, 'L', 'C', 1);
                        //$total = ($quantcomponente + $quantindicador);

                        $PDF->SetFont('Arial', 'B', 5);
                        //echo $iniLoop . "<br/>";
                        for ($n = $iniLoop; $n <= $total; $n++) {
                            $PDF->Cell($tamCol, 0.5, number_format($dadosmedia['_col' . $n], 2, ',', '.'), 1, 'L', 'R', 1);
                        }
                    endforeach;
                    $PDF->Output('indicadores_TCU_'.gmdate("d-m-Y-H:i:s").'.pdf','D');
                    //$PDF->Output();
                    break;
            }
        }
    }

    include APPRAIZ . 'includes/cabecalho.inc';
    
    echo "<br/>";
    
    $titulo_modulo = "Relatório Indicadores TCU";
    
    monta_titulo( $titulo_modulo,'');    
?>


<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

    $(document).ready(function(){
        $('#anoref').change(function(){
                document.location.href = '?modulo=relatorio/relIndicadoresTCU&acao=A&anoref='+this.value;
        });
    });

    function exibirRelatorio( param ) {
        if( param == 'PDF'){
            $('#exibir').val('relatorio_pdf ');
        }else{
            $('#exibir').val('relatorio_xls ');
        }
        $('#formulario').submit();
    }

</script>

<br>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="exibir" name="exibir" value="">

    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita"> Ano de referência</td>
            <td>
                <?PHP
                    $ano = date(Y)-1;
                    for( $x = $ano; $x >= 2006; $x-- ){
                        $arr_sql[] = array("codigo"=>$x, "descricao"=>$x);
                    }
                    $db->monta_combo('anoref', $arr_sql, 'S', '', '', '', '', 105, 'N', 'anoref', '', '');
                ?>
            </td>

        </tr>
        <tr bgcolor="#C0C0C0">
            <td align="center" colspan="2">
                <input type="button" value="Gerar PDF" onclick="exibirRelatorio('PDF');">
                <input type="button" value="Gerar XLS" onclick="exibirRelatorio('XLS');">
            </td>
        </tr>
    </table>

    <br>

    <table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
        <thead>
            <tr>
                <td width="5%" align="center" class="title" rowspan="2" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Cód IFES</strong>
                </td>
                <td width="5%" align="center" class="title" rowspan="2"onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Sigla IFES</strong>
                </td>
                <td width="45%" align="center" class="title" colspan="<?= $quantcomponente; ?>" style="border-bottom: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Componentes</strong>
                </td>
                <td width="45%" align="center" class="title" colspan="<?= $quantindicador; ?>" style="border-bottom: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">
                    <strong>Indicadores</strong>
                </td>
            </tr>
            <tr>
                <?PHP
                    foreach ($dados2 as $codigos) {
                        echo "<td align=\"center\"><b>" . $codigos['tcucodigo'] . "</b></td>";
                    }
                ?>
            </tr>
        </thead>

        <?PHP
            $nc = 0;

            if ($dados) {
                foreach ($dados as $registro) {
                    $cor = ($nc % 2) ? '"#F7F7F7"' : '"#FFFFFF"';

                    echo "
                        <tr bgcolor = $cor onmouseover='this.bgColor=\"#ffffcc\";' onmouseout='this.bgColor=$cor;'>
                            <td align='center'>" . $registro['entid'] . "</td>
                            <td>" . $registro['entsig'] . "</td>
                    ";
                    $total = ($quantcomponente + $quantindicador);

                    for ($i = 1; $i <= $total; $i++) {
                        echo "<td align='right'>" . number_format($registro['_col' . $i], 2, ',', '.') . "</td>";
                    }
                    echo "</tr>";
                    $nc++;
                }

                //ESCREVE A MÉDIA
                echo "
                    <tr bgcolor='#C5C5C5'>
                        <td align='center' colspan='2'>
                            <b>Média</b>
                        </td>
                ";
                $total = ($quantcomponente + $quantindicador);

                for ($i = 1; $i <= $total; $i++) {
                    echo "<td align='right'><b>" . number_format($dadosmedia['_col' . $i], 2, ',', '.') . "</b></td>";
                }
                echo "</tr>";
            }
        ?>

    </table>
</form>



