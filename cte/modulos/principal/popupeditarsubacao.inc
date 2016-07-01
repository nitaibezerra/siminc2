<?php
cte_verificaSessao();

$docid              = cte_pegarDocid($_SESSION['inuid']);
$estadoDocumento    = wf_pegarEstadoAtual($docid);
$editavel           = true;
ob_clean();

define('CTE_NOVA_SUBACAO', trim($_REQUEST['sbaid']) == '');

if (!$_REQUEST['anoExercicio'])
    $_REQUEST['anoExercicio'] = $_REQUEST['parsubacaoanocrt'] ? $_REQUEST['parsubacaoanocrt'] : date('Y');

if (!CTE_NOVA_SUBACAO) {
    $sql = '
    SELECT
        count(spt.ssuid)
    FROM
        cte.subacaoparecertecnico spt
    INNER JOIN
        cte.statussubacao ssu ON ssu.ssuid = spt.ssuid
    WHERE
        spt.sbaid = ' . $_REQUEST['sbaid'] . '
        AND
        trim(spt.sptparecer) <> \'\'
        AND
        spt.ssuid IS NOT NULL
        AND ssutipostatus = \'E\' 
        AND  ssutipostatus = \'E\'';

    $parecer = (integer) $db->pegaUm($sql) > 0;

    if ($estadoDocumento['esdid'] == CTE_ESTADO_PAR) {
        $editavel = !$parecer;
    }
}

if (cte_possuiPerfil(array(CTE_PERFIL_CONSULTORES))) {
    define('CTE_PRM_EDITAR_SUBACAO', $_REQUEST['bloqueado'] != '1');
} else {
    define('CTE_PRM_EDITAR_SUBACAO', cte_podeEditarParecer($_SESSION['inuid']) || ($db->testa_superuser() ||
                                                                                   cte_possuiPerfil(array(CTE_PERFIL_EQUIPE_MUNICIPAL,
                                                                                                          CTE_PERFIL_EQUIPE_LOCAL,
                                                                                                          CTE_PERFIL_EQUIPE_LOCAL_APROVACAO,
                                                                                                          CTE_PERFIL_EQUIPE_TECNICA,
                                                                                                          CTE_PERFIL_ALTA_GESTAO,
                                                                                                          CTE_PERFIL_EQUIPE_TECNICA_MEC_MUN))) && $_REQUEST['bloqueado'] !=='1' && $editavel);
}

define('CTE_PRM_EXCLUIR_SUBACAO', CTE_PRM_EDITAR_SUBACAO);


//!@------------------------------------------------------------- SALVAR DADOS
if ($_REQUEST['sbaid'] && $_REQUEST['formulario'] && CTE_PRM_EDITAR_SUBACAO) {
    //!@--------------------------------------------------------------- UPDATE
	$sbaporescola = $db->pegaUm('SELECT
                                    sbaporescola
                                 FROM
                                    cte.subacaoindicador
                                 WHERE sbaid = ' . $_REQUEST['sbaid']);
	$programaAtual = $db->pegaUm('SELECT
                                    prgid
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
                         (string)  $_REQUEST['sbaporescola'] == '' ? 'false' : (string)  $_REQUEST['sbaporescola'],
                         (integer) $_REQUEST['ppsid'       ] == 0 ? 'null' : (integer) $_REQUEST['ppsid'],
                         (integer) $_REQUEST['sbacategoria'],
                         (integer) $_REQUEST['sbaid'       ]);

   // echo $sql; die();
    $db->executar($sql);
    
    if($programaAtual != $_REQUEST['prgid'] ){
    	$sql = "UPDATE cte.subacaoparecertecnico SET sptplanointerno = '' WHERE sbaid = ".$_REQUEST['sbaid'];
    	$db->executar($sql);
    }

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
    //echo "<script>window.opener.location.reload();window.close();</script>";

    die();
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
$sql = 'select s.sbaid, s.aciid, s.undid, s.frmid, s.sbadsc, s.sbastgmpl, 
			s.sbaprm, s.sbapcr, s.sbadata, s.usucpf, s.sbaparecer, s.psuid, 
			s.ssuid, s.foaid, s.prgid, s.sbaporescola, s.ppsid, s.sbaordem, 
			s.sbaobjetivo, s.sbatexto, s.sbacategoria 
		from cte.subacaoindicador s 
			left join cte.programa p on p.prgid = s.prgid 
		where s.sbaid = ' . (integer) $_REQUEST['sbaid'];

$sai = array_map('sql2html', (array) $db->pegaLinha($sql));
extract($sai);

$sbaporescola = $sbaporescola == 't';

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
$indqtdporescola = $db->pegaUm('SELECT indqtdporescola FROM cte.proposicaosubacao WHERE ppsid = ' . (integer) $_REQUEST['ppsid']) == 't';


//------------------------------------------------------- CONSTRUÇÃO DE COMBOS
$select_formaAtendimento = $db->monta_combo('foaid',
                                            'select foaid as codigo, foadsc as descricao from cte.formaatendimento order by foadsc',
                                            'S', 'Escolha...', '', '', '', '', 'N', 'foaid', true);

$select_programa         = $db->monta_combo('prgid',
                                            'select prgid as codigo, substr(prgdsc, 1, 100) as descricao from cte.programa order by descricao',
                                            'S', 'Selecione', '', '', '', '350', 'S', 'prgid', true);

if (trim($ppsid) != '') {
    $select_unidadeMedida    = $db->monta_combo('undid',
                                                'SELECT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni INNER JOIN cte.proposicaosubacao pps ON uni.undid = pps.undid WHERE ppsid = ' . $ppsid . ' ORDER BY descricao',
                                                'S', 'Escolha a unidade de medida', '', '','','300','S', 'undid', true);
} else {
    $select_unidadeMedida    = $db->monta_combo('undid',
                                                'SELECT uni.undid as codigo, uni.unddsc as descricao FROM cte.unidademedida uni ORDER BY descricao',
                                                'S', 'Escolha a unidade de medida', '', '','','300','S', 'undid', true);
}

$sql = '
SELECT
    frmid as codigo,
    frmdsc as descricao
FROM
    cte.formaexecucao
WHERE
    frmtipo = \'' . $capturaTipo . '\'
    AND
    frmbrasilpro = false
ORDER BY
    descricao';

$select_formaExecucao = $db->monta_combo('frmid', $sql, 'S', 'Selecione', '', '','','','S', 'frmid', true);

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
  <body onunload="window.opener.location.reload();" >
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
                <input id="sbaporescola0" type="radio" name="sbaporescola" onclick="desabilitar(this.form,0)" value="false" <?php echo !$sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola0">Global</label>
                <input id="sbaporescola1" type="radio" name="sbaporescola" value="true"  <?php echo  $sbaporescola ? 'checked="checked"' : '' ?> /><label for="sbaporescola1">Por Escola</label>
              </td>
            </tr>           
            <tr>
              <td align="right" class="SubTituloDireita">&nbsp;</td>
              <td bgcolor="#dddddd">
              <input id="btngravar" type="submit" name="btngravar" value="Gravar" />
              <input id="btnfechar" type="button" name="btnfechar" value="Fechar" onclick="window.close();" />
              </td>
            </tr>
          </tbody>
        </table>
        <input type="hidden" id="validaCrono" name="validaCrono" value="<?=$perfilValidaCampo ?>">
      </form>
    </div>
<?
$perfilValidaCampo = cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR, CTE_PERFIL_SUPER_USUARIO)) ? "true" : "false";
//echo $perfilValidaCampo;
?>

    <script type="text/javascript">
    <!--
        this._closeWindows                   = false;
        this.opener.popupEditarSubacaoAberto = true;
		
		indice_marcado = 1
		
		function desabilitar(formulario,idradio){
		
			var cronogramaPorEscola    = $('cronograma').value;			
            var sbaporescolaGlobal = $('sbaporescola0').checked;
            var cronogramaPorEscola    = $('validaCrono').value;
            
            if (!$perfilValidaCampo)
            {
		
				if ((sbaporescolaGlobal == true) && (cronogramaPorEscola == "true") ) {
	    			formulario.sbaporescola[indice_marcado].checked = true
	    			formulario.sbaporescola[idradio].blur()
	    		}
	    		
	    		if ((sbaporescolaGlobal == true) && (cronogramaPorEscola == "true") ) {		    	
					alert('O campo cronograma não pode ser alterado!');
					return false;
				}
			}    		   		
    		
		}
		
		
        function validarFrmSubacao()
        {
            Form.enable('frmParSubacao');
            
            var sbaporescola0 = $('sbaporescola0').checked;

            var errors    = new Array();

            var sbadsc    = $('sbadsc');
            var sbastgmpl = $('sbastgmpl');
            var prgid     = $('prgid');
            var undid     = $('undid');
            var frmid     = $('frmid');

            if (sbadsc    && sbadsc.value == '')
                errors.push('Descrição');

            if (sbastgmpl && sbastgmpl.value == '')
                errors.push('Estratégia de Implementação');

            if (prgid     && (prgid.value == '0' || prgid.value == ''))
                errors.push('Programa');

            if (undid     && (undid.value == '0' || undid.value == ''))
                errors.push('Unidade de Medida');

            if (frmid     && (frmid.value == '0' || frmid.value == ''))
                errors.push('Forma de Execução');

            if (errors.length > 0) {
                if (errors.length == 1)
                    alert('O campo ' + errors.join('') + ' é obrigatório!');
                else
                    alert('Os campos abaixo são obrigatórios:\n - '
                         +errors.join('\n - ')
                         +'\n\nPor favor corrija para continuar!');

                return false;
            } else {
                return true;
            }
        }

<?php
    if( $capturaTipo == 'M' && !$db->testa_superuser() && !cte_possuiPerfil(array(CTE_PERFIL_ADMINISTRADOR)) ){
        echo 'Form.disable("frmParSubacao");';
    }

    if ($indqtdporescola) {
        echo '
        $("sbaporescola1").checked = "checked";';
    } else {
        echo '$("sbaporescola1").removeAttribute("disabled");'
            ,'$("sbaporescola0").removeAttribute("disabled");';
    }

    if (CTE_PRM_EDITAR_SUBACAO)
        echo '$(\'btngravar\').removeAttribute("disabled");';
?>
    $('btnfechar').removeAttribute("disabled");
    $('loader-container').hide();
      -->
    </script>
  </body>
</html>

