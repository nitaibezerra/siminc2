<?php
// Cabeçalho do arquivo para ele baixar
header('Content-Type: application/vnd.ms-excel');
header('Content-Disposition: attachment;filename="relatorio_de_modulos.xls"');
header('Cache-Control: max-age=0');
// Se for o IE9, isso talvez seja necessário
header('Cache-Control: max-age=1');

$entid = $_SESSION['academico']['entid'];
$orgid = $_SESSION['academico']['orgid'];

//LISTA CAMPUS

if($_REQUEST['entidcampus']){
    $entidcampus = addslashes($_REQUEST[entidcampus]);
    $where = "and  e.entid = {$entidcampus} ";
}

switch ($orgid) {
    case '1':
        $funid = ACA_ID_CAMPUS;
        break;
    case '2':
        $funid = ACA_ID_UNED;
        break;
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

        $modulos = $db->carregar($sql); ?>
        <table align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr>
                <td colspan="10" align="center" bgcolor="silver">
                    <b>CAMPUS</b>
                </td>
            </tr>
        </table>
        <table align="center" border="0" cellspacing="0" cellpadding="2" style="color:#333333;" class="listagem">
            <tr>
                <td colspan="10">
                    <?php echo $key['nome'] . "<br>"; ?>
                </td>
            </tr>
            <?php
            $mdlid = 0;
            foreach ($modulos as $key) {
                if ($mdlid != $key['mdlid']) {
                    ?>

                    <tr>
                        <td colspan="11">
                            Módulo: <?php echo $key['mdldsc'] ?>
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
                    </tr>

                <?php } ?>
                <tr>
                    <td>
                        <?php echo $key['ambdsc']; ?>
                    </td>
                    <td>
                        <?php if ($key['audacessibilidade'] == 'C') echo "Completa"; ?>
                        <?php if ($key['audacessibilidade'] == 'P') echo "Parcial"; ?>
                        <?php if ($key['audacessibilidade'] == 'I') echo "Inexistente"; ?>
                    </td>
                    <td>
                        <?php if ($key['audnivelqldequip'] == 'O') echo "Ótima"; ?>
                        <?php if ($key['audnivelqldequip'] == 'B') echo "Bom"; ?>
                        <?php if ($key['audnivelqldequip'] == 'R') echo "Regular"; ?>
                        <?php if ($key['audnivelqldequip'] == 'N') echo "Não se Aplica"; ?>
                    </td>
                    <td>
                        <?php if ($key['audnivelqtdequip'] == 'C') echo "Completa"; ?>
                        <?php if ($key['audnivelqtdequip'] == 'P') echo "Parcial"; ?>
                        <?php if ($key['audnivelqtdequip'] == 'I') echo "Inexistente"; ?>
                    </td>
                    <td>
                        <?=$key['unddsc']; ?>
                    </td>
                    <td>
                        <?php echo number_format($key['audqtd'], 0); ?>
                    </td>
                    <td>
                        <?=number_format($key['audareatotal'], 0); ?>
                    </td>
                    <td>
                        <?php if ($key['audmobiliada'] == 't') echo "Sim"; ?>
                        <?php if ($key['audmobiliada'] == 'f') echo "Não"; ?>
                    </td>

                    <td>
                        <?php echo $key['auddsc']; ?>
                    </td>
                </tr>
                <?php
                $mdlid = $key['mdlid'];
            } ?>

        </table>
    <?php
    }
}

//LISTA REITORIA

if($_REQUEST['entidreitoria']){
    $entidreitoria = addslashes($_REQUEST[entidreitoria]);
    $where = "and  e.entid = {$entidreitoria} ";
}

if( $orgid ){
    $funid = ACA_ID_REITORIA;
}else{
    $funid = ACA_ID_UNED;
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

        $modulos = $db->carregar($sql); ?>
        <br><br>
        <table align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
            <tr>
                <td colspan="10" align="center" bgcolor="silver">
                    <b>REITORIA</b>
                </td>
            </tr>
        </table>
        <table align="center" border="0" cellspacing="0" cellpadding="2" style="color:#333333;" class="listagem">
            <tr>
                <td colspan="10">
                    <?php echo $key['nome'] . "<br>"; ?>
                </td>
            </tr>
            <?php
            $mdlid = 0;
            foreach ($modulos as $key) {
                if ($mdlid != $key['mdlid']) {
                    ?>

                    <tr>
                        <td colspan="11">
                            Módulo: <?php echo $key['mdldsc'] ?>
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
                    </tr>

                <?php } ?>
                <tr>
                    <td>
                        <?php echo $key['ambdsc']; ?>
                    </td>
                    <td>
                        <?php if ($key['audacessibilidade'] == 'C') echo "Completa"; ?>
                        <?php if ($key['audacessibilidade'] == 'P') echo "Parcial"; ?>
                        <?php if ($key['audacessibilidade'] == 'I') echo "Inexistente"; ?>
                    </td>
                    <td>
                        <?php if ($key['audnivelqldequip'] == 'O') echo "Ótima"; ?>
                        <?php if ($key['audnivelqldequip'] == 'B') echo "Bom"; ?>
                        <?php if ($key['audnivelqldequip'] == 'R') echo "Regular"; ?>
                        <?php if ($key['audnivelqldequip'] == 'N') echo "Não se Aplica"; ?>
                    </td>
                    <td>
                        <?php if ($key['audnivelqtdequip'] == 'C') echo "Completa"; ?>
                        <?php if ($key['audnivelqtdequip'] == 'P') echo "Parcial"; ?>
                        <?php if ($key['audnivelqtdequip'] == 'I') echo "Inexistente"; ?>
                    </td>
                    <td>
                        <?=$key['unddsc']; ?>
                    </td>
                    <td>
                        <?php echo number_format($key['audqtd'], 0); ?>
                    </td>
                    <td>
                        <?=number_format($key['audareatotal'], 0); ?>
                    </td>
                    <td>
                        <?php if ($key['audmobiliada'] == 't') echo "Sim"; ?>
                        <?php if ($key['audmobiliada'] == 'f') echo "Não"; ?>
                    </td>

                    <td>
                        <?php echo $key['auddsc']; ?>
                    </td>
                </tr>

                <?php
                $mdlid = $key['mdlid'];
            } ?>

        </table>
    <?php
    }
}

die();



?>
