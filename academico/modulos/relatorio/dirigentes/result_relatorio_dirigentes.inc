<?php
    header('Content-Type: text/html; charset=iso-8859-1');

    ini_set("memory_limit","500M");
    set_time_limit(0);

    if ($_REQUEST['filtrosession']){
        $filtroSession = $_REQUEST['filtrosession'];
    }
?>

<html>
    <head>
        <script type="text/javascript" src="../includes/funcoes.js"></script>
        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
        <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    </head>
    <body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">

<?PHP
    include APPRAIZ. 'includes/classes/relatorio.class.inc';

    $dados   = monta_sql();
    $agrup = monta_agp();
    $col   = monta_coluna();

    $r = new montaRelatorio();
    $r->setAgrupador($agrup, $dados);
    $r->setColuna($col);
    $r->setBrasao(true);
    $r->setTotNivel(true);
    $r->setEspandir(false);
    $r->setMonstrarTolizadorNivel(true);
    $r->setTotalizador(true);
    $r->setTolizadorLinha(false);

    if($_POST['tipo_relatorio'] == 'xls'){
        ob_clean();
        $nomeDoArquivoXls="relatorio_avaliacao_final_".date('d-m-Y_H_i');
        echo $r->getRelatorioXls();
    }elseif($_POST['tipo_relatorio'] == 'visual'){
        echo $r->getRelatorio();
    }elseif($_POST['tipo_relatorio'] == 'pdf'){
       // ob_clean();
        echo geraRelatorioPDF( $dados );
    }
?>

    </body>
</html>

<?PHP

    function geraRelatorioPDF( $dados ){

        include_once APPRAIZ . "includes/dompdf/dompdf_config.inc.php";

        $img_file = APPRAIZ . 'www/imagens/brasao.gif';
	$imgData = base64_encode(file_get_contents($img_file));
	$src = 'data:' . mime_content_type($img_file) . ';base64,' . $imgData;
        $data = date('d/m/Y H:i:s');
                
        $style = <<<MARCASTRING
            <style>
                .text-left {
                    text-align: left;
                }
                .text-right {
                    text-align: right;
                }
                .text-center {
                    text-align: center;
                }
                .text-justify {
                    text-align: justify;
                }
                th {
                    text-align: left;
                }
                table {
                    border-collapse: collapse;
                    border-spacing: 0;
                }
                td, th {
                    padding: 0;
                }
                .table {
                    margin-bottom: 20px;
                    max-width: 100%;
                    width: 100%;
                }
                .table > tbody > tr > td, .table > tbody > tr > th, .table > tfoot > tr > td, .table > tfoot > tr > th, .table > thead > tr > td, .table > thead > tr > th {
                    border-top: 1px solid #ddd;
                    line-height: 1.42857;
                    padding: 8px;
                    vertical-align: top;
                }
                .table > thead > tr > th {
                    border-bottom: 2px solid #ddd;
                    vertical-align: bottom;
                }
                .table > caption + thead > tr:first-child > td, .table > caption + thead > tr:first-child > th, .table > colgroup + thead > tr:first-child > td, .table > colgroup + thead > tr:first-child > th, .table > thead:first-child > tr:first-child > td, .table > thead:first-child > tr:first-child > th {
                    border-top: 0 none;
                }
                .table > tbody + tbody {
                    border-top: 2px solid #ddd;
                }
                .table-condensed > tbody > tr > td, .table-condensed > tbody > tr > th, .table-condensed > tfoot > tr > td, .table-condensed > tfoot > tr > th, .table-condensed > thead > tr > td, .table-condensed > thead > tr > th {
                    padding: 5px;
                }
                .table-bordered {
                    border: 1px solid #ddd;
                }
                .table-bordered > tbody > tr > td, .table-bordered > tbody > tr > th, .table-bordered > tfoot > tr > td, .table-bordered > tfoot > tr > th, .table-bordered > thead > tr > td, .table-bordered > thead > tr > th {
                    border: 1px solid #ddd;
                }
                .table-bordered > thead > tr > td, .table-bordered > thead > tr > th {
                    border-bottom-width: 2px;
                }
                .table-hover > tbody > tr:hover {
                    background-color: #f5f5f5;
                }
                .thead{
                    text-align: center;
                    background-color: #d5d5d5;
                    vertical-align: middle;
                }
                .topo{
                    font-size: 13px;
                }
                .conteudo td{
                    font-size: 11px;
                }
                .conteudo th{
                    font-size: 13px;
                    display: table-cell;
                    text-align: center;
                    vertical-align: middle
                }
                .tabela-demanda td, {
                    background-color: #f4f4f4;
                }
                .tabela-entrega {
                    margin-left: 15px;;
                    width: 99%;
                }
                .tabela-entrega td {
                    background-color: #e3eaea;
                }
                .th_entrega {
                    background-color: #cecece;
                }
                .th_demanda {
                    background-color: #bebebe;
                }
            </style>
MARCASTRING;

        $html = <<<MARCASTRING
            <html>
		<head>
                {$style}
                </head>

		<body>
                    <br>
                    <table width="100%" class="table table-condensed topo">
                        <tr>
                            <td class='text-center'>
                                <img src="{$src}" width="60">
                            </td>
                            <td>
                                SIMEC - Sistema Integrado de Monitoramento do Minist?rio da Educa??o<br>
                                Ministério da Educação / SE - Secretaria Executiva<br>
                                Sistema - Demandas FIES
                            </td>
                            <td>
                                Impresso por: <b>{$_SESSION['usunome']}</b><br> Hora da Impressão:{$data}<br>
                            </td>
                        </tr>
                    </table>
                                
                    <table width="100%" class="table top">
                        <tr>
                            <td class="text-center" style="font-size:14px; font-weight: bold;">
                                Relatório - Dirigentes/Intelocutor/Membro do Conselho
                            </td>
                        </tr>
                    </table>

                    <table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="1">
MARCASTRING;

        if($dados != ''){
            foreach( $dados as $i => $colunas ){
                if( $i == 0 ){
                    $html .= "<tr>";

                    if($colunas['nome_campus'] != ''){
                        $html .= "<th>Campus</th>";
                    }

                    $html .= "<th>Cargo</th>";
                    $html .= "<th>Dirigente</th>";

                    if($colunas['tpmdsc'] != ''){
                        $html .= "<th>Tipo Membro</th>";
                    }

                    $html .= "<th>Início da Vigência</th>";
                    $html .= "<th>Fim da Vigência</th>";
                    $html .= "<th>Situação</th>";
                    $html .= "<th>Ato Legal</th>";
                    $html .= "</tr>";
                }

                $html .= "<tr>";

                if($colunas['nome_campus'] != ''){
                    $html .= "<td>{$colunas['nome_campus']}</td>";
                }

                $html .= "<td>{$colunas['fundsc']}</td>";
                $html .= "<td>{$colunas['entnome']}</td>";

                if($colunas['tpmdsc'] != ''){
                    $html .= "<td>{$colunas['tpmdsc']}</td>";
                }

                $html .= "<td>{$colunas['mcsvigenciainicial']}</td>";
                $html .= "<td>{$colunas['mcsvigenciafinal']}</td>";
                $html .= "<td>{$colunas['mcssituacao']}</td>";
                $html .= "<td>{$colunas['arqid']}</td>";
                $html .= "</tr>";
            }
        }else{
            $html = <<<MARCASTRING
                <table id="tabela_filtros" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
                    <tr> <td align="center" style="color:#cc0000;"> Não foram encontrados Registros. </td> </tr>
MARCASTRING;
        }
        
        $html .= <<<MARCASTRING
                </table>
            </body>
        </html>                
MARCASTRING;
                
        if($html != ''){
            $dompdf = new DOMPDF();
            $dompdf->load_html($html);
            $dompdf->set_paper('A4', 'landscape');
            $dompdf->render();
            $arqnome = "relatorio_dirigentes_intelocutor_membro_conselho";

            if (isset($_SERVER['WINDIR']) && $_SERVER['WINDIR'] == "C:\windows") {
                $caminhoArq = "C:\\temp" . $arqnome . ".pdf";
            } else {
                $caminhoArq = "/tmp/" . $arqnome . ".pdf";
            }

            //what if alliens heve been trying to cantact us via the net. but "are you human" filters keep stopping them
            file_put_contents($caminhoArq, $dompdf->output());
            $file = $caminhoArq;
            header("Content-type: application/pdf");
            header("Content-Disposition: attachment;filename=$file");
            readfile($file);
            exit;
	}
    }

    function monta_sql(){
	global $db;

        if( $_POST['unid_camp'] == 'C' ){
            $sql = montaArrayCampusUnidade( $entid, $_POST, $_POST['super_prof'] );
        }else{
            $sql = montaArrayUnidade( $entid, $_POST );
        }

        return $sql;
    }

    function montaArrayUnidade( $entid, $filtros ){
        global $db;

        extract($filtros);

        if( $unid_camp == 'U' && $funid == '21' ){
            $and = " AND mb.tpmid IS NULL AND fen.funid IN (21, 40) ";
            $order = " ORDER BY 7, 1 ASC";
        }

        if( $unid_camp == 'U' && $funid == '123' ){
            $and = " AND tm.tpmnivel = 'I' AND fen.funid IN (123) ";
            $order = " ORDER BY fundsc";
        }

        if( $mcsstatus == 't' || $mcsstatus == 'f' ){
            $and .= " AND mb.mcssituacao = '{$mcsstatus}' ";
        }

        if( $tpmid != '' ){
            $and .= " AND tm.tpmid = '{$tpmid}' ";
        }

        $sql = "
            SELECT  DISTINCT
                    mb.mcsid AS mcsid,
                    fun.fundsc AS fundsc,
                    ent.entnome,
                    tm.tpmdsc,
                    to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                    to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,
                    ende.endcep,
                    ende.endlog,
                    ende.endcom,
                    ende.endbai,
                    ende.estuf,
                    mun.mundsc,
                    CASE WHEN mb.mcssituacao = 't'
                        THEN 'Ativo'
                        ELSE CASE WHEN mb.mcssituacao = 'f'
                                THEN 'Inativo'
                                ELSE ''
                             END
                    END AS mcssituacao,

                    CASE WHEN mb.arqid > 0
                        THEN 'Ato Legal Anexado'
                        ELSE CASE WHEN mb.mcsid IS NULL
                                THEN ''
                                ELSE 'Não Anexado'
                             END
                    END AS arqid
            FROM entidade.funcaoentidade AS fen

            JOIN entidade.funcao AS fun ON fun.funid = fen.funid
            LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
            LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
            LEFT JOIN entidade.entidade AS ent ON ent.entid = fen.entid
            LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid
            LEFT JOIN entidade.funentassoc AS fua2 ON fua2.feaid = mb.feaid
			LEFT JOIN entidade.endereco AS ende ON ende.entid = ent.entid 
			LEFT JOIN public.municipio AS mun ON ende.muncod = mun.muncod 
            WHERE fua.entid = {$entid} {$and}

            {$order}
        ";

        return $db->carregar($sql);
    }

    function montaArrayCampusUnidade( $entid, $filtros, $orgid ){
        global $db;

        extract($filtros);

        switch ( $orgid ){
            case 'S':
                $orgid = ACA_ID_CAMPUS;
                break;
            case 'P':
                $orgid = ACA_ID_UNED;
                break;
        }

        if( $unid_camp == 'C' && $funid == '24' ){
            $and .= " AND mb.tpmid IS NULL AND fen.funid IN (24,40) ";
            $order = " ORDER BY 1 ASC ";
        }

        if( $unid_camp == 'C' && $funid == '123' ){
            $and .= " AND tm.tpmnivel = 'C' AND fen.funid IN (123) ";
            $order = " ORDER BY fundsc ";
        }

        if( $mcsstatus == 't' || $mcsstatus == 'f' ){
            $and .= " AND mb.mcssituacao = '{$mcsstatus}' ";
        }

        if( $tpmid != '' ){
            $and .= " AND tm.tpmid = '{$tpmid}' ";
        }

        if( is_array($camp_id) && count($camp_id) > 0 ){
        	$ids = implode($camp_id, ',');
        }
        
        $sql = "
            SELECT  e.entid AS id_campus,
                    e.entnome AS nome_campus
            FROM entidade.entidade e2

            INNER JOIN entidade.entidade e ON e2.entid = e.entid
            INNER JOIN entidade.funcaoentidade ef ON ef.entid = e.entid
            INNER JOIN entidade.funentassoc ea ON ea.fueid = ef.fueid

            WHERE ea.entid = {$entid} 
            AND e.entstatus = 'A' 
            AND ef.funid = {$orgid} ";

        if( is_array($camp_id) && count($camp_id) > 0 ){
        	$ids = implode($camp_id, ',');
            $sql.= " AND e.entid IN ({$ids}) ";
        }
            
        $sql.= "ORDER BY e.entnome";

        $array_campus = $db->carregar($sql);

        foreach( $array_campus as $i => $campus ){
            $sql = "
                SELECT  DISTINCT
                        mb.mcsid AS mcsid,
                        fun.fundsc AS fundsc,
                        ent.entnome,
                        tm.tpmdsc,
                        to_char(mb.mcsvigenciainicial, 'DD/MM/YYYY') AS mcsvigenciainicial,
                        to_char(mb.mcsvigenciafinal, 'DD/MM/YYYY') AS mcsvigenciafinal,
						ende.endcep,
	                    ende.endlog,
	                    ende.endcom,
	                    ende.endbai,
	                    ende.estuf,
	                    mun.mundsc,
                        CASE WHEN mb.mcssituacao = 't'
                            THEN 'Ativo'
                            ELSE CASE WHEN mb.mcssituacao = 'f'
                                    THEN 'Inativo'
                                    ELSE ''
                                 END
                        END AS mcssituacao,

                        CASE WHEN mb.arqid > 0
                            THEN 'Ato Legal Anexado'
                            ELSE CASE WHEN mb.mcsid IS NULL
                                    THEN ''
                                    ELSE 'Não Anexado'
                                 END
                        END AS arqid

                FROM entidade.funcaoentidade AS fen
                LEFT JOIN entidade.funcao AS fun ON fun.funid = fen.funid
                LEFT JOIN academico.membroconselho AS mb ON mb.entid = fen.entid
                LEFT JOIN academico.tipomembro AS tm ON tm.tpmid = mb.tpmid
                LEFT JOIN entidade.entidade AS ent ON ent.entid = mb.entid
                LEFT JOIN entidade.funentassoc AS fua ON fua.fueid = fen.fueid AND fua.feaid = mb.feaid
				LEFT JOIN entidade.endereco AS ende ON ende.entid = ent.entid 
				LEFT JOIN public.municipio AS mun ON ende.muncod = mun.muncod 
                WHERE fua.entid = {$campus['id_campus']} {$and}

                {$order}
            ";
            $array_dirigentes = $db->carregar($sql);

            #VERIFICA SE A RESULTADO DE DIRIGENTES PARA DETERMINADO CAMPUS
            if( $array_dirigentes[$i]['mcsid'] > 0 ){
                foreach( $array_dirigentes as $k => $dirigentes ){
                    $array_result[] = array_merge( $array_campus[$i], $dirigentes);
                }
            }
        }
        return $array_result;
    }

    function monta_coluna(){
	$colunas = $_POST['colunas'];
	$colunas = $colunas ? $colunas : array();

	$coluna = array();

	foreach ($colunas as $val){
            switch ($val){
                case 'funid':
                    if($_POST['tipo_relatorio'] == 'xls'){
                        array_push( $coluna,
                            array(
                                "campo" => "fundsc",
                                "label" => "Cargo",
                                "type"  => "string"
                            )
                        );
                    }else{
                        array_push( $coluna,
                            array(
                                "campo" => "fundsc",
                                "label" => "Cargo",
                                "type"  => "string",
                                "html"  => "<div style=\"text-align: left;\">{fundsc}</div>"
                            )
                        );
                    }
                    continue;
                break;
                case 'entnome':
                    if($_POST['tipo_relatorio'] == 'xls'){
                        array_push( $coluna,
                            array(
                                "campo" => "entnome",
                                "label" => "Dirigente",
                                "type"  => "string"
                            )
                        );
                    }else{
                        array_push( $coluna,
                            array(
                                "campo" => "entnome",
                                "label" => "Dirigente",
                                "type"  => "string",
                                "html"  => "<div style=\"text-align: left;\">{entnome}</div>"
                            )
                        );
                    }
                    continue;
                break;
                case 'nome_campus':
                    if($_POST['tipo_relatorio'] == 'xls'){
                        array_push( $coluna,
                            array(
                                "campo" => "nome_campus",
                                "label" => "Campus",
                                "type"  => "string"
                            )
                        );
                    }else{
                        array_push( $coluna,
                            array(
                                "campo" => "nome_campus",
                                "label" => "Campus",
                                "type"  => "string",
                                "html"  => "<div style=\"text-align: left;\">{nome_campus}</div>"
                            )
                        );
                    }
                    continue;
                break;
                case 'tpmdsc':
                    array_push( $coluna,
                        array(
                            "campo" => "tpmdsc",
                            "label" => "Tipo de Membro",
                            "type"  => "string"
                        )
                    );
                    continue;
                break;
                case 'mcsvigenciainicial':
                    array_push( $coluna,
                        array(
                            "campo" => "mcsvigenciainicial",
                            "label" => "Início da Vigência",
                            "type"  => "date"
                        )
                    );
                    continue;
                break;
                case 'mcsvigenciafinal':
                    array_push( $coluna,
                        array(
                            "campo" => "mcsvigenciafinal",
                            "label" => "Fim da Vigência",
                            "type"  => "date"
                        )
                    );
                    continue;
                break;
                case 'mcssituacao':
                    array_push( $coluna,
                        array(
                            "campo" => "mcssituacao",
                            "label" => "Situação",
                            "type"  => "string"
                        )
                    );
                    continue;
                break;
                case 'arqid':
                    array_push( $coluna,
                        array(
                            "campo" => "arqid",
                            "label" => "Ato Legal",
                            "type"  => "string"
                        )
                    );
                    continue;
                break;
                case 'endcep':
                	array_push( $coluna,
                		array(
                			"campo" => "endcep",
                			"label" => "CEP",
                			"type"  => "string"
                		)
                	);
                	continue;
                break;
                case 'endlog':
                	array_push( $coluna,
                		array(
                			"campo" => "endlog",
                			"label" => "Logradouro",
                			"type"  => "string"
               			)
                	);
                	continue;
                break; 
                case 'endcom':
                	array_push( $coluna,
                		array(
                			"campo" => "endcom",
                			"label" => "Complemento",
                			"type"  => "string"
               			)
                	);
                	continue;
                break;
                case 'endbai':
                	array_push( $coluna,
	                	array(
    		            	"campo" => "endbai",
            		    	"label" => "Bairro",
                			"type"  => "string"
                		)
                	);
                	continue;
                break;
                case 'estuf':
                	array_push( $coluna,
                		array(
                			"campo" => "estuf",
                			"label" => "UF",
                			"type"  => "string"
                		)
                	);
                	continue;
                break;
                case 'mundsc':
                	array_push( $coluna,
                		array(
                			"campo" => "mundsc",
                			"label" => "Município",
                			"type"  => "string"
                		)
                	);
                	continue;
                break;
            }
	}
	return $coluna;
    }

    function monta_agp(){
	$agrupador = $_POST['agrupador'];

	$agp = 	array(
            "agrupador" => array(),
            "agrupadoColuna" => array("nome_campus","fundsc","entnome","tpmdsc","mcsvigenciainicial","mcsvigenciafinal","mcssituacao","arqid","endcep","endlog","endcom","endbai","estuf","mundsc")
        );

	#Agrupador padrão.
        if( $_POST['unid_camp'] == 'C' ){
            array_push($agp['agrupador'], array("campo" => "nome_campus", "label" => "Campus"));
        }
	array_push($agp['agrupador'], array("campo" => "mcsid", "label" => "Código"));

	return $agp;
}
?>