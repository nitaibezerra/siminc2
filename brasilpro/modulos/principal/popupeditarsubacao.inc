<?php
//
// $Id$
//


cte_verificaSessao();



define('CTE_PRM_EDITAR_SUBACAO', cte_podeEditarParecer($_SESSION['inuid']));
define('CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);


if ($_REQUEST['req'] == 'prgplanointerno') {
    echo $db->pegaUm('SELECT
                          prgplanointerno
                      FROM
                          cte.programa
                      WHERE
                          prgid = ' . (integer) $_REQUEST['prgplanointerno']);
    die();
}


//!@------------------------------------------------------------- SALVAR DADOS
if ($_REQUEST['sbaid'] && $_REQUEST['formulario'] && CTE_PRM_EDITAR_SUBACAO) {
    //!@--------------------------------------------------------------- UPDATE
    $sbaporescola = $db->pegaUm('SELECT
                                    sbaporescola
                                 FROM
                                    cte.subacaoindicador
                                 WHERE sbaid = ' . $_REQUEST['sbaid']);
    $sql = '
    UPDATE cte.subacaoindicador SET
        aciid        = %d,
        undid        = %d,
        frmid        = %s,
        sbadsc       = \'%s\',
        sbastgmpl    = \'%s\',
        sbaprm       = %d,
        sbapcr       = \'%s\',
        sbadata      = CURRENT_TIMESTAMP,
        usucpf       = \'%s\',
        foaid        = %s,
        prgid        = %s,
        sbaporescola = %s,
        ppsid        = %s,
        sbacategoria = %d
    WHERE
        sbaid        = %d';


    $sql = sprintf($sql, (integer) $_REQUEST['aciid'       ],
                         (integer) $_REQUEST['undid'       ],
                         (string)  $_REQUEST['frmid'       ] == 0 ? 'null' : (integer) $_REQUEST['frmid'],
                         (string)  $_REQUEST['sbadsc'      ],
                         (string)  $_REQUEST['sbastgmpl'   ],
                         (integer) $_REQUEST['sbaprm'      ],
                         (string)  $_REQUEST['sbapcr'      ],
                         (string)  $_SESSION['usucpf'      ],
                         (integer) $_REQUEST['foaid'       ] == 0 ? 'null' : (integer) $_REQUEST['foaid'],
                         (integer) $_REQUEST['prgid'       ] == 0 ? 'null' : (integer) $_REQUEST['prgid'],
                         (string)  $_REQUEST['sbaporescola'],
                         (integer) $_REQUEST['ppsid'       ] == 0 ? 'null' : (integer) $_REQUEST['ppsid'],
                         (integer) $_REQUEST['sbacategoria'],
                         (integer) $_REQUEST['sbaid'       ]);

    //echo $sql; die();
    $db->executar($sql);

    $sql = '
    SELECT
        il.labid
    FROM
        cte.laboratorio l
    INNER JOIN
        cte.itenslaboratorio il ON l.labid = il.labid
    WHERE
        l.undid = ' . $_REQUEST['undid'];

    if ((integer) $db->pegaUm($sql) > 0) {
        $sql = '
        DELETE FROM
            cte.composicaosubacao
        WHERE
            sbaid = ' . (integer) $_REQUEST['sbaid'];

        $db->executar($sql);
    }

    if ($_REQUEST['sbaporescola'] == 'false') {
        $sql = '
        DELETE FROM
            cte.escolacomposicaosubacao
        WHERE
            cosid IN (SELECT
                        cosid
                      FROM
                        cte.composicaosubacao
                      WHERE
                        sbaid = ' . (integer) $_REQUEST['sbaid'] . ')';

        $db->executar($sql);

        $sql = '
        DELETE FROM
            cte.qtdfisicoano
        WHERE
            sbaid = ' . (integer) $_REQUEST['sbaid'];

        $db->executar($sql);
    }

    $db->commit();
    echo "<script>window.opener.location.reload();window.close();</script>";
}


/**
 * 
 */
function sql2html($coluna) {
    if (strtolower($coluna) == 't')
        return true;

    if (strtolower($coluna) == 'f')
        return false;

    if (strtolower($coluna) == 'null' || $coluna == null)
        return '';

    return $coluna;
}





$podeExcluirSubacao = cte_podeEditarSubacao($_SESSION["inuid"]);
$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$capturaTipo        = cte_pegarItrid($_SESSION['inuid']) == INSTRUMENTO_DIAGNOSTICO_ESTADUAL ? "E" : "M";





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
    s.sba1ano,
    s.sba2ano,
    s.sba3ano,
    s.sba4ano,
    s.sbaunt,
    s.sbauntdsc,
    s.sba1ini,
    s.sba1fim,
    s.sba2ini,
    s.sba2fim,
    s.sba3ini,
    s.sba3fim,
    s.sba4ini,
    s.sba4fim,
    s.sbadata,
    s.usucpf,
    s.sbaparecer,
    s.psuid,
    s.ssuid,
    s.sba0ano,
    s.sba0ini,
    s.sba0fim,
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

$aci = (array) $db->pegaLinha( $sql );
extract($aci);

$aciid           = trim($aciid) != '' ? $aciid : (integer) $_REQUEST['aciid'];
$indqtdporescola = $db->pegaUm('SELECT indqtdporescola FROM cte.indicador WHERE indid = ' . $indid) == 't';


//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
$select_formaAtendimento = $db->monta_combo('foaid',
                                            'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
                                            'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);

$select_programa         = $db->monta_combo('prgid',
                                            'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
                                            'S', 'Selecione', 'exibirPlanoInterno', '', '', '', 'S', 'prgid', true);

$select_unidadeMedida    = $db->monta_combo('undid',
                                            'SELECT DISTINCT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni INNER JOIN cte.indicadorunidademedida iun ON uni.undid = iun.undid WHERE undtipo = \'' . $capturaTipo . '\' and indid = ' . $_SESSION['indid'] . ' ORDER BY descricao',
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

$prgplanointerno = $db->pegaUm('SELECT
                                    prgplanointerno
                                FROM
                                    cte.subacaoindicador sba
                                INNER JOIN
                                    cte.programa prg ON prg.prgid = sba.prgid
                                WHERE
                                    sbaid = ' . (integer) $sbaid);




if($sbaporescola == 't'){
	$cronoescolglbal = 'true';	
}else{
	$cronoescolglbal = 'false';	
}

?>
<html>
  <head>
    <meta http-equiv="Cache-Control" content="no-cache">
    <meta http-equiv="Pragma" content="no-cache">
    <meta http-equiv="Connection" content="Keep-Alive">
    <meta http-equiv="Expires" content="-1">
    <title><?php echo $titulo ?></title>

    <script type="text/javascript" src="/includes/funcoes.js"></script>
    <script type="text/javascript" src="/includes/prototype.js"></script>

    <link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>

    <style type="text/css" media="screen">
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
  <body onunload="window.opener.location.reload();">
    <div id="loader-container">
      <div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
    </div>
    <div id="container">
      <?php cte_montaTitulo( $titulo_modulo, '' ); ?>
      <form method="post" id="frmParSubacao" action="<?php echo $_SERVER['REQUEST_URI']; ?>" onsubmit="return validarFrmSubacao();">
        <input type="hidden" name="formulario" value="1"/>
        <input type="hidden" name="ppsid" id="ppsid" value="<?php echo $ppsid ?>"/>
        <input type="hidden" name="aciid" id="aciid" value="<?php echo $aciid ?>"/>
        <input type="hidden" name="sbaid" id="sbaid" value="<?php echo $sbaid ?>"/>
        <input type="hidden" name="cronograma" id="cronograma" value="<?php echo $cronoescolglbal ?>"/>
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

            <tr style="background-color: #cccccc;">
              <td align="left" colspan="<?php echo sizeof($_cosano) + 2; ?>">
                <strong>Dados da Subação</strong>
              </td>
            </tr>

            <tr id="tr_categoriaDespesa">
              <td align="right" class="SubTituloDireita">Categoria de Despesa:</td>
              <td>
                <select name="sbacategoria" class="CampoEstilo" id="sbacategoria">
                  <option value="">Escolha...</option>
                  <option value="3" <?php echo $sbacategoria == 3 ? 'selected="selected"' : '' ?>>Custeio</option>
                  <option value="4" <?php echo $sbacategoria == 4 ? 'selected="selected"' : '' ?>>Capital</option>
                </select>
              </td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Forma de atendimento:</td>
              <td><?php echo $select_formaAtendimento; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Descrição da Subação:</td>
              <td><?php echo campo_textarea('sbadsc', "S", 'S', '', 70, 3, 255 ); ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Estratégia de Implementação:</td>
              <td><?php echo campo_textarea('sbastgmpl', 'S', 'S', '', 70, 3, 2000 ); ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Programa:</td>
              <td><?php echo $select_programa; ?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Plano Interno:</td>
              <td><?php echo campo_texto('prgplanointerno', "N", "N", "Plano Interno", 51, 100, "", "", 'left', '', 0, 'id="prgplanointerno"');?></td>
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
              <td align="right" class="SubTituloDireita">Instituição Parceira (se houver):</td>
              <td><?php echo campo_texto('sbapcr', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="sbapcr"');?></td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">Cronograma:</td>
              <td>
                <input id="sbaporescola0" type="radio" name="sbaporescola" value="false" <?php echo !$sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola0">Global</label>
                <input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  <?php echo  $sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola1">Por Escola</label>
<?php
if ($indqtdporescola) {
    echo '<script type="text/javascript">$("sbaporescola1").checked = "checked";$("sbaporescola1").disable();$("sbaporescola0").disable();</script>';
}
?>
              </td>
            </tr>

            <tr>
              <td align="right" class="SubTituloDireita">&nbsp;</td>
              <td bgcolor="#dddddd">
<?php
                if (CTE_PRM_EDITAR_SUBACAO)
                    echo '
                <input id="BtnGravar" type="submit" name="BtnGravar" value="Gravar" />' , "\n";
?>
              </td>
            </tr>
          </tbody>
        </table>
      </form>
    </div>

    <script type="text/javascript">
    <!--
        this._closeWindows                   = false;
        this.opener.popupEditarSubacaoAberto = true;

    if ($('frmid') && ($('frmid').value == 14 ||
                       $('frmid').value == 15 ||
                       $('frmid').value == 16 ||
                       $('frmid').value == 17))
    {
        //$('frmid').disable();
    }

    /**
     * 
     */
    function regraFormaExec(vl)
    {
    }


    /**
     * 
     */
    function exibirPlanoInterno(value)
    {
        new Ajax.Request(window.location.href,
                         {
                             parameters: 'req=prgplanointerno&prgplanointerno=' + value,
                             onComplete: function(e)
                             {
                                 $('prgplanointerno').value = e.responseText;
                             }
                         });
    }


    /**
     * 
     */
    function validarFrmSubacao()
    {
        var errors = new Array();

        if ($F('sbacategoria') == 0)
            errors.push('Categoria de Despesa');

        if ($F('sbadsc') == '')
            errors.push('Descrição');

        if ($F('sbastgmpl') == '')
            errors.push('Estratégia de Implementação');

        if ($F('prgid') == 0)
            errors.push('Programa');

        if ($F('undid') == 0)
            errors.push('Unidade de Medida');

        if ($F('frmid') == 0)
            errors.push('Forma de Execução');
		
		cronograma 		= document.getElementById('cronograma').value;
		sbaporescola0 	= document.getElementById('sbaporescola0').checked;

		if((sbaporescola0 == true) && (cronograma == "true") ){ // Se mudou de escola para geral no gronograma gera msn
			if (!confirm('Atenção! Todos os valores vinculados as escolas serão excluidos. Deseja Continuar?')) {
            	return false;
        	}
		}
		

        if (errors.length > 0) {
            if (errors.length == 1)
                alert('O campo ' + errors.join('') + ' é obrigatório!');
            else
                alert('Os campos abaixo são obrigatórios:\n - '
                     +errors.join('\n - ')
                     +'\n\nPor favor corrija para continua!');

            return false;
        } else {
            $('sbaporescola0').removeAttribute('disabled');
            $('sbaporescola1').removeAttribute('disabled');
            return true;
        }
    }

    $('loader-container').hide();
      -->
    </script>
  </body>
</html>

