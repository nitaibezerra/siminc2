<?php
//
// $Id$
//

cte_verificaSessao();

if($_REQUEST['pai_sbaid'] && !$_REQUEST['sbaid']) {
	$sql_pegarPaiAditivo = "SELECT * FROM cte.subacaoindicador WHERE sbaid = '".$_REQUEST['pai_sbaid']."'";
	$dadosPaiAditivo = (array) $db->carregar($sql_pegarPaiAditivo);
	if($dadosPaiAditivo) {
		$dadosPaiAditivo = current($dadosPaiAditivo);
	}
    $sql = '
        INSERT INTO cte.subacaoindicador (
            aciid,
            undid,
            frmid,
            sbadsc,
            sbastgmpl,
            sbaprm,
            sbapcr,
            sbadata,
            usucpf,
            foaid,
            prgid,
            sbaporescola,
            ppsid,
            sbacategoria
        ) VALUES (
            %d,
            %d,
            %s,
            \'%s\',
            \'%s\',
            %d,
            \'%s\',
            CURRENT_TIMESTAMP,
            \'%s\',
            %s,
            %d,
            \'%s\',
            %s,
            %d
        ) returning sbaid';

        $sql = sprintf($sql, (integer) $dadosPaiAditivo['aciid'		  ],
                             (integer) $dadosPaiAditivo['undid'		  ] == 0 ? 'null' : (integer) $dadosPaiAditivo['undid'],
                             (integer) $dadosPaiAditivo['frmid'       ] == 0 ? 'null' : (integer) $dadosPaiAditivo['frmid'],
                             (string)  $dadosPaiAditivo['sbadsc'      ],
                             (string)  $dadosPaiAditivo['sbastgmpl'   ],
                             (integer) $dadosPaiAditivo['sbaprm'      ],
                             (string)  $dadosPaiAditivo['sbapcr'      ],
                             (string)  $dadosPaiAditivo['usucpf'      ],
                             (integer) $dadosPaiAditivo['foaid'       ] == 0 ? 'null' : (integer) $dadosPaiAditivo['foaid'],
                             (integer) $dadosPaiAditivo['prgid'       ] == 0 ? 'null' : (integer) $dadosPaiAditivo['prgid'],
                             (string)  $dadosPaiAditivo['sbaporescola'],
                             (integer) $dadosPaiAditivo['ppsid'       ] == 0 ? 'null' : (integer) $dadosPaiAditivo['ppsid'],
                             (integer) $dadosPaiAditivo['sbacategoria']);
        $_REQUEST['sbaid'] = $db->pegaUm($sql);
        
        $sql = "INSERT INTO cte.subacaoindicadoraditivo(
         		sbaid, sbaidpai)
    	 		VALUES ('". $_REQUEST['sbaid'] ."', '". $_REQUEST['pai_sbaid'] ."');";
        $db->executar($sql);
    	$db->commit();
    	header("location: ?modulo=principal/aditivo_parsubacao&acao=A&pai_sbaid=" . $_REQUEST['pai_sbaid'] . "&sbaid=" . $_REQUEST['sbaid']);
    	exit;
        
} elseif($_REQUEST['sbaid'] && !$_REQUEST['pai_sbaid']) {
	$sql = "SELECT sbaidpai FROM cte.subacaoindicadoraditivo WHERE sbaid = '".$_REQUEST['sbaid']."'";
    $_REQUEST['pai_sbaid'] = $db->pegaUm($sql);
}elseif(!$_REQUEST['sbaid'] && !$_REQUEST['pai_sbaid']) {
	die("Falha nos parâmentros. Entre em contato com a equipe tecnica.");
}



define('CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');
define('CTE_PRM_EDITAR_SUBACAO', cte_podeEditarParecer($_SESSION['inuid']));
define('CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);



header('content-type: text/html; charset=iso-8859-1;');
//!@------------------------------------------------------------- SALVAR DADOS
if ($_REQUEST['formularioSumit'] == '1' && CTE_PRM_EDITAR_SUBACAO) {
    //!@------------------------------------------------------------- EXCLUSAO
    if ($_REQUEST['vlrProximaSubacao'] == '-1' && !CTE_NOVA_SUBACAO
                                               &&  CTE_PRM_EXCLUIR_SUBACAO) {
                                               		# Pegar parecer vinculado
		$sql = '
        SELECT
            COUNT(sbaid)
        FROM
            cte.subacaoparecertecnico spt
        WHERE
            sbaid = ' . $_REQUEST['sbaid'] . ' AND
            (
                ssuid IS NOT NULL or
                trim(sptparecer) <> \'\' or
                trim(sptuntdsc)  <> \'\'
            )';

		$parec = $db->pegaUm($sql);

		$sql = '
        SELECT
            COUNT(sac.sbaid)
        FROM
            cte.subacaoconvenio sac
        INNER JOIN
            cte.subacaoindicador sba ON sba.sbaid = sac.sbaid
        WHERE
            sac.sbaid = ' . $_REQUEST['sbaid'];

		$conv  = $db->pegaUm($sql);

		if ($parec >= 1 || $conv >= 1) {
	        $db->executar('
	        			   DELETE FROM
	                        cte.subacaoparecertecnico
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
		        
	        			   DELETE FROM
	                        cte.subacaobeneficiario
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.composicaosubacao
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.qtdfisicoano
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.termosubacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	
	                       DELETE FROM
	                        cte.subacaoindicadoraditivo
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid'] .';
	                        
	                        DELETE FROM
	                        cte.subacaoindicador
	                       WHERE
	                        sbaid = ' . (integer) $_REQUEST['sbaid']);
	
	        $db->commit();
	
	        echo '<script type="text/javascript">'
	            .'window.opener.location.reload();'
	            .'alert("Aditivo da subação foi excluída com sucesso.");'
	            .'window.opener.focus();'
	            .'window.close();'
	            .'</script>';
	
	        //                                                                  */
	        die();
		}else{
			die('<script>
					alert(\'Operação não realizada!\nFoi dado o parecer e/ou foi gerado convênio.\');
					location.href = \'?modulo=principal/aditivo_parsubacao&acao=A&sbaid='.$_REQUEST['sbaid'].'\'
				 </script>');
		}
    }
    //!@--------------------------------------------------------------- INSERT
    $anoExercicio = $_REQUEST['anoExercicio'];
    
    if (CTE_NOVA_SUBACAO) {
        $sql = '
        INSERT INTO cte.subacaoindicador (
            aciid,
            undid,
            frmid,
            sbadsc,
            sbastgmpl,
            sbaprm,
            sbapcr,
            sbadata,
            usucpf,
            foaid,
            prgid,
            sbaporescola,
            ppsid,
            sbacategoria
        ) VALUES (
            %d,
            %d,
            %s,
            \'%s\',
            \'%s\',
            %d,
            \'%s\',
            CURRENT_TIMESTAMP,
            \'%s\',
            %s,
            %d,
            %s,
            %s,
            %d
        ) returning sbaid';

        $sql = sprintf($sql, (integer) $_REQUEST['aciid'       ],
                             (integer) $_REQUEST['undid'       ] == 0 ? 'null' : (integer) $_REQUEST['undid'],
                             (integer) $_REQUEST['frmid'       ] == 0 ? 'null' : (integer) $_REQUEST['frmid'],
                             (string)  $_REQUEST['sbadsc'      ],
                             (string)  $_REQUEST['sbastgmpl'   ],
                             (integer) $_REQUEST['sbaprm'      ],
                             (string)  $_REQUEST['sbapcr'      ],
                             (string)  $_SESSION['usucpf'      ],
                             (integer) $_REQUEST['foaid'       ] == 0 ? 'null' : (integer) $_REQUEST['foaid'],
                             (integer) $_REQUEST['prgid'       ] == 0 ? 'null' : (integer) $_REQUEST['prgid'],
                             (string)  $_REQUEST['sbaporescola'],
                             (integer) $_REQUEST['ppsid'       ] == 0 ? 'null' : (integer) $_REQUEST['ppsid'],
                             (integer) $_REQUEST['sbacategoria']);

        $_REQUEST['sbaid'] = $db->pegaUm($sql);
        $db->commit();

        header('Location: ' . $_SERVER['REQUEST_URI'] . '&sbaid=' . $_REQUEST['sbaid']);
    } else {
        $sql = '
        SELECT
            count(sptid)
        FROM
            cte.subacaoparecertecnico
        WHERE
            sbaid  = ' . $_REQUEST['sbaid'] . '
            AND
            sptano = ' . $anoExercicio;

        if (($sbaid = $db->pegaUm($sql)) <= 0) {
            $sql = '
            INSERT INTO cte.subacaoparecertecnico (
                sbaid,
                sptparecer,
                sptunt,
                sptuntdsc,
                sptano,
                sptinicio,
                sptfim,
                tppid,
                ssuid
            ) VALUES (%d, \'%s\', %d, \'%s\', %d, %d, %d, 4, %s)';
        } else {
            $sql = '
            UPDATE cte.subacaoparecertecnico SET
                sbaid       = %d,
                sptparecer  = \'%s\',
                sptunt      = %d,
                sptuntdsc   = \'%s\',
                sptano      = %d,
                sptinicio   = %d,
                sptfim      = %d,
                tppid       = 4,
                ssuid       = %s
            WHERE
                sbaid       = %d
                AND
                sptano      = %d';
        }

        $sql = sprintf($sql, (integer) $_REQUEST['sbaid'                       ],
                             (string)  $_REQUEST['sptparecer_'  . $anoExercicio],
                             (integer) $_REQUEST['sptunt_'      . $anoExercicio],
                             (string)  $_REQUEST['sptuntdsc_'   . $anoExercicio],
                             (integer) $_REQUEST['anoExercicio'                ],
                             (integer) $_REQUEST['sptinicio_'   . $anoExercicio],
                             (integer) $_REQUEST['sptfim_'      . $anoExercicio],
                             (integer) $_REQUEST['ssuid_'       . $anoExercicio] == 0 ? 'null' : (integer) $_REQUEST['ssuid_'       . $anoExercicio],
                             (integer) $_REQUEST['sbaid'                       ],
                             (integer) $anoExercicio);

        $db->executar($sql);

        if (is_array($_REQUEST['cosqtd'])) {
            $sql = '
            UPDATE cte.composicaosubacao SET
                cosqtd    = %f,
                cosvlruni = %f
            WHERE
                cosid     = %d';

            foreach ($_REQUEST['cosqtd'] as $cosid => $cosqtd) {
                if (strpos($_REQUEST['cosvlruni'][$cosid], ',') !== false) {
                    $cosvlruni = str_replace(',', '.', str_replace('.', '', $_REQUEST['cosvlruni'][$cosid]));
                } else {
                    $cosvlruni = $_REQUEST['cosvlruni'][$cosid];
                }

                if (strpos($cosqtd, ',' === false)) {
                    $cosqtd = str_replace(',', '.', str_replace('.', '', $cosqtd));
                }

                $cosqtd    = number_format($cosqtd,    2, '.', '');
                $cosvlruni = number_format($cosvlruni, 2, '.', '');

                $db->executar(sprintf($sql, $cosqtd, $cosvlruni, $cosid));
            }
        }

        $db->commit();
        header('Location: ' . $_SERVER['REQUEST_URI'].'&parsubacaoanocrt='.$_REQUEST['anoExercicio'] . '&sbaid=' . $_REQUEST['sbaid']);
    }
}

//!@------------------------------------------------------------- FUNCOES AJAX
//---------------------- LISTAGEM, EXCULSAO DE ITENS-INDICADOR E TOTALIZADORES
if (array_key_exists('req', $_REQUEST)) {
    ob_clean();

    if (trim($_REQUEST['sbaid']) == '') {
        exit;
    }
	if($_REQUEST['req'] == 'carregarDetalhesSubacao') {

		
if(!$_REQUEST['paramano'])
	$_REQUEST['paramano'] = date('Y');

if ($_REQUEST['sbaporescola'] == 't') {
echo '<h3>Escolas</h3>';

//CARREGAR ESCOLAS
$sql = '
    SELECT
        e.entcodent,
        e.entnome,
        sum(coalesce(qfaqtd,0))
    FROM
        cte.qtdfisicoano q
    INNER JOIN
        entidade.entidade e on q.entid = e.entid
    WHERE
        q.qfaano = ' . $_REQUEST['paramano']    . '
        AND
        q.sbaid  = ' . $_REQUEST['pai_sbaid']  . '
    GROUP BY
        q.qfaid,
        e.entcodent,
        e.entnome
    ORDER BY
        entnome';
$db->monta_lista_simples($sql, array('Código INEP',
                                     'Escola',
                                     'Quantidade'), 1000, 1000, 'N', '100%','N');
}


echo '<h3>Itens que compoem a subação</h3>';
// CARREGAR ITENS COMPOSICAO
if ($_REQUEST['sbaporescola'] == 't') {
    $sql = '
    SELECT
         cos.cosdsc,
         und.undddsc,
         trim(to_char(coalesce(cos.cosqtd, 0), \'999990D9\')) as cosqtd,
         trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) as cosvlruni,
         trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) as vlrtotal
    FROM
        cte.composicaosubacao cos
    INNER JOIN
        cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
    WHERE
        cos.sbaid  = ' . $_REQUEST['pai_sbaid']  . '
        AND
        cos.cosano = ' . $_REQUEST['paramano'] . '
    ORDER BY
        cos.cosdsc';

} else {
    $sql = '
    SELECT
        cos.cosdsc
        ,und.undddsc
        ,trim(to_char(coalesce(cos.cosqtd, 0), \'999990D9\')) as cosqtd
        ,trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) as cosvlruni
        ,trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) as vlrtotal
     FROM
        cte.composicaosubacao cos
    INNER JOIN
        cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
    WHERE
        cos.sbaid  = ' . $_REQUEST['pai_sbaid']  . '
        AND
        cos.cosano = ' . $_REQUEST['paramano'] . '
    GROUP BY
        cos.cosdsc
        ,und.undddsc
        , cosqtd
        ,cosvlruni
        ,cos.cosid
    ORDER BY
        cos.cosdsc';
}

$db->monta_lista_simples($sql, array('Identificação do Item',
                                     'Un. de Medida',
                                     'Qtd',
                                     'Valor Unitário',
                                     'Total'), 1000, 1000, 'N', '100%','N');

echo '<h3>Beneficiarios da subacao</h3>';
$sql = '
SELECT
    be.bendsc,
    sb.vlrurbano as vlrurbano,
    sb.vlrrural  as vlrrural,
    sb.vlrurbano + sb.vlrrural as sabvlrtotal
FROM
    cte.subacaobeneficiario sb
INNER JOIN
    cte.beneficiario be ON sb.benid = be.benid
WHERE
    sb.sbaid  = ' . $_REQUEST['pai_sbaid']   . '
    AND
    sb.sabano = ' . $_REQUEST['paramano'];

$db->monta_lista_simples($sql, array('Beneficiário',
                                     'Zona Urbana',
                                     'Zona Rural',
                                     'Total'), 1000, 1000, 'S', '100%','N');



echo '<h3>Parecer técnico</h3>';
$sql = '
SELECT
    coalesce(spt.sptparecer, \'\')  as sptparecer,
    coalesce(spt.sptunt, 0)         as sptunt,
    coalesce(spt.sptuntdsc, \'\')   as sptuntdsc,
    coalesce(spt.sptano, 0)         as sptano,
    coalesce(spt.sptinicio, 0)      as sptinicio,
    coalesce(spt.sptfim, 0)         as sptfim,
    coalesce(spt.ssuid, 0)          as ssuid
FROM
    cte.subacaoparecertecnico spt
INNER JOIN
    cte.subacaoindicador sba on spt.sbaid = sba.sbaid
WHERE
    spt.sbaid  = ' . $_REQUEST['pai_sbaid'] . '
    AND
    spt.sptano = ' . $_REQUEST['paramano'];

$res = $db->pegaLinha($sql);

$meses = array(0 => 'Não selecionado',
			   1 => 'Janeiro',
			   2 => 'Fevereiro',
			   3 => 'Março',
			   4 => 'Abril',
			   5 => 'Maio',
			   6 => 'Junho',
			   7 => 'Julho',
			   8 => 'Agosto',
			   9 => 'Setembro',
			   10 => 'Outubro',
			   11 => 'Novembro',
			   12 => 'Dezembro');

if($res) {
	if ($_REQUEST['sbaporescola'] != 't') {
		$qtdhtml = "<tr bgcolor='' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='';\">
                 	<td>Quantidade:</td>
                 	<td>
                 	". $res['sptunt'] ."
                 	</td>
            		</tr>";
	}
	
     echo "<table width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\">
     		". $qtdhtml ."
     		<tr bgcolor='#F7F7F7' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#F7F7F7';\">
                 <td>Cronograma Físico:</td>
                 <td>
                 ". $meses[$res['sptinicio']] ." a ". $meses[$res['sptfim']] ."
                 </td>
            </tr>
     		<tr bgcolor='' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='';\">
                 <td>Parecer:</td>
                 <td>
                 ". $res['sptparecer'] ."
                 </td>
            </tr>
     		<tr bgcolor='#F7F7F7' onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#F7F7F7';\">
                 <td>Detalhamento/Comentário:</td>
                 <td>
                 ". $res['sptuntdsc'] ."
                 </td>
            </tr>
           </table>";
} else {
   echo "<table width=\"100%\" align=\"center\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\" style=\"color:333333;\" class=\"listagem\">
          <tbody>
          <tr><td align=\"center\" style=\"color:#cc0000;\">Não foram encontrados Registros.</td></tr>
          </tbody>
         </table>";
}
		
		
		
		
		
		
		
		
		
	}elseif ($_REQUEST['req'] == 'prgplanointerno') {
        echo $db->pegaUm('SELECT
                              prgplanointerno
                          FROM
                              cte.programa
                          WHERE
                              prgid = ' . (integer) $_REQUEST['prgplanointerno']);
    } elseif ($_REQUEST['req'] == 'verificarInsercaoDadosSubacao') {
        if ($_REQUEST['sbaporescola'] == 't') {
            $sql = '
            SELECT
                count(sba.sbaid)
            FROM
                cte.subacaoindicador sba
            INNER JOIN
                cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano    > 0 OR vlrrural > 0)
            INNER JOIN
                cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd       > 0)
            INNER JOIN
                cte.qtdfisicoano qfa ON sba.sbaid = qfa.sbaid        AND (qfaqtd       > 0)
            WHERE
                sba.sbaid = ' . $_REQUEST['sbaid'];
        } else {
            $sql = '
            SELECT
                count(sba.sbaid)
            FROM
                cte.subacaoindicador sba
            INNER JOIN
                cte.subacaobeneficiario ben ON sba.sbaid = ben.sbaid AND (vlrurbano    > 0 OR vlrrural > 0)
            INNER JOIN
                cte.composicaosubacao cos ON sba.sbaid = cos.sbaid   AND (cosqtd       > 0)
            WHERE
                sba.sbaid = ' . $_REQUEST['sbaid'];
        }

        echo '<script>_totalizar(' , $db->pegaUm($sql) , ');</script>';

    } elseif ($_REQUEST['req'] == 'carregarItensComposicao') {
        $sql = '
        SELECT
            count(sba.undid)
        FROM
            cte.subacaoindicador sba
        INNER JOIN
            cte.unidademedida und ON sba.undid  = und.undid
        INNER JOIN
            cte.laboratorio lab ON und.undid = lab.undid
        WHERE
            sbaid = ' . $_REQUEST['sbaid'];

        if ($db->pegaUm($sql) == 1) {
            $readonly = ' readonly="readonly"';
            $remover  = "alert(\\'Não é possível excluir itens de composição de laboratórios.\\');return false;";
        } else {
            $readonly = '';
            $remover  = 'removerItemComposicao';
        }

        if ($_REQUEST['sbaporescola'] == 't')
        {
            if ($readonly == '') {
                $sql = '
                SELECT
                    \'<center><img onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
                    ,cos.cosdsc
                    ,und.undddsc

                    ,\'<input size="10"
                            id="cosqtd\' || cos.cosid || \'"
                            type="text"
                            class="CampoEstilo"
                            onmouseover="MouseOver(this);"
                            onfocus="MouseClick(this);"
                            onmouseout="MouseOut(this);"
                            onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                            onkeyup="this.value=mascaraglobal(\'\'###.###,#\'\', this.value);"
                            onfocus="this.select()"
                            onclick="return popupSelecionarEscolasItemComposicao(this,\' || cos.cosid || \')"
                            name="cosqtd[\'|| cos.cosid ||\']"
                            readonly="readonly"
                            value="\' || trim(to_char(coalesce(ecs.total, 0), \'999990D9\')) || \'" />\' as cosqtd

                    ,\'<input size="20"
                            id="cosvlruni\' || cos.cosid || \'"
                            type="text"
                            class="itemComposicao"
                            style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
                            onmouseover="MouseOver(this);"
                            onmouseout="MouseOut(this);"
                            onblur="MouseBlur(this);if(this.value==\'\'\'\')this.value=this.defaultValue;verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                            onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
                            onfocus="if(this.value==this.defaultValue)this.value=\'\'\'\';"
                            onclick="this.select()"
                            name="cosvlruni[\'|| cos.cosid ||\']"
                            ' . $readonly . '
                            value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) || \'" />\' as cosvlruni

                    ,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(ecs.total, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
                FROM
                    cte.composicaosubacao cos
                LEFT JOIN
                    (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
                INNER JOIN
                    cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                WHERE
                    cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                    AND
                    cos.cosano = ' . $_REQUEST['cosano'] . '
                ORDER BY
                    cos.cosdsc';

                $sql   = $db->carregar($sql);

                $dat   = $db->carregar('
                                       SELECT
                        \' <div align="right"> Totais:</div>\' AS cosid,
						\'\' AS cosdsc,
						\'\' AS undddsc,
						\'\' AS cosqtd,
						\'\' AS cosvlruni,
							trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99,999,999.99\')) as vlrtotal
				                FROM
				                    cte.composicaosubacao cos
				                LEFT JOIN
				                    (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
				                INNER JOIN
				                    cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				                WHERE
				                    cos.sbaid  = ' . $_REQUEST['sbaid']  . '
				                    AND
				                    cos.cosano = ' . $_REQUEST['cosano']);

                $sql[] = $dat[0];
            } else {
                $sql = '
                SELECT
                    \'<center><img onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
                    ,cos.cosdsc
                    ,und.undddsc

                    ,\'<input size="10"
                            id="cosqtd\' || cos.cosid || \'"
                            type="text"
                            class="CampoEstilo"
                            onmouseover="MouseOver(this);"
                            onfocus="MouseClick(this);"
                            onmouseout="MouseOut(this);"
                            onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                            onkeyup="this.value=mascaraglobal(\'\'###.###,#\'\', this.value);"
                            onfocus="this.select()"
                            onclick="this.select()"
                            name="cosqtd[\'|| cos.cosid ||\']"
                            ' . $readonly . '
                            value="\' || trim(to_char(coalesce(cos.cosqtd, 0), \'999990D9\')) || \'" />\' as cosqtd

                    ,\'<input size="20"
                            id="cosvlruni\' || cos.cosid || \'"
                            type="text"
                            class="itemComposicao"
                            style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
                            onmouseover="MouseOver(this);"
                            onfocus="MouseClick(this);"
                            onmouseout="MouseOut(this);"
                            onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                            onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
                            onfocus="this.select()"
                            onclick="this.select()"
                            name="cosvlruni[\'|| cos.cosid ||\']"
                            ' . $readonly . '
                            value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) || \'" />\' as cosvlruni
                    ,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
                FROM
                    cte.composicaosubacao cos
                INNER JOIN
                    cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
                WHERE
                    cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                    AND
                    cos.cosano = ' . $_REQUEST['cosano'] . '
                ORDER BY
                    cos.cosdsc';

                $sql   = $db->carregar($sql);
                $dat   = $db->carregar('
                                       SELECT
                        \' <div align="right"> Totais:</div>\' AS cosid,
						\'\' AS cosdsc,
						\'\' AS undddsc,
						\'\' AS cosqtd,
						\'\' AS cosvlruni,
							trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'999999990D99\')) as vlrtotal
				                FROM
				                    cte.composicaosubacao cos
				                LEFT JOIN
				                    (SELECT cosid, SUM(ecsqtd) as total FROM cte.escolacomposicaosubacao GROUP BY cosid) ecs ON cos.cosid = ecs.cosid
				                INNER JOIN
				                    cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				                WHERE
				                    cos.sbaid  = ' . $_REQUEST['sbaid']  . '
				                    AND
				                    cos.cosano = ' . $_REQUEST['cosano']);

                $sql[] = $dat[0];
            }
        } else {
            $sql = '
            SELECT
                \'<center><img onclick="return ' .$remover. '(\' || cos.cosid || \')" src="/imagens/excluir.gif" alt="Excluir \' || cos.cosdsc || \'" /></center>\' as cosid
                ,cos.cosdsc
                ,und.undddsc

                ,\'<input size="10"
                        id="cosqtd\' || cos.cosid || \'"
                        type="text"
                        class="CampoEstilo"
                        onmouseover="MouseOver(this);"
                        onfocus="MouseClick(this);"
                        onmouseout="MouseOut(this);"
                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                        onkeyup="this.value=mascaraglobal(\'\'###.###,#\'\', this.value);"
                        onfocus="this.select()"
                        onclick="this.select()"
                        name="cosqtd[\'|| cos.cosid ||\']"
                        ' . $readonly . '
                        value="\' || trim(to_char(coalesce(cos.cosqtd, 0), \'999990D9\')) || \'" />\' as cosqtd

                ,\'<input size="20"
                        id="cosvlruni\' || cos.cosid || \'"
                        type="text"
                        class="itemComposicao"
                        style="border-right: #888888 1px solid;border-top: #888888 1px solid;font-size: 10px;vertical-align: middle;border-left: #000000 3px solid;border-bottom: #888888 1px solid;font-family: verdana;background-color: #ffffff"
                        onmouseover="MouseOver(this);"
                        onfocus="MouseClick(this);"
                        onmouseout="MouseOut(this);"
                        onblur="MouseBlur(this); verificarModificaoItensComposicao(this); calcularValorTotal(\' || cos.cosid || \');"
                        onkeyup="this.value=mascaraglobal(\'\'###.###.###.###,##\'\', this.value);"
                        onfocus="this.select()"
                        onclick="this.select()"
                        name="cosvlruni[\'|| cos.cosid ||\']"
                        ' . $readonly . '
                        value="\' || trim(to_char(coalesce(cos.cosvlruni, 0), \'0D99\')) || \'" />\' as cosvlruni

                ,\'<span style="display:block;width:50px;" id="cosvlrtotal\' || cos.cosid || \'">\' || trim(to_char(coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0), \'0D99\')) || \'</span>\' as vlrtotal
             FROM
                cte.composicaosubacao cos
            INNER JOIN
                cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
            WHERE
                cos.sbaid  = ' . $_REQUEST['sbaid']  . '
                AND
                cos.cosano = ' . $_REQUEST['cosano'] . '
            GROUP BY
				cos.cosdsc
				,und.undddsc
				, cosqtd
				,cosvlruni
				,cos.cosid
            ORDER BY
                cos.cosdsc';

            $sql   = $db->carregar($sql);

            $dat = $db->carregar('SELECT
					\' <div align="right"> totais : </div>\' AS cosid,
					\'\' AS cosdsc,
					\'\' AS undddsc,
					\'\' AS cosqtd,
					\'\' AS cosvlruni,
					trim(to_char(sum(cos.cosqtd * cos.cosvlruni), \'99,999,999.99\')) as vlrtotal
				FROM
					cte.composicaosubacao cos
				INNER JOIN
					cte.unidademedidadetalhamento und ON cos.unddid = und.unddid
				WHERE
					cos.sbaid  = ' . $_REQUEST['sbaid']  . '
					AND
					cos.cosano = ' . $_REQUEST['cosano']);

            $sql[] = $dat[0];
        }
       //dbg($sql);

        $db->monta_lista_simples($sql, array('Ações',
                                             'Identificação do Item',
                                             'Un. de Medida',
                                             'Qtd',
                                             'Valor Unitário',
                                             'Total'), 1000, 1000, 'N', '100%','N');
        //                                                                  */
    } elseif ($_REQUEST['req']   == 'removerItemComposicao' &&
              $_REQUEST['cosid'] != '')
    {
        //------------------------------------------- EXCLUIR ITENS COMPOSICAO
        $sql = '
        DELETE FROM
            cte.escolacomposicaosubacao
        WHERE
            cosid = ' . (integer) $_REQUEST['cosid'] . ';

        DELETE FROM
            cte.composicaosubacao
        WHERE
            cosid = ' . (integer) $_REQUEST['cosid'];

        $db->executar($sql);
        $db->commit();


    } elseif ($_REQUEST['req'] == 'carregarBeneficiarios' &&
              $_REQUEST['sbaid']  != ''                   &&
              $_REQUEST['sabano'] != '')
    {
        //--------------------------------------------- CARREGAR BENEFICIARIOS
        $sql = '
        SELECT
            \'<center><img onclick="return removerBeneficiario(\' || sb.benid || \')" src="/imagens/excluir.gif" style="border:none" alt="Escluir item" /></center>\' as benid,
            be.bendsc,
            sb.vlrurbano as vlrurbano,
            sb.vlrrural  as vlrrural,
            sb.vlrurbano + sb.vlrrural as sabvlrtotal
        FROM
            cte.subacaobeneficiario sb
        INNER JOIN
            cte.beneficiario be ON sb.benid = be.benid
        WHERE
            sb.sbaid  = ' . $_REQUEST['sbaid']   . '
            AND
            sb.sabano = ' . $_REQUEST['sabano'];

        $db->monta_lista_simples($sql, array('Ações',
                                             'Beneficiário',
                                             'Zona Urbana',
                                             'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%','N');

    } elseif ($_REQUEST['req']    == 'removerBeneficiario' &&
              $_REQUEST['benid']  != ''                    &&
              $_REQUEST['sabano'] != '')
    {
        //----------------------------------------------- REMOVER BENEFICIARIO
        $sql = '
        DELETE FROM
            cte.subacaobeneficiario
        WHERE
            benid  = ' . (integer) $_REQUEST['benid'] . '
            AND
            sabano = ' . (integer) $_REQUEST['sabano'];

        $db->executar($sql);
        $db->commit();


    } elseif ($_REQUEST['req']    == 'carregarEscolas' &&
              $_REQUEST['sbaid']  != ''                &&
              $_REQUEST['qfaano'] != '')
    {
        //--------------------------------------------------- CARREGAR ESCOLAS
        $sql = '
        SELECT
            \'<center><img onclick="return removerEscola(\' || q.qfaid || \')" src="/imagens/excluir.gif" style="border:none" alt="Escluir item" /> <img onclick="return extratoEscolas(\' || q.qfaid || \')" src="/imagens/consultar.gif" style="border:none" alt="Escluir item" /></center>\' as qfaid,
            e.entcodent,
            e.entnome,
            sum(coalesce(qfaqtd,0))
        FROM
            cte.qtdfisicoano q
        INNER JOIN
            entidade.entidade e on q.entid = e.entid
        WHERE
            q.qfaano = ' . $_REQUEST['qfaano'] . '
            AND
            q.sbaid  = ' . $_REQUEST['sbaid']  . '
        GROUP BY
            q.qfaid,
            e.entcodent,
            e.entnome
        ORDER BY
            entnome';

        $db->monta_lista_simples($sql, array('Ações',
                                             'Código INEP',
                                             'Escola',
                                             'Quantidade'), 1000, 1000, 'S', '100%','N');

    } elseif ($_REQUEST['req']   == 'removerEscola' &&
              $_REQUEST['qfaid'] != '')
    {
        //---------------------------------------------------- REMOVER ESCOLAS
        $sql = '
        DELETE FROM
            cte.escolacomposicaosubacao
        WHERE
            qfaid = ' . (integer) $_REQUEST['qfaid'] . ';

        DELETE FROM
            cte.qtdfisicoano
        WHERE
            qfaid = ' . (integer) $_REQUEST['qfaid'];

        $db->executar($sql);
        $db->commit();


    } elseif ($_REQUEST['req'] == 'carregarDadosParecer') {
        //------------------------------------------- DADOS DO PARECER TECNICO
        $sql = '
        SELECT
            coalesce(spt.sptparecer, \'\')  as sptparecer,
            coalesce(spt.sptunt, 0)         as sptunt,
            coalesce(spt.sptuntdsc, \'\')   as sptuntdsc,
            coalesce(spt.sptano, 0)         as sptano,
            coalesce(spt.sptinicio, 0)      as sptinicio,
            coalesce(spt.sptfim, 0)         as sptfim,
            coalesce(spt.ssuid, 0)          as ssuid
        FROM
            cte.subacaoparecertecnico spt
        INNER JOIN
            cte.subacaoindicador sba on spt.sbaid = sba.sbaid
        WHERE
            spt.sbaid  = ' . $_REQUEST['sbaid'] . '
            AND
            spt.sptano = ' . $_REQUEST['sbtano'];

        $res = $db->pegaLinha($sql);

        header('content-type: text/xml; charset=iso-8859-1');
        echo '<?xml version="1.0" encoding="iso-8859-1"?>'        , "\n"
            ,'<response>'                                         , "\n"
            ,'<sptparecer>', $res['sptparecer'] , '</sptparecer>' , "\n"
            ,'<sptunt>'    , $res['sptunt']     , '</sptunt>'     , "\n"
            ,'<sptuntdsc>' , $res['sptuntdsc']  , '</sptuntdsc>'  , "\n"
            ,'<sptano>'    , $res['sptano']     , '</sptano>'     , "\n"
            ,'<sptinicio>' , $res['sptinicio']  , '</sptinicio>'  , "\n"
            ,'<sptfim>'    , $res['sptfim']     , '</sptfim>'     , "\n"
            ,'<ssuid>'     , $res['ssuid']      , '</ssuid>'      , "\n"
            ,'</response>';

        //                                                                  */
    } elseif ($_REQUEST['req'] == 'carregarTotalizadores') {
        if ($_REQUEST['sbaporescola'] == 't') {
            // TOTALIZACAO POR ESCOLA
            $sql = '
            SELECT DISTINCT
                ent.entnome,
                sum(qfa.qfaqtd) as qtdtotal,
                sum(tot.cosvlruni * ecs.ecsqtd) as vlrtotal
            FROM
                cte.qtdfisicoano qfa
            LEFT JOIN
                cte.escolacomposicaosubacao ecs on qfa.qfaid = ecs.qfaid
            LEFT JOIN
                cte.composicaosubacao cos on cos.cosid = ecs.cosid
            INNER JOIN
                entidade.entidade ent on qfa.entid = ent.entid
            LEFT JOIN
                (SELECT DISTINCT
                    cos.cosid,
                    cos.cosvlruni
                 FROM
                    cte.escolacomposicaosubacao ecs
                 INNER JOIN
                    cte.composicaosubacao cos on cos.cosid = ecs.cosid
                 GROUP BY
                    cos.cosid,
                    cos.cosvlruni) as tot on tot.cosid = ecs.cosid
            WHERE
                qfa.sbaid = ' . $_REQUEST['sbaid'] . '
            GROUP BY
                ent.entnome
            ORDER BY
                ent.entnome';

            echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Escola</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
            $db->monta_lista_simples($sql, array('Escola',
                                                 'Quantidade',
                                                 'Total (R$)'), 1000, 1000, 'S', '100%','N');

            echo '</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';

        } else {
            echo '<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">'
                ,'<tr>'
                ,'<td style="font-weight:bold;text-align:center">Totalizador Por Itens de Composição</td>'
                ,'</tr>'
                ,'<tr>'
                ,'<td>';
        }


        $sql = '
        SELECT
            cos.cosdsc
            ,coalesce(cos.cosqtd, 0) as cosqtd
            ,coalesce(cos.cosqtd, 0) * coalesce(cos.cosvlruni, 0) as cosvlrtotal
        FROM
            cte.composicaosubacao cos
        WHERE
            cos.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Identificação do Item',
                                             'Qtd',
                                             //'Valor Unitário',
                                             'Total'), 1000, 1000, 'S', '100%');

        echo '</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td style="font-weight:bold;text-align:center">Totalizador Por Beneficiários</td>'
            ,'</tr>'
            ,'<tr>'
            ,'<td>';

        $sql = '
        SELECT
            be.bendsc,
            sb.vlrurbano as vlrurbano,
            sb.vlrrural  as vlrrural,
            sb.vlrurbano + sb.vlrrural as sabvlrtotal
        FROM
            cte.subacaobeneficiario sb
        INNER JOIN
            cte.beneficiario be ON sb.benid = be.benid
        WHERE
            sb.sbaid  = ' . $_REQUEST['sbaid'];

        $db->monta_lista_simples($sql, array('Beneficiário',
                                             'Zona Urbana',
                                             'Zona Rural',
                                             'Total'), 1000, 1000, 'S', '100%');
        echo '</td>'
            ,'</tr>'
            ,'</table>';
    }

    die();
}



function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}



/*!@
if ($db->testa_superuser() || cte_possuiPerfil(CTE_PERFIL_EQUIPE_TECNICA_FNDE)) {
    $display2007 = "";
} else {
    $display2007 = "style=\"display:none\"";
}
//                                                                          */

include APPRAIZ . "includes/registraracesso.php";





$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";



//---------------------------------------------- SUBAÇÕES - PROXIMA E ANTERIOR
$sql = '
    select
        sai.sbaid,
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        ind.indid
    from cte.subacaoindicador sai
        inner join cte.acaoindicador aci on
            aci.aciid = sai.aciid
        inner join cte.pontuacao pto on
            pto.ptoid = aci.ptoid
        inner join cte.indicador ind on
            ind.indid = pto.indid
        inner join cte.areadimensao ard on
            ard.ardid = ind.ardid
        inner join cte.dimensao dim on
            dim.dimid = ard.dimid
    where
        pto.inuid = ' . $_SESSION['inuid'] . ' and
        pto.ptostatus = \'A\'
    order by
        dim.dimcod,
        ard.ardcod,
        ind.indcod,
        sai.sbaordem,
        sai.sbadsc';


$subAcoes      = (array) $db->carregar($sql);
$sbaidAnterior = false;
$sbaidProximo  = false;

while (list($k, $subacao) = each($subAcoes)) {
    if ($subacao["sbaid"] == (integer) $_REQUEST['sbaid']) {

        if (array_key_exists($k - 1, $subAcoes)) {
            $sbaidAnterior = $subAcoes[$k - 1]["sbaid"];
        }

        if (array_key_exists($k + 1, $subAcoes)) {
            $sbaidProximo  = $subAcoes[$k + 1]["sbaid"];
        }

        break;
    }
}




//-------------------------------------------------- SUBAÇÕES - CARGA DE DADOS
$sql = '
select
    s.sbaid,
    s.aciid,
    s.undid,
    s.frmid,
    s.sbadsc,
    s.sbastgmpl,
    s.sbaprm,
    s.sbapcr,
    -- s.sba1ano,
    -- s.sba2ano,
    -- s.sba3ano,
    -- s.sba4ano,
    -- s.sbaunt,
    -- s.sbauntdsc,
    -- s.sba1ini,
    -- s.sba1fim,
    -- s.sba2ini,
    -- s.sba2fim,
    -- s.sba3ini,
    -- s.sba3fim,
    -- s.sba4ini,
    -- s.sba4fim,
    s.sbadata,
    s.usucpf,
    s.sbaparecer,
    s.psuid,
    s.ssuid,
    -- s.sba0ano,
    -- s.sba0ini,
    -- s.sba0fim,
    s.foaid,
    s.prgid,
    s.sbaporescola,
    s.ppsid,
    s.sbaordem,
    s.sbaobjetivo,
    s.sbatexto,
    s.sbacategoria
from
    cte.subacaoindicador s
left join
    cte.programa p on p.prgid = s.prgid
where
    s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);


$sbaporescola            = $sbaporescola == 't';
$existemEscolasAtendidas = $db->pegaUm('SELECT count(*) FROM cte.qtdfisicoano WHERE sbaid = ' . (integer) $_REQUEST['sbaid']) > 0;

$sql = '
select
    dim.dimid,
    dim.dimcod,
    dim.dimdsc,
    are.ardid,
    are.ardcod,
    are.arddsc,
    ind.indid,
    ind.indcod,
    ind.inddsc,
    aci.acidsc,
    aci.acilocalizador,
    pto.ptoid
from
    cte.acaoindicador aci
inner join
    cte.pontuacao pto on pto.ptoid = aci.ptoid and pto.ptostatus = \'A\'
inner join
    cte.indicador ind on ind.indid = pto.indid
inner join
    cte.areadimensao are on are.ardid = ind.ardid
inner join
    cte.dimensao dim on dim.dimid = are.dimid
where
    aci.aciid = ' . (trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid']);

$aci = (array) $db->pegaLinha($sql);
extract($aci);

$_SESSION['indid'] = $indid;
$aciid             = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];





$_cosano = array(2007,2008,2009,2010);
?><html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title>PAR - Plano de Metas - Subação<?php echo " - " , $titulo; ?></title>

    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>

    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script type="text/javascript" src="../includes/prototype.js"></script>
    <script type="text/javascript">
    function getElementText(tag, parentElem, index)
    {
        if (!index) {
            index=0;
        }

        var result = parentElem.responseXML.getElementsByTagName(tag)[index];

        if (result) {
            if (result.childNodes.length == 0) {
                return '';
            } else if (result.childNodes.length > 1) {
                return result.childNodes[1].nodeValue;
            } else {
                return result.firstChild.nodeValue;
            }
        } else {
            return '';
        }
    }
    </script>

    <style type="text/css" media="screen">
        #tabAbasSelecaoCosano td.container {
            height: 200px;
        }

        #tabAbasSelecaoCosano {
            border-left: 2px solid #DAE0D2;
            border-right: 2px solid #DAE0D2;
            background-color: #dcdcdc;
        }

        #abasSelecaoCosano {
            background: none;/*transparent url("../imagens/bg.gif") repeat-x bottom;*/
            line-height: normal;
            padding: 0 auto;
            text-align: center;
            border: 2px solid #DAE0D2;
        }

        #abasSelecaoCosano td {
            margin: 0;
            padding: 0 0 0 5px;
            background: url("../imagens/left_both.gif") no-repeat left top;
            border-bottom: 1px solid #765;
            background-position: 0% 0%;
        }

        #abasSelecaoCosano a {
            display: block;
            background: url("../imagens/right_both.gif") no-repeat right top;
            padding: 5px 0 4px 0;
            text-decoration: none;
            font-weight: bold;
            color: #765;
            width: auto;
        }

        #abasSelecaoCosano a:hover {
            background-position: 0% -150px;
            color: #333;
        }

        #abasSelecaoCosano td:hover, #abasSelecaoCosano td:hover a {
            background-position: 0% -150px;
            color: #333;
        }

        #abasSelecaoCosano td:hover a {
            background-position: 100% -150px;
        }

        #loader-container,
        #LOADER-CONTAINER
        {
            background: none;
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
  <body>
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>

    <div id="container">
      <?php cte_montaTitulo( $titulo_modulo, '' ); ?>

      <!-- LANÇAMENTO DE DADOS                                              -->

<?php

if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO) {

?>
      <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <colgroup>
          <col/>
        </colgroup>
        <tbody>
          <tr>
            <td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; vertical-align: top;" colspan="4">
              <img align="absmiddle" src="/imagens/atencao.png" /> <p>Para elaborar o Plano de Ações Articuladas é necessário finalizar o <a href="?modulo=principal/estrutura_avaliacao&acao=A" title="Exibir diagnóstico" onclick="return redirecionarFaseDiagnostico();">diagnóstico</a>.</p>
            </td>
          </tr>
        </tbody>
      </table>
    </div>
  </body>

  <script type="text/javascript">
  <!--
    function redirecionarFaseDiagnostico()
    {
        window.opener.location.href = '/brasilpro/brasilpro.php?modulo=principal/estrutura_avaliacao&acao=A';
        window.opener.focus();
        window.close();
    }

    document.getElementById('loader-container').style.display = 'none';
    -->
  </script>
<?php
    die();
} // if ($estadoDocumento['esdid'] == CTE_ESTADO_DIAGNOSTICO)

?>

      <form method="post" id="frmParSubacao" action="<?php echo $_SERVER['REQUEST_URI']; ?>">
        <input type="hidden" name="formularioSumit" value="1"/>
        <input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
        <input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
        <input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
        <input type="hidden" name="anoExercicio" id="anoExercicio" value="0"/>
        <input type="hidden" name="sbaobjetivo" id="sbaobjetivo" value="<?php echo $sbaobjetivo ?>"/>

        <table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
          <colgroup>
            <col width="25%" />
            <col width="75%" />
          </colgroup>

          <tbody>
            <tr>
              <td class="SubTituloDireita">Dimensão:</td>
              <td><?php echo $aci['dimcod'] , '. ' , $aci['dimdsc'] ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Área:</td>
              <td><?php echo $aci['ardcod'] , '. ' , $aci['arddsc']; ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Indicador:</td>
              <td>
                <?php echo $aci['indcod']   , '. ' , $aci['inddsc']; ?>
              </td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Demanda:</td>
              <td><?php echo $aci['acilocalizador'] == "E" ? "Rede Estadual" : "Rede Municipal"; ?></td>
            </tr>

            <tr>
              <td class="SubTituloDireita">Ação:</td>
              <td>
                <?php echo $aci['acidsc']; ?>
              </td>
            </tr>

<?php
// CTE_PRM_EDITAR_SUBACAO
if (!CTE_NOVA_SUBACAO) {

    $select_formaAtendimento = $db->pegaUm('SELECT foadsc FROM cte.formaatendimento WHERE foaid = ' . (integer) $foaid);
    $select_programa         = $db->pegaUm('SELECT prgdsc FROM cte.programa WHERE prgid = '         . (integer) $prgid);
    $select_unidadeMedida    = $db->pegaUm('SELECT unddsc FROM cte.unidademedida WHERE undid = '    . (integer) $undid);

    $select_formaExecucao    = $db->pegaUm('SELECT frmdsc FROM cte.formaexecucao WHERE frmid = '    . (integer) $frmid);

    $prgplanointerno         = $db->pegaUm('SELECT
                                                prgplanointerno
                                            FROM
                                                cte.subacaoindicador sba
                                            INNER JOIN
                                                cte.programa prg ON prg.prgid = sba.prgid
                                            WHERE
                                                sbaid = '    . (integer) $sbaid);

?>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
                <strong>Dados da Subação</strong>
              </td>
            </tr>

            <tr id="tr_categoriaDespesa">
              <td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
              <td><?php echo $sbacategoria == 3 ? 'Custeio' : 'Capital'; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo $sbadsc; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo $sbastgmpl; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Plano Interno:</td>
              <td><?php echo $prgplanointerno; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
              <td><?php echo $select_unidadeMedida; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Forma de Execução:</td>
              <td><?php echo $select_formaExecucao ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo $sbapcr; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Cronograma:</td>
              <td><?php echo !$sbaporescola ? 'Global' : 'Por Escola'; ?></td>
            </tr>
          </tbody>
        </table>
<?php

} else {


//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
    $select_formaAtendimento = $db->monta_combo('foaid',
                                                'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
                                                'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);

    $select_programa         = $db->monta_combo('prgid',
                                                'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
                                                'S', 'Selecione', 'exibirPlanoInterno', '', '', '', 'S', 'prgid', true);

    $select_unidadeMedida    = $db->monta_combo('undid',
                                                'SELECT DISTINCT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni INNER JOIN cte.indicadorunidademedida iun ON uni.undid = iun.undid WHERE undtipo = \'' . $capturaTipo . '\' and indid = ' . $indid . ' ORDER BY descricao',
                                                'S', 'Escolha a unidade de medida', '', '','','','S', 'undid', true);

    $sql = '
    SELECT
        frmid as codigo,
        frmdsc as descricao
    FROM
        cte.formaexecucao
    WHERE
        frmtipo = \'' . $capturaTipo . '\'
        AND
        frmbrasilpro = true
    ORDER BY
        descricao';

    $select_formaExecucao = $db->monta_combo('frmid', $sql, 'S', 'Selecione', 'regraFormaExec', '','','','S', 'frmid', true);

?>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
                <strong>Dados da Subação</strong>
              </td>
            </tr>

            <tr id="tr_categoriaDespesa">
              <td align='right' class="SubTituloDireita">Categoria de Despesa:</td>
              <td>
                <select name="sbacategoria" class="CampoEstilo" id="sbacategoria">
                  <option value="">Escolha...</option>
                  <option value="3" <?php echo $sbacategoria == 3 ? 'selected="selected"' : '' ?>>Custeio</option>
                  <option value="4" <?php echo $sbacategoria == 4 ? 'selected="selected"' : '' ?>>Capital</option>
                </select>
              </td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo campo_textarea('sbadsc', "S", 'S', '', 70, 3, 2000 ); ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo campo_textarea('sbastgmpl', 'S', 'S', '', 70, 3, 2000 ); ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Plano Interno:</td>
              <td><?php echo campo_texto('prgplanointerno', "N", "N", "Plano Interno", 51, 100, "", "", 'left', '', 0, 'id="prgplanointerno"' );?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Unidade de Medida:</td>
              <td><?php echo $select_unidadeMedida; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Forma de Execução:</td>
              <td><?php echo $select_formaExecucao ?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo campo_texto('sbapcr', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"');?></td>
            </tr>

            <tr>
              <td align='right' class="SubTituloDireita">Cronograma:</td>
              <td>
                <input id="sbaporescola0" type="radio" name="sbaporescola" value="false" <?php echo !$sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola0">Global</label>
                <input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  <?php echo  $sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola1">Por Escola</label>
              </td>
            </tr>

<?php

} // if (!CTE_NOVA_SUBACAO)

$colspan       = sizeof($_cosano) + 1;
$tamanhoCelula = round(100 / $colspan);
?>
          </tbody>
        </table>
<script>
function controledetalhes(img) {
	if(img.title == 'mais') {
		document.getElementById('detalhamentosubacao').style.display = '';
		img.src = '/imagens/menos.gif'
		img.title = 'menos';
		carregarDetalhesSubacao(<? echo date('Y'); ?>);
	} else {
		document.getElementById('detalhamentosubacao').style.display = 'none';
		img.src = '/imagens/mais.gif'
		img.title = 'mais';
	}
}
</script>
<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0">
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<? echo count($_cosano); ?>" style="padding: 5px; font-size: 110%">
                <strong><img src='/imagens/mais.gif' border='0' title='mais' onclick='controledetalhes(this);'> Detalhamento da Subação</strong>
              </td>
            </tr>
</table>        
<table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" style="display:none" id="detalhamentosubacao">
            <tr id="abasSelecaoCosano">
            <input type="hidden" id="crtabadetalhes">
<?php
    foreach ($_cosano as $ano) {
        $i = $ano;
        echo '<td><a id="abadetalhes', $i ,'" onclick="carregarDetalhesSubacao(', $i ,');">' , $i , '</a></td>' , "\n";
    }
?>
            </tr>

<tr>
<td colspan="<? echo count($_cosano); ?>" id="detalhessubacao"></td>
</tr>
</table>
<br />
        
        

        <table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" style="display:none" id="alertaPermitirCadastroItensSubacao">
          <tbody>
            <tr style="background-color: #cccccc;">
              <td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
                <strong>Finalize o cadastro da subação para que escolas, itens, beneficiários e parecer possam ser registrados.</strong>
              </td>
            </tr>
            <tr>
              <td align="center" colspan="<?php echo $colspan; ?>" style="padding: 15px; font-size: 110%">
                <input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar" value="Gravar" id="btnSalvar" <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?>/>
                <input onclick="return submeterFrmParSubacao(false, '');" type="button" name="btnCancelar" value="Fechar" id="btnCancelar" />
              </td>
            </tr>
          </tbody>
        </table>

        <table align="center" border="0" class="tabela listagem" cellpadding="0" cellspacing="0" id="tabAbasSelecaoCosano">
          <tbody>
            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo $colspan; ?>" style="padding: 5px; font-size: 110%">
                <strong>Detalhamento do aditivo da subação - <span id="spanAno"></span></strong>
              </td>
            </tr>

            <tr id="abasSelecaoCosano" style="display:none">
<?php
	$_cosano = array(date('Y'));
	
    foreach ($_cosano as $ano) {
        $i = $ano;
        echo '<td id="aba_' , $i , '" style="width: ' , $tamanhoCelula , '%;"><a onclick="return selecionarAba(' , $i , ', true)" href="' , $_SERVER['REQUEST_URI'] , '">' , $i , '</a></td>' , "\n";
    }?>
            </tr>

<?php
    foreach ($_cosano as $ano) {
        $i = $ano;
?>
            <tr class="container" style="display: none; height: auto; vertical-align: top" id="abaCosano<?php echo $i; ?>">
              <td align="center" colspan="<?php echo $colspan; ?>">
                <table cellpadding="0" cellspacing="0" width="100%">
                  <tr>
                    <td style="background-color: #ddd">
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Quantidades e Cronograma Físico</div>
                      <table align="center" border="0" class="tabela" cellpadding="2" cellspacing="1" style="margin-bottom: 10px;">
<?php
if (!$sbaporescola) {

?>
                        <tr id="cronogramaGlobal<?php echo $i; ?>">
                          <td class="SubTituloDireita">Quantidades:</td>
                          <td style="background-color: #fff"><?php echo campo_texto('sptunt_' . $i, "S", "S", 'Quantitativos', 10, 25 , '', '', 'left', '', 0, 'id="sptunt_' . $i . '"');?></td>
                        </tr>

<?php

}

?>
                        <tr>
                          <td class="SubTituloDireita">Cronograma Físico:</td>
                          <td style="background-color: #fff; padding-left: 2px">
                            <select name="sptinicio_<?php echo $i; ?>" id="sptinicio_<?php echo $i; ?>">
                              <option value="0">Selecione</option>
                              <option value="1"  <?php echo $sptinicio == '1'  ? 'selected="selected"' : '' ?>>Janeiro</option>
                              <option value="2"  <?php echo $sptinicio == '2'  ? 'selected="selected"' : '' ?>>Fevereiro</option>
                              <option value="3"  <?php echo $sptinicio == '3'  ? 'selected="selected"' : '' ?>>Março</option>
                              <option value="4"  <?php echo $sptinicio == '4'  ? 'selected="selected"' : '' ?>>Abril</option>
                              <option value="5"  <?php echo $sptinicio == '5'  ? 'selected="selected"' : '' ?>>Maio</option>
                              <option value="6"  <?php echo $sptinicio == '6'  ? 'selected="selected"' : '' ?>>Junho</option>
                              <option value="7"  <?php echo $sptinicio == '7'  ? 'selected="selected"' : '' ?>>Julho</option>
                              <option value="8"  <?php echo $sptinicio == '8'  ? 'selected="selected"' : '' ?>>Agosto</option>
                              <option value="9"  <?php echo $sptinicio == '9'  ? 'selected="selected"' : '' ?>>Setembro</option>
                              <option value="10" <?php echo $sptinicio == '10' ? 'selected="selected"' : '' ?>>Outubro</option>
                              <option value="11" <?php echo $sptinicio == '11' ? 'selected="selected"' : '' ?>>Novembro</option>
                              <option value="12" <?php echo $sptinicio == '12' ? 'selected="selected"' : '' ?>>Dezembro</option>
                            </select>
                            a
                            <select name="sptfim_<?php echo $i; ?>" id="sptfim_<?php echo $i; ?>">
                              <option value="0">Selecione</option>
                              <option value="1"  <?php echo $sptfim == '1'  ? 'selected="selected"' : '' ?>>Janeiro</option>
                              <option value="2"  <?php echo $sptfim == '2'  ? 'selected="selected"' : '' ?>>Fevereiro</option>
                              <option value="3"  <?php echo $sptfim == '3'  ? 'selected="selected"' : '' ?>>Março</option>
                              <option value="4"  <?php echo $sptfim == '4'  ? 'selected="selected"' : '' ?>>Abril</option>
                              <option value="5"  <?php echo $sptfim == '5'  ? 'selected="selected"' : '' ?>>Maio</option>
                              <option value="6"  <?php echo $sptfim == '6'  ? 'selected="selected"' : '' ?>>Junho</option>
                              <option value="7"  <?php echo $sptfim == '7'  ? 'selected="selected"' : '' ?>>Julho</option>
                              <option value="8"  <?php echo $sptfim == '8'  ? 'selected="selected"' : '' ?>>Agosto</option>
                              <option value="9"  <?php echo $sptfim == '9'  ? 'selected="selected"' : '' ?>>Setembro</option>
                              <option value="10" <?php echo $sptfim == '10' ? 'selected="selected"' : '' ?>>Outubro</option>
                              <option value="11" <?php echo $sptfim == '11' ? 'selected="selected"' : '' ?>>Novembro</option>
                              <option value="12" <?php echo $sptfim == '12' ? 'selected="selected"' : '' ?>>Dezembro</option>
                            </select>
                          </td>
                        </tr>
                      </table>
                      <br />
                    </td>
                  </tr>

<?php

if ($sbaporescola) {

?>
                  <tr id="cronogramaPorEscola<?php echo $i; ?>">
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Escolas</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="escolasSubAcao<?php echo $i; ?>"></div>
                      <div style="padding: 5px 0px 0px 5px">
                        <span style="cursor:pointer;display:block;float:left" onclick="return popupSelecionarEscolas(<?php echo $i; ?>);"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Editar / Inserir Escolas</span>
                        <!--<span id="totalizadorEscolas<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>
<?php

} // if ($sbaporescola)

?>
                  <tr>
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Itens</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="itensComposicaoSubAcao<?php echo $i; ?>"></div>
                      <div style="padding: 5px 0px 0px 5px">
                        <span style="cursor:pointer;display:block;float:left" onclick="return adicionarItensComposicao();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Editar / Inserir Itens de Composição</span>
                        <!--<span id="totalizadorItensComposicaoSubAcao<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>

                  <tr id="beneficiarioSubAcao<?php echo $i; ?>">
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Beneficiários</div>
                      <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="beneficiariosSubAcao<?php echo $i; ?>"></div>
                      <div style="padding: 5px 0px 0px 5px">
                        <span style="cursor:pointer;display:block;float:left" onclick="return adicionarBeneficiarios();"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Editar / Inserir Beneficiários</span>
                        <!--<span id="totalizadorBeneficiarios<?php echo $i; ?>">0</span>-->
                      </div><br />
                    </td>
                  </tr>

                  <tr>
                    <td>
                      <div class="SubtituloEsquerda" style="padding: 5px; text-align: center">Parecer Equipe Técnica</div>
                      <table id="parecerEquipeTecnica<?php echo $i; ?>" border="0" align="center" cellpadding="0" cellspacing="0" width="100%">
                        <colgroup>
                          <col width="25%" />
                          <col width="75%" />
                        </colgroup>
                        <tr>
                          <td align="right" class="SubTituloDireita">Parecer:</td>
                          <td><?php echo campo_textarea('sptparecer_' . $i, 'N', 'S', '', 100, 3, 0 ); ?></td>
                        </tr>
                        <tr>
                          <td align="right" class="SubTituloDireita">Detalhamento/Comentário:</td>
                          <td><?php echo campo_textarea('sptuntdsc_'  . $i, 'N', 'S' , '', 100, 10, 2000 ); ?></td>
                        </tr>
                        <tr>
                          <td align="right" class="SubTituloDireita">Status Subação:</td>
                          <td>
                              <?php
                              $db->monta_combo('ssuid_' . $i,
                                               "select ssuid as codigo, ssudescricao as descricao from cte.statussubacao WHERE ssutipostatus = 'E'",
                                               'S', 'Selecione', '', '', '', '', '', 'ssuid_' . $i);
                              ?>
                          </td>
                        </tr>
                      </table><br />
                    </td>
                  </tr>
                </table>
              </td>
            </tr>
<?php
    } // foreach ($_cosano as $ano)
?>

            <tr class="container" style="display: none; height: auto" id="abaCosano0000">
              <td align="left" colspan="<?php echo $colspan; ?>">
                <div style="margin: 0; padding: 0; height: auto; width: 100%; background-color: #eee; border: none;" class="div_rolagem" id="totalizadoresSubacao"></div>
              </td>
            </tr>
            <tr>
              <td align="center" colspan="<?php echo $colspan; ?>">
              <table width="100%">
                <tr>
                <td colspan="3" style="font-weight:bold;color:red;text-align : center; display:none" id="errorMessageContainer">
                    Documentos em análise financeira devem conter Escolas, Itens de Composição e Beneficiários cadastrados.
                  </td>
                </tr>

              	<tr id="buttonsContainer">

                <td style="text-align : center;">
                  <input type="hidden" name="sbaid" value="<?php echo $_REQUEST['sbaid']; ?>" />
                  <input type="hidden" name="vlrProximaSubacao" id="vlrProximaSubacao">
                  <input onclick="return submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');" type="button" name="btnSalvar"         value="Gravar"            id="btnSalvar"         <?php echo !CTE_PRM_EDITAR_SUBACAO ? 'disabled="disabled"' : ''; ?> />
                  <input onclick="return submeterFrmParSubacao(true,  -1);"                                  type="button" name="btnExcluir"        value="Excluir"           id="btnExcluir"        <?php echo !CTE_PRM_EXCLUIR_SUBACAO                   ? 'disabled="disabled"' : ''; ?>/>
                  <input onclick="return submeterFrmParSubacao(false, '');"                                  type="button" name="btnCancelar"       value="Fechar"            id="btnCancelar" />
                </td>
              	</tr>
              </table>
              </td>
            </tr>

          </tbody>
        </table>
      </form>
    </div>
  </body>


<?php
if ($undid) {
    $sql = '
    SELECT
        il.labid
    FROM
        cte.laboratorio l
    INNER JOIN
        cte.itenslaboratorio il ON l.labid = il.labid
    WHERE
        l.undid = ' . $undid;

    if (($labid = $db->pegaUm($sql)) === false)
        $labid = 'null';
} else {
    $labid = 'null';
}


?>
  <script type="text/javascript">
  <!--
    /**
     * @public
     * @global
     */
    var IExplorer               = !!document.all;
    var safari                  = navigator.userAgent.indexOf( 'Safari' ) > 0;
    var gecko                   = navigator.product == 'Gecko';
    var _displayBlock           = IExplorer ? 'block' : 'table-row';

    var TAB_TOTALIZADORES       = '0000';
    var existemEscolasAtendidas = <?php echo $sbaporescola && $existemEscolasAtendidas ? 'true' : 'false' ?>;
    var anoExercicio            = '<?php echo ($_REQUEST['parsubacaoanocrt'])?$_REQUEST['parsubacaoanocrt']:date('Y');  ?>';
    var permitirCadastroSubacao = <?php echo trim($_REQUEST['sbaid']) != '' ? 'true' : 'false'; ?>;

	var subacaoid = <? echo $_REQUEST['sbaid']; ?>;
    var itemModificado          = false;

    var totalizadorItensComposicao = 0;
    var totalizadorBeneficiarios   = 0;
    var totalizadorEscolas         = 0;
    var labid                      = <?php echo $labid; ?>;
    var total                      = 0;

    function _totalizar(x)
    {
        total = x;
    }


    /**
     * 
     */
    function carregarDetalhesSubacao(ano)
    {

        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: 'req=carregarDetalhesSubacao&paramano=' + ano + '&sbaid='+ subacaoid +'&sbaporescola=<?php echo $sbaporescola ? 't' : 'f'; ?>',
                                    onComplete: function(res)
                                    {
                                        $('detalhessubacao').innerHTML = res.responseText;
										if(document.getElementById('crtabadetalhes').value != '') {
        									$('abadetalhes' + $('crtabadetalhes').value).style.backgroundPosition            = '0% 0%';
										}
        								$('abadetalhes' + ano)      .style.backgroundPosition            = '0% -150px';
        								$('crtabadetalhes').value = ano;
                                    }
                                });
    }


    /**
     * 
     */
    function carregarItensComposicao()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: 'req=carregarItensComposicao&cosano=' + anoExercicio + '&sbaid='+ subacaoid +'&sbaporescola=<?php echo $sbaporescola ? 't' : 'f'; ?>',
                                    onComplete: function(res)
                                    {
                                        $('itensComposicaoSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
    }


    /**
     * 
     */
    function carregarBeneficiarios()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarBeneficiarios&sbaid='+ subacaoid +'&sabano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        $('beneficiariosSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
    }


    /**
     * 
     */
    function carregarDadosParecer()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarDadosParecer&sbaid='+ subacaoid +'&sbtano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        var sptparecer      = 'sptparecer_' + anoExercicio;
                                        var sptunt          = 'sptunt_'     + anoExercicio;
                                        var sptuntdsc       = 'sptuntdsc_'  + anoExercicio;
                                        var sptinicio       = 'sptinicio_'  + anoExercicio;
                                        var sptfim          = 'sptfim_'     + anoExercicio;
                                        var ssuid           = 'ssuid_'      + anoExercicio;

                                        var ssuidValue      = getElementText('ssuid'    , res);
                                        var sptinicioValue  = getElementText('sptinicio', res);
                                        var sptfimValue     = getElementText('sptfim'   , res);

                                        for (var i = 0; i < $(ssuid).options.length; i++) {
                                            if (ssuidValue == $(ssuid).options[i].value)
                                                $(ssuid).selectedIndex = $(ssuid).options[i].index;
                                        }

                                        for (var i = 0; i < $(sptinicio).options.length; i++) {
                                            if (sptinicioValue == $(sptinicio).options[i].value)
                                                $(sptinicio).selectedIndex = $(sptinicio).options[i].index;
                                        }

                                        for (var i = 0; i < $(sptfim).options.length; i++) {
                                            if (sptfimValue == $(sptfim).options[i].value)
                                                $(sptfim).selectedIndex =  $(sptfim).options[i].index;
                                        }

                                        if ($(sptunt))
                                            $(sptunt).value = getElementText('sptunt',    res);

                                        $(sptparecer).update(getElementText('sptparecer', res));
                                        $(sptuntdsc ).update(getElementText('sptuntdsc',  res));
                                    }
                                });
    }


    /**
     * 
     */
    function carregarTotalizadores()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&sbaid='+ subacaoid +'&req=carregarTotalizadores&sbaporescola=<?php echo $sbaporescola ? 't' : 'f'; ?>',
                                    onComplete: function(res)
                                    {
                                        $('totalizadoresSubacao').innerHTML = res.responseText;
                                    }
                                });
    }

    /**
     * 
     */
    function adicionarItensComposicao()
    {
        //if (itemModificado && !confirm(('Existem itens de composição que não foram salvos. Ao adicionar novos itens, os dados modificados serão perdidos.\n' +
        //                                'Deseja continuar sem salvá-los?'))) {
        //    return false;
        //} else {
        if (itemModificado)
            submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');

        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastraritenssubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosano=' + anoExercicio,
                          'adicionarItensComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        //}
    }


    /**
     * 
     */
    function adicionarBeneficiarios()
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastrarbeneficiariossubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&sabano=' + anoExercicio,
                          'adicionarBeneficiarios',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function removerItemComposicao(cosid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerItemComposicao&sbaid='+ subacaoid +'&cosid=' + cosid,
                                       onComplete: function(res)
                                       {
                                           carregarItensComposicao();
                                       }
                                   });

        return false;
    }


    /**
     * 
     */
    function removerBeneficiario(benid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        var req = new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerBeneficiario&sbaid='+ subacaoid +'&benid=' + benid + '&sabano=' + anoExercicio,
                                       onComplete: function(res)
                                       {
                                           carregarBeneficiarios();
                                       }
                                   });

        return false;
    }


<?php
//------------------------- FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
if ($sbaporescola) {

?>
    /**
     * 
     */
    function removerEscola(qfaid)
    {
        if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
            return false;
        }

        return new Ajax.Request(window.location.href,
                                   {
                                       method: 'post',
                                       parameters: '&req=removerEscola&sbaid='+ subacaoid +'&qfaid=' + qfaid,
                                       onComplete: function(res)
                                       {
                                           carregarEscolas();
                                       }
                                   });

        return false;
    }


    /**
     * 
     */
    function extratoEscolas(qfaid)
    {
        /*@
        if (itemModificado)
            submeterFrmParSubacao(true,  '<?php echo $_REQUEST['sbaid']; ?>');
        */

        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/extratoescolassubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaid=' + qfaid,
                          'extratoEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function carregarEscolas()
    {
        return new Ajax.Request(window.location.href,
                                {
                                    method: 'post',
                                    parameters: '&req=carregarEscolas&sbaid='+ subacaoid +'&qfaano=' + anoExercicio,
                                    onComplete: function(res)
                                    {
                                        $('escolasSubAcao' + anoExercicio).innerHTML = res.responseText;
                                    }
                                });
    }


    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {
        if (itemModificado && !confirm(('Existem itens de composição modificados que não foram salvos.\n' +
                                        'Deseja continuar sem salvá-los?'))) {
            return false;
        } else {
            itemModificado = false;
        }

        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecer();
                carregarEscolas();
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
//------------------------ !FUNCOES RELATIVAS SOMENTE A CRONOGRAMAS POR ESCOLA
<?
} else {

?>
    /*!@
     * Funções relacionadas a visualização/manipulação de abas para itens
     * da composição da subação
     */
    function selecionarAba(cosano, carregarDados)
    {
        if (itemModificado && !confirm(('Existem itens de composição modificados que não foram salvos.\n' +
                                        'Deseja continuar sem salvá-los?'))) {

            return false;
        } else {
            itemModificado = false;
        }

        var celula;
        var tabela;
        var totalizadores = cosano == '0000';

        // Última selecionada
        $('abaCosano' + anoExercicio).hide();
        $('aba_'      + anoExercicio).style.backgroundPosition            = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.backgroundPosition = '0% 0%';
        $('aba_'      + anoExercicio).firstChild.style.color              = '#333';

        $('abaCosano' + cosano)      .show();
        $('aba_'      + cosano)      .style.backgroundPosition            = '0% -150px';
        $('aba_'      + cosano)      .firstChild.style.backgroundPosition = '100% -150px';
        $('aba_'      + cosano)      .firstChild.style.color              = '#333';

        anoExercicio = cosano;

        if (totalizadores) {
            $('spanAno').innerHTML = 'Totalizadores';
            carregarTotalizadores();
        } else {
            if (carregarDados) {
                carregarItensComposicao();
                carregarBeneficiarios();
                carregarDadosParecer();
            }

            $('spanAno').innerHTML = cosano;
        }

        return false;
    }
<?php

} // if ($sbaporescola) {

?>


    /**
     * 
     */
    function verificarModificaoItensComposicao(el)
    {
        if (!itemModificado)
            itemModificado = el.value != el.defaultValue;
    }


    /**
     * 
     */
    function regraFormaExec()
    {
        return true;
    }

    function exibirPlanoInterno(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: 'req=prgplanointerno&sbaid='+ subacaoid +'&prgplanointerno=' + value,
                             onComplete: function(e)
                             {
                                 $('prgplanointerno').value = e.responseText;
                             }
                         });
    }

    //---------------------------------------------------------------- POP-UPS
    /**
     * 
     */
    function popupSelecionarEscolas(qfaano)
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/cadastrarescolassubacao&acao=A&tipo=cadastrarEscolas&sbaid=<?php echo $_REQUEST['sbaid']; ?>&qfaano=' + qfaano,
                          'popupSelecionarEscolas',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
        return false;
    }

    /**
     * 
     */
    function popupEditarSubacao()
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/popupeditarsubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>',
                          'editarSubacao',
                          'height=700,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    /**
     * 
     */
    function popupSelecionarEscolasItemComposicao(input, cosid)
    {
        return windowOpen('/brasilpro/brasilpro.php?modulo=principal/escolasescolhasubacao&acao=A&sbaid=<?php echo $_REQUEST['sbaid']; ?>&cosid='+cosid+'&qfaano='+anoExercicio,
                          'escolherEscolasItemComposicao',
                          'height=400,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
    }


    /**
     * 
     */
    function calcularValorTotal(cosid)
    {
        var vlrtotal = string2float($F('cosqtd'    + cosid)) *
                       string2float($F('cosvlruni' + cosid)) ;

        $('cosvlrtotal' + cosid).innerHTML = float2string(vlrtotal);
    }

    /**
     * 
     */
    function string2float(str)
    {
        if (str.indexOf(',') == -1)
            return parseFloat(str);
        else
            return parseFloat(str.replace(/\./g, '').replace(/,/g, '.'));
    }

    /**
     * 
     */
    function float2string(num)
    {
        return mascaraglobal('###.###.###.###.###,##', (parseFloat(num).toFixed(2)) + '');
    }


    /**
     * @private
     */
    //this._closeWindows = false;

<?php
if (!CTE_PRM_EDITAR_SUBACAO) {

?>
    function submeterFrmParSubacao(submit)
    {
	    window.opener.location.reload();
        window.opener.focus();
        window.close();
    }

    Form.disable('frmParSubacao');

    $('frmParSubacao').getInputs('button').each(function(el)
                                                {
                                                    if (el.enable)
                                                        el.enable();

                                                    if (el)
                                                        el.removeAttribute('disabled');
                                                });
    $('BtnEditarSubacao').setAttribute('disabled', 'disabled');
<?php
} else {
?>

    var submeter = false;
    
    function submeterFrmParSubacao(submit,vlrProximaSubacao)
    {
        $('anoExercicio').value = anoExercicio;
        if (vlrProximaSubacao == '-1' && permitirCadastroSubacao) {
            if (confirm('Deseja realmente excluir a subação?')) {
                $('vlrProximaSubacao').value = vlrProximaSubacao;
                $('frmParSubacao').submit();
                return true;
            } else {
                return false;
            }
        }

        if (submit) {
            if (Number($F('sptinicio_' + anoExercicio)) > Number($F('sptfim_' + anoExercicio))) {
                alert('O mês fim para o campo Cronograma Físico deve ser maior ou igual o mês inicial.');
                return false;
            }

            var errors = new Array();

            if ($('sbacategoria') && $F('sbacategoria') == 0)
                errors.push('Categoria de Despesa');

            if ($('sbadsc')       && $F('sbadsc') == '')
                errors.push('Descrição');

            if ($('sbastgmpl')    && $F('sbastgmpl') == '')
                errors.push('Estratégia de Implementação');

            if ($('prgid')        && $F('prgid') == 0)
                errors.push('Programa');

            if ($('undid')        && $F('undid') == 0)
                errors.push('Unidade de Medida');

            if ($('frmid')        && $F('frmid') == 0)
                errors.push('Forma de Execução');

            //                                                              */
            if (errors.length > 0) {
                if (errors.length == 1)
                    alert('O campo ' + errors.join('') + ' é obrigatório!');
                else
                    alert('Os campos abaixo são obrigatórios:\n - '
                         +errors.join('\n - ')
                         +'\n\nPor favor corrija para continua!');

                return false;
            } else {
<?php
if (in_array($estadoDocumento['esdid'], array(30,31,26))) {
?>
                if (total <= 0) {
                    alert('Em fase de análise financeira, a sub-ação deve conter Escolas, Itens de Composição e Beneficiários viculados.');
                    return false;
                }
<?php
} // if ($estadoDocumento['esdid'] == 59)@1823
?>

                var submeter = true;
                var inputs   = $('frmParSubacao').getInputs('text');

                for (var i = 0; i < inputs.length; i++) {
                    if (/cosvlruni/.test(inputs[i].getAttribute('name')) ||
                        /cosqtd/   .test(inputs[i].getAttribute('name'))) {
                        if (string2float(inputs[i].value) <= 0) {
                            inputs[i].setStyle({backgroundColor: '#ffd'});
                            submeter = false;
                        } else {
                            inputs[i].setStyle({backgroundColor: '#fff'});
                        }
                    }
                }

                if (submeter) {
                    $('frmParSubacao').submit();
                    return true;
                } else {
                    alert('O dados para Quantidade e Valor Unitário dos Itens de Composição devem ser preenchidos.');
                    return false;
                }
            }
        } else {
                window.opener.location.reload();
                window.opener.focus();
                window.close();
        }
    }

<?php

} // if (!CTE_PRM_EDITAR_SUBACAO) {

if (trim($_REQUEST['sbaid']) != '')
    echo '        selecionarAba(anoExercicio, true);';
else {
    echo '        $("alertaPermitirCadastroItensSubacao").show();
        $("tabAbasSelecaoCosano").hide();
        $("btnExcluir").setAttribute("disabled", "disabled");';
}

?>

    $('loader-container').hide();

<?php
if (in_array($estadoDocumento['esdid'], array(30,31,26))) {
?>
    new Ajax.PeriodicalUpdater('loader-container', window.location.href,
                               {
                                   frequency: 5,
                                   decay: 2,
                                   parameters: 'req=verificarInsercaoDadosSubacao&sbaporescola=<?php echo $sbaporescola ? 't' : 'f'; ?>',
                                   method: 'post',
                                   evalScripts:true
                               });
<?php
} // if ($estadoDocumento['esdid'] == 59)@1823

echo '</script>';

die();
?>