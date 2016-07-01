<?php

set_time_limit(0);
ini_set("memory_limit", "4000M");
montaBotaoForm();
//include APPRAIZ. 'includes/classes/relatorio.class.inc';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if ($_POST['limpaSession']) {
    unset($_SESSION['par']['post']);
    $_POST['limpaSession'] = "";
}

if (empty($_SESSION['par']['post'])) {
    $_SESSION['par']['post'] = $_POST;
}

$col = monta_coluna();
$coluna = array();
$cabecalho = array();
$id = array();

foreach ($col as $c) {
    if ($c['campo']) {
        $coluna[] = $c['campo'];
    }
    $cabecalho[] = $c['label'];

    if (strstr(strtoupper($c['label']), strtoupper('valor'))) {
        $formato[] = "n";
    } else {
        $formato[] = "";
    }
    $id[] = $c['id'];
}

$arSql = monta_sql($coluna, $id);
sort($arSql['sql']);

//gera relatorio XLS
if ($_POST['exporta'] == "true") {
    global $db;
    ob_clean();
    header('content-type: text/html; charset=ISO-8859-1');
    sort($arSql['excel']);
    $db->sql_to_excel($arSql['excel'], 'relFinanceiroPar', $cabecalho, $formato);
    exit;
    $exporta = "false";
}

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
        "<td colspan=\"100\">" .
        monta_cabecalho_relatorio('100') .
        "</td>" .
        "</tr>
			  </table>";

echo $cabecalhoBrasao;
$db->monta_lista_array($arSql['sql'], $cabecalho, 100000, 1, 'N', 'center', '');

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaJoinColunas($arColunas, $arFiltros) {
    $arJoin = array();
    $arCols = array();

    //foreach( $arColunas as $stColuna ){
    /* if( in_array( 'responsavel', $arColunas ) || in_array( 'responsavel', $arFiltros ) ){
      (in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
      $join = "left join emenda.responsavel res on res.resid = eme.resid";
      (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
      } */
    /* if( in_array( 'tpeid', $arColunas ) || in_array( 'tpeid', $arFiltros ) ){
      (in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
      $join = "left join emenda.responsavel res on res.resid = eme.resid";
      (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
      $join = "left join emenda.tipoensino tpe on tpe.resid = res.resid and tpe.tpestatus = 'A'";
      ( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);

      } */
    /* if( in_array( 'entidade', $arColunas) || in_array( 'valorentidade', $arColunas) || in_array( 'usucpf', $arColunas) || in_array( 'entidade', $arFiltros) || 
      in_array( 'estufentidade', $arFiltros) || in_array( 'mundescricao', $arColunas) ){
      ( in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
      $join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
      left join territorios.municipio mun on mun.muncod = ent.muncod";
      ( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);

      } */
    /* ver($arColunas,d);
      if( in_array( 'emeano', $arColunas ) || in_array( 'autnome', $arColunas ) ){
      //(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
      $join = "inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
      left join emenda.autor aut
      inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
      on aut.autid = vede.autid";
      (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
      } */
    $arJoinPTA = array();
    if (in_array('ptrcod', $arColunas) || in_array('ptivalortotal', $arColunas) || in_array('pmcnumconveniosiafi', $arColunas) || in_array('ptrnumconvenio', $arColunas) /* || 
      in_array( 'esddsc', $arColunas)  || in_array( 'exfvalor', $arColunas) ||
      in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'esddsc', $arFiltros) || in_array( 'ininome', $arColunas) ||
      in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) || in_array( 'ptrnumconvenio', $arFiltros) ||
      in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas ) || in_array( 'iniid', $arFiltros ) ||
      in_array( 'unisigla', $arColunas ) || in_array( 'ptrnumprocessoempenho', $arFiltros ) || in_array( 'ptrnumprocessoempenho', $arColunas )
      || in_array( 'pedvalor', $arFiltros ) */) {

        (in_array("(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer", $arCols) ? '' : $arCols[] = "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer");
        //(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
        //(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
        //(in_array( 'ptr.ptrid', $arCols ) ? '' : $arCols[] = 'ptr.ptrid');

        /* if( in_array( 'ininome', $arColunas) || in_array( 'iniid', $arFiltros ) ){
          $join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);

          $join = "inner join emenda.iniciativa ini on ini.iniid = pti.iniid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
          } */

        /* if( in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) ){				
          $join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);

          $join = "inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);

          $join = "inner join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);

          $join = "inner join emenda.especificacao esp on esp.espid = ine.espid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
          } */

        /* if( in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'datareferencia', $arColunas ) ){
          $join = "left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
          left join emenda.ptpublicacao pt on pt.pmcid = ptmi.pmcid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
          }

          if( in_array( 'esddsc', $arColunas) || in_array( 'esddsc', $arFiltros) ){
          $join = "left join workflow.documento doc on ptr.docid = doc.docid
          left join workflow.estadodocumento esd on esd.esdid = doc.esdid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
          } */

        /*
          if( in_array( 'pmcnumconveniosiafi', $arColunas) ){
          $join = "left join emenda.dadosconveniosiafi dcs on substring(dcs.it_nu_original from 1 for 6) = ptr.ptrnumconvenio";
          (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
          }
         */

        if (in_array('empenhado', $arFiltros) || in_array('pagamento', $arFiltros)) {
            $join = "left join emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and pte.pedid = ef.pedid and ef.exfstatus = 'A'
						left join emenda.ordembancaria ord on ord.exfid = ef.exfid ";
            (in_array($join, $arJoin) ? '' : $arJoin[] = $join);
        }
        if (in_array('ptivalortotal', $arColunas)) {
            $join = "LEFT JOIN
                        (SELECT ptrid, SUM(ptivalortotal) AS ptivalortotal
                         FROM emenda.v_ptiniciativa 
                         GROUP BY ptrid) vpti ON vpti.ptrid = ptr.ptrid";
            (in_array($join, $arJoin) ? '' : $arJoin[] = $join);
        }
        /* if( in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas) ){
          $join = "left join emenda.ptvigencia ptv ON ptv.ptrid = ptr.ptrid";
          (in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
          } */

        /* $join = "left join emenda.ptemendadetalheentidade pede
          inner join emenda.planotrabalho ptr on ptr.ptrid = pede.ptrid and ptr.ptrstatus = 'A' and ptr.ptrexercicio = ".$_SESSION['exercicio']."
          ".implode(' ', $arJoinPTA)."
          on ede.edeid = pede.edeid";

          (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
          $join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
          left join territorios.municipio mun on mun.muncod = ent.muncod";
          ( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join); */

        //ver($arColunas,$arJoin,d);
    }
    /* if( in_array( 'obcdsc', $arColunas) ){
      (in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
      (in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
      }
      if( in_array( 'iniid', $arColunas) || in_array( 'iniid', $arFiltros) ){
      (in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
      (in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');

      if( in_array( 'iniid', $arColunas) ){
      $join = "left join emenda.iniciativadetalheentidade ide
      inner join emenda.iniciativa ini1 on ini1.iniid = ide.iniid
      on ide.edeid = ede.edeid";
      (in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
      }

      } */
    //}
    return array("colunas" => $arCols,
        "joins" => $arJoin);
}

function montaBotaoForm() {
    echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}

function monta_sql($cols, $id) {
    global $db;
    $arFiltros = array();
    $carregaCPFresp = false;
    $inner = "";
    $select = "";
    extract($_SESSION['par']['post']);

    $arColunas = array_merge($cols, $id);
    $arJoinColunas = montaJoinColunas($arColunas, $arFiltros);
    $arJoinColunas['colunas'] = array_merge($arJoinColunas['colunas'], $cols);
    $stColunas = (is_array($arJoinColunas['colunas']) && $arJoinColunas['colunas'][0]) ? ', ' . implode(', ', $arJoinColunas['colunas']) : "*";

    if ($estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )) {
        $where[] = " (iu.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $estuf) . "') OR iu.mun_estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $estuf) . "'))";
    }
    if ($documentos[0] && ($documentos_campo_flag || $documentos_campo_flag == '1' )) {
        $where[] = " dop.mdoid " . (( $documentos_campo_excludente == null || $documentos_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $documentos) . "') ";
        $inner .= "INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid";
    }
    if ($planointerno[0] && ($planointerno_campo_flag || $planointerno_campo_flag == '1' )) {
        $where[] = " sd.sbdplanointerno " . (( $planointerno_campo_excludente == null || $planointerno_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $planointerno) . "') ";
    }
    if ($ptres[0] && ($ptres_campo_flag || $ptres_campo_flag == '1' )) {
        $where[] = " sd.sbdptres " . (( $ptres_campo_excludente == null || $ptres_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $ptres) . "') ";
    }
    if ($programa[0] && ($programa_campo_flag || $programa_campo_flag == '1' )) {
        $where[] = " s.prgid " . (( $programa_campo_excludente == null || $programa_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $programa) . "') ";
    }
    if ($statuspar[0] && ($statuspar_campo_flag || $statuspar_campo_flag == '1' )) {
        $where[] = " esd.esdid " . (( $statuspar_campo_excludente == null || $statuspar_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $statuspar) . "') ";
        $inner .= " INNER JOIN workflow.documento doc ON doc.docid = iu.docid 
					INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid ";
    }
    if ($statussubacao[0] && ($statussubacao_campo_flag || $statussubacao_campo_flag == '1' )) {
        $where[] = " esd2.esdid " . (( $statussubacao_campo_excludente == null || $statussubacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $statussubacao) . "') ";
        $inner .= " INNER JOIN workflow.documento doc2 ON doc2.docid = s.docid 
					INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid ";
    }
    if ($municipio[0] && ($municipio_campo_flag || $municipio_campo_flag == '1' )) {
        $where[] = " iu.muncod " . (( $municipio_campo_excludente == null || $municipio_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $municipio) . "') ";
    }
    if ($forma[0] && ($forma_campo_flag || $forma_campo_flag == '1' )) {
        $where[] = " s.frmid " . (( $forma_campo_excludente == null || $forma_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $forma) . "') ";
        $inner .= " INNER JOIN par.formaexecucao frm ON frm.frmid = s.frmid ";
    }
    if ($dimensao[0] && ($dimensao_campo_flag || $dimensao_campo_flag == '1' )) {
        $where[] = " dim.dimcod " . (( $dimensao_campo_excludente == null || $dimensao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $dimensao) . "') ";
        $inner .= " INNER JOIN par.criterio					c	ON c.crtid = p.crtid
					INNER JOIN par.indicador				i	ON i.indid = c.indid
					INNER JOIN par.area						are	ON are.areid = i.areid
					INNER JOIN par.dimensao					dim ON dim.dimid = are.dimid ";
    }
    if ($tiposubacao[0] && ($tiposubacao_campo_flag || $tiposubacao_campo_flag == '1' )) {
        $where[] = " s.ptsid " . (( $tiposubacao_campo_excludente == null || $tiposubacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $tiposubacao) . "') ";
    }
    if ($subacao[0] && ($subacao_campo_flag || $subacao_campo_flag == '1' )) {
        $where[] = " s.ppsid " . (( $subacao_campo_excludente == null || $subacao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $subacao) . "') ";
    }
    if ($itemcomposicao[0] && ($itemcomposicao_campo_flag || $itemcomposicao_campo_flag == '1' )) {
        $item = " AND sic.picid " . (( $itemcomposicao_campo_excludente == null || $itemcomposicao_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode("','", $itemcomposicao) . "') ";
        $inner .= " INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S' {$item} ";
    }
    if ($grupo[0] && ($grupo_campo_flag || $grupo_campo_flag == '1' )) {
        if ($grupo_campo_excludente == null || $grupo_campo_excludente == '0') {
            $inner .= " INNER JOIN territorios.muntipomunicipio mtm ON mtm.muncod = mun.muncod AND mtm.estuf = mun.estuf AND mtm.tpmid IN ('" . implode("','", $grupo) . "') ";
        } else {
            $where[] = " mun.muncod NOT IN (SELECT mtm.muncod FROM territorios.muntipomunicipio mtm WHERE mtm.tpmid IN ( '" . implode("','", $grupo) . "' ))";
        }
    }

    if (!empty($esfera)) {
        if ($esfera == 'E') {
            $where[] = "iu.itrid = 1";
        } else {
            $where[] = "iu.itrid = 2";
        }
    }

    if (!empty($termovalidado)) {
        if ($termovalidado == 'S') {
            $where[] = "dopusucpfvalidacaogestor IS NOT NULL";
        } else {
            $where[] = "dopusucpfvalidacaogestor IS NULL";
        }
        if (!in_array('documento', $arColunas) && (!$documentos[0] || !$documentos_campo_flag)) {
            $inner .= " INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid";
        }
    }
    if (!empty($empenhado)) {
        if ($empenhado == 'S') {
            $where[] = " s.sbaid IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' ) ";
        } else {
            $where[] = " s.sbaid NOT IN ( SELECT sbaid FROM par.empenhosubacao where eobstatus = 'A' ) ";
        }
    }
    if (!empty($empenhoparcial)) {
        if ($empenhoparcial == 'S') {
            $where[] = " s.sbaid IN ( SELECT sbaid FROM par.empenhosubacao WHERE eobpercentualemp <> 100 and eobstatus = 'A') ";
        } else {
            $where[] = " s.sbaid IN ( SELECT sbaid FROM par.empenhosubacao WHERE eobpercentualemp = 100 and eobstatus = 'A') ";
        }
    }

    if (in_array('documento', $arColunas)) {
        $select .= ",dop.dopid || '/' || TO_CHAR(dopdatainclusao, 'YYYY') as documento";
        $group[] = " dop.dopid ";
        $group[] = " dopdatainclusao ";
        if (!$documentos[0] || !$documentos_campo_flag) {
            $inner .= "INNER JOIN par.vm_documentopar_ativos dop ON dop.prpid = prp.prpid";
        }
    }
    if (in_array('codigo_subacao', $arColunas)) {
        $select .= ",dim.dimcod || '.' || arecod || '.' || indcod || '.' || s.sbaordem as codigo_subacao";
        $group[] = " dim.dimcod ";
        $group[] = " arecod ";
        $group[] = " indcod ";
        $group[] = " s.sbaordem ";
        if ((!$dimensao[0] || !$dimensao_campo_flag)) {
            $inner .= " INNER JOIN par.criterio					c	ON c.crtid = p.crtid
						INNER JOIN par.indicador				i	ON i.indid = c.indid
						INNER JOIN par.area						are	ON are.areid = i.areid
						INNER JOIN par.dimensao					dim ON dim.dimid = are.dimid ";
        }
    }
    if (in_array('valor_subacao', $arColunas)) {
        $select .= ",(SELECT par.recuperavalorvalidadossubacaoporano(sd.sbaid, sd.sbdano)) as valor_subacao";
        $group[] = " sd.sbaid ";
        $group[] = " sd.sbdano ";
    }
    if (in_array('valor_subacao', $arColunas) || in_array('codigo_subacao', $arColunas) || in_array('subacao', $arColunas) || in_array('statussubacao', $arColunas) || in_array('tiposubacao', $arColunas)) {
        $select .= ",sd.sbdid";
        $group[] = " sd.sbdid ";
    }
    if (in_array('sbdplanointerno', $arColunas)) {
        $select .= ",sd.sbdplanointerno";
        $group[] = " sd.sbdplanointerno ";
    }
    if (in_array('sbdptres', $arColunas)) {
        $select .= ",sd.sbdptres";
        $group[] = " sd.sbdptres ";
    }
    if (in_array('prgdsc', $arColunas)) {
        $select .= ",prg.prgdsc";
        $group[] = " prg.prgdsc ";
        $inner .= " INNER JOIN par.programa prg ON prg.prgid = s.prgid AND prg.prgstatus = 'A' ";
    }
    if (in_array('statussubacao', $arColunas)) {
        $select .= ",esd2.esddsc as statussubacao";
        $group[] = " esd2.esddsc ";
        if ((!$statussubacao[0] || !$statussubacao_campo_flag)) {
            $inner .= " INNER JOIN workflow.documento doc2 ON doc2.docid = s.docid 
						INNER JOIN workflow.estadodocumento esd2 ON esd2.esdid = doc2.esdid ";
        }
    }
    if (in_array('forma', $arColunas)) {
        $select .= ",frm.frmdsc as forma";
        $group[] = " frm.frmdsc ";
        if ((!$forma[0] || !$forma_campo_flag)) {
            $inner .= " INNER JOIN par.formaexecucao frm ON frm.frmid = s.frmid ";
        }
    }
    if (in_array('esddsc', $arColunas)) {
        $select .= ",esd.esddsc";
        $group[] = " esd.esddsc ";
        if ((!$statuspar[0] || !$statuspar_campo_flag)) {
            $inner .= " INNER JOIN workflow.documento doc ON doc.docid = iu.docid 
						INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid ";
        }
    }

    if (in_array('tiposubacao', $arColunas)) {

        $select .= ",tps.tpsdescricao as tiposubacao";
        $group[] = " tps.tpsdescricao ";
        $inner .= " INNER JOIN par.propostasubacao pps ON pps.ppsid = s.ppsid
					INNER JOIN par.propostatiposubacao pts ON pts.ptsid = pps.ptsid 
					INNER JOIN  par.tiposubacao tps on tps.tpsid = pts.tpsid
					";
    }
    if (in_array('subacao', $arColunas)) {
        $select .= ",s.sbadsc as subacao";
        $group[] = " s.sbadsc ";
    }
    if (in_array('dimensao', $arColunas)) {
        $select .= ",dim.dimcod as dimensao";
        $group[] = " dim.dimcod ";
        if ((!$dimensao[0] || !$dimensao_campo_flag)) {
            if (!in_array('codigo_subacao', $arColunas)) {
                $inner .= " INNER JOIN par.criterio					c	ON c.crtid = p.crtid
							INNER JOIN par.indicador				i	ON i.indid = c.indid
							INNER JOIN par.area						are	ON are.areid = i.areid
							INNER JOIN par.dimensao					dim ON dim.dimid = are.dimid ";
            }
        }
    }
    $groupby = "";
    if (in_array('qtditemcomposicao', $arColunas)) {
        $select .= " ,CASE WHEN s.sbacronograma = 1 THEN 
							sum(sic.icoquantidadetecnico)
						ELSE
							sum(ssi.seiqtdtecnico)
						END as qtditemcomposicao ";
        $group[] = " s.sbacronograma ";
        $group[] = " iu.itrid ";
        $group[] = " iu.estuf ";
        $group[] = " iu.mun_estuf ";
        $group[] = " mun.mundescricao ";
        $group[] = " prp.prpnumeroprocesso ";
        $groupby = ' GROUP BY ' . implode(", ", $group);
        if ((!$itemcomposicao[0] || !$itemcomposicao_campo_flag)) {
            $inner .= " INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = s.sbaid AND sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S' ";
        }
        $inner .= " LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid ";
    }

    if ($stColunas) { // processo
        $nivel_resultado = $_SESSION['par']['post']['nivel_resultado'];
        
        if ($nivel_resultado == 'processo') {
            $sql = "SELECT DISTINCT
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as local,
                        prp.prpnumeroprocesso
                        {$select}
                    FROM
                        par.processopar prp
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                    INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                    INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
                    INNER JOIN par.instrumentounidade iu  ON iu.inuid = p.inuid
                    LEFT JOIN  territorios.municipio mun ON mun.muncod = iu.muncod
                    {$inner}
                    WHERE prp.prpstatus = 'A'
                    " . (($where) ? ' and ' . implode(" AND ", $where) : "") . "
                    {$groupby}
                    ORDER BY
                        uf, local";
        } else {
            $sql = "SELECT DISTINCT
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE iu.mun_estuf END as uf,
                        CASE WHEN iu.itrid = 1 THEN iu.estuf ELSE mun.mundescricao END as local,
                        prp.prpnumeroprocesso
                        {$select}
                    FROM par.empenhosubacao emps
                    INNER JOIN par.v_saldo_empenho_por_subacao vemp ON vemp.empid = emps.empid 
                    INNER JOIN par.processopar prp ON prp.prpnumeroprocesso = vemp.empnumeroprocesso and prp.prpstatus = 'A'
                    INNER JOIN par.processoparcomposicao ppc ON ppc.prpid = prp.prpid and ppc.ppcstatus = 'A'
                    INNER JOIN par.subacaodetalhe sd  ON sd.sbdid = ppc.sbdid
                    INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
                    INNER JOIN par.acao a ON a.aciid = s.aciid AND a.acistatus = 'A'
                    INNER JOIN par.pontuacao p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
                    INNER JOIN par.instrumentounidade iu  ON iu.inuid = p.inuid
                    LEFT JOIN  territorios.municipio mun ON mun.muncod = iu.muncod
                    {$inner}
                    WHERE prp.prpstatus = 'A'
                    " . (($where) ? ' and ' . implode(" AND ", $where) : "") . "
                    {$groupby}
                    ORDER BY
                        uf, local";
        }
        $arDados = $db->carregar($sql);
    } else {
        die("<script>alert('Colunas não selecionadas');</script>");
    }
    $arRegistro = array();
    $arExcel = array();
    if ($arDados) {
        $arDadosArray = array();
        foreach ($arDados as $key => $value) {
            if (in_array('empenho', $id)) {
                $html = '';
                if (in_array('valor_subacao', $arColunas) || in_array('codigo_subacao', $arColunas) || in_array('subacao', $arColunas) || in_array('statussubacao', $arColunas) || in_array('tiposubacao', $arColunas)) {
                    $sql = "SELECT
                                COALESCE(emp.empnumero,'-') AS empnumero, 
                                SUM(vemp.saldo) AS eobvalorempenho
                            FROM par.v_saldo_empenho_por_subacao vemp 
                            INNER JOIN par.empenho emp ON emp.empid = vemp.empid
                            INNER JOIN par.subacaodetalhe sd ON sd.sbaid = vemp.sbaid AND sd.sbdano = vemp.eobano
                            WHERE vemp.saldo > 0 AND sd.sbdid = " . $value['sbdid'] . "
                            GROUP BY emp.empnumero";
                } else {
                    $sql = "SELECT
                                COALESCE(emp.empnumero,'-') as empnumero, 
                                SUM(vemp.saldo)::double as eobvalorempenho
                            FROM par.v_saldo_empenho_por_subacao vemp 
                            INNER JOIN par.empenho emp ON emp.empid = vemp.empid
                            INNER JOIN par.processopar prp ON prp.prpnumeroprocesso = vemp.empnumeroprocesso
                            WHERE vemp.saldo > 0 AND prp.prpnumeroprocesso = '" . $value['prpnumeroprocesso'] . "'
                            GROUP BY emp.empnumero";
                }
                $arEmpenho = $db->carregar($sql);
                $arEmpenho = $arEmpenho ? $arEmpenho : array();
                $html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
                            <tr>
                                <th>Nº Empenho&nbsp;&nbsp;&nbsp;</th>
                                <th>Valor do Empenho&nbsp;&nbsp;&nbsp;</th>';
                $html .= '</tr>';
                if ($arEmpenho) {
                    foreach ($arEmpenho as $empenho) {
                        $html .= '<tr>
                                        <td nowrap>' . $empenho['empnumero'] . '&nbsp;&nbsp;&nbsp;</td>
                                        <td nowrap>R$ ' . number_format($empenho['eobvalorempenho'], 2, ',', '.') . '&nbsp;&nbsp;&nbsp;</td>';
                        $html .= '</tr>';
                    }
                    $html .= '</table>';

                    $htmlEmpenho = $html;
                    $empenhoExcel = $empenho['eobvalorempenho'];
                    $empenhoNumeroExcel = $empenho['empnumero'];
                } else {
                    $htmlEmpenho = '<center>Não</center>';
                    $empenhoExcel = 'Não';
                    $empenhoNumeroExcel = $empenho['empnumero'];
                }
            }

            if (in_array('pagamento', $id)) {
                $htmlP = '';
                if (in_array('valor_subacao', $arColunas) || in_array('codigo_subacao', $arColunas) || in_array('subacao', $arColunas) || in_array('statussubacao', $arColunas) || in_array('tiposubacao', $arColunas)) {
                    $sql = "SELECT 
                                    sum(pgs.pobvalorpagamento) as pobvalorpagamento
                            FROM 
                                    par.pagamentosubacao pgs
                            INNER JOIN par.pagamento pag ON pag.pagid = pgs.pagid AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                            INNER JOIN par.subacaodetalhe sd ON sd.sbaid = pgs.sbaid AND sd.sbdano = pgs.pobano
                            WHERE
                                    pag.pagstatus = 'A' and pobstatus = 'A' AND sd.sbdid = " . $value['sbdid'];
                } else {
                    $sql = "SELECT 
                                    sum(pgs.pobvalorpagamento) as pobvalorpagamento
                            FROM 
                                    par.pagamentosubacao pgs
                            INNER JOIN par.pagamento pag ON pag.pagid = pgs.pagid AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                            INNER JOIN par.empenho emp ON emp.empid = pag.empid and empstatus = 'A'
                            WHERE
                                    pag.pagstatus = 'A' and pobstatus = 'A' AND emp.empnumeroprocesso = '" . $value['prpnumeroprocesso'] . "'";
                }
                $arPagamento = $db->carregar($sql);
                $arPagamento = $arPagamento ? $arPagamento : array();
                $pg = 0;
                //verifico se algum tem reforço	
                $htmlP = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Valor do Pagamento</th>
							</tr>';
                if ($arPagamento) {
                    foreach ($arPagamento as $pagamento) {
                        if ($pagamento['pobvalorpagamento']) {
                            $htmlP .= '<tr>
                                        <td nowrap>R$ ' . number_format($pagamento['pobvalorpagamento'], 2, ',', '.') . '</td>
                                    </tr>';
                            $pg++;
                        }
                    }
                    $htmlP .= '</table>';
                }
                if ($pg == 0) {
                    $htmlPagamento = '<center>Não</center>';
                    $pagamentoExcel = 'Não';
                } else {
                    $htmlPagamento = $htmlP;
                    $pagamentoExcel = number_format($pagamento['pobvalorpagamento'], 2, ',', '.');
                }
            }

            if (in_array('itemcomposicao', $id)) {
                $htmlP = '';
                if (in_array('valor_subacao', $arColunas) || in_array('codigo_subacao', $arColunas) || in_array('subacao', $arColunas) || in_array('statussubacao', $arColunas) || in_array('tiposubacao', $arColunas)) {
                    $sql = "SELECT
                                    picdescricao, quantidadeaprov, quantidadeplan
                            FROM (
                            SELECT 
                                    pic.picdescricao,
                                    CASE WHEN s.sbacronograma = 1 THEN 
                                            sum(sic.icoquantidade)
                                    ELSE
                                            sum(ssi.seiqtd)
                                    END as quantidadeplan,
                                    CASE WHEN s.sbacronograma = 1 THEN 
                                            sum(sic.icoquantidadetecnico)
                                    ELSE
                                            sum(ssi.seiqtdtecnico)
                                    END as quantidadeaprov
                            FROM 
                                    par.subacao s
                            INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
                            INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S' {$item}
                            INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND pic.picstatus = 'A' 
                            LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid
                            WHERE
                                    sd.sbdid = " . $value['sbdid'] . "
                            GROUP BY
                                    pic.picdescricao, s.sbacronograma) as foo
                            WHERE
                                    foo.quantidadeaprov <> 0";
                } else {
                    $sql = "SELECT
                                    picdescricao, quantidadeaprov, quantidadeplan
                            FROM (
                            SELECT 
                                    pic.picdescricao,
                                    CASE WHEN s.sbacronograma = 1 THEN 
                                            sum(sic.icoquantidade)
                                    ELSE
                                            sum(ssi.seiqtd)
                                    END as quantidadeplan,
                                    CASE WHEN s.sbacronograma = 1 THEN 
                                            sum(sic.icoquantidadetecnico)
                                    ELSE
                                            sum(ssi.seiqtdtecnico)
                                    END as quantidadeaprov
                            FROM 
                                    par.subacao s
                            INNER JOIN par.subacaodetalhe sd ON s.sbaid = sd.sbaid
                            INNER JOIN par.subacaoitenscomposicao sic ON sic.sbaid = sd.sbaid AND sic.icoano = sd.sbdano AND sic.icovalidatecnico = 'S' {$item}
                            INNER JOIN par.propostaitemcomposicao pic ON pic.picid = sic.picid AND pic.picstatus = 'A' 
                            LEFT JOIN par.subescolas_subitenscomposicao ssi ON ssi.icoid = sic.icoid
                            WHERE
                                    sd.sbdid IN ( SELECT sbdid FROM par.subacaodetalhe sd INNER JOIN par.empenhosubacao ems ON ems.sbaid = sd.sbaid AND ems.eobano = sd.sbdano AND ems.eobstatus = 'A'
                                                                    INNER JOIN par.empenho emp ON emp.empid = ems.empid  AND emp.empnumeroprocesso = '" . $value['prpnumeroprocesso'] . "' and empstatus = 'A')
                            GROUP BY
                                    pic.picdescricao, s.sbacronograma) as foo
                            WHERE
                                    foo.quantidadeaprov <> 0";
                }
                $arItemComposicao = $db->carregar($sql);
                $arItemComposicao = $arItemComposicao ? $arItemComposicao : array();
                $pg = 0;
                //verifico se algum tem reforço	
                $htmlI = '<table id="table" width="100%" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
                            <tr>
                                <th>Item de Composição</th>
                                <th>Quantidade Planejada</th>
                                <th>Quantidade Aprovada</th>
                            </tr>';
                if ($arItemComposicao) {
                    foreach ($arItemComposicao as $itemcomposicao) {
                        $htmlI .= '<tr>
                                            <td nowrap>' . $itemcomposicao['picdescricao'] . '</td>
                                            <td nowrap align="right">' . $itemcomposicao['quantidadeplan'] . '</td>
                                            <td nowrap align="right">' . $itemcomposicao['quantidadeaprov'] . '</td>
                                    </tr>';
                        $pg++;
                    }
                    $htmlI .= '</table>';
                }
                if ($pg == 0) {
                    $htmlItem = '<center>Não</center>';
                    $itemExcel = 'Não';
                } else {
                    $htmlItem = $htmlI;
                    $itemExcel = 'Sim';
                }
            }

            $xx += isset($xx) ? 1 : 0;
            $boRegistro = false;

            foreach ($id as $key => $c) {
                if ($c == 'valor_subacao') {
                    $arRegistro[$xx][$c] = "R$ " . number_format($value[$c], 2, ',', '.');
                    $arExcel[$xx][$c] = $value[$c];
                } elseif ($c == 'prpnumeroprocesso') {
                    $arRegistro[$xx][$c] = formataProcesso($value[$c]);
                    $arExcel[$xx][$c] = formataProcesso($value[$c]);
                } else {
                    $arRegistro[$xx][$c] = $value[$c];
                    $arExcel[$xx][$c] = $value[$c];
                }
            }
            if (in_array('empenho', $id) && !empty($htmlEmpenho)) {
                $boRegistro = true;
                $arRegistro[$xx]['empenho'] = $htmlEmpenho;
                $arExcel[$xx]['empenho'] = $empenhoExcel;
                $arExcel[$xx]['numero_empenho'] = $empenhoNumeroExcel;
                $htmlEmpenho = '';
            }
            if (in_array('pagamento', $id) && !empty($htmlPagamento)) {
                $boRegistro = true;
                $arRegistro[$xx]['pagamento'] = $htmlPagamento;
                $arExcel[$xx]['pagamento'] = $pagamentoExcel;
                $htmlPagamento = '';
            }
            if (in_array('itemcomposicao', $id) && !empty($htmlItem)) {
                $boRegistro = true;
                $arRegistro[$xx]['itemcomposicao'] = $htmlItem;
                $arExcel[$xx]['itemcomposicao'] = $itemExcel;
                $htmlItem = '';
            }
        }// fim do foreach
    }
    return array("sql" => $arRegistro,
        "excel" => $arExcel);
}

function formataProcesso($numero) {
    if (!empty($numero))
        $processo = substr($numero, 0, 5) . "." . substr($numero, 5, 6) . "/" . substr($numero, 11, 4) . "-" . substr($numero, 15, 2);
    return $processo;
}

function monta_coluna() {

    $cols = $_SESSION['par']['post']['coluna'] ? $_SESSION['par']['post']['coluna'] : array();

    $coluna = array("coluna" => array());

    if (is_array($cols)) {
        for ($i = 0; $i < sizeof($cols); $i++) {

//		}	
//		foreach ($cols as $val) {
            switch ($cols[$i]) {
                case 'uf':
                    array_push($coluna['coluna'], array(
                        "campo" => "uf",
                        "id" => "uf",
                        "label" => "UF")
                    );
                    break;
                case 'local':
                    array_push($coluna['coluna'], array(
                        "campo" => "local",
                        "id" => "local",
                        "label" => "Local")
                    );
                    break;
                case 'prpnumeroprocesso':
                    array_push($coluna['coluna'], array(
                        "campo" => "prpnumeroprocesso",
                        "id" => "prpnumeroprocesso",
                        "label" => "Número do Processo")
                    );
                    break;
                case 'valor_subacao':
                    array_push($coluna['coluna'], array(
                        "campo" => "valor_subacao",
                        "id" => "valor_subacao",
                        "label" => "Valor da Subação")
                    );
                    break;
                case 'empenho':
                    if ($_POST['exporta'] == "true") {
                         array_push($coluna['coluna'], array(
                            "campo" => "",
                            "id" => "empenho",
                            "label" => "Valor do Empenho")
                        );
                        array_push($coluna['coluna'], array(
                            "campo" => "",
                            "id" => "numero_empenho",
                            "label" => "N° Empenho")
                        );
                        
                    }else {
                         array_push($coluna['coluna'], array(
                            "campo" => "",
                            "id" => "empenho",
                            "label" => "Empenho")
                        );
                    }
                    break;
                case 'pagamento':
                    array_push($coluna['coluna'], array(
                        "campo" => "",
                        "id" => "pagamento",
                        "label" => "Pagamento")
                    );
                    break;
                case 'documento':
                    array_push($coluna['coluna'], array(
                        "campo" => "documento",
                        "id" => "documento",
                        "label" => "Documento")
                    );
                    break;
                case 'sbdplanointerno':
                    array_push($coluna['coluna'], array(
                        "campo" => "sbdplanointerno",
                        "id" => "sbdplanointerno",
                        "label" => "Plano Interno")
                    );
                    break;
                case 'sbdptres':
                    array_push($coluna['coluna'], array(
                        "campo" => "sbdptres",
                        "id" => "sbdptres",
                        "label" => "PTRES")
                    );
                    break;
                case 'programa':
                    array_push($coluna['coluna'], array(
                        "campo" => "prgdsc",
                        "id" => "prgdsc",
                        "label" => "Programa")
                    );
                    break;
                case 'statuspar':
                    array_push($coluna['coluna'], array(
                        "campo" => "esddsc",
                        "id" => "esddsc",
                        "label" => "Status do PAR")
                    );
                    break;
                case 'statussubacao':
                    array_push($coluna['coluna'], array(
                        "campo" => "statussubacao",
                        "id" => "statussubacao",
                        "label" => "Status da Subação")
                    );
                    break;
                case 'forma':
                    array_push($coluna['coluna'], array(
                        "campo" => "forma",
                        "id" => "forma",
                        "label" => "Forma de Execução")
                    );
                    break;
                case 'dimensao':
                    array_push($coluna['coluna'], array(
                        "campo" => "dimensao",
                        "id" => "dimensao",
                        "label" => "Dimensão")
                    );
                    break;
                case 'tiposubacao':
                    array_push($coluna['coluna'], array(
                        "campo" => "tiposubacao",
                        "id" => "tiposubacao",
                        "label" => "Tipo da Subação")
                    );
                    break;
                case 'subacao':
                    array_push($coluna['coluna'], array(
                        "campo" => "subacao",
                        "id" => "subacao",
                        "label" => "Subação")
                    );
                    break;
                case 'itemcomposicao':
                    array_push($coluna['coluna'], array(
                        "campo" => "itemcomposicao",
                        "id" => "itemcomposicao",
                        "label" => "Item de Composição")
                    );
                    break;
                case 'qtditemcomposicao':
                    array_push($coluna['coluna'], array(
                        "campo" => "qtditemcomposicao",
                        "id" => "qtditemcomposicao",
                        "label" => "Quantidade Aprovada de Item de Composição")
                    );
                    break;
                case 'codigo_subacao':
                    array_push($coluna['coluna'], array(
                        "campo" => "codigo_subacao",
                        "id" => "codigo_subacao",
                        "label" => "Subação")
                    );
                    break;
            }
        }
    }
    return $coluna['coluna'];
}

?>