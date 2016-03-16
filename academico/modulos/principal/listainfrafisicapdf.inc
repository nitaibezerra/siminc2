<?php
$entid = $_SESSION['academico']['entid'];
$orgid = $_SESSION['academico']['orgid'];



//LISTA CAMPUS


switch ($orgid) {
    case '1':
        $funid = ACA_ID_CAMPUS;
        break;
    case '2':
        $funid = ACA_ID_UNED;
        break;
}



if($_REQUEST['entidcampus']){
    $entidcampus = addslashes($_REQUEST[entidcampus]);
    $where .= "and  e.entid = {$entidcampus} ";
}

if( ($_REQUEST['entidcampus'] == "" && $_REQUEST['entidreitoria'] == "") || ($_REQUEST['entidcampus'] != "" && $_REQUEST['entidreitoria'] == "") ) {
    $sql = "SELECT
                    e.entid entidcampus,
					upper(e.entnome) as nome,
					upper(mun.mundescricao) as municipio, upper(mun.estuf) as uf,
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' ||  e.entid || '\" colspan=\"6\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					entidade.entidade e2
				INNER JOIN
					entidade.entidade e ON e2.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ea.fueid = ef.fueid
				LEFT JOIN
					entidade.endereco ed ON ed.entid = e.entid
				LEFT JOIN
					territorios.municipio mun ON mun.muncod = ed.muncod
				WHERE
					ea.entid = {$entid} AND
					e.entstatus = 'A' AND ef.funid = {$funid}
					$where
				ORDER BY
					e.entnome";

    $dados = $db->carregar($sql);

    foreach ($dados as $key) {


        $sql = "select audid,ua.entidcampus, ua.entidunidade, ua.entidcampus,mdldsc,ambdsc,m.mdlid,audqtd,audmobiliada,auddsc,a.ambid,ua.entidcampus as entcampus,
                       audareatotal,audacessibilidade,audnivelqldequip,audnivelqtdequip,um.unddsc
                from academico.modulo m
                left join academico.moduloambiente ma on m.mdlid = ma.mdlid
                left join academico.ambiente a on a.ambid = ma.ambid
                LEFT JOIN academico.unidademed um on um.undid = a.undid
                left join academico.unidadeambiente ua on  ua.ambid = a.ambid and ma.mdlid = ua.mdlid and ua.entidcampus = $key[entidcampus]
                order by mdlid";

        $modulos = $db->carregar($sql);

        $html .= "<table align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>
                    <tr>
                        <td colspan='10' align='center' bgcolor='silver'>
                            <b>CAMPUS</b>
                        </td>
                    </tr>
                </table>";

        $html .= "<table align='center' border='0' cellspacing='0' cellpadding='2' style='color:#333333;width: 600px;margin-top: 10px;
			font-size: 11px;
			padding: 3px;
			/*text-align: center;*/
			/*width: 98%;*/
			border-top: 2px solid #404040;
			border-bottom: 3px solid #dfdfdf;
			/*table-layout: fixed;*/
			border-collapse: collapse;'>
        <tr>
            <td colspan='10' style='background: #E3E3E3;text-align: center'>
                $key[nome]
            </td>
        </tr>";

        $mdlid = 0;
        foreach ($modulos as $key) {
            if ($mdlid != $key['mdlid']) {
                $html .= "

                <tr>
                    <td colspan='11' style='font-weight: bold;border-top:2px solid #ccc '>
                        Módulo: $key[mdldsc]
                    </td>
                </tr>
                <tr>
                        <td>
                            Ambiente
                        </td>
                        <td>
                            Acessibilidade
                        </td>
                        <td>
                            Nível Qualidade Equipamento(s)
                        </td>
                        <td>
                            Nível Quantidade Equipamento(s)
                        </td>
                        <td>
                            Unidade
                        </td>
                        <td>
                            Quantidade
                        </td>
                        <td>
                            Área Total (m²)
                        </td>
                        <td>
                            Mobiliada
                        </td>
                        <td>
                            Descrição
                        </td>
                </tr>";

            }

            $html .= "<tr>

                <td>
                    $key[ambdsc]
                </td>

                <td>";
            if ($key['audacessibilidade'] == 'C') $html .= "Completa";
            if ($key['audacessibilidade'] == 'P') $html .= "Parcial";
            if ($key['audacessibilidade'] == 'I') $html .= "Inexistente";
            $html .= "
                </td>

                <td>";
            if ($key['audnivelqldequip'] == 'O') $html .= "Ótima";
            if ($key['audnivelqldequip'] == 'B') $html .= "Bom";
            if ($key['audnivelqldequip'] == 'R') $html .= "Regular";
            if ($key['audnivelqldequip'] == 'N') $html .= "Não se Aplica";
            $html .= "
                </td>

                <td>";
            if ($key['audnivelqtdequip'] == 'C') $html .= "Completa";
            if ($key['audnivelqtdequip'] == 'P') $html .= "Parcial";
            if ($key['audnivelqtdequip'] == 'I') $html .= "Inexistente";
            $html .= "
                </td>

                <td>
                   $key[unddsc]
                </td>
                <td>
                   " . number_format($key[audqtd], 0) . "
                </td>
                <td>
                   " . number_format($key[audareatotal], 0) . "
                </td>

                <td>";
            if ($key['audmobiliada'] == 't') $html .= "Sim";
            if ($key['audmobiliada'] == 'f') $html .= "Não";
            $html .= "
                </td>

                <td>
                    $key[auddsc]
                </td>


            </tr>";
            $mdlid = $key['mdlid'];
        }
        $html .= "</table>" ?>
    <?php
    }
}



//LISTA REITORIA


if( $orgid ){
    $funid = ACA_ID_REITORIA;
}else{
    $funid = ACA_ID_UNED;
}

if($_REQUEST['entidreitoria']){
    $entidreitoria = addslashes($_REQUEST[entidreitoria]);
    $where .= "and  e.entid = {$entidreitoria} ";
}

if( ($_REQUEST['entidcampus'] == "" && $_REQUEST['entidreitoria'] == "") || ($_REQUEST['entidreitoria'] != "" && $_REQUEST['entidcampus'] == "") ) {
    $sql = "SELECT
                    e.entid entidreitoria,
					upper(e.entnome) as nome,
					'<tr><td style=\"padding:0px;margin:0;\"></td><td id=\"td' ||  e.entid || '\" colspan=\"6\" style=\"padding:0px;display:none;border: 5px red\"></td><td style=\"padding:0px;margin:0;\"></td></tr>' as tr
				FROM
					entidade.entidade e2
				INNER JOIN
					entidade.entidade e ON e2.entid = e.entid
				INNER JOIN
					entidade.funcaoentidade ef ON ef.entid = e.entid
				INNER JOIN
					entidade.funentassoc ea ON ea.fueid = ef.fueid
				WHERE
					ea.entid = {$entid} AND
					e.entstatus = 'A' AND ef.funid = {$funid}
					$where
				ORDER BY
					e.entnome";
    
    unset($dados);
    $dados = $db->carregar($sql);

    foreach ($dados as $key) {

        $sql = "select audid,ua.entidcampus, ua.entidunidade, ua.entidcampus,mdldsc,ambdsc,m.mdlid,audqtd,audmobiliada,auddsc,a.ambid,ua.entidcampus as entcampus,
                       audareatotal,audacessibilidade,audnivelqldequip,audnivelqtdequip,um.unddsc
                from academico.modulo m
                left join academico.moduloambiente ma on m.mdlid = ma.mdlid
                left join academico.ambiente a on a.ambid = ma.ambid
                LEFT JOIN academico.unidademed um on um.undid = a.undid
                left join academico.unidadeambiente ua on  ua.ambid = a.ambid and ma.mdlid = ua.mdlid and ua.entidcampus = $key[entidreitoria]
                order by mdlid";

        $modulos = $db->carregar($sql);

        $html .= "<br><br>
                <table align='center' border='0' cellspacing='0' cellpadding='2' class='listagem'>
                    <tr>
                        <td colspan='10' align='center' bgcolor='silver'>
                            <b>REITORIA</b>
                        </td>
                    </tr>
                </table>";

        $html .= "<table align='center' border='0' cellspacing='0' cellpadding='2' style='color:#333333;width: 600px;margin-top: 10px;
			font-size: 11px;
			padding: 3px;
			/*text-align: center;*/
			/*width: 98%;*/
			border-top: 2px solid #404040;
			border-bottom: 3px solid #dfdfdf;
			/*table-layout: fixed;*/
			border-collapse: collapse;'>
        <tr>
            <td colspan='10' style='background: #E3E3E3;text-align: center'>
                $key[nome]
            </td>
        </tr>";

        $mdlid = 0;
        foreach ($modulos as $key) {
            if ($mdlid != $key['mdlid']) {
                $html .= "

                <tr>
                    <td colspan='11' style='font-weight: bold;border-top:2px solid #ccc '>
                        Módulo: $key[mdldsc]
                    </td>
                </tr>
                <tr>
                        <td>
                            Ambiente
                        </td>
                        <td>
                            Acessibilidade
                        </td>
                        <td>
                            Nível Qualidade Equipamento(s)
                        </td>
                        <td>
                            Nível Quantidade Equipamento(s)
                        </td>
                        <td>
                            Unidade
                        </td>
                        <td>
                            Quantidade
                        </td>
                        <td>
                            Área Total (m²)
                        </td>
                        <td>
                            Mobiliada
                        </td>
                        <td>
                            Descrição
                        </td>
                </tr>";

            }

            $html .= "<tr>

                <td>
                    $key[ambdsc]
                </td>

                <td>";
            if ($key['audacessibilidade'] == 'C') $html .= "Completa";
            if ($key['audacessibilidade'] == 'P') $html .= "Parcial";
            if ($key['audacessibilidade'] == 'I') $html .= "Inexistente";
            $html .= "
                </td>

                <td>";
            if ($key['audnivelqldequip'] == 'O') $html .= "Ótima";
            if ($key['audnivelqldequip'] == 'B') $html .= "Bom";
            if ($key['audnivelqldequip'] == 'R') $html .= "Regular";
            if ($key['audnivelqldequip'] == 'N') $html .= "Não se Aplica";
            $html .= "
                </td>

                <td>";
            if ($key['audnivelqtdequip'] == 'C') $html .= "Completa";
            if ($key['audnivelqtdequip'] == 'P') $html .= "Parcial";
            if ($key['audnivelqtdequip'] == 'I') $html .= "Inexistente";
            $html .= "
                </td>

                <td>
                   $key[unddsc]
                </td>
                <td>
                   " . number_format($key[audqtd], 0) . "
                </td>
                <td>
                   " . number_format($key[audareatotal], 0) . "
                </td>

                <td>";
            if ($key['audmobiliada'] == 't') $html .= "Sim";
            if ($key['audmobiliada'] == 'f') $html .= "Não";
            $html .= "
                </td>

                <td>
                    $key[auddsc]
                </td>


            </tr>";
            $mdlid = $key['mdlid'];
        }
        $html .= "</table>" ?>
    <?php
    }
}


include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";
//ob_clean();
$dompdf = new DOMPDF();
$dompdf->load_html($html);
$dompdf->set_paper('A4','portrait');
$dompdf->render();

$arqnome = "Campus_Reitoria_";

if(isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows"){
    $caminhoArq = "C:/tmp/".$arqnome.".pdf";
} else {
    $caminhoArq = "/tmp/".$arqnome.".pdf";
}

file_put_contents($caminhoArq, $dompdf->output());

// caminho completo
$file = $caminhoArq;

// cabeçalho
header("Content-type: application/pdf");
header("Content-Disposition: attachment;filename=$file");

// mostra o download
readfile($file);


?>
